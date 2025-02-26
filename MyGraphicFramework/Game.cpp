#include "Game.h"
#include "TriangleComponent.h"
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <chrono>
#include <iostream>
#include <windowsx.h>

Game* Game::instance = nullptr;

// --- Constructor --- //
Game::Game() : 
  device(nullptr),
  context(nullptr),
  swapChain(nullptr),
  renderView(nullptr),
  backBuffer(nullptr),
  window(nullptr),
  input(nullptr),
  screenWidth(0),
  screenHeight(0),
  totalTime(0.0f),
  frameCount(0),
  isExitRequested(false) {}

// --- Init --- //
void Game::Initialize(
  int sw,
  int sh
) {
  screenWidth = sw;
  screenHeight = sh;

  window = new DisplayWin32(screenHeight, screenWidth, applicationName);
  window->Display();

  input = new InputDevice(this);

  DXGI_SWAP_CHAIN_DESC swapDesc = {};
  swapDesc.BufferCount = 2;
  swapDesc.BufferDesc.Width = screenWidth;
  swapDesc.BufferDesc.Height = screenHeight;
  swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapDesc.BufferDesc.RefreshRate.Numerator = 60;
  swapDesc.BufferDesc.RefreshRate.Denominator = 1;
  swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
  swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapDesc.OutputWindow = window->hWnd;
  swapDesc.Windowed = true;
  swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
  swapDesc.SampleDesc.Count = 1;
  swapDesc.SampleDesc.Quality = 0;

  D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

  auto res = D3D11CreateDeviceAndSwapChain(
    nullptr,
    D3D_DRIVER_TYPE_HARDWARE,
    nullptr,
    D3D11_CREATE_DEVICE_DEBUG,
    &featureLevel,
    1,
    D3D11_SDK_VERSION,
    &swapDesc,
    &swapChain,
    &device,
    nullptr,
    &context
  );
  
  if (FAILED(res)) {
    // Well, that was unexpected
    MessageBox(
      window->hWnd,
      L"Failed to create DirectX device and swap chain",
      L"Error",
      MB_OK
    );
  }

  CreateBackBuffer();

  res = device->CreateRenderTargetView(
    backBuffer,
    nullptr,
    &renderView
  );

  if (FAILED(res))
  {
    // Well, that was unexpected
    MessageBox(
      window->hWnd,
      L"Failed to create render target view",
      L"Error",
      MB_OK
    );
  }
  
  // --- OMG, POINTS!!! --- //
  // default points and indexes
  std::vector<DirectX::XMFLOAT4> points = {
      DirectX::XMFLOAT4(0.5f,  0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
      DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
      DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
      DirectX::XMFLOAT4(-0.5f,  0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
      DirectX::XMFLOAT4(0.5f,  0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
      DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
      DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
      DirectX::XMFLOAT4(-0.5f,  0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
  };
  std::vector<int> indexes = { 0,1,2, 0,2,3 };
  std::vector<UINT> strides = { sizeof(DirectX::XMFLOAT4) * 2 };
  std::vector<UINT> offsets = { 0 };

  TriangleComponent* triangleX2 = new TriangleComponent(this);
  triangleX2->Initialize(
    L"./Shaders/MainShader.hlsl",
    points,
    indexes,
    strides,
    offsets
  );
  components.push_back(triangleX2);

  /*
  // new points and indexes
  std::vector<DirectX::XMFLOAT4> pointsNew = {
      //0
      DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 2.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),
      //1
      DirectX::XMFLOAT4(-0.175f, 0.1f, 1.0f, 2.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),
      //2
      DirectX::XMFLOAT4(0.175f, 0.1f, 1.0f, 2.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),
      //3
      DirectX::XMFLOAT4(0, 0.2f, 1.0f, 2.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),
      //4
      DirectX::XMFLOAT4(-0.1f, -0.1f, 1.0f, 2.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),
      //5
      DirectX::XMFLOAT4(0.1f, -0.1f, 1.0f, 2.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)
  };
  std::vector<int> indexesNew = { 0,1,2, 0,3,4, 0,3,5 };    
  

  // trying to draw more
  TriangleComponent* triangleNew = new TriangleComponent(this);
  triangleNew->Initialize(
      L"./Shaders/MainShader.hlsl",
      pointsNew,
      indexesNew,
      strides,
      offsets
  );
  components.push_back(triangleNew);
  */

  PrevTime = std::chrono::steady_clock::now();
}

void Game::CreateBackBuffer() {
  auto res = swapChain->GetBuffer(
    0,
    __uuidof(ID3D11Texture2D),
    (void**)&backBuffer
  );

  if (FAILED(res)) {
    // Well, that was unexpected
    MessageBox(
      window->hWnd,
      L"Failed to get back buffer",
      L"Error",
      MB_OK
    );
  }
}

// VIEWPORT BASE
void Game::PrepareViewport(
  float Width,
  float Height,
  float TopLeftX,
  float TopLeftY,
  float MinDepth,
  float MaxDepth
) {

  D3D11_VIEWPORT vp = {};
  vp.Width = Width;
  vp.Height = Height;
  vp.TopLeftX = TopLeftX;
  vp.TopLeftY = TopLeftY;
  vp.MinDepth = MinDepth;
  vp.MaxDepth = MaxDepth;

  context->RSSetViewports(1, &vp);
}

void Game::PrepareFrame() {
  context->ClearState();
  
  // VIEWPORT
  // default (single)
  PrepareViewport(
    static_cast<float>(screenWidth),
    static_cast<float>(screenHeight),
    0,
    0,
    0.0f,
    1.0f
  );
}

void Game::Draw() {
  context->OMSetRenderTargets(1, &renderView, nullptr);
  
  // background color
  float color[] = { totalTime, 0.1f, 0.1f, 1.0f };

  context->ClearRenderTargetView(renderView, color);

  for (auto comp : components) {
    comp->Draw();
  }

  /*
  // modification: doubled viewport
  // left
  PrepareViewport(
    static_cast<float>(screenWidth) * 0.25f,
    static_cast<float>(screenHeight) * 0.25f,
    static_cast<float>(screenWidth) * 0.15f,
    static_cast<float>(screenHeight) * 0.35f,
    0.0f,
    1.0f
  );

  for (auto comp : components) {
    comp->Draw();
  }

  // right
  PrepareViewport(
    static_cast<float>(screenWidth) * 0.25f,
    static_cast<float>(screenHeight) * 0.25f,
    static_cast<float>(screenWidth) * 0.60f,
    static_cast<float>(screenHeight) * 0.35f,
    0.0f,
    1.0f
  );

  for (auto comp : components) {
    comp->Draw();
  }
  */

  context->OMSetRenderTargets(0, nullptr, nullptr);
  swapChain->Present(1, 0);
}

void Game::Update() {
  auto curTime = std::chrono::steady_clock::now();
  float	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - PrevTime).count() / 1000000.0f;
  PrevTime = curTime;

  totalTime += deltaTime;
  frameCount++;

  if (totalTime > 1.0f) {
    float fps = frameCount / totalTime;

    totalTime -= 1.0f;

    WCHAR text[256];
    swprintf_s(text, TEXT("FPS: %f"), fps);
    SetWindowText(window->hWnd, text);

    frameCount = 0;
  }

  PrepareFrame();
  Draw();
  EndFrame();
}

void Game::Run() {
  PrevTime = std::chrono::steady_clock::now();
  totalTime = 0;
  frameCount = 0;

  while (!isExitRequested) {
    MessageHandler();
    Update();
  }  

  Exit();
}

void Game::EndFrame() {
  // something for the future
}

void Game::MessageHandler() {
  while (PeekMessage(
    &msg,
    nullptr,
    0,
    0,
    PM_REMOVE
  )) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  if (msg.message == WM_QUIT) {
    isExitRequested = true;
  }
}

int Game::Exit() {
  DestroyResources();
  return 0;
}

void Game::DestroyResources() {
  window->~DisplayWin32();

  for (auto comp : components)
  {
    comp->DestroyResources();
    comp->~GameComponent();
    delete comp;
  }

  if (context) context->Release();
  if (device) device->Release();
  if (swapChain) swapChain->Release();
  if (backBuffer) backBuffer->Release();
  if (renderView) renderView->Release();

  delete window;
  delete input;
}