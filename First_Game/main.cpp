#include "Game.hpp"

Game *game = nullptr;

int main(int argc, char *argv[]) {
	game = new Game();
	game->init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, true);

	while (game->running()) {
		game->handle_events();
		game->update();
		game->render();
	}
	game->clean();
	return 0;
}