#pragma once
#include "GameComponent.h"

#include <wrl.h>
#include <iostream>
#include <algorithm>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <vector>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

class TriangleComponent : public GameComponent {
public:
  TriangleComponent(Game* game) : GameComponent(game) {
    vertexShader == nullptr;
    pixelShader = nullptr;
    inputLayout = nullptr;
    vertexBuffer = nullptr;
    pixelShader = nullptr;
    pixelByteCode = nullptr;
    indexBuffer = nullptr;
    rastState = nullptr;
  }

  void Initialize(
    LPCWSTR shaderSource,
    std::vector<DirectX::XMFLOAT4> pointsInput,
    std::vector<int> indecesInput,
    std::vector<UINT> stridesInput,
    std::vector<UINT> offsetsInput
  );
  
  virtual void Draw();
  virtual void Update();
  virtual void DestroyResources();

private:
  ID3D11VertexShader* vertexShader;
  ID3DBlob* vertexByteCode;
  ID3D11PixelShader* pixelShader;
  ID3DBlob* pixelByteCode;
  ID3D11InputLayout* inputLayout;
  ID3D11Buffer* vertexBuffer;
  ID3D11Buffer* indexBuffer;
  ID3D11RasterizerState* rastState;

  std::vector<DirectX::XMFLOAT4> verticles;
  std::vector<int> indexesData;
  std::vector<UINT> strides;
  std::vector<UINT> offsets;
};