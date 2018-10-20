#pragma once

#include "FrameworkWin.h"
#include "GFX.h"
#include <string>

class HWNDKey
{
private:
	HWNDKey( const HWNDKey& ) = delete;
	HWNDKey& operator=( const HWNDKey& ) = delete;
protected:
	HWNDKey() = default;
private:
	friend GFX::GFX( HWNDKey& key );
protected:
	HWND h_app_wnd = nullptr;
};

class APP_WND : public HWNDKey
{
public:
	APP_WND( HINSTANCE h_inst );
	~APP_WND();

	APP_WND( const APP_WND& ) = delete;
	APP_WND( APP_WND&& ) = delete;
	APP_WND& operator=( const APP_WND& ) = delete;
	APP_WND& operator=( APP_WND&& ) = delete;

	bool Process_Message();
	auto Get_WND_Handler();

private:
	static LRESULT CALLBACK Wnd_Proc( HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param );
	static LRESULT CALLBACK Wnd_Proc_Thunk( HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param );
	LRESULT	Handle_Msg( HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param );
	
private:
	// WIN32 Attributes
	HINSTANCE				h_app_inst;
	static constexpr char	name[] = "order*";

};