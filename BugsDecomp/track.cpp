
#include "stdafx.h"
#include "common.h"
#include "track.h"
#include "crt.h"


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
