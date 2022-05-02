#pragma once

#include "SpriteEx.h"

class PowerUp : public SpriteEx
{
private:
	float rotationSpeed;
	int powerType;
public:
	PowerUp(SDL_Texture* tex, SDL_Rect srcRect, SDL_Rect destRect, float rotSpeed, int type)
		: SpriteEx(tex, srcRect, destRect)
	{
		rotationSpeed = rotSpeed;
		m_X = destRect.x;
		m_Y = destRect.y;
		m_dRadius = destRect.w * 0.5;
		powerType = type;
	}

	int getType() { return powerType; }

	void Update()
	{
		angle += rotationSpeed;
	}


};