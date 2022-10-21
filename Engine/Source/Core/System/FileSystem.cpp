#include "FileSystem.h"

namespace Eggy
{
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
}


