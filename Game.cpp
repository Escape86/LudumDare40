#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "AreaTrigger.h"
#include "Display.h"
#include "Constants.h"

int debug_enemy_collision_text_id = -1;
int debug_areatrigger_collision_text_id = -1;

#pragma region Constructor

Game::Game()
{
	this->player = new Player();

	this->enemies.push_back(new Enemy(10, 25));

	this->areaTriggers.push_back(new AreaTrigger(500, 500));

	debug_enemy_collision_text_id = Display::CreateText("Player Collision", 0, 0);
	Display::SetTextIsVisible(debug_enemy_collision_text_id, false);

	debug_areatrigger_collision_text_id = Display::CreateText("AreaTrigger Collision", 150, 0);
	Display::SetTextIsVisible(debug_areatrigger_collision_text_id, false);
}

#pragma endregion

#pragma region Public Methods

Game::~Game()
{
	if (this->player)
	{
		delete this->player;
	}

	for (Enemy* e : this->enemies)
	{
		delete e;
	}
	this->enemies.clear();

	for (AreaTrigger* at : this->areaTriggers)
	{
		delete at;
	}
	this->areaTriggers.clear();
}

void Game::InjectFrame()
{
	for (Enemy* const enemy : this->enemies)
	{
		enemy->InjectFrame();
	}

	this->player->InjectFrame();

	//now that movements are updated check for collisions
	bool enemyCollision = false;
	for (Enemy* const enemy : this->enemies)
	{
		if (this->player->TestCollision(enemy))
		{
			enemyCollision = true;
			break;
		}
	}
	Display::SetTextIsVisible(debug_enemy_collision_text_id, enemyCollision);

	bool areaTriggerCollision = false;
	for (AreaTrigger* const at : this->areaTriggers)
	{
		if (this->player->TestCollision(at))
		{
			areaTriggerCollision = true;
			break;
		}
	}
	Display::SetTextIsVisible(debug_areatrigger_collision_text_id, areaTriggerCollision);

	//draw area triggers
	for (AreaTrigger* const at : this->areaTriggers)
	{
		at->Draw();
	}

	//draw enemies
	for (Enemy* const enemy : this->enemies)
	{
		enemy->Draw();
	}

	//draw player
	this->player->Draw();
}

void Game::InjectKeyDown(int key)
{
	this->player->OnKeyDown(key);
}

void Game::InjectKeyUp(int key)
{
	this->player->OnKeyUp(key);
}

void Game::InjectControllerStickMovement(uint8_t axis, int16_t value)
{
	//X axis motion
	if (axis == 0)
	{
		//Left of dead zone
		if (value < -JOYSTICK_DEAD_ZONE)
		{
			//fake a keypress for the left direction
			this->player->OnKeyDown(SDLK_LEFT);
		}
		//Right of dead zone
		else if (value > JOYSTICK_DEAD_ZONE)
		{
			//fake a keypress for the right direction
			this->player->OnKeyDown(SDLK_RIGHT);
		}
		else
		{
			this->player->ResetHorizontalVelocity();
		}
	}
	//Y axis motion
	else if (axis == 1)
	{
		//Above of dead zone
		if (value < -JOYSTICK_DEAD_ZONE)
		{
			//fake a keypress for the up direction
			this->player->OnKeyDown(SDLK_UP);
		}
		//Below of dead zone
		else if (value > JOYSTICK_DEAD_ZONE)
		{
			//fake a keypress for the down direction
			this->player->OnKeyDown(SDLK_DOWN);
		}
		else
		{
			this->player->ResetVerticalVelocity();
		}
	}
}

#pragma endregion
