#pragma once
#include "Component.h"
#include "SoundEvent.h"
#include <string>
#include <vector>


//This component is good only for FPS games, in third-person games the audio would still
//go from/in camera see page 239 in GMCPP

//Doppler Effect -> see 240 in GMCPP
class AudioComponent : public Component
{

public:
	AudioComponent(class Actor* owner, int updateOrder = 200);
	~AudioComponent();

	void Update(float deltaTime) override;
	void OnUpdateWorldTransform() override;

	SoundEvent PlayEvent(const std::string& name);
	void StopAllEvents();

private:
	std::vector<SoundEvent> mEvents2D;
	std::vector<SoundEvent> mEvents3D;
};

