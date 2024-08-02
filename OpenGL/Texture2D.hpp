#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include "Macros.hpp"
#include "stb_image.h"

namespace Jam
{
	static void EnableTextureBlending(void)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	constexpr GLint TEXTURE_FILTER_MODE = GL_LINEAR;

	class Texture2D
	{
	public:
		Texture2D()
		{
			glGenTextures(1, &mTextureID);
			glBindTexture(GL_TEXTURE_2D, mTextureID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TEXTURE_FILTER_MODE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TEXTURE_FILTER_MODE);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		}
		
		Texture2D(const std::string& filePath)
			:mFilePath(filePath)
		{

			stbi_set_flip_vertically_on_load(1);
			mTextureData = stbi_load(filePath.c_str(), &mWidth, &mHeight, &mBPP, 4);
			if (mTextureData == nullptr)
			{
				spdlog::error("Failed to load texture");
				DebugBreak();
			}

			glGenTextures(1, &mTextureID);
			glBindTexture(GL_TEXTURE_2D, mTextureID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TEXTURE_FILTER_MODE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TEXTURE_FILTER_MODE);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mTextureData);

			mImgFree = stbi_image_free;
		}

		Texture2D(const Texture2D&) = delete;

		Texture2D(Texture2D&& other) noexcept
		{
			other.mTextureID = this->mTextureID;
			other.mTextureData = this->mTextureData;

			other.mWidth = this->mWidth;
			other.mHeight = this->mHeight;

			other.mBPP = this->mBPP;

			other.mImgFree = this->mImgFree;

			this->mTextureID = 0;
			this->mTextureData = nullptr;

			this->mWidth = 0;
			this->mHeight = 0;
			this->mBPP = 0;

			//this->mImgFree = no can do
		}

		~Texture2D()
		{
			if(mImgFree == nullptr)
				stbi_image_free(mTextureData);

			mTextureData = nullptr; 
			glDeleteTextures(1, &mTextureID);
		}

		int GetWidth(void) const
		{
			return mWidth;
		}
		int GetHeight(void) const
		{
			return mHeight;
		}
		int GetBPP(void) const
		{
			return mBPP;
		}

		void Bind(uint8_t slot)
		{
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, mTextureID);
		}

		void Load(const std::string& filePath)
		{
			mImgFree(mTextureData);
			mTextureData = stbi_load(filePath.c_str(), &mWidth, &mHeight, &mBPP, 4);
			if (mTextureData == nullptr)
			{
				spdlog::error("Failed to load texture");
				DebugBreak();
			}
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mTextureData);

			mImgFree = stbi_image_free;
		}

		void Load(const void* data, int width, int height, int BPP)
		{
			stbi_image_free(mTextureData);

			size_t dataSize = width * height * BPP;
			mTextureData = malloc(dataSize);

			if (mTextureData == nullptr)
			{
				spdlog::error("failed to allocate texture buffer on CPU");
				DebugBreak();
			}

			for (size_t i = 0; mTextureData != nullptr && i < dataSize; ++i)
			{
				((char*)mTextureData)[i] = ((char*)data)[i]; // why tf is the compiler bitching?
			}

			mWidth = width;
			mHeight = height;
			mBPP = BPP;

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mTextureData);
			mImgFree = std::free;
		}

	private:
		GLuint mTextureID = 0;
		std::string mFilePath;
		int mWidth = 0, mHeight = 0;
		int mBPP = 0;
		void* mTextureData = nullptr;

		void (*mImgFree)(void*) = nullptr; //over-engineering
	};
}
