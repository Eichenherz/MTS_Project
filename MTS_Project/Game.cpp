#include "Game.h"
#include "APP.h"

Game::Game( APP_WND& _wnd )
	:
	wnd { _wnd },
	gfx { wnd }
{
	gfx.Load_Texture( p_tile_texture, L"Resources/Tile.dds" );
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
}

void Game::Draw_Model()
{
	gfx.Draw_Texture( p_tile_texture, Vec2 { 100,100 } );
}
