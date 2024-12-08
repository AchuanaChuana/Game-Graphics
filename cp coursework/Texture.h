#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <string>
#include <d3d11.h>
#include "dxCore.h"
#include <map>
#include <DirectXMath.h> 
#include <stdexcept> 

class sampler
{
public:
    ID3D11SamplerState* state;

    void init(DxCore* core)
    {
        D3D11_SAMPLER_DESC samplerDesc;
        ZeroMemory(&samplerDesc, sizeof(samplerDesc));
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        samplerDesc.MinLOD = 0;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
        core->device->CreateSamplerState(&samplerDesc, &state);
    }

};

class Texture
{
 public:
    ID3D11Texture2D* texture;
    ID3D11ShaderResourceView* srv;
    sampler sam;
    bool isNormalMap;

    void init(DxCore* core,float width, float height, int channels, unsigned char* data,DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB)
    {
        D3D11_TEXTURE2D_DESC texDesc;
        memset(&texDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));
        texDesc.Width = width;
        texDesc.Height = height;
        texDesc.MipLevels = 1;
        texDesc.ArraySize = 1;
        texDesc.Format = format;
        texDesc.SampleDesc.Count = 1;
        texDesc.Usage = D3D11_USAGE_DEFAULT;
        texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        texDesc.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA initData;
        memset(&initData, 0, sizeof(D3D11_SUBRESOURCE_DATA));
        initData.pSysMem = data;
        initData.SysMemPitch = width * channels;
        core->device->CreateTexture2D(&texDesc, &initData, &texture);

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        srvDesc.Format = format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = 1;
        core->device->CreateShaderResourceView(texture, &srvDesc, &srv);

    }

	void load(DxCore* core,std::string filename)
	{
        int width = 0;
        int height = 0;
        int channels = 0;
        unsigned char* texels = stbi_load(filename.c_str(), &width, &height, &channels, 0);
        if (channels == 3) 
        {
            channels = 4;
            unsigned char* texelsWithAlpha = new unsigned char[width * height * channels];
            for (int i = 0; i < (width * height); i++)
            {
                texelsWithAlpha[i * 4] = texels[i * 3];
                texelsWithAlpha[(i * 4) + 1] = texels[(i * 3) + 1];
                texelsWithAlpha[(i * 4) + 2] = texels[(i * 3) + 2];
                texelsWithAlpha[(i * 4) + 3] = 255;
            }
            // Initialize texture using width, height, channels, and texelsWithAlpha
            init(core, width, height, channels, texelsWithAlpha);
            delete[] texelsWithAlpha;
        }
        else 
        {
            init(core, width, height, channels, texels);
            // Initialize texture using width, height, channels, and texels
        }
        stbi_image_free(texels);
        sam.init(core);
	}

   /* void free()
    {
        srv->Release();
        texture->Release();
    }*/

    void loadNormalMap(DxCore* core, std::string filename)
    {
        int width = 0, height = 0, channels = 0;
        unsigned char* texels = stbi_load(filename.c_str(), &width, &height, &channels, 0);
        if (!texels)
        {
            throw std::runtime_error("Failed to load normal map: " + filename);
        }

        // 法线贴图格式为 RGBA，确保有 4 个通道
        if (channels < 3)
        {
            stbi_image_free(texels);
            throw std::runtime_error("Invalid normal map format: " + filename);
        }

        // 标记为法线贴图
        isNormalMap = true;

        // 使用 DXGI_FORMAT_R8G8B8A8_UNORM 处理法线贴图
        init(core, width, height, channels, texels, DXGI_FORMAT_R8G8B8A8_UNORM);
        stbi_image_free(texels);
        sam.init(core);
    }
};

class textureManager
{
public:
    std::map<std::string, Texture*> textures;
   // TilingBuffer tilingBuffer;

    void load(DxCore* core, std::string filename)
    {
        std::map<std::string, Texture*>::iterator it = textures.find(filename);
        if (it != textures.end())
        {
            return;
        }
        Texture* texture = new Texture();
        texture->load(core, filename);
        textures.insert({ filename, texture });
    }

    void loadNormalMap(DxCore* core, std::string filename)
    {
        if (textures.find(filename) != textures.end())
        {
            return;
        }

        Texture* texture = new Texture();
        texture->loadNormalMap(core, filename);
   
        textures.insert({ filename, texture });
    }

    ID3D11ShaderResourceView* find(std::string name)
    {
        return textures[name]->srv;
    }

    ID3D11ShaderResourceView* findNormalMap(const std::string& name)
    {
        auto it = textures.find(name);
        if (it != textures.end() && it->second->isNormalMap)
        {
            return it->second->srv;
        }

        printf("Warning: Normal Map %s not found.\n", name.c_str());
        return nullptr;
    }

    ID3D11ShaderResourceView* findResourse(std::string name)
    {
        name = "Resources/" + name;
        return textures[name]->srv;
    }


  /* void unload(std::string name)
    {
        textures[name]->free();
        textures.erase(name);
    }

    ~textureManager()
    {
        for (auto it = textures.cbegin(); it != textures.cend(); )
        {
            it->second->free();
            textures.erase(it++);
        }
    }*/
};