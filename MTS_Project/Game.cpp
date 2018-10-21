#include "Game.h"
#include "APP.h"
#include <algorithm>

using namespace TILE_DRAW_PARAMS;

wchar_t Map_Key_to_WChar( DirectX::Keyboard::State state, Kbd_Tracker tracker );


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
	swap_iter = tiles.end();

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
			gs = GAME_STATE::PLAYING;
			timer.Start();
		}
		break;
	}
	case Game::GAME_STATE::PLAYING:
	{
		// GET TIME
		seconds = timer.Mark();

		// PREVENTS SEVERAL SWAPS PER FRAME
		clicked_in_this_frame = false;

		const auto mouse_state = wnd.p_mouse->GetState();
		if ( mouse_state.leftButton ) // MANDATORY CONDITION
		{
			const auto mouse_pos = Vec2 { (float) mouse_state.x, (float) mouse_state.y };

			if ( dummy_tile.state == Tile::STATE::UNMOVED )
			{
				// AQUIRE TILE
				for ( auto iter = tiles.begin(); iter != tiles.end(); ++iter )
				{
					if ( AABB_vs_Point( iter->aabb, mouse_pos ) )
					{
						dummy_tile.number = iter->number;
						dummy_tile.state = iter->state = Tile::STATE::PENDING;
						swap_iter = iter;
						break;
					}
				}
			}

			dummy_tile.aabb.c = mouse_pos;
			
			if ( dummy_tile.state != Tile::STATE::UNMOVED )
			{
				// SWAP TILES
				for ( auto& tile : tiles )
				{
					const auto tile_collision = AABB_vs_AABB( tile.aabb, dummy_tile.aabb );
					const auto contains_mouse = AABB_vs_Point( tile.aabb, mouse_pos );
					if ( &tile != &( *swap_iter ) )
					{
						tile.state = ( tile_collision && contains_mouse ) ? 
									 Tile::STATE::COLLIDE: 
									 Tile::STATE::UNMOVED;
					}
					if ( tile_collision && contains_mouse && 
						 mouse_state.rightButton && !clicked_in_this_frame )
					{
						std::swap( tile.number, swap_iter->number );
						clicked_in_this_frame = true;
						dummy_tile.state = Tile::STATE::UNMOVED;
						break;
					}
				}
			}
		}
		else
		{
			dummy_tile.state = Tile::STATE::UNMOVED;
			for ( auto& tile : tiles )
			{
				// AFTER NO ACTION TOOK PLACE GO BACK TO NORMAL STATE
				tile.state = Tile::STATE::UNMOVED;
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
			// GET PLAYER TIME
			time = timer.Mark();
		}
		break;
	}
	case Game::GAME_STATE::RESULTS:
	{
		const auto kbd_stat = wnd.p_kbd->GetState();
		kbd_tracker.Update( kbd_stat );
		if ( !kbd_tracker.IsKeyPressed(DirectX::Keyboard::Keys::Enter) )
		{
			const auto Wchar = Map_Key_to_WChar( kbd_stat, kbd_tracker );
			if ( Wchar != L'0' )
			{
				nickname += Wchar;
			}
		}
		else
		{
			gs = GAME_STATE::ENDGAME;
		}
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
		gfx.Draw_Text( p_cont_font, Vec2 { GFX::width / 2,100 }, std::to_wstring( seconds ) + L"s" );
		gfx.Draw_Color_Text( p_inst_font, Vec2 { GFX::width / 2, 570 }, L"* arrange the numbers as fast as you can", DirectX::Colors::OrangeRed );
		for ( const auto& tile : tiles )
		{
			switch ( tile.state ) // COLOR ACCORDING TO STATE
			{
			case Tile::STATE::COLLIDE:
			{
				gfx.Draw_Color_Texture( p_tile_texture, tile.aabb.c, DirectX::Colors::Red );
				break;
			}
			case Tile::STATE::PENDING:
			{
				gfx.Draw_Color_Texture( p_tile_texture, tile.aabb.c, DirectX::Colors::DarkSlateGray );
				break;
			}
			case Tile::STATE::UNMOVED:
			{
				gfx.Draw_Texture( p_tile_texture, tile.aabb.c );
				break;
			}
			}
			gfx.Draw_Text( p_title_font, tile.aabb.c + tile_offset, tile.number );
		}

		if ( dummy_tile.state != Tile::STATE::UNMOVED )
		{
			const auto color = ( dummy_tile.state == Tile::STATE::COLLIDE ) ?
								DirectX::Colors::Red :
								DirectX::Colors::White;
			gfx.Draw_Texture( p_tile_texture, dummy_tile.aabb.c );
			gfx.Draw_Text( p_title_font, dummy_tile.aabb.c + tile_offset, dummy_tile.number );
		}
		break;
	}
	case Game::GAME_STATE::ENDGAME:
		// want to show same scene, but not to take user imp
	case Game::GAME_STATE::RESULTS:
	{
		gfx.Draw_Color_Text( p_inst_font, Vec2 { GFX::width / 2, 570 }, L"* type in your nickname", DirectX::Colors::OrangeRed );
		gfx.Draw_Text( p_cont_font, Vec2 { GFX::width / 2, 200 }, std::to_wstring( time ) + L"s" + nickname );
		break;
	}

	}
	
}

wchar_t Map_Key_to_WChar( DirectX::Keyboard::State state, Kbd_Tracker tracker )
{
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::A ) ) { return L'A';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::B ) ) { return L'B';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::C ) ) { return L'C';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::D ) ) { return L'D';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::E ) ) { return L'E';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::F ) ) { return L'F';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::G ) ) { return L'G';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::H ) ) { return L'H';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::I ) ) { return L'I';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::J ) ) { return L'J';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::K ) ) { return L'K';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::L ) ) { return L'L';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::M ) ) { return L'M';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::N ) ) { return L'N';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::O ) ) { return L'O';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::P ) ) { return L'P';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::Q ) ) { return L'Q';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::R ) ) { return L'R';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::S ) ) { return L'S';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::T ) ) { return L'T';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::U ) ) { return L'U';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::V ) ) { return L'V';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::W ) ) { return L'W';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::X ) ) { return L'X';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::Y ) ) { return L'Y';}
	if( tracker.IsKeyPressed( DirectX::Keyboard::Keys::Z ) ) { return L'Z';}

	return L'0';
}
