#pragma once

#include <string>
#include "SaveFile.h"

#define NUM_SAVE_FORMATS 2
enum class SaveFormats { NotValid, Nintendo64, PC };

class SaveData
{
private:
	SaveFile* saveFile;
	SaveFormats format;

public:
	SaveData();
	~SaveData();

	void Load(const std::string filePath);
	void Save(const std::string filePath);

	inline bool IsSaveFileLoaded() const { return saveFile != nullptr; }
	inline SaveFile* GetSaveFile() const { return saveFile; }
	inline SaveFormats GetFormat() const { return format; }
	void SetFormat(const SaveFormats format);

private:
	void ClearSaveFile();

	static SaveFormats CalculateFormat(uint8_t* fileBuffer);
};
