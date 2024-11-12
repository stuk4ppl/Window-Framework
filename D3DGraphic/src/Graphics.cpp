#include "../include/Graphics.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")

Graphics::Graphics(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC SwapChainDesc = { 0 };
	SwapChainDesc.BufferDesc.Width = 0;
	SwapChainDesc.BufferDesc.Height = 0;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.OutputWindow = hWnd;
	SwapChainDesc.Windowed = true;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChainDesc.Flags = 0;

	HRESULT Result = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&SwapChainDesc,
		&SwapChain,
		&Device,
		nullptr,
		&DeviceContext
	);

	SetRenderTarget();
}

void Graphics::ClearBufferWithColors(float r, float g, float b, float a)
{
	float Color[4] = { r,g,b,a };
	DeviceContext->ClearRenderTargetView(RenderTargetView.Get(), Color);
}

void Graphics::DrawTriangle()
{
	//Thiet lap cac dinh va mau cua dinh
	struct Vertex {
		float pos[3] = { 0.0f, 0.0f, 0.0f };
		float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	};

	Vertex vertices[] = {
		{{0.0f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
		{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
		{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 1.0f, 1.0f}}
	};

	//Tao buffer voi cac dinh da duoc thiet lap de
	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = (ARRAYSIZE(vertices) * sizeof(Vertex));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA dt = { 0 };
	dt.pSysMem = vertices;

	ComPtr<ID3D11Buffer> VertexBuffer;
	Device->CreateBuffer(&bd, &dt, VertexBuffer.GetAddressOf());

	UINT Stride = sizeof(Vertex);
	UINT Offset = 0;

	//Set buffer
	DeviceContext->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &Stride, &Offset);

	//Dua Shader vao chuong trinh de su dung
	ComPtr<ID3DBlob> VSBlob;
	ComPtr<ID3D11VertexShader> VertexShader;
	D3DReadFileToBlob(L"VertexShader.cso", VSBlob.GetAddressOf());
	Device->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, VertexShader.GetAddressOf());
	DeviceContext->VSSetShader(VertexShader.Get(), nullptr, 0);

	ComPtr<ID3DBlob> PSBlob;
	ComPtr<ID3D11PixelShader> PixelShader;
	D3DReadFileToBlob(L"PixelShader.cso", PSBlob.GetAddressOf());
	Device->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, PixelShader.GetAddressOf());
	DeviceContext->PSSetShader(PixelShader.Get(), nullptr, 0);

	//Thiet lap rastersize
	D3D11_RASTERIZER_DESC RasterizerDesc = {};
	RasterizerDesc.FillMode = D3D11_FILL_SOLID;
	RasterizerDesc.CullMode = D3D11_CULL_BACK;
	RasterizerDesc.FrontCounterClockwise = FALSE;

	ComPtr<ID3D11RasterizerState> RasterizerState;
	Device->CreateRasterizerState(&RasterizerDesc, RasterizerState.GetAddressOf());
	DeviceContext->RSSetState(RasterizerState.Get());

	//Xac dinh cach Direct3D doc du lieu tu vertices
	D3D11_INPUT_ELEMENT_DESC Layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ComPtr<ID3D11InputLayout> InputLayout;
	Device->CreateInputLayout(Layout, ARRAYSIZE(Layout), VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), InputLayout.GetAddressOf());
	DeviceContext->IASetInputLayout(InputLayout.Get());

	//Thiet lap hinh muon ve va ve
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DeviceContext->Draw(3, 0);
}

void Graphics::OnRender()
{
	SwapChain->Present(1, 0);
}

void Graphics::SetRenderTarget()
{
	ComPtr<ID3D11Texture2D> BackBuffer;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &BackBuffer);
	if (BackBuffer != nullptr) {
		Device->CreateRenderTargetView(BackBuffer.Get(), nullptr, RenderTargetView.GetAddressOf());
		DeviceContext->OMSetRenderTargets(1, RenderTargetView.GetAddressOf(), nullptr);
	}
}
