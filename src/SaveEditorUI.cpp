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
			for (int s = 0; s < ACTUAL_NUM_SAVE_SLOTS; s++)
			{
				if (ImGui::BeginTabItem(tabNames[s]))
				{
					SaveSlot* saveSlot = saveFile->GetSaveSlot(s);

					if (!saveSlot)
					{
						const char emptyText[] = "The slot is empty.";
						ImVec2 size = ImGui::CalcTextSize(emptyText);
						ImVec2 windowSize = ImGui::GetWindowSize();
						ImGui::SetCursorPosX((windowSize.x / 2.0f) - (size.x / 2.0f));
						ImGui::SetCursorPosY((windowSize.y / 2.0f) - (size.y / 2.0f));
						ImGui::Text("%s", emptyText);
					}
					else
					{
						PrintChecksum(saveSlot->GetChecksum(saveData.NeedsEndianSwap()));

						if (ImGui::BeginTabBar("Sections", tab_bar_flags))
						{
							RenderLevelDataSection(saveData, saveSlot);
							RenderAbilitiesItemsSection(saveData, saveSlot);
							RenderProgressFlagsSection(saveData, saveSlot);

							ImGui::EndTabBar();
						}
					}

					ImGui::EndTabItem();
				}
			}

			RenderGlobalDataSection(saveData, saveFile);

			ImGui::EndTabBar();
		}
	}
	ImGui::End();
}

void SaveEditorUI::RenderLevelDataSection(const SaveData& saveData, SaveSlot* saveSlot)
{
	if (!ImGui::BeginTabItem("Level Data")) return;

	ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuter;

	if (ImGui::BeginTable("LevelsTable", 6, flags))
	{
		uint8_t totalJiggies = 0;
		uint8_t totalHoneycombs = 0;
		uint16_t totalNotes = 0;
		uint32_t totalPlayTime = 0;

		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableSetupColumn("#");
		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Jiggies");
		ImGui::TableSetupColumn("Honeycombs");
		ImGui::TableSetupColumn("Note Score");
		ImGui::TableSetupColumn("Play Time (s)");

		ImGui::TableHeadersRow();

		for (int l = 0; l < TOTAL_LEVEL_COUNT; l++)
		{
			ImGui::PushID(l);

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%i", l + 1);

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%s", levelNames[l]);

			ImGui::TableSetColumnIndex(2);

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 0));

			for (int j = levelJiggiesIndexRanges[l].min; j <= levelJiggiesIndexRanges[l].max; j++)
			{
				if (levelJiggiesIndexRanges[l].Count() == 0) continue;

				ImGui::PushID(j);

				bool value = saveSlot->GetJiggy(j);
				if (ImGui::Checkbox("##Jiggy", &value))
				{
					saveSlot->SetJiggy(j, value);
					saveSlot->UpdateChecksum(saveData.NeedsEndianSwap());
				}

				if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNone))
					ImGui::SetTooltip("%s", levelJiggiesNames[j]);

				if (value) totalJiggies++;

				ImGui::PopID();

				if (j < levelJiggiesIndexRanges[l].max) ImGui::SameLine();
			}

			ImGui::PopStyleVar();

			ImGui::TableSetColumnIndex(3);

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 0));

			for (int h = levelHoneycombsIndexRanges[l].min; h <= levelHoneycombsIndexRanges[l].max; h++)
			{
				if (levelHoneycombsIndexRanges[l].Count() == 0) continue;

				ImGui::PushID(h);

				bool value = saveSlot->GetHoneycomb(h);
				if (ImGui::Checkbox("##Honeycomb", &value))
				{
					saveSlot->SetHoneycomb(h, value);
					saveSlot->UpdateChecksum(saveData.NeedsEndianSwap());
				}

				if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNone))
					ImGui::SetTooltip("%s", levelHoneycombsNames[h]);

				if (value) totalHoneycombs++;

				ImGui::PopID();

				if (h < levelHoneycombsIndexRanges[l].max) ImGui::SameLine();
			}

			ImGui::PopStyleVar();

			ImGui::TableSetColumnIndex(4);

			if (levelHasNotes[l])
			{
				uint8_t noteValue = saveSlot->GetNotes(l);
				if (ImGui::InputScalar("##Notes Score", ImGuiDataType_U8, &noteValue, NULL, NULL, "%u"))
				{
					if (noteValue > MAX_NOTES_PER_LEVEL) noteValue = MAX_NOTES_PER_LEVEL;
					saveSlot->SetNotes(l, noteValue);
					saveSlot->UpdateChecksum(saveData.NeedsEndianSwap());
				}

				totalNotes += noteValue;
			}

			ImGui::TableSetColumnIndex(5);

			uint16_t time = saveSlot->GetPlayTime(l, saveData.NeedsEndianSwap());
			if (ImGui::InputScalar("##Play Time", ImGuiDataType_U16, &time, NULL, NULL, "%u"))
			{
				saveSlot->SetPlayTime(l, time, saveData.NeedsEndianSwap());
				saveSlot->UpdateChecksum(saveData.NeedsEndianSwap());
			}

			if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNone))
				ImGui::SetTooltip("%s", Utils::GetTimeString(time).c_str());

			totalPlayTime += time;

			ImGui::PopID();
		}

		// Totals -----------------------------------------------

		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		ImGui::Text("--");

		ImGui::TableSetColumnIndex(1);
		ImGui::Text("Totals");

		ImGui::TableSetColumnIndex(2);
		ImGui::Text("%u / %u", totalJiggies, JIGGIES_COUNT);

		ImGui::TableSetColumnIndex(3);
		ImGui::Text("%u / %u", totalHoneycombs, HONEYCOMB_COUNT);

		ImGui::TableSetColumnIndex(4);
		ImGui::Text("%u / %u", totalNotes, NOTES_COUNT);

		ImGui::TableSetColumnIndex(5);
		ImGui::Text("%u", totalPlayTime);

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNone))
			ImGui::SetTooltip("%s", Utils::GetTimeString(totalPlayTime).c_str());

		ImGui::EndTable();
	}

	if (ImGui::BeginTable("MumboTokensTable", 3, flags))
	{
		uint32_t totalMumboTokens = 0;

		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableSetupColumn("#");
		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Mumbo Tokens");

		ImGui::TableHeadersRow();

		for (int l = 1; l < TOTAL_LEVEL_COUNT; l++)
		{
			ImGui::PushID(l);

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%i", l + 1);

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%s", levelNames[l]);

			ImGui::TableSetColumnIndex(2);

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 0));

			for (int t = levelMumboTokensIndexRanges[l].min; t <= levelMumboTokensIndexRanges[l].max; t++)
			{
				ImGui::PushID(t);

				bool value = saveSlot->GetMumboToken(t);
				if (ImGui::Checkbox("##Token", &value))
				{
					saveSlot->SetMumboToken(t, value);
					saveSlot->UpdateChecksum(saveData.NeedsEndianSwap());
				}

				if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNone))
					ImGui::SetTooltip("%s", MumboTokenNames[t]);

				if (value) totalMumboTokens++;

				ImGui::PopID();

				if (t < levelMumboTokensIndexRanges[l].max) ImGui::SameLine();
			}

			ImGui::PopStyleVar();

			ImGui::PopID();
		}

		// Totals -----------------------------------------------

		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		ImGui::Text("--");

		ImGui::TableSetColumnIndex(1);
		ImGui::Text("Totals");

		ImGui::TableSetColumnIndex(2);
		ImGui::Text("%u / %u", totalMumboTokens, ACTUAL_MUMBO_TOKEN_COUNT);

		ImGui::EndTable();
	}

	ImGui::EndTabItem();
}

void SaveEditorUI::RenderAbilitiesItemsSection(const SaveData& saveData, SaveSlot* saveSlot)
{
	if (!ImGui::BeginTabItem("Abilities / Items")) return;

	ImGui::SeparatorText("Learned / Used Abilities");

	if (ImGui::BeginTable("AbilitiesTable", 4, 0))
	{
		ImGui::TableSetupColumn("Column1", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Column2", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Column3", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Column4", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);

		CheckboxAbility(saveData, saveSlot, "Barge", Abilities::ABILITY_0_BARGE);
		CheckboxAbility(saveData, saveSlot, "Beak Bomb", Abilities::ABILITY_1_BEAK_BOMB);
		CheckboxAbility(saveData, saveSlot, "Beak Buster", Abilities::ABILITY_2_BEAK_BUSTER);
		CheckboxAbility(saveData, saveSlot, "Camera Control", Abilities::ABILITY_3_CAMERA_CONTROL);
		CheckboxAbility(saveData, saveSlot, "Claw Swipe", Abilities::ABILITY_4_CLAW_SWIPE);

		ImGui::TableSetColumnIndex(1);

		CheckboxAbility(saveData, saveSlot, "Climb", Abilities::ABILITY_5_CLIMB);
		CheckboxAbility(saveData, saveSlot, "Eggs", Abilities::ABILITY_6_EGGS);
		CheckboxAbility(saveData, saveSlot, "Feathery Flap", Abilities::ABILITY_7_FEATHERY_FLAP);
		CheckboxAbility(saveData, saveSlot, "Flap Flip", Abilities::ABILITY_8_FLAP_FLIP);
		CheckboxAbility(saveData, saveSlot, "Flight", Abilities::ABILITY_9_FLIGHT);

		ImGui::TableSetColumnIndex(2);

		CheckboxAbility(saveData, saveSlot, "Jump Higher", Abilities::ABILITY_A_HOLD_A_JUMP_HIGHER);
		CheckboxAbility(saveData, saveSlot, "Ratatat Rap", Abilities::ABILITY_B_RATATAT_RAP);
		CheckboxAbility(saveData, saveSlot, "Roll", Abilities::ABILITY_C_ROLL);
		CheckboxAbility(saveData, saveSlot, "Shock Jump", Abilities::ABILITY_D_SHOCK_JUMP);
		CheckboxAbility(saveData, saveSlot, "Wading Boots", Abilities::ABILITY_E_WADING_BOOTS);

		ImGui::TableSetColumnIndex(3);

		CheckboxAbility(saveData, saveSlot, "Dive", Abilities::ABILITY_F_DIVE);
		CheckboxAbility(saveData, saveSlot, "Talon Trot", Abilities::ABILITY_10_TALON_TROT);
		CheckboxAbility(saveData, saveSlot, "Turbo Talon", Abilities::ABILITY_11_TURBO_TALON);
		CheckboxAbility(saveData, saveSlot, "Wonderwing", Abilities::ABILITY_12_WONDERWING);
		CheckboxAbility(saveData, saveSlot, "Open Notedoors", Abilities::ABILITY_13_1ST_NOTEDOOR);

		ImGui::EndTable();
	}

	ImGui::SeparatorText("Held Items");

	ImGui::PushItemWidth(40);

	uint8_t mumboTokens = saveSlot->GetHeldItem(HeldItems::MumboTokens);
	if (ImGui::InputScalar("Mumbo Tokens", ImGuiDataType_U8, &mumboTokens, NULL, NULL, "%u"))
	{
		if (mumboTokens > MAX_MUMBO_TOKENS) mumboTokens = MAX_MUMBO_TOKENS;
		saveSlot->SetHeldItem(HeldItems::MumboTokens, mumboTokens);
		saveSlot->UpdateChecksum(saveData.NeedsEndianSwap());
	}

	uint8_t eggs = saveSlot->GetHeldItem(HeldItems::Eggs);
	if (ImGui::InputScalar("Eggs", ImGuiDataType_U8, &eggs, NULL, NULL, "%u"))
	{
		if (eggs > MAX_EGGS) eggs = MAX_EGGS;
		saveSlot->SetHeldItem(HeldItems::Eggs, eggs);
		saveSlot->UpdateChecksum(saveData.NeedsEndianSwap());
	}

	uint8_t redFeathers = saveSlot->GetHeldItem(HeldItems::RedFeathers);
	if (ImGui::InputScalar("Red Feathers", ImGuiDataType_U8, &redFeathers, NULL, NULL, "%u"))
	{
		if (redFeathers > MAX_RED_FEATHERS) redFeathers = MAX_RED_FEATHERS;
		saveSlot->SetHeldItem(HeldItems::RedFeathers, redFeathers);
		saveSlot->UpdateChecksum(saveData.NeedsEndianSwap());
	}

	uint8_t goldFeathers = saveSlot->GetHeldItem(HeldItems::GoldFeathers);
	if (ImGui::InputScalar("Gold Feathers", ImGuiDataType_U8, &goldFeathers, NULL, NULL, "%u"))
	{
		if (goldFeathers > MAX_GOLD_FEATHERS) goldFeathers = MAX_GOLD_FEATHERS;
		saveSlot->SetHeldItem(HeldItems::GoldFeathers, goldFeathers);
		saveSlot->UpdateChecksum(saveData.NeedsEndianSwap());
	}

	uint8_t jiggies = saveSlot->GetHeldItem(HeldItems::Jiggies);
	if (ImGui::InputScalar("Jiggies", ImGuiDataType_U8, &jiggies, NULL, NULL, "%u"))
	{
		if (jiggies > JIGGIES_COUNT) jiggies = JIGGIES_COUNT;
		saveSlot->SetHeldItem(HeldItems::Jiggies, jiggies);
		saveSlot->UpdateChecksum(saveData.NeedsEndianSwap());
	}

	ImGui::PopItemWidth();

	ImGui::EndTabItem();
}

void SaveEditorUI::RenderProgressFlagsSection(const SaveData& saveData, SaveSlot* saveSlot)
{
	if (!ImGui::BeginTabItem("Progress Flags")) return;

	ImGuiTableFlags tableFlags = ImGuiTableFlags_ScrollY;
	if (ImGui::BeginTable("FlagsTable", 4, tableFlags))
	{
		ImGui::TableSetupColumn("Column1", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Column2", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Column3", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Column4", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);

		BeginProgressFlagsGroup("Unlocked Doors");
		CheckboxProgressFlags(saveData, saveSlot, "50 Note Door", ProgressFlags::FILEPROG_3A_NOTE_DOOR_50_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "180 Note Door", ProgressFlags::FILEPROG_3B_NOTE_DOOR_180_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "260 Note Door", ProgressFlags::FILEPROG_3C_NOTE_DOOR_260_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "350 Note Door", ProgressFlags::FILEPROG_3D_NOTE_DOOR_350_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "450 Note Door", ProgressFlags::FILEPROG_3E_NOTE_DOOR_450_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "640 Note Door", ProgressFlags::FILEPROG_3F_NOTE_DOOR_640_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "765 Note Door", ProgressFlags::FILEPROG_40_NOTE_DOOR_765_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "810 Note Door", ProgressFlags::FILEPROG_41_NOTE_DOOR_810_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "828 Note Door", ProgressFlags::FILEPROG_42_NOTE_DOOR_828_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "846 Note Door", ProgressFlags::FILEPROG_43_NOTE_DOOR_846_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "864 Note Door", ProgressFlags::FILEPROG_44_NOTE_DOOR_864_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "882 Note Door", ProgressFlags::FILEPROG_45_NOTE_DOOR_882_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "Grunty's Door", ProgressFlags::FILEPROG_E2_DOOR_OF_GRUNTY_OPEN);
		EndProgressFlagsGroup();

		BeginProgressFlagsGroup("Unlocked Grates");
		CheckboxProgressFlags(saveData, saveSlot, "To BGS Puzzle", ProgressFlags::FILEPROG_1E_LAIR_GRATE_TO_BGS_PUZZLE_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "To MMM Puzzle", ProgressFlags::FILEPROG_CE_GRATE_TO_MMM_PUZZLE_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "To RBB Puzzle", ProgressFlags::FILEPROG_C2_GRATE_TO_RBB_PUZZLE_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "To Water Switch 3", ProgressFlags::FILEPROG_CD_GRATE_TO_WATER_SWITCH_3_OPEN);
		EndProgressFlagsGroup();

		BeginProgressFlagsGroup("Cutscenes");
		CheckboxProgressFlags(saveData, saveSlot, "Enter Lair", ProgressFlags::FILEPROG_BD_ENTER_LAIR_CUTSCENE);
		CheckboxProgressFlags(saveData, saveSlot, "MMM Dinning Room", ProgressFlags::FILEPROG_1D_MMM_DINNING_ROOM_CUTSCENE);
		CheckboxProgressFlags(saveData, saveSlot, "Enter Furnace Fun", ProgressFlags::FILEPROG_F4_ENTER_FF_CUTSCENE);
		EndProgressFlagsGroup();

		BeginProgressFlagsGroup("Jiggies Placed");
		InputProgressFlags(saveData, saveSlot, "MM Picture", ProgressFlags::FILEPROG_5D_MM_PUZZLE_PIECES_PLACED, 1, 1);
		InputProgressFlags(saveData, saveSlot, "TTC Picture", ProgressFlags::FILEPROG_5E_TCC_PUZZLE_PIECES_PLACED, 2, 2);
		InputProgressFlags(saveData, saveSlot, "CC Picture", ProgressFlags::FILEPROG_60_CC_PUZZLE_PIECES_PLACED, 3, 5);
		InputProgressFlags(saveData, saveSlot, "BGS Picture", ProgressFlags::FILEPROG_63_BGS_PUZZLE_PIECES_PLACED, 3, 7);
		InputProgressFlags(saveData, saveSlot, "FP Picture", ProgressFlags::FILEPROG_66_FP_PUZZLE_PIECES_PLACED, 4, 8);
		InputProgressFlags(saveData, saveSlot, "GV Picture", ProgressFlags::FILEPROG_6A_GV_PUZZLE_PIECES_PLACED, 4, 9);
		InputProgressFlags(saveData, saveSlot, "MMM Picture", ProgressFlags::FILEPROG_6E_MMM_PUZZLE_PIECES_PLACED, 4, 10);
		InputProgressFlags(saveData, saveSlot, "RBB Picture", ProgressFlags::FILEPROG_72_RBB_PUZZLE_PIECES_PLACED, 4, 12);
		InputProgressFlags(saveData, saveSlot, "CCW Picture", ProgressFlags::FILEPROG_76_CCW_PUZZLE_PIECES_PLACED, 4, 15);
		InputProgressFlags(saveData, saveSlot, "Grunty Picture", ProgressFlags::FILEPROG_7A_DOG_PUZZLE_PIECES_PLACED, 5, 25);
		InputProgressFlags(saveData, saveSlot, "Honeycomb Picture", ProgressFlags::FILEPROG_7F_DOUBLE_HEALTH_PUZZLE_PIECES_PLACED, 3, 4);
		EndProgressFlagsGroup();

		BeginProgressFlagsGroup("Cauldrons");
		CheckboxProgressFlags(saveData, saveSlot, "Pink 1 Active", ProgressFlags::FILEPROG_49_PINK_CAULDRON_1_ACTIVE);
		CheckboxProgressFlags(saveData, saveSlot, "Pink 2 Active", ProgressFlags::FILEPROG_4A_PINK_CAULDRON_2_ACTIVE);
		CheckboxProgressFlags(saveData, saveSlot, "Green 1 Active", ProgressFlags::FILEPROG_4B_GREEN_CAULDRON_1_ACTIVE);
		CheckboxProgressFlags(saveData, saveSlot, "Green 2 Active", ProgressFlags::FILEPROG_4C_GREEN_CAULDRON_2_ACTIVE);
		CheckboxProgressFlags(saveData, saveSlot, "Red 1 Active", ProgressFlags::FILEPROG_4D_RED_CAULDRON_1_ACTIVE);
		CheckboxProgressFlags(saveData, saveSlot, "Red 2 Active", ProgressFlags::FILEPROG_4E_RED_CAULDRON_2_ACTIVE);
		CheckboxProgressFlags(saveData, saveSlot, "Yellow 1 Active", ProgressFlags::FILEPROG_51_YELLOW_CAULDRON_1_ACTIVE);
		CheckboxProgressFlags(saveData, saveSlot, "Yellow 2 Active", ProgressFlags::FILEPROG_52_YELLOW_CAULDRON_2_ACTIVE);
		CheckboxProgressFlags(saveData, saveSlot, "Unused? 1 Active", ProgressFlags::FILEPROG_4F_UNUSED_CAULDRON_1_ACTIVE);
		CheckboxProgressFlags(saveData, saveSlot, "Unused? 2 Active", ProgressFlags::FILEPROG_50_UNUSED_CAULDRON_2_ACTIVE);
		CheckboxProgressFlags(saveData, saveSlot, "Unlocked Cauldron Warp", ProgressFlags::FILEPROG_F5_COMPLETED_A_WARP_CAULDRON_SET);
		EndProgressFlagsGroup();

		BeginProgressFlagsGroup("Mumbo Jumbo");
		CheckboxProgressFlags(saveData, saveSlot, "Has Met Mumbo", ProgressFlags::FILEPROG_11_HAS_MET_MUMBO);
		CheckboxProgressFlags(saveData, saveSlot, "Has Transformed Before", ProgressFlags::FILEPROG_12_HAS_TRANSFORMED_BEFORE);
		CheckboxProgressFlags(saveData, saveSlot, "Had Enough Tokens Before", ProgressFlags::FILEPROG_DC_HAS_HAD_ENOUGH_TOKENS_BEFORE);
		CheckboxProgressFlags(saveData, saveSlot, "Paid Termite Cost", ProgressFlags::FILEPROG_90_PAID_TERMITE_COST);
		CheckboxProgressFlags(saveData, saveSlot, "Paid Croc Cost", ProgressFlags::FILEPROG_93_PAID_CROC_COST);
		CheckboxProgressFlags(saveData, saveSlot, "Paid Walrus Cost", ProgressFlags::FILEPROG_92_PAID_WALRUS_COST);
		CheckboxProgressFlags(saveData, saveSlot, "Paid Pumpkin Cost", ProgressFlags::FILEPROG_91_PAID_PUMPKIN_COST);
		CheckboxProgressFlags(saveData, saveSlot, "Paid Bee Cost", ProgressFlags::FILEPROG_94_PAID_BEE_COST);
		InputProgressFlags(saveData, saveSlot, "Mistakes Index", ProgressFlags::FILEPROG_BB_MUMBO_MISTAKE_INDEX, 2, 3);
		EndProgressFlagsGroup();

		ImGui::TableSetColumnIndex(1);

		BeginProgressFlagsGroup("Stats");
		CheckboxProgressFlags(saveData, saveSlot, "Double Health", ProgressFlags::FILEPROG_B9_DOUBLE_HEALTH);
		CheckboxProgressFlags(saveData, saveSlot, "Cheato Blue Eggs", ProgressFlags::FILEPROG_BE_CHEATO_BLUEEGGS);
		CheckboxProgressFlags(saveData, saveSlot, "Cheato Red Feathers", ProgressFlags::FILEPROG_BF_CHEATO_REDFEATHERS);
		CheckboxProgressFlags(saveData, saveSlot, "Cheato Gold Feathers", ProgressFlags::FILEPROG_C0_CHEATO_GOLDFEATHERS);
		EndProgressFlagsGroup();

		BeginProgressFlagsGroup("Unlocked Worlds");
		CheckboxProgressFlags(saveData, saveSlot, "Mumbo's Mountain", ProgressFlags::FILEPROG_31_MM_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "Treasure Trove Cove", ProgressFlags::FILEPROG_32_TTC_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "Clanker's Cavern", ProgressFlags::FILEPROG_33_CC_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "Bubblegloop Swamp", ProgressFlags::FILEPROG_34_BGS_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "Freezeezy Peak", ProgressFlags::FILEPROG_35_FP_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "Gobi's Valley", ProgressFlags::FILEPROG_36_GV_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "Mad Monster Mansion", ProgressFlags::FILEPROG_37_MMM_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "Rusty Bucket Bay", ProgressFlags::FILEPROG_38_RBB_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "Click Clock Wood", ProgressFlags::FILEPROG_39_CCW_OPEN);
		EndProgressFlagsGroup();

		BeginProgressFlagsGroup("Entered Worlds");
		CheckboxProgressFlags(saveData, saveSlot, "Mumbo's Mountain##Entered", ProgressFlags::FILEPROG_B0_HAS_ENTERED_MM);
		CheckboxProgressFlags(saveData, saveSlot, "Treasure Trove Cove", ProgressFlags::FILEPROG_B2_HAS_ENTERED_TTC);
		CheckboxProgressFlags(saveData, saveSlot, "Clanker's Cavern", ProgressFlags::FILEPROG_B8_HAS_ENTERED_CC);
		CheckboxProgressFlags(saveData, saveSlot, "Bubblegloop Swamp", ProgressFlags::FILEPROG_B1_HAS_ENTERED_BGS);
		CheckboxProgressFlags(saveData, saveSlot, "Freezeezy Peak", ProgressFlags::FILEPROG_B6_HAS_ENTERED_FP);
		CheckboxProgressFlags(saveData, saveSlot, "Gobi's Valley", ProgressFlags::FILEPROG_B3_HAS_ENTERED_GV);
		CheckboxProgressFlags(saveData, saveSlot, "Mad Monster Mansion", ProgressFlags::FILEPROG_B7_HAS_ENTERED_MMM);
		CheckboxProgressFlags(saveData, saveSlot, "Rusty Bucket Bay", ProgressFlags::FILEPROG_B4_HAS_ENTERED_RBB);
		CheckboxProgressFlags(saveData, saveSlot, "Click Clock Wood", ProgressFlags::FILEPROG_B5_HAS_ENTERED_CCW);
		CheckboxProgressFlags(saveData, saveSlot, "Final Fight", ProgressFlags::FILEPROG_CF_HAS_ENTERED_FINAL_FIGHT);
		EndProgressFlagsGroup();

		BeginProgressFlagsGroup("Texts Read");
		CheckboxProgressFlags(saveData, saveSlot, "Music Note", ProgressFlags::FILEPROG_3_MUSIC_NOTE_TEXT);
		CheckboxProgressFlags(saveData, saveSlot, "Mumbo Token", ProgressFlags::FILEPROG_4_MUMBO_TOKEN_TEXT);
		CheckboxProgressFlags(saveData, saveSlot, "Blue Egg", ProgressFlags::FILEPROG_5_BLUE_EGG_TEXT);
		CheckboxProgressFlags(saveData, saveSlot, "Red Feather", ProgressFlags::FILEPROG_6_RED_FEATHER_TEXT);
		CheckboxProgressFlags(saveData, saveSlot, "Gold Feather", ProgressFlags::FILEPROG_7_GOLD_FEATHER_TEXT);
		CheckboxProgressFlags(saveData, saveSlot, "Honeycomb", ProgressFlags::FILEPROG_A_HONEYCOMB_TEXT);
		CheckboxProgressFlags(saveData, saveSlot, "Empty Honeycomb", ProgressFlags::FILEPROG_B_EMPTY_HONEYCOMB_TEXT);
		CheckboxProgressFlags(saveData, saveSlot, "Extra Life", ProgressFlags::FILEPROG_C_EXTRA_LIFE_TEXT);
		CheckboxProgressFlags(saveData, saveSlot, "Jinjo", ProgressFlags::FILEPROG_E_JINJO_TEXT);
		CheckboxProgressFlags(saveData, saveSlot, "Beehive", ProgressFlags::FILEPROG_D_BEEHIVE_TEXT);
		CheckboxProgressFlags(saveData, saveSlot, "Orange", ProgressFlags::FILEPROG_8_ORANGE_TEXT);
		CheckboxProgressFlags(saveData, saveSlot, "Gold Bullion", ProgressFlags::FILEPROG_9_GOLD_BULLION_TEXT);
		CheckboxProgressFlags(saveData, saveSlot, "Enter MMM", ProgressFlags::FILEPROG_15_ENTER_MMM_TEXT);
		CheckboxProgressFlags(saveData, saveSlot, "Magic Weak", ProgressFlags::FILEPROG_83_MAGIC_GET_WEAK_TEXT);
		CheckboxProgressFlags(saveData, saveSlot, "Magic Gone", ProgressFlags::FILEPROG_84_MAGIC_ALL_GONE_TEXT);
		CheckboxProgressFlags(saveData, saveSlot, "Entered Lair", ProgressFlags::FILEPROG_97_ENTERED_LAIR_TEXT);
		CheckboxProgressFlags(saveData, saveSlot, "Exited Level", ProgressFlags::FILEPROG_98_EXITED_LEVEL_TEXT);
		CheckboxProgressFlags(saveData, saveSlot, "Past 50 Note Door", ProgressFlags::FILEPROG_99_PAST_50_NOTE_DOOR_TEXT);
		CheckboxProgressFlags(saveData, saveSlot, "Near Puzzle Podium", ProgressFlags::FILEPROG_A7_NEAR_PUZZLE_PODIUM_TEXT);
		CheckboxProgressFlags(saveData, saveSlot, "Has Seen Trex", ProgressFlags::FILEPROG_BA_HAS_SEEN_TREX_TEXT);
		CheckboxProgressFlags(saveData, saveSlot, "Baddies Escaped", ProgressFlags::FILEPROG_C1_BADDIES_ESCAPE_TEXT);
		EndProgressFlagsGroup();

		BeginProgressFlagsGroup("Final Battle");
		CheckboxProgressFlags(saveData, saveSlot, "Activated Jinjo Statue", ProgressFlags::FILEPROG_D1_HAS_ACTIVATED_A_JINJO_STATUE_IN_FINAL_FIGHT);
		CheckboxProgressFlags(saveData, saveSlot, "Spawned Jinjo Statue", ProgressFlags::FILEPROG_D2_HAS_SPAWNED_A_JINJO_STATUE_IN_FINAL_FIGHT);
		CheckboxProgressFlags(saveData, saveSlot, "Grunty Defeated", ProgressFlags::FILEPROG_FC_DEFEAT_GRUNTY);
		EndProgressFlagsGroup();

		ImGui::TableSetColumnIndex(2);

		BeginProgressFlagsGroup("Pressed Witch Switches");
		CheckboxProgressFlags(saveData, saveSlot, "Lair Jump Pad", ProgressFlags::FILEPROG_C6_LAIR_JUMP_PAD_SWITCH_PRESSED);
		CheckboxProgressFlags(saveData, saveSlot, "MM Jiggy", ProgressFlags::FILEPROG_18_MM_WITCH_SWITCH_JIGGY_PRESSED);
		CheckboxProgressFlags(saveData, saveSlot, "TTC Jiggy", ProgressFlags::FILEPROG_1A_TTC_WITCH_SWITCH_JIGGY_PRESSED);
		CheckboxProgressFlags(saveData, saveSlot, "CC", ProgressFlags::FILEPROG_9A_CC_WITCH_SWITCH_PRESSED);
		CheckboxProgressFlags(saveData, saveSlot, "CC Eyes Active", ProgressFlags::FILEPROG_9B_LAIR_CC_WITCH_SWITCH_EYES_ACTIVE);
		CheckboxProgressFlags(saveData, saveSlot, "CC Left Eye", ProgressFlags::FILEPROG_9C_LAIR_CC_WITCH_SWITCH_LEFT_EYE_PRESSED);
		CheckboxProgressFlags(saveData, saveSlot, "CC Right Eye", ProgressFlags::FILEPROG_9D_LAIR_CC_WITCH_SWITCH_RIGHT_EYE_PRESSED);
		CheckboxProgressFlags(saveData, saveSlot, "BGS Jiggy", ProgressFlags::FILEPROG_9F_BGS_WITCH_SWITCH_JIGGY_PRESSED);
		CheckboxProgressFlags(saveData, saveSlot, "FP Jiggy", ProgressFlags::FILEPROG_47_FP_WITCH_SWITCH_JIGGY_PRESSED);
		CheckboxProgressFlags(saveData, saveSlot, "FP Advent Door Open", ProgressFlags::FILEPROG_48_FP_WITCH_SWITCH_ADVENT_DOOR_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "GV Jiggy", ProgressFlags::FILEPROG_A0_GV_WITCH_SWITCH_JIGGY_PRESSED);
		CheckboxProgressFlags(saveData, saveSlot, "GV Stop 'N' Swop", ProgressFlags::FILEPROG_A3_GV_SNS_SWITCH_PRESSED);
		CheckboxProgressFlags(saveData, saveSlot, "MMM Jiggy", ProgressFlags::FILEPROG_19_MMM_WITCH_SWITCH_JIGGY_PRESSED);
		CheckboxProgressFlags(saveData, saveSlot, "RBB Jiggy", ProgressFlags::FILEPROG_1C_RBB_WITCH_SWITCH_JIGGY_PRESSED);
		CheckboxProgressFlags(saveData, saveSlot, "CCW Jiggy", ProgressFlags::FILEPROG_46_CCW_WITCH_SWITCH_JIGGY_PRESSED);
		CheckboxProgressFlags(saveData, saveSlot, "CCW Podium Switch", ProgressFlags::FILEPROG_53_CCW_PUZZLE_PODIUM_SWITCH_PRESSED);
		EndProgressFlagsGroup();

		BeginProgressFlagsGroup("Pressed Misc Switches");
		CheckboxProgressFlags(saveData, saveSlot, "Water Switch 1", ProgressFlags::FILEPROG_22_WATER_SWITCH_1_PRESSED);
		CheckboxProgressFlags(saveData, saveSlot, "Water Switch 2", ProgressFlags::FILEPROG_24_WATER_SWITCH_2_PRESSED);
		CheckboxProgressFlags(saveData, saveSlot, "Water Switch 3", ProgressFlags::FILEPROG_26_WATER_SWITCH_3_PRESSED);
		EndProgressFlagsGroup();

		BeginProgressFlagsGroup("Level Events");
		CheckboxProgressFlags(saveData, saveSlot, "Has Died", ProgressFlags::FILEPROG_A8_HAS_DIED);
		CheckboxProgressFlags(saveData, saveSlot, "BGS Piranha Water", ProgressFlags::FILEPROG_F_HAS_TOUCHED_PIRAHANA_WATER);
		CheckboxProgressFlags(saveData, saveSlot, "BGS Met Flibbits", ProgressFlags::FILEPROG_1B_MET_YELLOW_FLIBBITS);
		CheckboxProgressFlags(saveData, saveSlot, "FP Twinklies Minigame", ProgressFlags::FILEPROG_13_COMPLETED_TWINKLIES_MINIGAME);
		CheckboxProgressFlags(saveData, saveSlot, "FP Touched Icy Water", ProgressFlags::FILEPROG_14_HAS_TOUCHED_FP_ICY_WATER);
		CheckboxProgressFlags(saveData, saveSlot, "Met Twinklies", ProgressFlags::FILEPROG_82_MET_TWINKLIES);
		CheckboxProgressFlags(saveData, saveSlot, "GV SNS Sarcophagus", ProgressFlags::FILEPROG_A4_GV_SNS_SARCOPHAGUS_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "GV Touched Sand Eel", ProgressFlags::FILEPROG_10_HAS_TOUCHED_SAND_EEL_SAND);
		InputProgressFlags(saveData, saveSlot, "GV Pyramid State", ProgressFlags::FILEPROG_F8_KING_SANDYBUTT_PYRAMID_STATE, 2, 3);
		CheckboxProgressFlags(saveData, saveSlot, "MMM Thorn Hedge", ProgressFlags::FILEPROG_86_HAS_TOUCHED_MMM_THORN_HEDGE);
		CheckboxProgressFlags(saveData, saveSlot, "MMM Tried Loggo as Bear", ProgressFlags::FILEPROG_88_TRIED_LOGGO_AS_BEAR);
		CheckboxProgressFlags(saveData, saveSlot, "MMM Entered Loggo", ProgressFlags::FILEPROG_89_ENTERED_LOGGO_AS_PUMPKIN);
		CheckboxProgressFlags(saveData, saveSlot, "MMM Exited Loggo", ProgressFlags::FILEPROG_8A_EXITED_LOGGO);
		CheckboxProgressFlags(saveData, saveSlot, "RBB Touched Oven", ProgressFlags::FILEPROG_A9_HAS_TOUCHED_RBB_OVEN);
		CheckboxProgressFlags(saveData, saveSlot, "RBB Swim Oily Water", ProgressFlags::FILEPROG_AB_SWIM_OILY_WATER);
		CheckboxProgressFlags(saveData, saveSlot, "RBB Dive Oily Water", ProgressFlags::FILEPROG_AC_DIVE_OILY_WATER);
		CheckboxProgressFlags(saveData, saveSlot, "CCW Podium Active", ProgressFlags::FILEPROG_54_CCW_PUZZLE_PODIUM_ACTIVE);
		CheckboxProgressFlags(saveData, saveSlot, "CCW Spring Open", ProgressFlags::FILEPROG_8B_CCW_SPRING_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "CCW Summer Open", ProgressFlags::FILEPROG_8C_CCW_SUMMER_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "CCW Autumn Open", ProgressFlags::FILEPROG_8D_CCW_AUTUMN_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "CCW Winer Open", ProgressFlags::FILEPROG_8E_CCW_WINTER_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "CCW Flower Spring", ProgressFlags::FILEPROG_E3_CCW_FLOWER_SPRING);
		CheckboxProgressFlags(saveData, saveSlot, "CCW Flower Summer", ProgressFlags::FILEPROG_E4_CCW_FLOWER_SUMMER);
		CheckboxProgressFlags(saveData, saveSlot, "CCW Flower Autumn", ProgressFlags::FILEPROG_E5_CCW_FLOWER_AUTUMN);
		CheckboxProgressFlags(saveData, saveSlot, "CCW Spring Eyrie", ProgressFlags::FILEPROG_E6_SPRING_EYRIE_HATCHED);
		CheckboxProgressFlags(saveData, saveSlot, "CCW Summer Eyrie", ProgressFlags::FILEPROG_E7_SUMMER_EYRIE_FED);
		CheckboxProgressFlags(saveData, saveSlot, "CCW Autumn Eyrie", ProgressFlags::FILEPROG_E8_AUTMN_EYRIE_FED);
		CheckboxProgressFlags(saveData, saveSlot, "CCW Met Bees", ProgressFlags::FILEPROG_8F_MET_BEE_INFESTED_BEEHIVE);
		CheckboxProgressFlags(saveData, saveSlot, "CCW Bramble Field", ProgressFlags::FILEPROG_AA_HAS_TOUCHED_CCW_BRAMBLE_FIELD);
		CheckboxProgressFlags(saveData, saveSlot, "CCW Touched Icy Water", ProgressFlags::FILEPROG_DD_HAS_TOUCHED_CCW_ICY_WATER);
		EndProgressFlagsGroup();

		ImGui::TableSetColumnIndex(3);

		BeginProgressFlagsGroup("Brentilda");
		CheckboxProgressFlags(saveData, saveSlot, "Met Brentilda", ProgressFlags::FILEPROG_96_MET_BRENTILDA);
		CheckboxProgressFlags(saveData, saveSlot, "Healed by Brentilda 1", ProgressFlags::FILEPROG_E9_HEALED_BY_BRENTILDA_1);
		CheckboxProgressFlags(saveData, saveSlot, "Healed by Brentilda 2", ProgressFlags::FILEPROG_EA_HEALED_BY_BRENTILDA_2);
		CheckboxProgressFlags(saveData, saveSlot, "Healed by Brentilda 3", ProgressFlags::FILEPROG_EB_HEALED_BY_BRENTILDA_3);
		CheckboxProgressFlags(saveData, saveSlot, "Healed by Brentilda 4", ProgressFlags::FILEPROG_EC_HEALED_BY_BRENTILDA_4);
		CheckboxProgressFlags(saveData, saveSlot, "Healed by Brentilda 5", ProgressFlags::FILEPROG_ED_HEALED_BY_BRENTILDA_5);
		CheckboxProgressFlags(saveData, saveSlot, "Healed by Brentilda 6", ProgressFlags::FILEPROG_EE_HEALED_BY_BRENTILDA_6);
		CheckboxProgressFlags(saveData, saveSlot, "Healed by Brentilda 7", ProgressFlags::FILEPROG_EF_HEALED_BY_BRENTILDA_7);
		CheckboxProgressFlags(saveData, saveSlot, "Healed by Brentilda 8", ProgressFlags::FILEPROG_F0_HEALED_BY_BRENTILDA_8);
		CheckboxProgressFlags(saveData, saveSlot, "Healed by Brentilda 9", ProgressFlags::FILEPROG_F1_HEALED_BY_BRENTILDA_9);
		CheckboxProgressFlags(saveData, saveSlot, "Healed by Brentilda 10", ProgressFlags::FILEPROG_F2_HEALED_BY_BRENTILDA_10);
		EndProgressFlagsGroup();

		BeginProgressFlagsGroup("Broken Stuff");
		CheckboxProgressFlags(saveData, saveSlot, "Ice Ball to Cheato", ProgressFlags::FILEPROG_C3_ICE_BALL_TO_CHEATO_BROKEN);
		CheckboxProgressFlags(saveData, saveSlot, "Statue Eye", ProgressFlags::FILEPROG_C4_STATUE_EYE_BROKEN);
		CheckboxProgressFlags(saveData, saveSlot, "Rareware Box", ProgressFlags::FILEPROG_C5_RAREWARE_BOX_BROKEN);
		CheckboxProgressFlags(saveData, saveSlot, "Wall to Wading Boots", ProgressFlags::FILEPROG_C8_LAIR_BRICKWALL_TO_WADINGBOOTS_BROKEN);
		CheckboxProgressFlags(saveData, saveSlot, "Wall to Shock Jump Pad", ProgressFlags::FILEPROG_C9_LAIR_BRICKWALL_TO_SHOCKJUMP_PAD_BROKEN);
		CheckboxProgressFlags(saveData, saveSlot, "Cobweb Purple Cauldron", ProgressFlags::FILEPROG_CA_COBWEB_BLOCKING_PURPLE_CAULDRON_BROKEN);
		CheckboxProgressFlags(saveData, saveSlot, "Cobweb Flight Pad", ProgressFlags::FILEPROG_CB_LAIR_COBWEB_OVER_FLIGHTPAD_BROKEN);
		CheckboxProgressFlags(saveData, saveSlot, "Cobweb Green Cauldron", ProgressFlags::FILEPROG_CC_LAIR_COBWEB_OVER_GREEN_CAULDRON_BROKEN);
		EndProgressFlagsGroup();

		BeginProgressFlagsGroup("Cheats Unlocked");
		CheckboxProgressFlags(saveData, saveSlot, "Cheato Blue Eggs", ProgressFlags::FILEPROG_AD_CHEATO_BLUEEGGS_UNLOCKED);
		CheckboxProgressFlags(saveData, saveSlot, "Cheato Red Feathers", ProgressFlags::FILEPROG_AE_CHEATO_REDFEATHERS_UNLOCKED);
		CheckboxProgressFlags(saveData, saveSlot, "Cheato Gold Feathers", ProgressFlags::FILEPROG_AF_CHEATO_GOLDFEATHERS_UNLOCKED);
		InputProgressFlags(saveData, saveSlot, "Banned Cheats Entered", ProgressFlags::FILEPROG_FD_CHEATCODES_ENTERED, 2, 3);
		EndProgressFlagsGroup();

		BeginProgressFlagsGroup("SM / Lair Events");
		CheckboxProgressFlags(saveData, saveSlot, "Skipped Tutorial", ProgressFlags::FILEPROG_DB_SKIPPED_TUTORIAL);
		CheckboxProgressFlags(saveData, saveSlot, "Stood on Jiggy Podium", ProgressFlags::FILEPROG_16_STOOD_ON_JIGSAW_PODIUM);
		CheckboxProgressFlags(saveData, saveSlot, "Have Enough Jiggies", ProgressFlags::FILEPROG_17_HAS_HAD_ENOUGH_JIGSAW_PIECES);
		CheckboxProgressFlags(saveData, saveSlot, "Used All Jiggies", ProgressFlags::FILEPROG_DE_USED_ALL_YOUR_PUZZLE_PIECES);
		CheckboxProgressFlags(saveData, saveSlot, "Remove All Jiggies", ProgressFlags::FILEPROG_DF_CAN_REMOVE_ALL_PUZZLE_PIECES);
		CheckboxProgressFlags(saveData, saveSlot, "Can Place All Jiggies", ProgressFlags::FILEPROG_E0_CAN_PLACE_ALL_PUZZLE_PIECES);
		CheckboxProgressFlags(saveData, saveSlot, "CC Lobby Pipe 1 Raised", ProgressFlags::FILEPROG_1F_CC_LOBBY_PIPE_1_RAISED);
		CheckboxProgressFlags(saveData, saveSlot, "CC Lobby Pipe 2 Raised", ProgressFlags::FILEPROG_20_CC_LOBBY_PIPE_2_RAISED);
		CheckboxProgressFlags(saveData, saveSlot, "CC Lobby Pipe 3 Raised", ProgressFlags::FILEPROG_21_CC_LOBBY_PIPE_3_RAISED);
		CheckboxProgressFlags(saveData, saveSlot, "Statue Hat Open", ProgressFlags::FILEPROG_A1_STATUE_HAT_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "GV Lobby Coffin Open", ProgressFlags::FILEPROG_A2_GV_LOBBY_COFFIN_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "Jump Pad Active", ProgressFlags::FILEPROG_C7_LAIR_JUMP_PAD_ACTIVE);
		CheckboxProgressFlags(saveData, saveSlot, "Crypt Gate Open", ProgressFlags::FILEPROG_A5_LAIR_CRYPT_GATE_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "Crypt Coffin Open", ProgressFlags::FILEPROG_9E_CRYPT_COFFIN_LID_OPEN);
		CheckboxProgressFlags(saveData, saveSlot, "Transformed in Crypt", ProgressFlags::FILEPROG_F7_HAS_TRANSFORMED_IN_CRYPT);
		CheckboxProgressFlags(saveData, saveSlot, "Water Level 1", ProgressFlags::FILEPROG_23_LAIR_WATER_LEVEL_1);
		CheckboxProgressFlags(saveData, saveSlot, "Water Level 2", ProgressFlags::FILEPROG_25_LAIR_WATER_LEVEL_2);
		CheckboxProgressFlags(saveData, saveSlot, "Water Level 3", ProgressFlags::FILEPROG_27_LAIR_WATER_LEVEL_3);
		CheckboxProgressFlags(saveData, saveSlot, "Met Dingpot", ProgressFlags::FILEPROG_F3_MET_DINGPOT);
		CheckboxProgressFlags(saveData, saveSlot, "Seen Grunty Podium Door", ProgressFlags::FILEPROG_F6_SEEN_DOOR_OF_GRUNTY_PUZZLE_PODIUM);
		EndProgressFlagsGroup();

		BeginProgressFlagsGroup("Furnace Fun");
		CheckboxProgressFlags(saveData, saveSlot, "BK Instructions", ProgressFlags::FILEPROG_55_FF_BK_SQUARE_INSTRUCTIONS);
		CheckboxProgressFlags(saveData, saveSlot, "Picture Instructions", ProgressFlags::FILEPROG_56_FF_PICTURE_SQUARE_INSTRUCTIONS);
		CheckboxProgressFlags(saveData, saveSlot, "Music Instructions", ProgressFlags::FILEPROG_57_FF_MUSIC_SQAURE_INSTRUCTIONS);
		CheckboxProgressFlags(saveData, saveSlot, "Minigame Instructions", ProgressFlags::FILEPROG_58_FF_MINIGAME_SQAURE_INSTRUCTIONS);
		CheckboxProgressFlags(saveData, saveSlot, "Grunty Instructions", ProgressFlags::FILEPROG_59_FF_GRUNTY_SQAURE_INSTRUCTIONS);
		CheckboxProgressFlags(saveData, saveSlot, "Death Instructions", ProgressFlags::FILEPROG_5A_FF_DEATH_SQAURE_INSTRUCTIONS);
		CheckboxProgressFlags(saveData, saveSlot, "Joker Instructions", ProgressFlags::FILEPROG_5B_FF_JOKER_SQAURE_INSTRUCTIONS);
		CheckboxProgressFlags(saveData, saveSlot, "Game Complete", ProgressFlags::FILEPROG_A6_FURNACE_FUN_COMPLETE);
		CheckboxProgressFlags(saveData, saveSlot, "Pattern Set", ProgressFlags::FILEPROG_5C_FF_PATTERN_SET);
		InputProgressFlags(saveData, saveSlot, "Pattern", ProgressFlags::FILEPROG_D3_FF_PATTERN, 8, 255);
		EndProgressFlagsGroup();

		ImGui::EndTable();
	}

	ImGui::EndTabItem();
}

void SaveEditorUI::RenderGlobalDataSection(const SaveData& saveData, SaveFile* saveFile)
{
	if (!ImGui::BeginTabItem("Global Data")) return;

	PrintChecksum(saveFile->GetGlobalData()->GetChecksum(saveData.NeedsEndianSwap()));

	ImGui::SeparatorText("Unlocked / Collected Stop 'N' Swop Items");

	CheckboxSnS(saveData, "Cyan Egg", SnS::UNLOCKED_CYAN_EGG, SnS::COLLECTED_CYAN_EGG);
	CheckboxSnS(saveData, "Pink Egg", SnS::UNLOCKED_PINK_EGG, SnS::COLLECTED_PINK_EGG);
	CheckboxSnS(saveData, "Blue Egg", SnS::UNLOCKED_BLUE_EGG, SnS::COLLECTED_BLUE_EGG);
	CheckboxSnS(saveData, "Green Egg", SnS::UNLOCKED_GREEN_EGG, SnS::COLLECTED_GREEN_EGG);
	CheckboxSnS(saveData, "Red Egg", SnS::UNLOCKED_RED_EGG, SnS::COLLECTED_RED_EGG);
	CheckboxSnS(saveData, "Yellow Egg", SnS::UNLOCKED_YELLOW_EGG, SnS::COLLECTED_YELLOW_EGG);
	CheckboxSnS(saveData, "Ice Key", SnS::UNLOCKED_ICE_KEY, SnS::COLLECTED_ICE_KEY);

	ImGui::EndTabItem();
}

bool SaveEditorUI::CheckboxProgressFlags(const SaveData& saveData, SaveSlot* saveSlot, const char* label, const ProgressFlags flag) const
{
	bool value = saveSlot->GetProgressFlag(flag);

	if (ImGui::Checkbox(label, &value))
	{
		saveSlot->SetProgressFlag(flag, value);
		saveSlot->UpdateChecksum(saveData.NeedsEndianSwap());
	}

	return value;
}

uint8_t SaveEditorUI::InputProgressFlags(const SaveData& saveData, SaveSlot* saveSlot, const char* label, const ProgressFlags flag, const uint8_t bitsCount, const uint8_t maxValue) const
{
	uint8_t value = saveSlot->GetProgressValue(flag, bitsCount);

	ImGui::SetNextItemWidth(28);
	if (ImGui::InputScalar(label, ImGuiDataType_U8, &value, NULL, NULL, "%u"))
	{
		if (value > maxValue) value = maxValue;
		saveSlot->SetProgressValue(flag, bitsCount, value);
		saveSlot->UpdateChecksum(saveData.NeedsEndianSwap());
	}

	return value;
}

void SaveEditorUI::CheckboxAbility(const SaveData& saveData, SaveSlot* saveSlot, const char* label, const Abilities ability) const
{
	bool learned = saveSlot->GetLearnedAbility(ability);
	bool used = saveSlot->GetUsedAbility(ability);

	char learnedId[64];
	snprintf(learnedId, 64, "##%s Learned", label);

	if (ImGui::Checkbox(learnedId, &learned))
	{
		saveSlot->SetLearnedAbility(ability, learned);
		saveSlot->UpdateChecksum(saveData.NeedsEndianSwap());
	}

	ImGui::SameLine();

	if (ImGui::Checkbox(label, &used))
	{
		saveSlot->SetUsedAbility(ability, used);
		saveSlot->UpdateChecksum(saveData.NeedsEndianSwap());
	}
}

void SaveEditorUI::CheckboxSnS(const SaveData& saveData, const char* label, const SnS unlockedSnsItem, const SnS collectedSnsItem) const
{
	GlobalData* globalData = saveData.GetSaveFile()->GetGlobalData();

	bool unlocked = globalData->GetSnsItem(unlockedSnsItem);
	bool collected = globalData->GetSnsItem(collectedSnsItem);

	char unlockedId[64];
	snprintf(unlockedId, 64, "##%s Unlocked", label);

	if (ImGui::Checkbox(unlockedId, &unlocked))
	{
		globalData->SetSnsItem(unlockedSnsItem, unlocked);
		globalData->UpdateChecksum(saveData.NeedsEndianSwap());
	}

	ImGui::SameLine();

	if (ImGui::Checkbox(label, &collected))
	{
		globalData->SetSnsItem(collectedSnsItem, collected);
		globalData->UpdateChecksum(saveData.NeedsEndianSwap());
	}
}

void SaveEditorUI::PrintChecksum(const uint32_t checksum) const
{
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("Checksum: 0xFFFFFFFF").x - 32);
	ImGui::TextColored(ImVec4(0.7f, 0.5f, 0.6f, 1.0f), "Checksum: 0x%x", checksum);
}

void SaveEditorUI::PrintHeader(const char* label) const
{
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.5f, 0.6f, 1.0f));
	ImGui::SeparatorText(label);
	ImGui::PopStyleColor();
}

void SaveEditorUI::BeginProgressFlagsGroup(const char* label) const
{
	PrintHeader(label);
	ImGui::PushID(label);
}

void SaveEditorUI::EndProgressFlagsGroup() const
{
	ImGui::PopID();
}
