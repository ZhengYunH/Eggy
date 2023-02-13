#include "Graphics/RHI/IRenderPass.h"

namespace Eggy
{
	class ImguiPass : public RenderPass
	{
		struct ImGuiData
		{
			static bool sUseCustomColor;

			Vector4 cCustomColor{ 1.f, 1.f, 1.f, 1.f };
			float cCustomScale{ 1.0f };
		};

	public:
		struct ImguiContext
		{
			ImguiContext();
		};
		static ImguiContext* Context;
	public:
		ImguiPass();
		RenderPass* Connect(RenderPass* input);
		void Compile(RenderGraphBuilder* builder) override;
		void Consolidate() override;
		void EncodeRenderPassEnd() override;

	protected:
		bool InitImGui();
		ImGuiData mData;
	};
}