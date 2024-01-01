
#include "stdafx.h"
#include "common.h"
#include "track.h"
#include "crt.h"


PATCH_CODE(0x401000, 0x401000, InitTrack);
extern "C" int InitTrack(
    TRACK *track,
    LPDIRECTSOUND dsound,
    LPCWAVEFORMATEX wfxIn,
    LPCWAVEFORMATEX wfxOut,
    const TRACK_PARAMS *params,
    void *convBuf
)
{
    LPDIRECTSOUNDBUFFER dsBuffer;

    // track should not have a DS buffer already allocated
    if (track->dsBuffer)
        return 2;

    // validate formats
    // output must be PCM
    bool valid = wfxOut->wFormatTag == WAVE_FORMAT_PCM;
    // channel counds must match
    if (wfxOut->wFormatTag == WAVE_FORMAT_PCM)
        valid = wfxOut->nChannels == wfxIn->nChannels;
    if (!valid)
        return 4;
    // if input is PCM, sample sizes must mach
    if (wfxIn->wFormatTag == WAVE_FORMAT_PCM)
        valid = wfxOut->wBitsPerSample == wfxIn->wBitsPerSample;
    if (!valid)
        return 4;
    // sample rates must match
    valid = wfxOut->nSamplesPerSec == wfxIn->nSamplesPerSec;
    if (!valid)
        return 4;
    // if input is ADPCM, input/output must have 4/16 bits per sample, resp.
    if (wfxIn->wFormatTag == WAVE_FORMAT_ADPCM)
        valid = wfxIn->wBitsPerSample == 4 && wfxOut->wBitsPerSample == 16;
    if (!valid)
        return 4;
    // also validate block align and bytes per sec
    if (wfxIn->nBlockAlign != wfxIn->nChannels * wfxIn->wBitsPerSample / 8)
        return 4;
    if (wfxOut->nBlockAlign != wfxOut->nChannels * wfxOut->wBitsPerSample / 8)
        return 4;
    if (wfxIn->nAvgBytesPerSec != wfxIn->nBlockAlign * wfxIn->nSamplesPerSec)
        return 4;
    if (wfxOut->nAvgBytesPerSec != wfxOut->nBlockAlign * wfxOut->nSamplesPerSec)
        return 4;

    // validate timing params
    if (params->field_8 + params->msConvBufLen >= params->msSoundBufLen)
        return 5;

    // start track out with default state
    *track = trackDefault;

    // build up DS buffer desc
    DSBUFFERDESC1 dsBufDesc = { 0 };
    dsBufDesc.dwSize = sizeof dsBufDesc;
    dsBufDesc.dwBufferBytes = DWORD(
        double(params->msSoundBufLen) *
        wfxOut->nAvgBytesPerSec *
        BDBL(0x45c224, 1.0 / 1000.0)
    );
    dsBufDesc.lpwfxFormat = const_cast<LPWAVEFORMATEX>(wfxOut);
    dsBufDesc.dwFlags =
        DSBCAPS_GETCURRENTPOSITION2 |
        DSBCAPS_CTRLVOLUME |
        DSBCAPS_CTRLPAN |
        DSBCAPS_CTRLFREQUENCY;

    // create DS buffer
    HRESULT hr = dsound->CreateSoundBuffer(
        reinterpret_cast<const DSBUFFERDESC *>(&dsBufDesc),
        &dsBuffer,
        nullptr
    );
    if (hr != S_OK)
        dsBuffer = nullptr;
    track->dsBuffer = dsBuffer;
    if (!dsBuffer)
        return 3;

    // precompute number of bytes per millisecond (for timing param conversion)
    double msBytes = wfxOut->nAvgBytesPerSec * BDBL(0x45c224, 1.0 / 1000.0);

    // setup conversion buffer
    if (convBuf)
    {
        // caller provided a buffer to borrow
        track->convBuf = convBuf;
        track->convBufOwned = FALSE;
    }
    else
    {
        // need to allocate a buffer
        track->convBuf = malloc_bugs(
            size_t(params->msConvBufLen * msBytes + 0.5)
        );
        track->convBufOwned = TRUE;
    }

    // copy wave formats
    track->wfxIn = *wfxIn;
    track->wfxOut = *wfxOut;

    // convert timing params
    track->soundBufSize = size_t(params->msSoundBufLen * msBytes + 0.5);
    track->convBufSize = size_t(params->msConvBufLen * msBytes + 0.5);
    track->field_64 = size_t(params->field_8 * msBytes + 0.5);
    track->field_68 = size_t(params->field_C * msBytes + 0.5);

    return 0;
}

PATCH_CODE(0x401330, 0x401330, FiniTrack);
extern "C" void FiniTrack(TRACK *track)
{
    ResetTrack(track);

    track->dsBuffer->Release();

    // if the track owns the conversion buffer, free it now
    if (track->convBufOwned)
        free_bugs(track->convBuf);

    // clear track object to default state
    *track = trackDefault;
}

PATCH_CODE(0x401490, 0x401490, ResetTrack);
extern "C" void ResetTrack(TRACK *track)
{
    if (!track->dsBuffer)
        return;

    StopTrack(track);
    track->dsBuffer->SetCurrentPosition(0);
    track->trackInSize = 0;
    track->fd = 0; // should be set to -1 since 0 is technically a valid fd
    track->readBytes = 0;
    track->field_38 = 0;
    track->field_3C = 0;
    track->field_40 = 0;
    track->field_44 = 0;
    track->trackOutSize = 0;
    track->field_4C = 0;
    track->field_50 = 0;
    track->flag_54 = FALSE;
    track->flag_70 = FALSE;
    track->field_78 = 0;
}

PATCH_CODE(0x4018d0, 0x4018d0, PlayTrack);
extern "C" void PlayTrack(TRACK *track)
{
    // do nothing if already playing, or if nothing to play
    if (track->playing || !track->trackInSize)
        return;

    // unknown additional condition check
    if (track->flag_70)
        return;

    track->dsBuffer->Play(0, 0, DSBPLAY_LOOPING);
    track->playing = TRUE;
}

PATCH_CODE(0x401910, 0x401910, StopTrack);
extern "C" void StopTrack(TRACK *track)
{
    // do nothing if not playing
    if (!track->playing)
        return;

    track->dsBuffer->Stop();
    track->playing = FALSE;
}

PATCH_CODE(0x401af0, 0x401af0, ConvertTrackAudio);
extern "C" void ConvertTrackAudio(
    TRACK *track,
    const void *src,
    void *dst,
    size_t count
)
{
    // if input/output formats are the same, just straight copy the audio
    if (track->wfxIn.wBitsPerSample == track->wfxOut.wBitsPerSample &&
        track->wfxIn.nSamplesPerSec == track->wfxOut.nSamplesPerSec &&
        track->wfxIn.nChannels == track->wfxOut.nChannels &&
        track->wfxIn.wFormatTag == track->wfxOut.wFormatTag)
    {
        memcpy(dst, src, track->wfxIn.nBlockAlign * count);
        return;
    }

    // if input isn't ADPCM, or output isn't PCM, don't convert
    if (track->wfxIn.wFormatTag != WAVE_FORMAT_ADPCM ||
        track->wfxOut.wFormatTag != WAVE_FORMAT_PCM)
    {
        return;
    }

    if (track->wfxIn.nChannels == 2 && track->wfxOut.nChannels == 2)
    {
        // input stereo, output stereo

        // don't support sample rate conversion
        if (track->wfxIn.nSamplesPerSec != track->wfxOut.nSamplesPerSec)
            return;
        // input must be 4bps, output 16bs
        if (track->wfxIn.wBitsPerSample != 4 ||
            track->wfxOut.wBitsPerSample != 16)
        {
            return;
        }

        CvtStereoAdpcm(track, src, dst, count);
    }
    else if (track->wfxIn.nChannels == 1 && track->wfxOut.nChannels == 2)
    {
        // input mono, output stero

        // don't support sample rate conversion
        if (track->wfxIn.nSamplesPerSec != track->wfxOut.nSamplesPerSec)
            return;
        // input must be 4bps, output 16bs
        if (track->wfxIn.wBitsPerSample != 4 ||
            track->wfxOut.wBitsPerSample != 16)
        {
            return;
        }

        CvtMonoAdpcm(track, src, dst, count);
    }
}

extern "C" void CvtStereoAdpcm(
    TRACK *track,
    const void *src,
    void *dst,
    size_t count
)
{
    // use old function for now
    auto fn = reinterpret_cast<
        void (*)(TRACK *, const void *, void *, size_t)
    >(0x401be0);
    fn(track, src, dst, count);
}

extern "C" void CvtMonoAdpcm(
    TRACK *track,
    const void *src,
    void *dst,
    size_t count
)
{
    // use old function for now
    auto fn = reinterpret_cast<
        void (*)(TRACK *, const void *, void *, size_t)
    >(0x401e10);
    fn(track, src, dst, count);
}
