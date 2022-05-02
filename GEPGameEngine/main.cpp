
#include "SDL.h"
#include "Game.h"

int main(int argc, char* argv[])
{

	if (!Game::Instance()->Init("SDL Framework", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 1024, 768, 0))
		return -1; //close the application if initialization fails

	while (Game::Instance()->IsRunning())
	{
		Game::Instance()->HandleEvents();
		Game::Instance()->Update();
		Game::Instance()->Render();
		SDL_Delay(10); //delay of 10 ms after every loop iteration
		

	}
	Game::Instance()->Clean();

	return 0;

}