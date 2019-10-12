#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include "Logger.h"
#include "ConstantBufferTypes.h"

template<class T>
class ConstantBuffer {
private:
	ConstantBuffer(const ConstantBuffer<T>& rhs);

	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	ID3D11DeviceContext* deviceContext; // For mapping and unmapping.
public:
	T  data;

	ConstantBuffer() {
		deviceContext = nullptr;
	}

	ID3D11Buffer* Get() const {
		return buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf() const {
		return buffer.GetAddressOf();
	}
	
	HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext) {
		this->deviceContext = deviceContext;

		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
		desc.StructureByteStride = 0;

		return device->CreateBuffer(&desc, 0, buffer.GetAddressOf());
	}

	bool Update() {
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT result = this->deviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		
		if (FAILED(result)) {
			Logger::Log(result, "Failed to map constant buffer.");
			return false;
		}
		
		CopyMemory(mappedResource.pData, &data, sizeof(T));
		this->deviceContext->Unmap(buffer.Get(), 0);

		return true;
	}
};