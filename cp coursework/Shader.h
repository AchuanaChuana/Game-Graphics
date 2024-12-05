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
			{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		core->device->CreateInputLayout(layoutDesc, 4, compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), &layout);

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
			{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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

	void updateTexturePS(DxCore* core, std::string name, ID3D11ShaderResourceView* srv)
	{
		core->devicecontext->PSSetShaderResources(textureBindPointsPS[name], 1, &srv);
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