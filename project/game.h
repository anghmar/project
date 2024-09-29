#pragma once
#include <SDL.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include "AudioSystem.h"

class Game
{
	public:
		Game();
		bool Initialize();
		void RunLoop();
		void Shutdown();

		void AddActor(class Actor* actor);
		void RemoveActor(class Actor* actor);

		class Renderer* GetRenderer() { return mRenderer; }
		class AudioSystem* GetAudioSystem() { return mAudioSystem; }
	private:
		void ProcessInput();
		void HandleKeyPress(int key);
		void UpdateGame();
		void GenerateOutput();
		void LoadData();
		void UnloadData();

		//Input system pointer
		class InputSystem* mInputSystem;

		// All the actors in the game
		std::vector<class Actor*> mActors;
		// Any pending actors
		std::vector<class Actor*> mPendingActors;

		class Renderer* mRenderer;
		// Audio System pointer
		class AudioSystem* mAudioSystem;

		Uint32 mTicksCount;
		bool mIsRunning;
		// Track if we're updating actors right now
		bool mUpdatingActors;

		// Game-specific code
		class CameraActor* mCameraActor;

		//Sound Events
		SoundEvent mMusicEvent;
		SoundEvent mReverbSnap;
};