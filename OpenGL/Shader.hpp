#pragma once

#include "Macros.hpp"

namespace Jam
{
	class Shader
	{
	public:
		Shader() = default;
		Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
		{
			std::ifstream vertexShaderFile(vertexShaderPath);
			if (!vertexShaderFile.is_open())
			{
				mErrorMessageLog << "Failed to Open Vertex Shader File, path = " << vertexShaderPath << '\n';
				return;
			}
			std::ifstream fragmentShaderFile(fragmentShaderPath);
			if (!fragmentShaderFile.is_open())
			{
				mErrorMessageLog << "Failed to Open Fragment Shader File, path = " << fragmentShaderPath << '\n';
				return;
			}

			std::stringstream vertexShaderSource, fragmentShaderSource;

			std::string lineBuffer;

			while (std::getline(vertexShaderFile, lineBuffer))
			{
				vertexShaderSource << lineBuffer << '\n';
			}
			while (std::getline(fragmentShaderFile, lineBuffer))
			{
				fragmentShaderSource << lineBuffer << '\n';
			}

			mProgramID = CreateShader(vertexShaderSource.str().c_str(), fragmentShaderSource.str().c_str(), *this);
		}

		Shader(const Shader&) = delete;

		~Shader()
		{
			glDeleteProgram(mProgramID);
			mProgramID = 0;
			mErrorMessageLog.clear();
		}

		bool IsOK(void) const
		{
			return mErrorMessageLog.str().length() == 0; // i.e empty string
		}

		const std::stringstream& GetErrorMessageStream(void) const
		{
			return mErrorMessageLog;
		}

		void ClearErrorLog(void)
		{
			mErrorMessageLog.clear();
		}

		void Bind(void) const
		{
			glUseProgram(mProgramID);
		}

		void LoadFromString(const std::string& vertexShaderSrc, const std::string& fragmentShaderSource)
		{
			glDeleteProgram(mProgramID);
			mUnifromLocationCache.clear();
			mProgramID = CreateShader(vertexShaderSrc.c_str(), fragmentShaderSource.c_str(), *this);
		}

		void LoadFromFile(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
		{
			glDeleteProgram(mProgramID);
			mUnifromLocationCache.clear();

			std::ifstream vertexShaderFile(vertexShaderPath);
			if (!vertexShaderFile.is_open())
			{
				mErrorMessageLog << "Failed to Open Vertex Shader File, path = " << vertexShaderPath << '\n';
				return;
			}
			std::ifstream fragmentShaderFile(fragmentShaderPath);
			if (!fragmentShaderFile.is_open())
			{
				mErrorMessageLog << "Failed to Open Fragment Shader File, path = " << fragmentShaderPath << '\n';
				return;
			}

			std::stringstream vertexShaderSource, fragmentShaderSource;

			std::string lineBuffer;

			while (std::getline(vertexShaderFile, lineBuffer))
			{
				vertexShaderSource << lineBuffer << '\n';
			}
			while (std::getline(fragmentShaderFile, lineBuffer))
			{
				fragmentShaderSource << lineBuffer << '\n';
			}

			mProgramID = CreateShader(vertexShaderSource.str().c_str(), fragmentShaderSource.str().c_str(), *this);
		}

		//float
		void SetUniform(const std::string& name, float v0)
		{
			GLuint location = GetUniformLocation(name);
			glUniform1f(location, v0);
		}

		void SetUniform(const std::string& name, float v0, float v1)
		{
			GLuint location = GetUniformLocation(name);
			glUniform2f(location, v0, v1);
		}

		void SetUniform(const std::string& name, float v0, float v1, float v2)
		{
			GLuint location = GetUniformLocation(name);
			glUniform3f(location, v0, v1, v2);
		}

		void SetUniform(const std::string& name, float v0, float v1, float v2, float v3)
		{
			GLuint location = GetUniformLocation(name);
			glUniform4f(location, v0, v1, v2, v3);
		}

		//double
		void SetUniform(const std::string& name, double v0)
		{
			GLuint location = GetUniformLocation(name);
			glUniform1d(location, v0);
		}

		void SetUniform(const std::string& name, double v0, double v1)
		{
			GLuint location = GetUniformLocation(name);
			glUniform2d(location, v0, v1);
		}

		void SetUniform(const std::string& name, double v0, double v1, double v2)
		{
			GLuint location = GetUniformLocation(name);
			glUniform3d(location, v0, v1, v2);
		}

		void SetUniform(const std::string& name, double v0, double v1, double v2, double v3)
		{
			GLuint location = GetUniformLocation(name);
			glUniform4d(location, v0, v1, v2, v3);
		}

		//int
		void SetUniform(const std::string& name, int v0)
		{
			GLuint location = GetUniformLocation(name);
			glUniform1i(location, v0);
		}
		void SetUniform(const std::string& name, int v0, int v1)
		{
			GLuint location = GetUniformLocation(name);
			glUniform2i(location, v0, v1);
		}
		void SetUniform(const std::string& name, int v0, int v1, int v2)
		{
			GLuint location = GetUniformLocation(name);
			glUniform3i(location, v0, v1, v2);
		}
		void SetUniform(const std::string& name, int v0, int v1, int v2, int v3)
		{
			GLuint location = GetUniformLocation(name);
			glUniform4i(location, v0, v1, v2, v3);
		}

		//uint
		void SetUniform(const std::string& name, unsigned int v0)
		{
			GLuint location = GetUniformLocation(name);
			glUniform1ui(location, v0);
		}
		void SetUniform(const std::string& name, unsigned int v0, unsigned int v1)
		{
			GLuint location = GetUniformLocation(name);
			glUniform2ui(location, v0, v1);
		}
		void SetUniform(const std::string& name, unsigned int v0, unsigned int v1, unsigned int v2)
		{
			GLuint location = GetUniformLocation(name);
			glUniform3ui(location, v0, v1, v2);
		}
		void SetUniform(const std::string& name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3)
		{
			GLuint location = GetUniformLocation(name);
			glUniform4ui(location, v0, v1, v2, v3);
		}

	private:
		GLuint mProgramID = 0;
		std::stringstream mErrorMessageLog;

		std::unordered_map<std::string, GLuint> mUnifromLocationCache;

		GLuint GetUniformLocation(const std::string& name)
		{
			auto location = mUnifromLocationCache.find(name);
			if (location == mUnifromLocationCache.end())//i.e not found
			{
				GLuint queriedLocation = glGetUniformLocation(mProgramID, name.c_str());
				mUnifromLocationCache[name] = queriedLocation;

				return queriedLocation;
			}
			return location->second; // std::pair, second element is value
		}

		static uint32_t CompileShader(uint32_t type, const char* source, std::stringstream& errMsg)
		{
			uint32_t shaderID = glCreateShader(type);
			glShaderSource(shaderID, 1, &source, nullptr);

			glCompileShader(shaderID);

			//Error Handling
			int status = 0;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);

			if (!status)
			{
				int length = 0;
				glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
				char* msg = new char[length];
				glGetShaderInfoLog(shaderID, length, &length, msg);

				std::string shaderType;

				if (type == GL_VERTEX_SHADER) shaderType = "Vertex Shader";
				else if (type == GL_FRAGMENT_SHADER) shaderType = "Fragment Shader";

				errMsg << shaderType << ": " << msg << "\n";
			}

			return shaderID;
		}

		static uint32_t CreateShader(const char* vertexShaderSrc, const char* fragmentShaderSrc, Shader& self)
		{
			uint32_t program = glCreateProgram();
			uint32_t vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSrc, self.mErrorMessageLog);
			uint32_t fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc, self.mErrorMessageLog);

			glAttachShader(program, vertexShader);
			glAttachShader(program, fragmentShader);

			glLinkProgram(program);
			glValidateProgram(program);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			return program;
		}
	};
}