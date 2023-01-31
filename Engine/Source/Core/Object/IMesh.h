#pragma once
#include "Core/Config.h"
#include "Core/Interface/IRenderMesh.h"
#include "Core/Engine/Resource/ResourceObject.h"
#include "Graphics/Renderer/VertexFactory.h"

namespace Eggy
{
	struct IMeshData
	{
		virtual size_t GetVertexData(void*& Data) = 0;
		virtual size_t GetIndexData(void*& Data) = 0;

		virtual size_t GetVertexStride() = 0;
		virtual size_t GetIndexStride() = 0;

		virtual void GetVertexDesc(List<IInputLayout::InputElementDesc>& Descs) = 0;
	};

	template<EVertexFormat Format>
	struct MeshData : public IMeshData
	{
		using VertexType = TVertexType<Format>;
		using IndexType = uint32;

		VertexType* Vertex{ nullptr };
		uint32	nVertex{ 0 };

		IndexType* Index{ nullptr };
		uint32	nIndex{ 0 };

		IRenderMesh* RenderMesh{ nullptr };

		MeshData() = default;
		~MeshData()
		{
			SafeDestroyArray(Vertex);
			SafeDestroyArray(Index);
		}

		void SetupData(List<VertexType>&& vertex, List<IndexType>&& index)
		{
			nVertex = static_cast<uint32>(vertex.size());
			Vertex = VertexFactory::AllocateVertex<Format>(nVertex);
			memcpy(Vertex, vertex.data(), nVertex * sizeof(VertexType));
	
			nIndex = static_cast<uint32>(index.size());
			Index = new IndexType[nIndex];
			memcpy(Index, index.data(), nIndex * sizeof(IndexType));
		}

		virtual size_t GetVertexData(void*& Data) override
		{
			HYBRID_CHECK(Vertex);

			Data = Vertex;
			return nVertex;
		}

		size_t GetIndexData(void*& Data) override
		{
			HYBRID_CHECK(Index);

			Data = Index;
			return nIndex;
		}

		size_t GetVertexStride() override
		{
			return VertexType::GetSize();
		}

		size_t GetIndexStride() override
		{
			return sizeof(IndexType);
		}

		void GetVertexDesc(List<IInputLayout::InputElementDesc>& Descs)
		{
			return VertexType::GetDesc(Descs);
		}
	};

	struct QuatMesh : public MeshData<EVF_P3F_C4B>
	{
		using Parent = MeshData<EVF_P3F_C4B>;
		QuatMesh() : Parent()
		{
			CreateMeshData();
		}

		QuatMesh(Color4B c) : Parent(), Color(c)
		{
			CreateMeshData();
		}

		void CreateMeshData()
		{
			SetupData(
				List<VertexType>({
						{ Vector3(-0.5f, -0.5f, 0.0f), Color },
						{ Vector3(-0.5f, 0.5f, 0.0f), Color },
						{ Vector3(0.5f, 0.5f, 0.0f), Color },
						{ Vector3(0.5f, -0.5f, 0.0f), Color }
					}),
				List<IndexType>({
						0, 1, 2,
						0, 2, 3
					})
			);
		}

		Color4B Color{ Color4B_RED };
	};

	struct MeshResource : public ResourceObject
	{
		void Deserialize(IFile* file) noexcept override
		{
		}

		IMeshData* mGeometry_;
	};

	class Mesh
	{
	public:
		void SetResource(MeshResource* Resource)
		{
			mResource_ = Resource;
		}

		void GetVertex(void*& Data, size_t& Size)
		{
			Size = mResource_->mGeometry_->GetVertexData(Data);
		}
		
		void GetIndex(void*& Data, size_t& Size)
		{
			Size = mResource_->mGeometry_->GetIndexData(Data);
		}
		
		size_t GetVertexStride()
		{
			return mResource_->mGeometry_->GetVertexStride();
		}

		size_t GetIndexStride()
		{
			return mResource_->mGeometry_->GetIndexStride();
		}

		void GetVertexDesc(List<IInputLayout::InputElementDesc>& Descs)
		{
			return mResource_->mGeometry_->GetVertexDesc(Descs);
		}

	protected:
		MeshResource* mResource_;
	};
}