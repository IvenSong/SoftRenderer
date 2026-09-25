#include "texture.h"

Texture::Texture() = default;

Texture::~Texture() {
	delete[] mBuffer;
}

void Texture::setBufferData(const uint32_t& width, const uint32_t& height, void* data)
{
	if (mBuffer != nullptr) {
		delete[] mBuffer;
		mBuffer = nullptr;
	}

	mWidth = width;
	mHeight = height;

	if (mBuffer == nullptr) {
		mBuffer = new RGBA[mWidth * mHeight];
	}

	memcpy(mBuffer, data, mWidth * mHeight * sizeof(RGBA));
}

vec4f Texture::getColor(float u, float v)
{
	RGBA resultColor;

	checkWrap(u, mWrapU);
	checkWrap(v, mWrapV);

	if (mFilter == TEXTURE_FILTER_NEAREST) {
		int x = std::round(u * (mWidth - 1));
		int y = std::round(v * (mHeight - 1));

		int position = y * mWidth + x;
		resultColor = mBuffer[position];
	}
	else if (mFilter == TEXTURE_FILTER_LINEAR) {
		float x = u * static_cast<float>(mWidth - 1);
		float y = v * static_cast<float>(mHeight - 1);

		int left = std::floor(x);
		int right = std::ceil(x);
		int bottom = std::floor(y);
		int top = std::ceil(y);

		float yScale = 0.0f;
		if (top == bottom) {
			yScale = 1.0f;
		}
		else {
			yScale = (y - static_cast<float>(bottom)) / static_cast<float>(top - bottom);
		}

		int positionLeftTop = top * mWidth + left;
		int positionLeftBottom = bottom * mWidth + left;
		int positionRightTop = top * mWidth + right;
		int positionRightBottom = bottom * mWidth + right;

		RGBA leftColor = Math::lerp(mBuffer[positionLeftBottom], mBuffer[positionLeftTop], yScale);
		RGBA rightColor = Math::lerp(mBuffer[positionRightBottom], mBuffer[positionRightTop], yScale);

		float xScale = 0.0f;
		if (right == left) {
			xScale = 1.0f;
		}
		else {
			xScale = (x - static_cast<float>(left)) / static_cast<float>(right - left);
		}

		resultColor = Math::lerp(leftColor, rightColor, xScale);

	}

	vec4f result;
	result.x = static_cast<float>(resultColor.mR) / 255.0f;
	result.y = static_cast<float>(resultColor.mG) / 255.0f;
	result.z = static_cast<float>(resultColor.mB) / 255.0f;
	result.w = static_cast<float>(resultColor.mA) / 255.0f;
	
	return result;
}

void Texture::setParameter(const uint32_t& type, const uint32_t& value) {
	switch (type) {
	case TEXTURE_FILTER:
		mFilter = value;
		break;
	case TEXTURE_WRAP_U:
		mWrapU = value;
		break;
	case TEXTURE_WRAP_V:
		mWrapV = value;
		break;
	default:
		break;
	}

}

void Texture::checkWrap(float& n, const uint32_t& type)
{
	if (n > 1.0f || n < 0.0f) {
		n = FRACTION(n);

		switch (type) {
		case TEXTURE_WRAP_REPEAT:
			n = FRACTION(n + 1);
			break;
		case TEXTURE_WRAP_MIRROR:
			n = 1.0f - FRACTION(n + 1);
			break;

		default:
			break;
		}
	}
}
