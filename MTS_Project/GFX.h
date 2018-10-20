#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <memory>
#include <DirectXColors.h>
#include <comdef.h>
#include <string>
#include "SpriteBatch.h"
#include "SimpleMath.h"

#pragma comment(lib,"d3d11.lib")


using Vec2 = DirectX::SimpleMath::Vector2;
using TEXTURE_PTR = Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>;

class GFX
{
	template<typename T>
	using COM_PTR = Microsoft::WRL::ComPtr<T>;
	// NOTE : 
	// to read addressof( COM_PTR p_obj ) use p_obj.GetAddressOf(), 
	// because it doesn't reset the obj pointed to by p_obj
	// operator& sets p_obj = nullptr and gives that address

public:
	GFX( class HWNDKey& key  );
	~GFX();

	GFX( const GFX& ) = delete;
	GFX& operator=( const GFX& ) = delete;
	
	void Begin();
	void End();
	void Draw_Texture( const TEXTURE_PTR& p_tex, Vec2 pos );
	void Load_Texture( TEXTURE_PTR& p_tex, const std::wstring& name );

private:
	void LogError( HRESULT hr, std::string message )
	{
		_com_error error( hr );
		std::string error_message = "Error: " + message + "\n" + error.ErrorMessage();
		MessageBox( NULL, error_message.c_str(), "Error", MB_ICONERROR );
	}

public:
	static constexpr UINT width = 800;
	static constexpr UINT height = 600;

private:
	COM_PTR<ID3D11Device>					p_device;
	COM_PTR<ID3D11DeviceContext>			p_inst_context;
	COM_PTR<IDXGISwapChain>					p_swap_chain;
	COM_PTR<ID3D11RenderTargetView>			p_render_target_view;
	COM_PTR<ID3D11Texture2D>				p_texture2d_buffer;
	
	D3D11_VIEWPORT							viewport;
	std::unique_ptr<DirectX::SpriteBatch>	p_sprite;
};