#include "ImGuiRenderer.h"
#include "Core/Engine/Engine.h"
#include "Core/System/InputSystem.h"


// TODO need more suitable solution
#include<imgui.cpp>
#include<imgui_demo.cpp>
#include<imgui_draw.cpp>
#include<imgui_tables.cpp>
#include<imgui_widgets.cpp>
#if D3D11_DEVICE
#include<backends/imgui_impl_dx11.cpp>
#endif
#if defined(_WIN32)
#include<backends/imgui_impl_win32.cpp>
#endif

namespace Eggy
{
	ImguiPass::ImguiContext* ImguiPass::Context = nullptr;
	ImguiPass::ImguiPass() : RenderPass()
	{
		InitImGui();
	}

	RenderPass* ImguiPass::Connect(RenderPass* input)
	{
		AddInput(input);
		return this;
	}

	bool ImguiPass::ImGuiData::sUseCustomColor = false;
	void ImguiPass::Compile(RenderGraphBuilder* builder)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		// for test
		if (true)
		{
			/*ImGui::ShowAboutWindow();
			ImGui::ShowDemoWindow();
			ImGui::ShowUserGuide();*/
		}

		if (ImGui::Begin("Use ImGui"))
		{
			ImGui::SliderFloat("Scale", &mData.cCustomScale, 0.2f, 2.0f);
			ImGui::Checkbox("Use Custom Color", &ImGuiData::sUseCustomColor);
			if (ImGuiData::sUseCustomColor)
			{
				ImGui::ColorEdit4("Color", reinterpret_cast<float*>(&mData.cCustomColor));
			}
		}
		ImGui::End();

		auto& gConstant = Pipeline->GetGlobalConstant();
		gConstant.Color = ImGuiData::sUseCustomColor ? mData.cCustomColor : Vector4(1.f, 1.f, 1.f, 1.f);

	}

	void ImguiPass::Consolidate()
	{
		RenderPass::Consolidate();
	}

	void ImguiPass::EncodeRenderPassEnd()
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	bool ImguiPass::InitImGui()
	{
		if (!Context)
		{
			Context = new ImguiPass::ImguiContext();
		}
		return true;
	}

	ImguiPass::ImguiContext::ImguiContext()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigWindowsMoveFromTitleBarOnly = true;

		// Style
		ImGui::StyleColorsDark();

		// Platform
		ImGui_ImplWin32_Init(Engine::Get()->Platform->GetWindow());
		ImGui_ImplDX11_Init((ID3D11Device*)GRenderDevice->GetDevice(), (ID3D11DeviceContext*)GRenderDevice->GetContext());

		InputSystem::Get()->MessageEvent.Bind([&](UINT msg, WPARAM wparam, LPARAM lparam) -> bool {
			ImGui_ImplWin32_WndProcHandler(Engine::Get()->Platform->GetWindow(), msg, wparam, lparam);
		if (IS_MOUSE_EVENT(msg) && io.WantCaptureMouse)
			return true;
		if (IS_KEY_EVENT(msg) && io.WantCaptureKeyboard)
			return true;
		return false;
			});
	}
}