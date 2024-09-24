#pragma once
#include <unordered_map>
#include <string>
#include "SoundEvent.h"

//SAMPLE DATA is the raw audio data that events reference. 
//STREAMING DATA is sample data that streams into memory in small pieces at a time (MUSIC AND DIALOGUES)

namespace FMOD
{
	class System;
	namespace Studio
	{
		class Bank;
		class EventDescription;
		class EventInstance;
		class System;
		class Bus;
	};
};

class AudioSystem
{
	public:
		AudioSystem(class Game* game);
		~AudioSystem();

		bool Initialize();
		void Shutdown();
		void Update(float deltaTime);

		//banks
		void LoadBank(const std::string& name);
		void UnloadBank(const std::string& name);
		void UnloadAllBanks(const std::string& name);

		//events
		SoundEvent PlayEvent(const std::string& name);

	protected:
		friend class SoundEvent;
		FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);

	private:
		class Game* mGame;
		//FMOD studio sys
		FMOD::Studio::System* mSystem;
		//fmod low level sys (if needed)
		FMOD::System* mCoreSystem;

		// Tracks the next ID to use for event instances
		static unsigned int sNextID;

		//Map of loaded banks
		std::unordered_map<std::string, FMOD::Studio::Bank*> mBanks; //string is filename
		//Map of event name to EventDescription
		std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents; //string is the name by fmod for the event
		// Map of event id to EventInstance
		std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> mEventInstances;
		// Map of buses
		std::unordered_map<std::string, FMOD::Studio::Bus*> mBuses;
};