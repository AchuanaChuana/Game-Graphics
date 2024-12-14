#pragma once
#include "mathLibrary.h"
#include "dxCore.h"
#include <d3d11.h>
#include "GEMloader.h"
#include "Texture.h"
#include "Shader.h"
#include "collision.h"

struct STATIC_VERTEX
{
	Vec3 pos;
	Vec3 normal;
	Vec3 tangent;
	float tu;
	float tv;
};

struct ANIMATED_VERTEX
{
	Vec3 pos;
	Vec3 normal;
	Vec3 tangent;
	float tu;
	float tv;
	unsigned int bonesIDs[4];
	float boneWeights[4];
};

struct Bone
{
	std::string name;
	Matrix44 offset;
	int parentIndex;
};

struct Skeleton
{
	std::vector<Bone> bones;
	Matrix44 globalInverse;
};

struct AnimationFrame
{
	std::vector<Vec3> positions;
	std::vector<Quaternion> rotations;
	std::vector<Vec3> scales;
};

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

	void initAnimated(DxCore* core,std::vector<ANIMATED_VERTEX> vertices, std::vector<unsigned int> indices)
	{
		init(core,&vertices[0], sizeof(ANIMATED_VERTEX), vertices.size(), &indices[0], indices.size());
	}

	void draw(DxCore* core)
	{
		UINT offsets = 0;
		core->devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		core->devicecontext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
		core->devicecontext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		core->devicecontext->DrawIndexed(indicesSize, 0, 0);
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
		vertices.push_back(addVertex(Vec3(-10000, 0, -10000), Vec3(0, 1, 0), 0, 0));
		vertices.push_back(addVertex(Vec3(10000, 0, -10000), Vec3(0, 1, 0), 1, 0));
		vertices.push_back(addVertex(Vec3(-10000, 0, 10000), Vec3(0, 1, 0), 0, 1));
		vertices.push_back(addVertex(Vec3(10000, 0, 10000), Vec3(0, 1, 0), 1, 1));
		std::vector<unsigned int> indices;
		indices.push_back(2); indices.push_back(1); indices.push_back(0);
		indices.push_back(1); indices.push_back(2); indices.push_back(3);
		mesh.init(core, vertices, indices);
	}

	void draw(DxCore* core, Shaders*shader, Matrix44 Worldpos,Matrix44 Transform)
	{
		shader->updateConstantVS("staticMeshBuffer", "W", &Worldpos);
		shader->updateConstantVS("staticMeshBuffer", "VP", &Transform);
		shader->apply(core);
		mesh.draw(core);
	}
};

class planewithTex
{
public:
	Mesh mesh;
	std::string materialFilename;

	STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv)
	{
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		v.tangent = Vec3(0, 0, 0);
		v.tu = tu;
		v.tv = tv;
		return v;
	}

	void init(DxCore* core, textureManager* textures, const std::string& materialPath,float size)
	{
		materialFilename = materialPath;
		std::vector<STATIC_VERTEX> vertices;
		vertices.push_back(addVertex(Vec3(-size/2, 0, -size / 2), Vec3(0, 1, 0), 0, 0));
		vertices.push_back(addVertex(Vec3(size / 2, 0, -size / 2), Vec3(0, 1, 0), 1, 0));
		vertices.push_back(addVertex(Vec3(-size / 2, 0, size / 2), Vec3(0, 1, 0), 0, 1));
		vertices.push_back(addVertex(Vec3(size / 2, 0, size / 2), Vec3(0, 1, 0), 1, 1));
		std::vector<unsigned int> indices;
		indices.push_back(2); indices.push_back(1); indices.push_back(0);
		indices.push_back(1); indices.push_back(2); indices.push_back(3);
		mesh.init(core, vertices, indices);
		textures->load(core, materialFilename);
	}

	void draw(DxCore* core, Shaders* shader, textureManager textures, float tile,std:: string filename,Matrix44 Worldpos, Matrix44 Transform)
	{
		shader->updateConstantVS("staticMeshBuffer", "W", &Worldpos);
		shader->updateConstantVS("staticMeshBuffer", "VP", &Transform);

		shader->updateConstantPS("ScrollParams", "tileTime", &tile);
		shader->apply(core);

		shader->updateTexturePS(core, "tex", textures.find(filename));

		mesh.draw(core);
	}
};

class Box
{
public:
	Mesh box;
	std::string materialFilename;

	STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv)
	{
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		v.tangent = Vec3(0, 0, 0);
		v.tu = tu;
		v.tv = tv;
		return v;
	}

	void init(DxCore* core, textureManager* textures, const std::string& materialPath, float width = 100.0f, float height = 100.0f, float depth = 100.0f)
	{
		materialFilename = materialPath;
		std::vector<STATIC_VERTEX> vertices;
		std::vector<unsigned int> indices;

		float halfW = width * 0.5f;
		float halfH = height * 0.5f;
		float halfD = depth * 0.5f;

		{
			Vec3 normal(0, 0, 1);
			Vec3 v0(-halfW, -halfH, halfD);
			Vec3 v1(halfW, -halfH, halfD);
			Vec3 v2(-halfW, halfH, halfD);
			Vec3 v3(halfW, halfH, halfD);

			unsigned startIndex = (unsigned)vertices.size();
			vertices.push_back(addVertex(v0, normal, 0.0f, 1.0f));
			vertices.push_back(addVertex(v1, normal, 1.0f, 1.0f));
			vertices.push_back(addVertex(v2, normal, 0.0f, 0.0f));
			vertices.push_back(addVertex(v3, normal, 1.0f, 0.0f));

			indices.push_back(startIndex + 0); indices.push_back(startIndex + 1); indices.push_back(startIndex + 2);
			indices.push_back(startIndex + 2); indices.push_back(startIndex + 1); indices.push_back(startIndex + 3);
		}

		{
			Vec3 normal(0, 0, -1);
			Vec3 v0(halfW, -halfH, -halfD);
			Vec3 v1(-halfW, -halfH, -halfD);
			Vec3 v2(halfW, halfH, -halfD);
			Vec3 v3(-halfW, halfH, -halfD);

			unsigned startIndex = (unsigned)vertices.size();
			vertices.push_back(addVertex(v0, normal, 0.0f, 1.0f));
			vertices.push_back(addVertex(v1, normal, 1.0f, 1.0f));
			vertices.push_back(addVertex(v2, normal, 0.0f, 0.0f));
			vertices.push_back(addVertex(v3, normal, 1.0f, 0.0f));

			indices.push_back(startIndex + 0); indices.push_back(startIndex + 1); indices.push_back(startIndex + 2);
			indices.push_back(startIndex + 2); indices.push_back(startIndex + 1); indices.push_back(startIndex + 3);
		}

		{
			Vec3 normal(-1, 0, 0);
			Vec3 v0(-halfW, -halfH, -halfD);
			Vec3 v1(-halfW, -halfH, halfD);
			Vec3 v2(-halfW, halfH, -halfD);
			Vec3 v3(-halfW, halfH, halfD);

			unsigned startIndex = (unsigned)vertices.size();
			vertices.push_back(addVertex(v0, normal, 0.0f, 1.0f));
			vertices.push_back(addVertex(v1, normal, 1.0f, 1.0f));
			vertices.push_back(addVertex(v2, normal, 0.0f, 0.0f));
			vertices.push_back(addVertex(v3, normal, 1.0f, 0.0f));

			indices.push_back(startIndex + 0); indices.push_back(startIndex + 1); indices.push_back(startIndex + 2);
			indices.push_back(startIndex + 2); indices.push_back(startIndex + 1); indices.push_back(startIndex + 3);
		}

		{
			Vec3 normal(1, 0, 0);
			Vec3 v0(halfW, -halfH, halfD);
			Vec3 v1(halfW, -halfH, -halfD);
			Vec3 v2(halfW, halfH, halfD);
			Vec3 v3(halfW, halfH, -halfD);

			unsigned startIndex = (unsigned)vertices.size();
			vertices.push_back(addVertex(v0, normal, 0.0f, 1.0f));
			vertices.push_back(addVertex(v1, normal, 1.0f, 1.0f));
			vertices.push_back(addVertex(v2, normal, 0.0f, 0.0f));
			vertices.push_back(addVertex(v3, normal, 1.0f, 0.0f));

			indices.push_back(startIndex + 0); indices.push_back(startIndex + 1); indices.push_back(startIndex + 2);
			indices.push_back(startIndex + 2); indices.push_back(startIndex + 1); indices.push_back(startIndex + 3);
		}

		{
			Vec3 normal(0, 1, 0);
			Vec3 v0(-halfW, halfH, halfD);
			Vec3 v1(halfW, halfH, halfD);
			Vec3 v2(-halfW, halfH, -halfD);
			Vec3 v3(halfW, halfH, -halfD);

			unsigned startIndex = (unsigned)vertices.size();
			vertices.push_back(addVertex(v0, normal, 0.0f, 1.0f));
			vertices.push_back(addVertex(v1, normal, 1.0f, 1.0f));
			vertices.push_back(addVertex(v2, normal, 0.0f, 0.0f));
			vertices.push_back(addVertex(v3, normal, 1.0f, 0.0f));

			indices.push_back(startIndex + 0); indices.push_back(startIndex + 1); indices.push_back(startIndex + 2);
			indices.push_back(startIndex + 2); indices.push_back(startIndex + 1); indices.push_back(startIndex + 3);
		}

		{
			Vec3 normal(0, -1, 0);
			Vec3 v0(-halfW, -halfH, -halfD);
			Vec3 v1(halfW, -halfH, -halfD);
			Vec3 v2(-halfW, -halfH, halfD);
			Vec3 v3(halfW, -halfH, halfD);

			unsigned startIndex = (unsigned)vertices.size();
			vertices.push_back(addVertex(v0, normal, 0.0f, 1.0f));
			vertices.push_back(addVertex(v1, normal, 1.0f, 1.0f));
			vertices.push_back(addVertex(v2, normal, 0.0f, 0.0f));
			vertices.push_back(addVertex(v3, normal, 1.0f, 0.0f));

			indices.push_back(startIndex + 0); indices.push_back(startIndex + 1); indices.push_back(startIndex + 2);
			indices.push_back(startIndex + 2); indices.push_back(startIndex + 1); indices.push_back(startIndex + 3);
		}

		box.init(core, vertices, indices);
		textures->load(core, materialFilename);
	}

	//AABB getAABB(const Matrix44& worldTransform, Vec3 mini, Vec3 maxi) const
	//{
	//	AABB localAABB;
	//	localAABB.minimum = mini;
	//	localAABB.maximum = maxi;

	//	// 转换AABB到世界坐标系
	//	return collision::transformAABB(localAABB, worldTransform);
	//}

	void draw(DxCore* core, Shaders* shader, textureManager textures, std::string filename, Matrix44 Worldpos, Matrix44 Transform)
	{
		shader->updateConstantVS("staticMeshBuffer", "W", &Worldpos);
		shader->updateConstantVS("staticMeshBuffer", "VP", &Transform);
		shader->apply(core);
		shader->updateTexturePS(core, "tex", textures.find(filename));
		box.draw(core);
	}
};

class Sphere
{
public:
	Mesh sph;
	std::string materialFilename;

	STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv)
	{
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		v.tangent = Vec3(0, 0, 0); // For now
		v.tu = tu;
		v.tv = tv;
		return v;
	}

void init(DxCore* core, textureManager* textures, const std::string& materialPath,int rings, int segments, float radius)
{
	materialFilename = materialPath;
	std::vector<STATIC_VERTEX> vertices;
	std::vector<unsigned int> indices;
	for (int lat = 0; lat <= rings; lat++) {
		float theta = lat * PI / rings;
		float sinTheta = sinf(theta);
		float cosTheta = cosf(theta);
		for (int lon = 0; lon <= segments; lon++) {
			float phi = lon * 2.0f * PI / segments;
			float sinPhi = sinf(phi);
			float cosPhi = cosf(phi);
			Vec3 position(radius * sinTheta * cosPhi, radius * cosTheta, radius * sinTheta * sinPhi);
			Vec3 normal = position.Normalize();
			float tu = 1.0f - (float)lon / segments;
			float tv = 1.0f - (float)lat / rings;
			vertices.push_back(addVertex(position, normal, tu, tv));
			int current = lat * (segments + 1) + lon;
			int next = current + segments + 1;
			indices.push_back(current);
			indices.push_back(next);
			indices.push_back(current + 1);
			indices.push_back(current + 1);
			indices.push_back(next);
			indices.push_back(next + 1);
		}
	}
	sph.init(core,vertices, indices);
	textures->load(core, materialFilename);
}

void draw(DxCore* core,Shaders* shader, textureManager textures, std::string filename,Matrix44 Worldpos, Matrix44 Transform)
{
	shader->updateConstantVS("staticMeshBuffer", "W", &Worldpos);
	shader->updateConstantVS("staticMeshBuffer", "VP", &Transform);
	shader->apply(core);
	shader->updateTexturePS(core, "tex", textures.find(filename));
	sph.draw(core);
}
};

class Snow {
public:
	std::vector<Mesh> snowset;                
	std::vector<Matrix44> activePositions;    
	std::string materialFilename;          
	textureManager* textures;                

	float snowSpeed = 120.0f;               

	STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv) {
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		v.tangent = Vec3(0, 0, 0);
		v.tu = tu;
		v.tv = tv;
		return v;
	}

	void initSingleSnowflake(Mesh& mesh, DxCore* core, int rings, int segments, float radius) {
		std::vector<STATIC_VERTEX> vertices;
		std::vector<unsigned int> indices;

		for (int lat = 0; lat <= rings; lat++) {
			float theta = lat * PI / rings;
			float sinTheta = sinf(theta);
			float cosTheta = cosf(theta);

			for (int lon = 0; lon <= segments; lon++) {
				float phi = lon * 2.0f * PI / segments;
				float sinPhi = sinf(phi);
				float cosPhi = cosf(phi);

				Vec3 position(radius * sinTheta * cosPhi, radius * cosTheta, radius * sinTheta * sinPhi);
				Vec3 normal = position.Normalize();
				float tu = 1.0f - (float)lon / segments;
				float tv = 1.0f - (float)lat / rings;

				vertices.push_back(addVertex(position, normal, tu, tv));

				int current = lat * (segments + 1) + lon;
				int next = current + segments + 1;

				if (lat < rings && lon < segments) {
					indices.push_back(current);
					indices.push_back(next);
					indices.push_back(current + 1);

					indices.push_back(current + 1);
					indices.push_back(next);
					indices.push_back(next + 1);
				}
			}
		}

		mesh.init(core, vertices, indices);
	}

	void init(DxCore* core, textureManager* textureManager, int rings, int segments, float radius, int instanceCount) {
		textures = textureManager;

		Mesh snowflakeMesh;
		initSingleSnowflake(snowflakeMesh, core, rings, segments, radius);
		snowset.push_back(snowflakeMesh);

		for (int i = 0; i < instanceCount; i++) {
			activePositions.push_back(generateRandomPosition(2000.0f, 500.0f, 3000.0f, 2000.0f, Vec3(0.5f, 0.5f, 0.5f)));
		}
	}

	Matrix44 generateRandomPosition(float rangeX, float minY, float maxY, float rangeZ, Vec3 scale) {
		Matrix44 randomPos;

		randomPos.a[0][3] = static_cast<float>((rand() % static_cast<int>(rangeX * 2)) - rangeX);
		randomPos.a[1][3] = static_cast<float>((rand() % static_cast<int>(maxY - minY)) + minY); 
		randomPos.a[2][3] = static_cast<float>((rand() % static_cast<int>(rangeZ * 2)) - rangeZ);

		randomPos.a[0][0] = scale.x;
		randomPos.a[1][1] = scale.y;
		randomPos.a[2][2] = scale.z;

		return randomPos;
	}

	void update(float deltaTime) 
	{
		for (int i = 0; i < activePositions.size(); i++)
		{
			activePositions[i].a[1][3] -= snowSpeed * deltaTime;

			if (activePositions[i].a[1][3] < -10.0f) {
				activePositions[i] = generateRandomPosition(2000.0f, 2000.0f, 3000.0f, 2000.0f, Vec3(0.5f, 0.5f, 0.5f));
			}
		}
	}

	void drawManyRand(DxCore* core, Shaders* shader, Matrix44 Transform) {
		for (const auto& Wpos : activePositions) {
			shader->updateConstantVS("staticMeshBuffer", "W", const_cast<void*>(reinterpret_cast<const void*>(&Wpos)));
			shader->updateConstantVS("staticMeshBuffer", "VP", &Transform);
			shader->apply(core);

			for (int j = 0; j < snowset.size(); j++) {
				snowset[j].draw(core);
			}
		}
	}
};

class staticMesh
{
public:
	std::vector<Mesh> geoset;
	std::vector<std::string> textureFilenames;
	std::vector<std::string> normalFilenames;

	void loadMesh(DxCore* core, std::string filename,  textureManager* textures)
	{
		GEMLoader::GEMModelLoader loader;
		std::vector<GEMLoader::GEMMesh> gemmeshes;
		loader.load(filename, gemmeshes);
		for (int i = 0; i < gemmeshes.size(); i++)
		{
			Mesh geo;
			std::vector<STATIC_VERTEX> vertices;

			for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++)
			{
				STATIC_VERTEX v;
				memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
				vertices.push_back(v);

				Vec3 pos(v.pos.x, v.pos.y, v.pos.z);
			}

			std::string diffuse = gemmeshes[i].material.find("diffuse").getValue();
			textureFilenames.push_back(diffuse);
			textures->load(core, diffuse);

			std::string normal = gemmeshes[i].material.find("normal").getValue();
			normalFilenames.push_back(normal);
			textures->load(core, normal);

			geo.init(core,vertices, gemmeshes[i].indices);
			geoset.push_back(geo);
		}
	}

	void draw(DxCore* core,Shaders*shader, textureManager textures , Matrix44 Worldpos, Matrix44 Transform)
	{
		shader->updateConstantVS("staticMeshBuffer", "W", &Worldpos);
		shader->updateConstantVS("staticMeshBuffer", "VP", &Transform);
		shader->apply(core);
		for(int i = 0; i < geoset.size();i++)
		{
			textures.bindTextureToPS(core, textureFilenames[i], 0);
			textures.bindTextureToPS(core, normalFilenames[i], 1);
		    geoset[i].draw(core);
		}
	}

	std::vector<Matrix44> generateRandomPositions(int instanceCount, float rangeX, float rangeY, float rangeZ,Vec3 scale)
	{
		std::vector<Matrix44> positions;

		for (int i = 0; i < instanceCount; i++)
		{
			Matrix44 randomPos;
			randomPos.a[0][3] = static_cast<float>((rand() % static_cast<int>(rangeX * 2)) - rangeX);
			randomPos.a[1][3] = static_cast<float>((rand() % static_cast<int>(rangeY * 2)) - rangeY);
			randomPos.a[2][3] = static_cast<float>((rand() % static_cast<int>(rangeZ * 2)) - rangeZ);
			randomPos.a[0][0] = scale.x;
			randomPos.a[1][1] = scale.y;
			randomPos.a[2][2] = scale.z;

			positions.push_back(randomPos);
		}

		return positions;
	}

	void drawManyRand(DxCore* core, Shaders* shader, textureManager& textures, Matrix44 Transform, const std::vector<Matrix44>& Wpositions)
	{
		for (int i = 0; i < Wpositions.size(); i++)
		{
			shader->updateConstantVS("staticMeshBuffer", "W", const_cast<void*>(reinterpret_cast<const void*>(&Wpositions[i])));
			shader->updateConstantVS("staticMeshBuffer", "VP", &Transform);
			shader->apply(core);

			for (int j = 0; j < geoset.size(); j++)
			{
				textures.bindTextureToPS(core, textureFilenames[j], 0);
				textures.bindTextureToPS(core, normalFilenames[j], 1);
				geoset[j].draw(core);
			}
		}
	}
};

class DrawBamboo : public staticMesh
{
public:
	float time = 0.0f;
	float maxHeight = 180.0f;      
	float intensity = 0.004f;   
	float frequency = 1.3f;     

	void update(float deltaTime)
	{
		time += deltaTime;
	}

	void drawb(DxCore* core, Shaders* shader, textureManager textures, Matrix44 Worldpos, Matrix44 Transform)
	{
		shader->updateConstantVS("WaveParams", "time", &time);
		shader->updateConstantVS("WaveParams", "maxHeight", &maxHeight);
		shader->updateConstantVS("WaveParams", "intensity", &intensity);
		shader->updateConstantVS("WaveParams", "frequency", &frequency);
		shader->updateConstantVS("staticMeshBuffer", "W", &Worldpos);
		shader->updateConstantVS("staticMeshBuffer", "VP", &Transform);
		shader->apply(core);

		for (int i = 0; i < geoset.size(); i++)
		{
			textures.bindTextureToPS(core, textureFilenames[i], 0);
			textures.bindTextureToPS(core, normalFilenames[i], 1);
			geoset[i].draw(core);
		}
	}

	void drawManyRandb(DxCore* core, Shaders* shader, textureManager& textures, Matrix44 Transform, const std::vector<Matrix44>& Wpositions)
	{
		shader->updateConstantVS("WaveParams", "time", &time);
		shader->updateConstantVS("WaveParams", "maxHeight", &maxHeight);
		shader->updateConstantVS("WaveParams", "intensity", &intensity);
		shader->updateConstantVS("WaveParams", "frequency", &frequency);

		for (int i = 0; i < Wpositions.size(); i++)
		{
			shader->updateConstantVS("staticMeshBuffer", "W", const_cast<void*>(reinterpret_cast<const void*>(&Wpositions[i])));
			shader->updateConstantVS("staticMeshBuffer", "VP", &Transform);
			shader->apply(core);

			for (int j = 0; j < geoset.size(); j++)
			{
				textures.bindTextureToPS(core, textureFilenames[j], 0);
				textures.bindTextureToPS(core, normalFilenames[j], 1);
				geoset[j].draw(core);
			}
		}
	}
};

class AnimationSequence
{
	// stores animation data for one animation
public:
	std::vector<AnimationFrame> frames; // transformation data for bones
	float ticksPerSecond; // speed of animation

	Vec3 interpolate(Vec3 p1, Vec3 p2, float t) {
		return ((p1 * (1.0f - t)) + (p2 * t)); // linear interpolation
	}
	Quaternion interpolate(Quaternion q1, Quaternion q2, float t) {
		return Quaternion::slerp(q1, q2, t); // ensure smooth rotation
	}
	float duration() {
		return ((float)frames.size() / ticksPerSecond);// total duration of animation sequence
	}

	void calcFrame(float t, int& frame, float& interpolationFact)
	{
		// find frame given time
		interpolationFact = t * ticksPerSecond;
		frame = (int)floorf(interpolationFact);
		interpolationFact = interpolationFact - (float)frame; // fractional part
		frame = min(frame, frames.size() - 1);
	}

	int nextFrame(int frame)
	{
		// find next frame, returns index for next frame
		return min(frame + 1, frames.size() - 1);
	}

	Matrix44 interpolateBoneToGlobal(Matrix44* matrices, int baseFrame, float interpolationFact, Skeleton* skeleton, int boneIndex) {
		//Interpolates transformations for a specific bone and calculates its global transformation.
		int nextFrameIndex = nextFrame(baseFrame);
		Matrix44 temp;
		// interpolate scale, rotation and translation
		Matrix44 scale = temp.scaling(interpolate(frames[baseFrame].scales[boneIndex], frames[nextFrameIndex].scales[boneIndex], interpolationFact));
		Matrix44 rotation = interpolate(frames[baseFrame].rotations[boneIndex], frames[nextFrameIndex].rotations[boneIndex], interpolationFact).toMatrix();
		Matrix44 translation = Matrix44::translation(interpolate(frames[baseFrame].positions[boneIndex], frames[nextFrameIndex].positions[boneIndex], interpolationFact));
		Matrix44 local = scale * rotation * translation;
		if (skeleton->bones[boneIndex].parentIndex > -1) {
			Matrix44 global = local * matrices[skeleton->bones[boneIndex].parentIndex];
			return global;
		}
		return local;
	}


};

class Animation
{
	// store skeleton and animations
public:
	std::map<std::string, AnimationSequence> animations; // map of animation sequences
	Skeleton skeleton; // The skeleton structure defining bone hierarchy and transformations.

	int bonesSize() {
		int bonesSize = skeleton.bones.size();
		return bonesSize;
	}

	// Calculation of the current frame and interpolation factor
	void calcFrame(std::string name, float t, int& frame, float& interpolationFact) {
		animations[name].calcFrame(t, frame, interpolationFact);
	}
	// bone transformation interpolation to the corresponding AnimationSequence.
	Matrix44 interpolateBoneToGlobal(std::string name, Matrix44* matrices, int baseFrame, float interpolationFact, int boneIndex) {
		return animations[name].interpolateBoneToGlobal(matrices, baseFrame, interpolationFact, &skeleton, boneIndex);
	}

	void calcFinalTransforms(Matrix44* matrices)
	{
		for (int i = 0; i < 44; i++)
		{
			matrices[i] = skeleton.bones[i].offset * matrices[i] * skeleton.globalInverse;
		}
	}
};

class AnimationInstance
{
public:
	Animation* animation;
	std::string currentAnimation;
	Matrix44 matrices[256];
	float t;

	void resetAnimationTime()
	{
		t = 0;
	}
	bool animationFinished()
	{
		if (t > animation->animations[currentAnimation].duration())
		{
			return true;
		}
		return false;
	}

	void update(std::string name, float dt) {
		if (name == currentAnimation) {
			t += dt;
		}
		else {
			currentAnimation = name;  t = 0;
		}
		if (animationFinished() == true) { resetAnimationTime(); }
		int frame = 0;
		float interpolationFact = 0;
		animation->calcFrame(name, t, frame, interpolationFact);
		for (int i = 0; i < animation->bonesSize(); i++)
		{
			matrices[i] = animation->interpolateBoneToGlobal(name, matrices, frame, interpolationFact, i);
		}
		animation->calcFinalTransforms(matrices);

	}

};

class animatedMesh
{
public:
	std::vector<Mesh> geoset;
	AnimationInstance instance;
	Animation animation1;
	std::vector<std::string> textureFilenames;
	std::vector<std::string> normalFilenames;
	float t = 0.f;

	void loadMesh(DxCore* core, std::string filename, textureManager* textures)
	{
		GEMLoader::GEMModelLoader loader;
		std::vector<GEMLoader::GEMMesh> gemmeshes;
		GEMLoader::GEMAnimation gemanimation;
		loader.load(filename, gemmeshes, gemanimation);
		for (int i = 0; i < gemmeshes.size(); i++) {
			Mesh geo;
			std::vector<ANIMATED_VERTEX> vertices;
			for (int j = 0; j < gemmeshes[i].verticesAnimated.size(); j++)
			{
				ANIMATED_VERTEX v;
				memcpy(&v, &gemmeshes[i].verticesAnimated[j], sizeof(ANIMATED_VERTEX));
				vertices.push_back(v);
			}
			geo.initAnimated(core, vertices, gemmeshes[i].indices);

			std::string diffuse = gemmeshes[i].material.find("diffuse").getValue();
			textureFilenames.push_back(diffuse);
			textures->load(core, diffuse);

			std::string normal = gemmeshes[i].material.find("normal").getValue();
			normalFilenames.push_back(normal);
			textures->load(core, normal);
	
			geoset.push_back(geo);
		}
		for (int i = 0; i < gemanimation.bones.size(); i++)
		{
			Bone bone;
			bone.name = gemanimation.bones[i].name;
			memcpy(&bone.offset, &gemanimation.bones[i].offset, 16 * sizeof(float));
			bone.parentIndex = gemanimation.bones[i].parentIndex;
			animation1.skeleton.bones.push_back(bone);
		}
		for (int i = 0; i < gemanimation.animations.size(); i++)
		{
			std::string name = gemanimation.animations[i].name;
			AnimationSequence aseq;
			aseq.ticksPerSecond = gemanimation.animations[i].ticksPerSecond;
			for (int n = 0; n < gemanimation.animations[i].frames.size(); n++)
			{
				AnimationFrame frame;
				for (int index = 0; index < gemanimation.animations[i].frames[n].positions.size(); index++)
				{
					Vec3 p;	Vec3 s;
					Quaternion q;
					memcpy(&p, &gemanimation.animations[i].frames[n].positions[index], sizeof(Vec3));
					frame.positions.push_back(p);
					memcpy(&q, &gemanimation.animations[i].frames[n].rotations[index], sizeof(Quaternion));
					frame.rotations.push_back(q);
					memcpy(&s, &gemanimation.animations[i].frames[n].scales[index], sizeof(Vec3));
					frame.scales.push_back(s);
				}
				aseq.frames.push_back(frame);
			}
			animation1.animations.insert({ name, aseq });
		}
		instance.animation = &animation1;
	}

	void draw(Shaders* shaders, DxCore* core, float dt, textureManager textures, std::string animename, Matrix44 Worldpos, Matrix44 Transform)
	{
		instance.update(animename, dt);
		shaders->updateConstantVS( "animatedMeshBuffer", "W", &Worldpos);
		shaders->updateConstantVS( "animatedMeshBuffer", "VP", &Transform);
		shaders->updateConstantVS( "animatedMeshBuffer", "bones", instance.matrices);
		shaders->apply(core);
		for (int i = 0; i < geoset.size(); i++)
		{
			shaders->updateTexturePS(core, "tex", textures.find(textureFilenames[i]));
			shaders->updateTexturePS(core, "normalMap", textures.find(normalFilenames[i]));
			geoset[i].draw(core);
		}
	}
};

class drawDinosaur : public animatedMesh
{
public:
	Vec3 position;      
	Vec3 target;        
	Vec3 forward;
	float speed = 300.0f; 
	float moveWidth;   
	float moveHeight;  
	float waitTimer = 0.0f; 
	bool isWaiting = false; 

	drawDinosaur(float sceneWidth, float sceneHeight): moveWidth(sceneWidth), moveHeight(sceneHeight)
	{
		position = Vec3(0.0f, 0.0f, 0.0f); 
		forward = Vec3(0.0f, 0.0f, 1.0f);
		generateRandomTarget();           
	}

	void generateRandomTarget()
	{
		float x = static_cast<float>(rand()) / RAND_MAX * moveWidth - moveWidth / 2.0f;
		float z = static_cast<float>(rand()) / RAND_MAX * moveHeight - moveHeight / 2.0f;
		target = Vec3(x, position.y, z); 
	}

	void updatePosition(float dt)
	{
		if (isWaiting) 
		{
			waitTimer += dt;
			if (waitTimer >= 3.0f) 
			{ 
				isWaiting = false;
				waitTimer = 0.0f;
				generateRandomTarget();
			}
			return;
		}

		Vec3 direction = target - position;
		float distance = direction.Length();
		Vec3 directionNor = direction / distance;

		if (distance > speed * dt)
		{
			position += directionNor * speed * dt;
			forward = directionNor; 
		}
		else
		{
			position = target;
			isWaiting = true;
		}
	}

	void draw(Shaders* shaders, DxCore* core, float dt, textureManager& textures, Matrix44 Transform)
	{
		updatePosition(dt); 

		std::string currentAnimation = (target - position).Length() > 0.1f ? "walk" : "idle2";
		instance.update(currentAnimation, dt);

		Vec3 up = Vec3(0.0f, 1.0f, 0.0f); 
		Vec3 right = up.Cross(forward).Normalize(); 
		Vec3 adjustedUp = forward.Cross(right).Normalize(); 
    	Matrix44 rotationMatrix
		(
			right.x, right.y, right.z, 0.0f,
			adjustedUp.x, adjustedUp.y, adjustedUp.z, 0.0f,
			forward.x, forward.y, forward.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		Matrix44 Worldpos = Matrix44::translation(position);
		Matrix44 scaleMatrix = Matrix44::scaling(Vec3(100, 100, 100)); 
		Matrix44 biggerDefault = scaleMatrix * rotationMatrix *Worldpos;

		shaders->updateConstantVS("animatedMeshBuffer", "W", &biggerDefault);
		shaders->updateConstantVS("animatedMeshBuffer", "VP", &Transform);
		shaders->updateConstantVS("animatedMeshBuffer", "bones", instance.matrices);
		shaders->apply(core);

		for (int i = 0; i < geoset.size(); i++)
		{
			textures.bindTextureToPS(core, textureFilenames[i], 0);
			geoset[i].draw(core);
		}
	}
};

class WaterPlane
{
public:
	Mesh mesh;                      
	float time = 0.0f;         
	float waveAmplitude = 12.0f;     
	float waveFrequency = 2.0f;     
	float waveSpeed = 1.0f;  
	int gridSize;           
	float transparency = 0.6f;

	float scrollSpeedX = 0.008f; 
	float scrollSpeedY = 0.005f;
	std::string materialFilename;

	STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv)
	{
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		v.tangent = Vec3(0, 0, 0);
		v.tu = tu;
		v.tv = tv;
		return v;
	}

	void init(DxCore* core, textureManager* textures, const std::string& materialPath, int gridSize)
	{
		materialFilename = materialPath;
		this->gridSize = gridSize;
		std::vector<STATIC_VERTEX> vertices;
		std::vector<unsigned int> indices;

		float step = 2000.0f / gridSize; 
		for (int z = 0; z <= gridSize; ++z)
		{
			for (int x = 0; x <= gridSize; ++x)
			{
				float posX = -1000.0f + x * step;
				float posZ = -1000.0f + z * step;
				vertices.push_back(addVertex(
					Vec3(posX, 0, posZ),
					Vec3(0, 1, 0),
					x / (float)gridSize,
					z / (float)gridSize));
			}
		}

		for (int z = 0; z < gridSize; ++z)
		{
			for (int x = 0; x < gridSize; ++x)
			{
				int topLeft = z * (gridSize + 1) + x;
				int topRight = topLeft + 1;
				int bottomLeft = topLeft + (gridSize + 1);
				int bottomRight = bottomLeft + 1;

				indices.push_back(topLeft);
				indices.push_back(bottomLeft);
				indices.push_back(topRight);

				indices.push_back(topRight);
				indices.push_back(bottomLeft);
				indices.push_back(bottomRight);
			}
		}
		textures->load(core, materialFilename);
		mesh.init(core, vertices, indices);
	}

	void update(float dt)
	{
		time += dt;
	}

	void draw(DxCore* core, Shaders* shader, textureManager textures, std::string filename,float tile,Matrix44 Worldpos, Matrix44 Transform)
	{
		shader->updateConstantVS("WaterParams", "time", &time);
		shader->updateConstantVS("WaterParams", "waveAmplitude", &waveAmplitude);
		shader->updateConstantVS("WaterParams", "waveFrequency", &waveFrequency);
		shader->updateConstantVS("WaterParams", "waveSpeed", &waveSpeed);

		shader->updateConstantVS("staticMeshBuffer", "W", &Worldpos);
		shader->updateConstantVS("staticMeshBuffer", "VP", &Transform);

		shader->updateConstantPS("ScrollParams", "time", &time);
		shader->updateConstantPS("ScrollParams", "scrollSpeedX", &scrollSpeedX);
		shader->updateConstantPS("ScrollParams", "scrollSpeedY", &scrollSpeedY);
		shader->updateConstantPS("ScrollParams", "tileTime", &tile);
		shader->updateConstantPS("ScrollParams", "transparency", &transparency);


		textures.bindTextureToPS(core, filename, 0);
		shader->apply(core);

		mesh.draw(core);
	}
};