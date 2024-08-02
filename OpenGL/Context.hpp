#pragma once

#include "Macros.hpp"

namespace Jam
{
	struct ContextInfo
	{
		uint16_t Width = 800, Height = 600;
		std::string Title = "Jam Window";

		bool VSync = true;
		bool DebugMode = true;
		bool Fullscreen = false;

		ContextInfo() = default;
	};

	class Context
	{
	public:
		Context(const ContextInfo& info)
			:mContextInfo(info)
		{

			if (!glfwInit())
			{
				spdlog::error("GLFW init failed!");
				return;
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

			if (mContextInfo.DebugMode)
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);


			mWindow = glfwCreateWindow(
				mContextInfo.Width, mContextInfo.Height,
				mContextInfo.Title.c_str(),
				mContextInfo.Fullscreen ? glfwGetPrimaryMonitor() : nullptr,
				nullptr
			);

			if (mWindow == nullptr)
			{
				spdlog::error(
					"Window Creation failed\nresolution = {0} X {1}\nTitile = {2}\nVSync = {3}\nDebugMode = {4}\nFullscreen = {5}\n",

					mContextInfo.Width, mContextInfo.Height,
					mContextInfo.Title,
					mContextInfo.VSync ? "Yes" : "No",
					mContextInfo.DebugMode ? "Yes" : "No",
					mContextInfo.Fullscreen ? "Yes" : "No"
				);

				return;
			}

			glfwMakeContextCurrent(mWindow);

			glfwSwapInterval(mContextInfo.VSync ? 1 : 0);

			if (!gladLoadGL())
			{
				spdlog::error("Glad Error, failed to load OpenGL Functions");
				return;
			}

			if (mContextInfo.DebugMode)
			{
				//callback built upon https://gist.github.com/liam-middlebrook/c52b069e4be2d87a6d2f
				auto callback = [](GLenum source, GLenum type, GLuint id,
					GLenum severity, GLsizei length,
					const GLchar* msg, const void* data)
					{
						std::string _source;
						std::string _type;
						std::string _severity;

						switch (source) {
						case GL_DEBUG_SOURCE_API:
							_source = "API";
							break;

						case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
							_source = "WINDOW SYSTEM";
							break;

						case GL_DEBUG_SOURCE_SHADER_COMPILER:
							_source = "SHADER COMPILER";
							break;

						case GL_DEBUG_SOURCE_THIRD_PARTY:
							_source = "THIRD PARTY";
							break;

						case GL_DEBUG_SOURCE_APPLICATION:
							_source = "APPLICATION";
							break;

						case GL_DEBUG_SOURCE_OTHER:
							_source = "UNKNOWN";
							break;

						default:
							_source = "UNKNOWN";
							break;
						}

						switch (type) {
						case GL_DEBUG_TYPE_ERROR:
							_type = "ERROR";
							break;

						case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
							_type = "DEPRECATED BEHAVIOR";
							break;

						case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
							_type = "UDEFINED BEHAVIOR";
							break;

						case GL_DEBUG_TYPE_PORTABILITY:
							_type = "PORTABILITY";
							break;

						case GL_DEBUG_TYPE_PERFORMANCE:
							_type = "PERFORMANCE";
							break;

						case GL_DEBUG_TYPE_OTHER:
							_type = "OTHER";
							break;

						case GL_DEBUG_TYPE_MARKER:
							_type = "MARKER";
							break;

						default:
							_type = "UNKNOWN";
							break;
						}

						switch (severity) {
						case GL_DEBUG_SEVERITY_HIGH:
							_severity = "HIGH";
							break;

						case GL_DEBUG_SEVERITY_MEDIUM:
							_severity = "MEDIUM";
							break;

						case GL_DEBUG_SEVERITY_LOW:
							_severity = "LOW";
							break;

						case GL_DEBUG_SEVERITY_NOTIFICATION:
							_severity = "NOTIFICATION";
							return;//opengl warning that a vertex buffer will use vram. This return prevents the warning from calling DebugBreak()

						default:
							_severity = "UNKNOWN";
							break;
						}

						spdlog::error("{0}: {1} of {2} severity, raised from {3}: {4}",
							id, _type, _severity, _source, msg);

						DebugBreak();
					};
				glEnable(GL_DEBUG_OUTPUT);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				glDebugMessageCallback(callback, nullptr);

			}

		}

		~Context()
		{
			glfwTerminate();
		}

		const ContextInfo& GetCotextInfo(void) const
		{
			return this->mContextInfo;
		}

		const unsigned char* GetVersion(void) const
		{
			return glGetString(GL_VERSION);
		}

		void MakeContextCurrent(void)
		{
			glfwMakeContextCurrent(this->mWindow);
		}

		void SwapBuffers(void)
		{
			glfwSwapBuffers(this->mWindow);
		}

		bool ShouldClose(void) const
		{
			return glfwWindowShouldClose(this->mWindow);
		}

		void PollEvents(void)
		{
			glfwPollEvents();
		}

	private:
		GLFWwindow* mWindow = nullptr;
		ContextInfo mContextInfo;

		friend class InputHandler;
	};
}
