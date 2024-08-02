#pragma once

#include "Macros.hpp"

namespace Jam
{

    inline size_t GetTypeSize(GLenum type)
    {
        switch (type)
        {
            case GL_FLOAT: return sizeof(GLfloat);
            case GL_DOUBLE: return sizeof(GLdouble);
            
            case GL_INT: return sizeof(GLint);
            case GL_UNSIGNED_INT: return sizeof(GLuint);
            case GL_BYTE: return sizeof(GLbyte);
            case GL_UNSIGNED_BYTE: return sizeof(GL_UNSIGNED_BYTE);

            default: return 0;
        }
    }

    struct VertexBufferElement
    {
        GLenum Type;
        size_t Count;
        GLboolean Normalized;
    };

    class VertexBufferLayout
    {
    public:
        VertexBufferLayout() = default;
        VertexBufferLayout(const VertexBufferLayout& other)
        {
            this->mElements = other.mElements;
            this->mStride = other.mStride;
        }
        VertexBufferLayout(VertexBufferLayout&& other) noexcept
        {
            this->mElements = std::move(other.mElements);
            this->mStride = other.mStride;

            other.mStride = 0;
        }

        template<typename T>
        void Push(size_t count) = delete;

        const std::vector<VertexBufferElement>& GetElements(void) const { return mElements; }
        uint32_t GetStride(void) const
        {
            return mStride;
        }
    private:
        std::vector<VertexBufferElement> mElements;
        size_t mStride = 0;
    };

    template<>
    inline void VertexBufferLayout::Push<GLfloat>(size_t count)
    {
        mElements.push_back({ GL_FLOAT , count , GL_FALSE });
        mStride += sizeof(GLfloat) * count;
    }
    template<>
    inline void VertexBufferLayout::Push<double>(size_t count)
    {
        mElements.push_back({ GL_DOUBLE , count , GL_FALSE });
        mStride += sizeof(GLdouble) * count;
    }

}