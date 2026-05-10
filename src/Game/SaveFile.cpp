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
	SaveData* newSaveData = new SaveData();
	stream.read((char*)newSaveData, SAVE_DATA_SIZE);

	// Check what type of file is and initialize

	fileType = CalculateType(newSaveData);

	switch (fileType)
	{
		case SaveFileTypes::NotValid:
		{
			return "The selected file is not a valid Super Mario 64 save file.";
		}

		case SaveFileTypes::BigEndian:
		{
			newSaveData->EndianSwap();
			break;
		}

		case SaveFileTypes::LittleEndian:
		{
			break;
		}

		default:
		{
			throw std::runtime_error("SaveFile type " + std::to_string((int)fileType) + " not implemented.");
		}
	}

	// It's a valid file type, but check if there are data inconsistencies that should trigger a warning

	std::string warningMessages;

	for (uint8_t s = 0; s < NUM_SAVE_SLOTS; s++)
	{
		if (newSaveData->saveSlots[s][0].IsValid()) continue;

		if (newSaveData->saveSlots[s][1].IsValid())
		{
			std::copy(&newSaveData->saveSlots[s][1], &newSaveData->saveSlots[s][1] + 1, &newSaveData->saveSlots[s][0]);
			warningMessages += std::string("Save slot \"") + std::to_string(s) + "\" is corrupted, but valid data has been restored from the backup data.\n\n";
		}
		else
		{
			warningMessages += std::string("Save slot \"") + std::to_string(s) + "\" is corrupted along with its backup. Data might be completely wrong.\n\n";
		}
	}

	if (!newSaveData->settings[0].IsValid())
	{
		if (newSaveData->settings[1].IsValid())
		{
			std::copy(&newSaveData->settings[1], &newSaveData->settings[1] + 1, &newSaveData->settings[0]);
			warningMessages += "Settings data is corrupted, but valid data has been restored from the backup data.\n\n";
		}
		else
		{
			warningMessages += "Settings data is corrupted along with its backup. Data might be completely wrong.\n\n";
		}
	}

	// Write the magic in all slots in case that data is uninitialized, which can happen in ports like SM64 Coop DX

	for (uint8_t cp = 0; cp < NUM_COPIES; cp++)
	{
		for (uint8_t s = 0; s < NUM_SAVE_SLOTS; s++)
		{
			newSaveData->saveSlots[s][cp].Magic = SAVE_SLOT_MAGIC_LE;
			newSaveData->saveSlots[s][cp].UpdateChecksum();
		}

		newSaveData->settings[cp].Magic = SETTINGS_DATA_MAGIC_LE;
		newSaveData->settings[cp].UpdateChecksum();
	}

	// Assign references to the loaded data before finishing

	saveData = newSaveData;

	return warningMessages;
}

void SaveFile::Write(std::ofstream& stream)
{
	// Make a copy of all data to their backup slots

	for (uint8_t s = 0; s < NUM_SAVE_SLOTS; s++)
	{
		std::copy(&saveData->saveSlots[s][0], &saveData->saveSlots[s][0] + 1, &saveData->saveSlots[s][1]);
	}

	std::copy(&saveData->settings[0], &saveData->settings[0] + 1, &saveData->settings[1]);

	// Do the saving

	if (fileType == SaveFileTypes::BigEndian) saveData->EndianSwap();

	stream.write((char*)saveData, SAVE_DATA_SIZE);

	// Keep the data in memory in little endian

	if (fileType == SaveFileTypes::BigEndian) saveData->EndianSwap();
}

SaveFileTypes SaveFile::CalculateType(SaveData* saveData)
{
	if (saveData == nullptr) return SaveFileTypes::NotValid;

	uint16_t headersum[2] = {};
	headersum[0] = (fileBuffer[1] << 8) | fileBuffer[0];
	headersum[1] = (fileBuffer[3] << 8) | fileBuffer[2];

	uint16_t checksum[2];
	SaveFile::CalculateChecksum(&fileBuffer[8], &fileBuffer[16], checksum);

	if (headersum[0] == checksum[0] && headersum[1] == checksum[1]) return SaveFileTypes::LittleEndian;
	if (Utils::Swap16(headersum[0]) == checksum[0] && Utils::Swap16(headersum[1]) == checksum[1]) return SaveFileTypes::BigEndian;

	return SaveFileTypes::NotValid;
}

//void SaveData::SetFormat(const SaveFormats format)
//{
//	if (SaveData::format == format) return;
//
//	SaveFormats previousFormat = SaveData::format;
//	SaveData::format = format;
//
//	for (uint8_t f = 0; f < ACTUAL_NUM_FILE_SLOTS; f++)
//	{
//		MultiplayerSetup* mpSetup = saveFile->GetMultiplayerSetup(f);
//		if (!mpSetup->IsUsed()) continue;
//
//		if (previousFormat == SaveFormats::PC && format == SaveFormats::Nintendo64)
//		{
//			for (uint8_t ws = 0; ws < NUM_MP_WEAPONSLOTS; ws++)
//			{
//				// Night Vision and IR Scanner are not available on N64
//
//				if (mpSetup->weaponSlots[ws] >= 35 && mpSetup->weaponSlots[ws] <= 36)
//				{
//					mpSetup->weaponSlots[ws] = 0;
//				}
//
//				// Offset items that are between the Night Vision and IR Scanner,
//				// and the classic weapons.
//
//				if (mpSetup->weaponSlots[ws] >= 37 && mpSetup->weaponSlots[ws] <= 38)
//				{
//					mpSetup->weaponSlots[ws] -= 2;
//				}
//
//				// Classic weapons are not available on N64
//
//				if (mpSetup->weaponSlots[ws] >= 39 && mpSetup->weaponSlots[ws] <= 46)
//				{
//					mpSetup->weaponSlots[ws] = 0;
//				}
//
//				// Offset weapons that are after the classic weapons
//
//				if (mpSetup->weaponSlots[ws] > 46)
//				{
//					mpSetup->weaponSlots[ws] -= 10;
//				}
//			}
//		}
//		else if (previousFormat == SaveFormats::Nintendo64 && format == SaveFormats::PC)
//		{
//			for (uint8_t ws = 0; ws < NUM_MP_WEAPONSLOTS; ws++)
//			{
//				// Offset Cloaking Device and Combat Boost to make room for
//				// Night Vision and IR Scanner.
//
//				if (mpSetup->weaponSlots[ws] >= 35 && mpSetup->weaponSlots[ws] <= 36)
//				{
//					mpSetup->weaponSlots[ws] += 2;
//				}
//
//				// Offset latest weapons to make room for the classic weapons
//
//				if (mpSetup->weaponSlots[ws] > 36)
//				{
//					mpSetup->weaponSlots[ws] += 10;
//				}
//			}
//		}
//	}
//}


#include "SaveData.h"
#include <fstream>
#include <cassert>
#include <cstring>
#include "Utils.h"


void SaveData::Load(const std::string filePath)
{
	std::ifstream stream = std::ifstream(filePath, std::ios::binary);

	if (!stream || !stream.is_open())
	{
		throw std::runtime_error("There was an error trying to open open the file.");
		return;
	}

	stream.seekg(0, std::ios_base::end);
	size_t size = stream.tellg();

	if (size != SAVE_DATA_SIZE)
	{
		stream.close();
		throw std::runtime_error("The selected file is not a valid Perfect Dark save file.");
		return;
	}

	// Read the whole file

	uint8_t fileBuffer[SAVE_DATA_SIZE] = {};

	stream.seekg(0, std::ios_base::beg);
	stream.read((char*)&fileBuffer, SAVE_DATA_SIZE);
	stream.close();

	// Check if it's a valid file and its format

	SaveFormats format = CalculateFormat(fileBuffer);

	if (format == SaveFormats::NotValid)
	{
		throw std::runtime_error("The selected file is not a valid Perfect Dark save file.");
		return;
	}

	// Create and load the SaveFile struct

	SaveFile* saveFile = new SaveFile();
	saveFile->Load(fileBuffer, format == SaveFormats::Nintendo64);

	ClearSaveFile();

	SaveData::saveFile = saveFile;
	SaveData::format = format;
}

void SaveData::Save(const std::string filePath)
{
	if (!IsSaveFileLoaded()) return;

	uint8_t fileBuffer[SAVE_DATA_SIZE];
	memset(fileBuffer, 0, SAVE_DATA_SIZE);

	saveFile->Save(fileBuffer, format == SaveFormats::Nintendo64);

	std::ofstream stream = std::ofstream(filePath, std::ios::binary);

	if (!stream || !stream.is_open())
	{
		throw std::runtime_error(std::string("Can't open file \"") + filePath + "\".");
	}

	stream.write((char*)fileBuffer, SAVE_DATA_SIZE);
	stream.close();
}
