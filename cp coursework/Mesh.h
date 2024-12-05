#pragma once
#include "mathLibrary.h"
#include "dxCore.h"
#include <d3d11.h>
#include "GEMloader.h"
#include "Texture.h"
#include "Shader.h"


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
		vertices.push_back(addVertex(Vec3(-1000, 0, -1000), Vec3(0, 1, 0), 0, 0));
		vertices.push_back(addVertex(Vec3(1000, 0, -1000), Vec3(0, 1, 0), 1, 0));
		vertices.push_back(addVertex(Vec3(-1000, 0, 1000), Vec3(0, 1, 0), 0, 1));
		vertices.push_back(addVertex(Vec3(1000, 0, 1000), Vec3(0, 1, 0), 1, 1));
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

	void init(DxCore* core, textureManager* textures, const std::string& materialPath)
	{
		materialFilename = materialPath;
		std::vector<STATIC_VERTEX> vertices;
		vertices.push_back(addVertex(Vec3(-5000, 0, -5000), Vec3(0, 1, 0), 0, 0));
		vertices.push_back(addVertex(Vec3(5000, 0, -5000), Vec3(0, 1, 0), 1, 0));
		vertices.push_back(addVertex(Vec3(-5000, 0, 5000), Vec3(0, 1, 0), 0, 1));
		vertices.push_back(addVertex(Vec3(5000, 0, 5000), Vec3(0, 1, 0), 1, 1));
		std::vector<unsigned int> indices;
		indices.push_back(2); indices.push_back(1); indices.push_back(0);
		indices.push_back(1); indices.push_back(2); indices.push_back(3);
		mesh.init(core, vertices, indices);
		textures->load(core, materialFilename);
	}

	void draw(DxCore* core, Shaders* shader, textureManager textures, std:: string filename,/*float tilingX, float tilingY, */Matrix44 Worldpos, Matrix44 Transform)
	{
		shader->updateConstantVS("staticMeshBuffer", "W", &Worldpos);
		shader->updateConstantVS("staticMeshBuffer", "VP", &Transform);
		//textures.bindTexture(core, shader, "Textures/grass.png", tilingX, tilingY);
		shader->apply(core);
		shader->updateTexturePS(core, "tex", textures.find(filename));
		//ID3D11ShaderResourceView* normalMap = textures.find("Textures/grass_normal.png");
	
		mesh.draw(core);
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

class staticMesh
{
public:
	std::vector<Mesh> geoset;
	std::vector<std::string> textureFilenames;

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
			}
			textureFilenames.push_back(gemmeshes[i].material.find("diffuse").getValue());
			textures->load(core, gemmeshes[i].material.find("diffuse").getValue());
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
			shader->updateTexturePS(core, "tex", textures.find(textureFilenames[i]));
		    geoset[i].draw(core);
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
			textureFilenames.push_back(gemmeshes[i].material.find("diffuse").getValue());
			textures->load(core, gemmeshes[i].material.find("diffuse").getValue());
			// Load texture with filename: gemmeshes[i].material.find("diffuse").getValue()
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

	void draw(Shaders* shaders, DxCore* core, float dt, textureManager textures, Matrix44 Worldpos, Matrix44 Transform)
	{
		instance.update("Run", dt);
		shaders->updateConstantVS( "animatedMeshBuffer", "W", &Worldpos);
		shaders->updateConstantVS( "animatedMeshBuffer", "VP", &Transform);
		shaders->updateConstantVS( "animatedMeshBuffer", "bones", instance.matrices);
		shaders->apply(core);
		for (int i = 0; i < geoset.size(); i++)
		{
			shaders->updateTexturePS(core, "tex", textures.find(textureFilenames[i]));
			geoset[i].draw(core);
		}
	}
};