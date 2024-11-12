#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
using Microsoft::WRL::ComPtr;

class Graphics
{
private:
	ComPtr<IDXGISwapChain> SwapChain;
	ComPtr<ID3D11Device> Device;
	ComPtr<ID3D11DeviceContext> DeviceContext;
	ComPtr<ID3D11RenderTargetView> RenderTargetView;

private:
	void SetRenderTarget(); // Cat dat RenderTargetView

public: // public Interface
	Graphics(HWND hWnd);
	~Graphics() = default;
	void ClearBufferWithColors(float r, float g, float b, float a); // Thao tac voi buffer
	void DrawTriangle();
	void OnRender();
};

