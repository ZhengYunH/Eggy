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

	FileHandle FileSystem::LoadFile(String resource)
	{
		auto fileHandleItr = mFilesCache_.find(resource);
		if (fileHandleItr != mFilesCache_.end())
		{
			return fileHandleItr->second;
		}

		size_t index = resource.rfind('.');
		FileHandle file;
		if (index == std::string::npos)
		{
			file = std::make_shared<File>(resource);
		}
		else
		{
			String postFix = resource.substr(index, std::string::npos);
			if (postFix == ".obj")
			{
				file = std::make_shared<ObjFile>(resource);
			}
			else if (postFix == ".fbx")
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


