#include "shader.h"

Math::vector4<float> Shader::getVector(
	const std::map<uint32_t, BindingDescription>& bindingMap,
	const std::map<uint32_t, std::unique_ptr<BufferObject>>& bufferMap, 
	const uint32_t& attributeLocation,
	const uint32_t& index)
{
	auto bindingIter = bindingMap.find(attributeLocation);
	if (bindingIter == bindingMap.end()) {
		assert(false);
	}

	auto bindingDescription = bindingIter->second;

	auto vboID = bindingDescription.mVBOid;
	auto vboIter = bufferMap.find(vboID);
	if (vboIter == bufferMap.end()) {
		assert(false);
	}
	BufferObject* vbo = vboIter->second.get();

	uint32_t dataOffset = bindingDescription.mStride * index + bindingDescription.mOffset;
	uint32_t dataSize = bindingDescription.mItemSize * sizeof(float);

	const std::byte* buffer = vbo->getBuffer() + dataOffset;

	vec4f result;

	memcpy(&result, buffer, dataSize);

	return result;
}

template <>
vec2f Shader::getVector<vec2f>(
	const std::map<uint32_t, BindingDescription>& bindingMap,
	const std::map<uint32_t, std::unique_ptr<BufferObject>>& bufferMap,
	const uint32_t& attributeLocation,
	const uint32_t& index)
{
	return static_cast<vec2f>(getVector(bindingMap, bufferMap, attributeLocation, index));
}

template <>
vec3f Shader::getVector<vec3f>(
	const std::map<uint32_t, BindingDescription>& bindingMap,
	const std::map<uint32_t, std::unique_ptr<BufferObject>>& bufferMap,
	const uint32_t& attributeLocation,
	const uint32_t& index)
{
	return static_cast<vec3f>(getVector(bindingMap, bufferMap, attributeLocation, index));
}

template <>
vec4f Shader::getVector<vec4f>(
	const std::map<uint32_t, BindingDescription>& bindingMap,
	const std::map<uint32_t, std::unique_ptr<BufferObject>>& bufferMap,
	const uint32_t& attributeLocation,
	const uint32_t& index)
{
	return getVector(bindingMap, bufferMap, attributeLocation, index);
}
