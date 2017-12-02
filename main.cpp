#include "Display.h"
#include "Game.h"

int main(int argc, char* args[])
{
	if (!Display::Initialize())
	{
		return -1;
	}

	Game* game = new Game();

	bool keepRunning = true;

	Display::SetEventCallback([&keepRunning, &game](SDL_Event e)
	{
		switch (e.type)
		{
			case SDL_KEYDOWN:
				if (e.key.repeat == 0)
					game->InjectKeyDown((int)e.key.keysym.sym);
				break;
			case SDL_KEYUP:
				if (e.key.repeat == 0)
					game->InjectKeyUp((int)e.key.keysym.sym);
				break;
			case SDL_QUIT:
				keepRunning = false;
			break;
		}
	});

	//game loop
	while (keepRunning)
	{
		//update game objects
		game->InjectFrame();

		//draw the frame
		Display::InjectFrame();
	}

	delete game;

	if (!Display::ShutDown())
	{
		return -1;
	}

	return 0;
}