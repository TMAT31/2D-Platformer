#include "Game.h"

Game *game = nullptr;

int help(int x) {
	return x;
}

int main(int argc, char *argv[]) {
	
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;
	
	game = new Game();
	game->init("Game", 800, 640, false);

	while (game->running()) {
		
		frameStart = SDL_GetTicks();
		
		game->handle_events();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < frameDelay) {
			SDL_Delay(frameDelay - frameTime);
		}
	}
	game->clean();
	return 0;
}