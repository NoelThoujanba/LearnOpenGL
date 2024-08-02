#pragma once

#include  <OpenGL/Renderer.hpp>
#include "Entity.hpp"

namespace Jam
{

	using EntityRef = std::shared_ptr<Box2DEntity>;
	using TextureRef = std::shared_ptr<Texture2D>;

	struct VertexData
	{
		double X, Y;
		double TexX, TexY;
		double TintR, TintG, TintB, TintA;
	};

	//a whole lot of indirections and subsiquent cache miss. Oh boy.....
	class RendererBase
	{
	public:
		RendererBase(const RendererBase&) = delete;

		RendererBase()
		{
			m_VA.Bind();
			m_VB.Bind();
			m_IB.Bind();
			m_Shader.LoadFromFile("shaders/vertex.shader", "shaders/fragment.shader");
			m_Shader.Bind();

			VertexBufferLayout vbl;
			vbl.Push<double>(2); //vert pos
			vbl.Push<double>(2); //tex cords
			vbl.Push<double>(4); //tint

			m_VA.SetBuffer(m_VB, vbl);
			m_Shader.SetUniform("u_TexSlot", 0);

			m_IB.SetData(RendererBase::RenderIndieces, 6);
		}

		virtual ~RendererBase() = default;

		void AddEntity(const std::string& name,EntityRef entity)
		{
			//spdlog::info("adding entity {0}", name);

			auto pos = m_EntityList.find(name);
			if (pos != m_EntityList.end())
				throw std::invalid_argument("Entity already exists!");

			m_EntityList[name] = entity;
		}

		void QueueEntityForRemoval(const std::string& name)
		{
			m_EntityRemovalQueue.push_back(&name);
		}
		
		void RemoveEntity(const std::string& name)
		{
			//debug purpose only
			//auto pos = m_EntityList.find(name);
			//if (pos == m_EntityList.end())
			//	throw std::invalid_argument("Entity doesnt exists! name: " + name);

			//spdlog::info("Removing entity, name: {0}", name);
			m_EntityList.erase(name);
		}

		void AddTexture(const std::string& name, const std::string& texturePath)
		{
			auto pos = m_TexturePool.find(name);
			if(pos != m_TexturePool.end())
				throw std::invalid_argument("Texture already exists!");

			m_TexturePool[name] = std::make_unique<Texture2D>(texturePath);
		}

		void  RemoveTexture(const std::string& name)
		{
			//for debuging  proposes
			//if (m_EntityList.find(name) == m_EntityList.end())
				//throw std::runtime_error("Tried to remove non existant entity, name=" + name);
			//spdlog::info("Removing Texture {0}", name);
			m_TexturePool.erase(name);
		}

		EntityRef GetEntity(const std::string& name)
		{
			auto pos = m_EntityList.find(name);
			if (pos == m_EntityList.end())
			{
				throw std::invalid_argument("entity does not exist in renderer's entity list");
			}

			auto& [eName, entity] = *pos;

			return entity;
		}

		virtual void OnEntityRender(const std::string& name, EntityRef entity) { }

		void RenderNow(void)
		{
			for (const auto&[name, entity] : m_EntityList)
			{

				const BoxData& boxData = entity->GetBoxData();
				for (size_t i = 0; i < 4; ++i)// 4 because box has 4 vertices
				{
					m_VertexData[i].X = boxData.Vertices[i].x;
					m_VertexData[i].Y = boxData.Vertices[i].y;

					m_VertexData[i].TexX = boxData.TexCords[i].x;
					m_VertexData[i].TexY = boxData.TexCords[i].y;

					m_VertexData[i].TintR = boxData.Tint[i].r;
					m_VertexData[i].TintG = boxData.Tint[i].g;
					m_VertexData[i].TintB = boxData.Tint[i].b;
					m_VertexData[i].TintA = boxData.Tint[i].a;
				}

				m_VB.SetData(&m_VertexData[0], sizeof(double) * 8 * 4, BufferUsage::DynamicDraw); // no. of elemets in VertexData struct = 8, no of vertices = 4


				m_VA.Bind();
				m_VB.Bind();
				m_IB.Bind();
				m_Shader.SetUniform("u_TexSlot", 0);

				auto texPos = m_TexturePool.find(boxData.TextureName);
				if (texPos == m_TexturePool.end())
					throw std::runtime_error("Texture does not exist");
				auto &[texname, tex] = *texPos;
				tex->Bind(0);

				//spdlog::info("I'm rendering this shit {0}", name);

				

				Render(m_VA, m_IB, m_Shader);

				OnEntityRender(name, entity);
			}
			
			for (const auto& namePtr : m_EntityRemovalQueue)
			{
				this->RemoveEntity(*namePtr);
			}
			m_EntityRemovalQueue.clear();
		}

	private:
		std::map<std::string, EntityRef> m_EntityList;
		std::array<VertexData, 4> m_VertexData = {};
		std::map<std::string, std::unique_ptr<Texture2D>> m_TexturePool;

		static constexpr uint32_t RenderIndieces[6] = { 0, 1, 2, 2, 3, 1 };
	
		VertexArray m_VA;
		VertexBuffer m_VB;
		IndexBuffer m_IB;

		Shader m_Shader;

		std::vector<const std::string const*> m_EntityRemovalQueue;
	};
}