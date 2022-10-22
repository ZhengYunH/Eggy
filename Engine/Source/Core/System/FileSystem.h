#pragma once
#include "Core/Config.h"
#include "Core/Interface/ISystem.h"
#include "Core/File/File.h"

namespace Eggy
{
	class FileSystem : public ISystem<FileSystem>
	{
		DeclareSystem(FileSystem);
	public:
		FileHandle LoadFile(String resource);

	protected:
		Map<String, FileHandle> mFilesCache_;

	};
}