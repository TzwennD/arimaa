#include <iostream>

#include "Game.hpp"
#include "Text_Based_GUI.hpp"

using namespace std;

int main()
{
  cout << "Welcome to Arimaa!!" << endl;
  
    Text_Based_GUI gui;

    Game game = gui.getGame();

    game.start();

    return 0;
}
