#include "APP.h"
#include "GFX.h"

int WINAPI WinMain( HINSTANCE h_inst, HINSTANCE, LPSTR p_args, INT )
{
	APP_WND wnd { h_inst };
	GFX gfx { wnd };
	while ( wnd.Process_Message() )
	{
		gfx.Test();
	}
	return 0;
}