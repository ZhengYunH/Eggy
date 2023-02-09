#include "Graphics/RHI/IRenderPipeline.h"
#include "Graphics/RHI/IRenderPass.h"


namespace Eggy
{
	class Denotator : public RenderPass
	{
	public:
		Denotator() = default;
	};

	class ScenePass : public RenderPass
	{
	public:
		ScenePass() {}
		RenderPass* Connect(RenderPass *input);
		void Compile() override;
	};

	class Postprocess : public RenderPass
	{
	public:
		Postprocess(String shaderPath) : mShaderPath(shaderPath)
		{
		}
		virtual void Consolidate() override;

	protected:
		String mShaderPath;
	};

	class BlurPostprocess : public Postprocess
	{
	public:
		BlurPostprocess() : Postprocess(StaticName("PostProcess/Blur"))
		{

		}

		RenderPass* Connect(RenderPass* input);
	};

	class ForwardPipeline : public RenderPipeline
	{
	public:
		RenderPass* Setup() override;
		void ResolveConnection(RenderPass* output) override;

	protected:
		Denotator denotator;
		ScenePass scenePass;
		BlurPostprocess blurPostprocess;
	};
}