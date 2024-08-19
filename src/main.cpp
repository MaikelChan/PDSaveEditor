#include "main.h"
#include "Config.h"
#include "MainUI.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

//#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

GLFWwindow* window = nullptr;

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void CloseMainWindow()
{
	if (window == nullptr) return;
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void SetImGuiStyle()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_FrameBg] = ImVec4(0.07f, 0.19f, 0.45f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.08f, 0.22f, 0.53f, 0.74f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.04f, 0.13f, 0.33f, 0.74f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.63f, 0.84f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.63f, 0.84f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 0.45f, 0.73f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.00f, 0.55f, 1.00f, 0.59f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.00f, 0.63f, 0.84f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.03f, 0.21f, 0.43f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.03f, 0.21f, 0.43f, 0.61f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.55f, 1.00f, 0.59f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.03f, 0.27f, 0.47f, 0.59f);
	colors[ImGuiCol_Separator] = ImVec4(0.00f, 0.95f, 1.00f, 0.25f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.55f, 1.00f, 0.59f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.00f, 0.63f, 0.84f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.00f, 0.58f, 0.94f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.03f, 0.21f, 0.43f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.55f, 1.00f, 0.59f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.55f, 1.00f, 0.59f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.00f, 0.55f, 1.00f, 0.59f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.04f, 0.12f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.00f, 0.63f, 0.84f, 0.43f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.04f, 0.12f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 0.63f, 0.84f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 0.77f, 0.84f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 0.45f, 0.73f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.00f, 0.04f, 0.12f, 0.94f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.69f);
}

int main()
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		return -1;
	}

	char title[64];
	snprintf(title, 64, "%s - v%s", WINDOW_TITLE, PROJECT_VER);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title, NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	if (monitor)
	{
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		if (mode)
		{
			glfwSetWindowPos(window, (mode->width >> 1) - (WINDOW_WIDTH >> 1), (mode->height >> 1) - (WINDOW_HEIGHT >> 1));
		}
	}

	//gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	// Imgui

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.IniFilename = NULL;
	io.Fonts->AddFontDefault();

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	SetImGuiStyle();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	bool show_demo_window = true;

	MainUI* mainUI = new MainUI();
	mainUI->SetIsVisible(true);

	while (!glfwWindowShouldClose(window))
	{
		/* Poll for and process events */
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		mainUI->Render();

		ImGui::Render();

		/* Render here */

		float windowOpacity = mainUI->GetWindowOpacity();
		glClearColor(0.0f * windowOpacity, 0.05f * windowOpacity, 0.1f * windowOpacity, windowOpacity);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/* Swap front and back buffers */

		glfwSwapBuffers(window);
	}

	delete mainUI;
	mainUI = nullptr;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}