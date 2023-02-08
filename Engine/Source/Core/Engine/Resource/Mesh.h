#pragma once
#include "Core/Config.h"
#include "ResourceObject.h"


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

		MeshData() = default;
		~MeshData()
		{
			SafeDestroyArray(Vertex);
			SafeDestroyArray(Index);
		}

		void SetupData(List<VertexType>& vertex, List<IndexType>& index)
		{
			nVertex = static_cast<uint32>(vertex.size());
			Vertex = VertexFactory::AllocateVertex<Format>(nVertex);
			memcpy(Vertex, vertex.data(), nVertex * sizeof(VertexType));

			nIndex = static_cast<uint32>(index.size());
			Index = new IndexType[nIndex];
			memcpy(Index, index.data(), nIndex * sizeof(IndexType));
		}

		void SetupData(List<VertexType>&& vertex, List<IndexType>&& index)
		{
			SetupData(vertex, index);
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

	template<EVertexFormat Format>
	struct CubeMesh : public MeshData<Format>
	{
		using Parent = MeshData<Format>;
		CubeMesh() : Parent()
		{
			CreateMeshData();
		}

		void CreateMeshData()
		{
		}
	};

	template<>
	struct CubeMesh<EVF_P3F_C4B_T2F> : public MeshData<EVF_P3F_C4B_T2F>
	{
		using Parent = MeshData<EVF_P3F_C4B_T2F>;
		CubeMesh() : Parent()
		{
			CreateMeshData();
		}

		void CreateMeshData()
		{
			SetupData(
				List<VertexType>({
						{ Vector3(-1.0f, -1.0f, -1.0f), Color4B(0.0f, 0.0f, 0.0f, 1.0f),  Vector2(0.0f, 0.0f)},
						{ Vector3(-1.0f, 1.0f, -1.0f), Color4B(1.0f, 0.0f, 0.0f, 1.0f),  Vector2(0.0f, 1.0f) },
						{ Vector3(1.0f, 1.0f, -1.0f), Color4B(1.0f, 1.0f, 0.0f, 1.0f),  Vector2(1.0f, 1.0f) },
						{ Vector3(1.0f, -1.0f, -1.0f), Color4B(0.0f, 1.0f, 0.0f, 1.0f),  Vector2(1.0f, 0.0f) },
						{ Vector3(-1.0f, -1.0f, 1.0f), Color4B(0.0f, 0.0f, 1.0f, 1.0f),  Vector2(0.0f, 0.0f) },
						{ Vector3(-1.0f, 1.0f, 1.0f), Color4B(1.0f, 0.0f, 1.0f, 1.0f),  Vector2(0.0f, 0.0f) },
						{ Vector3(1.0f, 1.0f, 1.0f), Color4B(1.0f, 1.0f, 1.0f, 1.0f),  Vector2(0.0f, 0.0f) },
						{ Vector3(1.0f, -1.0f, 1.0f), Color4B(0.0f, 1.0f, 1.0f, 1.0f),  Vector2(0.0f, 0.0f) }
					}),
				List<IndexType>({
						0, 1, 2, 2, 3, 0,	// Front
						4, 5, 1, 1, 0, 4,	// Left
						1, 5, 6, 6, 2, 1,	// Top
						7, 6, 5, 5, 4, 7,	// Back
						3, 2, 6, 6, 7, 3,	// Right
						4, 0, 3, 3, 7, 4	// Bottom
					})
			);
		}
	};

	template<EVertexFormat Format>
	struct PyramidMesh : public MeshData<Format>
	{
		using Parent = MeshData<Format>;
		PyramidMesh() : Parent()
		{
			CreateMeshData();
		}

		void CreateMeshData()
		{
		}
	};

	template<>
	struct PyramidMesh<EVF_P3F_C4B> : public MeshData<EVF_P3F_C4B>
	{
		using Parent = MeshData<EVF_P3F_C4B>;
		PyramidMesh() : Parent()
		{
			CreateMeshData();
		}

		void CreateMeshData()
		{
			SetupData(
				List<VertexType>({
						{ Vector3(0.f, 2.f, 0.f), Color4B_BLACK },
						{ Vector3(1.f, 0.f, 1.f), Color4B_WRITE },
						{ Vector3(1.f, 0.f, -1.f), Color4B_RED },
						{ Vector3(-1.f, 0.f, -1.f), Color4B_GREEN },
						{ Vector3(-1.f, 0.f, 1.f), Color4B_BLUE },
					}),
					List<IndexType>({
							0, 1, 2,
							0, 2, 3,
							0, 3, 4,
							0, 4, 1,
							1, 2, 3,
							1, 3, 4
						})
			);
		}
	};

	class MeshResource : public ResourceObject
	{
	public:
		void Deserialize(IFile* file) noexcept override;
		List<IMeshData*>& GetGeometrys() { return mGeometrys_; }
		void SetGeometrys(List<IMeshData*>&& mesh) { mGeometrys_.swap(mesh); }
		void SetGeometry(IMeshData*&& mesh) { mGeometrys_.push_back(mesh); }

	protected:
		List<IMeshData*> mGeometrys_;
	};
}

