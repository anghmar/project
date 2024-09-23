#pragma once
#include <string>
#include "Math.h"

class SoundEvent 
{
public:
	SoundEvent();
	//Returns true if associated FMOD event instance exists
	bool IsValid();
	//Restart and Stop event 
	void Restart();
	void Stop(bool allowFadeOut = true);
	//Setters
	void SetPaused(bool pause);
	void SetVolume(float value);
	void SetPitch(float value);
	void SetParameter(const std::string& name, float value);

	//Getters
	bool GetPaused() const;
	float GetVolume() const;
	float GetPitch() const;
	float GetParameter(const std::string& name);

protected:
	//Make this constructor protected and AudioSystem friendly so AudioSystem can access this constructor
	friend class AudioSystem;
	SoundEvent(class AudioSystem* system, unsigned int id);

private:
	class AudioSystem* mSystem;
	unsigned int mID;
};