#pragma once

#include "VertexBufferLayout.hpp"
#include "VertexBuffer.hpp"

namespace Jam
{
	class VertexArray
	{
	public:
		VertexArray()
		{
			glGenVertexArrays(1, &mVertexArrayID);
		}
		VertexArray(const VertexArray&) = delete;
		~VertexArray()
		{
			glDeleteVertexArrays(1, &mVertexArrayID);
			mIndexCount = 0;
		}

		void Bind(void) const
		{
			glBindVertexArray(mVertexArrayID);
		}

		void SetBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
		{
			this->Bind();
			vb.Bind();

			const auto& elements = layout.GetElements();
			const char* offset = 0;//for ptr arithmetic
			GLuint i = 0, j = 0;

			for (i = 0; i < elements.size(); ++i)
			{
				const auto& elem = elements[i];
				glEnableVertexAttribArray(i);
				glVertexAttribPointer(i, elem.Count, elem.Type, elem.Normalized, layout.GetStride(), (const void*)offset);
				offset += elem.Count * GetTypeSize(elem.Type);
			}

			for (j = i; j < mIndexCount; ++i)
				glDisableVertexAttribArray(j);
			mIndexCount = i;
		}

	private:
		GLuint mVertexArrayID = 0;
		size_t mIndexCount = 0;
	};
}