#pragma once
#include "SDL.h"
#include "SDL_image.h"

class Sprite
{
private:
	SDL_Rect spriteSrcRect;
	SDL_Rect spriteDestRect;
	SDL_Surface* sprite;
	SDL_Texture* texture;

	int lastColumnNum;

	bool isAnimationFinished = false;

	bool isAnimatable = false;
protected:
	int tRows;
	int tColumns;

	int currentRow;
	int currentColumn;
	int initialRow;

	float animSpeed = 10;


public:
	Sprite(const char* spritePath, SDL_Rect srcRect, SDL_Rect spritePosition, SDL_Renderer* renderer
		, int totalRows, int totalColumns
		, int initialRowNum, bool animatable
	);
	~Sprite();
	void Render();
	void Update();
};