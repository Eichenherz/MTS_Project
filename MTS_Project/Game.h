#pragma once

#include "GFX.h"

class Game
{
public:
	Game( class APP_WND& wnd );

	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;


	void Run();

private:
	void Update();
	void Draw_Model();

private:
	APP_WND&	wnd;
	GFX			gfx;

	// Store tile texture here since all tiles share same textures;
	TEXTURE_PTR p_tile_texture;
	FONT_PTR	p_title_font;
	FONT_PTR	p_cont_font
;

	enum class GAME_STATE
	{
		WELCOME,
		PLAYING,
		RESULTS,
		COUNT
	} gs = GAME_STATE::WELCOME;

};

// Place assets here for convenience 
class Tile
{
private:
	// For changing the color of the corresp number
	enum class STATE
	{
		UNMOVED,
		PENDING,
		COLLIED,
		COUNT
	} state = STATE::UNMOVED;

	Vec2			pos;
	wchar_t const*	number;
};