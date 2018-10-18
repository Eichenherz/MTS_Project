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


	HRESULT hres;
	for ( auto& driver : driver_types )
	{
		hres = D3D11CreateDeviceAndSwapChain( nullptr, driver, nullptr, make_device_flags,
											  feature_levels.data(), feature_levels.size(),
											  D3D11_SDK_VERSION, std::addressof( scd ), &p_swap_chain, 
											  &p_device, std::addressof( feature_level ), &p_inst_context );

		if ( SUCCEEDED( hres ) )
		{
			driver_type = driver;
			break;
		}
	}

	assert( FAILED( hres ) ); // can't have it m8 !


	// RENDER VIEW TARGET 

	/* Create */
	ID3D11Texture2D* p_back_buffer_tex;
	p_swap_chain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( p_back_buffer_tex ) );
	p_device->CreateRenderTargetView( p_back_buffer_tex, nullptr, &p_render_target_view );

	/* Bind */
	p_inst_context->OMSetRenderTargets( 1, &p_render_target_view, nullptr );

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
