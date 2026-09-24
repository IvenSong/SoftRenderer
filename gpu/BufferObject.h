#pragma once
#include "base.h"

class BufferObject {
public:
	BufferObject();
	~BufferObject();
	BufferObject(const BufferObject&) = delete; // ½ûÖ¹¿½±´¹¹Ôìº¯Êý

	void setBufferData(size_t dataSize, void* data);
	const std::byte* getBuffer() const;

private:
	size_t mBufferSize{ 0 }; // # of bytes
	std::vector<std::byte> mBuffer; // 


};