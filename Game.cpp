#include "Game.h"
#include "Level.h"
#include "Player.h"
#include "Enemy.h"
#include "AreaTrigger.h"
#include "Display.h"
#include "Constants.h"

#pragma region Constructor

Game::Game()
{
	//create the player object
	this->player = new Player();

	//load level 1
	this->currentLevel = Level::Load(1);

	if (this->currentLevel == nullptr)
	{
		printf("Failed to load level1!\n");
	}

	//create text controls for the UI
	playerElementStrengthTextId = Display::CreateText("x0", SCREEN_WIDTH - 35, 2, BLACK);
	playerHpTextId = Display::CreateText("100%", 2, 2, BLACK);
}

#pragma endregion

#pragma region Public Methods

Game::~Game()
{
	if (this->player)
	{
		delete this->player;
	}
}

void Game::InjectFrame()
{
	Uint32 elapsedTimeInMilliseconds = SDL_GetTicks();
	this->currentLevel->InjectFrame(elapsedTimeInMilliseconds);

	std::vector<AreaTrigger*>& shrines = this->currentLevel->GetShrines();
	std::vector<Enemy*>& enemies = this->currentLevel->GetEnemies();

	for (Enemy* const enemy : enemies)
	{
		enemy->InjectFrame();
	}

	this->player->InjectFrame();

	//now that movements are updated check for collisions
	for (std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end();)
	{
		bool enemyCollision = false;

		Enemy* enemy = *it;
		if (this->player->TestCollision(enemy))
		{
			enemyCollision = true;
			this->player->HandleElementCollision(enemy->GetElementType());
		}

		for (AreaTrigger* const shrine : shrines)
		{
			if (shrine->TestCollision(enemy))
			{
				//enemy reach an area trigger
				this->player->SetHp(this->player->GetHp() - 1);
				enemyCollision = true;
			}
		}

		if (enemyCollision)
		{
			it = enemies.erase(it);
		}
		else
		{
			++it;
		}
	}

	//draw area triggers
	for (AreaTrigger* const at : shrines)
	{
		at->Draw();
	}

	//draw enemies
	for (Enemy* const enemy : enemies)
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

void Game::InjectControllerStickMovement(unsigned char axis, short value)
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
