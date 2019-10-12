#pragma once

#include <d3d11.h>
#include <dxgi.h>
#include <vector>
#include <wrl/client.h>
#include "Logger.h"

class AdapterData {
public:
	IDXGIAdapter* pAdapter;
	DXGI_ADAPTER_DESC description;
	AdapterData(IDXGIAdapter* pAdapter);
	
};

class AdapterReader {
public:
	// Static so all objects can use the same set of adapters.
	static std::vector<AdapterData> GetAdapterData();
private:
	static std::vector<AdapterData> adapters;
};

