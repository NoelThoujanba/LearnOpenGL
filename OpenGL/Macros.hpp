#pragma once

#define JAM_WINDOWS_CONSOLE

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>
#include <glm/glm.hpp>


#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace Jam
{
    void DebugBreak(void)
    {
        #ifdef MSVC
        __debugbreak();
        #else
        throw std::runtime_error("Jam::DebugBreak()");
        #endif
    }

    template<typename T>
    T NormalizeFrom0To1(T value, T min, T max)
    {
        return (value - min) / (max - min);
    }

    template<typename T>
    T Denormalize(T value, T min, T max)
    {
        return value * (max - min) + min;
    }

    void EnableWireframeMode(void)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void DisableWireframeMode(void)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    using vec2 = glm::vec2;


    enum class BufferUsage : GLenum
    {
        StaticDraw = GL_STATIC_DRAW,
        DynamicDraw = GL_DYNAMIC_DRAW
    };
}