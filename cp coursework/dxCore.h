#pragma once
#include "adaptor.h"
#include "memory.h"
#include "mathLibrary.h"

struct InstanceData
{
	Vec3 position;
};

class DxCore
{
public:
ID3D11Device* device;
ID3D11DeviceContext* devicecontext;
IDXGISwapChain* swapchain;

ID3D11RenderTargetView* backbufferRenderTargetView;
ID3D11Texture2D* backbuffer;
ID3D11DepthStencilView* depthStencilView;
ID3D11Texture2D* depthbuffer;
D3D11_VIEWPORT viewport;
ID3D11RasterizerState* rasterizerState;

// Deferred Rendering resources
ID3D11Texture2D* gBufferTextures[3];               // G-buffer textures (Albedo, Normal, Depth)
ID3D11RenderTargetView* gBufferRTVs[3];            // Render target views for G-buffer
ID3D11ShaderResourceView* gBufferSRVs[3];          // Shader resource views for G-buffer

void Init(int width , int height, HWND hwnd, bool window_fullscreen)
	{
      configureSwapChain(width, height, hwnd, window_fullscreen);
	   configureDepthBuffer(width, height);
    	setViewport(width, height);
	   configRasterize();
	   configBlendState();
	}

void InitDeferred(int width, int height, HWND hwnd, bool window_fullscreen)
{
	configureSwapChain(width, height, hwnd, window_fullscreen);
	configureDepthBuffer(width, height);
	setViewport(width, height);
	configRasterize();
	configBlendState();

	// 延迟渲染特有的G-Buffer配置
	configGBuffer(width, height);
}

void configureSwapChain(int width, int height, HWND hwnd, bool window_fullscreen)
{
	DXGI_SWAP_CHAIN_DESC sd;
	memset(&sd, 0, sizeof(DXGI_SWAP_CHAIN_DESC));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = window_fullscreen ? false : true;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL fl;
	fl = D3D_FEATURE_LEVEL_11_0;

	Adapter a;
	a.findAdapter();

	D3D11CreateDeviceAndSwapChain(a.adapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		&fl,
		1,
		D3D11_SDK_VERSION,
		&sd,
		&swapchain,
		&device,
		NULL,
		&devicecontext);

	swapchain->SetFullscreenState(window_fullscreen, NULL);
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);
	device->CreateRenderTargetView(backbuffer, NULL, &backbufferRenderTargetView);
}

void configureDepthBuffer(int width, int height)
{
	D3D11_TEXTURE2D_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_TEXTURE2D_DESC));
	dsvDesc.Width = width;
	dsvDesc.Height = height;
	dsvDesc.MipLevels = 1;
	dsvDesc.ArraySize = 1;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.SampleDesc.Count = 1;
	dsvDesc.SampleDesc.Quality = 0;
	dsvDesc.Usage = D3D11_USAGE_DEFAULT;
	dsvDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsvDesc.CPUAccessFlags = 0;
	dsvDesc.MiscFlags = 0;

	device->CreateTexture2D(&dsvDesc, NULL, &depthbuffer);
	device->CreateDepthStencilView(depthbuffer, NULL, &depthStencilView);

	// 绑定渲染目标视图和深度模板视图
	devicecontext->OMSetRenderTargets(1, &backbufferRenderTargetView, depthStencilView);
}

void setViewport(int width, int height)
{
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	devicecontext->RSSetViewports(1, &viewport);
}

void configRasterize()
{
	D3D11_RASTERIZER_DESC rsdesc;
	ZeroMemory(&rsdesc, sizeof(D3D11_RASTERIZER_DESC));
	rsdesc.FillMode = D3D11_FILL_SOLID;
	rsdesc.CullMode = D3D11_CULL_NONE;
	device->CreateRasterizerState(&rsdesc, &rasterizerState);
	devicecontext->RSSetState(rasterizerState);
}

void configBlendState()
{
	// Define the blend state descriptor
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// Create the blend state
	ID3D11BlendState* blendState;
	HRESULT hr = device->CreateBlendState(&blendDesc, &blendState);
	

	// Set the blend state in the output merger (OM) stage
	float blendFactor[4] = { 0, 0, 0, 0 };
	UINT sampleMask = 0xffffffff;
	devicecontext->OMSetBlendState(blendState, blendFactor, sampleMask);

	// Release the blend state (not needed anymore after being set)
	blendState->Release();
}

void configGBuffer(int width, int height)
{
	DXGI_FORMAT formats[3] =
	{
		DXGI_FORMAT_R8G8B8A8_UNORM,       // Albedo
		DXGI_FORMAT_R16G16B16A16_FLOAT,   // Normal 
		DXGI_FORMAT_R32_FLOAT             // Depth
	};

	for (int i = 0; i < 3; i++)
	{
		D3D11_TEXTURE2D_DESC texDesc = {};
		texDesc.Width = width;
		texDesc.Height = height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = formats[i];
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		device->CreateTexture2D(&texDesc, NULL, &gBufferTextures[i]);

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = formats[i];
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
		device->CreateRenderTargetView(gBufferTextures[i], &rtvDesc, &gBufferRTVs[i]);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = formats[i];
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;
		device->CreateShaderResourceView(gBufferTextures[i], &srvDesc, &gBufferSRVs[i]);
	}
}

void setGBufferRenderTargets()
{
	devicecontext->OMSetRenderTargets(3, gBufferRTVs, depthStencilView);
}

void clearGBuffer()
{
	float clearColor[4] = { 0,0,0,0 };
	for (int i = 0; i < 3; i++)
	{
		devicecontext->ClearRenderTargetView(gBufferRTVs[i], clearColor);
	}
	devicecontext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void clear()
{
	float ClearColour[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
	devicecontext->ClearRenderTargetView(backbufferRenderTargetView, ClearColour);
	devicecontext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,1.0f,0);

}

void present()
{
	swapchain->Present(0, 0);
}
};
