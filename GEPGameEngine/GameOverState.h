#pragma once

#include "ScreenState.h"
#include <vector>
#include "Button.h"
using namespace std;

class GameOverState : public ScreenState
{
private:
	vector<Button*> m_vButtons;
	enum btn { resume, exit };
	int points;

public:
	GameOverState() {}
	GameOverState(int pts) { points = pts; }
	void Update(); // Method prototype.
	void Render();
	void Enter();
	void Exit();
	void Resume() {}

};