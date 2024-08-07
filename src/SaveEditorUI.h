#pragma once

#include <cstdint>
#include "BaseUI.h"
#include "SaveData.h"
class MainUI;

#define CONFIG_FILE_NAME "config.json"

const char* const tabNames[]
{
	"Game 1 (Banjo Sleeping)",
	"Game 2 (Banjo Cooking)",
	"Game 3 (Banjo Playing)"
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
	void RenderLevelDataSection(const SaveData& saveData, SaveSlot* saveSlot);
	void RenderAbilitiesItemsSection(const SaveData& saveData, SaveSlot* saveSlot);
	void RenderProgressFlagsSection(const SaveData& saveData, SaveSlot* saveSlot);
	void RenderGlobalDataSection(const SaveData& saveData, SaveFile* saveFile);

	bool CheckboxProgressFlags(const SaveData& saveData, SaveSlot* saveSlot, const char* label, const ProgressFlags flag) const;
	uint8_t InputProgressFlags(const SaveData& saveData, SaveSlot* saveSlot, const char* label, const ProgressFlags flag, const uint8_t bitsCount, const uint8_t maxValue) const;
	void CheckboxAbility(const SaveData& saveData, SaveSlot* saveSlot, const char* label, const Abilities ability) const;
	void CheckboxSnS(const SaveData& saveData, const char* label, const SnS unlockedSnsItem, const SnS collectedSnsItem) const;
	void PrintChecksum(const uint32_t checksum) const;
	void PrintHeader(const char* label) const;
	void BeginProgressFlagsGroup(const char* label) const;
	void EndProgressFlagsGroup() const;
};
