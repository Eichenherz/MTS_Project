#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <string>

class APP_WND
{
public:
	APP_WND( HINSTANCE h_inst );
	~APP_WND();

	APP_WND( const APP_WND& ) = delete;
	APP_WND( APP_WND&& ) = delete;
	APP_WND& operator=( const APP_WND& ) = delete;
	APP_WND& operator=( APP_WND&& ) = delete;


	bool Process_Message();

private:
	static LRESULT CALLBACK Wnd_Proc( HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param );
	static LRESULT CALLBACK Wnd_Proc_Thunk( HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param );
	LRESULT	Handle_Msg( HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param );
	
private:
	// WIN32 Attributes
	HINSTANCE				h_app_inst;
	HWND					h_app_wnd = NULL;
	static constexpr char	name[] = "order*";

};


class GFX
{
private:
	static constexpr UINT width = 800;
	static constexpr UINT height = 600;
};

class Game
{
public:
	//  Init, Logic & Drawing
	bool Init();
	void Update();
	void Render();
};