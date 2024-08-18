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

uint32_t SaveBuffer::ReadBits(const int32_t numBits)
{
	if (numBits == 0) return 0;

	uint32_t bit = 1 << (numBits - 1);
	uint32_t value = 0;

	for (; bit; bit >>= 1) {
		int32_t bitindex = bitPosition % 8;
		uint8_t mask = 1 << (7 - bitindex);
		int32_t byteindex = bitPosition / 8;

		if (bytes[byteindex] & mask) {
			value |= bit;
		}

		bitPosition++;
	}

	return value;
}

void SaveBuffer::Or(const uint32_t value, const int32_t numBits)
{
	uint32_t bit = 1 << (numBits - 1);

	for (; bit; bit >>= 1)
	{
		if (bit & value)
		{
			int32_t bitindex = bitPosition % 8;
			uint8_t mask = 1 << (7 - bitindex);
			int32_t byteindex = bitPosition / 8;

			bytes[byteindex] |= mask;
		}

		bitPosition++;
	}
}

void SaveBuffer::ReadGuid(FileGuid* guid)
{
	guid->id = ReadBits(7);
	guid->deviceSerial = ReadBits(13);
}

void SaveBuffer::WriteGuid(FileGuid* guid)
{
	Or(guid->id, 7);
	Or(guid->deviceSerial, 13);
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

void SaveBuffer::Clear()
{
	bitPosition = 0;
	memset(bytes, 0, SAVE_BUFFER_SIZE);
}

#pragma endregion

#pragma region PakFileHeader

void PakFileHeader::Load(uint8_t* fileBuffer, const bool isBigEndian)
{
	if (isBigEndian)
	{
		headersum[0] = (fileBuffer[0] << 8) | fileBuffer[1];
		headersum[1] = (fileBuffer[2] << 8) | fileBuffer[3];
		bodysum[0] = (fileBuffer[4] << 8) | fileBuffer[5];
		bodysum[1] = (fileBuffer[6] << 8) | fileBuffer[7];

		filetype = ((fileBuffer[8] & 0xFF) << 1) | (fileBuffer[9] >> 7);
		bodylen = ((fileBuffer[9] & 0x7F) << 4) | (fileBuffer[10] >> 4);
		filelen = ((fileBuffer[10] & 0xF) << 8) | (fileBuffer[11] >> 0);
		deviceSerial = ((fileBuffer[12] & 0xFF) << 5) | (fileBuffer[13] >> 3); // Shouldn't this always be 0xbaa?
		id = ((fileBuffer[13] & 0x7) << 4) | (fileBuffer[14] >> 4);
		generation = ((fileBuffer[14] & 0xF) << 5) | (fileBuffer[15] >> 3);
		occupied = (fileBuffer[15] >> 2) & 1;
		writecompleted = (fileBuffer[15] >> 1) & 1;
		version = (fileBuffer[15] >> 0) & 1;
	}
	else
	{
		headersum[0] = (fileBuffer[1] << 8) | fileBuffer[0];
		headersum[1] = (fileBuffer[3] << 8) | fileBuffer[2];
		bodysum[0] = (fileBuffer[5] << 8) | fileBuffer[4];
		bodysum[1] = (fileBuffer[7] << 8) | fileBuffer[6];

		filetype = (fileBuffer[8] >> 0) | ((fileBuffer[9] & 0x1) << 8);
		bodylen = (fileBuffer[9] >> 1) | ((fileBuffer[10] & 0xF) << 7);
		filelen = (fileBuffer[10] >> 4) | ((fileBuffer[11] & 0xFF) << 4);
		deviceSerial = (fileBuffer[12] >> 0) | ((fileBuffer[13] & 0x1F) << 8);
		id = (fileBuffer[13] >> 5) | ((fileBuffer[14] & 0xF) << 3);
		generation = (fileBuffer[14] >> 4) | ((fileBuffer[15] & 0x1F) << 4);
		occupied = (fileBuffer[15] >> 5) & 1;
		writecompleted = (fileBuffer[15] >> 6) & 1;
		version = (fileBuffer[15] >> 7) & 1;
	}
}

void PakFileHeader::Save(uint8_t* fileBuffer, const bool isBigEndian)
{
	if (occupied)
	{
		SaveFile::CalculateChecksum(&fileBuffer[PACK_HEADER_SIZE], &fileBuffer[PACK_HEADER_SIZE + bodylen], bodysum);
	}
	else
	{
		bodysum[0] = 0xFFFF;
		bodysum[1] = 0xFFFF;
	}

	if (isBigEndian)
	{
		fileBuffer[4] = bodysum[0] >> 8;
		fileBuffer[5] = bodysum[0] & 0xFF;
		fileBuffer[6] = bodysum[1] >> 8;
		fileBuffer[7] = bodysum[1] & 0xFF;

		fileBuffer[8] = filetype >> 1;
		fileBuffer[9] = (filetype & 0x1) << 7;

		fileBuffer[9] |= bodylen >> 4;
		fileBuffer[10] = (bodylen & 0xF) << 4;

		fileBuffer[10] |= filelen >> 8;
		fileBuffer[11] = (filelen & 0xFF) << 0;

		fileBuffer[12] = deviceSerial >> 5;
		fileBuffer[13] = (deviceSerial & 0x1F) << 3;

		fileBuffer[13] |= id >> 4;
		fileBuffer[14] = (id & 0xF) << 4;

		fileBuffer[14] |= generation >> 5;
		fileBuffer[15] = (generation & 0x1F) << 3;

		fileBuffer[15] |= occupied << 2;
		fileBuffer[15] |= writecompleted << 1;
		fileBuffer[15] |= version << 0;
	}
	else
	{
		fileBuffer[4] = bodysum[0] & 0xFF;
		fileBuffer[5] = bodysum[0] >> 8;
		fileBuffer[6] = bodysum[1] & 0xFF;
		fileBuffer[7] = bodysum[1] >> 8;

		fileBuffer[8] = filetype & 0xFF;
		fileBuffer[9] = filetype >> 8;

		fileBuffer[9] |= (bodylen & 0x7F) << 1;
		fileBuffer[10] = bodylen >> 7;

		fileBuffer[10] |= (filelen & 0xF) << 4;
		fileBuffer[11] = filelen >> 4;

		fileBuffer[12] = (deviceSerial & 0xFF) << 0;
		fileBuffer[13] = deviceSerial >> 8;

		fileBuffer[13] |= (id & 0x7) << 5;
		fileBuffer[14] = id >> 3;

		fileBuffer[14] |= (generation & 0xF) << 4;
		fileBuffer[15] = generation >> 4;

		fileBuffer[15] |= (occupied & 0x1) << 5;
		fileBuffer[15] |= (writecompleted & 0x1) << 6;
		fileBuffer[15] |= (version & 0x1) << 7;
	}

	SaveFile::CalculateChecksum(&fileBuffer[8], &fileBuffer[PACK_HEADER_SIZE], headersum);

	if (isBigEndian)
	{
		fileBuffer[0] = headersum[0] >> 8;
		fileBuffer[1] = headersum[0] & 0xFF;
		fileBuffer[2] = headersum[1] >> 8;
		fileBuffer[3] = headersum[1] & 0xFF;
	}
	else
	{
		fileBuffer[0] = headersum[0] & 0xFF;
		fileBuffer[1] = headersum[0] >> 8;
		fileBuffer[2] = headersum[1] & 0xFF;
		fileBuffer[3] = headersum[1] >> 8;
	}
}

#pragma endregion

#pragma region PakFile

void PakFile::Load(uint8_t* fileBuffer, const bool isBigEndian)
{
	pakFileHeader.Load(&fileBuffer[0], isBigEndian);

	uint16_t headerChecksum[2];
	SaveFile::CalculateChecksum(&fileBuffer[8], &fileBuffer[PACK_HEADER_SIZE], headerChecksum);

	uint16_t bodyChecksum[2];
	SaveFile::CalculateChecksum(&fileBuffer[PACK_HEADER_SIZE], &fileBuffer[PACK_HEADER_SIZE + pakFileHeader.bodylen], bodyChecksum);

	isChecksumValid = pakFileHeader.headersum[0] == headerChecksum[0] && pakFileHeader.headersum[1] == headerChecksum[1] &&
		pakFileHeader.bodysum[0] == bodyChecksum[0] && pakFileHeader.bodysum[1] == bodyChecksum[1];
}

void PakFile::Save(uint8_t* fileBuffer, const bool isBigEndian)
{
	pakFileHeader.Save(&fileBuffer[0], isBigEndian);
	isChecksumValid = pakFileHeader.occupied;
}

#pragma endregion

#pragma region BossFile

void BossFile::Load(uint8_t* fileBuffer, const bool isBigEndian)
{
	PakFile::Load(fileBuffer, isBigEndian);

	if (pakFileHeader.filelen != PACK_BOSS_SIZE + PACK_HEADER_SIZE)
		throw std::runtime_error(std::string("Unexpected size of \"BOSS\" file: ") + std::to_string(pakFileHeader.filelen) + ".");

	SaveBuffer buffer(&fileBuffer[PACK_HEADER_SIZE], SAVE_BUFFER_SIZE);

	buffer.ReadGuid(&guid);

	unk1 = buffer.ReadBits(1);
	language = buffer.ReadBits(4);

	for (int32_t tn = 0; tn < NUM_MP_TEAMS; tn++)
	{
		buffer.ReadString(teamNames[tn]);
	}

	tracknum = buffer.ReadBits(8);

	for (int32_t i = 0; i < 6; i++)
	{
		multipletracknums[i] = buffer.ReadBits(8);
	}

	usingmultipletunes = buffer.ReadBits(1);
	altTitleUnlocked = buffer.ReadBits(1);
	altTitleEnabled = buffer.ReadBits(1);
}

void BossFile::Save(uint8_t* fileBuffer, const bool isBigEndian)
{
	SaveBuffer buffer;

	buffer.WriteGuid(&guid);

	buffer.Or(unk1, 1);
	buffer.Or(language, 4);

	for (int32_t tn = 0; tn < NUM_MP_TEAMS; tn++)
	{
		buffer.WriteString(teamNames[tn]);
	}

	if (tracknum == 255) buffer.Or(255, 8);
	else buffer.Or(tracknum, 8);

	for (int32_t i = 0; i < 6; i++)
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

	PakFile::Save(fileBuffer, isBigEndian);
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

void GameFile::Load(uint8_t* fileBuffer, const bool isBigEndian)
{
	PakFile::Load(fileBuffer, isBigEndian);

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
		int32_t numBits = i == 8 ? 2 : 8;
		firingrangescores[i] = buffer.ReadBits(numBits);
	}

	for (uint8_t i = 0; i < 4; i++)
	{
		weaponsfound[i] = buffer.ReadBits(8);
	}
}

void GameFile::Save(uint8_t* fileBuffer, const bool isBigEndian)
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

	PakFile::Save(fileBuffer, isBigEndian);
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

void MultiplayerProfile::Load(uint8_t* fileBuffer, const bool isBigEndian)
{
	PakFile::Load(fileBuffer, isBigEndian);

	if (pakFileHeader.filelen != PACK_MPPLAYER_SIZE + PACK_HEADER_SIZE)
		throw std::runtime_error(std::string("Unexpected size of \"MPPLAYER\" file: ") + std::to_string(pakFileHeader.filelen) + ".");

	SaveBuffer buffer(&fileBuffer[PACK_HEADER_SIZE], SAVE_BUFFER_SIZE);

	buffer.ReadString(name);
	time = buffer.ReadBits(28);
	headIndex = buffer.ReadBits(7);
	bodyIndex = buffer.ReadBits(7);
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
		int32_t numBits = bitsremaining;
		if (numBits > 8) numBits = 8;

		gunfuncs[w] = buffer.ReadBits(numBits);

		bitsremaining -= 8;
		w++;
	}
}

void MultiplayerProfile::Save(uint8_t* fileBuffer, const bool isBigEndian)
{
	SaveBuffer buffer;

	buffer.WriteString(name);
	buffer.Or(time, 28);
	buffer.Or(headIndex, 7);
	buffer.Or(bodyIndex, 7);
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
		int32_t numBits = bitsremaining;
		if (numBits > 8) numBits = 8;

		buffer.Or(gunfuncs[w], numBits);

		bitsremaining -= 8;
		w++;
	}

	uint8_t* bytes = buffer.GetBytes();
	for (uint8_t i = 0; i < pakFileHeader.filelen - PACK_HEADER_SIZE; i++)
	{
		fileBuffer[PACK_HEADER_SIZE + i] = bytes[i % pakFileHeader.bodylen];
	}

	PakFile::Save(fileBuffer, isBigEndian);
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

	if (title > MultiplayerTitles::Perfect) return MultiplayerTitles::Perfect;
	return title;
}

#pragma endregion

#pragma region MultiplayerSetup

void MultiplayerSetup::Load(uint8_t* fileBuffer, const bool isBigEndian)
{
	PakFile::Load(fileBuffer, isBigEndian);

	if (pakFileHeader.filelen != PACK_MPSETUP_SIZE + PACK_HEADER_SIZE)
		throw std::runtime_error(std::string("Unexpected size of \"MPSETUP\" file: ") + std::to_string(pakFileHeader.filelen) + ".");

	SaveBuffer buffer(&fileBuffer[PACK_HEADER_SIZE], SAVE_BUFFER_SIZE);

	buffer.ReadString(name);
	numsims = buffer.ReadBits(4);
	stagenum = buffer.ReadBits(7);
	scenario = buffer.ReadBits(3);
	hillTime = buffer.ReadBits(8);
	options = buffer.ReadBits(21);

	for (uint8_t s = 0; s < MAX_SIMULANTS; s++)
	{
		botsData[s].type = buffer.ReadBits(5);
		botsData[s].difficulty = buffer.ReadBits(3);
		botsData[s].headIndex = buffer.ReadBits(7);
		botsData[s].bodyIndex = buffer.ReadBits(7);
		botsData[s].team = buffer.ReadBits(3);
	}

	for (uint8_t ws = 0; ws < NUM_MP_WEAPONSLOTS; ws++)
	{
		weaponSlots[ws] = buffer.ReadBits(7);
	}

	timelimit = buffer.ReadBits(6);
	scorelimit = buffer.ReadBits(7);
	teamscorelimit = buffer.ReadBits(9);

	for (uint8_t p = 0; p < MAX_PLAYERS; p++)
	{
		teams[p] = buffer.ReadBits(3);
	}
}

void MultiplayerSetup::Save(uint8_t* fileBuffer, const bool isBigEndian)
{
	SaveBuffer buffer;

	buffer.WriteString(name);
	buffer.Or(numsims, 4);
	buffer.Or(stagenum, 7);
	buffer.Or(scenario, 3);
	buffer.Or(hillTime, 8);
	buffer.Or(options, 21);

	for (uint8_t s = 0; s < MAX_SIMULANTS; s++)
	{
		buffer.Or(botsData[s].type, 5);
		buffer.Or(botsData[s].difficulty, 3);
		buffer.Or(botsData[s].headIndex, 7);
		buffer.Or(botsData[s].bodyIndex, 7);
		buffer.Or(botsData[s].team, 3);
	}

	for (uint8_t ws = 0; ws < NUM_MP_WEAPONSLOTS; ws++)
	{
		buffer.Or(weaponSlots[ws], 7);
	}

	buffer.Or(timelimit, 6);
	buffer.Or(scorelimit, 7);
	buffer.Or(teamscorelimit, 9);

	for (uint8_t p = 0; p < MAX_PLAYERS; p++)
	{
		buffer.Or(teams[p], 3);
	}

	uint8_t* bytes = buffer.GetBytes();
	for (uint8_t i = 0; i < pakFileHeader.filelen - PACK_HEADER_SIZE; i++)
	{
		fileBuffer[PACK_HEADER_SIZE + i] = bytes[i % pakFileHeader.bodylen];
	}

	PakFile::Save(fileBuffer, isBigEndian);
}

uint8_t MultiplayerSetup::GetArena() const
{
	for (uint8_t s = 0; s < NUM_MP_STAGES_AND_RANDOM; s++)
	{
		if (mpStageIndices[s] == stagenum) return s;
	}

	return 1; // Random
}

void MultiplayerSetup::SetArena(const uint8_t arena)
{
	stagenum = mpStageIndices[arena];
}

bool MultiplayerSetup::GetOptionsFlag(const MultiplayerSetupFlags flag) const
{
	return (options & (uint32_t)flag) != 0;
}

void MultiplayerSetup::SetOptionsFlag(const MultiplayerSetupFlags flag, const bool set)
{
	if (set) options |= (uint32_t)flag;
	else options &= ~(uint32_t)flag;
}

uint8_t MultiplayerSetup::GetSlowMotionMode() const
{
	if (GetOptionsFlag(MultiplayerSetupFlags::SLOWMOTION_SMART)) return 2;
	else if (GetOptionsFlag(MultiplayerSetupFlags::SLOWMOTION_ON)) return 1;
	return 0;
}

void MultiplayerSetup::SetSlowMotionMode(const uint8_t mode)
{
	SetOptionsFlag(MultiplayerSetupFlags::SLOWMOTION_SMART, false);
	SetOptionsFlag(MultiplayerSetupFlags::SLOWMOTION_ON, false);

	if (mode == 1) SetOptionsFlag(MultiplayerSetupFlags::SLOWMOTION_ON, true);
	else if (mode == 2) SetOptionsFlag(MultiplayerSetupFlags::SLOWMOTION_SMART, true);
}

uint8_t MultiplayerSetup::GetHillTime() const
{
	return hillTime + 10;
}

void MultiplayerSetup::SetHillTime(const uint8_t time)
{
	hillTime = time - 10;
}

#pragma endregion

#pragma region Terminator

void Terminator::Load(uint8_t* fileBuffer, const bool isBigEndian)
{
	PakFile::Load(fileBuffer, isBigEndian);

	if (pakFileHeader.filelen != PACK_TERMINATOR_SIZE + PACK_HEADER_SIZE)
		throw std::runtime_error(std::string("Unexpected size of \"TERMINATOR\" file: ") + std::to_string(pakFileHeader.filelen) + ".");
}

void Terminator::Save(uint8_t* fileBuffer, const bool isBigEndian)
{
	PakFile::Save(fileBuffer, isBigEndian);
}

#pragma endregion

#pragma region SaveFile

void SaveFile::Load(uint8_t* fileBuffer, const bool isBigEndian)
{
	PrintFileInfo(fileBuffer, isBigEndian);

	// Read headers

	uint8_t bossFilesCount = 0;
	uint8_t gameFilesCount = 0;
	uint8_t mpProfilesCount = 0;
	uint8_t mpSetupsCount = 0;

	int32_t p = 0;

	while (p < SAVE_FILE_SIZE)
	{
		PakFileHeader pakFileHeader = {};
		pakFileHeader.Load(&fileBuffer[p], isBigEndian);

		// Read data

		switch ((PakFileTypes)pakFileHeader.filetype)
		{
			case PakFileTypes::BOSS:
			{
				bossFiles[bossFilesCount++].Load(&fileBuffer[p], isBigEndian);
				break;
			}
			case PakFileTypes::MPPLAYER:
			{
				mpProfiles[mpProfilesCount++].Load(&fileBuffer[p], isBigEndian);
				break;
			}
			case PakFileTypes::MPSETUP:
			{
				mpSetups[mpSetupsCount++].Load(&fileBuffer[p], isBigEndian);
				break;
			}
			case PakFileTypes::GAME:
			{
				gameFiles[gameFilesCount++].Load(&fileBuffer[p], isBigEndian);
				break;
			}
			case PakFileTypes::TERMINATOR:
			{
				terminator.Load(&fileBuffer[p], isBigEndian);
				break;
			}
			default:
			{
				break;
			}
		}

		// Advance to next file

		p += pakFileHeader.filelen;
	}
}

void SaveFile::Save(uint8_t* fileBuffer, const bool isBigEndian)
{
	int32_t p = 0;

	for (uint8_t bf = 0; bf < ACTUAL_NUM_BOSS_FILE_SLOTS; bf++)
	{
		bossFiles[bf].Save(&fileBuffer[p], isBigEndian);
		p += bossFiles[bf].pakFileHeader.filelen;
	}

	for (uint8_t mpp = 0; mpp < ACTUAL_NUM_FILE_SLOTS; mpp++)
	{
		mpProfiles[mpp].Save(&fileBuffer[p], isBigEndian);
		p += mpProfiles[mpp].pakFileHeader.filelen;
	}

	for (uint8_t mps = 0; mps < ACTUAL_NUM_FILE_SLOTS; mps++)
	{
		mpSetups[mps].Save(&fileBuffer[p], isBigEndian);
		p += mpSetups[mps].pakFileHeader.filelen;
	}

	for (uint8_t gf = 0; gf < ACTUAL_NUM_FILE_SLOTS; gf++)
	{
		gameFiles[gf].Save(&fileBuffer[p], isBigEndian);
		p += gameFiles[gf].pakFileHeader.filelen;
	}

	if (p != SAVE_FILE_SIZE - PACK_TERMINATOR_ACTUAL_SIZE - PACK_HEADER_SIZE)
		throw std::runtime_error(std::string("Unexpected position of \"TERMINATOR\" file: ") + std::to_string(p) + ".");

	terminator.Save(&fileBuffer[p], isBigEndian);
}

uint8_t SaveFile::GetGameFileCount() const
{
	uint8_t count = 0;
	for (uint8_t f = 0; f < ACTUAL_NUM_FILE_SLOTS; f++)
	{
		if (gameFiles[f].IsUsed()) count++;
	}

	return count;
}

uint8_t SaveFile::GetMultiplayerProfileCount() const
{
	uint8_t count = 0;
	for (uint8_t f = 0; f < ACTUAL_NUM_FILE_SLOTS; f++)
	{
		if (mpProfiles[f].IsUsed()) count++;
	}

	return count;
}

uint8_t SaveFile::GetMultiplayerSetupCount() const
{
	uint8_t count = 0;
	for (uint8_t f = 0; f < ACTUAL_NUM_FILE_SLOTS; f++)
	{
		if (mpSetups[f].IsUsed()) count++;
	}

	return count;
}

GameFile* SaveFile::GetFirstUnusedGameFile(uint8_t* outFileIndex)
{
	for (uint8_t f = 0; f < ACTUAL_NUM_FILE_SLOTS; f++)
	{
		if (!gameFiles[f].IsUsed())
		{
			*outFileIndex = f;
			return &gameFiles[f];
		}
	}

	return nullptr;
}

MultiplayerProfile* SaveFile::GetFirstUnusedMultiplayerProfile(uint8_t* outFileIndex)
{
	for (uint8_t f = 0; f < ACTUAL_NUM_FILE_SLOTS; f++)
	{
		if (!mpProfiles[f].IsUsed())
		{
			*outFileIndex = f;
			return &mpProfiles[f];
		}
	}

	return nullptr;
}

MultiplayerSetup* SaveFile::GetFirstUnusedMultiplayerSetup(uint8_t* outFileIndex)
{
	for (uint8_t f = 0; f < ACTUAL_NUM_FILE_SLOTS; f++)
	{
		if (!mpSetups[f].IsUsed())
		{
			*outFileIndex = f;
			return &mpSetups[f];
		}
	}

	return nullptr;
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

void SaveFile::PrintFileInfo(uint8_t* fileBuffer, const bool isBigEndian) const
{
	printf("Position  Header CRC       Body CRC         Type (Name)            Size  ID  Used  Device  Generation  Written  Version\n");
	printf("-----------------------------------------------------------------------------------------------------------------------\n");

	uint8_t bossFilesCount = 0;
	uint8_t gameFilesCount = 0;
	uint8_t mpProfilesCount = 0;
	uint8_t mpSetupsCount = 0;

	int32_t p = 0;

	while (p < SAVE_FILE_SIZE)
	{
		PakFileHeader pakFileHeader = {};
		pakFileHeader.Load(&fileBuffer[p], isBigEndian);

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
			{
				type = "UNUSED_001           ";
				break;
			}
			case PakFileTypes::BLANK:
			{
				type = "BLANK                ";
				break;
			}
			case PakFileTypes::TERMINATOR:
			{
				type = "TERMINATOR           ";
				break;
			}
			case PakFileTypes::CAMERA:
			{
				type = "CAMERA               ";
				break;
			}
			case PakFileTypes::BOSS:
			{
				type = "BOSS                 ";
				break;
			}
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
			{
				type = "UNKNOWN              ";
				break;
			}
		}

		printf("0x%04X    %04X-%04X (%s)  %04X-%04X (%s)  %s  %3u   %2u  %u     %4u    %3u         %u        %u\n", p, pakFileHeader.headersum[0], pakFileHeader.headersum[1], headerSumResult, pakFileHeader.bodysum[0], pakFileHeader.bodysum[1], bodySumResult, type, pakFileHeader.bodylen, pakFileHeader.id, pakFileHeader.occupied, pakFileHeader.deviceSerial, pakFileHeader.generation, pakFileHeader.writecompleted, pakFileHeader.version);

		// Advance to next file

		p += pakFileHeader.filelen;
	}

	printf("\n");
}

uint32_t SaveFile::TransformSeed(uint64_t* seed)
{
	*seed = ((*seed << 63) >> 31 | (*seed << 31) >> 32) ^ (*seed << 44) >> 32;
	*seed = ((*seed >> 20) & 0xfff) ^ *seed;

	return (uint32_t)*seed;
}

#pragma endregion