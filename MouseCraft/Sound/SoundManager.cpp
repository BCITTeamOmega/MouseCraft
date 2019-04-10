#include "SoundManager.h"
#include <iostream>
#include <mutex>
//defines for track filenames
#define MAIN_BGM "res/music/bgm2.wav" 
#define MENU_BGM "res/music/bgm1.wav"
#define GAMEOVER_BGM "res/music/bgm3.wav"

//defines for SoundEffect Names
#define JUMP_SOUND "res/SoundEffects/jumpCat.wav"
#define SWIPE_SOUND "res/SoundEffects/swipe.wav"
#define TRAMP_SOUND "res/SoundEffects/trampoline.wav"
#define THUD_SOUND  "res/SoundEffects/obstacleThud.wav"
#define SQUEAK_SOUND "res/SoundEffects/squeak.wav"
#define CAT_HURT_SOUND "res/SoundEffects/catScream.wav"


//private mutex variable
std::mutex SoundMtx;
//our currently selected sound effect buffer to play sound effects from.
int currentSoundBuffer;

//Constructor. Creates our Sound Engine object, loads all our audio files into memory, and instantiates our Audio sources/buffers. Also subscribes us to events.
SoundManager::SoundManager() {
    //make engine object
    soundObject = new Sound();
    //make BGM objects
    soundObject->MakeBuffer(&bgmBuffer);
    soundObject->MakeSource(&bgmSource);
    //make Sound effect Objects
    for (int i = 0; i < MAX_SOUND_BUFFERS; i++) {
        soundObject->MakeSource(&seSource[i]);
        soundObject->MakeBuffer(&seBuffer[i]);
    }
    //set our Sound effect buffer to the first one
    currentSoundBuffer = 0;
    //load in audio files to memory
    loadAudioData();
    //subscribe to events
	EventManager::Subscribe(PLAY_SONG, this);
	EventManager::Subscribe(PLAY_SOUND, this);
}

//method that loads Audio Files into memory to minimize repeated file I/O
void SoundManager::loadAudioData() {
    //Load Music Section
    
    //load main BGM and store it
    AudioData temp; //temp storage
    temp = soundObject->GetAudioData(MAIN_BGM); //attempt to read file
    if (temp.data == NULL) {
        std::cout << "Main BGM failed to store correctly." << std::endl; //handle failed file read
    }
    Music.insert(std::pair<TrackList,AudioData>(MainBGM, temp)); //store in track memory with handle
    
    //load menu BGM, steps are identical to above but with different file and Handle
    temp = soundObject->GetAudioData(MENU_BGM);
    if (temp.data == NULL) {
        std::cout << "Menu BGM failed to store correctly." << std::endl;
    }
    Music.insert(std::pair<TrackList, AudioData>(MenuBGM, temp));

    //load game over music bgm.
    temp = soundObject->GetAudioData(GAMEOVER_BGM);
    if (temp.data == NULL) {
        std::cout << "Welcome BGM failed to store correctly." << std::endl;
    }
    Music.insert(std::pair<TrackList, AudioData>(GameoverBGM, temp));

    //Load Sounds Section
    //Jump Sound
    temp = soundObject->GetAudioData(JUMP_SOUND); //attempt to read file
    if (temp.data == NULL) {
        std::cout << "Jump SE failed to store correctly." << std::endl; //handle failed file read
    }
    SoundEffects.insert(std::pair<SoundsList, AudioData>(Jump, temp)); //store in Sounds Memory with handle

    //Cat Attacking Sound, steps are identical to above but with different file and Handle
    temp = soundObject->GetAudioData(SWIPE_SOUND);
    if (temp.data == NULL) {
        std::cout << "Swiping SE failed to store correctly." << std::endl;
    }
    SoundEffects.insert(std::pair<SoundsList, AudioData>(Swipe, temp));

    //Trampoline sound, steps are identical to above but with different file and Handle
    temp = soundObject->GetAudioData(TRAMP_SOUND);
    if (temp.data == NULL) {
        std::cout << "Trampoline SE failed to store correctly." << std::endl;
    }
    SoundEffects.insert(std::pair<SoundsList, AudioData>(Trampoline, temp));

    //Hitting obstacle sound, steps are identical to above but with different file and Handle
    temp = soundObject->GetAudioData(THUD_SOUND);
    if (temp.data == NULL) {
        std::cout << "Obstacle hit SE failed to store correctly." << std::endl;
    }
    SoundEffects.insert(std::pair<SoundsList, AudioData>(Thud, temp));

    //Mouse Damage sound, steps are identical to above but with different file and Handle
    temp = soundObject->GetAudioData(SQUEAK_SOUND);
    if (temp.data == NULL) {
        std::cout << "Squeak SE failed to store correctly." << std::endl;
    }
    SoundEffects.insert(std::pair<SoundsList, AudioData>(Squeak, temp));

    //Mouse Damage sound, steps are identical to above but with different file and Handle
    temp = soundObject->GetAudioData(CAT_HURT_SOUND);
    if (temp.data == NULL) {
        std::cout << "Squeak SE failed to store correctly." << std::endl;
    }
    SoundEffects.insert(std::pair<SoundsList, AudioData>(CatScream, temp));
}

void SoundManager::Update(float dt)
{ }

//destructor. Cleans up the engine object/ local memory and unsubscribes
SoundManager::~SoundManager() {
    CleanUp();
	EventManager::Unsubscribe(PLAY_SONG, this);
	EventManager::Unsubscribe(PLAY_SOUND, this);
}

//Method that plays the given song at the given position
void SoundManager::PlaySong(TrackList track, float x, float y, float z) {
    //ensure buffer is cleared
    soundObject->PauseAudio(bgmSource); //prevent accessing active memory
    soundObject->ClearBuffer(bgmBuffer,bgmSource); //empty buffer to load new data

    //switch statement that handles each song. Format is the same for each.
        // Song Label
            // load song data into the BGM buffer from memory and attach it to the source
            // set if we want this track to loop. We usually do, but not always
            // set the location from which the song should play.
            // actually play music from the source
    switch (track){
        case MainBGM:
            soundObject->BufferData(bgmBuffer, bgmSource, Music.find(MainBGM)->second);
            soundObject->ToggleLooping(bgmSource, true);
            soundObject->PlaceSource(bgmSource, x, y, z);
            soundObject->PlayAudio(bgmSource);
            break;
        case MenuBGM:
            soundObject->BufferData(bgmBuffer, bgmSource, Music.find(MenuBGM)->second);
            soundObject->ToggleLooping(bgmSource, true);
            soundObject->PlaceSource(bgmSource, x, y, z);
            soundObject->PlayAudio(bgmSource);
            break;
        case GameoverBGM:
            soundObject->BufferData(bgmBuffer, bgmSource, Music.find(GameoverBGM)->second);
            soundObject->ToggleLooping(bgmSource, true);
            soundObject->PlaceSource(bgmSource, x, y, z);
            soundObject->PlayAudio(bgmSource);
            break;
        default:
            break;
    }
}

//method that plays the given sound effect at the given location
void SoundManager::PlaySound(SoundsList soundEffect, float x, float y, float z) {
    
    //mutex check to ensure we don't commit access violations by accessing dead buffers while they're being remade.
    SoundMtx.lock();
    int selectedBuffer = currentSoundBuffer; //selct the current buffer
    currentSoundBuffer++; //increment the buffer to the next buffer
    if (currentSoundBuffer >= MAX_SOUND_BUFFERS) { //if we have gone past the number of buffers we have
        currentSoundBuffer = 0; //wrap back around to oldest buffer
    }
    SoundMtx.unlock(); //unlock the mutex
    
    //ensure that our selected buffer is cleared
    soundObject->PauseAudio(seSource[selectedBuffer]); //prevent access to active memory
    soundObject->ClearBuffer(seBuffer[selectedBuffer], seSource[selectedBuffer]); //empty out any existing data from the buffer

    //switch statement that handles each sound. Format is the same for each.
        // Sound Label
            // load sound data into the selected sound buffer from memory and attach it to the source
            // set if we want this track to loop. We usually don't, but not always
            // set the location from which the sound should play.
            // actually play sound from the source
    switch (soundEffect) {
    case Jump:
        soundObject->BufferData(seBuffer[selectedBuffer], seSource[selectedBuffer], SoundEffects.find(Jump)->second);
        soundObject->ToggleLooping(seSource[selectedBuffer], false);
        soundObject->PlaceSource(seSource[selectedBuffer], x, y, z);
        soundObject->changeSoundVolume(seSource[selectedBuffer], 1.0);
        soundObject->PlayAudio(seSource[selectedBuffer]);
        break;
    case Swipe:
        soundObject->BufferData(seBuffer[selectedBuffer], seSource[selectedBuffer], SoundEffects.find(Swipe)->second);
        soundObject->ToggleLooping(seSource[selectedBuffer], false);
        soundObject->PlaceSource(seSource[selectedBuffer], x, y, z);
        soundObject->changeSoundVolume(seSource[selectedBuffer], 1.0);
        soundObject->PlayAudio(seSource[selectedBuffer]);
        break;
    case Trampoline:
        soundObject->BufferData(seBuffer[selectedBuffer], seSource[selectedBuffer], SoundEffects.find(Trampoline)->second);
        soundObject->ToggleLooping(seSource[selectedBuffer], false);
        soundObject->PlaceSource(seSource[selectedBuffer], x, y, z);
        soundObject->changeSoundVolume(seSource[selectedBuffer], 1.0);
        soundObject->PlayAudio(seSource[selectedBuffer]);
        break;
    case Thud:
        soundObject->BufferData(seBuffer[selectedBuffer], seSource[selectedBuffer], SoundEffects.find(Thud)->second);
        soundObject->ToggleLooping(seSource[selectedBuffer], false);
        soundObject->PlaceSource(seSource[selectedBuffer], x, y, z);
        soundObject->changeSoundVolume(seSource[selectedBuffer], 1.0);
        soundObject->PlayAudio(seSource[selectedBuffer]);
        break;
    case Squeak:
        soundObject->BufferData(seBuffer[selectedBuffer], seSource[selectedBuffer], SoundEffects.find(Squeak)->second);
        soundObject->ToggleLooping(seSource[selectedBuffer], false);
        soundObject->PlaceSource(seSource[selectedBuffer], x, y, z);
        soundObject->changeSoundVolume(seSource[selectedBuffer], 1.0);
        soundObject->PlayAudio(seSource[selectedBuffer]);
        break;
    case CatScream:
        soundObject->BufferData(seBuffer[selectedBuffer], seSource[selectedBuffer], SoundEffects.find(CatScream)->second);
        soundObject->ToggleLooping(seSource[selectedBuffer], false);
        soundObject->PlaceSource(seSource[selectedBuffer], x, y, z);
        soundObject->changeSoundVolume(seSource[selectedBuffer], 1.0);
        soundObject->PlayAudio(seSource[selectedBuffer]);
        break;
    default:
        break;
    }
}

//helper for the destructor that deletes all of our stored memory
void SoundManager::CleanUp() {
    //clean up step

    //bgm cleanup
    alDeleteSources(1, &bgmSource);
    alDeleteBuffers(1, &bgmBuffer);

    //sound effect cleanup
    for (int i = 0; i < MAX_SOUND_BUFFERS; i++) {
        alDeleteSources(1, &seSource[i]);
        alDeleteBuffers(1, &seBuffer[i]);
    }

    //sound object cleanup
    soundObject->~Sound();
}

//notify method for the event system
void SoundManager::Notify(EventName eventName, Param* param) {

    //switch based on event name to handle properly
    switch (eventName) {
        case PLAY_SONG: { //play a piece of BGM
            TrackParams* TrackInfo;
            // Safetly cast generic param pointer to a specific type
            TypeParam<TrackParams *> *p = dynamic_cast<TypeParam<TrackParams*> *>(param);
            if (p != nullptr) {
                // Successful type cast
                //extract out the Track Params object
                TrackInfo = p->Param;
                //call the play song method with the Track Param data
                PlaySong(TrackInfo->track, TrackInfo->x, TrackInfo->y, TrackInfo->z);
            }
            break;
        }
        case PLAY_SOUND:{ //play a specific sound
            SoundParams* SoundInfo;
            // Safetly cast generic param pointer to a specific type
            TypeParam<SoundParams*> *sound = dynamic_cast<TypeParam<SoundParams*> *>(param);
            if (sound != nullptr) {
                // Successful type cast
                //extract out the Sound Params object
                SoundInfo = sound->Param;
                //call the play sound method with the Sound Param data
                PlaySound(SoundInfo->sound, SoundInfo->x, SoundInfo->y, SoundInfo->z);
                //delete pointers so they don't stay in the heap
                delete SoundInfo;
            }
            break;
        }
        default:
            break;
    }

}