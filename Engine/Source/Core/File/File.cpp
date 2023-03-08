#define TINYOBJLOADER_IMPLEMENTATION // For ObjFile
#include "File.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fbxsdk.h>
#if defined(_WIN32)
#include <windows.h>
#define stat _stat
#endif



namespace Eggy
{
	File::File(const String& filePath, EFileUsage usage /*= EFileUsage::LOAD*/, int openMode /*= std::fstream::binary*/) : IFile(filePath)
		, mUsage_(usage)
	{
		if (usage == EFileUsage::LOAD)
			openMode |= (std::fstream::ate | std::fstream::in);
		else if (usage == EFileUsage::SAVE)
			openMode |= (std::fstream::ate | std::fstream::out);

		

		mStream_.exceptions(std::fstream::failbit | std::fstream::badbit);
		try {
			mStream_.open(filePath, openMode);
		}
		catch (const std::fstream::failure& e) {
			// std::cout << strerror(errno);
			String info = e.code().message();;
			mState_ = EFileState::FAIL_TO_OPEN;
			mStream_.close();
		}
		mDataSize_ = mStream_.tellg();
		mState_ = EFileState::OPEN;
	}

	File::~File()
	{
		mState_ = EFileState::CLOSE;
		mStream_.close();
	}

	void File::Read(void* buffer, size_t& inOutDataSize)
	{
		if (inOutDataSize == 0)
		{
			inOutDataSize = mDataSize_;
			return;
		}
		HYBRID_CHECK(inOutDataSize <= mDataSize_);
		mStream_.seekg(0);
		mStream_.read((char*)buffer, inOutDataSize);
	}

	void File::Save(const char* str, size_t nStr)
	{
		HYBRID_CHECK(mUsage_ == EFileUsage::SAVE);
		mStream_.write(str, nStr);
	}

	long long File::GetModifyTime()
	{
		time_t modTime = 0;
		struct stat result;
		if (stat(mFilePath_.c_str(), &result) == 0)
		{
			modTime = result.st_mtime;
		}
		return modTime;
	}

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
				FbxStringList lUVSetNameList;
				pMesh->GetUVSetNames(lUVSetNameList);
				bool existUV = lUVSetNameList.GetCount() > 0;

				const char* nodeName = pMesh->GetName();
				for (int polygonCount = 0; polygonCount < pMesh->GetPolygonCount(); ++polygonCount)
				{
					int polygonTotal = pMesh->GetPolygonSize(polygonCount);
					int polygonStart = pMesh->GetPolygonVertexIndex(polygonCount);

					std::vector<uint32_t> indiceArray(polygonTotal);
					for (int vertIndex = 0; vertIndex < polygonTotal; ++vertIndex)
					{
						VertexType vertex{};
						int controlIndex = pMesh->GetPolygonVertices()[polygonStart + vertIndex];
						const auto& vertPos = pMesh->GetControlPointAt(controlIndex);
						vertex.Position = { (float)vertPos[0], (float)vertPos[1], (float)vertPos[2] };
						
						// normal
						FbxVector4 normal;
						pMesh->GetPolygonVertexNormal(polygonCount, vertIndex, normal);
						vertex.Normal = { (float)normal[0], (float)normal[1], (float)normal[2] };

						// uv0
						if (existUV)
						{
							FbxVector2 uv;
							bool pUnmapped = true;
							pMesh->GetPolygonVertexUV(polygonCount, vertIndex, lUVSetNameList[0], uv, pUnmapped);
							if (!pUnmapped)
								vertex.ST = { (float)uv[0], (float)uv[1] };
						}
						
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
					}
				}
			}
		}

		lSdkManager->Destroy();
		return true;
	}

	

}