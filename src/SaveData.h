#pragma once

#include <string>
#include "SaveFile.h"

#define NUM_SAVE_FORMATS 2

class SaveData
{
public:
	enum class Types { NotValid, Nintendo64, PC };

private:
	SaveFile* saveFile;
	Types type;

public:
	SaveData();
	~SaveData();

	void Load(const std::string filePath);
	void Save(const std::string filePath);

	inline bool IsSaveFileLoaded() const { return saveFile != nullptr; }
	inline SaveFile* GetSaveFile() const { return saveFile; }
	inline Types GetType() const { return type; }
	inline void SetType(const Types newType) { type = newType; }
	inline bool NeedsEndianSwap() const { return type == SaveData::Types::Nintendo64; }

private:
	void ClearSaveFile();

	static Types CalculateType(uint8_t* fileBuffer);
};
