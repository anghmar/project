#include "AudioSystem.h"
#include <fmod_studio.hpp>
#include <fmod_errors.h>
#include <SDL.h>
#include "SoundEvent.h"

unsigned int AudioSystem::sNextID = 0;

namespace
{
    FMOD_VECTOR VecToFMOD(const Vector3& in)
    {
        // Convert from our coordinates (+x forward, +y right, +z up)
        // to FMOD (+z forward, +x right, +y up)
        FMOD_VECTOR v;
        v.x = in.y;
        v.y = in.z;
        v.z = in.x;
        return v;
    }
}

AudioSystem::AudioSystem(Game* game)
    :mGame(game)
    ,mSystem(nullptr)
    ,mCoreSystem(nullptr)
{
}

AudioSystem::~AudioSystem()
{
}

void AudioSystem::SetListener(const Matrix4& viewMatrix)
{
    // Invert the view matrix to get the correct vectors
    Matrix4 invView = viewMatrix;
    invView.Invert();
    FMOD_3D_ATTRIBUTES listener;
    // Set pos, forward, up
    listener.position = VecToFMOD(invView.GetTranslation());
    // In the inverted view, third row is forward
    listener.forward = VecToFMOD(invView.GetZAxis());
    // In the inverted view, second row is up
    listener.up = VecToFMOD(invView.GetYAxis());
    // Set velocity to zero (fix if using Doppler effect)
    listener.velocity = { 0.0f, 0.0f, 0.0f };
    // Send to FMOD (0 = only one listener)
    mSystem->getListenerAttributes(0, &listener);
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

    LoadBank("Assets/Master Bank.strings.bank");
    LoadBank("Assets/Master Bank.bank");

    return true;
}

void AudioSystem::Shutdown()
{
    mSystem->release();
}

void AudioSystem::Update(float deltaTime)
{
    //Find any stopped event instances
    std::vector<unsigned int> done;
    for (auto& iter : mEventInstances)
    {
        FMOD::Studio::EventInstance* e = iter.second;

        //Get State of this event
        FMOD_STUDIO_PLAYBACK_STATE state;
        e->getPlaybackState(&state);
        if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
        {
            //Release the event and add ID to done
            e->release();
            done.emplace_back(iter.first);
        }
    }

    for (auto id : done)
    {
        mEventInstances.erase(id);
    }

    mSystem->update();
}

void AudioSystem::LoadBank(const std::string& name)
{
    //Prevent double loading
    if (mBanks.find(name) != mBanks.end())
    {
        //just this?
        return;
    }

    //Try to load bank
    FMOD::Studio::Bank* bank = nullptr;
    FMOD_RESULT result = mSystem->loadBankFile(
        name.c_str(),                      //filename of bank
        FMOD_STUDIO_LOAD_BANK_NORMAL,      //Normal loading
        &bank                              //Save pointer to bank
    );

    const int maxPathLength = 512;
    if (result == FMOD_OK)
    {
        //Add bank to map
        mBanks.emplace(name, bank);
        //Load all non-streaming sample data
        bank->loadSampleData();
        //Get the number of events in this bank
        int numEvents = 0;
        bank->getEventCount(&numEvents);
        
        if (numEvents > 0)
        {
            //Get list of event description in this bank
            std::vector<FMOD::Studio::EventDescription*> events(numEvents);
            bank->getEventList(events.data(), numEvents, &numEvents);
            char eventName[maxPathLength];
            for (int i = 0; i < numEvents; i++)
            {
                FMOD::Studio::EventDescription* e = events[i];
                //Get the path od this event (eg. event:/Explosion2D)
                e->getPath(eventName, maxPathLength, nullptr);
                //Add to event map
                mEvents.emplace(eventName, e);
            }
        }
    }
}

void AudioSystem::UnloadBank(const std::string& name)
{
    //Ignore if not loaded
    auto iter = mBanks.find(name);
    if (iter == mBanks.end())
    {
        return;
    }

    //First we need to remove all events from this bank
    FMOD::Studio::Bank* bank = iter->second;
    int numEvents = 0;
    bank->getEventCount(&numEvents);
    if (numEvents > 0)
    {
        // Get event descriptions for this bank
        std::vector<FMOD::Studio::EventDescription*> events(numEvents);
        // Get list of events
        bank->getEventList(events.data(), numEvents, &numEvents);
        char eventName[512];
        for (int i = 0; i < numEvents; i++)
        {
            FMOD::Studio::EventDescription* e = events[i];
            // Get the path of this event
            e->getPath(eventName, 512, nullptr);
            // Remove this event
            auto eventToRemove = mEvents.find(eventName);
            if (eventToRemove != mEvents.end())
            {
                mEvents.erase(eventToRemove);
            }
        }
    }

    int numBuses = 0;
    bank->getBusCount(&numBuses);
    if (numBuses > 0)
    {
        std::vector<FMOD::Studio::Bus*> buses(numBuses);
        bank->getBusList(buses.data(), numBuses, &numBuses);
        char busName[512];
        for (int i = 0; i < numBuses; i++)
        {
            FMOD::Studio::Bus* bus = buses[i];
            //Get the path of this bus eg (bus:/SFX)
            bus->getPath(busName, 512, nullptr);
            //Remove this bus
            auto busToRemove = mBuses.find(busName);
            if (busToRemove != mBuses.end())
            {
                mBuses.erase(busToRemove);
            }
        }
    }
    
    //Unload sample data and bank
    bank->unloadSampleData();
    bank->unload();
    //Remove from banks map
    mBanks.erase(iter);
}

void AudioSystem::UnloadAllBanks(const std::string& name)
{
    for (auto& iter : mBanks)
    {
        //Unload the sample data, then the bank itself
        iter.second->unloadSampleData();
        iter.second->unload();
    }

    mBanks.clear();
    //No banks, no events
    mEvents.clear();
}

SoundEvent AudioSystem::PlayEvent(const std::string& name)
{
    unsigned int retID = 0;
    auto iter = mEvents.find(name);
    if (iter != mEvents.end())
    {
        FMOD::Studio::EventInstance* event = nullptr;
        iter->second->createInstance(&event);
        if (event)
        {
            event->start();
            
            //might fuck
            retID = ++sNextID;
            mEventInstances.emplace(retID, event);
        }
    }
    return SoundEvent(this, retID);
}

FMOD::Studio::EventInstance* AudioSystem::GetEventInstance(unsigned int id)
{
    FMOD::Studio::EventInstance* event = nullptr;
    auto iter = mEventInstances.find(id);
    if (iter != mEventInstances.end())
    {
        event = iter->second;
    }

    return event;
}

float AudioSystem::GetBusVolume(const std::string& name) const
{
    float retVal = 0.0f;
    const auto iter = mBuses.find(name);
    if (iter != mBuses.end())
    {
        iter->second->getVolume(&retVal);
    }

    return retVal;
}

bool AudioSystem::GetBusPaused(const std::string& name) const
{
    bool retVal = false;
    const auto iter = mBuses.find(name);
    if (iter != mBuses.end())
    {
        iter->second->getPaused(&retVal);
    }

    return retVal;
}

void AudioSystem::SetBusVolume(const std::string& name, float volume)
{
    auto iter = mBuses.find(name);
    if (iter != mBuses.end())
    {
        iter->second->setVolume(volume);
    }
}

void AudioSystem::SetBusPaused(const std::string& name, bool pause)
{
    auto iter = mBuses.find(name);
    if (iter != mBuses.end())
    {
        iter->second->setPaused(pause);
    }
}
