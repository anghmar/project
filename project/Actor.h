#pragma once
#include <vector>
#include "Game.h"
#include "Math.h"

class Actor
{
public:
	//To track state of actor
	enum State
	{
		EActive,
		EPaused,
		EDead
	};
	Actor(class Game* game);
	virtual ~Actor();

	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime);

	//getters
	class Game* GetGame() { return mGame; }
	State GetState() { return mState; }
	const Vector2& GetPosition() const { return mPosition; }
	float GetScale() const { return mScale; }
	float GetRotation() const { return mRotation; }
	//setters
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	void SetScale(float scale) { mScale = scale; }
	void SetRotation(float rotation) { mRotation = rotation; }
	void SetState(State state) { mState = state; }

	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);
private:
	State mState;
	Vector2 mPosition;
	float mScale;
	float mRotation;
	std::vector<class Component*> mComponents;
	class Game* mGame;
};