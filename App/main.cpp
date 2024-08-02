#include "Init.hpp"
#include "ChildEntities.hpp"
#include <iostream>
#include <cmath>

std::vector<std::shared_ptr<Bullet>> bullets;
std::vector<ShooterEntity> shooterEntites;

std::shared_ptr<ShooterEntity> Shooter;

size_t BulletCount = 0;

void App::Init(void)
{
	this->AddTexture("Bullet", "textures/bullet.png");
	this->AddTexture("Shooter", "textures/shooter.png");


    Jam::BoxData data;
    data.Vertices = {
        glm::vec2(-0.09, +0.09),
        glm::vec2(+0.09, +0.09),
        glm::vec2(-0.09, -0.09),
        glm::vec2(+0.09, -0.09)
    };

    data.TexCords = {
        glm::vec2(-1.0, +1.0),
        glm::vec2(+1.0, +1.0),
        glm::vec2(-1.0, -1.0),
        glm::vec2(+1.0,-1.0)
    };

    data.Tint = {
        glm::vec4(1.0, 1.0, 1.0, 1.0),
        glm::vec4(1.0, 1.0, 1.0, 1.0),
        glm::vec4(1.0, 1.0, 1.0, 1.0),
        glm::vec4(1.0, 1.0, 1.0, 1.0)
    };

    data.TextureName = "Shooter";
	std::shared_ptr<Jam::Box2DEntity> shooter = std::make_shared<ShooterEntity>(data);
	this->AddEntity("player", shooter);

    Shooter = std::dynamic_pointer_cast<ShooterEntity>(shooter);
    if (Shooter == nullptr)
    {
        spdlog::error("Dynamic cast failed!");
        DebugBreak();
    }
}

void App::Update(double delta)
{
	 //memory leak incoming
    if (m_InputHandler.MouseLeftClicked())
    {

        //gotta center this shit
        glm::vec2 entityPoint = Shooter->GetBoxData().Vertices[0];
        glm::vec2 mousePoint = m_InputHandler.GetMousePos(glm::vec2(1, 1));
        
        auto GetUnitVector = [](glm::vec2 v)
            {
                double magnitude = sqrt(v.x * v.x + v.y * v.y);
                return glm::vec2(v.x / magnitude, v.y / magnitude);
            };

        std::shared_ptr<Bullet> bullet = Shooter->Shoot(GetUnitVector(mousePoint - entityPoint), BulletOffsetFromEnitity);

        //std::string bname = std::string("Bullet") + std::to_string(BulletCount++);
        this->AddEntity("Bullet" +  std::to_string(BulletCount++), bullet);
        //bullets.push_back(bullet);
    }

    if (m_InputHandler.KeyPressed(Jam::Key::ONE))
    {
        Jam::EnableWireframeMode();
    }
    if (m_InputHandler.KeyPressed(Jam::Key::TWO))
    {
        Jam::DisableWireframeMode();
    }
}

int main(void)
{
	Jam::ContextInfo info;
	App app(info);
    app.Init();
	app.Run();
}