#pragma once
//has Sounds; created by main
#include "Sound.h"
#include "../Event/EventManager.h"
#include "SoundParams.h"
#include "TrackParams.h"
#include <unordered_map>
#include "../Core/System.h"
#define MAX_SOUND_BUFFERS 10

//actual Manager class, is an ISubScriber to use the event system
class SoundManager : public ISubscriber, public System {
public:
    //constructor
	SoundManager();
    
    //destructor
    ~SoundManager();
    
    //method that takes in a Music track and plays it at the given position
    void PlaySong(TrackList track, float x, float y, float z);
    
    //method that takes in a Sound Effect Name and plays it at the given position
    void PlaySound(SoundsList soundEffect, float x, float y, float z);
    
    //method that deletes all of our stored memory before close. Called by Destructor
    void CleanUp();
    
    //Required Event Notification Method
    void Notify(EventName eventName, Param* param);
private:
    //internal method to load in audio data from a file to minimize I/O operations
    void loadAudioData();

    //references to the music buffer and source
    ALuint bgmBuffer;
    ALuint bgmSource;

    //references to the Sound Effect buffers and Source
    ALuint seBuffer[MAX_SOUND_BUFFERS];
    ALuint seSource[MAX_SOUND_BUFFERS];

    //pointer to our actual engine object
    Sound* soundObject;

    //Song and Sound Effect storage so we don't need to I/O everytime we want them.
    std::unordered_map<TrackList, AudioData> Music;
    std::unordered_map<SoundsList, AudioData> SoundEffects;

    // Inherited via System
    virtual void Update(float dt) override;
};