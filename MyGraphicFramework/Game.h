#pragma once

#include <wrl.h>
#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>
#include <vector>
#include <mutex>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include "InputDevice.h"
#include "GameComponent.h"
#include "DisplayWin32.h"

class DisplayWin32;

class Game {
public:
  DisplayWin32* window;
  std::vector<GameComponent*> components;
  InputDevice* inputDevice;
  MSG msg = {};
  bool isExitRequested = false;

  int screenWidth;
  int screenHeight;

  LPCWSTR applicationName = L"MyGraphicFramework";

  void Initialize(
    int screenWidth,
    int screenHeight
  );

  static Game* GetInstance() {
    if (instance == nullptr) {
      if (instance == nullptr) {
        instance = new Game();
      }
    }
    return instance;
  }

  void Run();
  void CreateBackBuffer();
  void Update();
  void PrepareFrame();
  void PrepareViewport(
    float Width,
    float Height,
    float TopLeftX,
    float TopLeftY,
    float MinDepth,
    float MaxDepth
  );
  void Draw();
  void MessageHandler();
  void EndFrame();
  void DestroyResources();
  int Exit();

  ID3D11Device* device;
  ID3D11DeviceContext* context;
  IDXGISwapChain* swapChain;
  ID3D11RenderTargetView* renderView;
  ID3D11Texture2D* backBuffer;

  InputDevice* input;

  std::chrono::steady_clock::time_point PrevTime;
  float totalTime = 0;
  unsigned int frameCount = 0;

private:
  Game();
  static Game* instance;
};