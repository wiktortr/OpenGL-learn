#include "VAO.h"

/// VAO

void VAO::createObj() {
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);
}

VAO::VAO(VAOType type, const VertexArray& vertexes, unsigned vertexSize, const IndiciesArray& indicies)
	: m_vertexes(vertexes), m_indicies(indicies), m_actualType(type), m_vertexSize(vertexSize)
{
	createObj(); 
}

VAO::VAO(VAOType type, VertexArray&& vertexes, unsigned vertexSize, IndiciesArray&& indicies)
	: m_vertexes(std::move(vertexes)), m_indicies(std::move(indicies)), m_actualType(type), m_vertexSize(vertexSize)
{
	createObj();
}

VAO::VAO() {
	createObj();
}

VAO::~VAO() {
	glDeleteVertexArrays(1, &m_VAO); 
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

///---------------------------------------------------------------

void VAO::use() const {
	glBindVertexArray(m_VAO);
}

///---------------------------------------------------------------

void VAO::saveVertexesToMemory(int GPUDataManage) const {
	use(); 

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	if (!m_vertexes.size())
		throw NoInfo("vertexes not exist");
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertexes.size(), &m_vertexes[0], GPUDataManage);

	if (m_actualType == VAOType::INDICIES) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		if (!m_indicies.size())
			throw NoInfo("indicies not exist");
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * m_indicies.size(), &m_indicies[0], GPUDataManage);
	}
}

///---------------------------------------------------------------

void VAO::setVertexMatrixPointer(unsigned attrib, unsigned bufferPosition, unsigned size, bool dataNormalize) const {
	use(); 
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(attrib, size, GL_FLOAT, dataNormalize, m_vertexSize * sizeof(GLfloat), (GLvoid*)(bufferPosition * sizeof(Vertex)));
	glEnableVertexAttribArray(attrib);
}

///---------------------------------------------------------------

void VAO::unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

///---------------------------------------------------------------

void VAO::toggleVAOType(VAOType type) {
	m_actualType = type; 
}

///---------------------------------------------------------------

void VAO::setVertexes(const VertexArray& vertexes) {
	m_vertexes = vertexes;
}

void VAO::setVertexes(VertexArray&& vertexes) {
	m_vertexes = std::move(vertexes);
}

const VAO::VertexArray& VAO::getVertexesCopy() const {
	return const_cast<const VertexArray&>(m_vertexes);
}

///---------------------------------------------------------------

void VAO::setIndicies(const IndiciesArray& indicies) {
	m_indicies = indicies;
}

void VAO::setIndicies(IndiciesArray&& indicies) {
	m_indicies = std::move(indicies);
}

const VAO::IndiciesArray& VAO::getIndiciesCopy() const {
	return const_cast<const IndiciesArray&>(m_indicies);
}

///---------------------------------------------------------------

unsigned & VAO::getIndicator(unsigned index) {
	if (index < 0 || index >= m_indicies.size())
		throw MemoryViolation(0, m_indicies.size());
	return m_indicies[index];
}

unsigned VAO::getIndicator(unsigned index) const {
	if (index < 0 || index >= m_indicies.size())
		throw MemoryViolation(0, m_indicies.size());
	return m_indicies[index];
}

///---------------------------------------------------------------

VAO::Vertex& VAO::operator[](unsigned index) {
	if (index < 0 || index >= m_vertexes.size())
		throw MemoryViolation(0, m_vertexes.size());
	return m_vertexes[index];
}

const VAO::Vertex& VAO::operator[](unsigned index) const {
	if (index < 0 || index >= m_vertexes.size())
		throw MemoryViolation(0, m_vertexes.size());
	return m_vertexes[index];
}

///---------------------------------------------------------------

void VAO::draw(int primitive, int n) {
	use();
	if (m_actualType == VAOType::INDICIES) 
		glDrawElements(primitive, (n>=1 ? n : m_indicies.size()), GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(primitive, 0, (n>=1 ? n : m_vertexes.size() / m_vertexSize));
}

/// MemoryVolation exception

VAO::MemoryViolation::MemoryViolation(int minr, int maxr) {
	std::ostringstream os;
	os << "VAO::MemoryViolation(" << minr << ", " << maxr << ")";
	m_message = os.str();
}

/// NoInfo exception

VAO::NoInfo::NoInfo(std::string message) {
	std::ostringstream os;
	os << "VAO::NoInfo(" << message << ")";
	m_message = os.str();
}
