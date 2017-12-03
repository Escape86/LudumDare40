#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "AreaTrigger.h"
#include "Display.h"
#include "Constants.h"

#pragma region Constructor

Game::Game()
{
	this->player = new Player();

	this->enemies.push_back(new Enemy(10, 100, 10, 100, ElementType::PLANT));
	this->enemies.push_back(new Enemy(10, 200, 10, 200, ElementType::WATER));
	this->enemies.push_back(new Enemy(10, 300, 10, 300, ElementType::FIRE));
	this->enemies.push_back(new Enemy(10, 400, 10, 400, ElementType::PLANT));
	this->enemies.push_back(new Enemy(10, 450, 10, 450, ElementType::WATER));
	this->enemies.push_back(new Enemy(10, 500, 10, 500, ElementType::FIRE));
	this->enemies.push_back(new Enemy(10, 550, 10, 550, ElementType::PLANT));

	this->enemies.push_back(new Enemy(500, 100, 500, 100, ElementType::FIRE));
	this->enemies.push_back(new Enemy(500, 200, 500, 200, ElementType::WATER));
	this->enemies.push_back(new Enemy(500, 300, 500, 300, ElementType::PLANT));
	this->enemies.push_back(new Enemy(500, 400, 500, 400, ElementType::FIRE));
	this->enemies.push_back(new Enemy(500, 450, 500, 450, ElementType::WATER));
	this->enemies.push_back(new Enemy(500, 500, 500, 500, ElementType::PLANT));
	this->enemies.push_back(new Enemy(500, 550, 500, 550, ElementType::WATER));

	this->areaTriggers.push_back(new AreaTrigger(250, 300, ElementType::WATER));
	this->areaTriggers.push_back(new AreaTrigger(250, 400, ElementType::FIRE));
	this->areaTriggers.push_back(new AreaTrigger(250, 500, ElementType::PLANT));

	playerElementStrengthTextId = Display::CreateText("x0", SCREEN_WIDTH - 25, 0, BLACK);
	playerHpTextId = Display::CreateText("100%", 0, 0, BLACK);
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
	for (std::vector<Enemy*>::iterator it = this->enemies.begin(); it != this->enemies.end();)
	{
		bool enemyCollision = false;

		Enemy* enemy = *it;
		if (this->player->TestCollision(enemy))
		{
			enemyCollision = true;
			this->player->HandleElementCollision(enemy->GetElementType());
		}

		for (AreaTrigger* const at : this->areaTriggers)
		{
			if (at->TestCollision(enemy))
			{
				//enemy reach an area trigger
				this->player->SetHp(this->player->GetHp() - 1);
				enemyCollision = true;
			}
		}

		if (enemyCollision)
		{
			it = this->enemies.erase(it);
		}
		else
		{
			++it;
		}
	}

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

	//update text controls
	char hpText[8];
	sprintf_s(hpText, "%d%%", this->player->GetHp());
	Display::UpdateText(this->playerHpTextId, hpText);

	char elementStrengthText[8];
	sprintf_s(elementStrengthText, "x%d", this->player->GetElementStrength());
	Display::UpdateText(this->playerElementStrengthTextId, elementStrengthText);
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
