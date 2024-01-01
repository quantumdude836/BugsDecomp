
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
    DWORD field_60;
    DWORD field_64;
    DWORD field_68;
    IDirectSoundBuffer *dsBuffer;
    BOOL flag_70;
    BOOL playing;
    DWORD field_78;
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
