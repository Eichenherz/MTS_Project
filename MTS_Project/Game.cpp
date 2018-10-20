#include "Game.h"
#include "APP.h"

Game::Game( APP_WND& _wnd )
	:
	wnd { _wnd },
	gfx { wnd }
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
		tiles [i].pos = Vec2( (float) tile_x_step * (i+1), (float) GFX::height / 2 );
	}
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
	
		break;
	}
	case Game::GAME_STATE::PLAYING:
	{
		
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
			gfx.Draw_Texture( p_tile_texture, tile.pos );
			gfx.Draw_Text( p_title_font, tile.pos + tile_offset, tile.number );
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
