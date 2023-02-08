#pragma once
#include "Core/Config.h"
#include "Core/Interface/ISystem.h"
#include "Core/File/File.h"

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
			for(size_t i = 0; i<mDirectories_.size(); ++i)
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

	class FileSystem : public ISystem<FileSystem>
	{
		DeclareSystem(FileSystem);
	public:
		FileHandle LoadFile(String resource);
		bool FileExist(String path);
		String& GetCacheDirectory();

		void Initialize() override {}
		void Finalize() override {}

	protected:
		Map<String, FileHandle> mFilesCache_;

	};
}