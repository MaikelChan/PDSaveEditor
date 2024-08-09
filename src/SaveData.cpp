#include "SaveData.h"
#include <fstream>
//#include <exception>
#include <cassert>
#include "Utils.h"

SaveData::SaveData()
{
	assert(sizeof(PakFileHeader) == PACK_HEADER_SIZE);
	/*assert(sizeof(GlobalData) == GLOBAL_DATA_SIZE);
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

	SaveData::Types type = CalculateType(fileBuffer);

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

SaveData::Types SaveData::CalculateType(uint8_t* fileBuffer)
{
	int32_t p = 0;

	while (p < SAVE_FILE_SIZE)
	{
		PakFileHeader header = {};
		memcpy(&header, &fileBuffer[p], PACK_HEADER_SIZE);

		uint16_t checksum[2];
		SaveFile::CalculateChecksum(&fileBuffer[p + 8], &fileBuffer[p + 16], checksum);

		if (header.headersum[0] == checksum[0] && header.headersum[1] == checksum[1]) return SaveData::Types::PC;
		if (Utils::Swap16(header.headersum[0]) == checksum[0] && Utils::Swap16(header.headersum[1]) == checksum[1]) return SaveData::Types::Nintendo64;

		p += header.filelen;
	}

	return SaveData::Types::NotValid;
}