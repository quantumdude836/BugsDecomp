
#include "stdafx.h"
#include "common.h"
#include "track.h"
#include "crt.h"
#include "misc.h"


// "default" state for an uninitialized track
#define trackDefault (*(const TRACK *)0x45c000)

// LUT for "state" adjustment based on input sample
#define adpcmStateAdj (*(const int (*)[16])0x45c080)

// LUT for base value based on current "state"
#define adpcmStateDecode (*(const int (*)[89])0x45c0c0)

// byte size of audio segments in the speech files; indexed by language, then by
// segment number (index 0 = segment 2?)
#define speechPartSizes (*(const size_t (*)[6][64])0x45c340)


TRACK_ERROR InitTrack(
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
        return TRACK_ERR_ALREADY_INIT;

    // validate formats
    // output must be PCM
    bool valid = wfxOut->wFormatTag == WAVE_FORMAT_PCM;
    // channel counts must match (should `wfxOut` be `wfxIn`? otherwise mono
    // ADPCM won't be allowed, despite supporting decoding)
    if (wfxOut->wFormatTag == WAVE_FORMAT_PCM)
        valid = wfxOut->nChannels == wfxIn->nChannels;
    if (!valid)
        return TRACK_ERR_AUDIO_FORMAT;
    // if input is PCM, sample sizes must mach
    if (wfxIn->wFormatTag == WAVE_FORMAT_PCM)
        valid = wfxOut->wBitsPerSample == wfxIn->wBitsPerSample;
    if (!valid)
        return TRACK_ERR_AUDIO_FORMAT;
    // sample rates must match
    valid = wfxOut->nSamplesPerSec == wfxIn->nSamplesPerSec;
    if (!valid)
        return TRACK_ERR_AUDIO_FORMAT;
    // if input is ADPCM, input/output must have 4/16 bits per sample, resp.
    if (wfxIn->wFormatTag == WAVE_FORMAT_ADPCM)
        valid = wfxIn->wBitsPerSample == 4 && wfxOut->wBitsPerSample == 16;
    if (!valid)
        return TRACK_ERR_AUDIO_FORMAT;
    // also validate block align and bytes per sec
    if (wfxIn->nBlockAlign != wfxIn->nChannels * wfxIn->wBitsPerSample / 8)
        return TRACK_ERR_AUDIO_FORMAT;
    if (wfxOut->nBlockAlign != wfxOut->nChannels * wfxOut->wBitsPerSample / 8)
        return TRACK_ERR_AUDIO_FORMAT;
    if (wfxIn->nAvgBytesPerSec != wfxIn->nBlockAlign * wfxIn->nSamplesPerSec)
        return TRACK_ERR_AUDIO_FORMAT;
    if (wfxOut->nAvgBytesPerSec != wfxOut->nBlockAlign * wfxOut->nSamplesPerSec)
        return TRACK_ERR_AUDIO_FORMAT;

    // validate timing params
    if (params->field_8 + params->msConvBufLen >= params->msSoundBufLen)
        return TRACK_ERR_TIMING_PARAMS;

    // start track out with default state
    *track = trackDefault;

    // build up DS buffer desc
    DSBUFFERDESC1 dsBufDesc = { 0 };
    dsBufDesc.dwSize = sizeof dsBufDesc;
    dsBufDesc.dwBufferBytes = (DWORD)(
        (double)(params->msSoundBufLen) *
        wfxOut->nAvgBytesPerSec *
        BFLT(0x45c224, 1.0 / 1000.0)
    );
    dsBufDesc.lpwfxFormat = (LPWAVEFORMATEX)wfxOut;
    dsBufDesc.dwFlags =
        DSBCAPS_GETCURRENTPOSITION2 |
        DSBCAPS_CTRLVOLUME |
        DSBCAPS_CTRLPAN |
        DSBCAPS_CTRLFREQUENCY;

    // create DS buffer
    HRESULT hr = IDirectSound_CreateSoundBuffer(
        dsound,
        (const DSBUFFERDESC *)&dsBufDesc,
        &dsBuffer,
        NULL
    );
    if (hr != S_OK)
        dsBuffer = NULL;
    track->dsBuffer = dsBuffer;
    if (!dsBuffer)
        return TRACK_ERR_DSBUFFER_CREATE;

    // precompute number of bytes per millisecond (for timing param conversion)
    double msBytes = wfxOut->nAvgBytesPerSec * BFLT(0x45c224, 1.0 / 1000.0);

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
            (size_t)(params->msConvBufLen * msBytes + 0.5)
        );
        track->convBufOwned = TRUE;
    }

    // copy wave formats
    track->wfxIn = *wfxIn;
    track->wfxOut = *wfxOut;

    // convert timing params
    track->soundBufSize = (size_t)(params->msSoundBufLen * msBytes + 0.5);
    track->convBufSize = (size_t)(params->msConvBufLen * msBytes + 0.5);
    track->field_64 = (size_t)(params->field_8 * msBytes + 0.5);
    track->field_68 = (size_t)(params->field_C * msBytes + 0.5);

    return TRACK_OK;
}

void FiniTrack(TRACK *track)
{
    ResetTrack(track);

    IDirectSoundBuffer_Release(track->dsBuffer);

    // if the track owns the conversion buffer, free it now
    if (track->convBufOwned)
        free_bugs(track->convBuf);

    // clear track object to default state
    *track = trackDefault;
}

TRACK_ERROR SetTrackSource(
    TRACK *track,
    size_t trackInSize,
    int fd,
    BOOL loop
)
{
    if (!track->dsBuffer)
        return TRACK_ERR_NOT_INIT;

    // if track already has a source, reset the track to clear it
    if (track->trackInSize)
        ResetTrack(track);

    track->fd = fd;
    track->trackInSize = trackInSize;
    track->loop = loop;
    // compute size of converted audio
    track->trackOutSize = (size_t)(
        (float)track->wfxOut.nAvgBytesPerSec *
        trackInSize /
        track->wfxIn.nAvgBytesPerSec
    );

    IDirectSoundBuffer_SetCurrentPosition(track->dsBuffer, 0);

    // fill the DS buffer with initial audio
    RefillTrackBuffer(track, -1, -1);

    return TRACK_OK;
}

void ResetTrack(TRACK *track)
{
    if (!track->dsBuffer)
        return;

    StopTrack(track);
    IDirectSoundBuffer_SetCurrentPosition(track->dsBuffer, 0);
    track->trackInSize = 0;
    track->fd = 0; // should be set to -1 since 0 is technically a valid fd
    track->readBytes = 0;
    track->field_38 = 0;
    track->field_3C = 0;
    track->prevPlayPos = 0;
    track->bufLoopCount = 0;
    track->trackOutSize = 0;
    memset(&track->adpcmState, 0, sizeof track->adpcmState);
    track->loop = FALSE;
    track->trackDone = FALSE;
    track->readDone = FALSE;
}

BOOL RefillTrackBuffer(TRACK *track, DWORD samples, int arg_8)
{
    // use old function for now
    return ((BOOL (*)(TRACK *, DWORD, int))0x4014f0)(track, samples, arg_8);
}

void PlayTrack(TRACK *track)
{
    // do nothing if already playing, or if nothing to play, or if done
    if (track->playing || !track->trackInSize || track->trackDone)
        return;

    IDirectSoundBuffer_Play(track->dsBuffer, 0, 0, DSBPLAY_LOOPING);
    track->playing = TRUE;
}

void StopTrack(TRACK *track)
{
    // do nothing if not playing
    if (!track->playing)
        return;

    IDirectSoundBuffer_Stop(track->dsBuffer);
    track->playing = FALSE;
}

BOOL CheckTrackDone(TRACK *track)
{
    DWORD curPlayPos, curWritePos;
    BOOL trackDone;

    IDirectSoundBuffer_GetCurrentPosition(
        track->dsBuffer,
        &curPlayPos,
        &curWritePos
    );
    // if track is looped, it will never be done
    if (track->loop)
        trackDone = FALSE;
    else
    {
        // compute total number of bytes played
        DWORD trackPlayedBytes =
            track->bufLoopCount * track->soundBufSize + curPlayPos;
        // check if it's past the requested size to play
        trackDone = trackPlayedBytes >= track->trackOutSize;
    }

    // if the track just hit the end, stop it
    if (trackDone && !track->trackDone)
        StopTrack(track);
    track->trackDone = trackDone;

    return trackDone;
}

DWORD UpdateTrack(TRACK *track, BOOL *wasStopped)
{
    DWORD curPlayPos, curWritePos;

    IDirectSoundBuffer_GetCurrentPosition(
        track->dsBuffer,
        &curPlayPos,
        &curWritePos
    );
    DWORD actualPlayPos = curPlayPos;
    if (actualPlayPos < track->prevPlayPos)
    {
        // play position has wrapped since last call; unwrap position and
        // increment loop count
        actualPlayPos += track->soundBufSize;
        track->bufLoopCount++;
    }

    // if all audio has been read, no more samples need to be transferred
    if (track->readDone)
        return 0;

    DWORD samples = 0;
    *wasStopped = FALSE;
    DWORD t2 = track->field_3C;
    if (t2 < track->field_38)
        t2 += track->soundBufSize;

    if (actualPlayPos >= track->field_38 && actualPlayPos <= t2)
    {
        // stop playing the track; either it's done playing, or the DS buffer's
        // position will be changed
        if (track->playing)
        {
            StopTrack(track);
            *wasStopped = TRUE;
        }

        if (!CheckTrackDone(track))
        {
            samples = track->convBufSize / track->wfxOut.nBlockAlign;
            IDirectSoundBuffer_SetCurrentPosition(
                track->dsBuffer,
                track->field_38
            );
            if (curPlayPos < track->prevPlayPos &&
                curPlayPos < track->field_38)
            {
                track->bufLoopCount--;
            }
        }
    }
    else
    {
        DWORD deltaPos = actualPlayPos - track->prevPlayPos;
        if (deltaPos < track->field_68)
            deltaPos = 0;

        samples = deltaPos / track->wfxOut.nBlockAlign;
    }

    if (samples)
        track->prevPlayPos = curPlayPos;

    return samples;
}

void ConvertTrackAudio(
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
        // input must be 4bps, output 16bps
        if (track->wfxIn.wBitsPerSample != 4 ||
            track->wfxOut.wBitsPerSample != 16)
        {
            return;
        }

        CvtStereoAdpcm(track, src, dst, count);
    }
    else if (track->wfxIn.nChannels == 1 && track->wfxOut.nChannels == 2)
    {
        // input mono, output stereo

        // don't support sample rate conversion
        if (track->wfxIn.nSamplesPerSec != track->wfxOut.nSamplesPerSec)
            return;
        // input must be 4bps, output 16bps
        if (track->wfxIn.wBitsPerSample != 4 ||
            track->wfxOut.wBitsPerSample != 16)
        {
            return;
        }

        CvtMonoAdpcm(track, src, dst, count);
    }
}

void CvtStereoAdpcm(
    TRACK *track,
    const void *src,
    void *dst,
    size_t count
)
{
    int leftSamp, rightSamp;
    int rightState, leftState;
    int leftVal, rightVal;
    int mag;
    BOOL sign;
    int sampDelta;

    const BYTE *srcPtr = (const BYTE *)src;
    short *dstPtr = (short *)dst;

    if (track)
    {
        // use provided track object for initial state
        leftSamp = track->adpcmState.leftSamp;
        rightSamp = track->adpcmState.rightSamp;
        leftState = track->adpcmState.leftState;
        rightState = track->adpcmState.rightState;
    }
    else
    {
        // otherwise use zeroes for initial state
        leftSamp = 0;
        rightSamp = 0;
        leftState = 0;
        rightState = 0;
    }

    while (count--)
    {
        BYTE srcSamp = *(srcPtr++);

        // decode left channel
        leftVal = adpcmStateDecode[leftState];
        BYTE srcLeft = srcSamp & 0xf;
        leftState += adpcmStateAdj[srcLeft];
        if (leftState < 0)
            leftState = 0;
        else if (leftState > 88)
            leftState = 88;

        sign = !!(srcLeft & 8);
        mag = srcLeft & 7;
        sampDelta = 0;
        for (int i = 0; i < 3; i++)
        {
            if (mag & (1 << (2 - i)))
                sampDelta += leftVal >> i;
        }
        sampDelta += leftVal >> 3;
        if (sign)
            leftSamp -= sampDelta;
        else
            leftSamp += sampDelta;

        // clamp sample and write
        if (leftSamp > 0x7fff)
            leftSamp = 0x7fff;
        else if (leftSamp < -0x8000)
            leftSamp = -0x8000;
        *(dstPtr++) = leftSamp;

        // decode right channel
        rightVal = adpcmStateDecode[rightState];
        BYTE srcRight = (srcSamp >> 4) & 0xf;
        rightState += adpcmStateAdj[srcRight];
        if (rightState < 0)
            rightState = 0;
        else if (rightState > 88)
            rightState = 88;

        sign = !!(srcRight & 8);
        mag = srcRight & 7;
        sampDelta = 0;
        for (int i = 0; i < 3; i++)
        {
            if (mag & (1 << (2 - i)))
                sampDelta += rightVal >> i;
        }
        sampDelta += rightVal >> 3;
        if (sign)
            rightSamp -= sampDelta;
        else
            rightSamp += sampDelta;

        // clamp sample and write
        if (rightSamp > 0x7fff)
            rightSamp = 0x7fff;
        else if (rightSamp < -0x8000)
            rightSamp = -0x8000;
        *(dstPtr++) = rightSamp;
    }

    if (track)
    {
        // write back decode state
        track->adpcmState.leftSamp = leftSamp;
        track->adpcmState.rightSamp = rightSamp;
        track->adpcmState.leftState = leftState;
        track->adpcmState.rightState = rightState;
    }
}

void CvtMonoAdpcm(
    TRACK *track,
    const void *src,
    void *dst,
    size_t count
)
{
    // the current format validation in InitTrack should prevent this from ever
    // being called
    MessageBoxA(
        hWndGame,
        "This shouldn't be possible",
        "BugsDecomp",
        MB_OK | MB_ICONERROR
    );
    DebugBreak();
}
