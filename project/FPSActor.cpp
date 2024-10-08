#include "FPSActor.h"
#include "MoveComponent.h"
#include "SDL_scancode.h"
#include "Renderer.h"
#include "AudioSystem.h"
#include "Game.h"
#include "AudioComponent.h"
#include "FPSCamera.h"
#include "MeshComponent.h"

FPSActor::FPSActor(Game* game)
	:Actor(game)
{
	mMoveComp = new MoveComponent(this);
	mAudioComp = new AudioComponent(this);
	mLastFootstep = 0.0f;
	mFootstep = mAudioComp->PlayEvent("event:/Footstep");
	mFootstep.SetPaused(true);

	mCameraComp = new FPSCamera(this);

	mFPSModel = new Actor(game);
	mFPSModel->SetScale(0.75f);
	//mMeshComp = new MeshComponent(mFPSModel);
	//mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/Rifle.gpmesh"));
}

void FPSActor::UpdateActor(float deltaTime)
{
}

void FPSActor::ActorInput(const uint8_t* keys)
{
}

void FPSActor::SetFootstepSurface(float value)
{
}

void FPSActor::SetVisible(bool visible)
{
}
