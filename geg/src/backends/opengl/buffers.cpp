#include "buffers.hpp"

namespace Geg {

	///--------------------------------------------- Vertex Buffer --------------------------------------///

	GLVertexBuffer::GLVertexBuffer(float* vertices, unsigned int size) {
		glCreateBuffers(1, &r_Id);
		glBindBuffer(GL_ARRAY_BUFFER, r_Id);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	GLVertexBuffer::~GLVertexBuffer() {
		glDeleteBuffers(1, &r_Id);
	}

	void GLVertexBuffer::setData(void *data, unsigned int size) {
		bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void GLVertexBuffer::bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, r_Id);
	}

	void GLVertexBuffer::unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	BufferLayout GLVertexBuffer::getLayout() const {
		return layout;
	}

	void GLVertexBuffer::setLayout(BufferLayout _layout) {
		layout = _layout;
	}

	///--------------------------------------------- Index Buffer --------------------------------------///

	GLIndexBuffer::GLIndexBuffer(unsigned int *indices, unsigned int _count) : count(_count){
		glCreateBuffers(1, &r_Id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r_Id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	GLIndexBuffer::~GLIndexBuffer() {
		glDeleteBuffers(1, &r_Id);
	}

	void GLIndexBuffer::setData(unsigned int *data, unsigned int size) {
		bind();
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
	}

	void GLIndexBuffer::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r_Id);
	}

	unsigned int GLIndexBuffer::getCount() const {
		return count;
	}

	void GLIndexBuffer::unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
