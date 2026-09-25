#pragma once
#include "base.h"
#include "frameBuffer.h"
#include "raster.h"
#include "../application/application.h"
#include "Mathlib.h"
#include "image.h"
#include "dataStructure.h"
#include "VAO.h"
#include "BufferObject.h"
#include "shader.h"
#include "clipper.h"
#include "texture.h"

#define sgl GPU::getInstance()

/*
* class GPU：
* simulate GPU function
*/
class GPU {
public:
	static GPU* getInstance();
	GPU();

	~GPU();

	//接受外界传入的bmp对应的内存指针以及窗体的宽/高
	void initSurface(const uint32_t& width, const uint32_t& height, void* buffer = nullptr);

	void clear();

	// Original GPU Simulation (can be discarded)

	void drawPoint(int x, int y, const RGBA& color);

	void drawPoint(pixel& p);

	void drawLine(int x1, int y1, int x2, int y2, const RGBA& color1, const RGBA& color2);

	void drawLine(pixel& p1, pixel& p2);

	void drawTriangle(pixel& p1, pixel& p2, pixel& p3);

	void drawImage(const image& img, int x, int y);

	void drawImageAlpha(const image& img, int x, int y, int alpha);

	void setTexture(image* img);

	image* getTexture() {
		return mImage;
	}

	RGBA nearestSampling(const vec2f uv) {
		auto UV = uv;

		int x = std::round(UV.x * (mImage->mWidth - 1));
		int y = std::round(UV.y * (mImage->mHeight - 1));

		int position = y * mImage->mWidth + x;

		return mImage->mData[position];
	}

	RGBA BilinearInterpolation(const vec2f& UV);

	void setTextureWrap(int n);

	vec2f checkWrap(const vec2f& UV);

	// -----------------Original GPU Simulation End----------------------

	// ------------Rendering Pipeline----------

	// keep initSurface and clear functions

	uint32_t genBuffer(); // generate buffer object and return buffer ID
	bool deleteBuffer(const uint32_t bufferID);
	void bindBuffer(const uint32_t& bufferType, const uint32_t& bufferID);
	void bufferData(const uint32_t& bufferType, size_t dataSize, void* data);

	uint32_t genVertexArray(); // generate VAO and return the ID
	bool deleteVertexArray(const uint32_t vaoID);
	void bindVertexArray(const uint32_t& vaoID);
	void vertexAttributePointer(
		const uint32_t binding,
		const uint32_t itemSize,
		const uint32_t stride,
		const uint32_t offset
	);

	void useProgram(Shader* shader);



	void drawElement(const uint32_t& drawMode, const uint32_t& first, const uint32_t& count);
	
	void enable(const uint32_t& value);

	void disable(const uint32_t& value);

	// cull face
	void frontFace(const uint32_t& value);
	void cullFace(const uint32_t& value);

	// Depth Test
	bool depthTest(const FsOutput& output);
	void depthFunc(const uint32_t depthFunc);

	// texture
	uint32_t genTexture();
	void deleteTexture(const uint32_t& texID);
	void bindTexture(const uint32_t& texID);
	void texImage2D(const uint32_t& width, const uint32_t& height, void* data);
	void texParameter(const uint32_t& param, const uint32_t& value);

	 
private:
	void vertexShaderStage(
		std::vector<VsOutput>& vsOutputs,
		const VertexArrayObject* vao,
		const BufferObject* ebo,
		const uint32_t first,
		const uint32_t count
	);

// Drawing Functions

	void perspectiveDivision(VsOutput& vsOutput); // for dividing omega (depth)
	void screenMapping(VsOutput& vsOutput); // NDC coordinate to Screen coordinate
	void perspectiveRecover(VsOutput& vsOutput);

	void trim(VsOutput& vsOutput);



	RGBA blend(const FsOutput& output);

private:
	FrameBuffer* mFrameBuffer{ nullptr };

	// VBO related and EBO
	uint32_t mCurrentVBO{ 0 };
	uint32_t mCurrentEBO{ 0 };
	uint32_t mBufferCounter{ 0 }; // number of VBO
	std::map<uint32_t, std::unique_ptr<BufferObject>> mBufferMap;

	// VAO related
	uint32_t mCurrentVAO{ 0 };
	uint32_t mVaoCounter{ 0 };
	std::map<uint32_t, std::unique_ptr<VertexArrayObject>> mVAOMap;

	Shader* mShader{ nullptr };
	mat4f mScreenMatrix; // get assigned when we do initSurface function

	// Cull face
	bool mEnableCullFace{ true };
	uint32_t mFrontFace{ FRONT_FACE_CCW };
	uint32_t mCullFace{ BACK_FACE };

	// Depth Test
	bool mEnableDepthTest{ true };
	uint32_t mDepthFunc{ DEPTH_LESS };


	// Blending
	bool mEnableBlending{ false };

	// Texturfe
	uint32_t mCurrentTexture{ 0 };
	uint32_t mTextureCounter{ 0 };
	std::map<uint32_t, Texture*> mTextureMap;
 

	// Original GPU Simulation (can be discarded)

	// FrameBuffer* mFrameBuffer{ nullptr };

	bool EnableBlendMode{ true };

	image* mImage{ nullptr };

	RGBA lerp(const RGBA& c1, const RGBA& c2, float weight);

	int mWrap{ TEXTURE_WRAP_REPEAT };
};



