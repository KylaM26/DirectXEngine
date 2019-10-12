#include "Shaders.h"

VertexShader::VertexShader() {
	shader = nullptr;
	shaderBuffer = nullptr;
}

bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT elementCount) {
	HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), this->shaderBuffer.GetAddressOf());

	if (FAILED(hr)) {
		Logger::Log(hr, "Failed to read vertex shader from file.");
		return false;
	}

	hr = device->CreateVertexShader(this->shaderBuffer.Get()->GetBufferPointer(), this->shaderBuffer->GetBufferSize(), 0, this->shader.GetAddressOf());

	if (FAILED(hr)) {
		Logger::Log(hr, "Failed to create vertex shader.");
		return false;
	}

	hr = device->CreateInputLayout(layoutDesc, elementCount, this->shaderBuffer->GetBufferPointer(), this->shaderBuffer->GetBufferSize(), this->inputLayout.GetAddressOf());

	if (FAILED(hr)) {
		Logger::Log(hr, "Failed to create input layout.");
		return false;
	}


	return true;
}

ID3D11VertexShader* VertexShader::GetShader() {
	return this->shader.Get();
}

ID3D10Blob* VertexShader::GetBuffer() {
	return this->shaderBuffer.Get();
;
}

ID3D11InputLayout* VertexShader::GetInputLayout() {
	return this->inputLayout.Get();
}

bool PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath) {
	
	HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), this->shaderBuffer.GetAddressOf());

	if (FAILED(hr)) {
		Logger::Log(hr, "Failed to read pixel shader from file.");
		return false;
	}

	hr = device->CreatePixelShader(this->shaderBuffer.Get()->GetBufferPointer(), this->shaderBuffer.Get()->GetBufferSize(), 0, this->shader.GetAddressOf());
	
	if(FAILED(hr)) {
		Logger::Log(hr, "Failed to create pixel shader.");
		return false;
	}

	return true;
}

ID3D11PixelShader* PixelShader::GetShader() {
	return this->shader.Get();
}

ID3D10Blob* PixelShader::GetShaderBuffer() {
	return this->shaderBuffer.Get();
}


