#include "GFX.h"
#include "APP.h"
#include <array>
#include <cassert>
#include "DDSTextureLoader.h"

GFX::GFX( HWNDKey& key )
{
	UINT make_device_flags = 0;

#ifdef _DEBUG

	make_device_flags |= D3D11_CREATE_DEVICE_DEBUG;

#endif // _DEBUG

	
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
	D3D11_TEXTURE2D_DESC tex_buf_desc = {};
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


	// Init sprite
	p_sprite = std::make_unique<DirectX::SpriteBatch>( p_inst_context.Get() );

}

GFX::~GFX()
{
	p_inst_context->ClearState();
	
#ifdef _DEBUG
	ID3D11Debug* p_debug_device = nullptr;
	HRESULT hr_mem_leak = p_device->QueryInterface( __uuidof( ID3D11Debug ), reinterpret_cast<void**>( std::addressof( p_debug_device ) ) );
	if ( FAILED( hr_mem_leak ) )
	{
		LogError( hr_mem_leak, "Failed to query" );
	}
	hr_mem_leak = p_debug_device->ReportLiveDeviceObjects( D3D11_RLDO_DETAIL );
	if ( FAILED( hr_mem_leak ) )
	{
		LogError( hr_mem_leak, "Failed to report live objs" );
	}
	if ( p_debug_device != nullptr ) { p_debug_device->Release(); }
#endif // _DEBUG
}

void GFX::Begin()
{
	// Set backgroung
	p_inst_context->ClearRenderTargetView( p_render_target_view.Get(), DirectX::Colors::CadetBlue );

	p_sprite->Begin();
}

void GFX::End()
{
	p_sprite->End();

	const HRESULT hr_draw = p_swap_chain->Present( 0, 0 );
	if ( FAILED( hr_draw ) )
	{
		LogError( hr_draw, "Failed to draw" );
	}
}

void GFX::Draw_Texture( const TEXTURE_PTR& p_tex, Vec2 pos )
{
	Draw_Color_Texture( p_tex.Get(), pos, DirectX::Colors::White );
}

void GFX::Draw_Color_Texture( const TEXTURE_PTR & p_tex, Vec2 pos, Color c )
{
	p_sprite->Draw( p_tex.Get(), pos, nullptr, c );
}

void GFX::Draw_Text( const FONT_PTR & p_font, Vec2 pos, const std::wstring& txt )
{
	Draw_Color_Text( p_font, pos, txt, DirectX::Colors::White );
}

void GFX::Draw_Color_Text( const FONT_PTR & p_font, Vec2 pos, const std::wstring & txt, Color c )
{
	// Draw relative to the string center
	Vec2 origin = p_font->MeasureString( txt.c_str() );
	origin /= 2;
	p_font->DrawString( p_sprite.get(), txt.c_str(), pos, c, 0.0f, origin );
}

void GFX::Load_Texture( TEXTURE_PTR & p_tex, const std::wstring& name )
{
	const HRESULT hr_tex_load = DirectX::CreateDDSTextureFromFile( p_device.Get(), name.c_str(), nullptr, p_tex.GetAddressOf() );
	if ( FAILED( hr_tex_load ) )
	{
		LogError( hr_tex_load, "Failed to load surface" );
	}
}

void GFX::Load_Font( FONT_PTR & p_font, const std::wstring& name )
{
	p_font = std::make_unique<DirectX::SpriteFont>( p_device.Get(), name.c_str() );
}
 