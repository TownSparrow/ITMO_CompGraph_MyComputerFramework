#include "TriangleComponent.h"
#include "Game.h"

void TriangleComponent::Initialize(LPCWSTR shaderSource,
	std::vector<DirectX::XMFLOAT4> pointsInput, std::vector<int> indexesInput,
	std::vector<UINT> stridesInput, std::vector<UINT> offsetsInput) {
		verticles = pointsInput;
		indexesData = indexesInput;

		strides = stridesInput;
		offsets = offsetsInput;

		ID3DBlob* errorVertexCode = nullptr;
		HRESULT res = D3DCompileFromFile(
			shaderSource,
			nullptr,
			nullptr,
			"VSMain",
			"vs_5_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&vertexByteCode,
			&errorVertexCode
		);

		D3D_SHADER_MACRO Shader_Macros[] = {
			"TEST",
			"1",
			"TCOLOR",
			"float4(0.0f, 1.0f, 0.0f, 1.0f)",
			nullptr,
			nullptr 
		};
		ID3DBlob* errorPixelCode = nullptr;

		res = D3DCompileFromFile(
			shaderSource,
			Shader_Macros,
			nullptr,
			"PSMain",
			"ps_5_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&pixelByteCode,
			&errorPixelCode
		);

		game->device->CreateVertexShader(
			vertexByteCode->GetBufferPointer(),
			vertexByteCode->GetBufferSize(),
			nullptr, &vertexShader
		);

		game->device->CreatePixelShader(
			pixelByteCode->GetBufferPointer(),
			pixelByteCode->GetBufferSize(),
			nullptr,
			&pixelShader
		);

		D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	game->device->CreateInputLayout(
		inputElements,
		2,
		vertexByteCode->GetBufferPointer(),
		vertexByteCode->GetBufferSize(),
		&inputLayout);

	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * verticles.size();

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = verticles.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	game->device->CreateBuffer(
		&vertexBufDesc,
		&vertexData,
		&vertexBuffer
	);

	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * std::size(indexesData);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indexesData.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	game->device->CreateBuffer(
		&indexBufDesc,
		&indexData,
		&indexBuffer
	);

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	res = game->device->CreateRasterizerState(&rastDesc, &rastState);
}

void TriangleComponent::Draw() {
	game->context->RSSetState(rastState);

	game->context->IASetInputLayout(inputLayout);
	game->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	game->context->IASetIndexBuffer(
		indexBuffer,
		DXGI_FORMAT_R32_UINT,
		0
	);
	game->context->IASetVertexBuffers(
		0,
		1,
		&vertexBuffer,
		strides.data(),
		offsets.data()
	);
	game->context->VSSetShader(
		vertexShader,
		nullptr, 
		0
	);
	game->context->PSSetShader(
		pixelShader,
		nullptr,
		0
	);
	game->context->DrawIndexed(indexesData.size(), 0, 0);
}

void TriangleComponent::Update() {
	// ???
}

void TriangleComponent::DestroyResources() {
	inputLayout->Release();
	vertexShader->Release();
	vertexByteCode->Release();
	vertexBuffer->Release();
	pixelShader->Release();
	pixelByteCode->Release();
	indexBuffer->Release();
	rastState->Release();
}
