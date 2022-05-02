#include "FinalState.h"
#include <iostream>
#include "Game.h"
#include "GameState.h"
#include "MainMenuState.h"

using namespace std;

// Begin PauseState
void FinalState::Enter()
{
	gFont = TTF_OpenFont("game_over.ttf", 100);

	cout << "Entering FinalState..." << endl;

	m_vButtons.push_back(new Button("Img/play.png", { 0,0,400,100 }, { 412,300,200,80 }));
	m_vButtons.push_back(new Button("Img/exit.png", { 0,0,400,100 }, { 412,400,200,80 }));
}

void FinalState::Update()
{
	// Update buttons. Allows for mouseovers.
	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Update();

	//close the pause menu popup
	if (m_vButtons[btn::play]->Clicked())
	{
		Game::Instance()->GetFSM()->ChangeState(new GameState());
	}
	//else if exit was clicked, we need to go back to main menu
	else if (m_vButtons[btn::exit]->Clicked())
	{
		Game::Instance()->GetFSM()->Clean(); // Clear all states, including GameState on bottom.
		//go back to main menu
		Game::Instance()->GetFSM()->ChangeState(new MainMenuState());
	}

}


void FinalState::Render()
{
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 64, 64, 128, 128);

	SDL_Rect rect = { 256, 128, 512, 512 };
	SDL_RenderFillRect(Game::Instance()->GetRenderer(), &rect);

	for (int i = 0; i < (int)m_vButtons.size(); i++)
		m_vButtons[i]->Render();

	string pts = "Final Score: " + to_string(score);
	ScreenState::RenderFont(true, pts.c_str(), 400, 150);

	ScreenState::Render();

}


void FinalState::Exit()
{

	cout << "Exiting Pause..." << endl;

	for (int i = 0; i < (int)m_vButtons.size(); i++)
	{
		delete m_vButtons[i];
		m_vButtons[i] = nullptr;
	}
	m_vButtons.clear();
	m_vButtons.shrink_to_fit();
}

