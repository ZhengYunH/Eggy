#pragma once
#include "Core/Config.h"
#include "Graphics/RHI/IRenderResource.h"
#include "Core/Interface/IRenderElement.h"
#include "Graphics/Renderer/VertexFactory.h"
#include "Graphics/RHI/IRenderPipeline.h"
#include "Graphics/RHI/IShadingState.h"
#include "Graphics/RHI/RenderItem.h"

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

		virtual void GetVertexDesc(List<IInputLayout::InputElementDesc>& Descs) override
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
			if (!ShadingState)
			{
				ShadingState = new IShadingState(info->Material_->GetShader());
				auto technique = ShadingState->GetShaderCollection()->GetShaderTechnique(ETechnique::Shading);
				auto table = technique->GetConstantTable(EShaderConstant::Batch);
				if(table)
				{
					mParams_ = new ShadingParameterCollection(table);
					ShadingState->GetBatch()->SetConstantBuffer(EShaderConstant::Batch, mParams_);
				}
			}

			info->GeometryBinding_ = Geometry;
			info->ShadingState_ = ShadingState;
		}

		ShadingParameterCollection* GetParameters()
		{
			return mParams_;
		}

		VertexInfo vertexInfo;
		IndexInfo indexInfo;

		void* Data{ nullptr };
		size_t VertexCount{ 0 };
		size_t Stride{ 0 };
		size_t ByteWidth{ 0 };

		GeometryBinding* Geometry{ nullptr };
		IShadingState* ShadingState{ nullptr };
		ShadingParameterCollection* mParams_;
	};


	class RenderHelperElement : public RenderElement<EVF_P3F_C4B, uint16>
	{
	public:
		RenderHelperElement() = default;
	};

}