#pragma once
#include "..\Core\Component.h"
#include "SoundParams.h"

class SoundComponent : public Component
{
public:
    SoundComponent(SoundsList sound);
    ~SoundComponent();
    void OnInitialized() override;
    void PlaySound(float x, float y, float z);
    void ChangeSound(SoundsList sound);
private:
    SoundsList ourSound;
};