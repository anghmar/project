#include "Component.h"
#include "Math.h"

class MoveComponent : public Component
{
public:
	//Lower udpate order to update first
	MoveComponent(class Actor* owner, int updateOrder = 10);

	void Update(float deltaTime) override;

	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }

private:
	//Rotation in radians/speed
	float mAngularSpeed;
	//Forward vector movement (units/second)
	float mForwardSpeed;
};