#pragma once

#include <cstdint>

#pragma region Data_Structures

#define SAVE_BUFFER_SIZE 220
#define SAVE_FILE_SIZE 2048
#define MAX_PACK_FILE_SIZE 256
#define PACK_HEADER_SIZE 16
#define PACK_BOSS_SIZE 96
#define PACK_MPPLAYER_SIZE 80
#define PACK_MPSETUP_SIZE 64
#define PACK_GAME_SIZE 160
#define PACK_TERMINATOR_SIZE 240
#define PACK_TERMINATOR_ACTUAL_SIZE 48
#define NUM_BOSS_FILE_SLOTS 1
#define ACTUAL_NUM_BOSS_FILE_SLOTS 2
#define NUM_FILE_SLOTS 4
#define ACTUAL_NUM_FILE_SLOTS 5

#define GAMEFILE_FLAGS_SIZE 10

enum class PakFileTypes
{
	UNUSED_001 = 0x001, // Unused
	BLANK = 0x002,      // Blank space (ie. deleted file)
	TERMINATOR = 0x004, // Marks the end of the device's filesystem
	CAMERA = 0x008,     // PerfectHead File
	BOSS = 0x010,       // EEPROM only, one per cart (stores language, MP team names, selected MP soundtracks)
	MPPLAYER = 0x020,   // Combat Simulator Player File
	MPSETUP = 0x040,    // Combat Simulator Settings File
	GAME = 0x080,       // Single Player Agent File
	ALL = 0x100         // Not really a file type
};

#pragma endregion

#pragma region General

#define MAX_NAME_LENGTH 10
#define MAX_PLAYERS 4

#define NUM_AIM_CONTROL_MODES 2
#define NUM_CONTROL_MODES 8
#define NUM_LANGUAGES 5
#define NUM_RATIOS 2
#define NUM_SCREEN_SIZES 3
#define NUM_SCREEN_SPLIT_MODES 2
#define NUM_SONGS 43
#define NUM_SOUND_MODES 4

const char* const aimControlModeNames[NUM_AIM_CONTROL_MODES]
{
	"Hold",
	"Toggle"
};

const char* const controlModeNames[NUM_CONTROL_MODES]
{
	"Single (1.1)",
	"Single (1.2)",
	"Single (1.3)",
	"Single (1.4)",
	"Double (2.1)",
	"Double (2.2)",
	"Double (2.3)",
	"Double (2.4)"
};

const char* const languageNames[NUM_LANGUAGES]
{
	"English",
	"French",
	"German",
	"Italian",
	"Spanish"
};

const char* const ratioNames[NUM_RATIOS]
{
	"Normal",
	"16:9"
};

const char* const screenSizeNames[NUM_SCREEN_SIZES]
{
	"Full",
	"Wide",
	"Cinema"
};

const char* const screenSplitModeNames[NUM_SCREEN_SPLIT_MODES]
{
	"Horizontal",
	"Vertical"
};

const char* const songNames[NUM_SONGS + 1]
{
	"Dark Combat",
	"Skedar Mystery",
	"CI Operative",
	"dataDyne Action",
	"Maian Tears",
	"Alien Conflict",
	"Carrington Institute",
	"dD Central",
	"dD Central X",
	"dD Research",
	"dD Research X",
	"dD Extraction",
	"dD Extraction X",
	"Carrington Villa",
	"Carrington Villa X",
	"Chicago",
	"Chicago X",
	"G5 Building",
	"G5 Building X",
	"A51 Infiltration",
	"A51 Infiltration X",
	"A51 Rescue",
	"A51 Rescue X",
	"A51 Escape",
	"A51 Escape X",
	"Air Base",
	"Air Base X",
	"Air Force One",
	"Air Force One X",
	"Crash Site",
	"Crash Site X",
	"Pelagic II",
	"Pelagic II X",
	"Deep Sea",
	"Deep Sea X",
	"Institute Defense",
	"Institute Defense X",
	"Attack Ship",
	"Attack Ship X",
	"Skedar Ruins",
	"Skedar Ruins X",
	"End Credits",
	"Skedar Warrior",
	"Random"
};

const char* const soundModeNames[NUM_SOUND_MODES]
{
	"Mono",
	"Stereo",
	"Headphone",
	"Surround"
};

#pragma endregion

#pragma region Single_Player

#define NUM_DIFFICULTIES 3
#define NUM_FIRING_RANGE_MEDALS 3
#define NUM_SOLOSTAGES 21

const char* const difficultyNames[NUM_DIFFICULTIES]
{
	"Agent",
	"Special Agent",
	"Perfect Agent"
};

const char* const firingRangeMedalNames[NUM_FIRING_RANGE_MEDALS]
{
	"Bronze",
	"Silver",
	"Gold"
};

const char* const stageNames[NUM_SOLOSTAGES]
{
	"dataDyne Central - Defection",
	"dataDyne Research - Investigation",
	"dataDyne Central - Extraction",
	"Carrington Villa - Hostage One",
	"Chicago - Stealth",
	"G5 Building - Reconnaissance",
	"Area 51 - Infiltration",
	"Area 51 - Rescue",
	"Area 51 - Escape",
	"Air Base - Espionage",
	"Air Force One - Antiterrorism",
	"Crash Site - Confrontation",
	"Pelagic II - Exploration",
	"Deep Sea - Nullify Threat",
	"Carrington Institute - Defense",
	"Attack Ship - Covert Assault",
	"Skedar Ruins - Battle Shrine",
	"Mr. Blonde's Revenge",
	"Maian SOS",
	"WAR!",
	"The Duel"
};

const char* const coOpPlayerNames[2]
{
	"Player 1",
	"Player 2"
};

const char* const thumbnailNames[NUM_SOLOSTAGES + 1]
{
	"Joanna Picture",
	"dataDyne Central - Defection",
	"dataDyne Research - Investigation",
	"dataDyne Central - Extraction",
	"Carrington Villa - Hostage One",
	"Chicago - Stealth",
	"G5 Building - Reconnaissance",
	"Area 51 - Infiltration",
	"Area 51 - Rescue",
	"Area 51 - Escape",
	"Air Base - Espionage",
	"Air Force One - Antiterrorism",
	"Crash Site - Confrontation",
	"Pelagic II - Exploration",
	"Deep Sea - Nullify Threat",
	"Carrington Institute - Defense",
	"Attack Ship - Covert Assault",
	"Skedar Ruins - Battle Shrine",
	"Mr. Blonde's Revenge",
	"Maian SOS",
	"WAR!",
	"The Duel"
};

enum class SinglePlayerFlags
{
	P1_FORWARDPITCH = 0x00,
	P1_AUTOAIM = 0x01,
	P1_AIMCONTROL = 0x02,
	P1_SIGHTONSCREEN = 0x03,
	P1_LOOKAHEAD = 0x04,
	P1_AMMOONSCREEN = 0x05,
	SCREENSIZE_WIDE = 0x06,
	SCREENRATIO = 0x07,
	SCREENSIZE_CINEMA = 0x08,
	P1_HEADROLL = 0x09,
	P1_SHOWGUNFUNCTION = 0x0a,
	INGAMESUBTITLES = 0x0b,
	P2_FORWARDPITCH = 0x0c,
	P2_AUTOAIM = 0x0d,
	P2_AIMCONTROL = 0x0e,
	P2_SIGHTONSCREEN = 0x0f,
	P2_LOOKAHEAD = 0x10,
	P2_AMMOONSCREEN = 0x11,
	P2_HEADROLL = 0x12,
	P2_SHOWGUNFUNCTION = 0x13,
	CUTSCENESUBTITLES = 0x14,
	P1_ALWAYSSHOWTARGET = 0x15,
	P2_ALWAYSSHOWTARGET = 0x16,
	P1_SHOWZOOMRANGE = 0x17,
	P2_SHOWZOOMRANGE = 0x18,
	SCREENSPLIT = 0x19,
	P1_SHOWMISSIONTIME = 0x1a,
	P2_SHOWMISSIONTIME = 0x1b,
	COOPRADARON = 0x1c,
	COOPFRIENDLYFIRE = 0x1d,
	ANTIRADARON = 0x1e,
	ANTIPLAYERNUM = 0x1f,
	P1_PAINTBALL = 0x20,
	P2_PAINTBALL = 0x21,
	HIRES = 0x22,
	USED_TRANSFERPAK = 0x23,
	CI_TOUR_DONE = 0x24,
	UNKNOWN_25 = 0x25,
	UNKNOWN_26 = 0x26,
	UNKNOWN_27 = 0x27,
	UNKNOWN_28 = 0x28,
	CI_HOLO7_DONE = 0x29,
	CI_HOLO6_DONE = 0x2a,
	CI_HOLO5_DONE = 0x2b,
	CI_HOLO4_DONE = 0x2c,
	CI_HOLO3_DONE = 0x2d,
	CI_HOLO2_DONE = 0x2e,
	CI_HOLO1_DONE = 0x2f,
	CI_CLOAK_DONE = 0x30,
	CI_DISGUISE_DONE = 0x31,
	CI_XRAY_DONE = 0x32,
	CI_IR_DONE = 0x33,
	CI_RTRACKER_DONE = 0x34,
	CI_DOORDECODER_DONE = 0x35,
	CI_NIGHTVISION_DONE = 0x36,
	CI_CAMSPY_DONE = 0x37,
	CI_ECMMINE_DONE = 0x38,
	CI_UPLINK_DONE = 0x39,
	CI_TOUR_STARTED = 0x3a,
	CRASHSITE_BIKE = 0x3b,
	DEFENSE_JON = 0x3c,
	AF1_ENTRY = 0x3d,
	RESCUE_MECHANIC_DEAD = 0x3e,
	G5_MINE = 0x3f, // true if thrown on top door
	LANGFILTERON = 0x40,
	FOUNDTIMEDMINE = 0x41,
	FOUNDPROXYMINE = 0x42,
	FOUNDREMOTEMINE = 0x43,
	LANGBIT1 = 0x44,
	LANGBIT2 = 0x45,
	LANGBIT3 = 0x46,
	HOWTO_HOVERCRATE = 0x47,
	HOWTO_HOVERBIKE = 0x48,
	HOWTO_DOORS = 0x49,
	HOWTO_ELEVATORS = 0x4a,
	HOWTO_TERMINALS = 0x4b,
	UNKNOWN_4C = 0x4c,
	UNKNOWN_4D = 0x4d,
	UNKNOWN_4E = 0x4e
};

#pragma endregion

#pragma region Multiplayer

#define MAX_SIMULANTS 8

#define NUM_MP_BODIES 61
#define NUM_MP_CHALLENGES 30
#define NUM_MP_HEADS 75
#define NUM_MP_SCENARIOS 6
#define NUM_MP_SIMULANT_DIFFICULTIES 6
#define NUM_MP_SIMULANT_TYPES 13
#define NUM_MP_SLOWMOTION_MODES 3
#define NUM_MP_STAGES 16
#define NUM_MP_STAGES_AND_RANDOM (NUM_MP_STAGES + 1)
#define NUM_MP_TEAMS 8
#define NUM_MP_TIERS_TALLIES 10
#define NUM_MP_TITLES 21
#define NUM_MP_WEAPONSLOTS 6

const char* const mpScenarioNames[NUM_MP_SCENARIOS]
{
	"Combat",
	"Hold the Briefcase",
	"Hacker Central",
	"Pop a Cap",
	"King of the Hill",
	"Capture the Case"
};

const char* const mpSimulantDifficultyNames[NUM_MP_SIMULANT_DIFFICULTIES + 1]
{
	"Meat",
	"Easy",
	"Normal",
	"Hard",
	"Perfect",
	"Dark",
	"Disabled"
};

const char* const mpSimulantTypeNames[NUM_MP_SIMULANT_TYPES]
{
	"GeneralSim",
	"PeaceSim",
	"ShieldSim",
	"RocketSim",
	"KazeSim",
	"FistSim",
	"PreySim",
	"CowardSim",
	"JudgeSim",
	"FeudSim",
	"SpeedSim",
	"TurtleSim",
	"VengeSim"
};

const char* const mpSlowMotionNames[NUM_MP_SLOWMOTION_MODES]
{
	"Off",
	"On",
	"Smart"
};

const char* const mpStageNames[NUM_MP_STAGES_AND_RANDOM]
{
	"Skedar",
	"Pipes",
	"Ravine",
	"G5 Building",
	"Sewers",
	"Warehouse",
	"Grid",
	"Ruins",
	"Area 52",
	"Base",
	"Fortress",
	"Villa",
	"Car Park",
	"Temple",
	"Complex",
	"Felicity",
	"Random"
};

const uint8_t mpStageIndices[NUM_MP_STAGES_AND_RANDOM] // struct mparena g_MpArenas[]
{
	0x32,	// STAGE_MP_SKEDAR
	0x29,	// STAGE_MP_PIPES
	0x17,	// STAGE_MP_RAVINE
	0x20,	// STAGE_MP_G5BUILDING
	0x42,	// STAGE_MP_SEWERS
	0x3c,	// STAGE_MP_WAREHOUSE
	0x47,	// STAGE_MP_GRID
	0x41,	// STAGE_MP_RUINS
	0x3b,	// STAGE_MP_AREA52
	0x39,	// STAGE_MP_BASE
	0x44,	// STAGE_MP_FORTRESS
	0x45,	// STAGE_MP_VILLA
	0x3d,	// STAGE_MP_CARPARK
	0x25,	// STAGE_MP_TEMPLE
	0x1f,	// STAGE_MP_COMPLEX
	0x43,	// STAGE_MP_FELICITY
	0x01	// STAGE_MP_RANDOM
};

const char* const teamNames[NUM_MP_TEAMS]
{
	"Red",
	"Yellow",
	"Blue",
	"Magenta",
	"Cyan",
	"Orange",
	"Pink",
	"Brown"
};

const char* const titleNames[NUM_MP_TITLES]
{
	"Beginner",
	"Trainee",
	"Amateur",
	"Rookie",
	"Novice",
	"Trooper",
	"Agent",
	"Star Agent",
	"Special Agent",
	"Expert",
	"Veteran",
	"Professional",
	"Dangerous",
	"Deadly",
	"Killer",
	"Assassin",
	"Lethal",
	"Elite",
	"Invincible",
	"Near Perfect",
	"Perfect"
};

enum class MultiplayerTitles
{
	BEGINNER,
	TRAINEE,
	AMATEUR,
	ROOKIE,
	NOVICE,
	TROOPER,
	AGENT,
	STARAGENT,
	SPECIALAGENT,
	EXPERT,
	VETERAN,
	PROFESSIONAL,
	DANGEROUS,
	DEADLY,
	KILLER,
	ASSASSIN,
	LETHAL,
	ELITE,
	INVINCIBLE,
	NEARPERFECT,
	PERFECT
};

enum class MultiplayerOptionsFlags
{
	FORWARDPITCH = 0x0001,
	LOOKAHEAD = 0x0002,
	SIGHTONSCREEN = 0x0004,
	AUTOAIM = 0x0008,
	AIMCONTROL = 0x0010,
	AMMOONSCREEN = 0x0020,
	SHOWGUNFUNCTION = 0x0040,
	HEADROLL = 0x0080,
	UNUSED_0100 = 0x0100,
	ALWAYSSHOWTARGET = 0x0200,
	SHOWZOOMRANGE = 0x0400,
	PAINTBALL = 0x0800,
	ASKEDSAVEPLAYER = 0x4000,
	SHOWMISSIONTIME = 0x8000
};

enum class MultiplayerSetupFlags
{
	ONEHITKILLS = 0x00000001,
	TEAMSENABLED = 0x00000002,
	NORADAR = 0x00000004,
	NOAUTOAIM = 0x00000008,
	NOPLAYERHIGHLIGHT = 0x00000010,
	NOPICKUPHIGHLIGHT = 0x00000020,
	SLOWMOTION_ON = 0x00000040,
	SLOWMOTION_SMART = 0x00000080,
	FASTMOVEMENT = 0x00000100,
	DISPLAYTEAM = 0x00000200,
	KILLSSCORE = 0x00000400,
	HTB_HIGHLIGHTBRIEFCASE = 0x00000800,
	HTB_SHOWONRADAR = 0x00001000,
	CTC_SHOWONRADAR = 0x00002000,
	KOH_HILLONRADAR = 0x00004000,
	KOH_MOBILEHILL = 0x00008000,
	UNUSED_00010000 = 0x00010000,
	HTM_HIGHLIGHTTERMINAL = 0x00020000,
	HTM_SHOWONRADAR = 0x00040000,
	PAC_HIGHLIGHTTARGET = 0x00080000,
	PAC_SHOWONRADAR = 0x00100000
};

enum class MultiplayerDisplayOptionsFlags
{
	HIGHLIGHTPLAYERS = 0x01,
	HIGHLIGHTPICKUPS = 0x02,
	RADAR = 0x04,
	HIGHLIGHTTEAMS = 0x08
};

enum class MultiplayerScenarios
{
	COMBAT = 0,
	HOLDTHEBRIEFCASE = 1,
	HACKERCENTRAL = 2,
	POPACAP = 3,
	KINGOFTHEHILL = 4,
	CAPTURETHECASE = 5
};

enum class SimulantDifficulties
{
	MEAT = 0,
	EASY = 1,
	NORMAL = 2,
	HARD = 3,
	PERFECT = 4,
	DARK = 5,
	DISABLED = 6
};

enum class SimulantTypes
{
	GENERAL = 0,
	PEACE = 1,
	SHIELD = 2,
	ROCKET = 3,
	KAZE = 4,
	FIST = 5,
	PREY = 6,
	COWARD = 7,
	JUDGE = 8,
	FEUD = 9,
	SPEED = 10,
	TURTLE = 11,
	VENGE = 12
};

#pragma endregion

#pragma region Weapons

#define NUM_WEAPONS 35
#define NUM_FIRING_RANGE_WEAPONS 32
#define NUM_MP_WEAPONS_N64 39
#define NUM_MP_WEAPONS_PC 47

const char* const weaponNames[]
{
	"Nothing",				// 0
	"Unarmed",				// 1
	"Falcon 2",				// 2
	"Falcon 2 (silencer)",	// 3
	"Falcon 2 (scope)",		// 4
	"MagSec 4",				// 5
	"Mauler",				// 6
	"Phoenix",				// 7
	"DY357 Magnum",			// 8
	"DY357-LX",				// 9
	"CMP150",				// 10
	"Cyclone",				// 11
	"Callisto NTG",			// 12
	"RC-P120",				// 13
	"Laptop Gun",			// 14
	"Dragon",				// 15
	"K7 Avenger",			// 16
	"AR34",					// 17
	"SuperDragon",			// 18
	"Shotgun",				// 19
	"Reaper",				// 20
	"Sniper Rifle",			// 21
	"FarSight XR-20",		// 22
	"Devastator",			// 23
	"Rocket Launcher",		// 24
	"Slayer",				// 25
	"Combat Knife",			// 26
	"Crossbow",				// 27
	"Tranquilizer",			// 28
	"Laser",				// 29
	"Grenade",				// 30
	"N-Bomb",				// 31
	"Timed Mine",			// 32
	"Proximity Mine",		// 33
	"Remote Mine",			// 34
	"Combat Boost",			// 35
	"PP9i",					// 36
	"CC13",					// 37
	"KLO1313",				// 38
	"KF7 Special",			// 39
	"ZZT (9mm)",			// 40
	"DMC",					// 41
	"AR53",					// 42
	"RC-P45",				// 43
	"Shield",				// 44
	"X-Ray Scanner",		// 45
	"Cloaking Device",		// 46
	"Disabled"				// 47
};

const uint8_t frWeaponNameIndices[NUM_FIRING_RANGE_WEAPONS] // frGetWeaponIndexByWeapon(u32 weaponnum)
{
	2,	// WEAPON_FALCON2
	4,	// WEAPON_FALCON2_SCOPE
	3,	// WEAPON_FALCON2_SILENCER
	5,	// WEAPON_MAGSEC4
	6,	// WEAPON_MAULER
	7,	// WEAPON_PHOENIX
	8,	// WEAPON_DY357MAGNUM
	9,	// WEAPON_DY357LX
	10,	// WEAPON_CMP150
	11,	// WEAPON_CYCLONE
	12,	// WEAPON_CALLISTO
	13,	// WEAPON_RCP120
	14,	// WEAPON_LAPTOPGUN
	15,	// WEAPON_DRAGON
	16,	// WEAPON_K7AVENGER
	17,	// WEAPON_AR34
	18,	// WEAPON_SUPERDRAGON
	19,	// WEAPON_SHOTGUN
	21,	// WEAPON_SNIPERRIFLE
	22,	// WEAPON_FARSIGHT
	27,	// WEAPON_CROSSBOW
	28,	// WEAPON_TRANQUILIZER
	20,	// WEAPON_REAPER
	23,	// WEAPON_DEVASTATOR
	24,	// WEAPON_ROCKETLAUNCHER
	25,	// WEAPON_SLAYER
	26,	// WEAPON_COMBATKNIFE
	29,	// WEAPON_LASER
	30,	// WEAPON_GRENADE
	32,	// WEAPON_TIMEDMINE
	33,	// WEAPON_PROXIMITYMINE
	34	// WEAPON_REMOTEMINE
};

const uint8_t mpWeaponNameIndicesN64[NUM_MP_WEAPONS_N64] // struct mpweapon g_MpWeapons[NUM_MPWEAPONS]
{
	0,	// WEAPON_NONE
	2,	// WEAPON_FALCON2
	3,	// WEAPON_FALCON2_SILENCER
	4,	// WEAPON_FALCON2_SCOPE
	5,	// WEAPON_MAGSEC4
	6,	// WEAPON_MAULER
	7,	// WEAPON_PHOENIX
	8,	// WEAPON_DY357MAGNUM
	9,	// WEAPON_DY357LX
	10,	// WEAPON_CMP150
	11,	// WEAPON_CYCLONE
	12,	// WEAPON_CALLISTO
	13,	// WEAPON_RCP120
	14,	// WEAPON_LAPTOPGUN
	15,	// WEAPON_DRAGON
	16,	// WEAPON_K7AVENGER
	17,	// WEAPON_AR34
	18,	// WEAPON_SUPERDRAGON
	19,	// WEAPON_SHOTGUN
	20,	// WEAPON_REAPER
	21,	// WEAPON_SNIPERRIFLE
	22,	// WEAPON_FARSIGHT
	23,	// WEAPON_DEVASTATOR
	24,	// WEAPON_ROCKETLAUNCHER
	25,	// WEAPON_SLAYER
	26,	// WEAPON_COMBATKNIFE
	27,	// WEAPON_CROSSBOW
	28,	// WEAPON_TRANQUILIZER
	30,	// WEAPON_GRENADE
	31,	// WEAPON_NBOMB
	32,	// WEAPON_TIMEDMINE
	33,	// WEAPON_PROXIMITYMINE
	34,	// WEAPON_REMOTEMINE
	29,	// WEAPON_LASER
	45,	// WEAPON_XRAYSCANNER
	46,	// WEAPON_CLOAKINGDEVICE
	35,	// WEAPON_COMBATBOOST
	44,	// WEAPON_MPSHIELD
	47	// WEAPON_DISABLED
};

const uint8_t mpWeaponNameIndicesPC[NUM_MP_WEAPONS_PC] // struct mpweapon g_MpWeapons[NUM_MPWEAPONS]
{
	0,	// WEAPON_NONE
	2,	// WEAPON_FALCON2
	3,	// WEAPON_FALCON2_SILENCER
	4,	// WEAPON_FALCON2_SCOPE
	5,	// WEAPON_MAGSEC4
	6,	// WEAPON_MAULER
	7,	// WEAPON_PHOENIX
	8,	// WEAPON_DY357MAGNUM
	9,	// WEAPON_DY357LX
	10,	// WEAPON_CMP150
	11,	// WEAPON_CYCLONE
	12,	// WEAPON_CALLISTO
	13,	// WEAPON_RCP120
	14,	// WEAPON_LAPTOPGUN
	15,	// WEAPON_DRAGON
	16,	// WEAPON_K7AVENGER
	17,	// WEAPON_AR34
	18,	// WEAPON_SUPERDRAGON
	19,	// WEAPON_SHOTGUN
	20,	// WEAPON_REAPER
	21,	// WEAPON_SNIPERRIFLE
	22,	// WEAPON_FARSIGHT
	23,	// WEAPON_DEVASTATOR
	24,	// WEAPON_ROCKETLAUNCHER
	25,	// WEAPON_SLAYER
	26,	// WEAPON_COMBATKNIFE
	27,	// WEAPON_CROSSBOW
	28,	// WEAPON_TRANQUILIZER
	30,	// WEAPON_GRENADE
	31,	// WEAPON_NBOMB
	32,	// WEAPON_TIMEDMINE
	33,	// WEAPON_PROXIMITYMINE
	34,	// WEAPON_REMOTEMINE
	29,	// WEAPON_LASER
	45,	// WEAPON_XRAYSCANNER
	46,	// WEAPON_CLOAKINGDEVICE
	35,	// WEAPON_COMBATBOOST
	36,	// WEAPON_PP9I
	37,	// WEAPON_CC13
	38,	// WEAPON_KL01313
	39,	// WEAPON_KF7SPECIAL
	40,	// WEAPON_ZZT
	41,	// WEAPON_DMC
	42,	// WEAPON_AR53
	43,	// WEAPON_RCP45
	44,	// WEAPON_MPSHIELD
	47	// WEAPON_DISABLED
};

#pragma endregion

struct FileGuid
{
	int32_t id;
	uint16_t deviceSerial;
};

struct SaveBuffer
{
private:
	uint32_t bitPosition;
	uint8_t bytes[SAVE_BUFFER_SIZE];

public:
	SaveBuffer();
	SaveBuffer(const uint8_t* bytes, const uint32_t size);

	uint8_t* GetBytes();

	uint32_t ReadBits(const int32_t numBits);
	void Or(const uint32_t value, const int32_t numBits);
	void ReadGuid(FileGuid* guid);
	void WriteGuid(FileGuid* guid);
	void ReadString(char* dst);
	void WriteString(char* src);
	void Clear();
};

struct SimulantData
{
public:
	uint8_t type = 0;
	uint8_t difficulty = 0;
	uint8_t headIndex = 0;
	uint8_t bodyIndex = 0;
	uint8_t team = 0;
};

struct PakFileHeader
{
public:
	uint16_t headersum[2];       // checksum from filetype to end of header
	uint16_t bodysum[2];
	uint32_t filetype : 9;       // PAKFILETYPE constant
	uint32_t bodylen : 11;       // not aligned
	uint32_t filelen : 12;       // aligned to 0x10
	uint32_t deviceSerial : 13;
	uint32_t id : 7;
	uint32_t generation : 9;     // increments by 1 each time the same file is saved
	uint32_t occupied : 1;
	uint32_t writecompleted : 1; // 0 while writing data, then updated to 1 afterwards
	uint32_t version : 1;        // 0, but can be set to 1 using -forceversion argument
};

struct PakFile
{
public:
	PakFileHeader pakFileHeader = {};

private:
	bool isValid = false;

public:
	virtual void Load(uint8_t* fileBuffer);
	virtual void Save(uint8_t* fileBuffer);

	bool IsUsed() const { return pakFileHeader.occupied; }
	bool IsValid() const { return isValid; }
};

struct BossFile : public PakFile
{
public:
	FileGuid guid = {};
	uint8_t unk1 = 0;
	uint8_t language = 0;
	char teamNames[NUM_MP_TEAMS][MAX_NAME_LENGTH + 1] = {};
	uint8_t tracknum = 255;
	uint8_t multipletracknums[6] = {};
	bool usingmultipletunes = false;
	bool altTitleUnlocked = false;
	bool altTitleEnabled = false;

public:
	void Load(uint8_t* fileBuffer) override;
	void Save(uint8_t* fileBuffer) override;

	bool IsMultiTrackSlotEnabled(const uint8_t slot) const;
	void SetMultiTrackSlotEnabled(const uint8_t slot, const bool enable);
};

struct GameFile : public PakFile
{
public:
	char name[MAX_NAME_LENGTH + 1] = {};
	uint8_t thumbnail = 0;
	uint32_t totaltime = 0;
	uint8_t autodifficulty = 0;
	uint8_t autostageindex = 0;
	uint8_t sfxVolume = 0;
	uint8_t musicVolume = 0;
	uint8_t soundMode = 0;
	uint8_t controlModes[2] = {};
	uint8_t flags[GAMEFILE_FLAGS_SIZE] = {};
	uint16_t unknown1 = 0;
	uint16_t besttimes[NUM_SOLOSTAGES][NUM_DIFFICULTIES] = {};
	uint8_t mpChallenges[NUM_MP_CHALLENGES] = {};
	uint32_t coopcompletions[NUM_DIFFICULTIES] = {};
	uint8_t firingrangescores[9] = {};
	uint8_t weaponsfound[6] = {};

public:
	void Load(uint8_t* fileBuffer) override;
	void Save(uint8_t* fileBuffer) override;

	bool GetFlag(const SinglePlayerFlags flag) const;
	void SetFlag(const SinglePlayerFlags flag, const bool set);
	uint8_t GetFiringRangeScore(const uint8_t weaponIndex) const;
	void SetFiringRangeScore(const uint8_t weaponIndex, const uint8_t difficulty);
	bool GetWeaponFound(const uint8_t weaponIndex) const;
	void SetWeaponFound(const uint8_t weaponIndex, const bool found);
};

struct MultiplayerProfile : public PakFile
{
public:
	char name[MAX_NAME_LENGTH + 1] = {};
	uint32_t time = 0;
	uint8_t headIndex = 0;
	uint8_t bodyIndex = 0;
	FileGuid guid = {};
	uint8_t displayoptions = 0;
	uint32_t kills = 0;
	uint32_t deaths = 0;
	uint32_t gamesplayed = 0;
	uint32_t gameswon = 0;
	uint32_t gameslost = 0;
	uint32_t distance = 0;
	uint16_t accuracy = 0;
	uint32_t damagedealt = 0;
	uint32_t painreceived = 0;
	uint32_t headshots = 0;
	uint32_t ammoused = 0;
	uint32_t accuracymedals = 0;
	uint32_t headshotmedals = 0;
	uint32_t killmastermedals = 0;
	uint16_t survivormedals = 0;
	uint8_t controlmode = 0;
	uint16_t options = 0;
	uint8_t mpChallenges[NUM_MP_CHALLENGES] = {};
	uint8_t gunfuncs[6] = {};

public:
	void Load(uint8_t* fileBuffer) override;
	void Save(uint8_t* fileBuffer) override;

	bool GetOptionsFlag(const MultiplayerOptionsFlags flag) const;
	void SetOptionsFlag(const MultiplayerOptionsFlags flag, const bool set);
	bool GetDisplayOptionsFlag(const MultiplayerDisplayOptionsFlags flag) const;
	void SetDisplayOptionsFlag(const MultiplayerDisplayOptionsFlags flag, const bool set);
	bool GetWeaponSecondaryFunction(const uint8_t weaponIndex) const;
	void SetWeaponSecondaryFunction(const uint8_t weaponIndex, const bool secondary);

	MultiplayerTitles GetPlayerTitle(const bool newMethod) const;
};

struct MultiplayerSetup : public PakFile
{
public:
	char name[MAX_NAME_LENGTH + 1] = {};
	uint8_t stagenum = 0;
	uint8_t scenario = 0;
	uint8_t scenarioParams = 0; // Changes depending on scenario (KoH: Hill time)
	uint32_t options = 0;
	SimulantData botsData[MAX_SIMULANTS] = {};
	uint8_t weaponSlots[NUM_MP_WEAPONSLOTS] = {};
	uint8_t timelimit = 0;
	uint8_t scorelimit = 0;
	uint16_t teamscorelimit = 0;
	uint8_t teams[MAX_PLAYERS] = {};

public:
	void Load(uint8_t* fileBuffer) override;

	uint8_t GetArena() const;
	void SetArena(const uint8_t arena);
	bool GetOptionsFlag(const MultiplayerSetupFlags flag) const;
	void SetOptionsFlag(const MultiplayerSetupFlags flag, const bool set);
	uint8_t GetSlowMotionMode() const;
	void SetSlowMotionMode(const uint8_t mode);
	uint8_t GetHillTime() const;
	void SetHillTime(const uint8_t time);
};

struct Terminator : public PakFile
{
public:
	void Load(uint8_t* fileBuffer) override;
	void Save(uint8_t* fileBuffer) override;
};

struct SaveFile
{
private:
	BossFile bossFiles[ACTUAL_NUM_BOSS_FILE_SLOTS] = {};
	GameFile gameFiles[ACTUAL_NUM_FILE_SLOTS] = {};
	MultiplayerProfile mpProfiles[ACTUAL_NUM_FILE_SLOTS] = {};
	MultiplayerSetup mpSetups[ACTUAL_NUM_FILE_SLOTS] = {};
	Terminator terminator = {};

public:
	void Load(uint8_t* fileBuffer);
	void Save(uint8_t* fileBuffer);

	BossFile* GetBossFile(const uint8_t index) { return &bossFiles[index]; }
	GameFile* GetGameFile(const uint8_t index) { return &gameFiles[index]; }
	MultiplayerProfile* GetMultiplayerProfile(const uint8_t index) { return &mpProfiles[index]; }
	MultiplayerSetup* GetMultiplayerSetup(const uint8_t index) { return &mpSetups[index]; }

	static void CalculateChecksum(uint8_t* start, uint8_t* end, uint16_t* checksum);

private:
	void PrintFileInfo(uint8_t* fileBuffer) const;

	static uint32_t TransformSeed(uint64_t* seed);
};