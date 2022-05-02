#include "Player.h"
#include "Game.h"
#include <algorithm> //for min/max functions

Player::Player(SDL_Texture* tex, double x, double y, int hlth, int liv, int sc)
	: SpriteExAnimated(tex, x - 25, y - 25, 0, 1, 4, 0.1f)
{
	isInvincible = false;
	rof = 500;
	deaths = 0;
	score = sc;
	scoreMulti = 1;
	lives = liv;
	health = hlth;
	spriteSrcRect = { 0, 0, 100, 100 };
	spriteDestRect = { (int)(m_X + 25), (int)(m_Y - 25), 50, 50 };
	m_dRadius = spriteDestRect.w / 2;
}

Player::~Player()
{
	for (int i = 0; i < (int)m_vBullets.size(); i++)
	{
		delete m_vBullets[i];
		m_vBullets[i] = nullptr;
	}
	m_vBullets.clear();
	m_vBullets.shrink_to_fit();
}

void Player::Render()
{
	for (int i = 0; i < (int)m_vBullets.size(); i++)
		m_vBullets[i]->Render();

	this->SpriteExAnimated::Render();
}

void Player::SetIdle()
{
	spriteSrcRect.y = 0;
	m_iFrame = 0;
	m_iSprite = 0;
}

void Player::Update()
{
	if (Game::Instance()->KeyDown(SDL_SCANCODE_A))
		angle -= 6.0;
	else if (Game::Instance()->KeyDown(SDL_SCANCODE_D))
		angle += 6.0;
	this->UpdatePlayer();
	this->UpdateBullets();
}

int Player::getLives() {
	return lives;
}

bool Player::tookDamage() {
	if (health != 0) {
		health -= 1;
		if (health == 0) {
			deaths += 1;
			return false;
		}
	}

	return true;
}

void Player::setInvincible(bool t) {
	isInvincible = t;
}

bool Player::getInvincible() {
	return isInvincible;
}

int Player::getHealth() {
	return health;
}

void Player::pickUpPowerUp(int p) {
	switch (p) {
	case 1:
		if (lives < 4) {
			lives += 1;
		}

		break;
	case 2:
		scoreMulti = 2;

		break;

	case 3:
		if (health != 5) {
			health += 1;
		}

		break;
	}
}

void Player::setScoreMulti(int sc) {
	scoreMulti = sc;
}

void Player::addScore(int sc) {
	int temp = sc * scoreMulti;
	score += temp;
}

int Player::getScore() {
	return score;
}

int Player::getScoreMulti() {
	return scoreMulti;
}

void Player::setROF(int r) {
	rof = r;
}

bool Player::getROF() {
	return rof;
}

void Player::setGunCD(bool b) {
	gunCooldown = b;
}

bool Player::getGunCD() {
	return gunCooldown;
}

void Player::MoveForward()
{
	m_DX = (float)cos((angle - 90) * M_PI / 180);
	m_DY = (float)sin((angle - 90) * M_PI / 180);

	m_velX += m_DX * m_dSpeed;
	m_velY += m_DY * m_dSpeed;

	m_velX = min(max(m_velX, -(m_velMax * fabs(m_DX))), (m_velMax * fabs(m_DX)));
	m_velY = min(max(m_velY, -(m_velMax * fabs(m_DY))), (m_velMax * fabs(m_DY)));
}

void Player::UpdatePlayer()
{
	if (Game::Instance()->KeyDown(SDL_SCANCODE_W))
	{
		spriteSrcRect.y = 100;
		/*
		if (&spriteSrcRect != nullptr) {
		}*/
		this->Animate();
		this->MoveForward();
	}
	else
	{
		m_velX *= 0.975;
		m_velY *= 0.975;

	}

	if (m_X < 0) m_X += (1024 + spriteDestRect.w);
	if (m_X >= (1024 + spriteDestRect.w)) m_X -= (1024 + spriteDestRect.w);
	if (m_Y < 0) m_Y += (768 + spriteDestRect.h);
	if (m_Y >= (768 + spriteDestRect.h)) m_Y -= (768 + spriteDestRect.h);

	m_X += m_velX;
	m_Y += m_velY;

	this->UpdateDestRect();

	spriteSrcRect.x = spriteSrcRect.w * m_iSprite;
}

void Player::UpdateBullets()
{
	for (int i = 0; i < (int)m_vBullets.size(); i++)
	{
		m_vBullets[i]->Update();

		if (m_vBullets[i]->m_X < 0 || m_vBullets[i]->m_X >
			1024 ||
			m_vBullets[i]->m_Y < 0 || m_vBullets[i]->m_Y > 768)
		{
			delete m_vBullets[i];
			m_vBullets[i] = nullptr;
			m_vBullets.erase(m_vBullets.begin() + i);
		}

	}

	m_vBullets.shrink_to_fit();
}

void Player::SpawnBullet()
{
	float dx = (float)cos((angle - 90) * M_PI / 180);
	float dy = (float)sin((angle - 90) * M_PI / 180);

	m_vBullets.push_back(new Bullet(texture, m_X, m_Y, angle, dx, dy));
	gunCooldown = true;
}