#include "Game.h"
#include <windows.h>
#include <iostream>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")


int main() {
  int screenWidth = 800;
  int screenHeight = 800;

  Game* game = Game::GetInstance();

  game->Initialize(
    screenWidth,
    screenHeight
  );
  
  game->Run();
  //game->Exit();

  return 0;
}