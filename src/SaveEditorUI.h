#pragma once

#include <cstdint>
#include "BaseUI.h"
#include "SaveData.h"
class MainUI;

const char* const tabNames[]
{
	"Single Player Files",
	"Combat Simulator Settings",
	"Combat Simulator Players",
	"Global Data"
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
	void RenderMultiplayerSetupsSection(SaveFile* saveFile);

	bool CheckboxProgressFlags(GameFile* gameFile, const char* label, const SinglePlayerFlags flag, const bool reverse = false) const;
	bool CheckboxMpProfileOptionsFlags(MultiplayerProfile* mpProfile, const char* label, const MultiplayerOptionsFlags flag, const bool reverse = false) const;
	bool CheckboxMpProfileDisplayOptionsFlags(MultiplayerProfile* mpProfile, const char* label, const MultiplayerDisplayOptionsFlags flag, const bool reverse = false) const;
	bool CheckboxMpSetupOptionsFlags(MultiplayerSetup* mpSetup, const char* label, const MultiplayerSetupFlags flag, const bool reverse = false) const;
	void NameInputField(const char* label, char* name) const;
	void InputScalarU16(const char* label, uint16_t* value, const uint8_t bits) const;
	void InputScalarU32(const char* label, uint32_t* value, const uint8_t bits) const;

	void PrintEmptySlot() const;
	void PrintHeader(const char* label) const;
};