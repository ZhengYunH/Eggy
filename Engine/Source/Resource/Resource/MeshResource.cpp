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

	void QuatHelperMesh::PrepareMeshData(List<VertexType>& Vertex, List<IndexType>& Index)
	{
		Vertex = List<VertexType>({
						{ Vector3(0.0f, 0.0f, 0.0f), mColor_ },
						{ Vector3(0.0f, 1.0f, 0.0f), mColor_ },
						{ Vector3(1.0f, 0.0f, 0.0f), mColor_ },
						{ Vector3(1.0f, 1.0f, 0.0f), mColor_ }
			});
		Index = List<IndexType>({
				0, 2, 1,
				1, 2, 3
			});
	}

	void CubeHelperMesh::PrepareMeshData(List<VertexType>& Vertex, List<IndexType>& Index)
	{
		Vertex = List<VertexType>({
			{ Vector3(-1.0f, -1.0f, -1.0f), mColor_ },
			{ Vector3(-1.0f, 1.0f, -1.0f), mColor_ },
			{ Vector3(1.0f, 1.0f, -1.0f), mColor_ },
			{ Vector3(1.0f, -1.0f, -1.0f), mColor_ },
			{ Vector3(-1.0f, -1.0f, 1.0f), mColor_ },
			{ Vector3(-1.0f, 1.0f, 1.0f), mColor_ },
			{ Vector3(1.0f, 1.0f, 1.0f), mColor_ },
			{ Vector3(1.0f, -1.0f, 1.0f), mColor_ }
		});
		
		Index = List<IndexType>({
			0, 1, 2, 2, 3, 0,	// Front
			4, 5, 1, 1, 0, 4,	// Left
			1, 5, 6, 6, 2, 1,	// Top
			7, 6, 5, 5, 4, 7,	// Back
			3, 2, 6, 6, 7, 3,	// Right
			4, 0, 3, 3, 7, 4	// Bottom
		});
	}


	void PyramidHelperMesh::PrepareMeshData(List<VertexType>& Vertex, List<IndexType>& Index)
	{
		Vertex = List<VertexType>({
						{ Vector3(0.f, 2.f, 0.f), mColor_},
						{ Vector3(1.f, 0.f, 1.f), mColor_},
						{ Vector3(1.f, 0.f, -1.f), mColor_},
						{ Vector3(-1.f, 0.f, -1.f), mColor_},
						{ Vector3(-1.f, 0.f, 1.f), mColor_},
			});
		Index = List<IndexType>({
				0, 1, 2,
				0, 2, 3,
				0, 3, 4,
				0, 4, 1,
				1, 2, 3,
				1, 3, 4
			});
	}
	

	void SphereHelperMesh::PrepareMeshData(List<VertexType>& Vertex, List<IndexType>& Index)
	{
		float startU = 0;
		float endU = 2 * MATH_PI;

		float startV = 0;
		float endV = MATH_PI;

		const size_t division = mDivision_;
		const float stepU = (endU - startU) / division;
		const float stepV = (endV - startV) / division;

		for (size_t i = 0; i < division; ++i)
		{
			for (size_t j = 0; j < division; ++j)
			{
				float u = i * stepU + startU;
				float v = j * stepV + startV;
				float un = (i + 1 == division) ? endU : (i + 1) * stepU + startU;
				float vn = (j + 1 == division) ? endV : (j + 1) * stepV + startV;

				Vector3 p0 = _GetUnitSphereSurfacePoint(u, v) * mRadius_;
				Vector3 p1 = _GetUnitSphereSurfacePoint(u, vn) * mRadius_;
				Vector3 p2 = _GetUnitSphereSurfacePoint(un, v) * mRadius_;
				Vector3 p3 = _GetUnitSphereSurfacePoint(un, vn) * mRadius_;

				Vertex.push_back({ p0, mColor_ });
				Vertex.push_back({ p1, mColor_ });
				Vertex.push_back({ p2, mColor_ });
				Vertex.push_back({ p3, mColor_ });

				uint32_t index = static_cast<uint32_t>(Vertex.size());
				Index.push_back(index + 0);
				Index.push_back(index + 1);
				Index.push_back(index + 2);

				Index.push_back(index + 1);
				Index.push_back(index + 3);
				Index.push_back(index + 2);
			}
		}
	}

}

