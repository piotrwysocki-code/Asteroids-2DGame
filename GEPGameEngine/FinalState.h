#pragma once
#include <iostream>
#include "ScreenState.h"
#include <vector>
#include "Button.h"
using namespace std;

class FinalState : public ScreenState
{
private:
	vector<Button*> m_vButtons;
	enum btn { play, exit };
public:
	FinalState() {}
	void Update(); 
	void Render();
	void Enter();
	void Exit();
	int score;
	void Resume() { cout << "Resuming Game..." << endl; }

};