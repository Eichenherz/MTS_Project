#pragma once

#include "GFX.h"
#include <array>


// NOTE: for simplicity I won't OOP the tiles
struct Tile
{
	// For changing the color of the corresp number
	enum class STATE
	{
		UNMOVED,
		PENDING,
		COLLIED,
		COUNT
	} state = STATE::UNMOVED;

	Vec2			pos;
	std::wstring	number;
};


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
;

	enum class GAME_STATE
	{
		WELCOME,
		PLAYING,
		RESULTS,
		COUNT
	} gs = GAME_STATE::PLAYING; // for final release se to WELCOME

	// GAME ASSETS :
	std::array<Tile, 9> tiles;
	Tile				dummy_tile;

	// Change these in GFX.cpp Draw_Texture_With color
	static constexpr UINT tile_width = 80;
	static constexpr UINT tile_height = 100;
	static constexpr UINT tile_slop = 15;
	static constexpr Vec2 tile_offset { tile_width / 2 - tile_slop, tile_height / 2 };
	static constexpr UINT tile_x_step = tile_width + tile_slop;
}; 
