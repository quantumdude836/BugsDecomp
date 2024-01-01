
// Code for playing and managing in-game audio tracks.

#pragma once


// represents an audio track
struct TRACK
{
    WAVEFORMATEX wfxIn; // format of audio from file
    size_t trackInSize; // size, in bytes, of the audio to play from file
    int fd;
    size_t readBytes; // number of bytes read so far
    void *convBuf; // conversion buffer
    WAVEFORMATEX wfxOut; // format of audio to DirectSound buffer
    DWORD field_38;
    DWORD field_3C;
    DWORD field_40;
    DWORD field_44;
    size_t trackOutSize; // size, in bytes, of the audio to play to DS buffer
    WORD field_4C;
    WORD field_4E;
    WORD field_50;
    WORD field_52;
    BOOL flag_54;
    BOOL convBufOwned; // whether `convBuf` should be freed on finalization
    size_t soundBufSize; // size, in bytes, of the DS buffer
    size_t convBufSize; // size, in bytes, of the audio conversion buffer
    size_t field_64; // byte-size equivalent of TRACK_PARAMS.field_8
    size_t field_68; // byte-size equivalent of TRACK_PARAMS.field_C
    LPDIRECTSOUNDBUFFER dsBuffer;
    BOOL flag_70;
    BOOL playing;
    DWORD field_78;
};


// params for audio tracks
struct TRACK_PARAMS
{
    size_t msSoundBufLen; // length of DS buffer, in milliseconds
    size_t msConvBufLen; // length of conversion buffer, in milliseconds
    size_t field_8; // in milliseconds
    size_t field_C; // in milliseconds
};


// "default" state for an uninitialized track
#define trackDefault (*reinterpret_cast<const TRACK *>(0x45c000))

// byte size of audio segments in the speech files; indexed by language, then by
// segment number (index 0 = segment 2?)
#define speechPartSizes (*reinterpret_cast<const size_t (*)[6][64]>(0x45c340))

// currently playing speech track
#define speechTrack (*reinterpret_cast<TRACK *>(0x5532a0))

// currently playing music track
#define musicTrack (*reinterpret_cast<TRACK *>(0x553320))


/// <summary>
/// Initializes an audio track.
/// </summary>
/// <param name="track">Track to initialize</param>
/// <param name="dsound">DirectSound instance</param>
/// <param name="wfxIn">Format of audio from file</param>
/// <param name="wfxOut">Format of audio to DS buffer</param>
/// <param name="params">Timing params</param>
/// <param name="convBuf">
/// Conversion buffer to use, or null to allocate one
/// </param>
/// <returns>
/// 0 for success, or non-zero for error:
/// - 2 - track already has a DirectSound buffer allocated
/// - 3 - unable to create DirectSound buffer
/// - 4 - invalid audio format
/// - 5 - invalid timing params
/// </returns>
extern "C" int InitTrack(
    TRACK *track,
    LPDIRECTSOUND dsound,
    LPCWAVEFORMATEX wfxIn,
    LPCWAVEFORMATEX wfxOut,
    const TRACK_PARAMS *params,
    void *convBuf
);

/// <summary>
/// Finalizes a track, releasing all owned resources.
/// </summary>
/// <param name="track">Track to finalize</param>
extern "C" void FiniTrack(TRACK *track);

/// <summary>
/// Resets a track to a default state. If the track is currently playing, it is
/// stopped.
/// </summary>
/// <param name="track">Track to reset</param>
extern "C" void ResetTrack(TRACK *track);

/// <summary>
/// Starts playing a music track.
/// </summary>
/// <param name="track">Track to play</param>
extern "C" void PlayTrack(TRACK *track);

/// <summary>
/// Stops playing a music track.
/// </summary>
/// <param name="track">Track to stop</param>
extern "C" void StopTrack(TRACK *track);

/// <summary>
/// Converts a chunk of track audio.
/// </summary>
/// <param name="track">Track used for conversion</param>
/// <param name="src">Source audio data</param>
/// <param name="dst">Conversion destination</param>
/// <param name="count">Number of elements to convert</param>
extern "C" void ConvertTrackAudio(
    TRACK *track,
    const void *src,
    void *dst,
    size_t count
);

/// <summary>
/// Converts stereo ADPCM audio.
/// </summary>
/// <param name="track">Track used for conversion</param>
/// <param name="src">Source audio data</param>
/// <param name="dst">Conversion destination</param>
/// <param name="count">Number of elements to convert</param>
extern "C" void CvtStereoAdpcm(
    TRACK *track,
    const void *src,
    void *dst,
    size_t count
);

/// <summary>
/// Converts mono ADPCM audio.
/// </summary>
/// <param name="track">Track used for conversion</param>
/// <param name="src">Source audio data</param>
/// <param name="dst">Conversion destination</param>
/// <param name="count">Number of elements to convert</param>
extern "C" void CvtMonoAdpcm(
    TRACK *track,
    const void *src,
    void *dst,
    size_t count
);