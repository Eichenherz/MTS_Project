#include "Game.h"
#include "APP.h"
#include <algorithm>

using namespace TILE_DRAW_PARAMS;

Game::Game( APP_WND& _wnd )
	:
	wnd { _wnd },
	gfx { wnd },
	rng { rd() }
{
	// Set textures
	gfx.Load_Font( p_title_font, L"Resources/title.spritefont" );
	gfx.Load_Font( p_cont_font, L"Resources/content.spritefont" );
	gfx.Load_Font( p_inst_font, L"Resources/instruction.spritefont" );
	gfx.Load_Texture( p_tile_texture, L"Resources/Rect70x100.dds" );

	// Set tiles
	
	for ( size_t i = 0; i < tiles.size(); ++i )
	{
		tiles [i].number = std::to_wstring( i + 1 );
		tiles [i].aabb.c = Vec2( (float) tile_x_step * ( i + 1 ), (float) GFX::height / 2 );
	}

	// Ramdom shuffle the tiles
	shuffle_tiles( tiles.begin(), tiles.end(), rng );
}

void Game::Run()
{
	gfx.Begin();
	Update();
	Draw_Model();
	gfx.End();
}

void Game::Update()
{
	switch ( gs )
	{

	case Game::GAME_STATE::WELCOME:
	{
		const auto mouse_event = wnd.p_mouse->GetState();
		
		if ( mouse_event.leftButton )
		{
			gs = GAME_STATE::PLAYING;;
		}
		break;
	}
	case Game::GAME_STATE::PLAYING:
	{
		const auto mouse_state = wnd.p_mouse->GetState();
		tracker.Update( mouse_state );
		if ( tracker.leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED )
		{
			const auto mouse_pos = Vec2 { (float) mouse_state.x, (float) mouse_state.y };

			auto iter = tiles.begin(); // Need to store the initial tile
			for ( ; iter != tiles.end(); ++iter )
			{
				if ( AABB_vs_Point( iter->aabb, mouse_pos ) )
				{
					dummy_tile.number = iter->number;
					dummy_tile.aabb.c = mouse_pos;
					engage_dummy_flag = true;
					break;
				}
			}

			for ( auto& tile : tiles )
			{
				if ( AABB_vs_AABB( tile.aabb, dummy_tile.aabb ) && 
					 tracker.leftButton == DirectX::Mouse::ButtonStateTracker::RELEASED )
				{
					std::swap( tile, *iter );
					engage_dummy_flag = false;
				}
			}
		}

		if ( std::is_sorted( tiles.cbegin(), tiles.cend(), 
			[] ( const Tile& t1, const Tile& t2 )->bool
			{
				const auto num1 = std::stoi( t1.number );
				const auto num2 = std::stoi( t2.number );

				return ( num1 < num2 );
			} ) )
		{
			gs = GAME_STATE::RESULTS;
		}
		break;
	}
	case Game::GAME_STATE::RESULTS:
	{
		
		break;
	}

	}

}

void Game::Draw_Model()
{
	// order* - title , this stays fixed
	gfx.Draw_Text( p_title_font, Vec2 { GFX::width / 2,50 }, L"order*" );


	// Instruction will change according to game state
	switch ( gs )
	{

	case Game::GAME_STATE::WELCOME:
	{
		gfx.Draw_Color_Text( p_inst_font, Vec2 { GFX::width / 2, 570 }, L"* click anywhere to START", DirectX::Colors::OrangeRed );

		
		break;
	}
	case Game::GAME_STATE::PLAYING:
	{
		gfx.Draw_Color_Text( p_inst_font, Vec2 { GFX::width / 2, 570 }, L"* arrange the numbers as fast as you can", DirectX::Colors::OrangeRed );
		for ( const auto& tile : tiles )
		{
			gfx.Draw_Texture( p_tile_texture, tile.aabb.c );
			gfx.Draw_Text( p_title_font, tile.aabb.c + tile_offset, tile.number );
		}
		if ( engage_dummy_flag )
		{
			gfx.Draw_Texture( p_tile_texture, dummy_tile.aabb.c );
			gfx.Draw_Text( p_title_font, dummy_tile.aabb.c + tile_offset, dummy_tile.number );
		}
		break; 
	}
	case Game::GAME_STATE::RESULTS:
	{
		gfx.Draw_Color_Text( p_inst_font, Vec2 { GFX::width / 2, 570 }, L"* type in your nickname", DirectX::Colors::OrangeRed );
		break;
	}

	}
}
