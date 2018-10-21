#pragma once

#include "GFX.h"
#include <array>
#include "Tile.h"
#include "Mouse.h"

using ButtonState = DirectX::Mouse::ButtonStateTracker;

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
	// FAMEWORK OBJECTS
	APP_WND&			wnd;
	GFX					gfx;
	// RANDOM STUFF
	std::random_device	rd;
	std::mt19937		rng;
	// MOUSE BUTTON TRACKER
	ButtonState			tracker;

	// GAME ASSETS
	// Store tile texture here since all tiles share same textures;
	TEXTURE_PTR			p_tile_texture;
	FONT_PTR			p_title_font;
	FONT_PTR			p_cont_font;
	FONT_PTR			p_inst_font;


	// GAME OBJECTS :
	// If you change the # of tiles go change it in Tile.h too
	std::array<Tile, 9>	tiles;
	Tile				dummy_tile;
	using Tile_Iter = std::array<Tile, 9>::iterator;
	Tile_Iter			swap_iter;

	// GAME STATE PARAMS
	enum class GAME_STATE
	{
		WELCOME,
		PLAYING,
		RESULTS
	};
	GAME_STATE  gs = GAME_STATE::WELCOME;
	bool		clicked_in_this_frame = false;
}; 
