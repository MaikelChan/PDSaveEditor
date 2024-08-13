#pragma once

#include <cstdint>
#include "BaseUI.h"
#include "SaveData.h"
class MainUI;

#define CONFIG_FILE_NAME "config.json"

const char* const tabNames[]
{
	"Global Data",
	"Single Player",
	"Multiplayer Profiles",
	"Multiplayer Settings"
};

class SaveEditorUI : public BaseUI
{
private:
	const MainUI* mainUI;

public:
	SaveEditorUI(const MainUI* mainUI);
	~SaveEditorUI();

protected:
	virtual void VisibilityChanged(const bool isVisible) override;
	virtual void DoRender() override;

private:
	void RenderGlobalDataSection(SaveFile* saveFile);
	void RenderSinglePlayerSection(SaveFile* saveFile);
	void RenderMultiplayerProfilesSection(SaveFile* saveFile);
	void RenderMultiplayerSettingsSection(SaveFile* saveFile);

	void RenderLevelDataSection(const SaveData& saveData, SaveSlot* saveSlot);
	void RenderAbilitiesItemsSection(const SaveData& saveData, SaveSlot* saveSlot);
	void RenderProgressFlagsSection(const SaveData& saveData, SaveSlot* saveSlot);

	bool CheckboxProgressFlags(GameFile* gameFile, const char* label, const SinglePlayerFlags flag, const bool reverse = false) const;
	void NameInputField(const char* label, char* name) const;
	void InputScalarU16(const char* label, uint16_t* value, const uint8_t bits) const;
	void InputScalarU32(const char* label, uint32_t* value, const uint8_t bits) const;


	void PrintEmptySlot() const;
	void PrintChecksum(const uint32_t checksum) const;
	void PrintHeader(const char* label) const;
	void BeginFlagsGroup(const char* label) const;
	void EndFlagsGroup() const;
};
