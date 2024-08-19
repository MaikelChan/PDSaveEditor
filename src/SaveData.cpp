#include "SaveData.h"
#include <fstream>
#include <cassert>
#include <cstring>
#include "Utils.h"

SaveData::SaveData()
{
	saveFile = nullptr;

	format = SaveFormats::NotValid;
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

	uint8_t fileBuffer[SAVE_FILE_SIZE] = {};

	stream.seekg(0, std::ios_base::beg);
	stream.read((char*)&fileBuffer, SAVE_FILE_SIZE);
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
	saveFile->Load(fileBuffer, NeedsEndianSwap());

	ClearSaveFile();

	SaveData::saveFile = saveFile;
	SaveData::format = format;
}

void SaveData::Save(const std::string filePath)
{
	if (!IsSaveFileLoaded()) return;

	uint8_t fileBuffer[SAVE_FILE_SIZE];
	memset(fileBuffer, 0, SAVE_FILE_SIZE);

	saveFile->Save(fileBuffer, NeedsEndianSwap());

	std::ofstream stream = std::ofstream(filePath, std::ios::binary);

	if (!stream || !stream.is_open())
	{
		throw std::runtime_error(std::string("Can't open file \"") + filePath + "\".");
	}

	stream.write((char*)fileBuffer, SAVE_FILE_SIZE);
	stream.close();
}

void SaveData::ClearSaveFile()
{
	if (!IsSaveFileLoaded()) return;

	delete saveFile;
	saveFile = nullptr;

	format = SaveFormats::NotValid;
}

SaveFormats SaveData::CalculateFormat(uint8_t* fileBuffer)
{
	uint16_t headersum[2] = {};
	headersum[0] = (fileBuffer[1] << 8) | fileBuffer[0];
	headersum[1] = (fileBuffer[3] << 8) | fileBuffer[2];

	uint16_t checksum[2];
	SaveFile::CalculateChecksum(&fileBuffer[8], &fileBuffer[16], checksum);

	if (headersum[0] == checksum[0] && headersum[1] == checksum[1]) return SaveFormats::PC;
	if (Utils::Swap16(headersum[0]) == checksum[0] && Utils::Swap16(headersum[1]) == checksum[1]) return SaveFormats::Nintendo64;
	return SaveFormats::NotValid;
}