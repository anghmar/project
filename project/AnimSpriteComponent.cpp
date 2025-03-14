#include "AnimSpriteComponent.h"
#include "Math.h"
#include <iostream>

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder),
	mCurrentAnimationIndex(0),
	mCurrFrame(0.0f),
	mAnimFPS(24.0f),
	mIsPlaying(true)
	//currentAnimation(nullptr)
	//currentAnimation(mAnimations[mCurrentAnimationIndex])
{
}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	//if (mIsPlaying && !mAnimations.empty())
	//{
	//	Animation& currentAnimation = mAnimations[mCurrentAnimationIndex];

	//	//Update the current frame based on frame rate and delta time
	//	mCurrFrame += mAnimFPS * deltaTime;

	//	if (currentAnimation.looping)
	//	{
	//		while (mCurrFrame >= currentAnimation.endIndex)
	//		{
	//			mCurrFrame -= currentAnimation.endIndex;
	//		}
	//	}
	//	else
	//	{
	//		// Stop Animation if it reaches the end
	//		if (mCurrFrame >= currentAnimation.endIndex)
	//		{
	//			mIsPlaying = false;
	//			mCurrFrame = currentAnimation.endIndex - 1;
	//		}
	//	}

	//	SetTexture(currentAnimation.textures[static_cast<int>(mCurrFrame)]);
	//}

	if (mAnimTextures.size() > 0)
	{
		//Update the current frame based on frame rate and delta time
		mCurrFrame += mAnimFPS * deltaTime;

		//Wrap current frame if needed
		while (mCurrFrame >= mAnimTextures.size())
		{
			mCurrFrame -= mAnimTextures.size();
		}

		//Set the current texture
		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<Texture*>& textures)
{
	mAnimTextures = textures;
	if (mAnimTextures.size() > 0)
	{
		// Set the active texture to first frame
		mCurrFrame = 0.0f;
		SetTexture(mAnimTextures[0]);
	}
}

void AnimSpriteComponent::SetAnimations(const std::vector<Animation>& animations)
{
	mAnimations = animations;

	if (!mAnimations.empty())
	{
		mCurrentAnimationIndex = 0;
		mCurrFrame = 0.0f;
		SetTexture(mAnimations[mCurrentAnimationIndex].textures[0]);
	}
}

void AnimSpriteComponent::SetCurrentAnimation(int animationIndex)
{
	if (animationIndex >= 0 && animationIndex < mAnimations.size())
	{
		mCurrentAnimationIndex = animationIndex;
		mCurrFrame = 0.0f;
		mIsPlaying = true;

		SetTexture(mAnimations[mCurrentAnimationIndex].textures[0]);
	}
}

void AnimSpriteComponent::SetCurrentAnimationManual(int frameIndex, int animationIndex)
{
	mCurrFrame = 0.0f;
	mIsPlaying = true;
	SetTexture(mAnimations[animationIndex].textures[frameIndex]);
}

void AnimSpriteComponent::SetCurrentAnimationLoop(AnimSpriteComponent* sprite, int animationIndex, int endIndex)
{
	mCurrFrame = 0.0f;
	mIsPlaying = true;
	for (int i = 0; i < endIndex; i++)
	{
		
		SetTexture(mAnimations[animationIndex].textures[i]);
		std::cout << i << std::endl;
	}
	sprite->SetLockControl(true);
}