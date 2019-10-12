#pragma once

#include "AdapterReader.h"
#include "Shaders.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"

// In 32 Debug
#include "SpriteBatch.h"
#include "SpriteFont.h"

#include "WICTextureLoader.h"

class Graphics {
private:
	Microsoft::WRL::ComPtr<ID3D11Device> device; // Used to create buffers
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext; // Set various resources (Ex: shader, textures,etc)
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain; // Swap out frame buffer
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView; // What the buffer will be rendered to.

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> myTexture;

	VertexBuffer<Vertex> vertexBuffer;
	IndexBuffer<DWORD> indexBuffer;
	ConstantBuffer<CB_VS_VertexShader> constantBuffer;

	D3D_FEATURE_LEVEL featureLevel;

	VertexShader vertexShader;
	PixelShader pixelShader;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;

	bool InitializeDirectX(HWND handle, int width, int height);
	bool InitializeShaders();
	bool InitializeScene();
public:
	bool Initialize(HWND handle, int width, int height);
	void RenderFrame();
};

