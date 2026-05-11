#include "SaveFile.h"

#include "../Utils.h"
#include "SaveData.h"

SaveFile::SaveFile()
{
	saveData = nullptr;

	filePath.clear();
	fileName.clear();
	fileType = SaveFileTypes::NotValid;
}

SaveFile::~SaveFile()
{
	if (saveData != nullptr)
	{
		delete saveData;
		saveData = nullptr;
	}
}

std::string SaveFile::Read(std::ifstream& stream, const size_t streamSize)
{
	// Read the whole file

	uint8_t fileBuffer[SAVE_DATA_SIZE] = {};

	stream.seekg(0, std::ios_base::beg);
	stream.read((char*)&fileBuffer, SAVE_DATA_SIZE);

	// Check what type of file is and initialize

	fileType = CalculateType(fileBuffer);

	if (fileType == SaveFileTypes::NotValid)
	{
		return "The selected file is not a valid Perfect Dark save file.";
	}

	// Create and load the SaveData struct

	SaveData* newSaveData = new SaveData();
	newSaveData->Load(fileBuffer, fileType == SaveFileTypes::BigEndian);

	// Assign references to the loaded data before finishing

	saveData = newSaveData;

	return "";
}

void SaveFile::Write(std::ofstream& stream)
{
	uint8_t fileBuffer[SAVE_DATA_SIZE];
	memset(fileBuffer, 0, SAVE_DATA_SIZE);

	saveData->Save(fileBuffer, fileType == SaveFileTypes::BigEndian);

	stream.write((char*)fileBuffer, SAVE_DATA_SIZE);
}

SaveFileTypes SaveFile::CalculateType(uint8_t* fileBuffer)
{
	uint16_t headersum[2] = {};
	headersum[0] = (fileBuffer[1] << 8) | fileBuffer[0];
	headersum[1] = (fileBuffer[3] << 8) | fileBuffer[2];

	uint16_t checksum[2];
	SaveData::CalculateChecksum(&fileBuffer[8], &fileBuffer[16], checksum);

#if SUPPORT_PC_SAVES
	if (headersum[0] == checksum[0] && headersum[1] == checksum[1]) return SaveFileTypes::LittleEndian;
#endif
	if (Utils::Swap16(headersum[0]) == checksum[0] && Utils::Swap16(headersum[1]) == checksum[1]) return SaveFileTypes::BigEndian;

	return SaveFileTypes::NotValid;
}

#if SUPPORT_PC_SAVES
void SaveFile::SetFileType(const SaveFileTypes _fileType)
{
	if (fileType == _fileType) return;

	SaveFileTypes previousFormat = fileType;
	fileType = _fileType;

	for (uint8_t f = 0; f < ACTUAL_NUM_FILE_SLOTS; f++)
	{
		MultiplayerSetup* mpSetup = saveData->GetMultiplayerSetup(f);
		if (!mpSetup->IsUsed()) continue;

		if (previousFormat == SaveFileTypes::LittleEndian && _fileType == SaveFileTypes::BigEndian)
		{
			for (uint8_t ws = 0; ws < NUM_MP_WEAPONSLOTS; ws++)
			{
				// Night Vision and IR Scanner are not available on N64

				if (mpSetup->weaponSlots[ws] >= 35 && mpSetup->weaponSlots[ws] <= 36)
				{
					mpSetup->weaponSlots[ws] = 0;
				}

				// Offset items that are between the Night Vision and IR Scanner,
				// and the classic weapons.

				if (mpSetup->weaponSlots[ws] >= 37 && mpSetup->weaponSlots[ws] <= 38)
				{
					mpSetup->weaponSlots[ws] -= 2;
				}

				// Classic weapons are not available on N64

				if (mpSetup->weaponSlots[ws] >= 39 && mpSetup->weaponSlots[ws] <= 46)
				{
					mpSetup->weaponSlots[ws] = 0;
				}

				// Offset weapons that are after the classic weapons

				if (mpSetup->weaponSlots[ws] > 46)
				{
					mpSetup->weaponSlots[ws] -= 10;
				}
			}
		}
		else if (previousFormat == SaveFileTypes::BigEndian && _fileType == SaveFileTypes::LittleEndian)
		{
			for (uint8_t ws = 0; ws < NUM_MP_WEAPONSLOTS; ws++)
			{
				// Offset Cloaking Device and Combat Boost to make room for
				// Night Vision and IR Scanner.

				if (mpSetup->weaponSlots[ws] >= 35 && mpSetup->weaponSlots[ws] <= 36)
				{
					mpSetup->weaponSlots[ws] += 2;
				}

				// Offset latest weapons to make room for the classic weapons

				if (mpSetup->weaponSlots[ws] > 36)
				{
					mpSetup->weaponSlots[ws] += 10;
				}
			}
		}
	}
}
#endif
