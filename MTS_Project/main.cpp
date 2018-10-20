#include "APP.h"
#include "Game.h"

int WINAPI WinMain( HINSTANCE h_inst, HINSTANCE, LPSTR p_args, INT )
{
	APP_WND		wnd { h_inst };
	Game		game { wnd };


	while ( wnd.Process_Message() )
	{
		game.Run();
	}
	return 0;
}