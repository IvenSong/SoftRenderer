#pragma once
#include "base.h"
#include  "dataStructure.h"
#include "BufferObject.h"
#include "MathLib.h"


class Shader {
public:
	Shader() {}
	~Shader() {}

	virtual VsOutput vertexShader(
		// bindingMap in VAO
		const std::map<uint32_t, BindingDescription>& bindingMap,
		// bufferMap in VBO
		const std::map<uint32_t, std::unique_ptr<BufferObject>>& bufferMap,
		// index that should be processed
		const uint32_t& index
	) = 0;

	virtual void fragmentShader(const VsOutput& input, FsOutput& output) = 0;

public:
	Math::vector4<float> getVector(
		const std::map<uint32_t, BindingDescription>& bindingMap,
		const std::map<uint32_t, std::unique_ptr<BufferObject>>& bufferMap,
		const uint32_t& attributeLocation,
		const uint32_t& index);

	RGBA vectorToRGBA(const Math::vector4<float> v) {
		RGBA color;
		color.mR = v.x * 255;
		color.mG = v.y * 255;
		color.mB = v.z * 255;
		color.mA = v.w * 255;
		return color;
	}
};