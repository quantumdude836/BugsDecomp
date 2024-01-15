
// Code for playing and managing in-game audio tracks.

#pragma once


// represents an audio track
typedef struct TRACK
{
    // format of audio from file
    WAVEFORMATEX wfxIn;
    // size, in bytes, of the audio to play from file
    size_t trackInSize;
    // file to read audio from
    int fd;
    // number of bytes read so far
    size_t readBytes;
    // conversion buffer
    void *convBuf;
    // format of audio to DirectSound buffer
    WAVEFORMATEX wfxOut;
    // position of last written audio
    DWORD curWritePos;
    // write position for next write period
    DWORD nextWritePos;
    // playback position as of most recent update
    DWORD prevPlayPos;
    // how many times the DS buffer has looped
    DWORD bufLoopCount;
    // size, in bytes, of the audio to play to DS buffer
    size_t trackOutSize;
    // ADPCM decode state
    struct {
        short leftSamp;
        short rightSamp;
        short leftState;
        short rightState;
    } adpcmState;
    // whether the track should loop
    BOOL loop;
    // whether `convBuf` should be freed on finalization
    BOOL convBufOwned;
    // size, in bytes, of the DS buffer
    size_t soundBufSize;
    // size, in bytes, of the audio conversion buffer
    size_t convBufSize;
    // refill period, in bytes
    size_t refillPeriodSize;
    // minimum buffer refill size, in bytes
    size_t minRefillSize;
    // DirectSound output buffer
    LPDIRECTSOUNDBUFFER dsBuffer;
    // whether track is done playing
    BOOL trackDone;
    // whether track is actively playing
    BOOL playing;
    // whether all audio has been read from file
    BOOL readDone;
} TRACK;

// timing params for audio tracks
typedef struct TRACK_PARAMS
{
    // length of DS buffer, in milliseconds
    DWORD msSoundBufLen;
    // length of conversion buffer, in milliseconds
    DWORD msConvBufLen;
    // refill period, in milliseconds
    DWORD msRefillPeriod;
    // minimum refill length, in milliseconds
    DWORD msMinRefill;
} TRACK_PARAMS;

// return codes for some track functions
typedef enum TRACK_ERROR
{
    // no error
    TRACK_OK,
    // track not initialized (does not have a DirectSound buffer)
    TRACK_ERR_NOT_INIT,
    // track already initialized (already has a DirectSound buffer)
    TRACK_ERR_ALREADY_INIT,
    // unable to create DirectSound buffer
    TRACK_ERR_DSBUFFER_CREATE,
    // invalid/unsupported audio format
    TRACK_ERR_AUDIO_FORMAT,
    // invalid timing params
    TRACK_ERR_TIMING_PARAMS,
} TRACK_ERROR;


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
/// <returns>TRACK_ERROR</returns>
EXTERN_C TRACK_ERROR InitTrack(
    TRACK *track,
    LPDIRECTSOUND dsound,
    LPCWAVEFORMATEX wfxIn,
    LPCWAVEFORMATEX wfxOut,
    const TRACK_PARAMS *params,
    void *convBuf
);
PATCH_CODE(0x401000, 0x401000, InitTrack);

/// <summary>
/// Finalizes a track, releasing all owned resources.
/// </summary>
/// <param name="track">Track to finalize</param>
EXTERN_C void FiniTrack(TRACK *track);
PATCH_CODE(0x401330, 0x401330, FiniTrack);

/// <summary>
/// Sets the source for an audio track. The given fd's file position should
/// already be set (via _lseek or equivalent).
/// </summary>
/// <param name="track">Track to setup</param>
/// <param name="trackInSize">Number of bytes to play</param>
/// <param name="fd">File descriptor to read audio from</param>
/// <param name="loop">Whether the track should loop</param>
/// <returns>TRACK_ERROR</returns>
EXTERN_C TRACK_ERROR SetTrackSource(
    TRACK *track,
    size_t trackInSize,
    int fd,
    BOOL loop
);
PATCH_CODE(0x4013c0, 0x4013c0, SetTrackSource);

/// <summary>
/// Resets a track to a default state. If the track is currently playing, it is
/// stopped.
/// </summary>
/// <param name="track">Track to reset</param>
EXTERN_C void ResetTrack(TRACK *track);
PATCH_CODE(0x401490, 0x401490, ResetTrack);

/// <summary>
/// Refills the DS buffer for an audio track.
/// </summary>
/// <param name="track">Track to refill</param>
/// <param name="frames">
/// Number of frames to convert, or -1 to auto-calculate
/// </param>
/// <param name="periodSize">
/// Size, in bytes, of the refill period, or -1 to use default
/// </param>
/// <returns>Whether the buffer was refilled</returns>
EXTERN_C BOOL RefillTrackBuffer(TRACK *track, size_t frames, size_t periodSize);
PATCH_CODE(0x4014f0, 0x4014f0, RefillTrackBuffer);

/// <summary>
/// Starts playing a music track.
/// </summary>
/// <param name="track">Track to play</param>
EXTERN_C void PlayTrack(TRACK *track);
PATCH_CODE(0x4018d0, 0x4018d0, PlayTrack);

/// <summary>
/// Stops playing a music track.
/// </summary>
/// <param name="track">Track to stop</param>
EXTERN_C void StopTrack(TRACK *track);
PATCH_CODE(0x401910, 0x401910, StopTrack);

/// <summary>
/// Check if a track is done playing, stopping it if so.
/// </summary>
/// <param name="track">Track to check</param>
/// <returns>Whether the track is done playing</returns>
EXTERN_C BOOL CheckTrackDone(TRACK *track);
PATCH_CODE(0x401930, 0x401930, CheckTrackDone);
