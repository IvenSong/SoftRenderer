#include "gpu.h"

GPU* GPU::mInstance = nullptr;
GPU* GPU::getInstance() {
	if (!mInstance) {
		mInstance = new GPU();
	}

	return mInstance;
}

GPU::GPU() {}

GPU::~GPU() {
	if (mFrameBuffer) {
		delete mFrameBuffer;
	}
}

void GPU::initSurface(const uint32_t& width, const uint32_t& height, void* buffer) {
	if (mFrameBuffer) {
		delete mFrameBuffer;
	}

	mFrameBuffer = new FrameBuffer(width, height, buffer);
	mScreenMatrix = Math::screenMatrix<float>(width - 1, height - 1);
}

void GPU::clear() {
	if (!mFrameBuffer) {
		return;
	}

	mFrameBuffer->clear();
}

void GPU::drawPoint(int x, int y, const RGBA& color) {
	if (!mFrameBuffer) {
		return;
	}

	mFrameBuffer->setPoint(x, y, color);
}

void GPU::drawPoint(pixel& p) {
	drawPoint(p.x, p.y, p.color);
}

void GPU::drawLine(int x1, int y1, int x2, int y2, const RGBA& color1, const RGBA& color2) {
	pixel p1(x1, y1, color1);
	pixel p2(x2, y2, color2);
	
	raster::RasterizeLine(p1, p2);
}

void GPU::drawLine(pixel& p1, pixel& p2) {

	raster::RasterizeLine(p1, p2);
}

void GPU::drawTriangle(pixel& p1, pixel& p2, pixel& p3) {
	raster::RasterizeTriangle(p1, p2, p3);
}


void GPU::drawImage(const image& img, int x, int y) {
	if (x < 0 || y < 0) {
		return;
	}
	for (int i = 0; i < img.mWidth; ++i) {
		for (int j = 0; j < img.mHeight; ++j) {
			drawPoint(x + i, y + j, img.mData[j * img.mWidth + i]);
		}
	}
}



void GPU::drawImageAlpha(const image& img, int x, int y, int alpha) {
	if (x < 0 || y < 0) {
		return;
	}
	for (int i = 0; i < img.mWidth; ++i) {
		for (int j = 0; j < img.mHeight; ++j) {
			RGBA color = img.mData[j * img.mWidth + i];
			color.mA = static_cast<byte>(std::clamp(alpha, 0, 255));
			drawPoint(x + i, y + j, color);
		}
	}
}

void GPU::setTexture(image* img) {
	mImage = img;
}

RGBA GPU::BilinearInterpolation(const vec2f& UV) {
	int width = mImage->mWidth;
	int height = mImage->mHeight;

	vec2f uv = checkWrap(UV);

	float u = std::clamp(uv.x, 0.0f, 1.0f);
	float v = std::clamp(uv.y, 0.0f, 1.0f); // vertically inversed

	float x = u * (width - 1);
	float y = v * (height - 1);

	int x0 = static_cast<int>(std::floor(x));
	int y0 = static_cast<int>(std::floor(y));

	int x1 = min(x0 + 1, width - 1);
	int y1 = min(y0 + 1, height - 1);

	float tx = x - x0;
	float ty = y - y0;

	const RGBA& c00 = mImage->mData[y0 * width + x0];
	const RGBA& c10 = mImage->mData[y0 * width + x1];
	const RGBA& c01 = mImage->mData[y1 * width + x0];
	const RGBA& c11 = mImage->mData[y1 * width + x1];

	return lerp(lerp(c00, c10, tx), lerp(c01, c11, tx), ty);
}

RGBA GPU::lerp(const RGBA& c1, const RGBA& c2, float weight) {
	return c1 * (1.0f - weight) + c2 * weight;
}

void GPU::setTextureWrap(int x) {
	if (x == 0) {
		mWrap = TEXTURE_WRAP_REPEAT;
	}
	else if (x == 1) {
		mWrap = TEXTURE_WRAP_MIRROR;
	}
}

vec2f GPU::checkWrap(const vec2f& UV) {
	vec2f uv = UV;
	if (uv.x < 0 || uv.x > 1) {
		switch (mWrap)
		{
		case TEXTURE_WRAP_REPEAT:
			uv.x = FRACTION(FRACTION(uv.x) + 1);
		case TEXTURE_WRAP_MIRROR:
			uv.x = 1 - FRACTION(FRACTION(uv.x) + 1);
		}
	}
	if (uv.y < 0 || uv.y > 1) {
		switch (mWrap)
		{
		case TEXTURE_WRAP_REPEAT:
			uv.y = FRACTION(FRACTION(uv.y) + 1);
		case TEXTURE_WRAP_MIRROR:
			uv.y = 1 - FRACTION(FRACTION(uv.y) + 1);
		}
	}
	return uv;
}



uint32_t GPU::genBuffer() {
	const uint32_t bufferID = ++mBufferCounter;

	mBufferMap.emplace(bufferID, std::make_unique<BufferObject>());

	return bufferID;
}

bool GPU::deleteBuffer(const uint32_t bufferID) {
	return mBufferMap.erase(bufferID) != 0;
}

uint32_t GPU::genVertexArray() {
	const uint32_t vaoID = ++mVaoCounter;
	mVAOMap.emplace(vaoID, std::make_unique<VertexArrayObject>());
	return vaoID;
}

bool GPU::deleteVertexArray(const uint32_t vaoID) {
	return mVAOMap.erase(vaoID) != 0;
}


void GPU::bindBuffer(const uint32_t& bufferType, const uint32_t& bufferID) {
	if (bufferType == ARRAY_BUFFER) {
		mCurrentVBO = bufferID;
	}
	else if (bufferType == ELEMENT_ARRAY_BUFFER) {
		mCurrentEBO = bufferID;
	}
}

void GPU::bufferData(const uint32_t& bufferType, size_t dataSize, void* data) {
	uint32_t bufferID;
	if (bufferType == ARRAY_BUFFER) {
		bufferID = mCurrentVBO;
	}
	else if (bufferType == ELEMENT_ARRAY_BUFFER) {
		bufferID = mCurrentEBO;
	}
	else {
		assert(false);
	}

	auto iter = mBufferMap.find(bufferID);
	if (iter == mBufferMap.end()) {
		assert(false);
	}

	std::unique_ptr<BufferObject>& bufferObject = iter->second;
	bufferObject->setBufferData(dataSize, data);
}

void GPU::bindVertexArray(const uint32_t& vaoID) {
	mCurrentVAO = vaoID;
}
void GPU::vertexAttributePointer(
	const uint32_t binding,
	const uint32_t itemSize,
	const uint32_t stride,
	const uint32_t offset
) {
	auto iter = mVAOMap.find(mCurrentVAO);
	if (iter == mVAOMap.end()) {
		assert(false);
	}

	auto& vao = iter->second;
	vao->set(binding, mCurrentVBO, itemSize, stride, offset);
}

void GPU::useProgram(Shader* shader)
{
	mShader = shader;
}

/*
drawMode: primitive drawing mode, such as triangles or lines
first:    index of the first vertex/index to process
count:    number of vertices/indices to process
*/
void GPU::drawElement(const uint32_t& drawMode, const uint32_t& first, const uint32_t& count)
{
	if (mCurrentVAO == 0 || !mShader || count == 0) {
		return;
	}

	// Get VAO
	auto vaoIter = mVAOMap.find(mCurrentVAO);
	if (vaoIter == mVAOMap.end()) {
		std::cerr << "Error: current VAO is invalid" << std::endl;
		return;
	}

	const VertexArrayObject* vao = vaoIter->second.get();
	auto bindingMap = vao->getBindingMap();

	// Get EBO
	auto eboIter = mBufferMap.find(mCurrentEBO);
	if (eboIter == mBufferMap.end()) {
		std::cerr << "Error: current VAO is invalid" << std::endl;
		return;
	}

	const BufferObject* ebo = eboIter->second.get();

	/* 
	* VertexShader process
	* Process vertices according to input EBO and put them into VsOutput
	*/
	std::vector<VsOutput> vsOutputs{};
	vertexShaderStage(vsOutputs, vao, ebo, first, count);

	if (vsOutputs.empty()) return;

	// Clip in homogeneous coordinates before perspective division.
	std::vector<VsOutput> clipOutputs;
	Clipper::doClipSpace(drawMode, vsOutputs, clipOutputs,
		mEnableCullFace, mFrontFace, mCullFace);
	if (clipOutputs.empty()) return;

	/* NDC Process: convert vertex into NDC*/
	for (auto& output : clipOutputs) {
		perspectiveDivision(output);
	}

	// Back Face Cull Process
	std::vector<VsOutput> cullOutputs = clipOutputs;
	if (drawMode == DRAW_TRIANGLES && mEnableCullFace) {
		cullOutputs.clear();
		for (int i = 0; i < clipOutputs.size() - 2; i += 3) {
			if (Clipper::cullFace(mFrontFace, mCullFace, clipOutputs[i], clipOutputs[i + 1], clipOutputs[i + 2])) {
				auto start = clipOutputs.begin() + i;
				auto end = clipOutputs.begin() + i + 3;
				cullOutputs.insert(cullOutputs.end(), start, end);

			}
		}
	}

	/*Screen Mapping*/

	for (auto& output : clipOutputs) {
		screenMapping(output);
	}

	/* Rasterization */
	std::vector<VsOutput> rasterOutputs;
	raster::rasterize(rasterOutputs, drawMode, clipOutputs);

	if (rasterOutputs.empty()) return;

	// Perspective Recover Process

	for (auto& output : rasterOutputs) {
		perspectiveRecover(output);
	}


	/* Color Output Process*/

	FsOutput fsOutput;
	uint32_t pixelPos = 0;
	for (uint32_t i = 0; i < rasterOutputs.size(); ++i) {
		mShader->fragmentShader(rasterOutputs[i], fsOutput);
		pixelPos = fsOutput.mPixelPos.y * mFrameBuffer->getWidth() + fsOutput.mPixelPos.x;
		mFrameBuffer->mColorBuffer[pixelPos] = fsOutput.mColor;
	}
}

void GPU::vertexShaderStage(std::vector<VsOutput>& vsOutputs, const VertexArrayObject* vao, const BufferObject* ebo, const uint32_t first, const uint32_t count)
{
	auto bindingMap = vao->getBindingMap();
	const std::byte* indicesData = ebo->getBuffer();

	uint32_t index = 0; 
	for (uint32_t i = first; i < first + count; ++i) {
		size_t indicesOffset = i * sizeof(uint32_t);
		memcpy(&index, indicesData + indicesOffset, sizeof(uint32_t));

		VsOutput output = mShader->vertexShader(bindingMap, mBufferMap, index);
		vsOutputs.push_back(output);
	}
}

void GPU::perspectiveDivision(VsOutput& vsOutput)
{
	float oneOverW = 1.0f / vsOutput.mPosition.w;

	vsOutput.mOneOverW = oneOverW;
	vsOutput.mPosition *= oneOverW;
	vsOutput.mPosition.w = 1.0f;

	vsOutput.mColor *= vsOutput.mOneOverW;
	vsOutput.mUV *= vsOutput.mOneOverW;

	trim(vsOutput);
}

void GPU::screenMapping(VsOutput& vsOutput)
{
	vsOutput.mPosition = mScreenMatrix * vsOutput.mPosition;
}

void GPU::perspectiveRecover(VsOutput& vsOutput)
{
	vsOutput.mColor /= vsOutput.mOneOverW;
	vsOutput.mUV /= vsOutput.mOneOverW;
}

void GPU::trim(VsOutput& vsOutput) {
	if (vsOutput.mPosition.x < -1.0f) {
		vsOutput.mPosition.x = -1.0f;
	}
	else if (vsOutput.mPosition.x > 1.0f) {
		vsOutput.mPosition.x = 1.0f;
	}
	if (vsOutput.mPosition.y < -1.0f) {
		vsOutput.mPosition.y = -1.0f;
	}
	else if (vsOutput.mPosition.y > 1.0f) {
		vsOutput.mPosition.y = 1.0f;
	}
	if (vsOutput.mPosition.y < -1.0f) {
		vsOutput.mPosition.y = -1.0f;
	}
	else if (vsOutput.mPosition.z > 1.0f) {
		vsOutput.mPosition.z = 1.0f;
	}

}
