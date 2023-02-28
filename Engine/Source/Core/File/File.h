#pragma once
#include "Core/Config.h"
#include <iostream>
#include <fstream>
#include <memory>

#include "Graphics/Renderer/VertexFactory.h"
#include <tiny_obj_loader.h>
#include "XMLArchive.h"


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
			if (*(path.rbegin()) != '/')
			{
				auto itr = outPath.rbegin();
				mFile_ = *itr;
				outPath.pop_back();
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
				mFile_ = mFile_.substr(0, index + 1) + postFix;
			}
			else
			{
				mFile_ = mFile_ + postFix;
			}
		}

		FPath GetParent()
		{
			FPath path = *this;
			if (path.IsFile())
				path.mFile_ = "";
			else
			{
				if (!path.mDirectories_.empty())
					path.mDirectories_.pop_back();
			}
			return path;
		}

		String GetLast()
		{
			if (IsFile())
				return mFile_;
			else
			{
				if (mDirectories_.empty())
					return "";
				else
					return mDirectories_.back();
			}
		}

		String getPostFix()
		{
			if (mFile_.empty())
				return "";
			auto index = mFile_.rfind('.');
			if (index != std::string::npos || index + 1 == mFile_.size())
				return mFile_.substr(index + 1, mFile_.size() - index - 1);
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

		bool IsDirectory()
		{
			return mFile_.empty();
		}

		bool IsFile()
		{
			return !mFile_.empty();
		}

		void ConvertToDirectory()
		{
			if (IsFile())
			{
				mDirectories_.emplace_back(std::move(mFile_));
				mFile_ = "";
			}
		}

		FPath operator+ (FPath&& rhs)
		{
			FPath ret = *this;
			if (ret.IsFile())
				ret.mDirectories_.emplace_back(std::move(ret.mFile_));

			ret.mDirectories_.reserve(ret.mDirectories_.size() + rhs.mDirectories_.size());
			ret.mDirectories_.insert(ret.mDirectories_.end(), rhs.mDirectories_.begin(), rhs.mDirectories_.end());
			ret.mFile_ = rhs.mFile_;
			return ret;
		}

		FPath operator+ (const char* str)
		{
			return *this + String(str);
		}

		operator bool() const noexcept
		{
			return !mDirectories_.empty() || !mFile_.empty();
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

	enum class EFileUsage
	{
		LOAD,
		SAVE
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
		using type = TVertexType<EVF_P3F_N3F_T2F>;
	};

	template<>
	class SelectVertexType<FBX>
	{
	public:
		using type = TVertexType<EVF_P3F_N3F_T2F>;
	};


	class IFile
	{
	public:
		IFile(const String& filePath) : mFilePath_(filePath) {}
		virtual ~IFile() {}

		bool IsOpen() { return mState_ == EFileState::OPEN; }
		virtual void Read(void* buffer, size_t& inOutDataSize) {};
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
		File(const String& filePath, EFileUsage usage = EFileUsage::LOAD, int openMode = std::fstream::binary)
			: IFile(filePath)
			, mUsage_(usage)
		{
			if (usage == EFileUsage::LOAD)
				openMode |= (std::fstream::ate | std::fstream::in);
			else if (usage == EFileUsage::SAVE)
				openMode |= (std::fstream::ate | std::fstream::out);

			mStream_.exceptions(std::fstream::failbit | std::fstream::badbit);
			try {
				mStream_.open(filePath, openMode);
			}
			catch (const std::fstream::failure& e) {
				// std::cout << strerror(errno);
				String info = e.code().message();;
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
			if (inOutDataSize == 0)
			{
				inOutDataSize = mDataSize_;
				return;
			}
			HYBRID_CHECK(inOutDataSize <= mDataSize_);
			mStream_.seekg(0);
			mStream_.read((char*)buffer, inOutDataSize);
		}

		void Save(const char* str, size_t nStr)
		{
			HYBRID_CHECK(mUsage_ == EFileUsage::SAVE);
			mStream_.write(str, nStr);
		}

	protected:
		std::fstream mStream_;
		EFileUsage mUsage_;
	};

	class XMLFile : public IFile
	{
	public:
		XMLFile(const String& filePath) : IFile(filePath) 
		{
			mAr_ = new XmlParser(filePath, ARCHIVE_USAGE::LOAD);
			mAr_->Load();
		}

		pt::ptree& GetRootNode() { return mAr_->GetPTree(); }

	protected:
		XmlParser* mAr_{ nullptr };
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
