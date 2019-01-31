#pragma once
//created by AssetLoader

//Audio library includes
#include "Al/al.h"
#include "Al/alc.h"

//class that defines the format for storing Audio Data
class AudioData{
public:
    int channel; //number of channels in the file
    int BitsPerSample;
    int size;
    int SampleRate;
    char* data; //actual sound data
    //default constructor values, allows us to check for errors
    AudioData() {
        channel = 0; BitsPerSample = 0; size = 0; SampleRate = 0;
        data = 0;
    }
};

//class that runs the Engine
class Sound {
public:
    //methods
    //constructor and Destructor
	Sound();
    ~Sound();
    
    //method that allows an audio source to begin playing
    void PlayAudio(ALuint source);
    //method that stops active playback of an audio source
    void PauseAudio(ALuint source);
    //method that checks if an audio source is currently playing
    bool IsPlaying(ALuint source);
    //method that takes a buffer and returns an empty one
    void ClearBuffer(ALuint buffer, ALuint source);
    //method that makes an empty buffer
    void MakeBuffer(ALuint* buffer);
    //method that makes an audio source
    void MakeSource(ALuint* source);
    //method that places an audio source in the world relative to the listener
    void PlaceSource(ALuint source, float x, float y, float z);
    //method that sets an audio source to loop or not
    void ToggleLooping(ALuint source, bool loop);
    //method that loads audio data into a buffer and attaches it to the source. File is the Original file that has been stored in memory, not a fresh I/O operation
    void BufferData(ALuint buffer, ALuint source, AudioData file);
    //method that reads from a file to create an AudioData object
    AudioData GetAudioData(const char* fn);


    //variables
    //our sound device
    ALCdevice* Device;
    //the sound context
    ALCcontext* Context;

private:
    //method that reads a wav file
    char * readWavFile(const char* fn, int& chan, int& samplerate, int& bps, int& size);
    //method that determines the sound format
    unsigned int determineFormat(int channels, int bps);
    //Helper method to convert byte data to the an int that is accurate to how this machine stores numbers
    int convertToInt(char* buffer, int len);
    //method that determines how this machine stores numbers
    bool isBigEndian();
};