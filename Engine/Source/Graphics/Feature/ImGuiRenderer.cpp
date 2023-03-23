#include "ImGuiRenderer.h"
#include "Core/Engine/Engine.h"
#include "Core/System/InputSystem.h"
#include "Core/System/UISystem.h"


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
	class ImGuiTranslator : public IUITranslator
	{
	public:
		void Panel(UIPanel* panel) override
		{
			ImGui::Begin(panel->GetTitle().c_str());
			auto children = panel->GetChildren();
			for (auto& pair : children)
			{
				pair.second->Translate(this);
			}
			ImGui::End();
		}


		void FloatSlider(FloatSliderWidget* widget) override
		{
			bool readOnly = widget->ReadOnly();
			uint8 memberSize = widget->MemberSize();
			float* pMin = readOnly ? nullptr : &widget->Min();
			float* pMax = readOnly ? nullptr : &widget->Max();

			ImGui::BeginDisabled(readOnly);
			{
				ImGui::SliderScalarN(widget->GetKey().c_str(), ImGuiDataType_Float, widget->GetData(), memberSize, pMin, pMax, "%.2f");
			}
			ImGui::EndDisabled();
		}
	};


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
		if (false)
		{
			ImGui::ShowAboutWindow();
			ImGui::ShowDemoWindow();
			ImGui::ShowUserGuide();
		}

		auto& gConstant = Pipeline->GetContext()->GetGlobalConstant();
		if (ImGui::Begin("Use ImGui"))
		{
			ImGui::SliderFloat("Scale", &mData.cCustomScale, 0.2f, 2.0f);
			ImGui::Checkbox("Use Custom Color", &ImGuiData::sUseCustomColor);
			ImGui::SliderFloat("Brightness", &gConstant.Brightness, 0.f, 2.f);
			if (ImGuiData::sUseCustomColor)
			{
				ImGui::ColorEdit4("Color", reinterpret_cast<float*>(&mData.cCustomColor));
			}
		}
		ImGui::End();
		
		// From UISystem
		{
			auto panels = UISystem::Get()->GetAllPanels();
			for (auto panel : panels)
			{
				ImGuiTranslator translator;
				panel->Translate(&translator);
			}
		}


		gConstant.Color = ImGuiData::sUseCustomColor ? mData.cCustomColor : Vector4(1.f, 1.f, 1.f, 1.f);

		RenderPass* inputPass = GetInput(0);
		SetOutput(builder, inputPass->GetOutput(0));
		SetDepthStencil(builder, inputPass->GetDepthStencil());
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