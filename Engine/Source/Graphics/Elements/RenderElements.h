#pragma once
#include "Core/Config.h"
#include "Graphics/RHI/IRenderResource.h"
#include "Core/Interface/IRenderElements.h"
#include "Graphics/Renderer/VertexFactory.h"
#include "Graphics/RHI/IShader.h"


namespace Eggy
{
	class RenderElements : public IRenderElements
	{
	public:
		IGeometry Geometry;
		IShaderCollection ShaderCollection;
	};

	class RenderTriangleElement : public RenderElements
	{
	public:
		using VertexType = TVertexType<EVF_P3F_C4B>;
		RenderTriangleElement()
		{
			mVertexs_ = new VertexType[3]
			{
				{ Vector3(0.0f, 0.5f, 0.5f), Color4B(0.0f, 1.0f, 0.0f, 1.0f) },
				{ Vector3(0.5f, -0.5f, 0.5f), Color4B(0.0f, 0.0f, 1.0f, 1.0f) },
				{ Vector3(-0.5f, -0.5f, 0.5f), Color4B(1.0f, 0.0f, 0.0f, 1.0f) }
			};

			ShaderCollection.SetShaderPath(EShaderType::VS, "Engine/Shader/HLSL/Basic_VS.hlsl");
			ShaderCollection.SetShaderPath(EShaderType::PS, "Engine/Shader/HLSL/Basic_PS.hlsl");

			// fill Layout Desc
			VertexType::GetDesc(Geometry.Layout.Descs);

			// file VertexBuffer Desc
			GetVertexData(Geometry.VertexBuffer.Data, Geometry.VertexBuffer.Size, Geometry.VertexBuffer.Stride);
			Geometry.VertexBuffer.Usage = EBufferUsage::Immutable;
			Geometry.Layout.ShaderCollection = &ShaderCollection;
		}

		void CreateResource(IRenderResourceFactory* factory) override
		{
			ShaderCollection.CreateDeviceResource(factory);
			Geometry.CreateDeviceResource(factory);
		}

	public:
		void GetVertexData(void*& Data, size_t& DataSize, size_t& Stride) override
		{
			Data = (void*)mVertexs_;
			DataSize = 3;
			Stride = sizeof(VertexType);
		}

	protected:
		VertexType* mVertexs_;

	};

}