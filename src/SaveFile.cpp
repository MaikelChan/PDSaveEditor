#include "SaveFile.h"
#include <stdexcept>
#include "Utils.h"

#pragma region SaveBuffer

SaveBuffer::SaveBuffer()
{
	Clear();
}

SaveBuffer::SaveBuffer(const uint8_t* bytes, const uint32_t size)
{
	if (size > SAVE_BUFFER_SIZE)
		throw std::runtime_error(std::string("Can't create a SaveBuffer bigger than ") + std::to_string(SAVE_BUFFER_SIZE) + " bytes.");

	Clear();
	memcpy(SaveBuffer::bytes, bytes, size);
}

uint8_t* SaveBuffer::GetBytes()
{
	return bytes;
}

uint32_t SaveBuffer::ReadBits(const int32_t numbits)
{
	uint32_t bit = 1 << (numbits - 1);
	uint32_t value = 0;

	for (; bit; bit >>= 1) {
		int32_t bitindex = bitpos % 8;
		uint8_t mask = 1 << (7 - bitindex);
		int32_t byteindex = bitpos / 8;

		if (bytes[byteindex] & mask) {
			value |= bit;
		}

		bitpos++;
	}

	return value;
}

void SaveBuffer::ReadGuid(FileGuid* guid)
{
	guid->fileid = ReadBits(7);
	guid->deviceserial = ReadBits(13);
}

void SaveBuffer::WriteGuid(FileGuid* guid)
{
	Or(guid->fileid, 7);
	Or(guid->deviceserial, 13);
}

void SaveBuffer::ReadString(char* dst)
{
	bool foundnull = false;
	int32_t index = 0;

	for (int32_t i = 0; i < MAX_NAME_LENGTH; i++)
	{
		uint32_t byte = ReadBits(8);

		if (foundnull) continue;

		if (byte == '\0')
		{
			foundnull = true;
		}
		else
		{
			dst[i] = byte;
			index = i;
		}
	}

	dst[++index] = '\0';
}

void SaveBuffer::WriteString(char* src)
{
	bool done = false;

	for (int32_t i = 0; i < MAX_NAME_LENGTH; i++)
	{
		if (!done)
		{
			if (src[i] == '\0')
			{
				done = true;
			}
			else if (src[i] == '\n')
			{
				done = true;
			}
			else
			{
				uint32_t c = src[i];
				Or(c, 8);
			}
		}

		if (done)
		{
			Or('\0', 8);
		}
	}
}

void SaveBuffer::Or(const uint32_t value, const int32_t numbits)
{
	uint32_t bit = 1 << (numbits - 1);

	for (; bit; bit >>= 1)
	{
		if (bit & value)
		{
			int32_t bitindex = bitpos % 8;
			uint8_t mask = 1 << (7 - bitindex);
			int32_t byteindex = bitpos / 8;

			bytes[byteindex] |= mask;
		}

		bitpos++;
	}
}

void SaveBuffer::Clear()
{
	bitpos = 0;
	memset(bytes, 0, SAVE_BUFFER_SIZE);
}

#pragma endregion

#pragma region PakFile

void PakFile::Load(uint8_t* fileBuffer)
{
	memcpy(&pakFileHeader, &fileBuffer[0], PACK_HEADER_SIZE);

	uint16_t headerChecksum[2];
	SaveFile::CalculateChecksum(&fileBuffer[8], &fileBuffer[PACK_HEADER_SIZE], headerChecksum);

	uint16_t bodyChecksum[2];
	SaveFile::CalculateChecksum(&fileBuffer[PACK_HEADER_SIZE], &fileBuffer[PACK_HEADER_SIZE + pakFileHeader.bodylen], bodyChecksum);

	isValid = pakFileHeader.headersum[0] == headerChecksum[0] && pakFileHeader.headersum[1] == headerChecksum[1] &&
		pakFileHeader.bodysum[0] == bodyChecksum[0] && pakFileHeader.bodysum[1] == bodyChecksum[1];
}

void PakFile::Save(uint8_t* fileBuffer)
{
	memcpy(&fileBuffer[0], &pakFileHeader, PACK_HEADER_SIZE);

	uint16_t headerChecksum[2];
	SaveFile::CalculateChecksum(&fileBuffer[8], &fileBuffer[PACK_HEADER_SIZE], headerChecksum);
	memcpy(&fileBuffer[0], headerChecksum, 4);

	if (pakFileHeader.occupied)
	{
		uint16_t bodyChecksum[2];
		SaveFile::CalculateChecksum(&fileBuffer[PACK_HEADER_SIZE], &fileBuffer[PACK_HEADER_SIZE + pakFileHeader.bodylen], bodyChecksum);
		memcpy(&fileBuffer[4], bodyChecksum, 4);
	}
	else
	{
		for (uint8_t i = 0; i < 4; i++) fileBuffer[4 + i] = 0xff;
	}

	isValid = true;
}

#pragma endregion

#pragma region BossFile

void BossFile::Load(uint8_t* fileBuffer)
{
	PakFile::Load(fileBuffer);

	if (pakFileHeader.filelen != PACK_BOSS_SIZE + PACK_HEADER_SIZE)
		throw std::runtime_error(std::string("Unexpected size of \"BOSS\" file: ") + std::to_string(pakFileHeader.filelen) + ".");

	SaveBuffer buffer(&fileBuffer[PACK_HEADER_SIZE], SAVE_BUFFER_SIZE);

	buffer.ReadGuid(&guid);

	unk1 = buffer.ReadBits(1);
	language = buffer.ReadBits(4);

	for (int32_t tn = 0; tn < TEAM_NAMES_COUNT; tn++)
	{
		buffer.ReadString(teamNames[tn]);
	}

	tracknum = buffer.ReadBits(8);

	for (int32_t i = 0; i < MULTIPLE_TRACKS_SIZE; i++)
	{
		multipletracknums[i] = buffer.ReadBits(8);
	}

	usingmultipletunes = buffer.ReadBits(1);
	altTitleUnlocked = buffer.ReadBits(1);
	altTitleEnabled = buffer.ReadBits(1);
}

void BossFile::Save(uint8_t* fileBuffer)
{
	SaveBuffer buffer;

	buffer.WriteGuid(&guid);

	buffer.Or(unk1, 1);
	buffer.Or(language, 4);

	for (int32_t tn = 0; tn < TEAM_NAMES_COUNT; tn++)
	{
		buffer.WriteString(teamNames[tn]);
	}

	if (tracknum == 255) buffer.Or(255, 8);
	else buffer.Or(tracknum, 8);

	for (int32_t i = 0; i < MULTIPLE_TRACKS_SIZE; i++)
	{
		buffer.Or(multipletracknums[i], 8);
	}

	buffer.Or(usingmultipletunes, 1);
	buffer.Or(altTitleUnlocked, 1);
	buffer.Or(altTitleEnabled, 1);

	uint8_t* bytes = buffer.GetBytes();
	for (uint8_t i = 0; i < pakFileHeader.filelen - PACK_HEADER_SIZE; i++)
	{
		fileBuffer[PACK_HEADER_SIZE + i] = bytes[i % pakFileHeader.bodylen];
	}

	PakFile::Save(fileBuffer);
}

bool BossFile::IsMultiTrackSlotEnabled(const uint8_t slot) const
{
	uint8_t index = slot >> 3;
	uint8_t value = 1 << (slot & 7);

	return (multipletracknums[index] & value) != 0;
}

void BossFile::SetMultiTrackSlotEnabled(const uint8_t slot, const bool enable)
{
	uint8_t value = 1 << (slot & 7);
	uint8_t index = slot >> 3;

	if (enable) multipletracknums[index] |= value;
	else multipletracknums[index] &= ~value;
}

#pragma endregion

#pragma region GameFile

void GameFile::Load(uint8_t* fileBuffer)
{
	PakFile::Load(fileBuffer);

	if (pakFileHeader.filelen != PACK_GAME_SIZE + PACK_HEADER_SIZE)
		throw std::runtime_error(std::string("Unexpected size of \"GAME\" file: ") + std::to_string(pakFileHeader.filelen) + ".");

	SaveBuffer buffer(&fileBuffer[PACK_HEADER_SIZE], SAVE_BUFFER_SIZE);

	buffer.ReadString(name);
	thumbnail = buffer.ReadBits(5);
	totaltime = buffer.ReadBits(32);
	autodifficulty = buffer.ReadBits(2);
	autostageindex = buffer.ReadBits(5);
	sfxVolume = buffer.ReadBits(6);
	musicVolume = buffer.ReadBits(6);
	soundMode = buffer.ReadBits(2);
	controlModes[0] = buffer.ReadBits(3);
	controlModes[1] = buffer.ReadBits(3);

	for (uint8_t i = 0; i < GAMEFILE_FLAGS_SIZE; i++)
	{
		flags[i] = buffer.ReadBits(8);
	}

	unknown1 = buffer.ReadBits(16);

	for (uint8_t i = 0; i < NUM_SOLOSTAGES; i++)
	{
		for (uint8_t j = 0; j < NUM_DIFFICULTIES; j++)
		{
			besttimes[i][j] = buffer.ReadBits(12);
		}
	}

	for (uint8_t i = 0; i < NUM_MP_CHALLENGES; i++)
	{
		mpChallenges[i] = 0;

		for (uint8_t j = 0; j < MAX_PLAYERS; j++)
		{
			uint8_t completed = buffer.ReadBits(1);
			mpChallenges[i] |= completed << j;
		}
	}

	for (uint8_t i = 0; i < NUM_DIFFICULTIES; i++)
	{
		coopcompletions[i] = buffer.ReadBits(NUM_SOLOSTAGES);
	}

	for (uint8_t i = 0; i < 9; i++)
	{
		int32_t numbits = i == 8 ? 2 : 8;
		firingrangescores[i] = buffer.ReadBits(numbits);
	}

	for (uint8_t i = 0; i < 4; i++)
	{
		weaponsfound[i] = buffer.ReadBits(8);
	}
}

void GameFile::Save(uint8_t* fileBuffer)
{
	SaveBuffer buffer;

	buffer.WriteString(name);
	buffer.Or(thumbnail, 5);
	buffer.Or(totaltime, 32);
	buffer.Or(autodifficulty, 2);
	buffer.Or(autostageindex, 5);
	buffer.Or(sfxVolume, 6);
	buffer.Or(musicVolume, 6);
	buffer.Or(soundMode, 2);
	buffer.Or(controlModes[0], 3);
	buffer.Or(controlModes[1], 3);

	for (uint8_t i = 0; i < GAMEFILE_FLAGS_SIZE; i++)
	{
		buffer.Or(flags[i], 8);
	}

	buffer.Or(unknown1, 16);

	for (uint8_t i = 0; i < NUM_SOLOSTAGES; i++)
	{
		for (uint8_t j = 0; j < NUM_DIFFICULTIES; j++)
		{
			buffer.Or(besttimes[i][j], 12);
		}
	}

	for (uint8_t i = 0; i < NUM_MP_CHALLENGES; i++)
	{
		for (uint8_t j = 0; j < MAX_PLAYERS; j++)
		{
			uint32_t completed = (mpChallenges[i] & (1 << j)) != 0 ? 1 : 0;
			buffer.Or(completed, 1);
		}
	}

	for (uint8_t i = 0; i < NUM_DIFFICULTIES; i++)
	{
		buffer.Or(coopcompletions[i], NUM_SOLOSTAGES);
	}

	for (uint8_t i = 0; i < 9; i++)
	{
		buffer.Or(firingrangescores[i], i == 8 ? 2 : 8);
	}

	for (uint8_t i = 0; i < 4; i++)
	{
		buffer.Or(weaponsfound[i], 8);
	}

	uint8_t* bytes = buffer.GetBytes();
	for (uint8_t i = 0; i < pakFileHeader.filelen - PACK_HEADER_SIZE; i++)
	{
		fileBuffer[PACK_HEADER_SIZE + i] = bytes[i % pakFileHeader.bodylen];
	}

	PakFile::Save(fileBuffer);
}

bool GameFile::GetFlag(const SinglePlayerFlags flag) const
{
	uint32_t flagnum = (uint32_t)flag;
	uint32_t byteindex = flagnum / 8;
	uint8_t mask = 1 << (flagnum % 8);

	return (flags[byteindex] & mask) != 0;
}

void GameFile::SetFlag(const SinglePlayerFlags flag, const bool set)
{
	uint32_t flagnum = (uint32_t)flag;
	uint32_t byteindex = flagnum / 8;
	uint8_t mask = 1 << (flagnum % 8);

	if (set) flags[byteindex] |= mask;
	else flags[byteindex] &= ~mask;
}

uint8_t GameFile::GetFiringRangeScore(const uint8_t weaponIndex) const
{
	return (firingrangescores[weaponIndex >> 2] >> (weaponIndex % 4) * 2) & 3;
}

void GameFile::SetFiringRangeScore(const uint8_t weaponIndex, const uint8_t difficulty)
{
	uint32_t byteindex = weaponIndex >> 2;
	uint32_t shiftamount = (weaponIndex % 4) * 2;
	uint32_t value = firingrangescores[byteindex];
	uint32_t mask = (1 << shiftamount) + (1 << (shiftamount + 1));

	value &= 255 - mask;
	value += (difficulty << shiftamount) & mask;

	firingrangescores[byteindex] = value;
}

bool GameFile::GetWeaponFound(const uint8_t weaponIndex) const
{
	uint32_t byteindex = weaponIndex >> 3;
	return (weaponsfound[byteindex] & (1 << (weaponIndex % 8))) != 0;
}

void GameFile::SetWeaponFound(const uint8_t weaponIndex, const bool found)
{
	uint32_t byteindex = weaponIndex >> 3;
	uint32_t value = weaponsfound[byteindex];

	if (found) weaponsfound[byteindex] |= (1 << (weaponIndex % 8));
	else weaponsfound[byteindex] &= ~(1 << (weaponIndex % 8));
}

#pragma endregion

#pragma region MultiplayerProfile

void MultiplayerProfile::Load(uint8_t* fileBuffer)
{
	PakFile::Load(fileBuffer);

	if (pakFileHeader.filelen != PACK_MPPLAYER_SIZE + PACK_HEADER_SIZE)
		throw std::runtime_error(std::string("Unexpected size of \"MPPLAYER\" file: ") + std::to_string(pakFileHeader.filelen) + ".");

	SaveBuffer buffer(&fileBuffer[PACK_HEADER_SIZE], SAVE_BUFFER_SIZE);

	buffer.ReadString(name);
	time = buffer.ReadBits(28);
	mpheadnum = buffer.ReadBits(7);
	mpbodynum = buffer.ReadBits(7);
	buffer.ReadGuid(&guid);
	displayoptions = buffer.ReadBits(8);
	kills = buffer.ReadBits(20);
	deaths = buffer.ReadBits(20);
	gamesplayed = buffer.ReadBits(19);
	gameswon = buffer.ReadBits(19);
	gameslost = buffer.ReadBits(19);
	distance = buffer.ReadBits(25);
	accuracy = buffer.ReadBits(10);
	damagedealt = buffer.ReadBits(26);
	painreceived = buffer.ReadBits(26);
	headshots = buffer.ReadBits(20);
	ammoused = buffer.ReadBits(30);
	accuracymedals = buffer.ReadBits(18);
	headshotmedals = buffer.ReadBits(18);
	killmastermedals = buffer.ReadBits(18);
	survivormedals = buffer.ReadBits(16);
	controlmode = buffer.ReadBits(2);
	options = buffer.ReadBits(12);

	for (uint8_t i = 0; i < NUM_MP_CHALLENGES; i++)
	{
		mpChallenges[i] = 0;

		for (uint8_t j = 0; j < MAX_PLAYERS; j++)
		{
			uint8_t completed = buffer.ReadBits(1);
			mpChallenges[i] |= completed << j;
		}
	}

	int32_t bitsremaining = NUM_WEAPONS;
	uint8_t w = 0;
	while (bitsremaining > 0)
	{
		int32_t numbits = bitsremaining;
		if (numbits > 8) numbits = 8;

		gunfuncs[w] = buffer.ReadBits(numbits);

		bitsremaining -= 8;
		w++;
	}
}

void MultiplayerProfile::Save(uint8_t* fileBuffer)
{
	SaveBuffer buffer;

	buffer.WriteString(name);
	buffer.Or(time, 28);
	buffer.Or(mpheadnum, 7);
	buffer.Or(mpbodynum, 7);
	buffer.WriteGuid(&guid);
	buffer.Or(displayoptions, 8);
	buffer.Or(kills, 20);
	buffer.Or(deaths, 20);
	buffer.Or(gamesplayed, 19);
	buffer.Or(gameswon, 19);
	buffer.Or(gameslost, 19);
	buffer.Or(distance, 25);
	buffer.Or(accuracy, 10);
	buffer.Or(damagedealt, 26);
	buffer.Or(painreceived, 26);
	buffer.Or(headshots, 20);
	buffer.Or(ammoused, 30);
	buffer.Or(accuracymedals, 18);
	buffer.Or(headshotmedals, 18);
	buffer.Or(killmastermedals, 18);
	buffer.Or(survivormedals, 16);
	buffer.Or(controlmode, 2);
	buffer.Or(options, 12);

	for (uint8_t i = 0; i < NUM_MP_CHALLENGES; i++)
	{
		for (uint8_t j = 0; j < MAX_PLAYERS; j++)
		{
			uint32_t completed = (mpChallenges[i] & (1 << j)) != 0 ? 1 : 0;
			buffer.Or(completed, 1);
		}
	}

	int32_t bitsremaining = NUM_WEAPONS;
	uint8_t w = 0;
	while (bitsremaining > 0)
	{
		int32_t numbits = bitsremaining;
		if (numbits > 8) numbits = 8;

		buffer.Or(gunfuncs[w], numbits);

		bitsremaining -= 8;
		w++;
	}

	uint8_t* bytes = buffer.GetBytes();
	for (uint8_t i = 0; i < pakFileHeader.filelen - PACK_HEADER_SIZE; i++)
	{
		fileBuffer[PACK_HEADER_SIZE + i] = bytes[i % pakFileHeader.bodylen];
	}

	PakFile::Save(fileBuffer);
}

bool MultiplayerProfile::GetOptionsFlag(const MultiplayerOptionsFlags flag) const
{
	return (options & (uint16_t)flag) != 0;
}

void MultiplayerProfile::SetOptionsFlag(const MultiplayerOptionsFlags flag, const bool set)
{
	if (set) options |= (uint16_t)flag;
	else options &= ~(uint16_t)flag;
}

bool MultiplayerProfile::GetDisplayOptionsFlag(const MultiplayerDisplayOptionsFlags flag) const
{
	return (displayoptions & (uint16_t)flag) != 0;
}

void MultiplayerProfile::SetDisplayOptionsFlag(const MultiplayerDisplayOptionsFlags flag, const bool set)
{
	if (set) displayoptions |= (uint16_t)flag;
	else displayoptions &= ~(uint16_t)flag;
}

bool MultiplayerProfile::GetWeaponSecondaryFunction(const uint8_t weaponIndex) const
{
	return gunfuncs[(weaponIndex - 1) >> 3] & (1 << ((weaponIndex - 1) & 7));
}

void MultiplayerProfile::SetWeaponSecondaryFunction(const uint8_t weaponIndex, const bool secondary)
{
	if (secondary) gunfuncs[(weaponIndex - 1) >> 3] |= (1 << ((weaponIndex - 1) & 7));
	else gunfuncs[(weaponIndex - 1) >> 3] &= ~(1 << ((weaponIndex - 1) & 7));
}

MultiplayerTitles MultiplayerProfile::GetPlayerTitle(const bool newMethod) const
{
	const uint32_t tiersNew[] = { 2, 4, 8, 16, 28, 60, 100, 150, 210, 300 };
	const uint32_t tiersOld[] = { 2, 4, 8, 16, 28, 48, 78, 138, 198, 300 };

	int32_t tallies[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int32_t max;
	int32_t i;

#define MULT(val) (val * (newMethod ? 3 : 1))
#define TIERS (newMethod ? tiersNew : tiersOld)

	for (i = 0; i < NUM_MP_TIERS_TALLIES; i++)
	{
		if (kills >= TIERS[i] * MULT(20)) tallies[0]++;
		else break;
	}

	for (i = 0; i < NUM_MP_TIERS_TALLIES; i++)
	{
		if (gameswon >= TIERS[i] * MULT(1))tallies[1]++;
		else break;
	}

	for (i = 0; i < NUM_MP_TIERS_TALLIES; i++)
	{
		if (accuracymedals >= TIERS[i] * MULT(1)) tallies[2]++;
		else break;
	}

	for (i = 0; i < NUM_MP_TIERS_TALLIES; i++)
	{
		if (headshotmedals >= TIERS[i] * MULT(1)) tallies[3]++;
		else break;
	}

	for (i = 0; i < NUM_MP_TIERS_TALLIES; i++)
	{
		if (killmastermedals >= TIERS[i] * MULT(1)) tallies[4]++;
		else break;
	}

	for (i = 0; i < NUM_MP_TIERS_TALLIES; i++)
	{
		if (time >= TIERS[i] * MULT(1200)) tallies[5]++;
		else break;
	}

	for (i = 0; i < NUM_MP_TIERS_TALLIES; i++)
	{
		if (distance >= TIERS[i] * MULT(100)) tallies[6]++;
		else break;
	}

	for (i = 0; i < NUM_MP_TIERS_TALLIES; i++)
	{
		if (damagedealt >= TIERS[i] * MULT(1)) tallies[7]++;
		else break;
	}

	for (i = 0; i < NUM_MP_TIERS_TALLIES; i++)
	{
		if (ammoused >= TIERS[i] * MULT(500)) tallies[8]++;
		else break;
	}

	for (i = 0; i < NUM_MP_TIERS_TALLIES; i++)
	{
		if (survivormedals >= TIERS[i] * MULT(1)) tallies[9]++;
		else break;
	}

	int32_t sum = 0;

	for (i = 0; i < NUM_MP_TIERS_TALLIES; i++)
	{
		sum = sum + tallies[i];
	}

	if (sum > 100) sum = 100;
	MultiplayerTitles title = (MultiplayerTitles)(sum / 5);

	if (title > MultiplayerTitles::PERFECT) return MultiplayerTitles::PERFECT;
	return title;
}

#pragma endregion

#pragma region MultiplayerSettings

void MultiplayerSettings::Load(uint8_t* fileBuffer)
{
	PakFile::Load(fileBuffer);

	if (pakFileHeader.filelen != PACK_MPSETUP_SIZE + PACK_HEADER_SIZE)
		throw std::runtime_error(std::string("Unexpected size of \"MPSETUP\" file: ") + std::to_string(pakFileHeader.filelen) + ".");

	SaveBuffer buffer(&fileBuffer[PACK_HEADER_SIZE], SAVE_BUFFER_SIZE);

	buffer.ReadString(name);
	buffer.ReadBits(4);

	stagenum = buffer.ReadBits(7);
	scenario = buffer.ReadBits(3);
	scenarioParams = buffer.ReadBits(8);
	options = buffer.ReadBits(21);

	for (uint8_t i = 0; i < MAX_SIMULANTS; i++)
	{
		botsData[i].type = buffer.ReadBits(5);
		botsData[i].difficulty = buffer.ReadBits(3);
		botsData[i].mpheadnum = buffer.ReadBits(7);
		botsData[i].mpbodynum = buffer.ReadBits(7);
		botsData[i].team = buffer.ReadBits(3);
	}

	for (uint8_t i = 0; i < NUM_MPWEAPONSLOTS; i++)
	{
		weaponSlots[i] = buffer.ReadBits(7);
	}

	timelimit = buffer.ReadBits(6);
	scorelimit = buffer.ReadBits(7);
	teamscorelimit = buffer.ReadBits(9);

	for (uint8_t i = 0; i < MAX_PLAYERS; i++)
	{
		teams[i] = buffer.ReadBits(3);
	}
}

uint8_t MultiplayerSettings::GetArena() const
{
	for (uint8_t s = 0; s < NUM_MP_STAGES_AND_RANDOM; s++)
	{
		if (mpStageIndices[s] == stagenum) return s;
	}

	return 1; // Random
}

void MultiplayerSettings::SetArena(const uint8_t arena)
{
	stagenum = mpStageIndices[arena];
}

bool MultiplayerSettings::GetOptionsFlag(const MultiplayerSetupFlags flag) const
{
	return (options & (uint32_t)flag) != 0;
}

void MultiplayerSettings::SetOptionsFlag(const MultiplayerSetupFlags flag, const bool set)
{
	if (set) options |= (uint32_t)flag;
	else options &= ~(uint32_t)flag;
}

uint8_t MultiplayerSettings::GetSlowMotionMode() const
{
	if (GetOptionsFlag(MultiplayerSetupFlags::SLOWMOTION_SMART)) return 2;
	else if (GetOptionsFlag(MultiplayerSetupFlags::SLOWMOTION_ON)) return 1;
	return 0;
}

void MultiplayerSettings::SetSlowMotionMode(const uint8_t mode)
{
	SetOptionsFlag(MultiplayerSetupFlags::SLOWMOTION_SMART, false);
	SetOptionsFlag(MultiplayerSetupFlags::SLOWMOTION_ON, false);

	if (mode == 1) SetOptionsFlag(MultiplayerSetupFlags::SLOWMOTION_ON, true);
	else if (mode == 2) SetOptionsFlag(MultiplayerSetupFlags::SLOWMOTION_SMART, true);
}

uint8_t MultiplayerSettings::GetHillTime() const
{
	return scenarioParams + 10;
}

void MultiplayerSettings::SetHillTime(const uint8_t time)
{
	scenarioParams = time - 10;
}

#pragma endregion

#pragma region Terminator

void Terminator::Load(uint8_t* fileBuffer)
{
	PakFile::Load(fileBuffer);

	if (pakFileHeader.filelen != PACK_TERMINATOR_SIZE + PACK_HEADER_SIZE)
		throw std::runtime_error(std::string("Unexpected size of \"TERMINATOR\" file: ") + std::to_string(pakFileHeader.filelen) + ".");
}

void Terminator::Save(uint8_t* fileBuffer)
{
	// This never changes, so just copy the header and don't recalculate checksums
	memcpy(&fileBuffer[0], &pakFileHeader, PACK_HEADER_SIZE);
}

#pragma endregion

#pragma region SaveFile

void SaveFile::Load(uint8_t* fileBuffer)
{
	// Read headers

	printf("Position  Header CRC       Body CRC         Type (Name)            Size  ID  Used  Device  Generation  Written  Version\n");
	printf("-----------------------------------------------------------------------------------------------------------------------\n");

	uint8_t bossFilesCount = 0;
	uint8_t gameFilesCount = 0;
	uint8_t mpProfilesCount = 0;
	uint8_t mpSettingsCount = 0;

	int32_t p = 0;

	while (p < SAVE_FILE_SIZE)
	{
		PakFileHeader pakFileHeader = {};
		memcpy(&pakFileHeader, &fileBuffer[p], PACK_HEADER_SIZE);

		// Read actual data

		switch ((PakFileTypes)pakFileHeader.filetype)
		{
			case PakFileTypes::BOSS:
			{
				bossFiles[bossFilesCount++].Load(&fileBuffer[p]);
				break;
			}
			case PakFileTypes::MPPLAYER:
			{
				mpProfiles[mpProfilesCount++].Load(&fileBuffer[p]);
				break;
			}
			case PakFileTypes::MPSETUP:
			{
				mpSettings[mpSettingsCount++].Load(&fileBuffer[p]);
				break;
			}
			case PakFileTypes::GAME:
			{
				gameFiles[gameFilesCount++].Load(&fileBuffer[p]);
				break;
			}
			case PakFileTypes::TERMINATOR:
			{
				terminator.Load(&fileBuffer[p]);
				break;
			}
			default:
			{
				break;
			}
		}

		// Print debug data

		uint16_t checksum[2];
		CalculateChecksum(&fileBuffer[p + 8], &fileBuffer[p + PACK_HEADER_SIZE], checksum);
		char* headerSumResult = (pakFileHeader.headersum[0] == checksum[0] && pakFileHeader.headersum[1] == checksum[1]) ? "OK " : "BAD";

		CalculateChecksum(&fileBuffer[p + PACK_HEADER_SIZE], &fileBuffer[p + PACK_HEADER_SIZE + pakFileHeader.bodylen], checksum);
		char* bodySumResult = (pakFileHeader.bodysum[0] == checksum[0] && pakFileHeader.bodysum[1] == checksum[1]) ? "OK " : "BAD";

		char* type;

		switch ((PakFileTypes)pakFileHeader.filetype)
		{
			case PakFileTypes::UNUSED_001:
				type = "UNUSED_001           ";
				break;
			case PakFileTypes::BLANK:
				type = "BLANK                ";
				break;
			case PakFileTypes::TERMINATOR:
				type = "TERMINATOR           ";
				break;
			case PakFileTypes::CAMERA:
				type = "CAMERA               ";
				break;
			case PakFileTypes::BOSS:
				type = "BOSS                 ";
				break;
			case PakFileTypes::MPPLAYER:
			{
				char name[MAX_NAME_LENGTH + 1];
				memcpy(name, &fileBuffer[p + PACK_HEADER_SIZE], 10);
				name[10] = '\0';
				char gameName[32];
				snprintf(gameName, 32, "MPPLAYER (%-10s)", name);
				type = gameName;
				break;
			}
			case PakFileTypes::MPSETUP:
			{
				char name[MAX_NAME_LENGTH + 1];
				memcpy(name, &fileBuffer[p + PACK_HEADER_SIZE], 10);
				name[10] = '\0';
				char gameName[32];
				snprintf(gameName, 32, "MPSETUP  (%-10s)", name);
				type = gameName;
				break;
			}
			case PakFileTypes::GAME:
			{
				char name[MAX_NAME_LENGTH + 1];
				memcpy(name, &fileBuffer[p + PACK_HEADER_SIZE], 10);
				name[10] = '\0';
				char gameName[32];
				snprintf(gameName, 32, "GAME     (%-10s)", name);
				type = gameName;
				break;
			}
			default:
				type = "UNKNOWN              ";
				break;
		}

		printf("0x%04X    %04X-%04X (%s)  %04X-%04X (%s)  %s  %3u   %2u  %u     %4u    %3u         %u        %u\n", p, pakFileHeader.headersum[0], pakFileHeader.headersum[1], headerSumResult, pakFileHeader.bodysum[0], pakFileHeader.bodysum[1], bodySumResult, type, pakFileHeader.bodylen, pakFileHeader.fileid, pakFileHeader.occupied, pakFileHeader.deviceserial, pakFileHeader.generation, pakFileHeader.writecompleted, pakFileHeader.version);

		// Advance to next file

		p += pakFileHeader.filelen;
	}
}

void SaveFile::Save(uint8_t* fileBuffer)
{
	int32_t p = 0;

	for (uint8_t bf = 0; bf < ACTUAL_NUM_BOSS_FILE_SLOTS; bf++)
	{
		bossFiles[bf].Save(&fileBuffer[p]);
		p += bossFiles[bf].pakFileHeader.filelen;
	}

	for (uint8_t mpp = 0; mpp < ACTUAL_NUM_FILE_SLOTS; mpp++)
	{
		mpProfiles[mpp].Save(&fileBuffer[p]);
		p += mpProfiles[mpp].pakFileHeader.filelen;
	}

	for (uint8_t mps = 0; mps < ACTUAL_NUM_FILE_SLOTS; mps++)
	{
		mpSettings[mps].Save(&fileBuffer[p]);
		p += mpSettings[mps].pakFileHeader.filelen;
	}

	for (uint8_t gf = 0; gf < ACTUAL_NUM_FILE_SLOTS; gf++)
	{
		gameFiles[gf].Save(&fileBuffer[p]);
		p += gameFiles[gf].pakFileHeader.filelen;
	}

	if (p != SAVE_FILE_SIZE - PACK_TERMINATOR_ACTUAL_SIZE - PACK_HEADER_SIZE)
		throw std::runtime_error(std::string("Unexpected position of \"TERMINATOR\" file: ") + std::to_string(p) + ".");

	terminator.Save(&fileBuffer[p]);
}

void SaveFile::CalculateChecksum(uint8_t* start, uint8_t* end, uint16_t* checksum)
{
	uint8_t* ptr;
	uint32_t salt = 0;
	uint64_t seed = 0x8f809f473108b3c1;
	uint32_t sum1 = 0;
	uint32_t sum2 = 0;

	for (ptr = start; ptr < end; ptr++, salt += 7) {
		seed += (uint64_t)(*ptr) << (salt & 0x0f);
		sum1 ^= TransformSeed(&seed);
	}

	for (ptr = end - 1; ptr >= start; ptr--, salt += 3) {
		seed += (uint64_t)(*ptr) << (salt & 0x0f);
		sum2 ^= TransformSeed(&seed);
	}

	checksum[0] = sum1 & 0xffff;
	checksum[1] = sum2 & 0xffff;
}

uint32_t SaveFile::TransformSeed(uint64_t* seed)
{
	*seed = ((*seed << 63) >> 31 | (*seed << 31) >> 32) ^ (*seed << 44) >> 32;
	*seed = ((*seed >> 20) & 0xfff) ^ *seed;

	return (uint32_t)*seed;
}

#pragma endregion