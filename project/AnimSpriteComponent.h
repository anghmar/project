#pragma once
#include "SpriteComponent.h"
#include <vector>

class AnimSpriteComponent : public SpriteComponent
{
public:

	struct Animation
	{
		std::vector<SDL_Texture*> textures;
		int endIndex;
		bool looping;

		Animation(const std::vector<SDL_Texture*>& _textures, int _endIndex, bool _looping)
			: textures(_textures), endIndex(_endIndex), looping(_looping) {}
	};

	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	//Update animation every frame (override from component)
	void Update(float deltaTime) override;
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
	void SetCurrentAnimationManual(int frameIndex, int animationIndex);
	void SetCurrentAnimationLoop(AnimSpriteComponent* sprite, int animationIndex, int endIndex);


	void SetAnimations(const std::vector<Animation>& animations);
	void SetCurrentAnimation(int animationIndex);
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
	//bool isAnimationLooping() { return currentAnimation.looping; }
	bool isAnimationPlaying() { return mIsPlaying; }
	bool GetLockControl() { return lockControl; }
	void SetLockControl(bool lock) { lockControl = lock; }

private:
	//All textures in the animation
	std::vector<SDL_Texture*> mAnimTextures; //it will probably get deleted..

	std::vector<Animation> mAnimations;
	//Animation& currentAnimation;
	int mCurrentAnimationIndex;
	float mCurrFrame;
	float mAnimFPS;
	bool mIsPlaying;
	bool lockControl = true;
};