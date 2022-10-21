#pragma once
#include "Core/Config.h"
#include <iostream>
#include <fstream>
#include <memory>

#include "Graphics/Renderer/VertexFactory.h"

// For ObjFile
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>


namespace Eggy
{
	enum class EFileState
	{
		INIT,
		FAIL_TO_OPEN,
		OPEN,
		CLOSE,
	};

	class IFile
	{
	public:
		IFile(const String& filePath) : mFilePath_(filePath) {}
		virtual ~IFile() {}

		bool IsOpen() { return mState_ == EFileState::OPEN; }
		virtual void Read(void* buffer, size_t& inOutDataSize) = 0;
		size_t GetSize() { return mDataSize_; }

	protected:
		String mFilePath_;
		EFileState mState_{ EFileState::INIT };
		size_t mDataSize_;

		DEBUG_RUN(String  mErrorMsg_;)
	};
	using FileHandle = std::shared_ptr<IFile>;


	class File : public IFile
	{
	public:
		File(const String& filePath, int openMode = std::ios::ate | std::ios::binary) : IFile(filePath)
		{
			mStream_ = std::ifstream(filePath, openMode);
			if (!mStream_.is_open())
			{
				mState_ = EFileState::FAIL_TO_OPEN;
				mStream_.close();
			}
			mDataSize_ = mStream_.tellg();
			mState_ = EFileState::OPEN;
			
		}
		virtual ~File()
		{
			mState_ = EFileState::CLOSE;
			mStream_.close();
		}

	public:
		void Read(void* buffer, size_t& inOutDataSize)
		{
			if (inOutDataSize > mDataSize_ || inOutDataSize == 0)
				inOutDataSize = mDataSize_;
			mStream_.seekg(0);
			mStream_.read((char*)buffer, inOutDataSize);
		}
		

	protected:
		std::ifstream mStream_;
	};

	class ObjFile : public IFile
	{
	public:
		using VertexType = TVertexType<EVF_P3F_N4B_T2F>;
		using IndexType = uint32;

	public:
		ObjFile(const String& filePath) : IFile(filePath)
		{
			tinyobj::attrib_t attrib;
			std::string err;

			if (!tinyobj::LoadObj(&mAttrib_, &mShapes_, &mMaterials_, &mErrorMsg_, filePath.c_str()))
			{
				mState_ = EFileState::FAIL_TO_OPEN;
				return;
			}

			mState_ = EFileState::OPEN;
		}

		void Read(void* buffer, size_t& inOutDataSize) override
		{
			throw std::logic_error("The method or operation is not implemented.");
		}


	protected:
		bool LoadFileData()
		{
			if (mbLoadedFileData_)
				return true;

			mVertexs_.resize(mShapes_.size());
			mIndexs_.resize(mShapes_.size());

			std::unordered_map<VertexType, uint32_t> uniqueVertices{};
			for (size_t i =0; i< mShapes_.size(); ++i)
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
					
					if(uniqueVertices.count(vertex) == 0)
					{
						uniqueVertices[vertex] = static_cast<IndexType>(vertexList.size());
						vertexList.push_back(vertex);
					}

					indexList.push_back(uniqueVertices[vertex]);
				}
			}

			mbLoadedFileData_ = true;
		}

	protected:
		tinyobj::attrib_t mAttrib_;
		std::vector<tinyobj::shape_t> mShapes_;
		std::vector<tinyobj::material_t> mMaterials_;
		
		List<List<VertexType>> mVertexs_;
		List<List<IndexType>> mIndexs_;


		bool mbLoadedFileData_;
	};


	class FbxFile : public IFile
	{
	public:
		FbxFile(const String& filePath) : IFile(filePath)
		{

		}

		void Read(void* buffer, size_t& inOutDataSize) override
		{
			throw std::logic_error("The method or operation is not implemented.");
		}
	};
}
