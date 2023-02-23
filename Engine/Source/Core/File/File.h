#pragma once
#include "Core/Config.h"
#include <iostream>
#include <fstream>
#include <memory>

#include "Graphics/Renderer/VertexFactory.h"
#include <tiny_obj_loader.h>


namespace Eggy
{
	class FPath
	{
	public:
		FPath()
		{
		}

		FPath(String path)
		{
			Normalize(path);
			FromString(path);
		}

		void FromString(const String& path)
		{
			List<String> outPath;
			SplitString(path, outPath);
			if (path.empty())
				return;
			if (*(path.cbegin()) != '/')
			{
				auto itr = outPath.erase(outPath.cbegin());
				mFile_ = *itr;
			}
			mDirectories_.swap(outPath);
		}

		String ToString()
		{
			std::stringstream ss;
			for (size_t i = 0; i < mDirectories_.size(); ++i)
			{
				ss << mDirectories_[i];
				ss << "/";
			}
			ss << mFile_;
			return ss.str();
		}

		void Append(const String& path)
		{
			HYBRID_CHECK(mFile_.empty());
			InsertDirectory(path);
		}

		void InsertDirectory(const String& dir)
		{
			List<String> outPath;
			SplitString(dir, outPath);
			mDirectories_.insert(mDirectories_.end(), outPath.begin(), outPath.end());
		}

		void replaceFilePostfix(const String& postFix)
		{
			if (mFile_.empty())
				return;
			auto index = mFile_.rfind('.');
			if (index != std::string::npos)
			{
				mFile_ = mFile_.substr(0, index) + postFix;
			}
			else
			{
				mFile_ = mFile_ + postFix;
			}
		}

		String getPostFix()
		{
			if (mFile_.empty())
				return "";
			auto index = mFile_.rfind('.');
			if (index != std::string::npos)
				return mFile_.substr(index, mFile_.size() - index);
			else
				return "";
		}

		static void Normalize(String& path)
		{
			std::replace(path.begin(), path.end(), '\\', '/');
		}

		static void SplitString(const String& srcPath, List<String>& outPath)
		{
			std::istringstream iss(srcPath);
			String token;
			while (std::getline(iss, token, '/'))
			{
				outPath.push_back(token);
			}
		}

	protected:
		List<String> mDirectories_;
		String mFile_;
	};


	enum class EFileState
	{
		INIT,
		FAIL_TO_OPEN,
		OPEN,
		CLOSE,
	};
	
	enum EFileType : uint16
	{
		NONE = 0,
		OBJ = 1,
		FBX = 2
	};

	template<EFileType _FileType> 
	class SelectVertexType {};
	
	template<>
	class SelectVertexType<OBJ>
	{
	public:
		using type = TVertexType<EVF_P3F_N4B_T2F>;
	};

	template<>
	class SelectVertexType<FBX>
	{
	public:
		using type = TVertexType<EVF_P3F_C4B>;
	};


	class IFile
	{
	public:
		IFile(const String& filePath) : mFilePath_(filePath) {}
		virtual ~IFile() {}

		bool IsOpen() { return mState_ == EFileState::OPEN; }
		virtual void Read(void* buffer, size_t& inOutDataSize) = 0;
		size_t GetSize() { return mDataSize_; }
		const EFileType GetType() { return mFileType_; }

	protected:
		String mFilePath_;
		EFileState mState_{ EFileState::INIT };
		EFileType mFileType_{ EFileType::NONE };
		size_t mDataSize_{ 0 };

		DEBUG_RUN(String  mErrorMsg_;)
	};
	using FileHandle = std::shared_ptr<IFile>;


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


	class ObjFile : public IFile
	{
	public:
		constexpr const static EFileType FileType = EFileType::OBJ;
		using VertexType = SelectVertexType<FileType>::type;
		using IndexType = uint32;

	public:
		ObjFile(const String& filePath);

		// for vertex data
		void Read(void* buffer, size_t& inOutDataSize) override;

		void GetIndex(List<List<IndexType>>& indexs)
		{
			LoadFileData();
			indexs = mIndexs_;
		}

	protected:
		bool LoadFileData();

	protected:
		tinyobj::attrib_t mAttrib_;
		std::vector<tinyobj::shape_t> mShapes_;
		std::vector<tinyobj::material_t> mMaterials_;
		
		List<List<VertexType>> mVertexs_;
		List<List<IndexType>> mIndexs_;

		bool mbLoadedFileData_{ false };
	};


	class FbxFile : public IFile
	{
	public:
		constexpr const static EFileType FileType = EFileType::FBX;
		using VertexType = SelectVertexType<FileType>::type;
		using IndexType = uint32;

	public:
		FbxFile(const String& filePath);

		void Read(void* buffer, size_t& inOutDataSize) override;

	public:
		bool LoadFileData();

		List<List<VertexType>> mVertexs_;
		List<List<IndexType>> mIndexs_;

		bool mbLoadedFileData_{ false };
	};

	template<EFileType _FileType>
	class SelectFileType {};

	template<>
	class SelectFileType<OBJ>
	{
	public:
		using type = ObjFile;
	};

}
