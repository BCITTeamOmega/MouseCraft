#include "SoundComponent.h"
#include "..\Event\EventManager.h"

SoundComponent::SoundComponent(SoundsList sound) { 
    ourSound = sound;
}

SoundComponent::~SoundComponent()
{
}

void SoundComponent::OnInitialized()
{

}

void SoundComponent::PlaySound(float x, float y, float z)
{
    //convert our information into a sound parameter
    SoundParams * ourParam = new SoundParams;
    //load sound handle
    ourParam->sound = ourSound;
    //Include Location data from arguments
    ourParam->x = x;
    ourParam->y = y;
    ourParam->z = z;
    //convert to event required form, fire event
    TypeParam<SoundParams*> param(ourParam);
    EventManager::Notify(PLAY_SOUND, &param);
}

void SoundComponent::ChangeSound(SoundsList sound)
{
    ourSound = sound;
}
