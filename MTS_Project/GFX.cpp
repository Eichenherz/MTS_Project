#include "GFX.h"
#include "APP.h"
#include <array>

GFX::GFX( HWND& hwnd )
{
	UINT make_device_flags = 0;

#ifdef _DEBUG

	make_device_flags |= D3D11_CREATE_DEVICE_DEBUG;

#endif // _DEBUG

	// Needed in case of no gfx card  // Might remove later
	std::array<D3D_DRIVER_TYPE, 3> driver_types =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	// Assume at least DX11
	std::array<D3D_FEATURE_LEVEL, 4> feature_level =
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

}
