#include "BufferObject.h"

BufferObject::BufferObject() {}

BufferObject::~BufferObject() {}

void BufferObject::setBufferData(size_t dataSize, void* data) {
	mBufferSize = dataSize;
	mBuffer.resize(dataSize);
    if (dataSize > 0 && data != nullptr) {
        std::memcpy(mBuffer.data(), data, dataSize);
    }
}


const std::byte* BufferObject::getBuffer() const {
	return mBuffer.data();
}
