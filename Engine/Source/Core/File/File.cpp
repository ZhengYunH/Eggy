#define TINYOBJLOADER_IMPLEMENTATION // For ObjFile
#include "File.h"

#include <fbxsdk.h>
#include <windows.h>


namespace Eggy
{
	ObjFile::ObjFile(const String& filePath) : IFile(filePath)
	{
		mFileType_ = EFileType::OBJ;

		tinyobj::attrib_t attrib;
		std::string err;

		if (!tinyobj::LoadObj(&mAttrib_, &mShapes_, &mMaterials_, &mErrorMsg_, filePath.c_str()))
		{
			mState_ = EFileState::FAIL_TO_OPEN;
			return;
		}

		mState_ = EFileState::OPEN;
		mbLoadedFileData_ = false;
	}

	void ObjFile::Read(void* buffer, size_t& inOutDataSize)
	{
		LoadFileData();

		inOutDataSize = sizeof(mVertexs_) / sizeof(VertexType);
		buffer = new VertexType[inOutDataSize];
		size_t offset = 0;
		for (size_t i = 0; i < mVertexs_.size(); ++i)
		{
			memcpy((char*)buffer + offset, mVertexs_[i].data(), sizeof(mVertexs_[i]));
			offset = sizeof(mVertexs_[i]);
		}

	}

	bool ObjFile::LoadFileData()
	{
		if (mbLoadedFileData_)
			return true;

		mVertexs_.resize(mShapes_.size());
		mIndexs_.resize(mShapes_.size());

		std::unordered_map<VertexType, uint32_t> uniqueVertices{};
		for (size_t i = 0; i < mShapes_.size(); ++i)
		{
			const auto& shape = mShapes_[i];
			auto& vertexList = mVertexs_[i];
			auto& indexList = mIndexs_[i];
			for (const auto& index : shape.mesh.indices)
			{
				VertexType vertex
				{
					// Position
					Vector3(
						float(mAttrib_.vertices[3 * index.vertex_index + 0]),
						float(mAttrib_.vertices[3 * index.vertex_index + 1]),
						float(mAttrib_.vertices[3 * index.vertex_index + 2])
					),
					// Normal
					Vector3(
						float(mAttrib_.normals[3 * index.normal_index + 0]),
						float(mAttrib_.normals[3 * index.normal_index + 1]),
						float(mAttrib_.normals[3 * index.normal_index + 2])
					),
					// ST
					Vector2(
						float(mAttrib_.texcoords[3 * index.texcoord_index + 0]),
						float(mAttrib_.texcoords[3 * index.texcoord_index + 1])
					)
				};

				if (uniqueVertices.count(vertex) == 0)
				{
					uniqueVertices[vertex] = static_cast<IndexType>(vertexList.size());
					vertexList.push_back(vertex);
				}

				indexList.push_back(uniqueVertices[vertex]);
			}
		}

		mbLoadedFileData_ = true;
		return true;
	}

	FbxFile::FbxFile(const String& filePath) : IFile(filePath)
	{
		mFileType_ = EFileType::FBX;
		mbLoadedFileData_ = false;
	}

	void FbxFile::Read(void* buffer, size_t& inOutDataSize)
	{
		LoadFileData();
	}

	bool FbxFile::LoadFileData()
	{
		FbxManager* lSdkManager = FbxManager::Create();

		// Create the IO settings object.
		FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
		lSdkManager->SetIOSettings(ios);

		// Create an importer using the SDK manager.
		FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

		if (!lImporter->Initialize(mFilePath_.c_str(), -1, lSdkManager->GetIOSettings()))
		{
			throw std::runtime_error(lImporter->GetStatus().GetErrorString());
		}
		// Create a new scene so that it can be populated by the imported file.
		FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

		// Import the contents of the file into the scene.
		lImporter->Import(lScene);

		// The file is imported, so get rid of the importer.
		lImporter->Destroy();

		std::unordered_map<VertexType, uint32_t> uniqueVertices{};
		mVertexs_.resize(lScene->GetGeometryCount());
		mIndexs_.resize(lScene->GetGeometryCount());
		for (int meshIndex = 0; meshIndex < lScene->GetGeometryCount(); ++meshIndex)
		{
			auto& outVertexs = mVertexs_[meshIndex];
			auto& outIndexs = mIndexs_[meshIndex];
			const FbxMesh* pMesh = static_cast<const FbxMesh*>(lScene->GetGeometry(meshIndex));
			if (pMesh)
			{
				const char* nodeName = pMesh->GetName();

				auto ttt = pMesh->GetPolygonCount();
				for (int polygonCount = 0; polygonCount < pMesh->GetPolygonCount(); ++polygonCount)
				{
					int polygonTotal = pMesh->GetPolygonSize(polygonCount);
					int polygonStart = pMesh->GetPolygonVertexIndex(polygonCount);

					std::vector<uint32_t> indiceArray(polygonTotal);
					for (int vertIndex = 0; vertIndex < polygonTotal; ++vertIndex)
					{
						VertexType vertex{};
						int vertexIndex = pMesh->GetPolygonVertices()[polygonStart + vertIndex];
						const auto& vertPos = pMesh->GetControlPointAt(vertexIndex);
						vertex.Position = { (float)vertPos[0], (float)vertPos[1], (float)vertPos[2] };
						vertex.Color = Color4B_WRITE;

						if (uniqueVertices.count(vertex) == 0) {
							uniqueVertices[vertex] = static_cast<uint32_t>(outVertexs.size());
							outVertexs.push_back(vertex);
						}

						indiceArray[vertIndex] = uniqueVertices[vertex];
					}

					for (size_t polygonCount = 0; polygonCount <= polygonTotal - 3; ++polygonCount)
					{
						outIndexs.push_back(indiceArray[polygonCount]);
						outIndexs.push_back(indiceArray[polygonCount + 1]);
						outIndexs.push_back(indiceArray[polygonCount + 2]);

						/*outIndices.push_back(indiceArray[polygonCount]);
						outIndices.push_back(indiceArray[polygonCount + polygonCount % 2 + 1]);
						outIndices.push_back(indiceArray[polygonCount + 2 - polygonCount % 2]);*/
					}
				}
			}
		}

		lSdkManager->Destroy();
		return true;
	}

}