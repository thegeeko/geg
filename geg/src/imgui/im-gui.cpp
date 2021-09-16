#include "im-gui.hpp"

namespace Geg {
	ImGuiLayer::ImGuiLayer() : debugName("ImGui Layer") {}

	void ImGuiLayer::onAttach() {
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags = ImGuiBackendFlags_HasMouseCursors | ImGuiBackendFlags_HasSetMousePos;

		io.KeyMap[ImGuiKey_Tab] 				= GEG_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] 	= GEG_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] 	= GEG_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] 		= GEG_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] 	= GEG_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] 			= GEG_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] 		= GEG_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] 				= GEG_KEY_HOME;
		io.KeyMap[ImGuiKey_End] 				= GEG_KEY_END;
		io.KeyMap[ImGuiKey_Insert] 			= GEG_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] 			= GEG_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] 	= GEG_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] 			= GEG_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] 			= GEG_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] 			= GEG_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_KeyPadEnter] = GEG_KEY_KP_ENTER;
		io.KeyMap[ImGuiKey_A] 					= GEG_KEY_A;
		io.KeyMap[ImGuiKey_C] 					= GEG_KEY_C;
		io.KeyMap[ImGuiKey_V] 					= GEG_KEY_V;
		io.KeyMap[ImGuiKey_X] 					= GEG_KEY_X;
		io.KeyMap[ImGuiKey_Y] 					= GEG_KEY_Y;
		io.KeyMap[ImGuiKey_Z] 					= GEG_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 450");
		GEG_TRACE("Im gui attached");
	}

	void ImGuiLayer::onDetach() {
		Layer::onDetach();
	}

	void ImGuiLayer::onUpdate() {
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2{static_cast<float>(App::get().getWindow().getWidth()), static_cast<float>(App::get().getWindow().getHeight())};

		float time = (float)glfwGetTime();
		io.DeltaTime = i_Time > 0.0f ? (time - i_Time) : (1.0f / 60.0f);
		i_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		float a = 0;
		{
			ImGui::ShowAboutWindow();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::onEvent(Event &event) {
		Dispatcher dispatcher(event);
		dispatcher.dispatch<MouseButtonPressedEvent>(GEG_BIND_CB(ImGuiLayer::onMouseButtonPressedEvent));
		dispatcher.dispatch<MouseButtonReleasedEvent>(GEG_BIND_CB(ImGuiLayer::onMouseButtonReleasedEvent));
		dispatcher.dispatch<MouseMovedEvent>(GEG_BIND_CB(ImGuiLayer::onMouseMovedEvent));
		dispatcher.dispatch<MouseScrolledEvent>(GEG_BIND_CB(ImGuiLayer::onMouseScrolledEvent));
		dispatcher.dispatch<KeyPressedEvent>(GEG_BIND_CB(ImGuiLayer::onKeyPressedEvent));
		dispatcher.dispatch<KeyTappedEvent>(GEG_BIND_CB(ImGuiLayer::onKeyTappedEvent));
		dispatcher.dispatch<KeyReleasedEvent>(GEG_BIND_CB(ImGuiLayer::onKeyReleasedEvent));
		dispatcher.dispatch<WindowResizeEvent>(GEG_BIND_CB(ImGuiLayer::onWindowResizeEvent));
	}

	bool ImGuiLayer::onMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.getMouseButton()] = true;
		
		return false;
	}

	bool ImGuiLayer::onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.getMouseButton()] = false;

		return false;
	}

	bool ImGuiLayer::onMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.getX(), e.getY());

		return false;
	}

	bool ImGuiLayer::onMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.getXOffset();
		io.MouseWheel += e.getYOffset();

		return false;
	}

	bool ImGuiLayer::onKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.getKeyCode()] = true;

		io.KeyCtrl 	= io.KeysDown[GEG_KEY_LEFT_CONTROL] || io.KeysDown[GEG_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GEG_KEY_LEFT_SHIFT] 	|| io.KeysDown[GEG_KEY_RIGHT_SHIFT];
		io.KeyAlt 	= io.KeysDown[GEG_KEY_LEFT_ALT] 		|| io.KeysDown[GEG_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GEG_KEY_LEFT_SUPER] 	|| io.KeysDown[GEG_KEY_RIGHT_SUPER];
		return false;
	}

	bool ImGuiLayer::onKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.getKeyCode()] = false;

		return false;
 }

	bool ImGuiLayer::onKeyTappedEvent(KeyTappedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.getKeyCode();
		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter((unsigned short)keycode);

		return false;
	}

	bool ImGuiLayer::onWindowResizeEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.getW(), e.getH());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, (int)e.getW(), (int)e.getH());

		return false;
	}
}