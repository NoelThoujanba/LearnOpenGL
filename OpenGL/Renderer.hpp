#pragma once

#include "VertexArray.hpp"
#include "Shader.hpp"
#include "IndexBuffer.hpp"

namespace Jam
{
	void ClearColorBufferBit(void)
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void Render(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
	{
		va.Bind();
		ib.Bind();
		shader.Bind();

		glDrawElements(GL_TRIANGLES, ib.Count(), GL_UNSIGNED_INT, nullptr);
	}
}