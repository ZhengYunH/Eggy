#pragma once
#include "Core/Config.h"
#include <iostream>
#include <fstream>
#include <memory>

// For ObjFile
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>


namespace Eggy
{
	enum class EFileState
	{
		INIT,
		FAIL_TO_OPEN,
		OPEN,
		CLOSE,
	};

	class IFile
	{
	public:
		IFile(const String& filePath) : mFilePath_(filePath) {}

		bool IsOpen() { return mState_ == EFileState::OPEN; }
		virtual void Read(void* buffer, size_t& inOutDataSize) = 0;
		size_t GetSize() { return mDataSize_; }

	protected:
		String mFilePath_;
		EFileState mState_{ EFileState::INIT };
		size_t mDataSize_;

		DEBUG_RUN(String  mErrorMsg_;)
	};
	using FileHandle = SharedPtr<IFile>;


	class File : public IFile
	{
	public:
		File(const String& filePath, int openMode = std::ios::ate | std::ios::binary) : IFile(filePath)
		{
			mStream_ = std::ifstream(filePath, openMode);
			if (!mStream_.is_open())
			{
				mState_ = EFileState::FAIL_TO_OPEN;
				mStream_.close();
			}
			mDataSize_ = mStream_.tellg();
			mState_ = EFileState::OPEN;
			
		}
		virtual ~File()
		{
			mState_ = EFileState::CLOSE;
			mStream_.close();
		}

	public:
		void Read(void* buffer, size_t& inOutDataSize)
		{
			if (inOutDataSize > mDataSize_ || inOutDataSize == 0)
				inOutDataSize = mDataSize_;
			mStream_.seekg(0);
			mStream_.read((char*)buffer, inOutDataSize);
		}
		

	protected:
		std::ifstream mStream_;
	};


	enum class EVertexFormat : uint16
	{
		EVF_None = 0,
		// Geometric vertex.
		EVF_P3F = 1,					// Position(3F)
		EVF_P3F_C4B_T2F = 2,			// Position(3F), Color(4B), TexCoord(2F)
		EVF_P3F_N4B_T2F = 3				// Position(3F), Normal(4B), TexCoord(2F)
	};

	struct VF_P3F_N4B_T2F
	{
		/*Vector3	Position;
		Color4B	Normal;
		Vector2 ST;*/

		static constexpr EVertexFormat FORMAT = EVertexFormat::EVF_P3F_N4B_T2F;
	};

	class ObjFile : public IFile
	{
	public:
		ObjFile(const String& filePath) : IFile(filePath)
		{
			tinyobj::attrib_t attrib;
			std::string err;

			if (!tinyobj::LoadObj(&mAttrib_, &mShapes_, &mMaterials_, &mErrorMsg_, filePath.c_str()))
			{
				mState_ = EFileState::FAIL_TO_OPEN;
				return;
			}

			mState_ = EFileState::OPEN;
		}

		void Read(void* buffer, size_t& inOutDataSize) override
		{
			throw std::logic_error("The method or operation is not implemented.");
		}

	protected:
		bool LoadFileData()
		{
			if (mbLoadedFileData_)
				return true;

			mbLoadedFileData_ = true;
		}

	protected:
		tinyobj::attrib_t mAttrib_;
		std::vector<tinyobj::shape_t> mShapes_;
		std::vector<tinyobj::material_t> mMaterials_;

		bool mbLoadedFileData_;
	};


	class FbxFile : public IFile
	{
	public:
		FbxFile(const String& filePath) : IFile(filePath)
		{

		}

		void Read(void* buffer, size_t& inOutDataSize) override
		{
			throw std::logic_error("The method or operation is not implemented.");
		}
	};
}