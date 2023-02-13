#include "Graphics/RHI/IRenderPass.h"

namespace Eggy
{
	class Denotator : public RenderPass
	{
	public:
		Denotator() = default;
		void Compile(RenderGraphBuilder* builder) override;
	};

	class ForwardScenePass : public RenderPass
	{
	public:
		ForwardScenePass() {}
		RenderPass* Connect(RenderPass* input);
		void Compile(RenderGraphBuilder* builder) override;
	};
}