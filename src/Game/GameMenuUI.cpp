#include "GameMenuUI.h"

#include <imgui/imgui.h>

#include "../MainUI.h"
#include "SaveData.h"
#include "SaveFile.h"
#include "SaveEditorUI.h"

GameMenuUI::GameMenuUI(Window* window, BaseUI* parentUi, SaveEditorUI* saveEditorUi) : BaseUI(window, parentUi)
{
	this->mainUi = (MainUI*)parentUi;
	this->saveEditorUi = saveEditorUi;
}

GameMenuUI::~GameMenuUI()
{
}

void GameMenuUI::VisibilityChanged(const bool _isVisible)
{
}

void GameMenuUI::DoRender()
{
	SaveData* saveData = mainUi->GetSaveFile()->GetSaveData();

	if (saveData && ImGui::BeginMenu("Tools"))
	{
		/*int format = (int)saveData.GetFormat() - 1;
		if (ImGui::Combo("Save Format", &format, saveFormatNames, NUM_SAVE_FORMATS))
		{
			saveData.SetFormat((SaveFormats)(format + 1));
		}*/

		uint8_t gameFileCount = saveData->GetGameFileCount();
		uint8_t mpSetupCount = saveData->GetMultiplayerSetupCount();
		uint8_t mpProfileCount = saveData->GetMultiplayerProfileCount();

		ImGui::SeparatorText("Copy");

		if (gameFileCount == 0 || gameFileCount >= NUM_FILE_SLOTS) ImGui::BeginDisabled();
		if (ImGui::BeginMenu("Single Player Agent File##CopyGameFile"))
		{
			uint8_t file = 0;

			for (uint8_t f = 0; f < ACTUAL_NUM_FILE_SLOTS; f++)
			{
				GameFile* gameFile = saveData->GetGameFile(f);
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
				MultiplayerSetup* mpSetup = saveData->GetMultiplayerSetup(f);
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
				MultiplayerProfile* mpProfile = saveData->GetMultiplayerProfile(f);
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
				GameFile* gameFile = saveData->GetGameFile(f);
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
				MultiplayerSetup* mpSetup = saveData->GetMultiplayerSetup(f);
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
				MultiplayerProfile* mpProfile = saveData->GetMultiplayerProfile(f);
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
}

void GameMenuUI::CopyGameFile(const GameFile* srcGameFile) const
{
	SaveData* saveData = mainUi->GetSaveFile()->GetSaveData();

	uint8_t file = 0;
	GameFile* dstGameFile = saveData->GetFirstUnusedGameFile(&file);

	if (dstGameFile == nullptr)
	{
		//popupDialog->SetMessage(MessageTypes::Error, "Error", "Couldn't find any unused GameFile.");
		//popupDialog->SetIsVisible(true);
		return;
	}

	uint16_t deviceSerial = dstGameFile->pakFileHeader.deviceSerial;
	uint8_t id = dstGameFile->pakFileHeader.id;

	*dstGameFile = *srcGameFile;

	dstGameFile->pakFileHeader.deviceSerial = deviceSerial;
	dstGameFile->pakFileHeader.id = id;
	snprintf(dstGameFile->name, MAX_NAME_LENGTH + 1, "New File %u", file & 0x7);
}

void GameMenuUI::CopyMultiplayerProfile(const MultiplayerProfile* srcMpProfile) const
{
	SaveData* saveData = mainUi->GetSaveFile()->GetSaveData();

	uint8_t file = 0;
	MultiplayerProfile* dstMpProfile = saveData->GetFirstUnusedMultiplayerProfile(&file);

	if (dstMpProfile == nullptr)
	{
		//popupDialog->SetMessage(MessageTypes::Error, "Error", "Couldn't find any unused MultiplayerProfile.");
		//popupDialog->SetIsVisible(true);
		return;
	}

	uint16_t deviceSerial = dstMpProfile->pakFileHeader.deviceSerial;
	uint8_t id = dstMpProfile->pakFileHeader.id;

	*dstMpProfile = *srcMpProfile;

	dstMpProfile->pakFileHeader.deviceSerial = deviceSerial;
	dstMpProfile->pakFileHeader.id = id;
	snprintf(dstMpProfile->name, MAX_NAME_LENGTH + 1, "New File %u", file & 0x7);
}

void GameMenuUI::CopyMultiplayerSetup(const MultiplayerSetup* srcMpSetup) const
{
	SaveData* saveData = mainUi->GetSaveFile()->GetSaveData();

	uint8_t file = 0;
	MultiplayerSetup* dstMpSetup = saveData->GetFirstUnusedMultiplayerSetup(&file);

	if (dstMpSetup == nullptr)
	{
		//popupDialog->SetMessage(MessageTypes::Error, "Error", "Couldn't find any unused MultiplayerSetup.");
		//popupDialog->SetIsVisible(true);
		return;
	}

	uint16_t deviceSerial = dstMpSetup->pakFileHeader.deviceSerial;
	uint8_t id = dstMpSetup->pakFileHeader.id;

	*dstMpSetup = *srcMpSetup;

	dstMpSetup->pakFileHeader.deviceSerial = deviceSerial;
	dstMpSetup->pakFileHeader.id = id;
	snprintf(dstMpSetup->name, MAX_NAME_LENGTH + 1, "New File %u", file & 0x7);
}

void GameMenuUI::DeleteGameFile(GameFile* gameFile) const
{
	gameFile->pakFileHeader.occupied = 0;
}

void GameMenuUI::DeleteMultiplayerProfile(MultiplayerProfile* mpProfile) const
{
	mpProfile->pakFileHeader.occupied = 0;
}

void GameMenuUI::DeleteMultiplayerSetup(MultiplayerSetup* mpSetup) const
{
	mpSetup->pakFileHeader.occupied = 0;
}
