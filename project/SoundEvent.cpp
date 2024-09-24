#include "SoundEvent.h"
#include "AudioSystem.h"
#include <fmod_studio.hpp>

SoundEvent::SoundEvent(class AudioSystem* system, unsigned int id)
	:mSystem(system)
	,mID(id)
{
}

SoundEvent::SoundEvent()
	:mSystem(nullptr)
	, mID(0)
{
}

bool SoundEvent::IsValid()
{
	return (mSystem && mSystem->GetEventInstance(mID) != nullptr);
}

void SoundEvent::Restart()
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		event->start();
	}
}

void SoundEvent::Stop(bool allowFadeOut)
{
}

void SoundEvent::SetPaused(bool pause)
{
}

void SoundEvent::SetVolume(float value)
{
}

void SoundEvent::SetPitch(float value)
{
}

void SoundEvent::SetParameter(const std::string& name, float value)
{
}

bool SoundEvent::GetPaused() const
{
	return false;
}

float SoundEvent::GetVolume() const
{
	return 0.0f;
}

float SoundEvent::GetPitch() const
{
	return 0.0f;
}

float SoundEvent::GetParameter(const std::string& name)
{
	return 0.0f;
}
