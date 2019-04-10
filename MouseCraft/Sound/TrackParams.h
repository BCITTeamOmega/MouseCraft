#pragma once
//for referencing songs. These are the internal names of each
enum TrackList {
    MainBGM, MenuBGM, GameoverBGM
};



//for notifying the song event, basically exists to let us use event system easily
struct TrackParams {
    TrackList track;
    float x;
    float y;
    float z;
};

//method to make it easier to declare a song change
void selectSong(TrackList track);
