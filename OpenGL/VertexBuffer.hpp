#pragma once

#include "Macros.hpp"

namespace Jam
{

	class VertexBuffer
	{
	public:
		VertexBuffer()
		{
			glGenBuffers(1, &mBufferID);
		}
		VertexBuffer(const void* data, size_t size, BufferUsage usage = BufferUsage::StaticDraw)
			:mSize(size)
		{
			glGenBuffers(1, &mBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, mBufferID);
			glBufferData(GL_ARRAY_BUFFER, size, data, (GLenum)usage);
		}
		VertexBuffer(const VertexBuffer&) = delete;

		~VertexBuffer() 
		{
			glDeleteBuffers(1, &mBufferID);
			mSize = 0;
			mBufferID = 0;
		}

		size_t Size(void) const
		{
			return mSize;
		}

		void Bind(void) const
		{
			glBindBuffer(GL_ARRAY_BUFFER, mBufferID);
		}

		void SetData(const void* data, size_t size, BufferUsage usage = BufferUsage::StaticDraw)
		{
			mSize = size;

			glBindBuffer(GL_ARRAY_BUFFER, mBufferID);
			glBufferData(GL_ARRAY_BUFFER, size, data, (GLenum)usage);
		}

	private:
		GLuint mBufferID = 0;
		size_t mSize = 0;
	};
}