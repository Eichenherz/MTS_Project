#pragma once

#include "GFX.h"
#include <array>
#include "Tile.h"


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
	FONT_PTR	p_cont_font;
	FONT_PTR	p_inst_font;

	// GAME ASSETS :
	// If you change the # of tiles go change it in Tile.h too
	std::array<Tile, 9>		tiles;
	Tile					dummy_tile;


	// GAME STATE Params
	enum class GAME_STATE
	{
		WELCOME,
		PLAYING,
		RESULTS,
		COUNT
	} gs = GAME_STATE::WELCOME;

	bool playing = false;

	std::random_device	rd;
	std::mt19937		rng;
}; 
