#include "DxLib.h"
#include "Game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	Game game;
	bool success=game.Initialize();

	if (success) {
		game.RunLoop();
	}
	
	game.Shutdown();
	return 0;
}