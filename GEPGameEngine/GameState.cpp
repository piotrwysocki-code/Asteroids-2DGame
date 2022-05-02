#include "GameState.h"
#include "Game.h"
#include "PauseMenuPopupState.h"
#include "Asteroid.h"
#include "PowerUp.h"
#include "Timer.h"
#include <vector>
#include <iostream>
#include "FinalState.h"

Uint32 gunCooldownTimer(Uint32 interval, void* player) {
	Player* temp = (Player*)player;
	temp->setGunCD(false);

	return 0;
}

Uint32 gameOverTimeout(Uint32 interval, void* p) {
	FinalState* final = new FinalState();
	Game* temp = (Game*)p;
	final->score = temp->score;
	cout << "Final Score: " << final->score << " - Instance() - > " << Game::Instance()->score;
	Game::Instance()->GetFSM()->ChangeState(final);

	return 0;
}

Uint32 PowerUpSpawnTimer(Uint32 interval, void* st) { 
	GameState* gs = (GameState*)st;
	gs->SpawnPowerUp();

	return interval;
}


Uint32 AsteroidSpawnTimer(Uint32 interval, void* st) { 
	GameState* gs = (GameState*)st;
	gs->SpawnAsteroid();

	return interval;
}

Uint32 DoublePointsTimer(Uint32 interval, void* p) {
	Player* temp = (Player*)p;
	temp->setScoreMulti(1);

	cout << "Double points has ended";

	return 0;
}

Uint32 RespawnTimeout(Uint32 interval, void* state) {
	GameState* gs = (GameState*)state;

	gs->playerRespawn(gs->getPlayer());
	cout << "Respawning player...";

	return 0;
}

Uint32 AsteroidVoidTimer(Uint32 interval, void* ast) {

	Asteroid* temp = (Asteroid*)ast;
	temp->setVoidState(false);

	return 0;
}

Uint32 InvincibilityTimer(Uint32 interval, void* player) {

	Player* temp = (Player*)player;
	temp->setInvincible(false);

	return 0;
}

void GameState::Enter()
{
	cout << "Entering Game...\n";
	GameOver = false;

	fireSound = Mix_LoadWAV("sounds/Fire.wav");
	powerUpSound = Mix_LoadWAV("sounds/powerup.wav");
	engineSound = Mix_LoadWAV("sounds/Engines.wav");
	playerDiedSound = Mix_LoadWAV("sounds/playerdied.wav");
	astHitSound = Mix_LoadWAV("sounds/asteroidhit.wav");
	playerHitSound = Mix_LoadWAV("sounds/playerhit.wav");
	gameOverSound = Mix_LoadWAV("sounds/gameover.wav");
	bgMusic = Mix_LoadWAV("sounds/bgmusic.ogg");
	asteroidBroke = Mix_LoadWAV("sounds/thunk.wav");

	Mix_VolumeChunk(asteroidBroke, 80);
	Mix_VolumeChunk(bgMusic, 50);
	Mix_VolumeChunk(gameOverSound, 80);
	Mix_VolumeChunk(fireSound, 60);
	Mix_VolumeChunk(astHitSound, 60);
	Mix_VolumeChunk(playerHitSound, 70);
	Mix_VolumeChunk(playerDiedSound, 70);
	Mix_VolumeChunk(powerUpSound, 60);
	Mix_VolumeChunk(engineSound, 50);

	Mix_PlayChannel(-1, bgMusic, 0);

	bgSpriteTex = Game::Instance()->LoadTexture("sprites/background.png");
	mainSpriteTex = Game::Instance()->LoadTexture("sprites/Sprites.png");

	astTex.push_back(Game::Instance()->LoadTexture("sprites/a1.png"));
	astTex.push_back(Game::Instance()->LoadTexture("sprites/a2.png"));
	astTex.push_back(Game::Instance()->LoadTexture("sprites/a3.png"));
	cout << "0 - " << astTex.size() << "\n";

	pwrTex = Game::Instance()->LoadTexture("sprites/powerups.png");
	lifePtsSpriteTex = Game::Instance()->LoadTexture("sprites/heartsprite.png");
	hbarSpriteTex = Game::Instance()->LoadTexture("sprites/health_sprite.png");

	gFont = TTF_OpenFont("game_over.ttf", 56);

	bgSrcRect.x = bgSrcRect.y = 0;
	SDL_QueryTexture(bgSpriteTex, NULL, NULL, &bgSrcRect.w, &bgSrcRect.h);

	bgDestRect.x = bgDestRect.y = 0;
	SDL_GetWindowSize(Game::Instance()->GetWindow(), &bgDestRect.w, &bgDestRect.h);

	bg = new SpriteEx(bgSpriteTex, bgSrcRect, bgDestRect);

	cout << "3 - " << astTex.size() << "\n";

	player = new Player(mainSpriteTex, bgDestRect.w * 0.5, bgDestRect.h * 0.5, 5, 3, 0);

	TimerID powerTimerID = Timer::Instance()->StartTimer(20000, PowerUpSpawnTimer, (void*)this);
	TimerID asteroidTimerID = Timer::Instance()->StartTimer(1000, AsteroidSpawnTimer, (void*)this);
}

Player* GameState::getPlayer() {
	return player;
}

void GameState::RenderHealthBar(bool refresh, int x, int y, int health) {
	SDL_Rect srcRect;
	SDL_Rect destRect;

	if (refresh) {
		srcRect.h = 124;
		srcRect.w = 411;
		srcRect.y = 0;
		srcRect.x = srcRect.w * health;

		destRect.x = x;
		destRect.y = y;
		destRect.w = 153.75;
		destRect.h = 46.5;
	}
	SDL_RenderCopy(Game::Instance()->GetRenderer(), hbarSpriteTex, &srcRect, &destRect);
}

void GameState::RenderLifePoints(bool refresh, int x, int y, int lives) {
	SDL_Rect srcRct;
	SDL_Rect destRct;

	if (refresh) {
		srcRct.h = 34;
		srcRct.w = 34 * lives;
		srcRct.y = 1;
		srcRct.x = 1;

		destRct.x = x;
		destRct.y = y;
		destRct.w = 34 * lives;
		destRct.h = 34;

		SDL_RenderCopy(Game::Instance()->GetRenderer(), lifePtsSpriteTex, &srcRct, &destRct);
	}
}

bool GameState::checkEndGame() {
	if (player->getLives() - 1 == 0) {
		Mix_PlayChannel(-1, gameOverSound, 0);

		return true;
	}
	return false;
}

void GameState::Update()
{
	if (Game::Instance()->KeyDown(SDL_SCANCODE_ESCAPE) == 1) {
		Game::Instance()->GetFSM()->PushState(new PauseMenuPopupState());
			return;
	}

	if ((int)asteroids.size() > 0) {
		for (int i = 0; i < (int)asteroids.size(); i++) {
			asteroids[i]->Update();
		}
	}

	if ((int)powerUps.size() > 0) {
		for (int i = 0; i < (int)powerUps.size(); i++) {
			powerUps[i]->Update();
		}
	}

	if (player) {
		this->CheckCollision();
		if (player) {
			player->Update();
		}

		if (Game::Instance()->KeyDown(SDL_SCANCODE_SPACE) && player)
		{
			if (player->getScoreMulti() == 1) {
				if (!player->getGunCD()) {
					Mix_PlayChannel(-1, fireSound, 0);
					player->SpawnBullet();
					TimerID gunCDTimer = Timer::Instance()->StartTimer(300, gunCooldownTimer, (void*)player);
				}
			}
			else {
				if (!player->getGunCD()) {
					Mix_PlayChannel(-1, fireSound, 0);
					player->SpawnBullet();
					TimerID gunCDTimer = Timer::Instance()->StartTimer(100, gunCooldownTimer, (void*)player);
				}
			}
		}

		if (Game::Instance()->KeyDown(SDL_SCANCODE_W))
		{
			if (!Mix_Playing(15))
				Mix_PlayChannel(15, engineSound, -1);
		}
		if (Game::Instance()->KeyUp(SDL_SCANCODE_W))
		{
			if (player)
			{
				Mix_FadeOutChannel(15, 250);
				player->SetIdle();

			}

		}
	}
}

void GameState::Render()
{
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 255, 255, 255, 255);
	SDL_RenderClear(Game::Instance()->GetRenderer());

	bg->Render();

	if (player) {
		player->Render();

		string pts = "Points: " + to_string(player->getScore());

		RenderHealthBar(true, 15, 680, (player != nullptr ? player->getHealth() : 0));
		RenderFont(true, pts.c_str(), 18, 660);
		RenderLifePoints(true, 15, 720, (player != nullptr ? player->getLives() : 0));
	}

	if ((int)asteroids.size() > 0) {
		for (int i = 0; i < (int)asteroids.size(); i++)
			asteroids[i]->Render();
	}

	if ((int)powerUps.size() > 0) {
		for (int i = 0; i < (int)powerUps.size(); i++)
			powerUps[i]->Render();
	}

	if (GameOver) {
		RenderFont(true, "Game Over!", 1000 * 0.5 - 40, 700 * 0.5);
		Mix_FadeOutChannel(15, 250);
	}

	SDL_RenderPresent(Game::Instance()->GetRenderer());

	ScreenState::Render();

}

void GameState::SpawnAsteroid() {
	cout << "1." << astTex.size() << "\n";

	if ((int)asteroids.size() < 8) {
		SDL_Rect asteroidSrcRect;
		SDL_Rect asteroidDestRect;
		asteroidSrcRect.x = 0;
		asteroidSrcRect.y = 0;
		float r;
		int rand1;
		int rand2;

		int astSize = 1 + rand() % (3 - 1 + 1);

		switch (astSize) {
			case 1: {
				asteroidSrcRect.w = asteroidSrcRect.h = 32;
				asteroidDestRect.w = asteroidDestRect.h = 32;

				r = (1 - rand() % 2 * 2) * (rand() % 6 + 1);
				rand1 = 1 + rand() % (2 - 1 + 1);
				rand2 = 1 + rand() % (2 - 1 + 1);

				asteroidDestRect.x = (rand() % 1000) + 1;
				asteroidDestRect.y = 700;

				Asteroid* ast = new Asteroid(astTex[0], asteroidSrcRect, asteroidDestRect, r, 0.5, rand1 == 2 ? rand1 - 1 : rand1 == 1 ? rand1 - 2 : rand1
					, rand2 == 2 ? rand2 - 1 : rand2 == 1 ? rand2 - 2 : rand2, 1);

				TimerID asteroidTimerID = Timer::Instance()->StartTimer(2000, AsteroidVoidTimer, (void*)ast);

				asteroids.push_back(ast);

				break;

			}

			case 2: {
				asteroidSrcRect.w = asteroidSrcRect.h = 64;
				asteroidDestRect.w = asteroidDestRect.h = 64;

				r = (1 - rand() % 2 * 2) * (rand() % 6 + 1);
				rand1 = 1 + rand() % (2 - 1 + 1);
				rand2 = 1 + rand() % (2 - 1 + 1);

				asteroidDestRect.x = (rand() % 1000) + 1;
				asteroidDestRect.y = 700;

				Asteroid* ast = new Asteroid(astTex[1], asteroidSrcRect, asteroidDestRect, r, 0.5, rand1 == 2 ? rand1 - 1 : rand1 == 1 ? rand1 - 2 : rand1
					, rand2 == 2 ? rand2 - 1 : rand2 == 1 ? rand2 - 2 : rand2, 2);

				TimerID asteroidTimerID = Timer::Instance()->StartTimer(2000, AsteroidVoidTimer, (void*)ast);

				asteroids.push_back(ast);

				break;
			}

			case 3: {
				asteroidSrcRect.w = asteroidSrcRect.h = 128;
				asteroidDestRect.w = asteroidDestRect.h = 128;

				r = (1 - rand() % 2 * 2) * (rand() % 6 + 1);
				rand1 = 1 + rand() % (2 - 1 + 1);
				rand2 = 1 + rand() % (2 - 1 + 1);

				asteroidDestRect.x = (rand() % 1000) + 1;
				asteroidDestRect.y = 700;

				Asteroid* ast = new Asteroid(astTex[2], asteroidSrcRect, asteroidDestRect, r, 0.5, rand1 == 2 ? rand1 - 1 : rand1 == 1 ? rand1 - 2 : rand1
					, rand2 == 2 ? rand2 - 1 : rand2 == 1 ? rand2 - 2 : rand2, 3);

				TimerID asteroidTimerID = Timer::Instance()->StartTimer(2000, AsteroidVoidTimer, (void*)ast);

				asteroids.push_back(ast);

				break;
			}
		}
	}

	cout << "2." << asteroids.size() << "\n";

}

void GameState::playerRespawn(Player* p) {
	int lives = p->getLives() - 1;
	int score = p->getScore();
	delete player;
	player = nullptr;
	SDL_Delay(2000);
	player = new Player(mainSpriteTex, bgDestRect.w * 0.5, bgDestRect.h * 0.5, 5, lives, score);
	player->setInvincible(true);
	TimerID immuneTimer = Timer::Instance()->StartTimer(3000, InvincibilityTimer, (void*)player);
}

void GameState::SpawnPowerUp() {
	if ((int)powerUps.size() < 3) {
		PowerUp* p;
		SDL_Rect pSrcRect;
		SDL_Rect pDestRect;
		pSrcRect.x = 0;
		pSrcRect.y = 0;
		pSrcRect.w = pSrcRect.h = 32;
		pDestRect.w = pDestRect.h = 32;
		float r;

		int rando = 1 + rand() % (3 - 1 + 1);

		switch (rando) {
		case 1:
			pDestRect.x = (rand() % 1000) + 1;
			pDestRect.y = (rand() % 700) + 1;

			p = new PowerUp(pwrTex, pSrcRect, pDestRect, 1, 1);
			powerUps.push_back(p);
			cout << "1. Spawned new power up - " << "Type: Life Up\n";
			break;

		case 2:
			pDestRect.x = (rand() % 1000) + 1;
			pDestRect.y = (rand() % 700) + 1;
			pSrcRect.x = 35;

			p = new PowerUp(pwrTex, pSrcRect, pDestRect, 1, 2);

			powerUps.push_back(p);
			cout << "2. Spawned new power up - " << "Type: Double Points\n";
			break;
		case 3:
			pDestRect.x = (rand() % 1000) + 1;
			pDestRect.y = (rand() % 700) + 1;
			pSrcRect.x = 69;

			p = new PowerUp(pwrTex, pSrcRect, pDestRect, 1, 3);

			powerUps.push_back(p);
			cout << "3. Spawned new power up - " << "Type: Repair\n";
			break;
		}
	}
}

void GameState::Exit()
{
	cout << "GameState: Cleaning up and shutting down engine...\n";

	Timer::Instance()->Clean();
	Mix_Chunk* bgMusic;
	vector<Asteroid*> asteroids;
	vector<SDL_Texture*> astTex;
	vector<PowerUp*> powerUps;
}

void GameState::CheckCollision() {
	if ((int)powerUps.size() > 0) {
		for (int i = 0; i < (int)powerUps.size(); i++) {
			if (RectCollisionTest(player->getDestRect(), powerUps[i]->getDestRect())) {
				switch (powerUps[i]->getType()) {
				case 1: {
					player->pickUpPowerUp(1);
					Mix_PlayChannel(-1, powerUpSound, 0);
					delete powerUps[i];
					powerUps[i] = nullptr;
					powerUps.erase(powerUps.begin() + i);
					break;
				}

				case 2: {
					player->pickUpPowerUp(2);
					Mix_PlayChannel(-1, powerUpSound, 0);
					delete powerUps[i];
					powerUps[i] = nullptr;
					powerUps.erase(powerUps.begin() + i);
					ptsFontColor = SDL_Color{ 255, 255, 255 };
					player->setROF(100);
					TimerID dblPts = Timer::Instance()->StartTimer(15000, DoublePointsTimer, (void*)player);
					break;
				}
				case 3: {
					player->pickUpPowerUp(3);
					Mix_PlayChannel(-1, powerUpSound, 0);
					delete powerUps[i];
					powerUps[i] = nullptr;
					powerUps.erase(powerUps.begin() + i);
					break;
				}
				}
				powerUps.shrink_to_fit();
			}
		}
	}

	if ((int)asteroids.size() > 0) {
		for (int i = 0; i < (int)asteroids.size(); i++)
		{
			if (!player->getInvincible()) {
				if (RectCollisionTest(player->getDestRect(), asteroids[i]->getDestRect()))
				{
					cout << "Player collided with an asteroid!\n";
					
					if (player->tookDamage()) {
						Mix_PlayChannel(-1, playerHitSound, 0);
						player->setInvincible(true);
						TimerID immuneTimer = Timer::Instance()->StartTimer(1000, InvincibilityTimer, (void*)player);
					}
					else if (checkEndGame()) {
						Mix_PlayChannel(-1, playerDiedSound, 0);
						Game::Instance()->score = player->getScore();
						cout << "\nGame instance: " << Game::Instance()->score << " - " << "Player: " << player->getScore() << "\n";

						TimerID gameOver = Timer::Instance()->StartTimer(3000, gameOverTimeout, (void*)Game::Instance());
						delete player;
						player = nullptr;
						GameOver = true;
					}
					else {
						Mix_PlayChannel(-1, playerDiedSound, 0);
						Mix_FadeOutChannel(15, 250);
						TimerID respawnTimeout = Timer::Instance()->StartTimer(0, RespawnTimeout, (void*)this);
					}
					return;
				}
			}
		}
	}

	bool isBreakOutOfLoop = false;
	if ((int)player->GetBullets().size() > 0) {
		for (int b = 0; b < (int)player->GetBullets().size(); b++)
		{
			for (int i = 0; i < (int)asteroids.size(); i++)
			{
				Bullet* bullet = player->GetBullets()[b];

				if (RectCollisionTest(bullet->getDestRect(), asteroids[i]->getDestRect())) {
					{
						cout << "Bullet collided with an asteroid!!" << asteroids[i]->getPtsVal() << "\n";
						Mix_PlayChannel(-1, astHitSound, 0);
						delete bullet;
						player->GetBullets()[b] = nullptr;
						player->GetBullets().erase(player->GetBullets().begin() + b);

						if (!asteroids[i]->takeDamage()) {
							player->addScore(asteroids[i]->getPtsVal());
							Mix_PlayChannel(-1, asteroidBroke, 0);
							delete asteroids[i];
							asteroids[i] = nullptr;
							asteroids.erase(asteroids.begin() + i);
						}
						isBreakOutOfLoop = true;
					}
					if (isBreakOutOfLoop)	break;
				}

				if (isBreakOutOfLoop)	break;
			}

			if (isBreakOutOfLoop)
			{
				player->GetBullets().shrink_to_fit();
				asteroids.shrink_to_fit();
			}
		}
	}
}