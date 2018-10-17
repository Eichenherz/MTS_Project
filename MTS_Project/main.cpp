#include "APP.h"
#include <conio.h>

int WINAPI WinMain( HINSTANCE h_inst, HINSTANCE, LPSTR p_args, INT )
{
	APP_WND wnd { h_inst };
	while ( wnd.Process_Message() )
	{
		// run game loop
	}
	return 0;
}