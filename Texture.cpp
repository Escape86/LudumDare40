#include "Texture.h"
#include "Display.h"
#include "SDL_image.h"

#ifdef _DEBUG
	#include <assert.h>
#endif

#pragma region Constructor

Texture::Texture(const std::string path)
{
	this->path = path;
	this->isLoaded = false;
}

#pragma endregion

#pragma region Public Methods

Texture::~Texture()
{
	//Free texture if it exists
	if (this->sdl_texture != nullptr)
	{
		SDL_DestroyTexture(this->sdl_texture);
		this->sdl_texture = nullptr;
		this->width = 0;
		this->height = 0;
	}
}

bool Texture::Load()
{
	if (this->isLoaded)
		return true;

	//clear any texture we already have loaded
	if (this->sdl_texture)
	{
		SDL_DestroyTexture(this->sdl_texture);
		this->sdl_texture = nullptr;
		this->width = 0;
		this->height = 0;
	}

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		return false;
	}

	//Color key image
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

	//Create texture from surface pixels
	this->sdl_texture = SDL_CreateTextureFromSurface(Display::GetRenderer(), loadedSurface);
	if (this->sdl_texture == nullptr)
	{
		printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		return false;
	}

	//Get image dimensions
	this->width = loadedSurface->w;
	this->height = loadedSurface->h;

	//Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);

	this->isLoaded = true;

	return true;
}

void Texture::Draw(int x, int y, SDL_Rect* clip /*= nullptr*/, double angle /*= 0.0*/, SDL_Point* center /*= nullptr*/, SDL_RendererFlip flip /*= SDL_FLIP_NONE*/)
{
	assert(this->isLoaded);

	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, this->width, this->height };

	//Set clip rendering dimensions
	if (clip != nullptr)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(Display::GetRenderer(), this->sdl_texture, clip, &renderQuad, angle, center, flip);
}

#pragma endregion