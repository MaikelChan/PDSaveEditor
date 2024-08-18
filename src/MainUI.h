#pragma once

#include "BaseUI.h"
class SaveEditorUI;
class PopupDialog;
class AboutWindow;
#include "SaveData.h"
#include <imgui/imgui.h>
#include <imfilebrowser.h>

#define CONFIG_FILE_NAME "config.json"

const char* const saveFormatNames[]
{
	"Nintendo 64",
	"PC"
};

class MainUI : public BaseUI
{
private:
	SaveEditorUI* saveEditor;
	PopupDialog* popupDialog;
	AboutWindow* aboutWindow;

	ImGui::FileBrowser fileDialog;
	bool fileDialogIsSave;

	SaveData saveData;

	std::filesystem::path currentPath;
	std::filesystem::path currentFilePath;

	float windowOpacity;

public:
	MainUI();
	~MainUI();

	inline const SaveData& GetSaveData() const { return saveData; }
	inline float GetWindowOpacity() const { return windowOpacity; }

protected:
	virtual void VisibilityChanged(const bool isVisible) override;
	virtual void DoRender() override;

private:
	void LoadConfig();
	void SaveConfig() const;

	void Load(std::filesystem::path filePath);
	void LoadingProcess() const;
	void Save(std::filesystem::path filePath);

	void CopyGameFile(const GameFile* srcGameFile) const;
	void CopyMultiplayerProfile(const MultiplayerProfile* srcMpProfile) const;
	void CopyMultiplayerSetup(const MultiplayerSetup* srcMpSetup) const;

	void DeleteGameFile(GameFile* gameFile) const;
	void DeleteMultiplayerProfile(MultiplayerProfile* mpProfile) const;
	void DeleteMultiplayerSetup(MultiplayerSetup* mpSetup) const;
};