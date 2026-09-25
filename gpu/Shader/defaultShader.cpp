#include "defaultShader.h"

VsOutput defaultShader::vertexShader(const std::map<uint32_t,
	BindingDescription>& bindingMap, 
	const std::map<uint32_t,std::unique_ptr<BufferObject>>& bufferMap, 
	const uint32_t& index)
{
	VsOutput output;

	vec4f position = getVector(bindingMap, bufferMap, 0, index);

	position.w = 1.0f;
	vec4f color = getVector(bindingMap, bufferMap, 1, index);

	vec2f uv = static_cast<vec2f>(getVector(bindingMap, bufferMap, 2, index));

	output.mPosition = mProjectionMatrix * mViewMatrix * mModelMatrix * position;
	output.mColor = color;
	output.mUV = uv;

	return output;
}

void defaultShader::fragmentShader(const VsOutput& input, FsOutput& output, const std::map<uint32_t, Texture*>& textures)
{
	output.mPixelPos.x = static_cast<int>(input.mPosition.x);
	output.mPixelPos.y = static_cast<int>(input.mPosition.y);
	output.mDepth = input.mPosition.z;
	output.mColor = vectorToRGBA(input.mColor);
}
