#pragma once

#include <SDL.h>
#include <sdl_ttf.h>
class ScreenState // This is the abstract base class for all specific states.
{
protected:
	TTF_Font* gFont;
	SDL_Texture* m_pFontTexture;
	SDL_Rect m_rFontRect;
	SDL_Color ptsFontColor;

public:
	virtual void Update() = 0;
	virtual void Render();
	void RenderFont(bool isRefreshText, const char *text, int x, int y);
	virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual void Resume() = 0;
	void setPtsFontColor(SDL_Color c);

};