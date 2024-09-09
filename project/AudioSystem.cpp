#include "AudioSystem.h"
#include <fmod_studio.hpp>
#include <fmod_errors.h>
#include <SDL.h>

AudioSystem::AudioSystem(Game* game)
    :mGame(game)
    ,mSystem(nullptr)
    ,mCoreSystem(nullptr)
{
}

AudioSystem::~AudioSystem()
{
}

bool AudioSystem::Initialize()
{
    FMOD::Debug_Initialize(
        FMOD_DEBUG_LEVEL_ERROR,     //LOG ONLY ERRORS
        FMOD_DEBUG_MODE_TTY);       //OUTPUT TO STDOUT

    FMOD_RESULT result;
    result = FMOD::Studio::System::create(&mSystem);
    if (result != FMOD_OK)
    {
        SDL_Log("Failed to create FMOD system: %s", FMOD_ErrorString(result));
        return false;
    }

    result = mSystem->initialize(
        512,                        // Max number of concurrent sounds/channels
        FMOD_STUDIO_INIT_NORMAL,    //Use def settings
        FMOD_INIT_NORMAL,           //Use def settings
        nullptr                     //usually null
    );
    if (result != FMOD_OK)
    {
        SDL_Log("Failed to init FMOD system: %s", FMOD_ErrorString(result));
        return false;
    }

    mSystem->getCoreSystem(&mCoreSystem);

    return true;
}

void AudioSystem::Shutdown()
{
    mSystem->release();
}

void AudioSystem::Update(float deltaTime)
{
    mSystem->update();
}