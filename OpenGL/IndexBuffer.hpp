#pragma once

#include "Macros.hpp"

namespace Jam
{
	class IndexBuffer
	{
	public:
		IndexBuffer()
		{
			glGenBuffers(1, &mBufferID);
		}
		IndexBuffer(const uint32_t* data, size_t count, BufferUsage usage = BufferUsage::StaticDraw)
			:mCount(count)
		{
			glGenBuffers(1, &mBufferID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data ,(GLenum)usage);
		}
		IndexBuffer(const IndexBuffer&) = delete;

		~IndexBuffer()
		{
			mCount = 0;
			mBufferID = 0;
			glDeleteBuffers(1, &mBufferID);
		}

		size_t Count(void) const
		{
			return mCount;
		}

		void Bind(void) const
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferID);
		}

		void SetData(const uint32_t* data, size_t count, BufferUsage usage = BufferUsage::StaticDraw)
		{
			mCount = count;

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, (GLenum)usage);
		}

	private:
		GLuint mBufferID = 0;
		size_t mCount = 0;
	};
}