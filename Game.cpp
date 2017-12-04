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
	this->playerOrbCountTextId = -1;
	this->playerHpTextId = -1;
	this->previousFrameEndTime = 0;

	//setup first level
	this->LoadLevel(1);
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
	if (this->currentLevel->ShouldAdvanceLevel())
	{
		this->LoadLevel(this->currentLevel->GetLevelNumber() + 1);
		return;
	}

	if (this->player)
	{
		//check health for gameover condition
		const int hp = this->player->GetHp();
		if (hp <= 0)
		{
			//game over!
			this->LoadLevel(GAMEOVER_LEVEL_ID);
			return;
		}
	}

	Uint32 elapsedTimeInMilliseconds = SDL_GetTicks();

	this->currentLevel->InjectFrame(elapsedTimeInMilliseconds - this->currentLevel->GetLevelLoadTime());

	std::vector<AreaTrigger*>& shrines = this->currentLevel->GetShrines();
	std::vector<Enemy*>& enemies = this->currentLevel->GetEnemies();

	for (Enemy* const enemy : enemies)
	{
		enemy->InjectFrame(elapsedTimeInMilliseconds, elapsedTimeInMilliseconds - this->previousFrameEndTime);
	}

	if (this->player)
	{
		this->player->InjectFrame(elapsedTimeInMilliseconds, elapsedTimeInMilliseconds - this->previousFrameEndTime);
	}

	//now that movements are updated check for collisions
	for (std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end();)
	{
		bool enemyCollision = false;

		Enemy* enemy = *it;
		if (this->player->TestCollision(enemy))
		{
			enemyCollision = true;
			this->player->HandleOrbCollision(enemy->GetElementType());
			this->currentLevel->InjectPlayerOrbCountChanged(this->player->GetOrbCount());
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

	if (this->player)
	{
		for (AreaTrigger* const shrine : shrines)
		{
			if (this->player->TestCollision(shrine))
			{
				this->player->HandleShrineCollision(shrine->GetElementType());
			}
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

	if (this->player)
	{
		//draw player
		this->player->Draw();

		//update text controls
		const int hp = this->player->GetHp();
		char hpText[8];
		sprintf_s(hpText, "%d%%", hp);
		SDL_Color hpColor = BLACK;
		if (hp <= 20)
		{
			hpColor = RED;
		}
		Display::UpdateText(this->playerHpTextId, hpText, hpColor);

		const int orbCount = this->player->GetOrbCount();
		const int maxOrbCount = this->player->GetMaxOrbCount();
		char orbCountText[8];
		sprintf_s(orbCountText, "x%d/%d", orbCount, maxOrbCount);
		SDL_Color orbCountColor = BLACK;
		if (this->player->GetIsOvercharged())
		{
			orbCountColor = RED;
		}
		int x = SCREEN_WIDTH - 62;
		if (orbCount >= 10)
		{
			if (orbCount > 20)
				x -= 15;	//1's don't take as much space, so non-teens need a bit more
			else
				x -= 8;
		}
		if (maxOrbCount >= 10)
		{
			x -= 8;
		}
		Display::MoveText(this->playerOrbCountTextId, x, 2);
		Display::UpdateText(this->playerOrbCountTextId, orbCountText, orbCountColor);
	}

	this->previousFrameEndTime = elapsedTimeInMilliseconds;
}

void Game::InjectKeyDown(int key)
{
	if(this->player)
		this->player->OnKeyDown(key);
}

void Game::InjectKeyUp(int key)
{
	this->currentLevel->InjectKeyPress();

	if(this->player)
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

void Game::LoadLevel(int levelNumber)
{
	//clear previous level stuff
	if (playerOrbCountTextId >= 0)
	{
		Display::RemoveText(playerOrbCountTextId);
		playerOrbCountTextId = -1;
	}

	if (playerHpTextId >= 0)
	{
		Display::RemoveText(playerHpTextId);
		playerHpTextId = -1;
	}

	delete this->currentLevel;

	if (this->player)
	{
		delete this->player;
		this->player = nullptr;
	}

	//load in new level
	this->currentLevel = Level::Load(levelNumber, SDL_GetTicks());

	if (this->currentLevel == nullptr)
	{
		printf("Failed to load level1!\n");
		return;
	}

	//skip gameplay stuff for level 1 since it's just the titlescreen // TODO: Make a not shit solution for this
	if (levelNumber != 1 && levelNumber != GAMEOVER_LEVEL_ID)
	{
		//create the player object
		this->player = new Player();

		//set player's orb counts
		this->player->SetOrbCount(this->currentLevel->GetNumberStartingOrbsForThisLevel(), this->currentLevel->GetStartingElementTypeThisLevel());
		this->player->SetMaxOrbCount(this->currentLevel->GetOrbCapacityForThisLevel());

		//create text controls for the UI
		playerOrbCountTextId = Display::CreateText("", SCREEN_WIDTH - 62, 2, Display::FontSize::TWENTY, BLACK);
		playerHpTextId = Display::CreateText("", 2, 2, Display::FontSize::TWENTY, BLACK);
	}
}

#pragma endregion
