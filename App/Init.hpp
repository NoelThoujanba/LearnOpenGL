#pragma once

#include <Engine/Renderer.hpp>
#include "ChildEntities.hpp"
#include <OpenGL/Context.hpp>
#include <OpenGL/InputHandler.hpp>

#include <chrono>

constexpr glm::vec2 BulletSize = glm::vec2(0.001, 0.001);
constexpr double BulletSpeed = 0.01;

constexpr glm::vec2 EntitySize = glm::vec2(0.1, 0.1);
constexpr glm::vec2 EntityMovementSpeed = glm::vec2(0.0001, 0.0001);

constexpr double BulletOffsetFromEnitity = 0.12;

class App : public Jam::Context ,public Jam::RendererBase
{
public:
	//to be implemented by the user
	App(const Jam::ContextInfo& info)
		:Context(info), m_InputHandler(this)
	{
		//empty for now
	}
	void Init(void);
	void Update(double delta);

	void Run()
	{
		static std::chrono::high_resolution_clock::time_point start, end;

		while (!this->ShouldClose())
		{
			start = std::chrono::high_resolution_clock::now();
			Jam::ClearColorBufferBit();

			Update(m_Delta);
			this->RenderNow();

			this->SwapBuffers();
			this->PollEvents();
			end = std::chrono::high_resolution_clock::now();

			m_Delta = std::chrono::duration<double>(end - start).count();
			//spdlog::info("Actual FPS = {0}", 1 / m_Delta);
		}
	}

	double GetDelta(void) const
	{
		return m_Delta;
	}

	void OnEntityRender(const std::string& name, Jam::EntityRef entity) override
	{
		std::shared_ptr<Bullet> BulletRef = std::dynamic_pointer_cast<Bullet>(entity);
		std::shared_ptr<ShooterEntity> ShooterRef = std::dynamic_pointer_cast<ShooterEntity>(entity);

		//spdlog::info("I'm rendering this shit {0}", name);

		if (BulletRef == nullptr && ShooterRef == nullptr)
		{
			spdlog::warn("entity {0} is neither shooter nor bullet", name);
			return;
		}
		else if (BulletRef == nullptr)
		{
			if (ShooterRef->ShouldDespawn())
			{
				this->QueueEntityForRemoval(name);

				//spdlog::info("Removing entity {0}", name);
			}
		}
		else
		{
			if (BulletRef->ShouldDespawn())
			{
				this->QueueEntityForRemoval(name);
				//spdlog::info("Removing entity {0}", name);
			}
			else
			{
				BulletRef->Update(this->GetDelta());
			}
		}

	}

private:
	//std::shared_ptr<Jam::RendererBase> m_Renderer;
	Jam::InputHandler m_InputHandler;
	

	double m_Delta = 0;
};