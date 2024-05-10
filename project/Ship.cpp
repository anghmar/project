#include "Ship.h"
#include "SpriteComponent.h"
#include "InputComponent.h"
#include "Game.h"
#include "Laser.h"

Ship::Ship(Game* game)
	:Actor(game)
	, mLaserCooldown(0.0f)
{
	// Create a sprite component
	SpriteComponent* sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));

	// Create an input component and set keys/speed
	InputComponent* ic = new InputComponent(this);
	ic->SetForwardKey(SDL_SCANCODE_W);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
	ic->SetMaxForwardSpeed(300.0f);
	ic->SetMaxAngularSpeed(Math::Pi);
}

void Ship::UpdateActor(float deltaTime)
{
	mLaserCooldown -= deltaTime;
}

void Ship::ActorInput(const uint8_t* keyState)
{
	if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f)
	{
		if (CurrentFrameWalk >= 6)
		{
			CurrentFrameWalk = 0;
		}
		mRightSpeed += 250.0f;
		mAnimSpriteComponent->SetCurrentAnimationManual(static_cast<int>(CurrentFrameWalk), 1);
		std::cout << "CurrentFrameWalk" << CurrentFrameWalk << std::endl;
		//CurrentFrameWalk++;
	}
	else if (state[SDL_SCANCODE_A])
	{
		if (CurrentFrameWalk >= 6)
		{
			CurrentFrameWalk = 0;
		}
		mRightSpeed -= 250.0f;
		mAnimSpriteComponent->SetCurrentAnimationManual(static_cast<int>(CurrentFrameWalk), 1);
		//CurrentFrameWalk++;
	}
	// up/down
	if (state[SDL_SCANCODE_S])
	{
		//mDownSpeed += 300.0f;
		mAnimSpriteComponent->SetCurrentAnimation(3);
	}
	if (state[SDL_SCANCODE_W])
	{
		//mDownSpeed -= 300.0f;

		if (CurrentFrameJump >= 9)
		{
			CurrentFrameJump = 0;
		}
		//mRightSpeed += 250.0f;
		mAnimSpriteComponent->SetCurrentAnimationManual(static_cast<int>(CurrentFrameJump), 2);
		std::cout << "CurrentFrameJump" << CurrentFrameJump << std::endl;
		//CurrentFrameWalk++;

		// Reset laser cooldown (half second)
		mLaserCooldown = 0.5f;
	}
}