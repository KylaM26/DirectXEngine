#include "Graphics.h"

bool Graphics::Initialize(HWND handle, int width, int height) {
	if (!InitializeDirectX(handle, width, height))
		return false;

	if (!InitializeShaders())
		return false;

	if (!InitializeScene())
		return false;

	return true;
}

static float bufferOffset = 0.0f;
void Graphics::RenderFrame() {
	float bgColor[] = { .0f, .0f, .0f, 1.0f };

	this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgColor);
	this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	this->deviceContext->IASetInputLayout(this->vertexShader.GetInputLayout());
	this->deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->deviceContext->RSSetState(this->rasterizerState.Get());
	this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
	this->deviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());

	this->deviceContext->VSSetShader(vertexShader.GetShader(), 0, 0);
	this->deviceContext->PSSetShader(pixelShader.GetShader(), 0, 0);

	UINT offset = 0;

	bufferOffset += 0.02;

	constantBuffer.data.xOffset = cosf(bufferOffset) * .5f, constantBuffer.data.yOffset = sinf(bufferOffset) * .5f;

	if (!constantBuffer.Update())
		return;

	this->deviceContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	
	// Setting texture in shader
	this->deviceContext->PSSetShaderResources(0, 1, this->myTexture.GetAddressOf());

	// FIRST
	this->deviceContext->IASetVertexBuffers(0, 1, this->vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset);
	this->deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	this->deviceContext->DrawIndexed(indexBuffer.BufferSize(), 0, 0);

	// Text rendering
	spriteBatch->Begin();
	spriteFont->DrawString(spriteBatch.get(), L"IncredibleX Game Engine", DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(1.0f, 1.0f));
	spriteBatch->End();

	this->swapChain->Present(1, 0);
}

bool Graphics::InitializeDirectX(HWND handle, int width, int height) {
	std::vector<AdapterData> adapters = AdapterReader::GetAdapterData();

	if (adapters.size() < 1) {
		Logger::Log("No IDXGI Adapters found.");
		return false;
	}

	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE, // Best driver
		D3D_DRIVER_TYPE_WARP, // CPU
		D3D_DRIVER_TYPE_REFERENCE // Worst
	};

	D3D_FEATURE_LEVEL featureLevels[] = { // For latest features
		D3D_FEATURE_LEVEL_11_0,
	};

	UINT driverCount = ARRAYSIZE(driverTypes);
	UINT featureCount = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // For RGBA
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = handle;

	scd.Windowed = TRUE;

	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT result;
	for (int i = 0; i < driverCount;) {
		result = D3D11CreateDeviceAndSwapChain(0, driverTypes[i], 0, 0, featureLevels, featureCount,
												D3D11_SDK_VERSION, &scd, this->swapChain.GetAddressOf(), 
												this->device.GetAddressOf(), &featureLevel, this->deviceContext.GetAddressOf());

		// If first driver succeeds, break out of this loop.
		if (SUCCEEDED(result))
			break;

		++i;
	}


	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

	result = this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));

	if (FAILED(result)) {
		Logger::Log(result, "Failed to get backbuffer.");
		return false;
	}

	result = this->device->CreateRenderTargetView(backBuffer.Get(), 0, this->renderTargetView.GetAddressOf());

	if (FAILED(result)) {
		Logger::Log(result, "Failed to create render target view.");
		return false;
	}

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	result = this->device->CreateTexture2D(&depthStencilDesc, 0, this->depthStencilBuffer.GetAddressOf());

	if (FAILED(result)) {
		Logger::Log(result, "Failed to create depth stencil buffer.");
		return false;
	}

	result = this->device->CreateDepthStencilView(this->depthStencilBuffer.Get(), 0, this->depthStencilView.GetAddressOf());

	if (FAILED(result)) {
		Logger::Log(result, "Failed to create depth stencil buffer.");
		return false;
	}

	this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());

	// Create depth stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilStateDesc.DepthEnable = true;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	result = this->device->CreateDepthStencilState(&depthStencilStateDesc, this->depthStencilState.GetAddressOf());

	if (FAILED(result)) {
		Logger::Log(result, "Failed to create depth stencil state.");
		return false;
	}

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0, viewport.TopLeftY = 0;
	viewport.Width = width, viewport.Height = height;
	viewport.MinDepth = 0.0f, viewport.MaxDepth = 1.0f;

	// Setting viewport
	this->deviceContext->RSSetViewports(1, &viewport);

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	// Font initiaization
	spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
	spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"../Data/Fonts/sourcesanspro.spritefont");

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; // X-COORDINATE
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; // Y-COORDINATE
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; // Z-COORDINATE
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	
	result = this->device->CreateSamplerState(&samplerDesc, this->samplerState.GetAddressOf());

	if (FAILED(result)) {
		Logger::Log(result, "Failed to create sampler");
		return false;
	}

	return true;
}

bool Graphics::InitializeShaders() {
std::wstring shaderFolder;
#pragma region DetermineShaderPath
	if (IsDebuggerPresent()) {
#ifdef _DEBUG
	#ifdef _WIN64
		shaderFolder = L"../x64/Debug/";
	#else
		shaderFolder = L"../Debug/";
	#endif
#else // Release mode
	#ifdef _WIN64
		shaderFolder = L"../x64/Release/";
	#else
		shaderFolder = L"../Release/";
	#endif
#endif

	}


	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT elementCount = ARRAYSIZE(layout);

	if (!vertexShader.Initialize(this->device, shaderFolder + L"vertexshader.cso", layout, elementCount))
		return false;

	if (!pixelShader.Initialize(this->device, shaderFolder + L"pixelshader.cso"))
		return false;

	return true;
}

bool Graphics::InitializeScene() {
	Vertex v[] = {
		// Order has to be clock-wise for default rasterizer state
		Vertex(Vector3(-0.5f, -0.5f, 1.0f),		Vector2(0.0f, 1.0f)), // bottom left [0]
		Vertex(Vector3(-0.5f, 0.5f, 1.0f),		Vector2(0.0f, 0.0f)), // top left [1]
		Vertex(Vector3(0.5f, 0.5f, 1.0f),		Vector2(1.0f, 0.0f)), // right top [2]
		Vertex(Vector3(0.5f, -0.5f, 1.0f),		Vector2(1.0f, 1.0f)) // right bottom[3]	
	};

	DWORD indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	
	vertexBuffer.Initialize(this->device.Get(), v, ARRAYSIZE(v));
	indexBuffer.Initialize(this->device.Get(), indices, ARRAYSIZE(indices));

	HRESULT hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"../Data/Textures/Wood.jpg", 0, myTexture.GetAddressOf());
	
	if (FAILED(hr)) {
		Logger::Log(hr, "Failed to load texture");
		return false;
	}

	if (FAILED(hr)) {
		Logger::Log(hr, "Failed to load texture");
		return false;
	}

	hr = this->constantBuffer.Initialize(this->device.Get(), this->deviceContext.Get());

	if (FAILED(hr)) {
		Logger::Log(hr, "Failed to initialize constant buffer");
		return false;
	}


	return true;
}
