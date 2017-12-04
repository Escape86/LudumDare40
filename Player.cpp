#include "Player.h"
#include "Texture.h"
#include "Display.h"
#include "Audio.h"
#include "Constants.h"

const int OverChargeTimerDuration = 1000;

#pragma region Constructor

Player::Player() : Object((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), BLACK_DOT_TEXTURE_PATH, ElementType::NONE)
{
	this->horizontalVelocity = 0;
	this->verticalVelocity = 0;

	this->orbCount = 0;
	this->hp = 100;
	this->isOvercharged = false;
	this->overchargeTimer = 0;

	this->overlayTexture = new Texture(PLAYER_OVERLAY_TEXTURE_PATH);
	this->overlayTexture->Load();

	//TODO: pulling this from texture size is probably not ideal
	this->width = this->overlayTexture->GetWidth();
	this->height = this->overlayTexture->GetHeight();

	//prevent level switching from causing keyups to occur without a corresponding keydown
	this->keydownPrimed = false;
}

#pragma endregion

#pragma region Public Methods

Player::~Player()
{
	if (this->overlayTexture)
	{
		delete this->overlayTexture;
	}
}

void Player::Draw()
{
	//draw the standard orb
	Object::Draw();

	//draw the custom overlay to help the player standout from other orbs
	Display::QueueTextureForRendering(this->overlayTexture, this->x, this->y);
}

void Player::InjectFrame(unsigned int elapsedGameTime, unsigned int previousFrameTime)
{
	//update position
	this->x += this->horizontalVelocity;
	this->y += this->verticalVelocity;

	//enforce screen bounds
	int halfWidth = this->width / 2;
	int halfHeight = this->height / 2;

	if (this->x - halfWidth < 0)
	{
		this->x = halfWidth;
	}
	else if (this->x + halfWidth > SCREEN_WIDTH)
	{
		this->x = SCREEN_WIDTH - halfWidth;
	}

	if (this->y - halfHeight < 0)
	{
		this->y = halfHeight;
	}
	else if (this->y + halfHeight > SCREEN_HEIGHT)
	{
		this->y = SCREEN_HEIGHT - halfHeight;
	}

	//are we overcharged?
	if (this->isOvercharged)
	{
		//are we still under overcharged conditions?
		if (this->orbCount > this->maxOrbCount)
		{
			//yes, so decrement timer and/or punish player
			this->overchargeTimer -= previousFrameTime;

			if (this->overchargeTimer <= 0)
			{
				this->hp -= (this->orbCount - this->maxOrbCount);
				this->overchargeTimer = OverChargeTimerDuration;
			}
		}
		else
		{
			//cleared!
			this->isOvercharged = false;
			this->overchargeTimer = 0;
		}
	}
	else
	{
		//check if we should be considered overcharged
		if (this->orbCount > this->maxOrbCount)
		{
			this->isOvercharged = true;
			this->overchargeTimer = OverChargeTimerDuration;
		}
	}
}

void Player::OnKeyDown(int key)
{
	switch (key)
	{
		case SDLK_w:
		case SDLK_UP:
			this->verticalVelocity -= PLAYER_VELOCITY;
			break;
		case SDLK_s:
		case SDLK_DOWN:
			this->verticalVelocity += PLAYER_VELOCITY;
			break;
		case SDLK_a:
		case SDLK_LEFT:
			this->horizontalVelocity -= PLAYER_VELOCITY;
			break;
		case SDLK_d:
		case SDLK_RIGHT:
			this->horizontalVelocity += PLAYER_VELOCITY;
			break;
	}

	//enforce velocity min/max values
	if (this->verticalVelocity > PLAYER_VELOCITY)
		this->verticalVelocity = PLAYER_VELOCITY;
	else if (this->verticalVelocity < -PLAYER_VELOCITY)
		this->verticalVelocity = -PLAYER_VELOCITY;

	if (this->horizontalVelocity > PLAYER_VELOCITY)
		this->horizontalVelocity = PLAYER_VELOCITY;
	else if (this->horizontalVelocity < -PLAYER_VELOCITY)
		this->horizontalVelocity = -PLAYER_VELOCITY;

	this->keydownPrimed = true;
}

void Player::OnKeyUp(int key)
{
	if (!this->keydownPrimed)
		return;

	switch (key)
	{
		case SDLK_w:
		case SDLK_UP:
			this->verticalVelocity += PLAYER_VELOCITY;
			break;
		case SDLK_s:
		case SDLK_DOWN:
			this->verticalVelocity -= PLAYER_VELOCITY;
			break;
		case SDLK_a:
		case SDLK_LEFT:
			this->horizontalVelocity += PLAYER_VELOCITY;
			break;
		case SDLK_d:
		case SDLK_RIGHT:
			this->horizontalVelocity -= PLAYER_VELOCITY;
			break;
	}

	//enforce velocity min/max values
	if (this->verticalVelocity > PLAYER_VELOCITY)
		this->verticalVelocity = PLAYER_VELOCITY;
	else if (this->verticalVelocity < -PLAYER_VELOCITY)
		this->verticalVelocity = -PLAYER_VELOCITY;

	if (this->horizontalVelocity > PLAYER_VELOCITY)
		this->horizontalVelocity = PLAYER_VELOCITY;
	else if (this->horizontalVelocity < -PLAYER_VELOCITY)
		this->horizontalVelocity = -PLAYER_VELOCITY;
}

void Player::ResetHorizontalVelocity()
{
	this->horizontalVelocity = 0;
}

void Player::ResetVerticalVelocity()
{
	this->verticalVelocity = 0;
}

void Player::HandleOrbCollision(ElementType typeFromCollision)
{
	//did we collide with our current element?
	if (this->type == typeFromCollision)
	{
		this->orbCount++;
		Audio::PlayAudio(Audio::AudioTracks::COLLECT_ORB);
		return;
	}

	//are we neutral?
	if (this->orbCount == 0)
	{
		this->type = typeFromCollision;
		this->orbCount = 1;
		Audio::PlayAudio(Audio::AudioTracks::COLLECT_ORB);

		//destroy old texture and replace with new one that matches our new element type
		this->SetTexture(ELEMENTTYPE_TO_DOT_TEXTURE[this->type]);
		this->width = this->overlayTexture->GetWidth();
		this->height = this->overlayTexture->GetHeight();

		return;
	}

	//check if we collided with our element's weakness
	if (ELEMENTTYPE_WEAKNESS[this->type] == typeFromCollision)
	{
		this->hp -= 10;
		this->orbCount--;
		Audio::PlayAudio(Audio::AudioTracks::DESTROY_ORB);
	}
	//check if we are the weakness of what we collided with
	else if (ELEMENTTYPE_WEAKNESS[typeFromCollision] == this->type)
	{
		this->orbCount--;
		Audio::PlayAudio(Audio::AudioTracks::DESTROY_ORB);
	}

	//did we become neutral as a result?
	if (this->orbCount == 0)
	{
		this->type = ElementType::NONE;

		//destroy old texture and replace with new one that matches our new element type
		this->SetTexture(ELEMENTTYPE_TO_DOT_TEXTURE[this->type]);
		this->width = this->overlayTexture->GetWidth();
		this->height = this->overlayTexture->GetHeight();
	}
}

void Player::HandleShrineCollision(ElementType shrineType)
{
	//are we neutral?
	if ((this->orbCount == 0) && (this->type == ElementType::NONE))
	{
		//nothing to do
		return;
	}

	//did we collide with our current element?
	if (this->type == shrineType)
	{
		this->orbCount = 0;
	}

	//did we become neutral as a result?
	if (this->orbCount == 0)
	{
		this->type = ElementType::NONE;

		//destroy old texture and replace with new one that matches our new element type
		this->SetTexture(ELEMENTTYPE_TO_DOT_TEXTURE[this->type]);
		this->width = this->overlayTexture->GetWidth();
		this->height = this->overlayTexture->GetHeight();
	}
}

int Player::GetOrbCount()
{
	return this->orbCount;
}

void Player::SetOrbCount(int value, ElementType type)
{
	this->type = type;
	this->orbCount = value;

	//destroy old texture and replace with new one that matches our new element type
	this->SetTexture(ELEMENTTYPE_TO_DOT_TEXTURE[this->type]);
	this->width = this->overlayTexture->GetWidth();
	this->height = this->overlayTexture->GetHeight();
}

int Player::GetMaxOrbCount()
{
	return this->maxOrbCount;
}

void Player::SetMaxOrbCount(int value)
{
	this->maxOrbCount = value;
}

bool Player::GetIsOvercharged()
{
	return this->isOvercharged;
}

int Player::GetHp()
{
	return this->hp;
}

void Player::SetHp(int hp)
{
	this->hp = hp;
}

#pragma endregion
