#pragma once
#include "Player.h"
#include "Asteroid.h"
#include "SDL_mixer.h"
#include "PowerUp.h"
#include "ScreenState.h"
#include "Timer.h"
#include "Game.h"
#include <vector>
#include <iostream>

using namespace std;

class GameState : public ScreenState
{
private:
	bool GameOver;
	SDL_Texture* mainSpriteTex;
	SDL_Texture* bgSpriteTex;

	SDL_Rect bgSrcRect;
	SDL_Rect bgDestRect;

	SDL_Texture* lifePtsSpriteTex;
	SDL_Texture* hbarSpriteTex;

	SpriteEx* bg;

	Player* player;

	vector<Asteroid*> asteroids;
	vector<SDL_Texture*> astTex;
	vector<PowerUp*> powerUps;
	SDL_Texture* pwrTex;

	SDL_Color ptsFontColor;

	Mix_Chunk* powerUpSound;
	Mix_Chunk* playerDiedSound;
	Mix_Chunk* gameOverSound;
	Mix_Chunk* bgMusic;
	Mix_Chunk* asteroidBroke;
	Mix_Chunk* astHitSound;
	Mix_Chunk* playerHitSound;
	Mix_Chunk* fireSound;
	Mix_Chunk* engineSound;

public:
	GameState() {}
	void Update();
	void Render();
	void Enter();
	void Exit();
	void Resume() { cout << "Resuming Game.." << endl; }

	bool checkEndGame();
	void SpawnPowerUp();
	void SpawnAsteroid();
	void RenderHealthBar(bool isRefresh, int x, int y, int health);
	void RenderLifePoints(bool refresh, int x, int y, int lives);
	void playerRespawn(Player* p);
	Player* getPlayer();
    static GameState* Instance();

	bool CircleCollisionTest(double x1, double y1, double x2, double y2, double r1, double r2)
	{
		return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) < (r1 + r2));

	}

	void CheckCollision();

	bool RectCollisionTest(SDL_Rect* a, SDL_Rect* b) {

		int leftA, leftB;
		int rightA, rightB;
		int topA, topB;
		int bottomA, bottomB;

		leftA = a->x;
		rightA = a->x + a->w;
		topA = a->y;
		bottomA = a->y + a->h;

		leftB = b->x;
		rightB = b->x + b->w;
		topB = b->y;
		bottomB = b->y + b->h;

		if (bottomA <= topB)
		{
			return false;
		}

		if (topA >= bottomB)
		{
			return false;
		}

		if (rightA <= leftB)
		{
			return false;
		}

		if (leftA >= rightB)
		{
			return false;
		}

		return true;
	}
};








