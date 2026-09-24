#pragma once
#include "shader.h"


class defaultShader:public Shader {
public:
	defaultShader() {};
	~defaultShader() {};

	VsOutput vertexShader(
		// bindingMap in VAO
		const std::map<uint32_t, BindingDescription>& bindingMap,
		// bufferMap in VBO
		const std::map<uint32_t, std::unique_ptr<BufferObject>>& bufferMap,
		// index that should be processed
		const uint32_t& index
	) override;

	void fragmentShader(const VsOutput& input, FsOutput& output);
public:
	//uniform matrices
	mat4f mModelMatrix;
	mat4f mViewMatrix;
	mat4f mProjectionMatrix;

};