#pragma once

#include <string>
#include "SaveFile.h"

class SaveData
{
public:
	enum class Types { NotValid, PC, Nintendo64 };

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
	inline bool NeedsEndianSwap() const { return type == SaveData::Types::Nintendo64; }

private:
	void ClearSaveFile();

	static Types CalculateType(SaveFile* saveFile);
};
