#include "main.h"
#include "MainUI.h"
#include "SaveEditorUI.h"
#include "PopupDialog.h"
#include "AboutWindow.h"
#include <fstream>
#define JSON_USE_IMPLICIT_CONVERSIONS 0
#include <nlohmann/json.hpp>
using json = nlohmann::json;

MainUI::MainUI() : BaseUI(nullptr)
{
	saveEditor = new SaveEditorUI(this);
	popupDialog = new PopupDialog(this);
	aboutWindow = new AboutWindow(this);

	fileDialogIsSave = false;

	currentPath.clear();
	currentFilePath.clear();

	windowOpacity = 0.9f;

	LoadConfig();
}

MainUI::~MainUI()
{
	SaveConfig();

	delete saveEditor;
	saveEditor = nullptr;

	delete popupDialog;
	popupDialog = nullptr;

	delete aboutWindow;
	aboutWindow = nullptr;
}

void MainUI::VisibilityChanged(const bool isVisible)
{
	BaseUI::VisibilityChanged(isVisible);
}

void MainUI::DoRender()
{
	BaseUI::DoRender();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open..."))
			{
				fileDialogIsSave = false;
				fileDialog = ImGui::FileBrowser(0);
				if (std::filesystem::exists(currentPath)) fileDialog.SetPwd(currentPath);
				fileDialog.SetTitle("Open a Perfect Dark save file");
				fileDialog.SetTypeFilters({ ".eep", ".bin", ".*" });
				fileDialog.Open();
			}

			if (ImGui::MenuItem("Save", NULL, false, saveData.IsSaveFileLoaded()))
			{
				Save(currentFilePath);
			}

			if (ImGui::MenuItem("Save As...", NULL, false, saveData.IsSaveFileLoaded()))
			{
				fileDialogIsSave = true;
				fileDialog = ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename);
				if (std::filesystem::exists(currentPath)) fileDialog.SetPwd(currentPath);
				fileDialog.SetTitle("Save the Perfect Dark save file");
				fileDialog.SetTypeFilters({ saveData.GetFormat() == SaveFormats::PC ? ".bin" : ".eep", ".*" });
				fileDialog.Open();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Quit"))
			{
				CloseMainWindow();
			}

			ImGui::EndMenu();
		}

		if (saveData.IsSaveFileLoaded() && ImGui::BeginMenu("Tools"))
		{
			int format = (int)saveData.GetFormat() - 1;
			if (ImGui::Combo("Save Format", &format, saveFormatNames, NUM_SAVE_FORMATS))
			{
				saveData.SetFormat((SaveFormats)(format + 1));
			}

			uint8_t gameFileCount = saveData.GetSaveFile()->GetGameFileCount();
			uint8_t mpSetupCount = saveData.GetSaveFile()->GetMultiplayerSetupCount();
			uint8_t mpProfileCount = saveData.GetSaveFile()->GetMultiplayerProfileCount();

			ImGui::SeparatorText("Copy");

			if (gameFileCount == 0 || gameFileCount >= NUM_FILE_SLOTS) ImGui::BeginDisabled();
			if (ImGui::BeginMenu("Single Player Agent File##CopyGameFile"))
			{
				uint8_t file = 0;

				for (uint8_t f = 0; f < ACTUAL_NUM_FILE_SLOTS; f++)
				{
					GameFile* gameFile = saveData.GetSaveFile()->GetGameFile(f);
					if (!gameFile->IsUsed()) continue;

					char menuName[32];
					snprintf(menuName, 32, "File %u (%s)", file + 1, gameFile->name);

					if (ImGui::MenuItem(menuName))
					{
						CopyGameFile(gameFile);
					}

					file++;
				}

				ImGui::EndMenu();
			}
			if (gameFileCount == 0 || gameFileCount >= NUM_FILE_SLOTS) ImGui::EndDisabled();

			if (mpSetupCount == 0 || mpSetupCount >= NUM_FILE_SLOTS) ImGui::BeginDisabled();
			if (ImGui::BeginMenu("Combat Simulator Settings File##CopyMpSetup"))
			{
				uint8_t file = 0;

				for (uint8_t f = 0; f < ACTUAL_NUM_FILE_SLOTS; f++)
				{
					MultiplayerSetup* mpSetup = saveData.GetSaveFile()->GetMultiplayerSetup(f);
					if (!mpSetup->IsUsed()) continue;

					char menuName[32];
					snprintf(menuName, 32, "File %u (%s)", file + 1, mpSetup->name);

					if (ImGui::MenuItem(menuName))
					{
						CopyMultiplayerSetup(mpSetup);
					}

					file++;
				}

				ImGui::EndMenu();
			}
			if (mpSetupCount == 0 || mpSetupCount >= NUM_FILE_SLOTS) ImGui::EndDisabled();

			if (mpProfileCount == 0 || mpProfileCount >= NUM_FILE_SLOTS) ImGui::BeginDisabled();
			if (ImGui::BeginMenu("Combat Simulator Player File##CopyMpProfile"))
			{
				uint8_t file = 0;

				for (uint8_t f = 0; f < ACTUAL_NUM_FILE_SLOTS; f++)
				{
					MultiplayerProfile* mpProfile = saveData.GetSaveFile()->GetMultiplayerProfile(f);
					if (!mpProfile->IsUsed()) continue;

					char menuName[32];
					snprintf(menuName, 32, "File %u (%s)", file + 1, mpProfile->name);

					if (ImGui::MenuItem(menuName))
					{
						CopyMultiplayerProfile(mpProfile);
					}

					file++;
				}

				ImGui::EndMenu();
			}
			if (mpProfileCount == 0 || mpProfileCount >= NUM_FILE_SLOTS) ImGui::EndDisabled();

			ImGui::SeparatorText("Delete");

			if (gameFileCount == 0) ImGui::BeginDisabled();
			if (ImGui::BeginMenu("Single Player Agent File##DeleteGameFile"))
			{
				uint8_t file = 0;

				for (uint8_t f = 0; f < ACTUAL_NUM_FILE_SLOTS; f++)
				{
					GameFile* gameFile = saveData.GetSaveFile()->GetGameFile(f);
					if (!gameFile->IsUsed()) continue;

					char menuName[32];
					snprintf(menuName, 32, "File %u (%s)", file + 1, gameFile->name);

					if (ImGui::MenuItem(menuName))
					{
						DeleteGameFile(gameFile);
					}

					file++;
				}

				ImGui::EndMenu();
			}
			if (gameFileCount == 0) ImGui::EndDisabled();

			if (mpSetupCount == 0) ImGui::BeginDisabled();
			if (ImGui::BeginMenu("Combat Simulator Settings File##DeleteMpSetup"))
			{
				uint8_t file = 0;

				for (uint8_t f = 0; f < ACTUAL_NUM_FILE_SLOTS; f++)
				{
					MultiplayerSetup* mpSetup = saveData.GetSaveFile()->GetMultiplayerSetup(f);
					if (!mpSetup->IsUsed()) continue;

					char menuName[32];
					snprintf(menuName, 32, "File %u (%s)", file + 1, mpSetup->name);

					if (ImGui::MenuItem(menuName))
					{
						DeleteMultiplayerSetup(mpSetup);
					}

					file++;
				}

				ImGui::EndMenu();
			}
			if (mpSetupCount == 0) ImGui::EndDisabled();

			if (mpProfileCount == 0) ImGui::BeginDisabled();
			if (ImGui::BeginMenu("Combat Simulator Player File##DeleteMpProfile"))
			{
				uint8_t file = 0;

				for (uint8_t f = 0; f < ACTUAL_NUM_FILE_SLOTS; f++)
				{
					MultiplayerProfile* mpProfile = saveData.GetSaveFile()->GetMultiplayerProfile(f);
					if (!mpProfile->IsUsed()) continue;

					char menuName[32];
					snprintf(menuName, 32, "File %u (%s)", file + 1, mpProfile->name);

					if (ImGui::MenuItem(menuName))
					{
						DeleteMultiplayerProfile(mpProfile);
					}

					file++;
				}

				ImGui::EndMenu();
			}
			if (mpProfileCount == 0) ImGui::EndDisabled();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Settings"))
		{
			ImGui::SliderFloat("Window Opacity", &windowOpacity, 0.0f, 1.0f);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About...", NULL, aboutWindow->GetIsVisible()))
			{
				aboutWindow->ToggleIsVisible();
			}

			ImGui::EndMenu();
		}

		if (saveData.IsSaveFileLoaded())
		{
			std::string fileText = std::string("Current file: ") + currentFilePath.filename().u8string();

			ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize(fileText.c_str()).x - 32);
			ImGui::Text("%s", fileText.c_str());
		}

		ImGui::EndMainMenuBar();
	}

	//ImGui::ShowDemoWindow();

	saveEditor->Render();
	popupDialog->Render();
	aboutWindow->Render();

	fileDialog.Display();

	if (fileDialog.HasSelected())
	{
		currentFilePath = fileDialog.GetSelected();
		currentPath = fileDialog.GetPwd();

		if (fileDialogIsSave) Save(currentFilePath);
		else Load(currentFilePath);

		SaveConfig();
		fileDialog.ClearSelected();
	}
}

void MainUI::LoadConfig()
{
	try
	{
		std::ifstream stream(CONFIG_FILE_NAME);

		if (!stream || !stream.is_open())
		{
			return;
		}

		json config;
		stream >> config;
		stream.close();

		if (config["lastPath"].is_string()) currentPath = std::filesystem::u8path(config["lastPath"].template get<std::string>());
		if (config["windowOpacity"].is_number_float()) windowOpacity = config["windowOpacity"].template get<float>();
	}
	catch (const json::parse_error& error)
	{
		printf("Json Error: %s\n", error.what());
	}
}

void MainUI::SaveConfig() const
{
	try
	{
		json config;
		config["lastPath"] = currentPath.u8string();
		config["windowOpacity"] = windowOpacity;

		// The setw manipulator was overloaded to set the indentation for pretty printing.

		std::ofstream stream(CONFIG_FILE_NAME);
		stream << std::setw(4) << config << std::endl;
		stream.close();
	}
	catch (const json::type_error& error)
	{
		printf("Json Error: %s\n", error.what());
	}
}

void MainUI::Load(std::filesystem::path filePath)
{
	try
	{
		saveData.Load(filePath.string());

		LoadingProcess();

		saveEditor->SetIsVisible(true);
	}
	catch (const std::runtime_error& error)
	{
		popupDialog->SetMessage(MessageTypes::Error, "Error", error.what());
		popupDialog->SetIsVisible(true);
	}
}

void MainUI::LoadingProcess() const
{
	if (!saveData.IsSaveFileLoaded()) return;

	std::string message;

	for (uint8_t f = 0; f < ACTUAL_NUM_BOSS_FILE_SLOTS; f++)
	{
		BossFile* bossFile = saveData.GetSaveFile()->GetBossFile(f);
		if (!bossFile->IsUsed()) continue;

		if (!bossFile->IsChecksumValid())
		{
			message += "Global data is corrupted. Data might be completely wrong.\n";
		}
	}

	uint8_t file = 0;
	if (!message.empty()) message += "\n";

	for (uint8_t f = 0; f < ACTUAL_NUM_FILE_SLOTS; f++)
	{
		GameFile* gameFile = saveData.GetSaveFile()->GetGameFile(f);
		if (!gameFile->IsUsed()) continue;

		if (!gameFile->IsChecksumValid())
		{
			message += std::string("Game file ") + std::to_string(file++) + " is corrupted. Data might be completely wrong.\n";
		}
	}

	file = 0;
	if (!message.empty()) message += "\n";

	for (uint8_t f = 0; f < ACTUAL_NUM_FILE_SLOTS; f++)
	{
		MultiplayerProfile* mpProfile = saveData.GetSaveFile()->GetMultiplayerProfile(f);
		if (!mpProfile->IsUsed()) continue;

		if (!mpProfile->IsChecksumValid())
		{
			message += std::string("Multiplayer profile ") + std::to_string(file++) + " is corrupted. Data might be completely wrong.\n";
		}
	}

	file = 0;
	if (!message.empty()) message += "\n";

	for (uint8_t f = 0; f < ACTUAL_NUM_FILE_SLOTS; f++)
	{
		MultiplayerSetup* mpSetup = saveData.GetSaveFile()->GetMultiplayerSetup(f);
		if (!mpSetup->IsUsed()) continue;

		if (!mpSetup->IsChecksumValid())
		{
			message += std::string("Multiplayer settings ") + std::to_string(file++) + " is corrupted. Data might be completely wrong.\n";
		}
	}

	if (!message.empty())
	{
		popupDialog->SetMessage(MessageTypes::Warning, "Load warnings", message);
		popupDialog->SetIsVisible(true);
	}
}

void MainUI::Save(std::filesystem::path filePath)
{
	if (!saveData.IsSaveFileLoaded()) return;

	try
	{
		saveData.Save(filePath.string());
	}
	catch (const std::runtime_error& error)
	{
		popupDialog->SetMessage(MessageTypes::Error, "Error", error.what());
		popupDialog->SetIsVisible(true);
	}
}

void MainUI::CopyGameFile(const GameFile* srcGameFile) const
{
	uint8_t file = 0;
	GameFile* dstGameFile = saveData.GetSaveFile()->GetFirstUnusedGameFile(&file);

	if (dstGameFile == nullptr)
	{
		popupDialog->SetMessage(MessageTypes::Error, "Error", "Couldn't find any unused GameFile.");
		popupDialog->SetIsVisible(true);
		return;
	}

	uint32_t deviceSerial = dstGameFile->pakFileHeader.deviceSerial;
	uint32_t id = dstGameFile->pakFileHeader.id;

	memcpy(dstGameFile, srcGameFile, sizeof(GameFile));

	dstGameFile->pakFileHeader.deviceSerial = deviceSerial;
	dstGameFile->pakFileHeader.id = id;
	snprintf(dstGameFile->name, MAX_NAME_LENGTH + 1, "New File %u", file & 0x7);
}

void MainUI::CopyMultiplayerProfile(const MultiplayerProfile* srcMpProfile) const
{
	uint8_t file = 0;
	MultiplayerProfile* dstMpProfile = saveData.GetSaveFile()->GetFirstUnusedMultiplayerProfile(&file);

	if (dstMpProfile == nullptr)
	{
		popupDialog->SetMessage(MessageTypes::Error, "Error", "Couldn't find any unused MultiplayerProfile.");
		popupDialog->SetIsVisible(true);
		return;
	}

	uint32_t deviceSerial = dstMpProfile->pakFileHeader.deviceSerial;
	uint32_t id = dstMpProfile->pakFileHeader.id;

	memcpy(dstMpProfile, srcMpProfile, sizeof(MultiplayerProfile));

	dstMpProfile->pakFileHeader.deviceSerial = deviceSerial;
	dstMpProfile->pakFileHeader.id = id;
	snprintf(dstMpProfile->name, MAX_NAME_LENGTH + 1, "New File %u", file & 0x7);
}

void MainUI::CopyMultiplayerSetup(const MultiplayerSetup* srcMpSetup) const
{
	uint8_t file = 0;
	MultiplayerSetup* dstMpSetup = saveData.GetSaveFile()->GetFirstUnusedMultiplayerSetup(&file);

	if (dstMpSetup == nullptr)
	{
		popupDialog->SetMessage(MessageTypes::Error, "Error", "Couldn't find any unused MultiplayerSetup.");
		popupDialog->SetIsVisible(true);
		return;
	}

	uint32_t deviceSerial = dstMpSetup->pakFileHeader.deviceSerial;
	uint32_t id = dstMpSetup->pakFileHeader.id;

	memcpy(dstMpSetup, srcMpSetup, sizeof(MultiplayerSetup));

	dstMpSetup->pakFileHeader.deviceSerial = deviceSerial;
	dstMpSetup->pakFileHeader.id = id;
	snprintf(dstMpSetup->name, MAX_NAME_LENGTH + 1, "New File %u", file & 0x7);
}

void MainUI::DeleteGameFile(GameFile* gameFile) const
{
	gameFile->pakFileHeader.occupied = 0;
}

void MainUI::DeleteMultiplayerProfile(MultiplayerProfile* mpProfile) const
{
	mpProfile->pakFileHeader.occupied = 0;
}

void MainUI::DeleteMultiplayerSetup(MultiplayerSetup* mpSetup) const
{
	mpSetup->pakFileHeader.occupied = 0;
}