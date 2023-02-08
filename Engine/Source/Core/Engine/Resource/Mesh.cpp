#include "Mesh.h"
#include "Core/File/File.h"


namespace Eggy
{
	void MeshResource::Deserialize(IFile* file) noexcept
	{
		if (!file)
			return;

		if (file->GetType() == EFileType::FBX)
		{
			size_t temp = 0;
			file->Read(nullptr, temp);
			FbxFile* fbxFile = static_cast<FbxFile*>(file);
			size_t nMesh = Min(fbxFile->mVertexs_.size(), fbxFile->mIndexs_.size());
			for (size_t i = 0; i < nMesh; ++i)
			{
				auto& vertex = fbxFile->mVertexs_[i];
				auto& index = fbxFile->mIndexs_[i];
				using MeshType = MeshData<FbxFile::VertexType::FORMAT>;
				MeshType* geometry = new MeshType();
				mGeometrys_.push_back(geometry);
				geometry->SetupData(vertex, index);
			}
		}
		else
		{
			Unimplement(0);
		}
	}

}

