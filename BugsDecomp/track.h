
// Code for playing and managing in-game audio tracks.

#pragma once


// represents an audio track
typedef struct TRACK
{
    WAVEFORMATEX wfxIn; // format of audio from file
    size_t trackInSize; // size, in bytes, of the audio to play from file
    int fd;
    size_t readBytes; // number of bytes read so far
    void *convBuf; // conversion buffer
    WAVEFORMATEX wfxOut; // format of audio to DirectSound buffer
    DWORD field_38;
    DWORD field_3C;
    DWORD prevPlayPos;
    DWORD loopCount; // how many times the DS buffer has looped
    size_t trackOutSize; // size, in bytes, of the audio to play to DS buffer
    struct {
        short leftSamp;
        short rightSamp;
        short leftState;
        short rightState;
    } adpcmState;
    BOOL flag_54;
    BOOL convBufOwned; // whether `convBuf` should be freed on finalization
    size_t soundBufSize; // size, in bytes, of the DS buffer
    size_t convBufSize; // size, in bytes, of the audio conversion buffer
    size_t field_64; // byte-size equivalent of TRACK_PARAMS.field_8
    size_t field_68; // byte-size equivalent of TRACK_PARAMS.field_C
    LPDIRECTSOUNDBUFFER dsBuffer;
    BOOL trackDone;
    BOOL playing;
    DWORD field_78;
} TRACK;


// params for audio tracks
typedef struct TRACK_PARAMS
{
    DWORD msSoundBufLen; // length of DS buffer, in milliseconds
    DWORD msConvBufLen; // length of conversion buffer, in milliseconds
    DWORD field_8; // in milliseconds
    DWORD field_C; // in milliseconds
} TRACK_PARAMS;


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
EXTERN_C int InitTrack(
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
/// <param name="flag_C">Unknown</param>
/// <returns>
/// 0 for success, or non-zero for error:
/// - 1 - track does not have a DirectSound buffer allocated
/// </returns>
EXTERN_C int SetTrackSource(
    TRACK *track,
    size_t trackInSize,
    int fd,
    BOOL flag_C
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
/// <param name="arg_4">Unknown</param>
/// <param name="arg_8">Unknown</param>
/// <returns>Unknown</returns>
EXTERN_C BOOL RefillTrackBuffer(TRACK *track, int arg_4, int arg_8);
//PATCH_CODE(0x4014f0, 0x4014f0, RefillTrackBuffer);

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

/// <summary>
/// Updates an audio track.
/// </summary>
/// <param name="track">Audio track to update</param>
/// <param name="wasStopped">Whether the track was playing, then stopped</param>
/// <returns>Sample position</returns>
EXTERN_C DWORD UpdateTrack(TRACK *track, BOOL *wasStopped);
PATCH_CODE(0x4019a0, 0x4019a0, UpdateTrack);

/// <summary>
/// Converts a chunk of track audio.
/// </summary>
/// <param name="track">Track used for conversion</param>
/// <param name="src">Source audio data</param>
/// <param name="dst">Conversion destination</param>
/// <param name="count">Number of elements to convert</param>
EXTERN_C void ConvertTrackAudio(
    TRACK *track,
    const void *src,
    void *dst,
    size_t count
);
PATCH_CODE(0x401af0, 0x401af0, ConvertTrackAudio);

/// <summary>
/// Converts stereo ADPCM audio.
/// </summary>
/// <param name="track">Track used for conversion</param>
/// <param name="src">Source audio data</param>
/// <param name="dst">Conversion destination</param>
/// <param name="count">Number of elements to convert</param>
EXTERN_C void CvtStereoAdpcm(
    TRACK *track,
    const void *src,
    void *dst,
    size_t count
);
PATCH_CODE(0x401be0, 0x401be0, CvtStereoAdpcm);

/// <summary>
/// Converts mono ADPCM audio.
/// </summary>
/// <param name="track">Track used for conversion</param>
/// <param name="src">Source audio data</param>
/// <param name="dst">Conversion destination</param>
/// <param name="count">Number of elements to convert</param>
EXTERN_C void CvtMonoAdpcm(
    TRACK *track,
    const void *src,
    void *dst,
    size_t count
);
PATCH_CODE(0x401e10, 0x401e10, CvtMonoAdpcm);
