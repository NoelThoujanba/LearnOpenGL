#pragma once

#include <Engine/Entity.hpp>
#include <Engine/Renderer.hpp>

constexpr uint32_t BulletEntityID = 0x00000002;
constexpr uint32_t ShooterEntityID = 0x00000003;

class Bullet : public Jam::Box2DEntity
{
public:
	Bullet(const Jam::BoxData& data)
		:Box2DEntity(data) {}
	void SetVelocity(const glm::vec2 velocity)
	{
		m_Velocity = velocity;
	}

	void Update(double delta)
	{
		this->Move(
			glm::vec2(m_Velocity.x * delta, m_Velocity.y * delta)
		);

		const auto& vertexData = this->GetBoxData().Vertices;

		if (abs(vertexData[0].x) > 1.5 || abs(vertexData[0].y) > 1.5)
		{
			m_ShouldDespawn = true;
			//spdlog::info("Despawn me now");
		}
	}

	void HasHit(const Jam::Box2DEntity& shooter ,const std::vector<Jam::EntityRef>& entities, std::vector<Jam::EntityRef>& hitEntites)
	{
		//TODO: Collision detection
	}

	uint32_t GetTypeID(void) const override
	{
		return BulletEntityID;
	}

	bool ShouldDespawn(void) const { return m_ShouldDespawn; }

private:
	glm::vec2 m_Velocity = glm::vec2(0.0, 0.0);
	bool m_ShouldDespawn = false;
};

class ShooterEntity : public Jam::Box2DEntity
{
public:
	ShooterEntity(const Jam::BoxData& data)
		:Box2DEntity(data) {}
	
	bool Kill(void)
	{
		m_ShouldDespawn = true;
	}

	std::unique_ptr<Bullet> Shoot(glm::vec2 velocity, double offsetMagnitude)
	{
		auto scaleAndOffsetVec2 = [](const std::array<glm::vec2, 4>& data, double factor, glm::vec2 offset) //  4 because box has four vertices
			{
				auto scaledData = data;
				for (auto& elem : scaledData)
				{
					elem *= factor;
					elem += offset;
				}
				return scaledData;
			};



		//think of a way to make bullet snap  convincingly to shooter entity
		int8_t i, j; //i,j are unit vectors;

		if (velocity.x > 0) i = +1.0;
		else if (velocity.x < 0) i = -1.0;
		else i = 0.0;

		if (velocity.y > 0) j = +1.0;
		else if (velocity.y < 0) j = -1.0;
		else j = 0.0;

		glm::vec2 voffset = glm::vec2(offsetMagnitude * i, offsetMagnitude * j);

		const auto& shooterData = this->GetBoxData();

		Jam::BoxData data;
		data.Vertices = scaleAndOffsetVec2(shooterData.Vertices, 0.3, voffset);
		data.TextureName = "Bullet";
		data.TexCords = shooterData.TexCords;
		data.Tint = shooterData.Tint;

		auto bullet = std::make_unique<Bullet>(data);
		bullet->SetVelocity(velocity);

		return bullet;
	}

	uint32_t GetTypeID(void) const override
	{
		return ShooterEntityID;
	}

	bool ShouldDespawn(void) const { return m_ShouldDespawn; }
	
private:
	std::vector<Bullet> m_BulletsFired;
	bool m_ShouldDespawn = false;
};
