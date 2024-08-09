#include "SaveFile.h"
#include <stdexcept>
#include "Utils.h"

#pragma region SaveSlot

//uint32_t SaveSlot::CalculateChecksum() const
//{
//	return SaveFile::CalculateChecksum((uint8_t*)&Magic, (uint8_t*)&Checksum);
//}

//void SaveSlot::UpdateChecksum(const bool endianSwap)
//{
//	const uint32_t checksum = CalculateChecksum();
//	Checksum = endianSwap ? Utils::Swap32(checksum) : checksum;
//}
//
//bool SaveSlot::IsValid(const bool endianSwap) const
//{
//	const uint32_t checksum = CalculateChecksum();
//	return Checksum == (endianSwap ? Utils::Swap32(checksum) : checksum);
//}

uint8_t SaveSlot::GetMagic() const
{
	return Magic;
}

uint8_t SaveSlot::GetSlotIndex() const
{
	if (SlotIndex == 2) return 3;
	if (SlotIndex == 3) return 2;
	return SlotIndex;
}

void SaveSlot::SetSlotIndex(const uint8_t slotIndex)
{
	if (slotIndex < 1 || slotIndex > 3) return;

	uint8_t index = slotIndex;
	if (index == 2) index = 3;
	else if (index == 3) index = 2;

	SlotIndex = index;
}

bool SaveSlot::GetJiggy(const uint8_t jiggy) const
{
	return (Jiggies[(jiggy - 1) / 8] & (1 << (jiggy & 7))) != 0;
}

void SaveSlot::SetJiggy(const uint8_t jiggy, bool value)
{
	if (value) Jiggies[(jiggy - 1) / 8] |= (1 << (jiggy & 7));
	else Jiggies[(jiggy - 1) / 8] &= ~(1 << (jiggy & 7));
}

bool SaveSlot::GetHoneycomb(const uint8_t honeycomb) const
{
	return (Honeycombs[(honeycomb - 1) / 8] & (1 << (honeycomb & 7))) != 0;
}

void SaveSlot::SetHoneycomb(const uint8_t honeycomb, bool value)
{
	if (value) Honeycombs[(honeycomb - 1) / 8] |= (1 << (honeycomb & 7));
	else Honeycombs[(honeycomb - 1) / 8] &= ~(1 << (honeycomb & 7));
}

bool SaveSlot::GetMumboToken(const uint8_t token) const
{
	return (MumboTokens[(token - 1) / 8] & (1 << (token & 7))) != 0;
}

void SaveSlot::SetMumboToken(const uint8_t token, bool value)
{
	if (value) MumboTokens[(token - 1) / 8] |= (1 << (token & 7));
	else MumboTokens[(token - 1) / 8] &= ~(1 << (token & 7));
}

uint8_t SaveSlot::GetNotes(const uint8_t level) const
{
	if (!levelHasNotes[level]) return 0;

	const uint64_t* noteValuesPtr = reinterpret_cast<const uint64_t*>(NoteScores);
	const int8_t bitOffset = levelNotesBitOffsets[level];

	return (Utils::Swap64(*noteValuesPtr) >> bitOffset) & 0x7F;
}

void SaveSlot::SetNotes(const uint8_t level, const uint8_t value) const
{
	if (!levelHasNotes[level]) return;

	uint64_t* noteValuesPtr = const_cast<uint64_t*>(reinterpret_cast<const uint64_t*>(NoteScores));
	const int8_t bitOffset = levelNotesBitOffsets[level];

	const uint64_t mask = static_cast<uint64_t>(0x7f) << bitOffset;
	*noteValuesPtr &= Utils::Swap64(~mask);
	*noteValuesPtr |= Utils::Swap64(static_cast<uint64_t>(value & 0x7f) << bitOffset);
}

uint16_t SaveSlot::GetPlayTime(const uint8_t level, const bool endianSwap) const
{
	uint16_t value = Times[levelIndices[level]];
	return endianSwap ? Utils::Swap16(value) : value;
}

void SaveSlot::SetPlayTime(const uint8_t level, const uint16_t value, const bool endianSwap)
{
	Times[levelIndices[level]] = endianSwap ? Utils::Swap16(value) : value;
}

bool SaveSlot::GetProgressFlag(const ProgressFlags flag) const
{
	const uint8_t index = static_cast<uint8_t>(flag);
	return Flags[index / 8] & (1 << (index & 7));
}

void SaveSlot::SetProgressFlag(const ProgressFlags flag, const bool value)
{
	const uint8_t index = static_cast<uint8_t>(flag);
	if (value) Flags[index / 8] |= (1 << (index & 7));
	else Flags[index / 8] &= ~(1 << (index & 7));
}

uint8_t SaveSlot::GetProgressValue(const ProgressFlags flag, const uint8_t bitsCount) const
{
	uint8_t value = 0;
	const uint8_t index = static_cast<uint8_t>(flag);

	for (uint8_t i = 0; i < bitsCount; i++)
	{
		value |= (GetProgressFlag(static_cast<ProgressFlags>(index + i)) << i);
	}

	return value;
}

void SaveSlot::SetProgressValue(const ProgressFlags flag, const uint8_t bitsCount, const uint8_t value)
{
	const uint8_t index = static_cast<uint8_t>(flag);

	for (uint8_t i = 0; i < bitsCount; i++)
	{
		SetProgressFlag(static_cast<ProgressFlags>(index + i), (value >> i) & 1);
	}
}

uint8_t SaveSlot::GetHeldItem(const HeldItems heldItem) const
{
	return Items[static_cast<int>(heldItem)];
}

void SaveSlot::SetHeldItem(const HeldItems heldItem, const uint8_t value)
{
	Items[static_cast<int>(heldItem)] = value;
}

bool SaveSlot::GetLearnedAbility(const Abilities ability) const
{
	const uint8_t index = static_cast<uint8_t>(ability);
	uint32_t* abilityValuesPtr = const_cast<uint32_t*>(reinterpret_cast<const uint32_t*>(LearnedAbilities));

	return Utils::Swap32(*abilityValuesPtr) & (1 << index);
}

void SaveSlot::SetLearnedAbility(const Abilities ability, const bool value)
{
	const uint8_t index = static_cast<uint8_t>(ability);
	uint32_t* abilityValuesPtr = const_cast<uint32_t*>(reinterpret_cast<const uint32_t*>(LearnedAbilities));

	if (value) *abilityValuesPtr |= Utils::Swap32(1 << index);
	else *abilityValuesPtr &= Utils::Swap32(~(1 << index));
}

bool SaveSlot::GetUsedAbility(const Abilities ability) const
{
	const uint8_t index = static_cast<uint8_t>(ability);
	uint32_t* abilityValuesPtr = const_cast<uint32_t*>(reinterpret_cast<const uint32_t*>(UsedAbilities));

	return Utils::Swap32(*abilityValuesPtr) & (1 << index);
}

void SaveSlot::SetUsedAbility(const Abilities ability, const bool value)
{
	const uint8_t index = static_cast<uint8_t>(ability);
	uint32_t* abilityValuesPtr = const_cast<uint32_t*>(reinterpret_cast<const uint32_t*>(UsedAbilities));

	if (value) *abilityValuesPtr |= Utils::Swap32(1 << index);
	else *abilityValuesPtr &= Utils::Swap32(~(1 << index));
}

uint32_t SaveSlot::GetChecksum(const bool endianSwap) const
{
	return endianSwap ? Utils::Swap32(Checksum) : Checksum;
}

#pragma endregion

#pragma region GlobalData

//uint32_t GlobalData::CalculateChecksum() const
//{
//	return SaveFile::CalculateChecksum((uint8_t*)&SnsItems, (uint8_t*)&Checksum);
//}

//void GlobalData::UpdateChecksum(const bool endianSwap)
//{
//	const uint32_t checksum = CalculateChecksum();
//	Checksum = endianSwap ? Utils::Swap32(checksum) : checksum;
//}
//
//bool GlobalData::IsValid(const bool endianSwap) const
//{
//	const uint32_t checksum = CalculateChecksum();
//	return Checksum == (endianSwap ? Utils::Swap32(checksum) : checksum);
//}

bool GlobalData::GetSnsItem(const SnS snsItem) const
{
	const uint8_t index = static_cast<uint8_t>(snsItem);
	return Utils::Swap32(SnsItems) & (1 << index);
}

void GlobalData::SetSnsItem(const SnS snsItem, const bool value)
{
	const uint8_t index = static_cast<uint8_t>(snsItem);

	if (value) SnsItems |= Utils::Swap32(1 << index);
	else SnsItems &= Utils::Swap32(~(1 << index));
}

uint32_t GlobalData::GetChecksum(const bool endianSwap) const
{
	return endianSwap ? Utils::Swap32(Checksum) : Checksum;
}

#pragma endregion

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

void SaveBuffer::ReadString(char* dst, const bool addlinebreak)
{
	bool foundnull = false;
	int32_t index = 0;

	for (int32_t i = 0; i < 10; i++)
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

	if (addlinebreak) dst[++index] = '\n';
	dst[++index] = '\0';
}

void SaveBuffer::Clear()
{
	bitpos = 0;
	memset(bytes, 0, SAVE_BUFFER_SIZE);
}

#pragma endregion

#pragma region SaveFile

//SaveSlot* SaveFile::GetRawSaveSlot(const uint8_t slotIndex)
//{
//	return &saveSlots[slotIndex];
//}
//
//SaveSlot* SaveFile::GetSaveSlot(const uint8_t slotIndex)
//{
//	for (uint8_t s = 0; s < TOTAL_NUM_SAVE_SLOTS; s++)
//	{
//		if (saveSlots[s].GetMagic() != SAVE_SLOT_MAGIC) continue;
//		if (saveSlots[s].GetSlotIndex() == slotIndex + 1) return &saveSlots[s];
//	}
//
//	return nullptr;
//}
//
//GlobalData* SaveFile::GetGlobalData()
//{
//	return &globalData;
//}

void SaveFile::Load(uint8_t* fileBuffer)
{
	// Read header

	memcpy(&pakFileHeader, &fileBuffer[0], sizeof(PakFileHeader));

	uint16_t checksum[2];
	CalculateChecksumU16Pair(&fileBuffer[8], &fileBuffer[16], checksum);

	if (pakFileHeader.headersum[0] != checksum[0] || pakFileHeader.headersum[1] != checksum[1])
	{
		// Bad checksum
	}

	// Read "boss file"

	SaveBuffer buffer(&fileBuffer[16], SAVE_BUFFER_SIZE);

	buffer.ReadGuid(&bossFile.guid);

	bossFile.unk1 = buffer.ReadBits(1);
	bossFile.language = buffer.ReadBits(4);

	for (int32_t t = 0; t < TEAM_NAMES_COUNT; t++)
	{
		buffer.ReadString(bossFile.teamNames[t], true);
	}

	bossFile.tracknum = buffer.ReadBits(8);

	for (int32_t i = 0; i < MULTIPLE_TRACKS_COUNT; i++)
	{
		bossFile.multipletracknums[i] = buffer.ReadBits(8);
	}

	bossFile.usingmultipletunes = buffer.ReadBits(1);
	bossFile.altTitleUnlocked = buffer.ReadBits(1);
	bossFile.altTitleEnabled = buffer.ReadBits(1);
}

void SaveFile::CalculateChecksumU16Pair(uint8_t* start, uint8_t* end, uint16_t* checksum)
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

	return *seed;
}

#pragma endregion