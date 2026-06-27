#pragma once
#include "base.h"
#include "math.h"

// structure of VAO desctiption
struct BindingDestcription {
	int mVBOid{ 0 };
	size_t mItemSize{ 0 };
	size_t mStride{ 0 };
	size_t mOffset{ 0 };
};

struct VsOutput {
	vec4f mPosition{ 0.0f,0.0f,0.0f,1.0f };
	vec4f mColor; // from 0.0 to 1.0
	vec2f mUV;
};

// after all process of rasterization(by VsOutput), data go into FsOutput
struct FsOutput {
	vec2i mPixelPos;
	float mDepth;
	RGBA mColor; // from 0-255
};

