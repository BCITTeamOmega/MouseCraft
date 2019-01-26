#include <Windows.h>
#include "Game.h"
#include "Core/OmegaEngine.h"

int main() {

	OmegaEngine::instance().initialize();
	OmegaEngine::instance().loop();

    Game game = Game();

	while (true) {
        game.Update(1.0 / 59.94);
		Sleep(1000.0 / 59.94);
	}
}