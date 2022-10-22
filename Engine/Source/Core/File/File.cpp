#define TINYOBJLOADER_IMPLEMENTATION // For ObjFile
#include "File.h"


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

}