#include "SaveEditorUI.h"
#include "MainUI.h"
#include "Utils.h"

SaveEditorUI::SaveEditorUI(const MainUI* mainUI) : BaseUI(mainUI)
{
	SaveEditorUI::mainUI = mainUI;
}

SaveEditorUI::~SaveEditorUI()
{

}

void SaveEditorUI::VisibilityChanged(const bool isVisible)
{
	BaseUI::VisibilityChanged(isVisible);
}

void SaveEditorUI::DoRender()
{
	BaseUI::DoRender();

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);

	if (ImGui::Begin("Save Editor", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground))
	{
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;

		const SaveData& saveData = mainUI->GetSaveData();
		SaveFile* saveFile = saveData.GetSaveFile();

		if (saveFile && ImGui::BeginTabBar("Save Slots", tab_bar_flags))
		{
			for (int s = 0; s < 4; s++)
			{
				if (ImGui::BeginTabItem(tabNames[s]))
				{
					switch (s)
					{
						case 0:
							RenderGlobalDataSection(saveFile);
							break;

						case 1:
							RenderSinglePlayerSection(saveFile);
							break;

						case 2:
							RenderMultiplayerProfilesSection(saveFile);
							break;

						case 3:
							RenderMultiplayerSetupsSection(saveFile);
							break;
					}

					ImGui::EndTabItem();
				}
			}

			ImGui::EndTabBar();
		}
	}

	ImGui::End();
}

void SaveEditorUI::RenderGlobalDataSection(SaveFile* saveFile)
{
	BossFile* bossFile = nullptr;

	for (uint8_t f = 0; f < ACTUAL_NUM_BOSS_FILE_SLOTS; f++)
	{
		if (saveFile->GetBossFile(f)->IsUsed())
		{
			bossFile = saveFile->GetBossFile(f);
			break;
		}
	}

	if (bossFile == nullptr) return;

	if (ImGui::BeginTable("GlobalSettingsTable", 2, 0))
	{
		ImGui::TableSetupColumn("Column1", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Column2", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);

		PrintHeader("Settings");

		int language = bossFile->language;
		if (ImGui::Combo("##Language", &language, languageNames, NUM_LANGUAGES))
		{
			bossFile->language = language;
		}

		ImGui::Checkbox("Alt. title screen unlocked", &bossFile->altTitleUnlocked);
		ImGui::Checkbox("Alt. title screen enabled", &bossFile->altTitleEnabled);

		ImGui::TableSetColumnIndex(1);

		PrintHeader("Team Names");

		for (uint8_t t = 0; t < NUM_MP_TEAMS; t++)
		{
			NameInputField(teamNames[t], bossFile->teamNames[t]);
		}

		ImGui::EndTable();
	}

	PrintHeader("Soundtrack");

	if (ImGui::BeginTable("SoundtrackTable", 4, 0))
	{
		ImGui::TableSetupColumn("Column1", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Column2", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Column3", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Column4", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);

		ImGui::Checkbox("Use multiple tunes", &bossFile->usingmultipletunes);

		if (!bossFile->usingmultipletunes) ImGui::BeginDisabled();

		if (ImGui::Button("Select All"))
		{
			for (uint8_t s = 0; s < NUM_SONGS; s++)
			{
				bossFile->SetMultiTrackSlotEnabled(s, true);
			}
		}

		if (ImGui::Button("Select None"))
		{
			for (uint8_t s = 0; s < NUM_SONGS; s++)
			{
				bossFile->SetMultiTrackSlotEnabled(s, false);
			}
		}

		if (ImGui::Button("Select Random"))
		{
			for (uint8_t s = 0; s < NUM_SONGS; s++)
			{
				bool enabled = (bool)(rand() % 2);
				bossFile->SetMultiTrackSlotEnabled(s, enabled);
			}
		}

		if (!bossFile->usingmultipletunes) ImGui::EndDisabled();

		ImGui::TableSetColumnIndex(1);

		if (bossFile->usingmultipletunes)
		{
			for (uint8_t s = 0; s < NUM_SONGS; s++)
			{
				if (s == 15) ImGui::TableSetColumnIndex(2);
				else if (s == 30) ImGui::TableSetColumnIndex(3);

				bool enabled = bossFile->IsMultiTrackSlotEnabled(s);
				if (ImGui::Checkbox(songNames[s], &enabled))
				{
					bossFile->SetMultiTrackSlotEnabled(s, enabled);
				}
			}
		}
		else
		{
			int song = bossFile->tracknum == 255 ? NUM_SONGS : bossFile->tracknum;

			for (uint8_t s = 0; s < NUM_SONGS + 1; s++)
			{
				if (s == 15) ImGui::TableSetColumnIndex(2);
				else if (s == 30) ImGui::TableSetColumnIndex(3);

				if (ImGui::RadioButton(songNames[s], &song, s))
				{
					bossFile->tracknum = s == NUM_SONGS ? 255 : s;
				}
			}
		}

		ImGui::EndTable();
	}
}

void SaveEditorUI::RenderSinglePlayerSection(SaveFile* saveFile)
{
	GameFile* gameFiles[NUM_FILE_SLOTS] = {};
	uint8_t file = 0;

	for (uint8_t f = 0; f < ACTUAL_NUM_FILE_SLOTS; f++)
	{
		GameFile* gameFile = saveFile->GetGameFile(f);
		if (gameFile->IsUsed())
		{
			gameFiles[file] = gameFile;
			file++;
		}
	}

	if (ImGui::BeginTabBar("Single Player Slots", ImGuiTabBarFlags_None))
	{
		for (uint8_t f = 0; f < NUM_FILE_SLOTS; f++)
		{
			GameFile* gameFile = gameFiles[f];

			// "###Slot%u is used as tab id, to prevent regenerating
			// the whole window when updating the name of the file

			char tabName[32];
			snprintf(tabName, 32, "File %u (%s)###Slot%u", f + 1, gameFile != nullptr ? gameFile->name : "Empty", f + 1);

			if (ImGui::BeginTabItem(tabName))
			{
				if (gameFile == nullptr)
				{
					PrintEmptySlot();
					ImGui::EndTabItem();
					continue;
				}

				ImGui::BeginChild("Single Player Frame", ImVec2(0, 0), false, 0);

				PrintHeader("File Info");

				NameInputField("Name", gameFile->name);

				InputScalarU32("Play Time (s)", &gameFile->totaltime, 32);
				if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNone))
					ImGui::SetTooltip("%s", Utils::GetTimeString(gameFile->totaltime).c_str());

				int thumbnail = gameFile->thumbnail;
				if (ImGui::Combo("File Thumbnail", &thumbnail, thumbnailNames, NUM_SOLOSTAGES + 1))
				{
					gameFile->thumbnail = thumbnail;
				}

				int autostageindex = gameFile->autostageindex;
				if (ImGui::Combo("Last Played Stage", &autostageindex, stageNames, NUM_SOLOSTAGES))
				{
					gameFile->autostageindex = autostageindex;
				}

				int autodifficulty = gameFile->autodifficulty;
				if (ImGui::Combo("Last Played Difficulty", &autodifficulty, difficultyNames, NUM_DIFFICULTIES))
				{
					gameFile->autodifficulty = autodifficulty;
				}

				PrintHeader("Settings");

				if (ImGui::BeginTabBar("Single Player Settings", ImGuiTabBarFlags_None))
				{
					if (ImGui::BeginTabItem("Audio"))
					{
						const ImU8 volumeMin = 0, volumeMax = 40;
						ImGui::SliderScalar("Sound Volume", ImGuiDataType_U8, &gameFile->sfxVolume, &volumeMin, &volumeMax, "%u");
						ImGui::SliderScalar("Music Volume", ImGuiDataType_U8, &gameFile->musicVolume, &volumeMin, &volumeMax, "%u");

						int soundMode = gameFile->soundMode;
						if (ImGui::Combo("Sound Mode", &soundMode, soundModeNames, NUM_SOUND_MODES))
						{
							gameFile->soundMode = soundMode;
						}

						CheckboxProgressFlags(gameFile, "Language Filter", SinglePlayerFlags::LANGFILTERON);

						ImGui::EndTabItem();
					}

					if (ImGui::BeginTabItem("Video"))
					{
						int size = 0;
						if (gameFile->GetFlag(SinglePlayerFlags::SCREENSIZE_CINEMA)) size = 2;
						else if (gameFile->GetFlag(SinglePlayerFlags::SCREENSIZE_WIDE)) size = 1;

						if (ImGui::Combo("Screen Size", &size, screenSizeNames, NUM_SCREEN_SIZES))
						{
							gameFile->SetFlag(SinglePlayerFlags::SCREENSIZE_WIDE, size == 1);
							gameFile->SetFlag(SinglePlayerFlags::SCREENSIZE_CINEMA, size == 2);
						}

						int ratio = gameFile->GetFlag(SinglePlayerFlags::SCREENRATIO) ? 1 : 0;
						if (ImGui::Combo("Ratio", &ratio, ratioNames, NUM_RATIOS))
						{
							gameFile->SetFlag(SinglePlayerFlags::SCREENRATIO, ratio == 1);
						}

						int split = gameFile->GetFlag(SinglePlayerFlags::SCREENSPLIT) ? 1 : 0;
						if (ImGui::Combo("2-Player Screen Split", &split, screenSplitModeNames, NUM_SCREEN_SPLIT_MODES))
						{
							gameFile->SetFlag(SinglePlayerFlags::SCREENSPLIT, split == 1);
						}

						CheckboxProgressFlags(gameFile, "Hi-Res", SinglePlayerFlags::HIRES);

						ImGui::EndTabItem();
					}

					if (ImGui::BeginTabItem("Controls"))
					{
						if (ImGui::BeginTable("ControlSettingsTable", 2, 0))
						{
							ImGui::TableSetupColumn("Column1", ImGuiTableColumnFlags_WidthStretch);
							ImGui::TableSetupColumn("Column2", ImGuiTableColumnFlags_WidthStretch);

							ImGui::TableNextRow();

							for (uint8_t p = 0; p < 2; p++)
							{
								ImGui::PushID(p);
								ImGui::TableSetColumnIndex(p);

								char tabName[10];
								snprintf(tabName, 10, "Player %u", p + 1);
								PrintHeader(tabName);

								int controlMode = gameFile->controlModes[p];
								if (ImGui::Combo("Control Mode", &controlMode, controlModeNames, NUM_CONTROL_MODES))
								{
									gameFile->controlModes[p] = controlMode;
								}

								if (p == 0)
								{
									CheckboxProgressFlags(gameFile, "Reverse Pitch", SinglePlayerFlags::P1_FORWARDPITCH, true);
									CheckboxProgressFlags(gameFile, "Look Ahead", SinglePlayerFlags::P1_LOOKAHEAD);
									CheckboxProgressFlags(gameFile, "Head Roll", SinglePlayerFlags::P1_HEADROLL);
									CheckboxProgressFlags(gameFile, "Auto-Aim", SinglePlayerFlags::P1_AUTOAIM);

									int aimControl = gameFile->GetFlag(SinglePlayerFlags::P1_AIMCONTROL) ? 1 : 0;
									if (ImGui::Combo("Aim Control", &aimControl, aimControlModeNames, NUM_AIM_CONTROL_MODES))
									{
										gameFile->SetFlag(SinglePlayerFlags::P1_AIMCONTROL, aimControl == 1);
									}
								}
								else
								{
									CheckboxProgressFlags(gameFile, "Reverse Pitch", SinglePlayerFlags::P2_FORWARDPITCH, true);
									CheckboxProgressFlags(gameFile, "Look Ahead", SinglePlayerFlags::P2_LOOKAHEAD);
									CheckboxProgressFlags(gameFile, "Head Roll", SinglePlayerFlags::P2_HEADROLL);
									CheckboxProgressFlags(gameFile, "Auto-Aim", SinglePlayerFlags::P2_AUTOAIM);

									int aimControl = gameFile->GetFlag(SinglePlayerFlags::P2_AIMCONTROL) ? 1 : 0;
									if (ImGui::Combo("Aim Control", &aimControl, aimControlModeNames, NUM_AIM_CONTROL_MODES))
									{
										gameFile->SetFlag(SinglePlayerFlags::P2_AIMCONTROL, aimControl == 1);
									}
								}

								ImGui::PopID();
							}

							ImGui::EndTable();
						}

						ImGui::EndTabItem();
					}

					if (ImGui::BeginTabItem("Display"))
					{
						CheckboxProgressFlags(gameFile, "In-Game Subtitles", SinglePlayerFlags::INGAMESUBTITLES);
						CheckboxProgressFlags(gameFile, "Cutscene Subtitles", SinglePlayerFlags::CUTSCENESUBTITLES);

						if (ImGui::BeginTable("DisplaySettingsTable", 2, 0))
						{
							ImGui::TableSetupColumn("Column1", ImGuiTableColumnFlags_WidthStretch);
							ImGui::TableSetupColumn("Column2", ImGuiTableColumnFlags_WidthStretch);

							ImGui::TableNextRow();

							for (uint8_t p = 0; p < 2; p++)
							{
								ImGui::PushID(p);
								ImGui::TableSetColumnIndex(p);

								char tabName[10];
								snprintf(tabName, 10, "Player %u", p + 1);
								PrintHeader(tabName);

								if (p == 0)
								{
									CheckboxProgressFlags(gameFile, "Sight on Screen", SinglePlayerFlags::P1_SIGHTONSCREEN);
									CheckboxProgressFlags(gameFile, "Always Show Target", SinglePlayerFlags::P1_ALWAYSSHOWTARGET);
									CheckboxProgressFlags(gameFile, "Show Zoom Range", SinglePlayerFlags::P1_SHOWZOOMRANGE);
									CheckboxProgressFlags(gameFile, "Ammo on Screen", SinglePlayerFlags::P1_AMMOONSCREEN);
									CheckboxProgressFlags(gameFile, "Show Gun Function", SinglePlayerFlags::P1_SHOWGUNFUNCTION);
									CheckboxProgressFlags(gameFile, "Paintball", SinglePlayerFlags::P1_PAINTBALL);
									CheckboxProgressFlags(gameFile, "Show Mission Time", SinglePlayerFlags::P1_SHOWMISSIONTIME);
								}
								else
								{
									CheckboxProgressFlags(gameFile, "Sight on Screen", SinglePlayerFlags::P2_SIGHTONSCREEN);
									CheckboxProgressFlags(gameFile, "Always Show Target", SinglePlayerFlags::P2_ALWAYSSHOWTARGET);
									CheckboxProgressFlags(gameFile, "Show Zoom Range", SinglePlayerFlags::P2_SHOWZOOMRANGE);
									CheckboxProgressFlags(gameFile, "Ammo on Screen", SinglePlayerFlags::P2_AMMOONSCREEN);
									CheckboxProgressFlags(gameFile, "Show Gun Function", SinglePlayerFlags::P2_SHOWGUNFUNCTION);
									CheckboxProgressFlags(gameFile, "Paintball", SinglePlayerFlags::P2_PAINTBALL);
									CheckboxProgressFlags(gameFile, "Show Mission Time", SinglePlayerFlags::P2_SHOWMISSIONTIME);
								}

								ImGui::PopID();
							}

							ImGui::EndTable();
						}

						ImGui::EndTabItem();
					}

					ImGui::EndTabBar();
				}

				if (ImGui::BeginTable("TutorialFlagsTable", 3, 0))
				{
					ImGui::TableSetupColumn("Column1", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableSetupColumn("Column2", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableSetupColumn("Column3", ImGuiTableColumnFlags_WidthStretch);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);

					PrintHeader("Carrington Institute");
					CheckboxProgressFlags(gameFile, "Tour Started", SinglePlayerFlags::CI_TOUR_STARTED);
					CheckboxProgressFlags(gameFile, "Tour Done", SinglePlayerFlags::CI_TOUR_DONE);

					PrintHeader("How-to messages");
					CheckboxProgressFlags(gameFile, "Use HoverCrates", SinglePlayerFlags::HOWTO_HOVERCRATE);
					CheckboxProgressFlags(gameFile, "Use HoveBikes", SinglePlayerFlags::HOWTO_HOVERBIKE);
					CheckboxProgressFlags(gameFile, "Use Doors", SinglePlayerFlags::HOWTO_DOORS);
					CheckboxProgressFlags(gameFile, "Use Elevators", SinglePlayerFlags::HOWTO_ELEVATORS);
					CheckboxProgressFlags(gameFile, "Use Terminals", SinglePlayerFlags::HOWTO_TERMINALS);

					ImGui::TableSetColumnIndex(1);

					PrintHeader("Device Training");
					CheckboxProgressFlags(gameFile, "Data Uplink", SinglePlayerFlags::CI_UPLINK_DONE);
					CheckboxProgressFlags(gameFile, "ECM Mine", SinglePlayerFlags::CI_ECMMINE_DONE);
					CheckboxProgressFlags(gameFile, "CamSpy", SinglePlayerFlags::CI_CAMSPY_DONE);
					CheckboxProgressFlags(gameFile, "Night Vision", SinglePlayerFlags::CI_NIGHTVISION_DONE);
					CheckboxProgressFlags(gameFile, "Door Decoder", SinglePlayerFlags::CI_DOORDECODER_DONE);
					CheckboxProgressFlags(gameFile, "R-Tracker", SinglePlayerFlags::CI_RTRACKER_DONE);
					CheckboxProgressFlags(gameFile, "IR Scanner", SinglePlayerFlags::CI_IR_DONE);
					CheckboxProgressFlags(gameFile, "X-Ray Scanner", SinglePlayerFlags::CI_XRAY_DONE);
					CheckboxProgressFlags(gameFile, "Disguise", SinglePlayerFlags::CI_DISGUISE_DONE);
					CheckboxProgressFlags(gameFile, "Cloaking Device", SinglePlayerFlags::CI_CLOAK_DONE);

					ImGui::TableSetColumnIndex(2);

					PrintHeader("Holotraining");
					CheckboxProgressFlags(gameFile, "Holo 1 - Looking Around", SinglePlayerFlags::CI_HOLO1_DONE);
					CheckboxProgressFlags(gameFile, "Holo 2 - Movement 1", SinglePlayerFlags::CI_HOLO2_DONE);
					CheckboxProgressFlags(gameFile, "Holo 3 - Movement 2", SinglePlayerFlags::CI_HOLO3_DONE);
					CheckboxProgressFlags(gameFile, "Holo 4 - Unarmed Combat 1", SinglePlayerFlags::CI_HOLO4_DONE);
					CheckboxProgressFlags(gameFile, "Holo 5 - Unarmed Combat 2", SinglePlayerFlags::CI_HOLO5_DONE);
					CheckboxProgressFlags(gameFile, "Holo 6 - Live Combat 1", SinglePlayerFlags::CI_HOLO6_DONE);
					CheckboxProgressFlags(gameFile, "Holo 7 - Live Combat 2", SinglePlayerFlags::CI_HOLO7_DONE);

					ImGui::EndTable();
				}

				if (ImGui::BeginTable("CoOpFlagsTable", 2, 0))
				{
					ImGui::TableSetupColumn("Column1", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableSetupColumn("Column2", ImGuiTableColumnFlags_WidthStretch);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);

					PrintHeader("Co-Operative Options");
					CheckboxProgressFlags(gameFile, "Radar On###RadarCoOp", SinglePlayerFlags::COOPRADARON);
					CheckboxProgressFlags(gameFile, "Friendly Fire", SinglePlayerFlags::COOPFRIENDLYFIRE);

					ImGui::TableSetColumnIndex(1);

					PrintHeader("Counter-Operative Options");
					CheckboxProgressFlags(gameFile, "Radar On###RadarCounterOp", SinglePlayerFlags::ANTIRADARON);
					int counterOpPlayer = gameFile->GetFlag(SinglePlayerFlags::ANTIPLAYERNUM) ? 1 : 0;
					if (ImGui::Combo("Counter-Operative", &counterOpPlayer, coOpPlayerNames, 2))
					{
						gameFile->SetFlag(SinglePlayerFlags::ANTIPLAYERNUM, counterOpPlayer == 1);
					}

					ImGui::EndTable();
				}

				if (ImGui::BeginTable("StageMiscFlagsTable", 2, 0))
				{
					ImGui::TableSetupColumn("Column1", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableSetupColumn("Column2", ImGuiTableColumnFlags_WidthStretch);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);

					PrintHeader("Stage Flags");
					CheckboxProgressFlags(gameFile, "G5 Building - Mine Position", SinglePlayerFlags::G5_MINE);
					CheckboxProgressFlags(gameFile, "Area 51 Rescue - Mechanic Dead", SinglePlayerFlags::RESCUE_MECHANIC_DEAD);
					CheckboxProgressFlags(gameFile, "Air Force One - Entry", SinglePlayerFlags::AF1_ENTRY);
					CheckboxProgressFlags(gameFile, "Crash Site - Bike", SinglePlayerFlags::CRASHSITE_BIKE);
					CheckboxProgressFlags(gameFile, "CI Defense - Jonathan Alive", SinglePlayerFlags::DEFENSE_JON);

					ImGui::TableSetColumnIndex(1);

					PrintHeader("Misc. Flags");
					CheckboxProgressFlags(gameFile, "Used Transfer Pak", SinglePlayerFlags::USED_TRANSFERPAK);
					CheckboxProgressFlags(gameFile, "Found Timed Mine", SinglePlayerFlags::FOUNDTIMEDMINE);
					CheckboxProgressFlags(gameFile, "Found Proximity Mine", SinglePlayerFlags::FOUNDPROXYMINE);
					CheckboxProgressFlags(gameFile, "Found Remote Mine", SinglePlayerFlags::FOUNDREMOTEMINE);
					CheckboxProgressFlags(gameFile, "Language Flag 1", SinglePlayerFlags::LANGBIT1);
					CheckboxProgressFlags(gameFile, "Language Flag 2", SinglePlayerFlags::LANGBIT2);
					CheckboxProgressFlags(gameFile, "Language Flag 3", SinglePlayerFlags::LANGBIT3);

					ImGui::EndTable();
				}

				ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuter;

				PrintHeader("Stages");

				if (ImGui::BeginTable("StagesTable", 4, flags))
				{
					ImGui::TableSetupColumn("Stage Name");
					ImGui::TableSetupColumn("Agent");
					ImGui::TableSetupColumn("Special Agent");
					ImGui::TableSetupColumn("Perfect Agent");

					ImGui::TableHeadersRow();

					for (uint8_t s = 0; s < NUM_SOLOSTAGES; s++)
					{
						ImGui::PushID(s);

						ImGui::TableNextRow();

						ImGui::TableSetColumnIndex(0);
						ImGui::Text("%s", stageNames[s]);

						for (uint8_t d = 0; d < NUM_DIFFICULTIES; d++)
						{
							ImGui::PushID(d);
							ImGui::TableSetColumnIndex(d + 1);

							InputScalarU16("##Best Time", &gameFile->besttimes[s][d], 12);
							if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNone))
								ImGui::SetTooltip("Best time (s) = %s", Utils::GetTimeString(gameFile->besttimes[s][d]).c_str());

							ImGui::SameLine();

							bool completed = (gameFile->coopcompletions[d] & (1 << s)) != 0;
							if (ImGui::Checkbox("##Coop", &completed))
							{
								if (completed) gameFile->coopcompletions[d] |= (1 << s);
								else gameFile->coopcompletions[d] &= ~(1 << s);
							}

							if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNone))
								ImGui::SetTooltip("Coop Completed");

							ImGui::PopID();
						}

						ImGui::PopID();
					}

					ImGui::EndTable();
				}

				if (ImGui::BeginTable("ChallengesWeaponsTables", 2, 0))
				{
					ImGui::TableSetupColumn("Column1", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableSetupColumn("Column2", ImGuiTableColumnFlags_WidthStretch);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);

					PrintHeader("Weapons");

					if (ImGui::BeginTable("Weapons Table", 3, flags))
					{
						ImGui::TableSetupColumn("Weapon Name");
						ImGui::TableSetupColumn("Weapon Found");
						ImGui::TableSetupColumn("Firing Range Medal");

						ImGui::TableHeadersRow();

						for (uint8_t w = 0; w < NUM_FIRING_RANGE_WEAPONS; w++)
						{
							ImGui::PushID(w);

							ImGui::TableNextRow();

							ImGui::TableSetColumnIndex(0);
							ImGui::Text(weaponNames[frWeaponNameIndices[w]]);

							ImGui::TableSetColumnIndex(1);
							bool found = gameFile->GetWeaponFound(w);
							if (ImGui::Checkbox("##Found", &found))
							{
								gameFile->SetWeaponFound(w, found);
							}

							ImGui::TableSetColumnIndex(2);
							int medal = gameFile->GetFiringRangeScore(w);
							char* name = medal == 0 ? "None" : firingRangeMedalNames[medal - 1];
							if (ImGui::SliderInt("##Medal", &medal, 0, NUM_FIRING_RANGE_MEDALS, name, ImGuiSliderFlags_NoInput))
							{
								gameFile->SetFiringRangeScore(w, medal);
							}

							ImGui::PopID();
						}

						ImGui::EndTable();
					}

					ImGui::TableSetColumnIndex(1);

					PrintHeader("Completed Challenges (for unlocking features)");

					if (ImGui::BeginTable("Challenges Table", 5, flags))
					{
						ImGui::TableSetupColumn("Challenge Number");
						ImGui::TableSetupColumn("Player 1");
						ImGui::TableSetupColumn("Player 2");
						ImGui::TableSetupColumn("Player 3");
						ImGui::TableSetupColumn("Player 4");

						ImGui::TableHeadersRow();

						for (uint8_t c = 0; c < NUM_MP_CHALLENGES; c++)
						{
							ImGui::PushID(c);

							ImGui::TableNextRow();

							ImGui::TableSetColumnIndex(0);
							ImGui::Text("Challenge %u", c + 1);

							for (uint8_t p = 0; p < MAX_PLAYERS; p++)
							{
								ImGui::PushID(p);
								ImGui::TableSetColumnIndex(p + 1);

								bool completed = (gameFile->mpChallenges[c] & (1 << p)) != 0;
								if (ImGui::Checkbox("##Challenge completed", &completed))
								{
									if (completed) gameFile->mpChallenges[c] |= (1 << p);
									else gameFile->mpChallenges[c] &= ~(1 << p);
								}

								ImGui::PopID();
							}

							ImGui::PopID();
						}

						ImGui::EndTable();
					}

					ImGui::EndTable();
				}

				ImGui::EndChild();
				ImGui::EndTabItem();
			}
		}

		ImGui::EndTabBar();
	}
}

void SaveEditorUI::RenderMultiplayerProfilesSection(SaveFile* saveFile)
{
	MultiplayerProfile* mpProfiles[NUM_FILE_SLOTS] = {};
	uint8_t file = 0;

	for (uint8_t f = 0; f < ACTUAL_NUM_FILE_SLOTS; f++)
	{
		MultiplayerProfile* mpProfile = saveFile->GetMultiplayerProfile(f);
		if (mpProfile->IsUsed())
		{
			mpProfiles[file] = mpProfile;
			file++;
		}
	}

	if (ImGui::BeginTabBar("Multiplayer Profile Slots", ImGuiTabBarFlags_None))
	{
		for (uint8_t f = 0; f < NUM_FILE_SLOTS; f++)
		{
			MultiplayerProfile* mpProfile = mpProfiles[f];

			// "###Slot%u is used as tab id, to prevent regenerating
			// the whole window when updating the name of the file

			char tabName[32];
			snprintf(tabName, 32, "File %u (%s)###Slot%u", f + 1, mpProfile != nullptr ? mpProfile->name : "Empty", f + 1);

			if (ImGui::BeginTabItem(tabName))
			{
				if (mpProfile == nullptr)
				{
					PrintEmptySlot();
					ImGui::EndTabItem();
					continue;
				}

				ImGui::BeginChild("Multiplayer Profile Frame", ImVec2(0, 0), false, 0);

				if (ImGui::BeginTable("MpPlayerInfoStatsTable", 2, 0))
				{
					ImGui::TableSetupColumn("Column1", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableSetupColumn("Column2", ImGuiTableColumnFlags_WidthStretch);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);

					PrintHeader("File Info");

					NameInputField("Name", mpProfile->name);

					InputScalarU32("Play Time (s)", &mpProfile->time, 28);
					if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNone))
						ImGui::SetTooltip("%s", Utils::GetTimeString(mpProfile->time).c_str());

					const ImU8 headsMin = 0, headsMax = NUM_MP_HEADS - 1;
					ImGui::SliderScalar("Character Head", ImGuiDataType_U8, &mpProfile->headIndex, &headsMin, &headsMax, "%u");

					const ImU8 bodyMin = 0, bodyMax = NUM_MP_BODIES - 1;
					char bodyName[64];
					snprintf(bodyName, 64, "%u (%s)", mpProfile->bodyIndex, mpBodyNames[mpProfile->bodyIndex]);
					ImGui::SliderScalar("Character Body", ImGuiDataType_U8, &mpProfile->bodyIndex, &bodyMin, &bodyMax, bodyName);

					ImGui::TableSetColumnIndex(1);
					PrintHeader("Statistics");

					InputScalarU32("Kills", &mpProfile->kills, 20);
					InputScalarU32("Deaths", &mpProfile->deaths, 20);
					InputScalarU16("Accuracy", &mpProfile->accuracy, 10);
					InputScalarU32("Head Shots", &mpProfile->headshots, 20);
					ImGui::Separator();
					InputScalarU32("Ammo Used", &mpProfile->ammoused, 30);
					InputScalarU32("Damage Dealt", &mpProfile->damagedealt, 26);
					InputScalarU32("Pain Received", &mpProfile->painreceived, 26);
					ImGui::Separator();
					InputScalarU32("Games Played", &mpProfile->gamesplayed, 19);
					InputScalarU32("Games Won", &mpProfile->gameswon, 19);
					InputScalarU32("Games Lost", &mpProfile->gameslost, 19);
					InputScalarU32("Distance", &mpProfile->distance, 25);
					ImGui::Separator();
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0, 1.0, 0.1, 1.0));
					InputScalarU32("Accuracy Medals", &mpProfile->accuracymedals, 18);
					ImGui::PopStyleColor();
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9, 0.9, 0.0, 1.0));
					InputScalarU32("Head Shot Medals", &mpProfile->headshotmedals, 18);
					ImGui::PopStyleColor();
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 0.5, 0.5, 1.0));
					InputScalarU32("KillMaster Medals", &mpProfile->killmastermedals, 18);
					ImGui::PopStyleColor();
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0, 0.9, 0.9, 1.0));
					InputScalarU16("Survivor Medals", &mpProfile->survivormedals, 16);
					ImGui::PopStyleColor();
					ImGui::Separator();
					uint8_t title = (uint8_t)mpProfile->GetPlayerTitle(true);
					ImGui::Text("%s: %u", titleNames[title], NUM_MP_TITLES - title);

					ImGui::EndTable();
				}

				if (ImGui::BeginTable("MpOptionsTable", 2, 0))
				{
					ImGui::TableSetupColumn("Column1", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableSetupColumn("Column2", ImGuiTableColumnFlags_WidthStretch);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);

					PrintHeader("Control");

					int controlMode = mpProfile->controlmode;
					if (ImGui::Combo("Control Mode", &controlMode, controlModeNames, NUM_CONTROL_MODES - 4))
					{
						mpProfile->controlmode = controlMode;
					}

					CheckboxMpProfileOptionsFlags(mpProfile, "Reverse Pitch", MultiplayerOptionsFlags::FORWARDPITCH, true);
					CheckboxMpProfileOptionsFlags(mpProfile, "Look Ahead", MultiplayerOptionsFlags::LOOKAHEAD);
					CheckboxMpProfileOptionsFlags(mpProfile, "Head Roll", MultiplayerOptionsFlags::HEADROLL);
					CheckboxMpProfileOptionsFlags(mpProfile, "Auto-Aim", MultiplayerOptionsFlags::AUTOAIM);

					int aimControl = mpProfile->GetOptionsFlag(MultiplayerOptionsFlags::AIMCONTROL) ? 1 : 0;
					if (ImGui::Combo("Aim Control", &aimControl, aimControlModeNames, NUM_AIM_CONTROL_MODES))
					{
						mpProfile->SetOptionsFlag(MultiplayerOptionsFlags::AIMCONTROL, aimControl != 0);
					}

					CheckboxMpProfileOptionsFlags(mpProfile, "Sight on Screen", MultiplayerOptionsFlags::SIGHTONSCREEN);
					CheckboxMpProfileOptionsFlags(mpProfile, "Show Target", MultiplayerOptionsFlags::ALWAYSSHOWTARGET);
					CheckboxMpProfileOptionsFlags(mpProfile, "Zoom Range", MultiplayerOptionsFlags::SHOWZOOMRANGE);
					CheckboxMpProfileOptionsFlags(mpProfile, "Ammo on Screen", MultiplayerOptionsFlags::AMMOONSCREEN);
					CheckboxMpProfileOptionsFlags(mpProfile, "Gun Function", MultiplayerOptionsFlags::SHOWGUNFUNCTION);
					CheckboxMpProfileOptionsFlags(mpProfile, "Paintball", MultiplayerOptionsFlags::PAINTBALL);

					ImGui::TableSetColumnIndex(1);

					PrintHeader("Player Options");

					CheckboxMpProfileDisplayOptionsFlags(mpProfile, "Highlight Pickups", MultiplayerDisplayOptionsFlags::HIGHLIGHTPICKUPS);
					CheckboxMpProfileDisplayOptionsFlags(mpProfile, "Highlight Players", MultiplayerDisplayOptionsFlags::HIGHLIGHTPLAYERS);
					CheckboxMpProfileDisplayOptionsFlags(mpProfile, "Highlight Teams", MultiplayerDisplayOptionsFlags::HIGHLIGHTTEAMS);
					CheckboxMpProfileDisplayOptionsFlags(mpProfile, "Radar", MultiplayerDisplayOptionsFlags::RADAR);

					PrintHeader("Hidden Flags");

					CheckboxMpProfileOptionsFlags(mpProfile, "\"Save Player\" Window Shown", MultiplayerOptionsFlags::ASKEDSAVEPLAYER);
					CheckboxMpProfileOptionsFlags(mpProfile, "Show Mission Time", MultiplayerOptionsFlags::SHOWMISSIONTIME);

					ImGui::EndTable();
				}

				if (ImGui::BeginTable("MpChallengesWeaponsTables", 2, 0))
				{
					ImGui::TableSetupColumn("Column1", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableSetupColumn("Column2", ImGuiTableColumnFlags_WidthStretch);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);

					ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuter;

					PrintHeader("Weapon Functions");

					if (ImGui::BeginTable("Weapons Functions Table", 2, flags))
					{
						ImGui::TableSetupColumn("Weapon Name");
						ImGui::TableSetupColumn("Secondary Function");

						ImGui::TableHeadersRow();

						for (uint8_t w = 1 /* Unarmed */; w <= 35 /* Combat Boost */; w++)
						{
							ImGui::PushID(w);

							ImGui::TableNextRow();

							ImGui::TableSetColumnIndex(0);
							ImGui::Text(weaponNames[w]);

							ImGui::TableSetColumnIndex(1);
							bool secondaryFunc = mpProfile->GetWeaponSecondaryFunction(w);
							if (ImGui::Checkbox("##Seconday Func", &secondaryFunc))
							{
								mpProfile->SetWeaponSecondaryFunction(w, secondaryFunc);
							}

							ImGui::PopID();
						}

						ImGui::EndTable();
					}

					ImGui::TableSetColumnIndex(1);

					PrintHeader("Completed Challenges");

					if (ImGui::BeginTable("Challenges Table", 5, flags))
					{
						ImGui::TableSetupColumn("Challenge Number");
						ImGui::TableSetupColumn("Player 1");
						ImGui::TableSetupColumn("Player 2");
						ImGui::TableSetupColumn("Player 3");
						ImGui::TableSetupColumn("Player 4");

						ImGui::TableHeadersRow();

						for (uint8_t c = 0; c < NUM_MP_CHALLENGES; c++)
						{
							ImGui::PushID(c);

							ImGui::TableNextRow();

							ImGui::TableSetColumnIndex(0);
							ImGui::Text("Challenge %u", c + 1);

							for (uint8_t p = 0; p < MAX_PLAYERS; p++)
							{
								ImGui::PushID(p);
								ImGui::TableSetColumnIndex(p + 1);

								bool completed = (mpProfile->mpChallenges[c] & (1 << p)) != 0;
								if (ImGui::Checkbox("##Challenge completed", &completed))
								{
									if (completed) mpProfile->mpChallenges[c] |= (1 << p);
									else mpProfile->mpChallenges[c] &= ~(1 << p);
								}

								ImGui::PopID();
							}

							ImGui::PopID();
						}

						ImGui::EndTable();
					}

					ImGui::EndTable();
				}

				ImGui::EndChild();
				ImGui::EndTabItem();
			}
		}

		ImGui::EndTabBar();
	}
}

void SaveEditorUI::RenderMultiplayerSetupsSection(SaveFile* saveFile)
{
	MultiplayerSetup* mpSetups[NUM_FILE_SLOTS] = {};
	uint8_t file = 0;

	for (uint8_t f = 0; f < ACTUAL_NUM_FILE_SLOTS; f++)
	{
		MultiplayerSetup* mpSetup = saveFile->GetMultiplayerSetup(f);
		if (mpSetup->IsUsed())
		{
			mpSetups[file] = mpSetup;
			file++;
		}
	}

	if (ImGui::BeginTabBar("Multiplayer Settings Slots", ImGuiTabBarFlags_None))
	{
		for (uint8_t f = 0; f < NUM_FILE_SLOTS; f++)
		{
			MultiplayerSetup* mpSetup = mpSetups[f];

			// "###File%u is used as tab id, to prevent regenerating
			// the whole window when updating the name of the file

			char tabName[32];
			snprintf(tabName, 32, "File %u (%s)###Slot%u", f + 1, mpSetup != nullptr ? mpSetup->name : "Empty", f + 1);

			if (ImGui::BeginTabItem(tabName))
			{
				if (mpSetup == nullptr)
				{
					PrintEmptySlot();
					ImGui::EndTabItem();
					continue;
				}

				ImGui::BeginChild("Multiplayer Settings Frame", ImVec2(0, 0), false, 0);

				if (ImGui::BeginTable("MpSetupAndScenarioOptionsTable", 2, 0))
				{
					ImGui::TableSetupColumn("Column1", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableSetupColumn("Column2", ImGuiTableColumnFlags_WidthStretch);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);

					PrintHeader("Game Setup");

					NameInputField("Name", mpSetup->name);

					int arena = mpSetup->GetArena();
					if (ImGui::Combo("Arena", &arena, mpStageNames, NUM_MP_STAGES_AND_RANDOM))
					{
						mpSetup->SetArena(arena);
					}

					int scenario = mpSetup->scenario;
					if (ImGui::Combo("Scenario", &scenario, mpScenarioNames, NUM_MP_SCENARIOS))
					{
						mpSetup->scenario = scenario;
					}

					ImGui::TableSetColumnIndex(1);

					switch ((MultiplayerScenarios)scenario)
					{
						case MultiplayerScenarios::Combat:
						{
							PrintHeader("Combat Options");

							CheckboxMpSetupOptionsFlags(mpSetup, "One-Hit Kills", MultiplayerSetupFlags::ONEHITKILLS);

							int slowmo = mpSetup->GetSlowMotionMode();
							if (ImGui::Combo("Slow Motion", &slowmo, mpSlowMotionNames, NUM_MP_SLOWMOTION_MODES))
							{
								mpSetup->SetSlowMotionMode(slowmo);
							}

							CheckboxMpSetupOptionsFlags(mpSetup, "Fast Movement", MultiplayerSetupFlags::FASTMOVEMENT);
							CheckboxMpSetupOptionsFlags(mpSetup, "Display Team", MultiplayerSetupFlags::DISPLAYTEAM);
							CheckboxMpSetupOptionsFlags(mpSetup, "No Radar", MultiplayerSetupFlags::NORADAR);
							CheckboxMpSetupOptionsFlags(mpSetup, "No Auto-Aim", MultiplayerSetupFlags::NOAUTOAIM);
							CheckboxMpSetupOptionsFlags(mpSetup, "No Player Highlight", MultiplayerSetupFlags::NOPLAYERHIGHLIGHT);
							CheckboxMpSetupOptionsFlags(mpSetup, "No Pickup Highlight", MultiplayerSetupFlags::NOPICKUPHIGHLIGHT);

							break;
						}

						case MultiplayerScenarios::HoldTheBriefcase:
						{
							PrintHeader("Briefcase Options");

							CheckboxMpSetupOptionsFlags(mpSetup, "One-Hit Kills", MultiplayerSetupFlags::ONEHITKILLS);

							int slowmo = mpSetup->GetSlowMotionMode();
							if (ImGui::Combo("Slow Motion", &slowmo, mpSlowMotionNames, NUM_MP_SLOWMOTION_MODES))
							{
								mpSetup->SetSlowMotionMode(slowmo);
							}

							CheckboxMpSetupOptionsFlags(mpSetup, "Fast Movement", MultiplayerSetupFlags::FASTMOVEMENT);
							CheckboxMpSetupOptionsFlags(mpSetup, "Display Team", MultiplayerSetupFlags::DISPLAYTEAM);
							CheckboxMpSetupOptionsFlags(mpSetup, "No Radar", MultiplayerSetupFlags::NORADAR);
							CheckboxMpSetupOptionsFlags(mpSetup, "No Auto-Aim", MultiplayerSetupFlags::NOAUTOAIM);
							CheckboxMpSetupOptionsFlags(mpSetup, "Kills Score", MultiplayerSetupFlags::KILLSSCORE);
							CheckboxMpSetupOptionsFlags(mpSetup, "Highlight Briefcase", MultiplayerSetupFlags::HTB_HIGHLIGHTBRIEFCASE);
							CheckboxMpSetupOptionsFlags(mpSetup, "Show on Radar", MultiplayerSetupFlags::HTB_SHOWONRADAR);

							break;
						}

						case MultiplayerScenarios::HackerCentral:
						{
							PrintHeader("Hacker Options");

							CheckboxMpSetupOptionsFlags(mpSetup, "One-Hit Kills", MultiplayerSetupFlags::ONEHITKILLS);

							int slowmo = mpSetup->GetSlowMotionMode();
							if (ImGui::Combo("Slow Motion", &slowmo, mpSlowMotionNames, NUM_MP_SLOWMOTION_MODES))
							{
								mpSetup->SetSlowMotionMode(slowmo);
							}

							CheckboxMpSetupOptionsFlags(mpSetup, "Fast Movement", MultiplayerSetupFlags::FASTMOVEMENT);
							CheckboxMpSetupOptionsFlags(mpSetup, "Display Team", MultiplayerSetupFlags::DISPLAYTEAM);
							CheckboxMpSetupOptionsFlags(mpSetup, "No Radar", MultiplayerSetupFlags::NORADAR);
							CheckboxMpSetupOptionsFlags(mpSetup, "No Auto-Aim", MultiplayerSetupFlags::NOAUTOAIM);
							CheckboxMpSetupOptionsFlags(mpSetup, "Kills Score", MultiplayerSetupFlags::KILLSSCORE);
							CheckboxMpSetupOptionsFlags(mpSetup, "Highlight Terminal", MultiplayerSetupFlags::HTM_HIGHLIGHTTERMINAL);
							CheckboxMpSetupOptionsFlags(mpSetup, "Show on Radar", MultiplayerSetupFlags::HTM_SHOWONRADAR);

							break;
						}

						case MultiplayerScenarios::PopACap:
						{
							PrintHeader("Pop a Cap Options");

							CheckboxMpSetupOptionsFlags(mpSetup, "One-Hit Kills", MultiplayerSetupFlags::ONEHITKILLS);

							int slowmo = mpSetup->GetSlowMotionMode();
							if (ImGui::Combo("Slow Motion", &slowmo, mpSlowMotionNames, NUM_MP_SLOWMOTION_MODES))
							{
								mpSetup->SetSlowMotionMode(slowmo);
							}

							CheckboxMpSetupOptionsFlags(mpSetup, "Fast Movement", MultiplayerSetupFlags::FASTMOVEMENT);
							CheckboxMpSetupOptionsFlags(mpSetup, "Display Team", MultiplayerSetupFlags::DISPLAYTEAM);
							CheckboxMpSetupOptionsFlags(mpSetup, "No Radar", MultiplayerSetupFlags::NORADAR);
							CheckboxMpSetupOptionsFlags(mpSetup, "No Auto-Aim", MultiplayerSetupFlags::NOAUTOAIM);
							CheckboxMpSetupOptionsFlags(mpSetup, "Kills Score", MultiplayerSetupFlags::KILLSSCORE);
							CheckboxMpSetupOptionsFlags(mpSetup, "Highlight Target", MultiplayerSetupFlags::PAC_HIGHLIGHTTARGET);
							CheckboxMpSetupOptionsFlags(mpSetup, "Show on Radar", MultiplayerSetupFlags::PAC_SHOWONRADAR);

							break;
						}

						case MultiplayerScenarios::KingOfTheHill:
						{
							PrintHeader("Hill Options");

							CheckboxMpSetupOptionsFlags(mpSetup, "One-Hit Kills", MultiplayerSetupFlags::ONEHITKILLS);

							int slowmo = mpSetup->GetSlowMotionMode();
							if (ImGui::Combo("Slow Motion", &slowmo, mpSlowMotionNames, NUM_MP_SLOWMOTION_MODES))
							{
								mpSetup->SetSlowMotionMode(slowmo);
							}

							CheckboxMpSetupOptionsFlags(mpSetup, "Fast Movement", MultiplayerSetupFlags::FASTMOVEMENT);
							CheckboxMpSetupOptionsFlags(mpSetup, "Display Team", MultiplayerSetupFlags::DISPLAYTEAM);
							CheckboxMpSetupOptionsFlags(mpSetup, "No Radar", MultiplayerSetupFlags::NORADAR);
							CheckboxMpSetupOptionsFlags(mpSetup, "No Auto-Aim", MultiplayerSetupFlags::NOAUTOAIM);
							CheckboxMpSetupOptionsFlags(mpSetup, "Kills Score", MultiplayerSetupFlags::KILLSSCORE);
							CheckboxMpSetupOptionsFlags(mpSetup, "Hill on Radar", MultiplayerSetupFlags::KOH_HILLONRADAR);
							CheckboxMpSetupOptionsFlags(mpSetup, "Mobile Hill", MultiplayerSetupFlags::KOH_MOBILEHILL);

							const ImU8 hillMin = 10, hillMax = 120;
							uint8_t hillTime = mpSetup->GetHillTime();
							if (ImGui::SliderScalar("Time (s / Point)", ImGuiDataType_U8, &hillTime, &hillMin, &hillMax))
							{
								mpSetup->SetHillTime(hillTime);
							}

							break;
						}

						case MultiplayerScenarios::CaptureTheCase:
						{
							PrintHeader("Capture Options");

							CheckboxMpSetupOptionsFlags(mpSetup, "One-Hit Kills", MultiplayerSetupFlags::ONEHITKILLS);

							int slowmo = mpSetup->GetSlowMotionMode();
							if (ImGui::Combo("Slow Motion", &slowmo, mpSlowMotionNames, NUM_MP_SLOWMOTION_MODES))
							{
								mpSetup->SetSlowMotionMode(slowmo);
							}

							CheckboxMpSetupOptionsFlags(mpSetup, "Fast Movement", MultiplayerSetupFlags::FASTMOVEMENT);
							CheckboxMpSetupOptionsFlags(mpSetup, "Display Team", MultiplayerSetupFlags::DISPLAYTEAM);
							CheckboxMpSetupOptionsFlags(mpSetup, "No Radar", MultiplayerSetupFlags::NORADAR);
							CheckboxMpSetupOptionsFlags(mpSetup, "No Auto-Aim", MultiplayerSetupFlags::NOAUTOAIM);
							CheckboxMpSetupOptionsFlags(mpSetup, "Kills Score", MultiplayerSetupFlags::KILLSSCORE);
							CheckboxMpSetupOptionsFlags(mpSetup, "Show on Radar", MultiplayerSetupFlags::CTC_SHOWONRADAR);

							break;
						}
					}

					ImGui::EndTable();
				}

				PrintHeader("Simulants");

				/*if (ImGui::BeginTabBar("Multiplayer Simulants Settings", ImGuiTabBarFlags_None))
				{
					for (uint8_t s = 0; s < MAX_SIMULANTS; s++)
					{
						char tabName[32];
						snprintf(tabName, 32, "Simulant %u", s + 1);

						if (ImGui::BeginTabItem(tabName))
						{
							int difficulty = mpSetup->botsData[s].difficulty;
							if (ImGui::Combo("Difficulty", &difficulty, mpSimulantDifficultyNames, NUM_MP_SIMULANT_DIFFICULTIES + 1))
							{
								mpSetup->botsData[s].difficulty = difficulty;
							}

							if ((SimulantDifficulties)difficulty == SimulantDifficulties::DISABLED) ImGui::BeginDisabled();

							int type = mpSetup->botsData[s].type;
							if (ImGui::Combo("Type", &type, mpSimulantTypeNames, NUM_MP_SIMULANT_TYPES))
							{
								mpSetup->botsData[s].type = type;
							}

							int team = mpSetup->botsData[s].team;
							if (ImGui::Combo("Team", &team, teamNames, NUM_MP_TEAMS))
							{
								mpSetup->botsData[s].team = team;
							}

							const ImU8 headsMin = 0, headsMax = NUM_MP_HEADS;
							ImGui::SliderScalar("Character Head", ImGuiDataType_U8, &mpSetup->botsData[s].headIndex, &headsMin, &headsMax, "%u");

							const ImU8 bodyMin = 0, bodyMax = NUM_MP_BODIES;
							ImGui::SliderScalar("Character Body", ImGuiDataType_U8, &mpSetup->botsData[s].bodyIndex, &bodyMin, &bodyMax, "%u");

							if ((SimulantDifficulties)difficulty == SimulantDifficulties::DISABLED) ImGui::EndDisabled();

							ImGui::EndTabItem();
						}
					}

					ImGui::EndTabBar();
				}*/

				ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuter;

				if (ImGui::BeginTable("SimulantsTable", 6, flags))
				{
					ImGui::TableSetupColumn("##SimulantNumber");
					ImGui::TableSetupColumn("Difficulty");
					ImGui::TableSetupColumn("Type");
					ImGui::TableSetupColumn("Team");
					ImGui::TableSetupColumn("Character Head");
					ImGui::TableSetupColumn("Character Body");

					ImGui::TableHeadersRow();

					constexpr float columnWidth = 110;

					for (uint8_t s = 0; s < MAX_SIMULANTS; s++)
					{
						ImGui::PushID(s);

						ImGui::TableNextRow();

						ImGui::TableSetColumnIndex(0);
						ImGui::Text("Simulant %u", s + 1);

						ImGui::TableSetColumnIndex(1);

						ImGui::PushItemWidth(columnWidth);
						int difficulty = mpSetup->botsData[s].difficulty;
						if (ImGui::Combo("##Difficulty", &difficulty, mpSimulantDifficultyNames, NUM_MP_SIMULANT_DIFFICULTIES + 1))
						{
							mpSetup->botsData[s].difficulty = difficulty;
						}
						ImGui::PopItemWidth();

						ImGui::TableSetColumnIndex(2);

						if ((SimulantDifficulties)difficulty == SimulantDifficulties::Disabled) ImGui::BeginDisabled();

						ImGui::PushItemWidth(columnWidth);
						int type = mpSetup->botsData[s].type;
						if (ImGui::Combo("##Type", &type, mpSimulantTypeNames, NUM_MP_SIMULANT_TYPES))
						{
							mpSetup->botsData[s].type = type;
						}
						ImGui::PopItemWidth();

						ImGui::TableSetColumnIndex(3);

						ImGui::PushItemWidth(columnWidth);
						int team = mpSetup->botsData[s].team;
						if (ImGui::Combo("##Team", &team, teamNames, NUM_MP_TEAMS))
						{
							mpSetup->botsData[s].team = team;
						}
						ImGui::PopItemWidth();

						ImGui::TableSetColumnIndex(4);

						ImGui::PushItemWidth(columnWidth);
						const ImU8 headsMin = 0, headsMax = NUM_MP_HEADS - 1;
						ImGui::SliderScalar("##Character Head", ImGuiDataType_U8, &mpSetup->botsData[s].headIndex, &headsMin, &headsMax, "%u");
						ImGui::PopItemWidth();

						ImGui::TableSetColumnIndex(5);

						ImGui::PushItemWidth(columnWidth + 95);
						const ImU8 bodyMin = 0, bodyMax = NUM_MP_BODIES - 1;
						char bodyName[64];
						snprintf(bodyName, 64, "%u (%s)", mpSetup->botsData[s].bodyIndex, mpBodyNames[mpSetup->botsData[s].bodyIndex]);
						ImGui::SliderScalar("##Character Body", ImGuiDataType_U8, &mpSetup->botsData[s].bodyIndex, &bodyMin, &bodyMax, bodyName);
						ImGui::PopItemWidth();

						if ((SimulantDifficulties)difficulty == SimulantDifficulties::Disabled) ImGui::EndDisabled();

						ImGui::PopID();
					}

					ImGui::EndTable();
				}


				if (ImGui::BeginTable("MpWeaponsLimitsTeamsTable", 3, 0))
				{
					ImGui::TableSetupColumn("Column1", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableSetupColumn("Column2", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableSetupColumn("Column3", ImGuiTableColumnFlags_WidthStretch);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);

					PrintHeader("Weapons");

					const char* wNames[NUM_MP_WEAPONS_PC] = {};
					for (uint8_t w = 0; w < NUM_MP_WEAPONS_PC; w++) wNames[w] = weaponNames[mpWeaponNameIndicesPC[w]];

					for (uint8_t ws = 0; ws < NUM_MP_WEAPONSLOTS; ws++)
					{
						char weaponSlotLabel[16];
						snprintf(weaponSlotLabel, 16, "Weapon %u", ws + 1);

						int weapon = mpSetup->weaponSlots[ws];
						if (ImGui::Combo(weaponSlotLabel, &weapon, wNames, NUM_MP_WEAPONS_PC))
						{
							mpSetup->weaponSlots[ws] = weapon;
						}
					}

					ImGui::TableSetColumnIndex(1);
					PrintHeader("Limits");

					const ImU8 timeMin = 1, timeMax = 61;
					uint8_t timelimit = mpSetup->timelimit + 1;
					if (ImGui::SliderScalar("Time (min)", ImGuiDataType_U8, &timelimit, &timeMin, &timeMax, timelimit > 60 ? "No Limit" : "%u"))
					{
						mpSetup->timelimit = timelimit - 1;
					}

					const ImU8 scoreMin = 1, scoreMax = 101;
					uint8_t scorelimit = mpSetup->scorelimit + 1;
					if (ImGui::SliderScalar("Score", ImGuiDataType_U8, &scorelimit, &scoreMin, &scoreMax, scorelimit > 100 ? "No Limit" : "%u"))
					{
						mpSetup->scorelimit = scorelimit - 1;
					}

					const ImU16 teamScoreMin = 1, teamScoreMax = 401;
					uint16_t teamscorelimit = mpSetup->teamscorelimit + 1;
					if (ImGui::SliderScalar("Team Score", ImGuiDataType_U16, &teamscorelimit, &teamScoreMin, &teamScoreMax, teamscorelimit > 400 ? "No Limit" : "%u"))
					{
						mpSetup->teamscorelimit = teamscorelimit - 1;
					}

					ImGui::TableSetColumnIndex(2);
					PrintHeader("Player Teams");

					CheckboxMpSetupOptionsFlags(mpSetup, "Teams Enabled", MultiplayerSetupFlags::TEAMSENABLED);

					if (!mpSetup->GetOptionsFlag(MultiplayerSetupFlags::TEAMSENABLED)) ImGui::BeginDisabled();

					for (uint8_t p = 0; p < MAX_PLAYERS; p++)
					{
						ImGui::PushID(p);

						char playerName[16];
						snprintf(playerName, 16, "Player %u", p + 1);

						int team = mpSetup->teams[p];
						if (ImGui::Combo(playerName, &team, teamNames, NUM_MP_TEAMS))
						{
							mpSetup->teams[p] = team;
						}

						ImGui::PopID();
					}

					if (!mpSetup->GetOptionsFlag(MultiplayerSetupFlags::TEAMSENABLED)) ImGui::EndDisabled();

					ImGui::EndTable();
				}

				ImGui::EndChild();
				ImGui::EndTabItem();
			}
		}

		ImGui::EndTabBar();
	}
}

bool SaveEditorUI::CheckboxProgressFlags(GameFile* gameFile, const char* label, const SinglePlayerFlags flag, const bool reverse) const
{
	bool value = gameFile->GetFlag(flag);
	if (reverse) value = !value;

	if (ImGui::Checkbox(label, &value))
	{
		if (reverse) value = !value;
		gameFile->SetFlag(flag, value);
	}

	return value;
}

bool SaveEditorUI::CheckboxMpProfileOptionsFlags(MultiplayerProfile* mpProfile, const char* label, const MultiplayerOptionsFlags flag, const bool reverse) const
{
	bool value = mpProfile->GetOptionsFlag(flag);
	if (reverse) value = !value;

	if (ImGui::Checkbox(label, &value))
	{
		if (reverse) value = !value;
		mpProfile->SetOptionsFlag(flag, value);
	}

	return value;
}

bool SaveEditorUI::CheckboxMpProfileDisplayOptionsFlags(MultiplayerProfile* mpProfile, const char* label, const MultiplayerDisplayOptionsFlags flag, const bool reverse) const
{
	bool value = mpProfile->GetDisplayOptionsFlag(flag);
	if (reverse) value = !value;

	if (ImGui::Checkbox(label, &value))
	{
		if (reverse) value = !value;
		mpProfile->SetDisplayOptionsFlag(flag, value);
	}

	return value;
}

bool SaveEditorUI::CheckboxMpSetupOptionsFlags(MultiplayerSetup* mpSetup, const char* label, const MultiplayerSetupFlags flag, const bool reverse) const
{
	bool value = mpSetup->GetOptionsFlag(flag);
	if (reverse) value = !value;

	if (ImGui::Checkbox(label, &value))
	{
		if (reverse) value = !value;
		mpSetup->SetOptionsFlag(flag, value);
	}

	return value;
}

void SaveEditorUI::NameInputField(const char* label, char* name) const
{
	char cleanedName[MAX_NAME_LENGTH + 1] = {};
	char previousName[MAX_NAME_LENGTH + 1] = {};

	// Workaround for Spanish team name brown, the only name with special characters
	if (strcmp("Marrón", name) == 0) snprintf(cleanedName, MAX_NAME_LENGTH + 1, "Marron");
	else strcpy(cleanedName, name);

	strcpy(previousName, cleanedName);

	if (ImGui::InputText(label, cleanedName, MAX_NAME_LENGTH + 1))
	{
		bool valid = true;
		for (uint8_t c = 0; c < MAX_NAME_LENGTH + 1; c++)
		{
			if (cleanedName[c] == '\0')
			{
				if (c == 0) valid = false; // Name is empty
				break;
			}

			uint8_t val = (uint8_t)cleanedName[c];

			//      Space            !              .                      0-9                     ?                       A-Z                             a-z
			if (!(val == 0x20 || val == 0x21 || val == 0x2e || (val >= 0x30 && val <= 0x39) || val == 0x3f || (val >= 0x41 && val <= 0x5a) || (val >= 0x61 && val <= 0x7a)))
			{
				valid = false;
				break;
			}
		}

		if (valid)
		{
			memset(name, 0, MAX_NAME_LENGTH + 1);
			strcpy(name, cleanedName);
		}
	}
}

void SaveEditorUI::InputScalarU16(const char* label, uint16_t* value, const uint8_t bits) const
{
	uint32_t value32 = *value;
	if (ImGui::InputScalar(label, ImGuiDataType_U32, &value32, NULL, NULL))
	{
		uint16_t mask = (uint16_t)(((uint32_t)1 << bits) - 1);
		if (value32 > mask) value32 = mask;
		*value = (uint16_t)value32;
	}
}

void SaveEditorUI::InputScalarU32(const char* label, uint32_t* value, const uint8_t bits) const
{
	uint64_t value64 = *value;
	if (ImGui::InputScalar(label, ImGuiDataType_U64, &value64, NULL, NULL))
	{
		uint32_t mask = (uint32_t)(((uint64_t)1 << bits) - 1);
		if (value64 > mask) value64 = mask;
		*value = (uint32_t)value64;
	}
}

void SaveEditorUI::PrintEmptySlot() const
{
	const char emptyText[] = "The slot is empty.";
	ImVec2 size = ImGui::CalcTextSize(emptyText);
	ImVec2 windowSize = ImGui::GetWindowSize();
	ImGui::SetCursorPosX((windowSize.x / 2.0f) - (size.x / 2.0f));
	ImGui::SetCursorPosY((windowSize.y / 2.0f) - (size.y / 2.0f));
	ImGui::Text("%s", emptyText);
}

void SaveEditorUI::PrintHeader(const char* label) const
{
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.5f, 0.6f, 1.0f));
	ImGui::SeparatorText(label);
	ImGui::PopStyleColor();
}