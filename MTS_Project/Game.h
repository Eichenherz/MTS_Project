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

	using TEXTURE_PTR = Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>;
	// Store tile texture here since all tiles share same textures;
	TEXTURE_PTR p_tile_texture;

};