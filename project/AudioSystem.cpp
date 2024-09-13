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
    mSystem->update();
}

void AudioSystem::LoadBank(const std::string& name)
{
    //Prevent double loading
    if (mBanks.find(name) != mBanks.end())
    {
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
