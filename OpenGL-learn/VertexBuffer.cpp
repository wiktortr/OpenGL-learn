#include "VertexBuffer.h"
#include "ErrorHandler.h"
#include <iostream>

// independent blocks

std::size_t getDataTypeSize(DataType dtype) {
	switch (dtype) {
		case DataType::Float:
			return sizeof(float);

		case DataType::Double:
			return sizeof(double); 

		case DataType::Int:
			return sizeof(int);

		case DataType::UInt:
			return sizeof(unsigned int);

		default:
			return 0; 
	}
}

// -------------------------------------------------------------------------------------------------------

GLuint getGLType(DataType dtype) {
	switch (dtype) {
		case DataType::Float:
			return GL_FLOAT;

		case DataType::Double:
			return GL_DOUBLE;

		case DataType::Int:
			return GL_INT;

		case DataType::UInt:
			return GL_UNSIGNED_INT;

		default:
			return 0;
	}
}

// -------------------------------------------------------------------------------------------------------

GLuint getDrawMode(DrawMode mode) {
	switch (mode) {
		case DrawMode::Points:
			return GL_POINTS;

		case DrawMode::Lines:
			return GL_LINES;

		case DrawMode::Triangles:
			return GL_TRIANGLES;

		default:
			return 0; 
	}
}

// -------------------------------------------------------------------------------------------------------

GLuint getBufferType(BufferType btype) {
	switch (btype) {
		case BufferType::StaticDraw:
			return GL_STATIC_DRAW;

		case BufferType::DynamicDraw:
			return GL_DYNAMIC_DRAW;

		case BufferType::StreamDraw:
			return GL_STREAM_DRAW;

		default:
			return 0; 
	}
}

// VertexBufferAttribute class

VertexBufferAttribute::VertexBufferAttribute(const std::string& name, DataType dataType, size_t size) 
	:  m_dataType(dataType), m_size(size) 
{}

// -------------------------------------------------------------------------------------------------------

VertexBufferAttribute::VertexBufferAttribute(VertexBufferAttribute&& m)
	: m_dataType(std::move(m.m_dataType)), m_size(std::move(m.m_size))
{}

// VertexBufferManagement class

inline std::size_t VertexBufferManagement::getAttributesByteSize() {
	std::size_t strideSize = 0;
	for (auto& attr : m_attributes)
		strideSize += attr.m_size * sizeof(float);
	return strideSize; 
}

// -------------------------------------------------------------------------------------------------------

void VertexBufferManagement::checkErrors() {
	if (m_vertexDataPointer == nullptr || m_vertexDataSize == 0)
		errorHandler::throwRuntimeError("Empty vertex data");

	if (m_elementDataPointer == nullptr || m_elementDataSize == 0)
		errorHandler::throwRuntimeError("Empty element data");

	if (m_attributes.size() == 0)
		errorHandler::throwRuntimeError("No attributes");

	if (m_vertexDataSize % getAttributesByteSize() != 0)
		errorHandler::throwRuntimeError("Attributes do not fit to vertex data");

	if (m_elementDataSize % getDataTypeSize(m_elementDataType) != 0)
		errorHandler::throwRuntimeError("Element data error");	
}

// -------------------------------------------------------------------------------------------------------

VertexBufferManagement::VertexBufferManagement(DrawMode drawMode, const Attributes& attributes)
	: m_drawMode(drawMode), m_attributes(attributes) 
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);
}

// -------------------------------------------------------------------------------------------------------

VertexBufferManagement::~VertexBufferManagement() {
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
}

// -------------------------------------------------------------------------------------------------------

void VertexBufferManagement::toggleDrawMode(DrawMode drawMode) {
	m_drawMode = drawMode; 
}

// -------------------------------------------------------------------------------------------------------

VertexBufferManagement::Attributes& VertexBufferManagement::getAttributesRef() {
	return m_attributes; 
}

// -------------------------------------------------------------------------------------------------------

void VertexBufferManagement::setVertexDataPointer(const BytesPtr data, std::size_t size) {
	m_vertexDataPointer = data; 
	m_vertexDataSize = size; 
}

// -------------------------------------------------------------------------------------------------------

void VertexBufferManagement::setElementDataPointer(const BytesPtr data, std::size_t size, DataType dtype) {
	m_elementDataPointer = data;
	m_elementDataSize = size;
	m_elementDataType = dtype;
}

// -------------------------------------------------------------------------------------------------------

void VertexBufferManagement::saveVertexDartaToBuffer() {
	checkErrors();
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertexDataSize, m_vertexDataPointer, getBufferType(m_bufferType));
}

// -------------------------------------------------------------------------------------------------------

void VertexBufferManagement::saveElementDataToBuffer() {
	checkErrors();
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_elementDataSize, m_elementDataPointer, getBufferType(m_bufferType));
}

// -------------------------------------------------------------------------------------------------------

void VertexBufferManagement::saveAllToBuffer() {
	checkErrors(); 
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertexDataSize, m_vertexDataPointer, getBufferType(m_bufferType));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_elementDataSize, m_elementDataPointer, getBufferType(m_bufferType));
}

// -------------------------------------------------------------------------------------------------------

void VertexBufferManagement::setBufferAttributes() {
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	std::size_t strideSize = getAttributesByteSize();

	if (m_vertexDataSize % strideSize != 0)
		errorHandler::throwRuntimeError("Attributes do not fit to vertex data");
	
	std::size_t pos = 0;
	std::size_t len = m_attributes.size(); 
	for (std::size_t i = 0; i < len; ++i) {
		auto& attr = m_attributes[i];
		glVertexAttribPointer(i, attr.m_size, getGLType(attr.m_dataType), GL_FALSE, strideSize, (void*)pos);
		glEnableVertexAttribArray(i);
		pos += attr.m_size * getDataTypeSize(attr.m_dataType);
	}
}

// -------------------------------------------------------------------------------------------------------

void VertexBufferManagement::draw() const {
	glBindVertexArray(m_vao);
	glDrawElements(getDrawMode(m_drawMode), m_elementDataSize / getDataTypeSize(m_elementDataType),
		getGLType(m_elementDataType), nullptr);
	glBindVertexArray(0);
}

// friend functions

void saveModelToFile(const std::string& filename, VertexBufferManagement& model) {
	std::ofstream file(filename, std::ios_base::out | std::ios_base::binary);
	if (!file.is_open())
		errorHandler::throwRuntimeError("saveModelToFile error");

	DrawMode drawMode = model.m_drawMode;
	std::size_t attributesSize = model.m_attributes.size(); 
	std::size_t vertexDataSize = model.m_vertexDataSize;
	std::size_t elementDataSize = model.m_elementDataSize;
	DataType elementDataType = model.m_elementDataType;

	file.write(reinterpret_cast<char*>(&drawMode), sizeof(drawMode));

	file.write((char*)&attributesSize, sizeof(attributesSize));
	for (auto& el : model.m_attributes) 
		file.write(reinterpret_cast<char*>(&el), sizeof(el));

	file.write(reinterpret_cast<char*>(&vertexDataSize), sizeof(vertexDataSize));
	file.write(model.m_vertexDataPointer, vertexDataSize);

	file.write(reinterpret_cast<char*>(&elementDataSize), sizeof(elementDataSize));
	file.write(reinterpret_cast<char*>(&elementDataType), sizeof(elementDataType));
	file.write(model.m_elementDataPointer, elementDataSize);

	file.close(); 
}

// -------------------------------------------------------------------------------------------------------

VertexBufferManagement* loadModelFromFile(const std::string& filename, char* vertexDataPointer, char* elementDataPointer) {
	std::ifstream file(filename, std::ios_base::in | std::ios_base::binary);
	if (!file.is_open())
		errorHandler::throwRuntimeError("saveModelToFile error");

	DrawMode drawMode;
	std::size_t attributesSize;
	std::size_t vertexDataSize;
	std::size_t elementDataSize;
	DataType elementDataType;

	file.read(reinterpret_cast<char*>(&drawMode), sizeof(drawMode));
	VertexBufferManagement::Attributes attributesVec;

	file.read(reinterpret_cast<char*>(&attributesSize), sizeof(attributesSize));

	VertexBufferAttribute tAttr;
	for (std::size_t i = 0; i < attributesSize; ++i) {
		file.read(reinterpret_cast<char*>(&tAttr), sizeof(tAttr));
		attributesVec.push_back(tAttr); 
	}
	
	file.read(reinterpret_cast<char*>(&vertexDataSize), sizeof(vertexDataSize));
	vertexDataPointer = new char[vertexDataSize];
	file.read(vertexDataPointer, vertexDataSize);

	file.read(reinterpret_cast<char*>(&elementDataSize), sizeof(elementDataSize));
	file.read(reinterpret_cast<char*>(&elementDataType), sizeof(elementDataType));
	elementDataPointer = new char[elementDataSize];
	file.read(elementDataPointer, elementDataSize);

	file.close();

	VertexBufferManagement* model = new VertexBufferManagement(drawMode, attributesVec);
	model->setVertexDataPointer(vertexDataPointer, vertexDataSize);
	model->setElementDataPointer(elementDataPointer, elementDataSize, elementDataType);
	model->saveAllToBuffer(); 
	model->setBufferAttributes(); 

	return model; 
}



