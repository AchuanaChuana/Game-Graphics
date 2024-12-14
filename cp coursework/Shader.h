#pragma once
#include "dxCore.h"
#include "ShaderReflection.h"
#include <d3d11shader.h>
#include <string>
#include <D3Dcompiler.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include "mathLibrary.h"
#pragma comment(lib,"dxguid.lib")

class Shaders
{
public:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;
	ID3D11Buffer* cb;
	ConstantBuffer constBufferCPU;
	std::vector<ConstantBuffer> psConstantBuffers;
	std::vector<ConstantBuffer> vsConstantBuffers;
	std::map<std::string, int> textureBindPointsVS;
	std::map<std::string, int> textureBindPointsPS;
	std::map<std::string, int> textureBindPoints;
	bool hasLayout=1;

	std::string readFile(std::string filename)
	{
		std::ifstream file(filename);
		std::stringstream buffer;
		buffer << file.rdbuf();
		file.close();
		return buffer.str();
	}

	void initStatic(std::string vs, std::string ps, DxCore* core)
	{
		// Read the vertex shader and pixel shader .txt files
		std::string vsShader = readFile(vs);
		std::string psShader = readFile(ps);

		initConstBuffer(sizeof(ConstantBuffer) + (16 - sizeof(ConstantBuffer) % 16), core);
		// Compile & create vertex shader and pixel shader
		loadVSStatic(core, vsShader);
		loadPS(core, psShader);
	}

	void initStaticins(std::string vs, std::string ps, DxCore* core)
	{
		std::string vsShader = readFile(vs);
		std::string psShader = readFile(ps);

		initConstBuffer(sizeof(ConstantBuffer) + (16 - sizeof(ConstantBuffer) % 16), core);
		// Compile & create vertex shader and pixel shader
		loadVSStaticins(core, vsShader);
		loadPS(core, psShader);
	}

	void initAnimated(std::string vs, std::string ps, DxCore* core)
	{
		// Read the vertex shader and pixel shader .txt files
		std::string vsShader = readFile(vs);
		std::string psShader = readFile(ps);

		initConstBuffer(sizeof(ConstantBuffer) + (16 - sizeof(ConstantBuffer) % 16), core);
		// Compile & create vertex shader and pixel shader
		loadVSAnimated(core,vsShader);
		loadPS(core,psShader);
	}

	void initDeferred(std::string vs, std::string ps, DxCore* core)
	{
		std::string vsShader = readFile(vs);
		std::string psShader = readFile(ps);

		initConstBuffer(sizeof(ConstantBuffer) + (16 - sizeof(ConstantBuffer) % 16), core);

		// 加载专门写入G-Buffer的VS和PS
		loadVSStatic(core, vsShader);
		loadPS(core, psShader);
	}

	void initConstBuffer(int sizeInBytes, DxCore*core)
	{
		constBufferCPU.time = 0.f;
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA data;
		bd.ByteWidth = sizeInBytes;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		core->device->CreateBuffer(&bd, NULL, &cb);
	}

	void loadVSStatic(DxCore* core,std::string vertexShaderHLSL)
	{
		ID3DBlob* compiledVertexShader;
		ID3DBlob* status;
		HRESULT hr = D3DCompile(vertexShaderHLSL.c_str(), strlen(vertexShaderHLSL.c_str()), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &compiledVertexShader, &status);

		if (FAILED(hr))
		{
			MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Vertex Shader Error", 0);
			exit(0);
		}
		core->device->CreateVertexShader(compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), NULL, &vertexShader);

		D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		core->device->CreateInputLayout(layoutDesc, 4, compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), &layout);

		ConstantBufferReflection reflection;
		reflection.build(core, compiledVertexShader, vsConstantBuffers, textureBindPointsVS, ShaderStage::VertexShader);
		compiledVertexShader->Release();
	}

	void loadVSStaticins(DxCore* core, std::string vertexShaderHLSL)
	{
		ID3DBlob* compiledVertexShader;
		ID3DBlob* status;
		HRESULT hr = D3DCompile(vertexShaderHLSL.c_str(), vertexShaderHLSL.length(), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &compiledVertexShader, &status);

		if (FAILED(hr))
		{
			MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Vertex Shader Error", 0);
			exit(0);
		}
		core->device->CreateVertexShader(compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), NULL, &vertexShader);

		D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			 { "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            	{ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            	{ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	            { "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		};

		core->device->CreateInputLayout(layoutDesc, 8, compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), &layout);

		ConstantBufferReflection reflection;
		reflection.build(core, compiledVertexShader, vsConstantBuffers, textureBindPointsVS, ShaderStage::VertexShader);
		compiledVertexShader->Release();
	}

	void loadVSAnimated(DxCore* core, std::string vertexShaderHLSL)
	{
		ID3DBlob* compiledVertexShader;
		ID3DBlob* status;
		HRESULT hr = D3DCompile(vertexShaderHLSL.c_str(), strlen(vertexShaderHLSL.c_str()), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &compiledVertexShader, &status);

		if (FAILED(hr))
		{
			MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Vertex Shader Error", 0);
			exit(0);
		}

		core->device->CreateVertexShader(compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), NULL, &vertexShader);

		D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			 { "BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		core->device->CreateInputLayout(layoutDesc, 6, compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), &layout);

		ConstantBufferReflection reflection;
		reflection.build(core, compiledVertexShader, vsConstantBuffers, textureBindPointsVS, ShaderStage::VertexShader);
		compiledVertexShader->Release();
	}

	void loadPS(DxCore* core, std::string filename)
	{
		ID3DBlob* shader;
		ID3DBlob* status;
		HRESULT hr = D3DCompile(filename.c_str(), strlen(filename.c_str()), NULL, NULL, NULL, "PS", "ps_5_0", 0, 0, &shader, &status);
		if (FAILED(hr))
		{
			printf("%s\n", (char*)status->GetBufferPointer());
			exit(0);
		}
		core->device->CreatePixelShader(shader->GetBufferPointer(), shader->GetBufferSize(), NULL, &pixelShader);
		ConstantBufferReflection reflection;
		reflection.build(core, shader, psConstantBuffers, textureBindPointsPS, ShaderStage::PixelShader);
	}

	void apply(DxCore* core)
	{
		if (hasLayout == 1)
		{
			core->devicecontext->IASetInputLayout(layout);
		}
		else
		{
			core ->devicecontext ->IASetInputLayout(NULL);
		}
		core->devicecontext->VSSetShader(vertexShader, NULL, 0);
		core->devicecontext->PSSetShader(pixelShader, NULL, 0);
		core->devicecontext->PSSetConstantBuffers(0, 1, &cb);

		for (int i = 0; i < vsConstantBuffers.size(); i++)
		{
			vsConstantBuffers[i].upload(core);
		}

		for (int i = 0; i < psConstantBuffers.size(); i++)
		{
			psConstantBuffers[i].upload(core);
		}


	}

	void updateConstant(std::string constantBufferName, std::string variableName, void* data, std::vector<ConstantBuffer>& buffers)
	{
		for (int i = 0; i < buffers.size(); i++)
		{
			if (buffers[i].name == constantBufferName)
			{
				buffers[i].update(variableName, data);
				return;
			}
		}
	}

	void updateConstantVS(std::string constantBufferName, std::string variableName, void* data)
	{
		updateConstant(constantBufferName, variableName, data, vsConstantBuffers);
	}

	void updateConstantPS(std::string constantBufferName, std::string variableName, void* data)
	{
		updateConstant(constantBufferName, variableName, data, psConstantBuffers);
	}

	void updateLightingPS(std::string constantBufferName, std::string variableName, void* data)
	{
		// 直接在当前着色器实例的psConstantBuffers中查找并更新
		for (int i = 0; i < (int)psConstantBuffers.size(); i++)
		{
			if (psConstantBuffers[i].name == constantBufferName)
			{
				psConstantBuffers[i].update(variableName, data);
				return;
			}
		}
		// 若未找到可输出调试信息
		// printf("updateLightingPS failed: CB=%s Var=%s not found.\n", constantBufferName.c_str(), variableName.c_str());
	}

	void updateTexturePS(DxCore* core, std::string name, ID3D11ShaderResourceView* srv)
	{
		auto it = textureBindPointsPS.find(name);
		core->devicecontext->PSSetShaderResources(textureBindPointsPS[name], 1, &srv);
	}
};

struct FullscreenVertex
{
	float x, y, z;
	float u, v;
};

class LightingPass
{
public:
	Shaders lightingShaders;
	ID3D11Buffer* fullScreenVB = nullptr;

	void initLighting(DxCore* core, std::string vsFile, std::string psFile)
	{
		lightingShaders.initConstBuffer(sizeof(ConstantBuffer) + (16 - sizeof(ConstantBuffer) % 16), core);

		std::string vsHLSL = lightingShaders.readFile(vsFile);
		std::string psHLSL = lightingShaders.readFile(psFile);

		loadVSForLighting(core, vsHLSL);
		loadPSForLighting(core, psHLSL);
		initFullScreenQuad(core);
	}

	void renderLightingPass(DxCore* core, ID3D11ShaderResourceView* gBufferSRVs[3])
	{
		lightingShaders.apply(core);

		core->devicecontext->PSSetShaderResources(0, 1, &gBufferSRVs[0]); 
		core->devicecontext->PSSetShaderResources(1, 1, &gBufferSRVs[1]);
		core->devicecontext->PSSetShaderResources(2, 1, &gBufferSRVs[2]); 

		drawFullScreenQuad(core);
	}

	void loadVSForLighting(DxCore* core, const std::string& vsHLSL)
	{
		ID3DBlob* compiledVS = nullptr;
		ID3DBlob* status = nullptr;
		HRESULT hr = D3DCompile(vsHLSL.c_str(), vsHLSL.size(), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &compiledVS, &status);
		if (FAILED(hr))
		{
			MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Lighting Vertex Shader Error", 0);
			exit(0);
		}
		core->device->CreateVertexShader(compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), NULL, &lightingShaders.vertexShader);

		D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		core->device->CreateInputLayout(layoutDesc, 2, compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), &lightingShaders.layout);

		ConstantBufferReflection reflection;
		reflection.build(core, compiledVS, lightingShaders.vsConstantBuffers, lightingShaders.textureBindPointsVS, ShaderStage::VertexShader);

		compiledVS->Release();
	}

	void loadPSForLighting(DxCore* core, const std::string& psHLSL)
	{
		ID3DBlob* compiledPS = nullptr;
		ID3DBlob* status = nullptr;
		HRESULT hr = D3DCompile(psHLSL.c_str(), psHLSL.size(), NULL, NULL, NULL, "PS", "ps_5_0", 0, 0, &compiledPS, &status);
		if (FAILED(hr))
		{
			printf("%s\n", (char*)status->GetBufferPointer());
			exit(0);
		}
		core->device->CreatePixelShader(compiledPS->GetBufferPointer(), compiledPS->GetBufferSize(), NULL, &lightingShaders.pixelShader);

		ConstantBufferReflection reflection;
		reflection.build(core, compiledPS, lightingShaders.psConstantBuffers, lightingShaders.textureBindPointsPS, ShaderStage::PixelShader);
		compiledPS->Release();
	}

	void initFullScreenQuad(DxCore* core)
	{

		FullscreenVertex quadVertices[6] = {
			{ -1.0f, -1.0f, 0.0f, 0.0f, 1.0f}, 
			{ -1.0f,  1.0f, 0.0f, 0.0f, 0.0f}, 
			{  1.0f,  1.0f, 0.0f, 1.0f, 0.0f}, 

			{ -1.0f, -1.0f, 0.0f, 0.0f, 1.0f}, 
			{  1.0f,  1.0f, 0.0f, 1.0f, 0.0f}, 
			{  1.0f, -1.0f, 0.0f, 1.0f, 1.0f}  
		};

		D3D11_BUFFER_DESC bd = {};
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = (UINT)(sizeof(FullscreenVertex) * 6);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = quadVertices;

		HRESULT hr = core->device->CreateBuffer(&bd, &initData, &fullScreenVB);
		if (FAILED(hr))
		{
			std::cerr << "Failed to create fullscreen quad vertex buffer.\n";
			exit(0);
		}
	}

	void drawFullScreenQuad(DxCore* core)
	{
		UINT stride = sizeof(FullscreenVertex);
		UINT offset = 0;
		core->devicecontext->IASetInputLayout(lightingShaders.layout);
		core->devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		core->devicecontext->IASetVertexBuffers(0, 1, &fullScreenVB, &stride, &offset);
		core->devicecontext->Draw(6, 0);
	}
};

//class shaderManager
//{
//public:
//	std::map<std::string, Shaders>shaders;
//
//	void load(std::string& name, std::string& vsFilename, std::string& psFilename, DxCore* core)
//	{
//		Shaders shader;
//		shader.loadVS(core,vsFilename);
//		shader.loadPS(core, psFilename);
//		shaders[name] = shader;
//	}
//
//	Shaders* getShader(std::string& name)
//	{
//		auto it = shaders.find(name);
//		if (it != shaders.end())
//		{
//			return &it->second;
//		}
//		return nullptr;
//	}
//
//	void apply(std::string& name, DxCore* core)
//	{
//		Shaders* shader = getShader(name);
//		if (shader)
//		{
//			shader->apply(core);
//		}
//	}
//
//};