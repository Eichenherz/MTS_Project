#include "APP.h"
#include <cassert>


APP_WND::APP_WND( HINSTANCE h_inst )
	:
	h_app_inst { h_inst }
{
	const DWORD wnd_style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

	WNDCLASSEX wcx;

	
	wcx.cbSize = sizeof( WNDCLASSEX );
	wcx.style = CS_CLASSDC;
	wcx.lpfnWndProc = APP_WND::Wnd_Proc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = h_app_inst;
	wcx.hIcon = LoadIcon( nullptr, IDI_APPLICATION );
	wcx.hCursor = LoadCursor( nullptr, IDC_ARROW );
	wcx.hbrBackground = nullptr;
	wcx.lpszMenuName = nullptr;
	wcx.lpszClassName = "APP_WND_CLASS";
	wcx.hIconSm = LoadIcon( nullptr, IDI_APPLICATION );
	

	RegisterClassEx( std::addressof( wcx ) );
	


	RECT window_rect { 350,100, GFX::width + 350, GFX::height + 100 };
	AdjustWindowRect( std::addressof( window_rect ), wnd_style, FALSE );
	const UINT WR_width = window_rect.right - window_rect.left;
	const UINT WR_height = window_rect.bottom - window_rect.top;
	const UINT x = ( GetSystemMetrics( SM_CXSCREEN ) - WR_width ) / 2;
	const UINT y = ( GetSystemMetrics( SM_CYSCREEN ) - WR_height ) / 2;
	h_app_wnd = CreateWindow( wcx.lpszClassName, name, wnd_style, x, y, WR_width, WR_height,
							  nullptr, nullptr, h_app_inst, this );

	assert( h_app_wnd ); // FAILED TO CREATE WND;

	ShowWindow( h_app_wnd, SW_SHOW );
	UpdateWindow( h_app_wnd );

	// Init devices
	p_kbd = std::make_unique<DirectX::Keyboard>();
	p_mouse = std::make_unique<DirectX::Mouse>();
	p_mouse->SetWindow( h_app_wnd );
}

APP_WND::~APP_WND()
{
	UnregisterClass( "APP_WND_CLASS", h_app_inst );
}

LRESULT APP_WND::Handle_Msg( HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param )
{
	switch ( msg )
	{
	case WM_DESTROY : 
	{
		PostQuitMessage( 0 );
		return 0;
	}

	// mouse imput
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		DirectX::Mouse::ProcessMessage( msg, w_param, l_param );
		break;

	// kbd imput
	case WM_KEYDOWN:
	case WM_KEYUP:
		DirectX::Keyboard::ProcessMessage( msg, w_param, l_param );
		break;
	}

	return DefWindowProc( hwnd, msg, w_param, l_param );
}

auto APP_WND::Get_WND_Handler()
{
	return h_app_wnd;
}

bool APP_WND::Process_Message()
{
	MSG msg;

	while ( PeekMessage( std::addressof( msg ), nullptr, 0, 0, PM_REMOVE ) )
	{
		TranslateMessage( std::addressof( msg ) );
		DispatchMessage( std::addressof( msg ) );
	
		if ( msg.message == WM_QUIT )
		{
			return false;
		}
	}

	return true;
}

LRESULT APP_WND::Wnd_Proc( HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param )
{
	APP_WND* p_wnd;
	if ( msg == WM_CREATE )
	{
		const CREATESTRUCTW* const p_create = reinterpret_cast<CREATESTRUCTW*>( l_param );
		p_wnd = reinterpret_cast<APP_WND*>( p_create->lpCreateParams );
		///// might
		SetWindowLongPtr( hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>( &APP_WND::Wnd_Proc_Thunk ) );
	}
	else
	{
		p_wnd = reinterpret_cast<APP_WND*>( GetWindowLongPtr( hwnd, GWLP_USERDATA ) );
	}
	return p_wnd->Handle_Msg( hwnd, msg, w_param, l_param );
}

LRESULT APP_WND::Wnd_Proc_Thunk( HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param )
{
	// retrive ptr & forward msg to our handler
	APP_WND* const p_wnd = reinterpret_cast<APP_WND*>( GetWindowLongPtr( hwnd, GWLP_USERDATA ) );
	return p_wnd->Handle_Msg( hwnd, msg, w_param, l_param );
}
