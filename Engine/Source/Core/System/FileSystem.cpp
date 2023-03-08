#include "FileSystem.h"
#include "Core/Misc/StringHelper.h"

#include <sys/types.h>
#include <sys/stat.h>
#if defined(_WIN32)
#	include <filesystem>
#	include <windows.h>
#define stat _stat
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

	long long FileSystem::GetFileModifyTime(String path)
	{
		time_t modTime = 0;
		struct stat result;
		if (stat(path.c_str(), &result) == 0)
		{
			modTime = result.st_mtime;
		}
		return modTime;
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

	FileHandle FileSystem::CreateFile(String filePath)
	{
		FPath path = filePath;
		HYBRID_CHECK(path.IsFile());
		List<String> toCreateSubDir;
		path = path.GetParent();
		while (path && !DirectoryExist(path.ToString()))
		{
			toCreateSubDir.push_back(path.GetLast());
			path = path.GetParent();
		}
		HYBRID_CHECK(path);
#if defined(_WIN32)
		for (auto itr = toCreateSubDir.rbegin(); itr != toCreateSubDir.rend(); ++itr)
		{
			path = path + *itr;
			CreateDirectory(path.ToString());
#endif
		}
		return std::make_shared<File>(filePath, EFileUsage::SAVE, 0);
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

	bool FileSystem::DirectoryExist(String path)
	{
#if defined(_WIN32)
		DWORD ftyp = GetFileAttributesA(path.c_str());
		if (ftyp == INVALID_FILE_ATTRIBUTES)
			return false;
		return ftyp & FILE_ATTRIBUTE_DIRECTORY;
#endif
		return false;
	}

	void FileSystem::CreateDirectory(String path)
	{
#if defined(_WIN32)
		CreateDirectoryA(path.c_str(), NULL);
#endif
	}

	String& FileSystem::GetCacheDirectory()
	{
		static String s_CachePath = "Cache/";
		return s_CachePath;
	}

	

}


