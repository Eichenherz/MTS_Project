#pragma once

#include <wrl.h>
#include <d3d11.h>

#pragma comment(lib,"d3d11.lib")

class GFX
{
public:
	GFX( class HWND& hwnd );
private:

public:
	static constexpr UINT width = 800;
	static constexpr UINT height = 600;

private:
	template<typename T> 
	using COM_PTR = Microsoft::WRL::ComPtr<T>;


	COM_PTR<ID3D11Device>			p_device;
	COM_PTR<ID3D11DeviceContext>	p_inst_context;
	COM_PTR<IDXGISwapChain>			p_swap_chain;
	COM_PTR<ID3D11RenderTargetView> p_render_target_view;
	D3D_DRIVER_TYPE					driver_type;
	D3D_FEATURE_LEVEL				feature_level;
	D3D11_VIEWPORT					viewport;



};