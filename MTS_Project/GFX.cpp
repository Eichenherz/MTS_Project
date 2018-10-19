#include "GFX.h"
#include "APP.h"
#include <array>
#include <cassert>

GFX::GFX( HWNDKey& key )
{
	UINT make_device_flags = 0;

#ifdef _DEBUG

	make_device_flags |= D3D11_CREATE_DEVICE_DEBUG;

#endif // _DEBUG

	// Needed in case of no gfx card  // Might remove later
	constexpr std::array<D3D_DRIVER_TYPE, 3> driver_types =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	// Assume at least DX11
	constexpr std::array<D3D_FEATURE_LEVEL, 4> feature_levels =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	// Swap Chain description
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory( std::addressof( scd ), sizeof( DXGI_SWAP_CHAIN_DESC ) );

	scd.BufferCount = 1; // Double buffer
	scd.BufferDesc.Width = GFX::width;
	scd.BufferDesc.Height = GFX::height;
	scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // Color format
	// Might reverse these 2 if BUGs
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = key.h_app_wnd;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Windowed = true;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	//scd.Flags = Don't// keep it Window not fullscreen


	// Chnages these if your machine requires so
	D3D_FEATURE_LEVEL	feature_levels_requested = D3D_FEATURE_LEVEL_11_0;
	D3D_DRIVER_TYPE		driver_type = D3D_DRIVER_TYPE_HARDWARE;
	
	const HRESULT hres = D3D11CreateDeviceAndSwapChain(	nullptr, driver_type, nullptr, make_device_flags,
														std::addressof(feature_levels_requested), 1,
														D3D11_SDK_VERSION, std::addressof( scd ), &p_swap_chain, 
														&p_device, nullptr, &p_inst_context );

	if( FAILED( hres ) )
	{
		LogError( hres, "Failed to create device and swapchain" );
	}
	

	// MAKE TEXTURE2D BUFFER DESC
	D3D11_TEXTURE2D_DESC tex_buf_desc;
	tex_buf_desc.Width = width;
	tex_buf_desc.Height = height;
	tex_buf_desc.MipLevels = 1;
	tex_buf_desc.ArraySize = 1;
	tex_buf_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tex_buf_desc.SampleDesc.Count = 1;
	tex_buf_desc.SampleDesc.Quality = 0;
	tex_buf_desc.Usage = D3D11_USAGE_DYNAMIC;
	tex_buf_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tex_buf_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tex_buf_desc.MiscFlags = 0;

	/* Create */
	const HRESULT hres_tex = p_device->CreateTexture2D( std::addressof( tex_buf_desc ), nullptr, &p_texture2d_buffer );
	if ( FAILED( hres_tex ) )
	{
		LogError( hres, "Failed to create 2D texture" );
	}

	// RENDER VIEW TARGET 

	/* Create */
	p_swap_chain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( std::addressof( p_texture2d_buffer ) ) );
	p_device->CreateRenderTargetView( p_texture2d_buffer.Get(), nullptr, &p_render_target_view );

	/* Bind */
	p_inst_context->OMSetRenderTargets( 1, p_render_target_view.GetAddressOf(), nullptr );

	// MAKE VIEWPORT
	viewport.Width = float( width );
	viewport.Height = float( height );
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	// Bind it !
	p_inst_context->RSSetViewports( 1, std::addressof( viewport ) );

}

GFX::~GFX()
{
	if ( p_inst_context )
	{
		p_inst_context->ClearState();
	}
}

void GFX::Test()
{
	p_inst_context->ClearRenderTargetView( p_render_target_view.Get(), DirectX::Colors::Red );
	p_swap_chain->Present( 0, 0 );
}
