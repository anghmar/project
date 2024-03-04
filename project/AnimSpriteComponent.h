#pragma once
#include "SpriteComponent.h"
#include <vector>

class AnimSpriteComponent : public SpriteComponent
{
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	//Update animation every frame (override from component)
	void Update(float deltaTime) override;
	//Set textures used for animation
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
	//Set/Get the animation FPS
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }

private:
	//All textures in the animation
	std::vector<SDL_Texture*> mAnimTextures;
	//Current frame displayed
	float mCurrFrame;
	//Animation Frame Rate
	float mAnimFPS;
};