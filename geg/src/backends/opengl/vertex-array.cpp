#include "vertex-array.hpp"
#include "glad/glad.h"

namespace Geg {

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
  switch (type) {
  case ShaderDataType::Float:
  case ShaderDataType::Float2:
  case ShaderDataType::Float3:
  case ShaderDataType::Float4:
  case ShaderDataType::Mat3:
  case ShaderDataType::Mat4: {
    return GL_FLOAT;
  }
  case ShaderDataType::Int:
  case ShaderDataType::Int2:
  case ShaderDataType::Int3:
  case ShaderDataType::Int4: {
    return GL_INT;
  }
  case ShaderDataType::Bool: {
    return GL_BOOL;
  }
  default: {
    GEG_CORE_ERROR("Can't convert unknown ");
    return 0;
  }
  }
}

GLVertexArray::GLVertexArray() { glCreateVertexArrays(1, &r_Id); }

GLVertexArray::~GLVertexArray() { glDeleteVertexArrays(1, &r_Id); }

void GLVertexArray::bind() const { glBindVertexArray(r_Id); }

void GLVertexArray::unbind() const { glBindVertexArray(0); }

void GLVertexArray::addVertexBuffer(
    const std::shared_ptr<VertexBuffer> &_vertexBuffer) {
  GEG_CORE_ASSERT(!_vertexBuffer->getLayout().getElements().empty(),
                  "Trying to add vertex buffer without layout");

  vertexBuffer = _vertexBuffer;

  bind();
  vertexBuffer->bind();

  unsigned int offset = 0;

  const auto &layout = vertexBuffer->getLayout();
  for (const auto &element : layout) {
    switch (element.type) {
    case ShaderDataType::None:

    case ShaderDataType::Float:
    case ShaderDataType::Float2:
    case ShaderDataType::Float3:
    case ShaderDataType::Float4: {
      glEnableVertexAttribArray(v_Index);
      glVertexAttribPointer(v_Index, element.size,
                            ShaderDataTypeToOpenGLBaseType(element.type),
                            element.normalized ? GL_TRUE : GL_FALSE,
                            layout.getStride(), (void *)(size_t)offset);
      v_Index++;
      offset += element.getComponentCount() * element.size;
    }
    case ShaderDataType::Bool:
    case ShaderDataType::Int:
    case ShaderDataType::Int2:
    case ShaderDataType::Int3:
    case ShaderDataType::Int4: {
      glEnableVertexAttribArray(v_Index);
      glVertexAttribIPointer(v_Index, element.size,
                             ShaderDataTypeToOpenGLBaseType(element.type),
                             layout.getStride(), (void *)(size_t)offset);
      v_Index++;
      offset += element.getComponentCount() * element.size;
    }
    case ShaderDataType::Mat3:
    case ShaderDataType::Mat4: {
      uint8_t count = element.getComponentCount();
      for (uint8_t i = 0; i < count; i++) {
        glEnableVertexAttribArray(v_Index);
        glVertexAttribPointer(v_Index, count,
                              ShaderDataTypeToOpenGLBaseType(element.type),
                              element.normalized ? GL_TRUE : GL_FALSE,
                              layout.getStride(), (void *)(size_t)offset);
        glVertexAttribDivisor(v_Index, 1);
        v_Index++;
        offset += element.getComponentCount() * element.size;
      }
    }
    }
  }
}

void GLVertexArray::setIndexBuffer(
    const std::shared_ptr<IndexBuffer> &_indexBuffer) {
  bind();
  indexBuffer = _indexBuffer;
  indexBuffer->bind();
}

const std::shared_ptr<VertexBuffer> &GLVertexArray::getVertexBuffer() const {
  return vertexBuffer;
}

const std::shared_ptr<IndexBuffer> &GLVertexArray::getIndexBuffer() const {
  return indexBuffer;
}
} // namespace Geg
