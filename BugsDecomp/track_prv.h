
// Private declarations for track module.

#pragma once


/// <summary>
/// Updates an audio track.
/// </summary>
/// <param name="track">Audio track to update</param>
/// <param name="wasStopped">Whether the track was playing, then stopped</param>
/// <returns>Number of frames that should be converted</returns>
EXTERN_C size_t UpdateTrack(TRACK *track, BOOL *wasStopped);
PATCH_CODE(0x4019a0, 0x4019a0, UpdateTrack);

/// <summary>
/// Converts a chunk of track audio.
/// </summary>
/// <param name="track">Track used for conversion</param>
/// <param name="src">Source audio data</param>
/// <param name="dst">Conversion destination</param>
/// <param name="count">Number of frames to convert</param>
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
/// <param name="count">Number of frames to convert</param>
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
/// <param name="count">Number of frames to convert</param>
EXTERN_C void CvtMonoAdpcm(
    TRACK *track,
    const void *src,
    void *dst,
    size_t count
);
PATCH_CODE(0x401e10, 0x401e10, CvtMonoAdpcm);
