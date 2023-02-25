#include "MeshResource.h"
#include "ResourceItem.h"
#include "System/FileSystem.h"


namespace Eggy
{
	DefineResource(MeshResource, EResourceType::Mesh);

	bool MeshResource::LoadObject() noexcept
	{
		FPath root = FileSystem::Get()->GetPackageRoot() + GetItem()->GetPath();
		FPath metaFilePath = root + "meta.xml";
		XMLFile* metaFile = dynamic_cast<XMLFile*>(FileSystem::Get()->LoadFile(metaFilePath.ToString()).get());

		if (!metaFile)
			return false;

		auto _Mesh = metaFile->GetRootNode().get_child("Mesh");
		String _MeshSource = _Mesh.get<String>("Source");
		FPath sourceFilePath = root + _MeshSource;
		IFile* sourceFile = FileSystem::Get()->LoadFile(sourceFilePath.ToString()).get();
		if (sourceFile->GetType() == EFileType::FBX)
		{
			size_t temp = 0;
			sourceFile->Read(nullptr, temp);
			FbxFile* fbxFile = static_cast<FbxFile*>(sourceFile);
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
		SetLoaded();
		return true;
	}

	bool MeshResource::SaveObject() noexcept
	{
		Unimplement();
		return false;
	}

	const QuatMesh QuatMesh::ConstMesh;

}

