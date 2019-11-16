#ifndef VAO_H_
#define VAO_H_

#include <GL/glew.h>
#include <vector>
#include <exception>
#include <sstream>

class VAO {

public:

	using Vertex = float; 
	using VertexArray = std::vector<Vertex>;
	using IndiciesArray = std::vector<unsigned int>;

	enum class VAOType {
		SERIES,
		INDICIES
	};

private:
	VertexArray m_vertexes;
	IndiciesArray m_indicies; 
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_EBO; 

	VAOType m_actualType; 
	unsigned m_vertexSize; 

private:
	void createObj(); 

public:
	VAO(VAOType type, const VertexArray& vertexes, unsigned vertexSize, const IndiciesArray& indicies = IndiciesArray());
	VAO(VAOType type, VertexArray&& vertexes, unsigned vertexSize, IndiciesArray&& indicies = IndiciesArray());
	VAO();
	~VAO(); 

	void use() const;
	void saveVertexesToMemory(int GPUDataManage) const;
	void setVertexMatrixPointer(unsigned attrib, unsigned bufferPosition, unsigned size, bool dataNormalize = false) const;
	void unbind() const; 
	void toggleVAOType(VAOType type); 

public: 
	void setVertexes(const VertexArray& vertexes);
	void setVertexes(VertexArray&& vertexes);
	const VertexArray& getVertexesCopy() const; 

	void setIndicies(const IndiciesArray& indicies);
	void setIndicies(IndiciesArray&& indicies);
	const IndiciesArray& getIndiciesCopy() const;

	unsigned& getIndicator(unsigned index);
	unsigned getIndicator(unsigned index) const;

	Vertex& operator[](unsigned index);
	const Vertex& operator[](unsigned index) const;

	void draw(int primitive, int n=0); 

public:

	class MemoryViolation : public std::exception {
	private:
		std::string m_message;

	public:
		MemoryViolation(int minr, int maxr);
		// const char* what() const throw {
		// 	return m_message.c_str(); 
		// }
	};

	class NoInfo : public std::exception {
	private:
		std::string m_message; 

	public:
		NoInfo(std::string message);
		// const char* what() const throw {
		// 	return m_message.c_str(); 
		// }
	};
};

#endif
