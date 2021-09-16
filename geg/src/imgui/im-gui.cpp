#include "im-gui.hpp"

// ---------- compiling imgui impls ------------//
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include "vendor/imgui/backends/imgui_impl_opengl3.cpp"
#include "vendor/imgui/backends/imgui_impl_glfw.cpp"
// ---------- compiling imgui impls ------------//

namespace Geg {
	ImGuiLayer::ImGuiLayer() : debugName("ImGui Layer") {}


	void ImGuiLayer::onAttach() {
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(App::get().getWindow().getRawWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 430");

		GEG_CORE_INFO("Im gui attached");
	}

	void ImGuiLayer::onDetach() {
		GEG_CORE_INFO("Detaching Imgui");
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::begin() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::end() {
		ImGuiIO& io = ImGui::GetIO();
		App& app = App::get();
		io.DisplaySize = ImVec2((float)app.getWindow().getWidth(), (float)app.getWindow().getHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::onUiUpdate() {
		static bool a = true;
		ImGui::ShowDemoWindow(&a);
	}


}