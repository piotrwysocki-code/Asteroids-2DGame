#pragma once
#include "SDL.h"
#include <iostream>

class SpriteEx
{
protected:
	SDL_Rect spriteSrcRect;
	SDL_Rect spriteDestRect;
	SDL_Texture* texture;
	double angle;
	double m_X,
		m_Y,
		m_DX,
		m_DY,
		m_dSpeed;

	double m_dRadius;

public:
	SpriteEx() {}
	SpriteEx(SDL_Texture* tex, SDL_Rect srcRect, SDL_Rect dstRect)
	{
		texture = tex;
		spriteSrcRect = srcRect;
		spriteDestRect = dstRect;
	}

	SDL_Rect* getDestRect() {
		return &spriteDestRect;
	}

	void setDestRectX(int x) {
		spriteDestRect.x = x;
	}

	void setDestRectY(int y) {
		spriteDestRect.y = y;
	}

	void Render();
	void UpdateDestRect();

	double GetRadius() { return m_dRadius; }
	double GetX() { return m_X; }
	double GetY() { return m_Y; }
};


class SpriteExAnimated : public SpriteEx
{
protected:
	int m_iSprite = 0,
		m_iSpriteMax,
		m_iFrame = 0,
		m_iFrameMax;

public:
	void Animate();

	SpriteExAnimated(SDL_Texture* tex, double x, double y,
		double a, int frameMax, int spriteMax, double speed);

};