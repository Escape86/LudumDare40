#include "Display.h"
#include "Texture.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const char* fontPath = "resources/lazy.ttf";
const int fontSize = 28;

#pragma region Public Methods

bool Display::Initialize()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		printf("Warning: Linear texture filtering not enabled!");
	}

	//Create window
	Display::window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Create vsynced renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}

	//Initialize Font
	Display::font = TTF_OpenFont(fontPath, fontSize);
	if (Display::font == nullptr)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}

	//everything initialized correctly!
	return true;
}

bool Display::ShutDown()
{
	//Free loaded images
	for (auto it = Display::textureQueue.begin(); it != Display::textureQueue.end();)
	{
		delete (it->texture);
		it = textureQueue.erase(it);
	}

	//Free font
	TTF_CloseFont(Display::font);
	Display::font = nullptr;

	//Destroy window	
	SDL_DestroyRenderer(Display::renderer);
	SDL_DestroyWindow(Display::window);
	Display::window = nullptr;
	Display::renderer = nullptr;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();

	return true;
}

void Display::InjectFrame()
{
	//Handle events on queue
	SDL_Event eventHandler;
	while (SDL_PollEvent(&eventHandler) != 0)
	{
		//publish events
		if (Display::eventCallback != nullptr)
		{
			Display::eventCallback(eventHandler);
		}
	}

	//Move the dot and check collision
	/*dot.move(wall);*/

	//Clear screen
	SDL_SetRenderDrawColor(Display::renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(Display::renderer);

	//Render thing
	//SDL_Rect thing;
	//thing.x = 300;
	//thing.y = 40;
	//thing.w = 40;
	//thing.h = 400;

	SDL_SetRenderDrawColor(Display::renderer, 0x00, 0x00, 0x00, 0xFF);
	//SDL_RenderDrawRect(Display::renderer, &thing);

	//Render dot
	//dot.render();

	//draw our queued up textures
	for (std::vector<QueuedTexture>::iterator it = Display::textureQueue.begin(); it != Display::textureQueue.end(); ++it)
	{
		Texture* t = it->texture;
		t->Draw(it->x, it->y);
	}

	Display::textureQueue.clear();


	Display::textQueue.push_back(QueuedText { 0, 0, "hello world", { 0, 0, 0 } });

	for (std::vector<QueuedText>::iterator it = Display::textQueue.begin(); it != Display::textQueue.end(); ++it)
	{
		Texture* t = Texture::CreateFromText(it->text);
		
		if(t)
			t->Draw(it->x, it->y);

		delete t;
	}

	Display::textQueue.clear();
	
	//Update screen
	SDL_RenderPresent(Display::renderer);
}

void Display::SetEventCallback(std::function<void(SDL_Event e)> eventCallback)
{
	Display::eventCallback = eventCallback;
}

SDL_Renderer* const Display::GetRenderer()
{
	return Display::renderer;
}

void Display::QueueTextureForRendering(Texture* const texture, int x, int y)
{
	Display::textureQueue.push_back({ texture, x, y });
}

int Display::GetScreenWidth()
{
	return SCREEN_WIDTH;
}

int Display::GetScreenHeight()
{
	return SCREEN_HEIGHT;
}

TTF_Font* const Display::GetFont()
{
	return Display::font;
}

#pragma endregion

#pragma region Static Member Initialization

SDL_Window* Display::window = nullptr;
SDL_Renderer* Display::renderer = nullptr;
TTF_Font* Display::font;
std::function<void(SDL_Event e)> Display::eventCallback;
std::vector<Display::QueuedTexture> Display::textureQueue;
std::vector<Display::QueuedText> Display::textQueue;

#pragma endregion