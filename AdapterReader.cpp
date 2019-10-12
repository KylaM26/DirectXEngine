#include "AdapterReader.h"

std::vector<AdapterData> AdapterReader::adapters;


AdapterData::AdapterData(IDXGIAdapter* pAdapter) {
	this->pAdapter = pAdapter;

	HRESULT hr = pAdapter->GetDesc(&this->description);

	if (FAILED(hr)) {
		Logger::Log(hr, "Failed to get description for IDXGIAdapter");
	}
}

std::vector<AdapterData> AdapterReader::GetAdapterData() {
	if (adapters.size() > 0) // If already intiialized
		return adapters;

	Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;

	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));

	if (FAILED(hr)) {
		Logger::Log("Failed to create DXGIFactory for enumatoring adapters.");
		exit(-1);
	}

	IDXGIAdapter* pAdapter;
	UINT index = 0;

	while (SUCCEEDED(pFactory->EnumAdapters(index, &pAdapter))) {
		adapters.push_back(AdapterData(pAdapter));
		index += 1;
	}

	return adapters;
}
