#pragma once
#include "Core/Config.h"
#include "Graphics/RHI/IRenderResource.h"
#include "Core/Interface/IRenderElement.h"
#include "Graphics/Renderer/VertexFactory.h"
#include "Graphics/RHI/IRenderPipeline.h"
#include "Graphics/RHI/IShadingState.h"
#include "Core/DataType/WeakPtr.h"

#include "Core/Math/Matrix4x3.h"
#include "Core/Math/Matrix4x4.h"



namespace Eggy
{
	template<EVertexFormat TVertex, typename TIndex=uint32>
	class RenderElement : public IRenderElement
	{
	protected:
		using VertexType = TVertexType<TVertex>;
		using IndexType = TIndex;

	public:
		struct VertexInfo
		{
			void* Data{ nullptr };
			size_t Count{ 0 };
			size_t Stride{ 0 };
			
			VertexInfo() {}

			VertexInfo(void* data, size_t count, size_t stride)
			{
				Data = data;
				Count = count;
				Stride = stride;
			}

			constexpr size_t GetByteWidth()
			{
				return Count * Stride;
			}
		};

		struct IndexInfo
		{
			void* Data{ nullptr };
			size_t Count{ 0 };
			size_t Stride{ 0 };
			size_t ByteWidth{ 0 };

			IndexInfo() {}

			IndexInfo(void* data, size_t count, size_t stride)
			{
				Data = data;
				Count = count;
				Stride = stride;
			}

			constexpr size_t GetByteWidth()
			{
				return Count * Stride;
			}
		};

		RenderElement()
		{
			Geometry = new GeometryBinding();
			ShadingState = new IShadingState();
		}

		void GetVertexData(void*& outData, size_t& outVertexCount, size_t& outByteWidth) override
		{
			outData = vertexInfo.Data;
			outVertexCount = vertexInfo.Count;
			outByteWidth = vertexInfo.GetByteWidth();
		}

		void GetIndexData(void*& outData, size_t& outIndexCount, size_t& outByteWidth) override
		{
			outData = indexInfo.Data;
			outIndexCount = indexInfo.Count;
			outByteWidth = indexInfo.GetByteWidth();
		}

		void GetVertexDesc(List<IInputLayout::InputElementDesc>& Descs) override
		{
			VertexType::GetDesc(Descs);
		}
		
		void Initialize() override
		{
			GetVertexData(Geometry->VertexBuffer.Data, Geometry->VertexBuffer.Count, Geometry->VertexBuffer.ByteWidth);
			GetIndexData(Geometry->IndexBuffer.Data, Geometry->IndexBuffer.Count, Geometry->IndexBuffer.ByteWidth);
			GetVertexDesc(Geometry->Layout.Descs);
		}

		void PrepareRenderItemInfo(class RenderContext* context, class RenderItemInfo* info) override
		{
			info->GeometryBinding_ = Geometry;
			info->ShadingState_ = ShadingState;
		}

		VertexInfo vertexInfo;
		IndexInfo indexInfo;

		void* Data{ nullptr };
		size_t VertexCount{ 0 };
		size_t Stride{ 0 };
		size_t ByteWidth{ 0 };

		GeometryBinding* Geometry;
		IShadingState* ShadingState;
	};


	class RenderHelperElement : public RenderElement<EVF_P3F_C4B, uint16>
	{
	public:
		RenderHelperElement() = default;

	protected:
		virtual void FillInData() = 0;

	protected:
		List<VertexType> mVertexs_;
		List<IndexType> mIndexs_;
	};

	class RenderTriangleElement : public RenderHelperElement
	{
	protected:
		virtual void FillInData() override
		{
			mVertexs_ = List<VertexType>({
				{ Vector3(-0.5f, 0.0f, 0.0f), Color4B_GREEN },
				{ Vector3(0.0f, 0.5f, 0.0f), Color4B_BLUE },
				{ Vector3(0.5f, 0.0f, 0.0f), Color4B_RED }
			});

			mIndexs_ = List<IndexType>({0, 1, 2});
		}
	};

	class RenderCubeElement : public RenderHelperElement
	{
	protected:
		virtual void FillInData() override
		{
			mVertexs_ = List<VertexType>({
				{ Vector3(-1.0f, -1.0f, -1.0f), Color4B(0.0f, 0.0f, 0.0f, 1.0f) },
				{ Vector3(-1.0f, 1.0f, -1.0f), Color4B(1.0f, 0.0f, 0.0f, 1.0f) },
				{ Vector3(1.0f, 1.0f, -1.0f), Color4B(1.0f, 1.0f, 0.0f, 1.0f) },
				{ Vector3(1.0f, -1.0f, -1.0f), Color4B(0.0f, 1.0f, 0.0f, 1.0f) },
				{ Vector3(-1.0f, -1.0f, 1.0f), Color4B(0.0f, 0.0f, 1.0f, 1.0f) },
				{ Vector3(-1.0f, 1.0f, 1.0f), Color4B(1.0f, 0.0f, 1.0f, 1.0f) },
				{ Vector3(1.0f, 1.0f, 1.0f), Color4B(1.0f, 1.0f, 1.0f, 1.0f) },
				{ Vector3(1.0f, -1.0f, 1.0f), Color4B(0.0f, 1.0f, 1.0f, 1.0f) }
			});

			mIndexs_ = List<IndexType>({
				0, 1, 2, 2, 3, 0,	// Front
				4, 5, 1, 1, 0, 4,	// Left
				1, 5, 6, 6, 2, 1,	// Top
				7, 6, 5, 5, 4, 7,	// Back
				3, 2, 6, 6, 7, 3,	// Right
				4, 0, 3, 3, 7, 4	// Bottom
			});
		}
	};

	class RenderPyramidElement : public RenderHelperElement
	{
	protected:
		void FillInData() override
		{
			mVertexs_ = List<VertexType>({
				{ Vector3(0.f, 2.f, 0.f), Color4B_BLACK },
				{ Vector3(1.f, 0.f, 1.f), Color4B_WRITE },
				{ Vector3(1.f, 0.f, -1.f), Color4B_RED },
				{ Vector3(-1.f, 0.f, -1.f), Color4B_GREEN },
				{ Vector3(-1.f, 0.f, 1.f), Color4B_BLUE },
			});

			mIndexs_ = List<IndexType>({
				0, 1, 2,
				0, 2, 3,
				0, 3, 4,
				0, 4, 1,
				1, 2, 3,
				1, 3, 4
			});
		}
	};
}