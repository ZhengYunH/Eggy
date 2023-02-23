#pragma once
#include "Core/Config.h"
#include "Core/Interface/ISystem.h"
#include "Core/File/File.h"

namespace Eggy
{
	class FileSystem : public TSystem<FileSystem>
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