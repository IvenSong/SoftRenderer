#pragma once
#include "base.h"
#include  "dataStructure.h"
#include "BufferObject.h"
#include "MathLib.h"
#include "texture.h"


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

	virtual void fragmentShader(const VsOutput& input, FsOutput& output, const std::map<uint32_t, Texture*>& textures) = 0;

public:
	Math::vector4<float> getVector(
		const std::map<uint32_t, BindingDescription>& bindingMap,
		const std::map<uint32_t, std::unique_ptr<BufferObject>>& bufferMap,
		const uint32_t& attributeLocation,
		const uint32_t& index);

	// Choose vec2f, vec3f or vec4f explicitly; smaller vectors keep the first components.
	template <typename Vector>
	Vector getVector(
		const std::map<uint32_t, BindingDescription>& bindingMap,
		const std::map<uint32_t, std::unique_ptr<BufferObject>>& bufferMap,
		const uint32_t& attributeLocation,
		const uint32_t& index);

	RGBA vectorToRGBA(const Math::vector4<float> v) {
		vec4f c = v;
		c.x = std::clamp(c.x, 0.0f, 1.0f);
		c.y = std::clamp(c.y, 0.0f, 1.0f);
		c.z = std::clamp(c.z, 0.0f, 1.0f);
		c.w = std::clamp(c.w, 0.0f, 1.0f);

		RGBA color;
		color.mR = static_cast<byte>(c.x * 255);
		color.mG = static_cast<byte>(c.y * 255);
		color.mB = static_cast<byte>(c.z * 255);
		color.mA = static_cast<byte>(c.w * 255);
		return color;
	}
};

template <>
vec2f Shader::getVector<vec2f>(
	const std::map<uint32_t, BindingDescription>& bindingMap,
	const std::map<uint32_t, std::unique_ptr<BufferObject>>& bufferMap,
	const uint32_t& attributeLocation,
	const uint32_t& index);

template <>
vec3f Shader::getVector<vec3f>(
	const std::map<uint32_t, BindingDescription>& bindingMap,
	const std::map<uint32_t, std::unique_ptr<BufferObject>>& bufferMap,
	const uint32_t& attributeLocation,
	const uint32_t& index);

template <>
vec4f Shader::getVector<vec4f>(
	const std::map<uint32_t, BindingDescription>& bindingMap,
	const std::map<uint32_t, std::unique_ptr<BufferObject>>& bufferMap,
	const uint32_t& attributeLocation,
	const uint32_t& index);
