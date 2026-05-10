#pragma once

#include <cstdint>

#include "../BaseUI.h"

class MainUI;
class SaveEditorUI;

struct GameFile;
struct MultiplayerProfile;
struct MultiplayerSetup;

class GameMenuUI : public BaseUI
{
private:
	MainUI* mainUi;
	SaveEditorUI* saveEditorUi;

public:
	GameMenuUI(Window* window, BaseUI* parentUi, SaveEditorUI* saveEditorUi);
	~GameMenuUI();

protected:
	void VisibilityChanged(const bool _isVisible) override;
	void DoRender() override;

private:
	void CopyGameFile(const GameFile* srcGameFile) const;
	void CopyMultiplayerProfile(const MultiplayerProfile* srcMpProfile) const;
	void CopyMultiplayerSetup(const MultiplayerSetup* srcMpSetup) const;

	void DeleteGameFile(GameFile* gameFile) const;
	void DeleteMultiplayerProfile(MultiplayerProfile* mpProfile) const;
	void DeleteMultiplayerSetup(MultiplayerSetup* mpSetup) const;
};