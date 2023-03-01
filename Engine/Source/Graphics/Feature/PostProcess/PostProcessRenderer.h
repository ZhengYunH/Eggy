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
		}
	};

	class Postprocess : public RenderPass
	{
	public:
		Postprocess(String shaderPath) : RenderPass()
		{
			mRenderElement.Initialize();
			mInfo_.Object = new RenderObject();
			mInfo_.Material_ = new Material(new Shader(shaderPath));
			mInfo_.Object->ModelTransform.SetIdentity();
		}
		virtual void Consolidate() override;
		void Render(RenderContext* context) override;

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