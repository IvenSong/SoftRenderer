#include "lightShader.h"

namespace {
vec3f normalizeOrZero(const vec3f& value) {
	const float lengthSquared = Math::lengthSQ(value);
	if (!std::isfinite(lengthSquared) || lengthSquared <= 0.0f) {
		return vec3f(0.0f, 0.0f, 0.0f);
	}
	return value / std::sqrt(lengthSquared);
}
}

LightShader::LightShader()  {
}

LightShader::~LightShader() {}

VsOutput LightShader::vertexShader(const std::map<uint32_t, BindingDescription>& bindingMap, const std::map<uint32_t, std::unique_ptr<BufferObject>>& bufferMap, const uint32_t& index)
{
	VsOutput output;

	vec4f position = getVector(bindingMap, bufferMap, 0, index);

	position.w = 1.0f;

	vec3f normal = getVector<vec3f>(bindingMap, bufferMap, 1, index);

	vec2f uv = getVector<vec2f>(bindingMap, bufferMap, 2, index);

	output.mPosition = mProjectionMatrix * mViewMatrix * mModelMatrix * position;
	// Lighting directions are in world space. Inverse transpose also handles nonuniform scale.
	const mat4f normalMatrix = Math::transpose(Math::inverse(mModelMatrix));
	const vec4f worldNormal = normalMatrix * vec4f(normal.x, normal.y, normal.z, 0.0f);
	output.mNormal = normalizeOrZero(static_cast<vec3f>(worldNormal));
	output.mUV = uv;

	return output;
}

void LightShader::fragmentShader(const VsOutput& input, FsOutput& output, const std::map<uint32_t, Texture*>& textures) {
	output.mPixelPos.x = static_cast<int>(input.mPosition.x);
	output.mPixelPos.y = static_cast<int>(input.mPosition.y);
	output.mDepth = input.mPosition.z;

	const auto normal = normalizeOrZero(input.mNormal);
	const auto lightDirection = normalizeOrZero(mDirectionalLight.direction);

	auto iter = textures.find(mDiffuseTexture);
	auto texture = iter->second;

	vec4f texColor = texture->getColor(input.mUV.x, input.mUV.y);

	const float diff = std::clamp(Math::dot(normal, -lightDirection), 0.0f, 1.0f);
	const vec3f illumination = mEnvLight + mDirectionalLight.color * diff;
	// Illumination affects RGB; opacity comes from the texture.
	output.mColor = vectorToRGBA(vec4f(
		texColor.x * illumination.x,
		texColor.y * illumination.y,
		texColor.z * illumination.z,
		texColor.w));
}
