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
	aboutWindow = new AboutWindow(this);
	popupDialog = new PopupDialog(this);

	currentFilePath.clear();
	currentFileName.clear();

	windowOpacity = 0.9f;

	fileDialog.SetTitle("Open a Perfect Dark save file");
	fileDialog.SetTypeFilters({ ".bin", ".*" });

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
				fileDialog.Open();
			}

			if (ImGui::MenuItem("Save", NULL, false, saveData.IsSaveFileLoaded()))
			{
				Save();
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
			for (uint8_t s = 0; s < ACTUAL_NUM_FILE_SLOTS; s++)
			{
				if (ImGui::BeginMenu(tabNames[s]))
				{
					if (ImGui::BeginMenu("Copy"))
					{
						for (uint8_t ds = 0; ds < ACTUAL_NUM_FILE_SLOTS; ds++)
						{
							if (s == ds) continue;

							char menuName[27];
							snprintf(menuName, 27, "To %s", tabNames[ds]);

							if (ImGui::MenuItem(menuName))
							{
								CopySlot(s, ds);
							}
						}

						ImGui::EndMenu();
					}

					if (ImGui::MenuItem("Delete"))
					{
						DeleteSlot(s);
					}

					ImGui::EndMenu();
				}
			}

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
			std::string fileText = std::string("Current file: ") + currentFileName;

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
		Load();
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

		if (config["lastPath"].is_string()) fileDialog.SetPwd(std::filesystem::u8path(config["lastPath"].template get<std::string>()));
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
		config["lastPath"] = fileDialog.GetPwd().u8string();
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

void MainUI::Load()
{
	try
	{
		saveData.Load(fileDialog.GetSelected().string());

		currentFilePath = fileDialog.GetSelected().string();
		currentFileName = fileDialog.GetSelected().filename().string();

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

		if (!bossFile->IsValid())
		{
			message += "Global data is corrupted. Data might be completely wrong.\n";
		}
	}

	uint8_t slot = 0;
	if (!message.empty()) message += "\n";

	for (uint8_t f = 0; f < ACTUAL_NUM_FILE_SLOTS; f++)
	{
		GameFile* gameFile = saveData.GetSaveFile()->GetGameFile(f);
		if (!gameFile->IsUsed()) continue;

		if (!gameFile->IsValid())
		{
			message += std::string("Game file ") + std::to_string(slot++) + " is corrupted. Data might be completely wrong.\n";
		}
	}

	slot = 0;
	if (!message.empty()) message += "\n";

	for (uint8_t f = 0; f < ACTUAL_NUM_FILE_SLOTS; f++)
	{
		MultiplayerProfile* mpProfile = saveData.GetSaveFile()->GetMultiplayerProfile(f);
		if (!mpProfile->IsUsed()) continue;

		if (!mpProfile->IsValid())
		{
			message += std::string("Multiplayer profile ") + std::to_string(slot++) + " is corrupted. Data might be completely wrong.\n";
		}
	}

	slot = 0;
	if (!message.empty()) message += "\n";

	for (uint8_t f = 0; f < ACTUAL_NUM_FILE_SLOTS; f++)
	{
		MultiplayerSetup* mpSetup = saveData.GetSaveFile()->GetMultiplayerSetup(f);
		if (!mpSetup->IsUsed()) continue;

		if (!mpSetup->IsValid())
		{
			message += std::string("Multiplayer settings ") + std::to_string(slot++) + " is corrupted. Data might be completely wrong.\n";
		}
	}

	if (!message.empty())
	{
		popupDialog->SetMessage(MessageTypes::Warning, "Load warnings", message);
		popupDialog->SetIsVisible(true);
	}
}

void MainUI::Save()
{
	if (!saveData.IsSaveFileLoaded()) return;

	try
	{
		saveData.Save(currentFilePath);
	}
	catch (const std::runtime_error& error)
	{
		popupDialog->SetMessage(MessageTypes::Error, "Error", error.what());
		popupDialog->SetIsVisible(true);
	}
}

void MainUI::CopySlot(const uint8_t originSlotIndex, const uint8_t destinationSlotIndex) const
{
	/*if (!saveData.IsSaveFileLoaded()) return;
	if (originSlotIndex == destinationSlotIndex) return;

	SaveSlot* origin = saveData.GetSaveFile()->GetSaveSlot(originSlotIndex);

	if (origin == nullptr)
	{
		DeleteSlot(destinationSlotIndex);
		return;
	}

	SaveSlot* destination = saveData.GetSaveFile()->GetSaveSlot(destinationSlotIndex);

	if (destination != nullptr)
	{
		memcpy(destination, origin, SAVE_SLOT_SIZE);
		destination->SetSlotIndex(destinationSlotIndex + 1);
		destination->UpdateChecksum(saveData.NeedsEndianSwap());
	}
	else
	{
		for (uint8_t s = 0; s < TOTAL_NUM_SAVE_SLOTS; s++)
		{
			destination = saveData.GetSaveFile()->GetRawSaveSlot(s);
			if (destination->GetMagic() == SAVE_SLOT_MAGIC) continue;

			memcpy(destination, origin, SAVE_SLOT_SIZE);
			destination->SetSlotIndex(destinationSlotIndex + 1);
			destination->UpdateChecksum(saveData.NeedsEndianSwap());

			break;
		}
	}*/
}

void MainUI::DeleteSlot(const uint8_t slotIndex) const
{
	/*if (!saveData.IsSaveFileLoaded()) return;

	SaveSlot* saveSlot = saveData.GetSaveFile()->GetSaveSlot(slotIndex);
	if (saveSlot == nullptr) return;

	memset(saveSlot, 0, SAVE_SLOT_SIZE);
	saveSlot->UpdateChecksum(saveData.NeedsEndianSwap());*/
}
