#include "SaveData.h"
#include <fstream>
//#include <exception>
#include <cassert>
#include "Utils.h"

SaveData::SaveData()
{
	/*assert(sizeof(SaveSlot) == SAVE_SLOT_SIZE);
	assert(sizeof(GlobalData) == GLOBAL_DATA_SIZE);
	assert(sizeof(SaveFile) == SAVE_FILE_SIZE);*/

	saveFile = nullptr;

	type = SaveData::Types::NotValid;
}

SaveData::~SaveData()
{
	ClearSaveFile();
}

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

	if (size != SAVE_FILE_SIZE)
	{
		stream.close();
		throw std::runtime_error("The selected file is not a valid Perfect Dark save file.");
		return;
	}

	// Read the whole file

	uint8_t fileBuffer[SAVE_FILE_SIZE];

	stream.seekg(0, std::ios_base::beg);
	stream.read((char*)&fileBuffer, SAVE_FILE_SIZE);
	stream.close();

	// Create and load the SaveFile struct

	SaveFile* saveFile = new SaveFile();
	saveFile->Load(fileBuffer);

	SaveData::Types type = CalculateType(saveFile);

	if (type == SaveData::Types::NotValid)
	{
		delete saveFile;
		throw std::runtime_error("The selected file is not a valid Perfect Dark save file.");
		return;
	}

	ClearSaveFile();

	SaveData::saveFile = saveFile;
	SaveData::type = type;
}

void SaveData::Save(const std::string filePath)
{
	if (!IsSaveFileLoaded()) return;

	std::ofstream stream = std::ofstream(filePath, std::ios::binary);

	if (!stream || !stream.is_open())
	{
		throw std::runtime_error(std::string("Can't open file \"") + filePath + "\".");
	}

	stream.write((char*)saveFile, SAVE_FILE_SIZE);
	stream.close();
}

void SaveData::ClearSaveFile()
{
	if (!IsSaveFileLoaded()) return;

	delete saveFile;
	saveFile = nullptr;

	type = SaveData::Types::NotValid;
}

SaveData::Types SaveData::CalculateType(SaveFile* saveFile)
{
	if (!saveFile) return SaveData::Types::NotValid;

	/*for (uint8_t s = 0; s < TOTAL_NUM_SAVE_SLOTS; s++)
	{
		uint32_t checksum = saveFile->GetRawSaveSlot(s)->CalculateChecksum();
		if (checksum == saveFile->GetRawSaveSlot(s)->GetChecksum(false)) return SaveData::Types::PC;
		if (checksum == saveFile->GetRawSaveSlot(s)->GetChecksum(true)) return SaveData::Types::Nintendo64;
	}

	uint32_t checksum = saveFile->GetGlobalData()->CalculateChecksum();
	if (checksum == saveFile->GetGlobalData()->GetChecksum(false)) return SaveData::Types::PC;
	if (checksum == saveFile->GetGlobalData()->GetChecksum(true)) return SaveData::Types::Nintendo64;*/

	return SaveData::Types::PC;
}