#pragma once
#include <D3D11.h>
#include <vector>
#include <dxgi1_6.h>

#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "DXGI.lib")

//extern "C" 
//{
//	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
//}

class Adapter
{
public:
	IDXGIAdapter1* adapter;
	void findAdapter() 
	{

		IDXGIAdapter1* adapterf;
		std::vector<IDXGIAdapter1*> adapters;
		IDXGIFactory6* factory = NULL;
		CreateDXGIFactory(__uuidof(IDXGIFactory6), (void**)&factory);
		int i = 0;
		while (factory->EnumAdapters1(i, &adapterf) != DXGI_ERROR_NOT_FOUND)
		{
			adapters.push_back(adapterf);
			i++;
		}
		long long maxVideoMemory = 0;
		int useAdapterIndex = 0;
		for (int i = 0; i < adapters.size(); i++)
		{
			DXGI_ADAPTER_DESC desc;
			adapters[i]->GetDesc(&desc);
			if (desc.DedicatedVideoMemory > maxVideoMemory)
			{
				maxVideoMemory = desc.DedicatedVideoMemory;
				useAdapterIndex = i;
			}
		}
		adapter = adapters[useAdapterIndex];
		factory->Release();
	}
	IDXGIAdapter1* getAdapter() const {
		return adapter;
	}
};