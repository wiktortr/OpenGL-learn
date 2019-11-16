#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <GL/glew.h>
#include <string>
#include <exception>
#include <vector>
#include <fstream>

enum class DataType : uint32_t { Float, Double, Int, UInt };
enum class DrawMode : uint32_t { Points, Lines, Triangles };
enum class BufferType : uint32_t { StaticDraw, DynamicDraw, StreamDraw };

//-------------------------------------------------------------------------------
	
class VertexBufferAttribute {
public:
	//std::string m_name;
	DataType m_dataType; 
	uint32_t m_size;
public:
	VertexBufferAttribute(const std::string& name, DataType dataType, size_t size);
	VertexBufferAttribute(VertexBufferAttribute&& m);
	VertexBufferAttribute(const VertexBufferAttribute& m) = default;
	VertexBufferAttribute() = default;
};

//-------------------------------------------------------------------------------

class VertexBufferManagement {
public:
	using BytesPtr = char*; 
	using Attributes = std::vector<VertexBufferAttribute>;

private:
	unsigned int m_vao = 0; 
	unsigned int m_vbo = 0;
	unsigned int m_ebo = 0;

	BytesPtr m_vertexDataPointer = nullptr;
	uint32_t m_vertexDataSize = 0; 

	BytesPtr m_elementDataPointer = nullptr; 
	uint32_t m_elementDataSize = 0;
	DataType m_elementDataType;

	Attributes m_attributes;
	DrawMode m_drawMode; 
	BufferType m_bufferType; 

private:
	inline std::size_t getAttributesByteSize();
	void checkErrors();

public:
	VertexBufferManagement(const VertexBufferManagement&) = delete;
	const VertexBufferManagement& operator=(const VertexBufferManagement&) = delete;
	VertexBufferManagement() = delete;

public:
	VertexBufferManagement(DrawMode drawMode = DrawMode::Triangles, const Attributes& attributes = {});
	~VertexBufferManagement();

	void toggleDrawMode(DrawMode drawMode);
	Attributes& getAttributesRef();  

	void setVertexDataPointer(const BytesPtr data, std::size_t size);
	void setElementDataPointer(const BytesPtr data, std::size_t size, DataType dtype); 

	void saveVertexDartaToBuffer();
	void saveElementDataToBuffer(); 
	void saveAllToBuffer();

	void setBufferAttributes(); 

	void draw() const;

	//friend functions
	friend void saveModelToFile(const std::string& filename, VertexBufferManagement& model);
	friend VertexBufferManagement* loadModelFromFile(const std::string& filename, char* vertexDataPointer, char* elementDataPointer);
};


#endif 
