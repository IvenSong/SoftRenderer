#pragma once
#include "base.h"
#include "MathLib.h"
#include "shader.h"

struct DirectionalLight {
	vec3f color;
	vec3f direction;
};


class LightShader : public Shader {
public:
	LightShader();
	~LightShader();


	VsOutput vertexShader(
		// bindingMap in VAO
		const std::map<uint32_t, BindingDescription>& bindingMap,
		// bufferMap in VBO
		const std::map<uint32_t, std::unique_ptr<BufferObject>>& bufferMap,
		// index that should be processed
		const uint32_t& index
	) override;

	void fragmentShader(const VsOutput& input, FsOutput& output,
		const std::map<uint32_t, Texture*>& textures) override;


public:
	//uniform matrices
	mat4f mModelMatrix;
	mat4f mViewMatrix;
	mat4f mProjectionMatrix;

	uint32_t mDiffuseTexture{ 0 };
	DirectionalLight mDirectionalLight;
	vec3f mEnvLight;


};