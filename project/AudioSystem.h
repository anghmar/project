#include <Sound>
#include <fmod_studio_common.h>

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

private:
	class Game* game;
	//FMOD studio sys
	FMOD::Studio::System* mSystem;
	//fmod low level sys (if needed)
	FMOD::System* mLowLevelSystem;
};