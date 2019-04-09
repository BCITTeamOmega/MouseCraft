#include "TrackParams.h"
#include "../Event/EventManager.h"

void selectSong(TrackList track)
{
    //create Track Params for event
    TrackParams * initial = new TrackParams();
    //select song
    initial->track = track;
    //specify song location. Usually fine to leave with default values of 0
    initial->x = 0;
    initial->y = 0;
    initial->z = 0;
    //create a type Param from the track params and pass it into the event notifier
    TypeParam<TrackParams*> param(initial);
    EventManager::Notify(PLAY_SONG, &param);
}
