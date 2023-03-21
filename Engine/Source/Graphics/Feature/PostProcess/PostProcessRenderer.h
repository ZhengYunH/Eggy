#pragma once
#include "Graphics/RHI/IRenderPass.h"
#include "Graphics/Elements/RenderElement.h"
#include "Resource/MeshResource.h"
#include "Object/Material.h"
#include "Object/Shader.h"


namespace Eggy
{
	class PostprocessElement : public RenderElement<EVF_P3F_C4B_T2F>
	{
	public:
		PostprocessElement() : RenderElement<EVF_P3F_C4B_T2F>()
		{
			vertexInfo.Count = QuatMesh::ConstMesh.GetVertexData(vertexInfo.Data);
			vertexInfo.Stride = QuatMesh::ConstMesh.GetVertexStride();

			indexInfo.Count = QuatMesh::ConstMesh.GetIndexData(indexInfo.Data);
			indexInfo.Stride = QuatMesh::ConstMesh.GetIndexStride();

			Initialize();
		}
	};

	class Postprocess : public RenderPass
	{
	public:
		Postprocess(String shaderPath) : RenderPass()
		{
			mInfo_.Object = new RenderObject();
			auto shader = new Shader(shaderPath);
			mInfo_.Material_ = new Material(shader);
			mInfo_.Object->ModelTransform.SetIdentity();
			mInfo_.ShadingState_ = new IShadingState(shader);
		}
		virtual void Consolidate() override;
		void Render(RenderContext* context) override;
		ShadingParameterCollection* GetParameters()
		{
			return mInfo_.Material_->GetParameters();
		}

	protected:
		PostprocessElement mRenderElement;
		RenderItemInfo mInfo_;
	};

	class BlurPostprocess : public Postprocess
	{
	public:
		BlurPostprocess() : Postprocess(StaticName(PostProcess/SimpleBlur))
		{

		}

		RenderPass* Connect(RenderPass* input);
		void Compile(RenderGraphBuilder* builder) override;
	};

	class TextureToScreen : public Postprocess
	{
	public:
		TextureToScreen() : Postprocess(StaticName(PostProcess/TextureToScreen))
		{

		}
		RenderPass* Connect(RenderPass* input);
		void Compile(RenderGraphBuilder* builder) override;
	};

}