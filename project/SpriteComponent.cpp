#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Math.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	:Component(owner)
	, mTexture(nullptr)
	, mDrawOrder(drawOrder)
	, mTextureWidth(0)
	, mTextureHeight(0)
{
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(Shader* shader)
{
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	//if (mTexture)
	//{
	//	SDL_Rect r;
	//	// Scale the width/height by owner's scale
	//	r.w = static_cast<int>(mTextureWidth * mOwner->GetScale());
	//	r.h = static_cast<int>(mTextureHeight * mOwner->GetScale());
	//	// Center the rectangle around the position of the owner
	//	r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
	//	r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);

	//// Draw
	//SDL_RenderCopyEx(renderer,
	//	mTexture, // Texture to draw
	//	nullptr, // Source rectangle
	//	&r, // Destination rectangle
	//	-Math::ToDegrees(mOwner->GetRotation()), // (Convert angle)
	//	nullptr, // Point of rotation
	//	SDL_FLIP_NONE); // Flip behavior
	//}
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	mTexture = texture;
	// Set width/height
	SDL_QueryTexture(texture, nullptr, nullptr, &mTextureWidth, &mTextureHeight);
}
