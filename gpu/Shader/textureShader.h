#pragma once
#include "Shader.h"

class TextureShader : public Shader {
public:
	TextureShader();
	~TextureShader();

	VsOutput vertexShader(
		// bindingMap in VAO
		const std::map<uint32_t, BindingDescription>& bindingMap,
		// bufferMap in VBO
		const std::map<uint32_t, std::unique_ptr<BufferObject>>& bufferMap,
		// index that should be processed
		const uint32_t& index
	) override;

	void fragmentShader(const VsOutput& input, FsOutput& output, const std::map<uint32_t, Texture*>& textures);

public:
	// uniforms
	mat4f mModelMatrix;
	mat4f mViewMatrix;
	mat4f mProjectionMatrix;

	uint32_t mDiffuseTexture{ 0 };
};
