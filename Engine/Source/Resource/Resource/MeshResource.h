#pragma once
#include "Core/Config.h"
#include "Graphics/RHI/IRenderResource.h"
#include "Graphics/Renderer/VertexFactory.h"
#include "Resource/ResourceObject.h"


namespace Eggy
{
	struct IMeshData
	{
		virtual size_t GetVertexData(void*& Data) const = 0;
		virtual size_t GetIndexData(void*& Data) const = 0;

		virtual size_t GetVertexStride() const = 0;
		virtual size_t GetIndexStride() const = 0;

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

		virtual size_t GetVertexData(void*& Data) const override
		{
			HYBRID_CHECK(Vertex);

			Data = Vertex;
			return nVertex;
		}

		size_t GetIndexData(void*& Data) const override
		{
			HYBRID_CHECK(Index);

			Data = Index;
			return nIndex;
		}

		size_t GetVertexStride() const override
		{
			return VertexType::GetSize();
		}

		size_t GetIndexStride() const override
		{
			return sizeof(IndexType);
		}

		void GetVertexDesc(List<IInputLayout::InputElementDesc>& Descs)
		{
			return VertexType::GetDesc(Descs);
		}
	};

	struct QuatMesh : public MeshData<EVF_P3F_C4B_T2F>
	{
		using Parent = MeshData<EVF_P3F_C4B_T2F>;
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
						{ Vector3(0.0f, 0.0f, 0.0f), Color, Vector2(0, 0)},
						{ Vector3(0.0f, 1.0f, 0.0f), Color, Vector2(0, 1)},
						{ Vector3(1.0f, 0.0f, 0.0f), Color, Vector2(1, 0)},
						{ Vector3(1.0f, 1.0f, 0.0f), Color,  Vector2(1, 1) }
					}),
				List<IndexType>({
						0, 2, 1,
						1, 2, 3
					})
			);
		}

		Color4B Color{ Color4B_WRITE };

		static const QuatMesh ConstMesh;
	};

	enum class EHelperMesh
	{
		Quat,
		Cube,
		Pyramid,
		Sphere
	};

	class HelperMesh : public MeshData<EVF_P3F_C4B>
	{
	public:
		HelperMesh()
		{
			mColor_.u8[3] = 255 / 2;
		}

		void Consolidate()
		{
			List<VertexType> Vertex;
			List<IndexType> Index;
			PrepareMeshData(Vertex, Index);
			SetupData(Vertex, Index);
		}

		void SetColor(Color4B& color)
		{
			mColor_ = color;
		}

	protected:
		virtual void PrepareMeshData(List<VertexType>& Vertex, List<IndexType>& Index) = 0;
		
	protected:
		Color4B mColor_{ Color4B_WRITE };
	};

	template<EHelperMesh _T> static HelperMesh* GetHelperMesh() { return nullptr; }

#define DefineHelperMesh(EMesh, MeshClass) \
	template<> static HelperMesh* GetHelperMesh<EHelperMesh::EMesh>() \
	{\
		static MeshClass* G_HelperMesh = nullptr; \
		if(!G_HelperMesh) { G_HelperMesh = new MeshClass(); G_HelperMesh->Consolidate(); } \
		return G_HelperMesh; \
	}

	class QuatHelperMesh : public HelperMesh
	{
	protected:
		void PrepareMeshData(List<VertexType>& Vertex, List<IndexType>& Index) override;
	};
	DefineHelperMesh(Quat, QuatHelperMesh);


	class CubeHelperMesh : public HelperMesh
	{
	protected:
		void PrepareMeshData(List<VertexType>& Vertex, List<IndexType>& Index) override;
	};
	DefineHelperMesh(Cube, CubeHelperMesh);
	

	class PyramidHelperMesh : public HelperMesh
	{
	protected:
		void PrepareMeshData(List<VertexType>& Vertex, List<IndexType>& Index) override;

	};
	DefineHelperMesh(Pyramid, PyramidHelperMesh);


	class SphereHelperMesh : public HelperMesh
	{
	public:
		void SetResolution(size_t division) { mDivision_ = division; }

	protected:
		void PrepareMeshData(List<VertexType>& Vertex, List<IndexType>& Index) override;
		Vector3 _GetUnitSphereSurfacePoint(float u, float v)
		{
			return Vector3(Cos(u) * Sin(v), Cos(v), Sin(u) * Sin(v));
		}
	private:
		size_t mDivision_{ 24 };
		float mRadius_{ 1.f };
	};
	DefineHelperMesh(Sphere, SphereHelperMesh);


	class MeshResource : public ResourceObject
	{
		DeclareResource(MeshResource, EResourceType::Mesh);
	public:
		bool LoadObject() noexcept override;
		bool SaveObject() noexcept override;
		List<IMeshData*>& GetGeometrys() { return mGeometrys_; }
		void SetGeometrys(List<IMeshData*>&& mesh) { mGeometrys_.swap(mesh); }
		void SetGeometry(IMeshData*&& mesh) { mGeometrys_.push_back(mesh); }

	protected:
		List<IMeshData*> mGeometrys_;
	};
}

