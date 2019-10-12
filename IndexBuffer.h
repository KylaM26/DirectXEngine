#pragma once

#include <d3d11.h>
#include <wrl/client.h>

template<class T>
class IndexBuffer {
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	UINT bufferSize;
	IndexBuffer(const IndexBuffer& rhs);
public:
	IndexBuffer() {
		bufferSize = 0;
	}

	ID3D11Buffer* Get() const {
		return buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf() const {
		return buffer.GetAddressOf();
	}

	UINT BufferSize() const {
		return this->bufferSize;
	}

	HRESULT Initialize(ID3D11Device* device, DWORD* data, UINT indexCount) {
		this->bufferSize = indexCount;
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(DWORD) * indexCount;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA resoureData;
		resoureData.pSysMem = data;

		HRESULT hr = device->CreateBuffer(&bufferDesc, &resoureData, buffer.GetAddressOf());
		return hr;
	}
};