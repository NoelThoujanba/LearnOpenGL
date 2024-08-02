#pragma once

#include <OpenGL/Renderer.hpp>
#include <OpenGL/Texture2D.hpp>


/* vertex arrangement

		0---------1
		|		  |
		|		  |
		|		  |
		2---------3

*/

namespace Jam
{

	constexpr uint32_t BaseEntityID = 0x00000001;

	struct BoxData
	{
		std::array<glm::vec2, 4> Vertices = {};
		std::array<glm::vec2, 4> TexCords = {};
		std::array<glm::vec4, 4> Tint = {};

		std::string TextureName;
	};

	class Box2DEntity
	{
	public:
		Box2DEntity() = default;

		Box2DEntity(const BoxData& boxData)
			:mEntityData(boxData)
		{
			const auto& vertices = boxData.Vertices;

			if (
				glm::dot((vertices[1] - vertices[0]), (vertices[2] - vertices[0])) != 0 ||
				glm::dot((vertices[3] - vertices[2]), (vertices[3] - vertices[1])) != 0
				)
			{
				throw std::domain_error("Vertices for Box2D Entity must be a rectangle");
			}
		}

		Box2DEntity(const Box2DEntity&) = delete;

		virtual ~Box2DEntity() = default;

		//will possibly remove this. dynamic_cast preety much does the same
		virtual uint32_t GetTypeID(void) const { return BaseEntityID; }

		void SetData(const BoxData& boxData)
		{
			const auto& vertices = boxData.Vertices;

			if (
				glm::dot((vertices[1] - vertices[0]), (vertices[2] - vertices[0])) != 0 ||
				glm::dot((vertices[3] - vertices[2]), (vertices[3] - vertices[1])) != 0
				)
			{
				throw std::invalid_argument("Vertices for Box2D Entity must be a rectangle");
			}

			mEntityData = boxData;
		}

		void SetVertices(glm::vec2 v0, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3)
		{
			if (
				glm::dot((v1 - v0), (v2 - v0)) != 0 ||
				glm::dot((v3 - v2), (v3 - v1)) != 0
				)
			{
				throw std::invalid_argument("Vertices for Box2D Entity must be a rectangle");
			}

			mEntityData.Vertices[0] = v0;
			mEntityData.Vertices[1] = v1;
			mEntityData.Vertices[2] = v2;
			mEntityData.Vertices[3] = v3;

		}

		void SetTextureCords(glm::vec2 t0, glm::vec2 t1, glm::vec2 t2, glm::vec2 t3)
		{
			mEntityData.TexCords[0] = t0;
			mEntityData.TexCords[1] = t1;
			mEntityData.TexCords[2] = t2;
			mEntityData.TexCords[3] = t3;
		}

		void SetTintColaor(glm::vec4 c0, glm::vec4 c1, glm::vec4 c2, glm::vec4 c3)
		{
			mEntityData.Tint[0] = c0;
			mEntityData.Tint[1] = c1;
			mEntityData.Tint[2] = c2;
			mEntityData.Tint[3] = c3;
		}

		void SetTextureName(const std::string& textureName)
		{
			mEntityData.TextureName = textureName;
		}

		const BoxData& GetBoxData(void) const
		{
			return mEntityData;
		}

		
		void Move(glm::vec2 disp)
		{
			for (auto& vertex : mEntityData.Vertices)
			{
				vertex += disp;
			}
		}

	private:
		BoxData mEntityData;
	};
}