#pragma once
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postProcess.h"
#include "base.h"
#include "MathLib.h"
#include "image.h"

class LightShader;

struct Vertex {
	vec3f position;
	vec3f normal;
	vec2f texCoords;
};




class Mesh {
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t> indices, uint32_t diffuseTexture, const mat4f localMatrix);
	~Mesh();

	void addChild(Mesh* mesh);

	void draw(const mat4f& preMatrix, LightShader* shader);

private:
	friend class Model;
	uint32_t mVao{ 0 };
	uint32_t mVbo{ 0 };
	uint32_t mEbo{ 0 };
	uint32_t mTexture{ 0 };
	uint32_t mIndicesCount{ 0 };

	mat4f mLocalMatrix;

	std::vector<Mesh*> mChildren;

};



class Model {
public:
	Model();
	~Model();

	void read(const std::string& path);

	void draw(LightShader* shader);

	void setModelMatrix(const mat4f& m);

private:
	void processNode(Mesh* parent, aiNode* ainode, const aiScene* scene);

	Mesh* processMesh(aiMesh* aimesh, const aiScene* scene);

	// material: which material you want to process
	// type what kind of type you want to know
	uint32_t processTexture(const aiMaterial* material, const aiTextureType& type, const aiScene* scene);

	uint32_t createTexture(image* image);

	static mat4f getMat4f(const aiMatrix4x4& value);

private:
	std::string mRootPath{};
	Mesh* mRootMesh{ nullptr };
	std::vector<Mesh*> mMeshes;

	// image path and texture ID
	std::map<std::string, uint32_t> mTextureCache;
 };
