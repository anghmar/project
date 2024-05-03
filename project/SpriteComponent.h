#pragma once
#include "Component.h"
#include "SDL.h"
#include "Shader.h"

class SpriteComponent : public Component
{
public:
	// Lower drawk order corresponds with further back
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();
	virtual void Draw(Shader* shader);
	virtual void SetTexture(SDL_Texture* texture);

	int GetDrawOrder() const { return mDrawOrder; }
	int GetTextureHeight() const { return mTextureHeight; }
	int GetTextureWidth() const { return mTextureWidth; }

protected:
	// Texture to draw
	SDL_Texture* mTexture;

	// Draw order used for painters algorithm
	int mDrawOrder;
	int mTextureHeight;
	int mTextureWidth;
};