#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

template<class T>
class VertexBuffer {
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	
	std::unique_ptr<UINT> stride;
	UINT bufferSize;

	VertexBuffer(const VertexBuffer<T>& rhs);
public:
	VertexBuffer() {
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

	const UINT Stride() const {
		return this->stride.get();
	}

	const UINT* StridePtr() const {
		return this->stride.get();
	}

	HRESULT Initialize(ID3D11Device* device, T* data, UINT vertexCount) {
		this->bufferSize = vertexCount;
		this->stride = std::make_unique<UINT>(sizeof(T));

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(T) * vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory(&vertexBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
		vertexBufferData.pSysMem = data;

		HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->buffer.GetAddressOf());
		return result;
	}

};

