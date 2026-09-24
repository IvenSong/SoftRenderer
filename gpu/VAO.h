#pragma once
#include "base.h"
#include "dataStructure.h"

class VertexArrayObject {
public:
	

	void set(uint32_t binding, uint32_t vboId, size_t itemSize ,size_t stride, size_t offset);

	std::map<uint32_t, BindingDescription> getBindingMap() const;

	const void print();

private:
	// key:bindingId - value:bindingDescription
	std::map<uint32_t, BindingDescription> mBindingMap;
};