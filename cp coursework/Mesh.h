#pragma once
#include "mathLibrary.h"
#include "dxCore.h"
#include <d3d11.h>
#include "ShaderReflection.h"
#include "GEMloader.h"

struct STATIC_VERTEX
{
	Vec3 pos;
	Vec3 normal;
	Vec3 tangent;
	float tu;
	float tv;
};

//class Triangle
//{
// public:
//	 ID3D11Buffer* vertexBuffer;
//	 Vertex vertices[3];
//
//	 //define a triagle
//	void Init()
//	{
//	vertices[0].position = Vec3(0, 1.0f, 0);
//	vertices[0].color = Color(0, 1.0f, 0);
//	vertices[1].position = Vec3(-1.0f, -1.0f, 0);
//	vertices[1].color = Color(1.0f, 0, 0);
//	vertices[2].position = Vec3(1.0f, -1.0f, 0);
//	vertices[2].color = Color(0, 0, 1.0f);
//	
//	}
//
//	//create a buffer in GPU to store verts
//	void createBuffer(ID3D11Device* device)
//	{
//		D3D11_BUFFER_DESC bd;
//		bd.Usage = D3D11_USAGE_DEFAULT;
//		bd.CPUAccessFlags = 0;
//		bd.MiscFlags = 0;
//		D3D11_SUBRESOURCE_DATA uploadData;
//		bd.ByteWidth = sizeof(Vertex) * 3;
//		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//		uploadData.pSysMem = vertices;
//		uploadData.SysMemPitch = 0;
//		uploadData.SysMemSlicePitch = 0;
//		device->CreateBuffer(&bd, &uploadData, &vertexBuffer);
//	}
//
//	//draw triangle
//	void draw(ID3D11DeviceContext* devicecontext)
//	{
//		UINT offsets;
//		offsets = 0;
//		UINT strides = sizeof(Vertex);
//		devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//		devicecontext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
//		devicecontext->Draw(3, 0);
//	}
//
//};

class Mesh
{
public:
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertexBuffer;
	int indicesSize;
	UINT strides;

	void init(DxCore* core,void* vertices, int vertexSizeInBytes, int numVertices, unsigned int* indices, int numIndices) 
	{
		D3D11_BUFFER_DESC bd;
		memset(&bd, 0, sizeof(D3D11_BUFFER_DESC));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * numIndices;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		D3D11_SUBRESOURCE_DATA data;
		memset(&data, 0, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = indices;
		core->device->CreateBuffer(&bd, &data, &indexBuffer);
		bd.ByteWidth = vertexSizeInBytes * numVertices;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		data.pSysMem = vertices;
		core->device->CreateBuffer(&bd, &data, &vertexBuffer);
		indicesSize = numIndices;
		strides = vertexSizeInBytes;
	}

	void init(DxCore* core,std::vector<STATIC_VERTEX> vertices, std::vector<unsigned int> indices)
	{
		init(core,&vertices[0], sizeof(STATIC_VERTEX), vertices.size(), &indices[0], indices.size());
	}

	void draw(ID3D11DeviceContext* devicecontext)
	{
		UINT offsets = 0;
		devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		devicecontext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
		devicecontext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		devicecontext->DrawIndexed(indicesSize, 0, 0);
	}
};

class plane
{
public:
	Mesh mesh;

	STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv)
	{
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		v.tangent = Vec3(0, 0, 0);//For Now
		v.tu = tu;
		v.tv = tv;
		return v;
	}

	void init(DxCore* core)
	{
		std::vector<STATIC_VERTEX> vertices;
		vertices.push_back(addVertex(Vec3(-1000, 0, -1000), Vec3(0, 1, 0), 0, 0));
		vertices.push_back(addVertex(Vec3(1000, 0, -1000), Vec3(0, 1, 0), 1, 0));
		vertices.push_back(addVertex(Vec3(-1000, 0, 1000), Vec3(0, 1, 0), 0, 1));
		vertices.push_back(addVertex(Vec3(1000, 0, 1000), Vec3(0, 1, 0), 1, 1));
		std::vector<unsigned int> indices;
		indices.push_back(2); indices.push_back(1); indices.push_back(0);
		indices.push_back(1); indices.push_back(2); indices.push_back(3);
		mesh.init(core, vertices, indices);
	}

	void draw(DxCore* core)
	{
		mesh.draw(core->devicecontext);
	}
};


//class Sphere
//{
//public:
//
//void init(int rings, int segments, float radius)
//{
//	for (int lat = 0; lat <= rings; lat++) {
//		float theta = lat * PI / rings;
//		float sinTheta = sinf(theta);
//		float cosTheta = cosf(theta);
//		for (int lon = 0; lon <= segments; lon++) {
//			float phi = lon * 2.0f * PI / segments;
//			float sinPhi = sinf(phi);
//			float cosPhi = cosf(phi);
//			Vec3 position(radius * sinTheta * cosPhi, radius * cosTheta, radius * sinTheta * sinPhi);
//			Vec3 normal = position.Normalize();
//			float tu = 1.0f - (float)lon / segments;
//			float tv = 1.0f - (float)lat / rings;
//			vertices.push_back(addVertex(position, normal, tu, tv));
//		}
//	}
//
//}
//};

class StaticMesh
{
public:
	Mesh geo;

	STATIC_VERTEX addVertex(Vec3 p, Vec3 n,Vec3 t, float tu, float tv)
	{
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		v.tangent = t;
		v.tu = tu;
		v.tv = tv;
		return v;
	}

	void init(DxCore* core, const std::string& filename)
	{
		GEMLoader::GEMModelLoader modelLoader;
		std::vector<GEMLoader::GEMMesh> gemmesh;
		modelLoader.load(filename, gemmesh);
		std::vector<STATIC_VERTEX> vertices;
		std::vector<unsigned int> indices;
	  for (int i = 0; i < gemmesh.size(); i++) 
		{
				if (!gemmesh[i].isAnimated())
			    {
					for (int j = 0; j < gemmesh[i].indices.size(); j++)
					{
						int index = gemmesh[i].indices[j]; 
						GEMLoader::GEMVec3 pos = gemmesh[i].verticesStatic[index].position;
						GEMLoader::GEMVec3 normal = gemmesh[i].verticesStatic[index].normal;
						GEMLoader::GEMVec3 tangent = gemmesh[i].verticesStatic[index].tangent;
						float u = gemmesh[i].verticesStatic[index].u;
						float v = gemmesh[i].verticesStatic[index].v;
						vertices.push_back(addVertex(Vec3(pos.x, pos.y, pos.z), Vec3(normal.x, normal.y, normal.z), Vec3(tangent.x, tangent.y, tangent.z), u, v));
						indices.push_back(index);
					}
			    }
		}
		geo.init(core, vertices, indices);
	}

	void draw(DxCore* core)
	{
		geo.draw(core->devicecontext);
	}
};