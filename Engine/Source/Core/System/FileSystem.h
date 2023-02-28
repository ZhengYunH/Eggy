#pragma once
#include "Core/Config.h"
#include "Core/Interface/ISystem.h"
#include "Core/File/File.h"

namespace Eggy
{
	class IFileSystem
	{

	};

	class SubFileSystem : public IFileSystem
	{

	};

	class FileSystem : public TSystem<FileSystem>, public IFileSystem
	{
		DeclareSystem(FileSystem);
	public:
		FileHandle LoadFile(String resource);
		bool FileExist(String path);
		bool DirectoryExist(String path);
		void CreateDirectory(String path);
		String& GetCacheDirectory();

		void Initialize() override;
		void Finalize() override {}

		FileHandle CreateFile(String filePath);
	public:
		FPath GetRoot()
		{
			return mRoot_;
		}

		FPath GetPackageRoot()
		{
			return mRoot_ + "Package/";
		}

		FPath GetShaderRoot()
		{
			return mRoot_ + "Engine/Shader/";
		}

		FPath GetToolRoot()
		{
			return mRoot_ + "Tools/";
		}

		FPath GetLogRoot()
		{
			return mRoot_ + "Cache/Log/";
		}

		List<String> GetSubDirectories(FPath path, bool retAbsPath=false);

	protected:
		Map<String, FileHandle> mFilesCache_;
		FPath mRoot_;
	};
}