#include <Windows.h>
#include "Game.h"

int main() {
    Game game = Game();

	while (true) {
        game.Update(1.0 / 59.94);
		Sleep(1000.0 / 59.94);
	}
}