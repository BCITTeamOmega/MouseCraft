#include <Windows.h>
#include <iostream>
#include "Game.h"
#include "Loading/ModelLoader.h"

int main() {
    Game game = Game();

	/*
		Test code; remove later
	*/
	std::cout << "Test" << std::endl;
	ModelLoader::loadModel("teapot.obj");
	/*
		End of test code
	*/
	
	while (true) {
        game.Update(1.0 / 59.94);
		Sleep(1000.0 / 59.94);
	}
}