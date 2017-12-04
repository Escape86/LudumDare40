#pragma once

#include "Object.h"

class Player : public Object
{
public:
	Player();
	~Player();

	void Draw() override;

	void InjectFrame(unsigned int elapsedGameTime, unsigned int previousFrameTime) override;
	void OnKeyDown(int key);
	void OnKeyUp(int key);

	void ResetHorizontalVelocity();
	void ResetVerticalVelocity();

	void HandleOrbCollision(ElementType typeFromCollision);
	void HandleShrineCollision(ElementType shrineType);
	int GetOrbCount();
	void SetOrbCount(int value, ElementType type);
	int GetMaxOrbCount();
	void SetMaxOrbCount(int value);
	bool GetIsOvercharged();

	int GetHp();
	void SetHp(int hp);

private:
	int horizontalVelocity;
	int verticalVelocity;

	int orbCount;
	int maxOrbCount;
	int hp;

	bool isOvercharged;
	int overchargeTimer;

	bool keydownPrimed;

	Texture* overlayTexture;
};