#include "Ship.h"
#include "AnimSpriteComponent.h"
#include "Game.h"
#include <iostream>

Ship::Ship(Game* game)
	:Actor(game)
	, mRightSpeed(0.0f)
	, mDownSpeed(0.0f)
	, CurrentFrameWalk(0)
{
	// Create an animated sprite component
	mAnimSpriteComponent = new AnimSpriteComponent(this);
	//AnimSpriteComponent* asc = new AnimSpriteComponent(this);

	std::vector<SDL_Texture*> IdleAnim = {
		game->GetTexture("Assets/Character01.png"),
	};

	std::vector<SDL_Texture*> WalkAnim = {
		game->GetTexture("Assets/Character01.png"),
		game->GetTexture("Assets/Character02.png"),
		game->GetTexture("Assets/Character03.png"),
		game->GetTexture("Assets/Character04.png"),
		game->GetTexture("Assets/Character05.png"),
		game->GetTexture("Assets/Character06.png"),
	};

	std::vector<SDL_Texture*> JumpAnim = {
		game->GetTexture("Assets/Character07.png"),
		game->GetTexture("Assets/Character08.png"),
		game->GetTexture("Assets/Character09.png"),
		game->GetTexture("Assets/Character10.png"),
		game->GetTexture("Assets/Character11.png"),
		game->GetTexture("Assets/Character12.png"),
		game->GetTexture("Assets/Character13.png"),
		game->GetTexture("Assets/Character14.png"),
		game->GetTexture("Assets/Character15.png"),
	};

	std::vector<SDL_Texture*> FightAnim = {
		game->GetTexture("Assets/Character16.png"),
		game->GetTexture("Assets/Character17.png"),
		game->GetTexture("Assets/Character18.png"),
	};

	/*std::vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/Ship01.png"),
		game->GetTexture("Assets/Ship02.png"),
		game->GetTexture("Assets/Ship03.png"),
		game->GetTexture("Assets/Ship04.png"),
	};*/

	mAnimSpriteComponent->SetAnimations({
		{ IdleAnim, 1, true },  // Walking animation, looping
		{ WalkAnim, 6, true },  // Walking animation, looping
		{ JumpAnim, 9, false }, // Jumping animation, non-looping
		{ FightAnim, 3, false } // Fighting animation, non-looping
		});

	//asc->SetAnimTextures(anims);
}

void Ship::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	// Update position based on speeds and delta time
	Vector2 pos = GetPosition();
	pos.x += mRightSpeed * deltaTime;
	pos.y += mDownSpeed * deltaTime;
	// Restrict position to left half of screen
	if (pos.x < 25.0f)
	{
		pos.x = 25.0f;
	}
	else if (pos.x > 500.0f)
	{
		pos.x = 500.0f;
	}
	if (pos.y < 25.0f)
	{
		pos.y = 25.0f;
	}
	else if (pos.y > 743.0f)
	{
		pos.y = 743.0f;
	}
	SetPosition(pos);

	//if (!mAnimSpriteComponent->isAnimationPlaying())
	//{
	//	// Set the animation to idle animation
	//	mAnimSpriteComponent->SetCurrentAnimation(0); // Idle animation
	//}
}

void Ship::ProcessKeyboard(const uint8_t* state)
{
	mRightSpeed = 0.0f;
	mDownSpeed = 0.0f;

	CurrentFrameWalk += 1;
	CurrentFrameJump += 1;
	//int CurrentFrame = 0;
	// right/left
	if (state[SDL_SCANCODE_D])
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

		//mAnimSpriteComponent->SetCurrentAnimation(2);
	}
	//if (!state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_S] && !state[SDL_SCANCODE_W])
	//{
	//	mAnimSpriteComponent->SetCurrentAnimation(0); // Walking animation
	//}
}