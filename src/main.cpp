#if defined(_MSC_VER) && !defined(NDEBUG)
// Test leaks with _CrtDumpMemoryLeaks()
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <imgui/imgui.h>

#include "Config.h"
#include "MainUI.h"
#include "WindowSDL.h"

static int RunMain()
{
	const FileDialogFilter openDialogFilters[] =
	{
		{ "N64 Saves (*.eep; *.sav; *.bin)", "eep;sav;bin" },
		{ "All files (*.*)", "*" }
	};

	WindowParams windowParams = {};
	windowParams.title = std::string("Perfect Dark - Save Editor - v") + PROJECT_VER;
	windowParams.description = "This is a Perfect Dark cross-platform save editor.\nIt is able to edit everything of a save file from both the Nintendo 64 and PC\nversions of the game, and also convert between both formats.";
	windowParams.author = "PacoChan";
	windowParams.url = "https://pacochan.net/software/pd-save-editor/";
	windowParams.initialWidth = 800;
	windowParams.initialHeight = 654;
	windowParams.backgroundColor = ImVec4(0.0f, 0.05f, 0.1f, 1.0f);
	windowParams.openDialogTitle = "Open a Perfect Dark save file";
	windowParams.openDialogFiltersCount = sizeof(openDialogFilters) / sizeof(openDialogFilters[0]);
	windowParams.openDialogFilters = openDialogFilters;
	windowParams.configureStyleCallback = [](ImVec4* colors)
		{
			ImGui::StyleColorsDark();

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
			colors[ImGuiCol_TabSelected] = ImVec4(0.00f, 0.58f, 0.94f, 1.00f);
			colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.03f, 0.21f, 0.43f, 1.00f);
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
			colors[ImGuiCol_TextLink] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.64f);
		};
	windowParams.configureFontsCallback = [](ImFontAtlas* fontAtlas)
		{
			ImFontConfig configVector;
			configVector.MergeMode = false;
			configVector.OversampleH = 2;
			configVector.OversampleV = 2;
			configVector.RasterizerMultiply = 1.5f;

			fontAtlas->AddFontDefaultVector(&configVector);
		};

	try
	{
		WindowSDL window(windowParams);

		MainUI mainUi(&window);
		window.Run(mainUi);
	}
	catch (const std::runtime_error& error)
	{
		printf("%s\n", error.what());
		return -1;
	}

	return 0;
}

int main()
{
	// Run everything in RunMain so when the function exits, all the destructors,
	// including those of objects with automatic storage duration like std::string are called.
	// That will prevent _CrtDumpMemoryLeaks() from having some false positives.

	int result = RunMain();

#if defined(_MSC_VER) && !defined(NDEBUG)
	// Cause an intentional leak to check if the leak detector is working

	char* leakTest = new char[10];
	snprintf(leakTest, 10, "%s", "LEAK TEST");

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
#endif

	return result;
}
