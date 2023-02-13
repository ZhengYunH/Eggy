#include "Graphics/RHI/IRenderPass.h"
#include "Graphics/Elements/RenderElement.h"
#include "Core/Engine/Resource/Mesh.h"


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
		}
	};

	class Postprocess : public RenderPass
	{
	public:
		Postprocess(String shaderPath) : RenderPass()
		{
			mMaterialResource_.mShader_ = shaderPath;
			mMaterial_.SetResource(&mMaterialResource_);
			mRenderElement.Initialize(&mMaterial_);
			mInfo_.Object = new RenderObject();
			mInfo_.Object->ObjectConstantData_.ModelTransform.SetIdentity();
		}
		virtual void Consolidate() override;
		void Render(RenderContext* context) override;

	protected:
		PostprocessElement mRenderElement;
		RenderItemInfo mInfo_;
		Material mMaterial_;
		MaterialResource mMaterialResource_;
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