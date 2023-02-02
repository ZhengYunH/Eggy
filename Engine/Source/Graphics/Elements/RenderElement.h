#pragma once
#include "Core/Config.h"
#include "Graphics/RHI/IRenderResource.h"
#include "Core/Interface/IRenderObject.h"
#include "Core/Interface/IRenderElement.h"
#include "Graphics/Renderer/VertexFactory.h"
#include "Graphics/RHI/IShader.h"


#include "Core/Math/Matrix4x3.h"
#include "Core/Math/Matrix4x4.h"

#include <DirectXMath.h>
#include <DirectXMathMatrix.inl>


namespace Eggy
{
	class RenderElement : public IRenderElement
	{
	public:
		void GetVertexData(void*& Data, size_t& VertexCount, size_t& ByteWidth) override
		{
			mMesh->GetVertex(Data, VertexCount);
			ByteWidth = VertexCount * mMesh->GetVertexStride();
		}

		void GetIndexData(void*& Data, size_t& IndexCount, size_t& ByteWidth) override
		{
			mMesh->GetIndex(Data, IndexCount);
			ByteWidth = IndexCount * mMesh->GetIndexStride();
		}

		void GetVertexDesc(List<IInputLayout::InputElementDesc>& Descs) override
		{
			mMesh->GetVertexDesc(Descs);
		}
	};

	struct RenderObject : public IRenderObject
	{
		GeometryBinding Geometry;
		IShaderCollection ShaderCollection;
		ObjectInfo ObjectConstantData;
		IConstantBuffer ConstantBuffer;	
		List<ITexture*> Textures;
		List<SamplerState*> Samplers;

		void CreateDeviceResource_Impl(IRenderResourceFactory* factory) override
		{
			ShaderCollection.CreateDeviceResource(factory);
			Geometry.CreateDeviceResource(factory);
			if (ConstantBuffer.Usage == EBufferUsage::Dynamic)
			{

			}
			ConstantBuffer.CreateDeviceResource(factory);
			for (ITexture* tex : Textures)
			{
				tex->CreateDeviceResource(factory);
			}
			for (SamplerState* state : Samplers)
			{
				state->CreateDeviceResource(factory);
			}
		}

		void Consolidate() override
		{
			Geometry.Layout.ShaderCollection = &ShaderCollection;
			ConstantBuffer.ByteWidth = sizeof(ObjectConstantData);
			ConsolidateResource();
		}

		void ConsolidateResource()
		{
			Samplers.resize(Textures.size());
			for (auto& sample : Samplers)
			{
				sample = new SamplerState();
			}
		}

		void SetTexture(String key, const TextureResource& resource)
		{
			Textures.push_back(new ITexture());
			auto& texture = *(Textures.back());
			texture.Data = resource.GetData();
			texture.Width = resource.Size.x;
			texture.Height = resource.Size.y;
			texture.Mips = resource.Mips;
			texture.Format = resource.Format;
			texture.ByteWidth = resource.ByteWidth;
			texture.TextureType = resource.TextureType;
		}

		~RenderObject()
		{
			for (ITexture* tex : Textures)
			{
				delete tex;
			}
			Textures.clear();

			for (SamplerState* sampler : Samplers)
			{
				delete sampler;
			}
			Samplers.clear();
		}
	};

	class RenderHelperElement : public RenderObject
	{
	public:
		using VertexType = TVertexType<EVF_P3F_C4B>;
		using IndexType = uint16;
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