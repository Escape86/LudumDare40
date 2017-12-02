#pragma once

#include "SDL.h"
#include <string>

class Texture
{
public:
	Texture(const std::string path);
	~Texture();

	bool Load();
	void Draw(int x, int y, SDL_Rect* clip = nullptr, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

private:
	bool isLoaded;
	int width;
	int height;
	std::string path;

	SDL_Texture* sdl_texture;
};