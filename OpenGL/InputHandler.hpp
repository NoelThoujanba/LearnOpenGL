#pragma once

#include "Context.hpp"
#include "KeyAlias.hpp"

namespace Jam
{

    struct NormalizationInfo
    {
        double XMin = -1, XMax = +1;
        double YMin = -1, YMax = +1;
    };

    //context's lifetime could end and then the whole program will be fucked
    class InputHandler
    {
    public:
        InputHandler(Jam::Context* context)
            :mContext(context) {}
        InputHandler(const InputHandler&) = delete;
        ~InputHandler()
        {
            mContext = nullptr;
        }

        bool KeyPressed(Key key)
        {
            return glfwGetKey(mContext->mWindow, (int)key) == GLFW_PRESS;
        }
        bool KeyReleased(Key key)
        {
            return glfwGetKey(mContext->mWindow, (int)key) == GLFW_RELEASE;
        }
        bool KeyRepeated(Key key)
        {
            return glfwGetKey(mContext->mWindow, (int)key) == GLFW_REPEAT;
        }

        bool MouseLeftClicked(void)
        {
            return glfwGetMouseButton(mContext->mWindow, GLFW_MOUSE_BUTTON_LEFT);
        }
        bool MouseRightClicked(void)
        {
            return glfwGetMouseButton(mContext->mWindow, GLFW_MOUSE_BUTTON_RIGHT);
        }

        bool MouseMiddleClicked(void)
        {
            return glfwGetMouseButton(mContext->mWindow, GLFW_MOUSE_BUTTON_MIDDLE);
        }

        bool MouseButton1Clicked(void)
        {
            return glfwGetMouseButton(mContext->mWindow, GLFW_MOUSE_BUTTON_1);
        }
        bool MouseButton2Clicked(void)
        {
            return glfwGetMouseButton(mContext->mWindow, GLFW_MOUSE_BUTTON_2);
        }
        bool MouseButton3Clicked(void)
        {
            return glfwGetMouseButton(mContext->mWindow, GLFW_MOUSE_BUTTON_3);
        }
        bool MouseButton4Clicked(void)
        {
            return glfwGetMouseButton(mContext->mWindow, GLFW_MOUSE_BUTTON_4);
        }

        

        //01 normalization formula
        //Zi = [Xi - min(X)] / [Max(X) - Min(X)]
        glm::vec2 GetMousePos(vec2 aspectRatio)
        {
            
            double winWidth = mContext->GetCotextInfo().Width;
            double winHeight = mContext->GetCotextInfo().Height;

            double rawXPos, rawYPos;
            glfwGetCursorPos(mContext->mWindow, &rawXPos, &rawYPos);

            double normalized01X = NormalizeFrom0To1<double>(rawXPos, 0, winWidth);
            double normalized01Y = NormalizeFrom0To1<double>(rawYPos, 0, winHeight);

            double normalizedX = Denormalize<double>(normalized01X, -1, +1); 
            double normalizedY = -Denormalize<double>(normalized01Y, -1, +1);

            return glm::vec2(normalizedX, normalizedY);
        }

    private:
        Jam::Context* mContext = nullptr;
    };
}