#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <memory>
#include <DirectXColors.h>
#include <comdef.h>
#include <string>

#pragma comment(lib,"d3d11.lib")


class GFX
{
public:
	GFX( class HWNDKey& key  );
	~GFX();

	GFX( const GFX& ) = delete;
	GFX& operator=( const GFX& ) = delete;

	void Test();
	void LogError( HRESULT hr, std::string message )
	{
		_com_error error( hr );
		std::string error_message = "Error: " + message + "\n" + error.ErrorMessage();
		MessageBox( NULL, error_message.c_str(), "Error", MB_ICONERROR );
	}
private:

public:
	static constexpr UINT width = 800;
	static constexpr UINT height = 600;

private:
	template<typename T> 
	using COM_PTR = Microsoft::WRL::ComPtr<T>;
	// NOTE : 
	// to read addressof( COM_PTR p_obj ) use p_obj.GetAddressOf(), 
	// because it doesn't reset the obj pointed to by p_obj
	// operator& sets p_obj = nullptr and gives that address


	COM_PTR<ID3D11Device>			p_device;
	COM_PTR<ID3D11DeviceContext>	p_inst_context;
	COM_PTR<IDXGISwapChain>			p_swap_chain;
	COM_PTR<ID3D11RenderTargetView> p_render_target_view;
	COM_PTR<ID3D11Texture2D>		p_texture2d_buffer;
	D3D11_VIEWPORT					viewport;
	
};