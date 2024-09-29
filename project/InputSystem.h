#pragma once
#include <SDL_scancode.h>
#include <SDL_gamecontroller.h>
#include <SDL_mouse.h>
#include "Math.h"

//Button States
enum ButtonState
{
	ENone,
	EPressed,
	EReleased,
	EHeld
};

// Wrapper that contains current state of input
struct InputState
{
	KeyboardState Keyboard;
};

class InputSystem
{
public:
	bool Initialize();
	void Shutodwn();

	// Called right before SDL_PollEvents loop
	void PrepareForUpdate();
	// Called right after SDL_PollEvents loop
	void Update();
private:
	InputState mState;
};