#pragma once

#include "SpriteEx.h"
#include "Bullet.h"
#include <vector>
using namespace std;


class Player : public SpriteExAnimated
{
private:

	vector<Bullet*> m_vBullets;
	void UpdateBullets();

	double m_velX = 0,
		m_velY = 0,
		m_velMax = 10;

	void MoveForward();
	void UpdatePlayer();
	bool isInvincible;
	bool gunCooldown;
	int lives;
	int health;
	int score;
	int deaths;
	int scoreMulti;
	int rof;

public:
	Player(SDL_Texture* tex, double x, double y, int health, int lives, int sc);
	~Player();


	void Update();
	void Render();
	void SetIdle();
	void SpawnBullet();
	void hasDied();
	bool tookDamage();
	int getLives();
	int getHealth();
	int getScore();
	void addScore(int sc);
	void pickUpPowerUp(int p);
	int getScoreMulti();
	void setScoreMulti(int sc);
	void setInvincible(bool t);
	bool getInvincible();
	void setGunCD(bool b);
	bool getGunCD();
	void setROF(int r);
	bool getROF();

	vector<Bullet*>& GetBullets()
	{
		return m_vBullets;
	}

};