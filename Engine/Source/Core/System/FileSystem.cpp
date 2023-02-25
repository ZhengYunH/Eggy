#include "FileSystem.h"
#include "Core/Misc/StringHelper.h"

#if defined(_WIN32)
#	include <filesystem>
#endif

namespace Eggy
{
	DefineSystem(FileSystem);
	
	void FileSystem::Initialize()
	{
#if defined(_WIN32)
		mRoot_ = StringHelper::WString2String(std::filesystem::current_path());
#endif
		if (mRoot_.IsFile())
			mRoot_.ConvertToDirectory();
	}

	List<String> FileSystem::GetSubDirectories(FPath path, bool retAbsPath)
	{
		List<String> subDir;
		path.ConvertToDirectory();
		String pathName = path.ToString();
#if defined(_WIN32)
		std::filesystem::path p(pathName);
		for (auto& v : std::filesystem::directory_iterator(p))
		{
			if(retAbsPath)
				subDir.push_back(v.path().string());
			else
				subDir.push_back(v.path().filename().string());
		}
#endif
		return subDir;
	}

	FileHandle FileSystem::LoadFile(String resource)
	{
		auto fileHandleItr = mFilesCache_.find(resource);
		if (fileHandleItr != mFilesCache_.end())
		{
			return fileHandleItr->second;
		}

		FPath path = resource;
		HYBRID_CHECK(path.IsFile());
		String postFix = (path.getPostFix());
		FileHandle file;
		if (postFix.empty())
		{
			file = std::make_shared<File>(resource);
		}
		else
		{
			if (postFix == "xml")
			{
				file = std::make_shared<XMLFile>(resource);
			}
			else if (postFix == "obj")
			{
				file = std::make_shared<ObjFile>(resource);
			}
			else if (postFix == "fbx")
			{
				file = std::make_shared<FbxFile>(resource);
			}
			else
			{
				file = std::make_shared<File>(resource);
			}
		}
		
		mFilesCache_.insert({ resource, file });
		return file;
	}

	bool FileSystem::FileExist(String path)
	{
		std::ifstream ifile;
		ifile.open(path);
		if (ifile)
		{
			ifile.close();
			return true;
		}
		else
		{
			return false;
		}
	}

	String& FileSystem::GetCacheDirectory()
	{
		static String s_CachePath = "Cache/";
		return s_CachePath;
	}

	

}


