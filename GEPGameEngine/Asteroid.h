#pragma once

#include "SpriteEx.h"
using namespace std;

class Asteroid : public SpriteEx
{
private:
	float rotationSpeed;
	int size;
	int ptsVal;
	int health;
	bool isVoid;

public:
	Asteroid(SDL_Texture* tex, SDL_Rect srcRect, SDL_Rect destRect, float rotSpeed, double speed, double dir_x, double dir_y, int size)
		: SpriteEx(tex, srcRect, destRect)
	{
		switch (size) {
		case 1:
			this->ptsVal = 10;
			this->health = 1;
			this->size = 1;

			break;

		case 2:
			this->ptsVal = 25;
			this->health = 2;
			this->size = 2;

			break;

		case 3:
			this->ptsVal = 50;
			this->health = 3;
			this->size = 3;

			break;

		}

		isVoid = true;
		m_dSpeed = speed;
		m_DY = dir_y;
		m_DX = dir_x;
		rotationSpeed = rotSpeed;
		m_X = destRect.x;
		m_Y = destRect.y;
		m_dRadius = destRect.w / 2;
	}

	int getPtsVal() { return ptsVal; }
	int getSize() { return size; }

	void setVoidState(bool b) {
		isVoid = b;
	}
	bool getVoidState() {
		return isVoid;
	}

	bool takeDamage() {
		if (health > 0) {
			health -= 1;
			cout << "Asteroid took damage\n";
			if (health == 0) {
				cout << "Asteroid destroyed\n";

				return false;
			}
		}

		return true;
	}

	void Update()
	{
		angle += rotationSpeed;
		m_X += m_dSpeed * m_DX;
		m_Y += m_dSpeed * m_DY;
		spriteDestRect.y = m_X;
		spriteDestRect.x = m_Y;

		if (m_X < 0) m_X += (1500 + spriteDestRect.w);
		if (m_X >= (1500 + spriteDestRect.w)) m_X -= (1500 + spriteDestRect.w);
		if (m_Y < 0) m_Y += (1000 + spriteDestRect.h);
		if (m_Y >= (1000 + spriteDestRect.h)) m_Y -= (1000 + spriteDestRect.h);
	}

};