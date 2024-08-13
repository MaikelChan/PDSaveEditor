#pragma once

#include <cstdint>

// General

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

#define MAX_NAME_LENGTH 10
#define MAX_PLAYERS 4
#define NUM_WEAPONS 35 // TODO Is this correct?
#define NUM_FIRING_RANGE_WEAPONS 32
#define NUM_SONGS 43
#define MULTIPLE_TRACKS_SIZE 6
#define NUM_LANGUAGES 5
#define NUM_SCREEN_SIZES 3
#define NUM_RATIOS 2
#define NUM_SCREEN_SPLIT_MODES 2
#define NUM_AIM_CONTROL_MODES 2

// Solo game

#define GAMEFILE_FLAGS_SIZE 10
#define NUM_SOLOSTAGES 21
#define NUM_DIFFICULTIES 3
#define NUM_SOUND_MODES 4
#define NUM_CONTROL_MODES 8
#define NUM_FIRING_RANGE_MEDALS 3
#define NUM_FIRING_RANGE_WEAPONS 32

// Multiplayer

#define NUM_MP_CHALLENGES 30
#define MAX_BOTS 8
#define NUM_MPWEAPONSLOTS 6
#define NUM_MP_STAGES 16
#define NUM_MP_STAGES_AND_RANDOM (NUM_MP_STAGES + 1)
#define TEAM_NAMES_COUNT 8

const char* const languageNames[NUM_LANGUAGES]
{
	"English",
	"French",
	"German",
	"Italian",
	"Spanish"
};

const char* const teamNames[TEAM_NAMES_COUNT]
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

const char* const difficultyNames[NUM_DIFFICULTIES]
{
	"Agent",
	"Special Agent",
	"Perfect Agent"
};

const char* const soundModeNames[NUM_SOUND_MODES]
{
	"Mono",
	"Stereo",
	"Headphone",
	"Surround"
};

const char* const screenSizeNames[NUM_SCREEN_SIZES]
{
	"Full",
	"Wide",
	"Cinema"
};

const char* const ratioNames[NUM_RATIOS]
{
	"Normal",
	"16:9"
};

const char* const screenSplitModeNames[NUM_SCREEN_SPLIT_MODES]
{
	"Horizontal",
	"Vertical"
};

const char* const aimControlModeNames[NUM_AIM_CONTROL_MODES]
{
	"Hold",
	"Toggle"
};

const char* const coOpPlayerNames[2]
{
	"Player 1",
	"Player 2"
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

const char* const firingRangeMedalNames[NUM_FIRING_RANGE_MEDALS]
{
	"Bronze",
	"Silver",
	"Gold"
};

const char* const weaponNames[NUM_FIRING_RANGE_WEAPONS]
{
	"Falcon 2",
	"Falcon 2 (silencer)",
	"Falcon 2 (scope)",
	"MagSec 4",
	"Mauler",
	"Phoenix",
	"DY357 Magnum",
	"DY357-LX",
	"CMP150",
	"Cyclone",
	"Callisto NTG",
	"RC-P120",
	"Laptop Gun",
	"Dragon",
	"K7 Avenger",
	"AR34",
	"SuperDragon",
	"Shotgun",
	"Reaper",
	"Sniper Rifle",
	"FarSight XR-20",
	"Devastator",
	"Rocket Launcher",
	"Slayer",
	"Combat Knife",
	"Crossbow",
	"Tranquilizer",
	"Laser",
	"Grenade",
	"Timed Mine",
	"Proximity Mine",
	"Remote Mine"
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






#define SAVE_SLOT_MAGIC 0x11
#define SAVE_SLOT_SIZE 0x78
#define GLOBAL_DATA_SIZE 0x20

enum class FileTypes
{
	GAME,
	MPSETUP,
	MPPLAYER,
	CAMERA
};

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

struct Range
{
	uint8_t min;
	uint8_t max;

	inline uint8_t Count() const { return (min == 0 && max == 0) ? 0 : (max + 1) - min; }
};

#pragma region Levels_Data

#define TOTAL_LEVEL_COUNT 11

const uint8_t levelIndices[TOTAL_LEVEL_COUNT]
{
	10, 5, 0, 1, 2, 3, 4, 6, 9, 8, 7
};

const char* const levelNames[TOTAL_LEVEL_COUNT]
{
	"Spiral Mountain",
	"Gruntilda's Lair",
	"Mumbo's Mountain",
	"Treasure Trove Cove",
	"Clanker's Cavern",
	"Bubblegloop Swamp",
	"Freezeezy Peak",
	"Gobi's Valley",
	"Mad Monster Mansion",
	"Rusty Bucket Bay",
	"Click Clock Wood"
};

#pragma endregion

#pragma region Jiggies_Data

#define JIGGIES_COUNT 100

const Range levelJiggiesIndexRanges[TOTAL_LEVEL_COUNT]
{
	{ 0x00, 0x00 }, // Spiral Mountain
	{ 0x33, 0x3C }, // Gruntilda's Lair
	{ 0x01, 0x0A }, // Mumbo's Mountain
	{ 0x0B, 0x14 }, // Treasure Trove Cove
	{ 0x15, 0x1E }, // Clanker's Cavern
	{ 0x1F, 0x28 }, // Bubblegloop Swamp
	{ 0x29, 0x32 }, // Freezeezy Peak
	{ 0x3D, 0x46 }, // Gobi's Valley
	{ 0x5B, 0x64 }, // Mad Monster Mansion
	{ 0x51, 0x5A }, // Rusty Bucket Bay
	{ 0x47, 0x50 }  // Click Clock Wood
};

const char* const levelJiggiesNames[JIGGIES_COUNT + 1]
{
	"",
	"Jinjos",      "Ticker's Tower",   "Mumbo's Skull",   "Juju",              "Huts",             "Ruins",           "Hill",             "Orange Pads",       "Chimpy",           "Conga"           , // Mumbo's Mountain
	"Jinjos",      "Lighthouse",       "Alcove 1",        "Alcove 2",          "Pond",             "Sand Castle",     "Red X",            "Nipper",            "Lockup",           "Blubber"         , // Treasure Trove Cove
	"Jinjos",      "Snippets",         "Clanker Raised",  "Bolt",              "Tail",             "Long Pipe",       "Tooth",            "Rings",             "Slow Sawblades",   "Fast Sawblades"  , // Clanker's Cavern
	"Jinjos",      "Elevated Walkway", "Pink Egg",        "Croctus",           "Huts",             "Flibbits",        "Maze",             "Tanktup",           "Tiptup",           "Mr. Vile"        , // Bubblegloop Swamp
	"Jinjos",      "Bounce on Boggy",  "Pipe",            "Second Boggy Race", "Snowman Buttons",  "Presents",        "Christmas Tree",   "First Boggy Race",  "Sir Slush",        "Wozza"           , // Freezeezy Peak
	"First Jiggy", "MM Witch Switch",  "CC Witch Switch", "TTC Witch Switch",  "BGS Witch Switch", "FP Witch Switch", "MMM Witch Switch", "GV Witch Switch",   "RBB Witch Switch", "CCW Witch Switch", // Gruntilda's Lair
	"Jinjos",      "Grabba",           "Shyinx",          "Matching Game",     "Maze",             "Water Pyramid",   "Histup",           "Gobi 1",            "Gobi 2",           "Ancient Ones"    , // Gobi's Valley
	"Jinjos",      "House",            "Eyrie",           "Nabnut",            "Gnawty",           "Zubbas",          "Flower",           "Summer Leaf Jumps", "Tree Top",         "Top Room"        , // Click Clock Wood
	"Jinjos",      "Warehouse",        "Snorkel",         "Whistle",           "Funnel",           "Boss Boom Box",   "Propellor",        "Captain's Cabin",   "Crane Cage",       "Engine Room"     , // Rusty Bucket Bay
	"Jinjos",      "Well",             "Napper",          "Cellar",            "Church Roof",      "Motzhand",        "Rain Barrel",      "Tumblar",           "Flower Pots",      "Loggo"             // Mad Monster Mansion
};

#pragma endregion

#pragma region Honeycomb_Data

#define HONEYCOMB_COUNT 24

const Range levelHoneycombsIndexRanges[TOTAL_LEVEL_COUNT]
{
	{ 0x13, 0x18 }, // Spiral Mountain
	{ 0x00, 0x00 }, // Gruntilda's Lair
	{ 0x01, 0x02 }, // Mumbo's Mountain
	{ 0x03, 0x04 }, // Treasure Trove Cove
	{ 0x05, 0x06 }, // Clanker's Cavern
	{ 0x07, 0x08 }, // Bubblegloop Swamp
	{ 0x09, 0x0A }, // Freezeezy Peak
	{ 0x0B, 0x0C }, // Gobi's Valley
	{ 0x11, 0x12 }, // Mad Monster Mansion
	{ 0x0F, 0x10 }, // Rusty Bucket Bay
	{ 0x0D, 0x0E }  // Click Clock Wood
};

const char* const levelHoneycombsNames[HONEYCOMB_COUNT + 1]
{
	"",
	"Hill",           "Juju",                                                           // Mumbo's Mountain
	"Underwater",     "Floating Box",                                                   // Treasure Trove Cove
	"Underwater",     "Above Water",                                                    // Clanker's Cavern
	"Mumbo's Skull",  "Tanktup",                                                        // Bubblegloop Swamp
	"Wozza's Cave",   "Sir Slush",                                                      // Freezeezy Peak
	"Cactus",         "Gobi",                                                           // Gobi's Valley
	"Gnawty's House", "Nabnut's House",                                                 // Click Clock Wood
	"Boat House",     "Engine Room",                                                    // Rusty Bucket Bay
	"Church Rafters", "Floorboard",                                                     // Mad Monster Mansion
	"Stump",          "Waterfall",      "Underwater", "Tree", "Coliwobble", "Quarries"  // Spiral Mountain
};

#pragma endregion

#pragma region MumboTokens_Data

#define MUMBO_TOKEN_COUNT 126
#define ACTUAL_MUMBO_TOKEN_COUNT 115

const Range levelMumboTokensIndexRanges[TOTAL_LEVEL_COUNT]
{
	{ 0x00, 0x00 }, // Spiral Mountain
	{ 0x51, 0x5A }, // Gruntilda's Lair
	{ 0x01, 0x05 }, // Mumbo's Mountain
	{ 0x06, 0x0F }, // Treasure Trove Cove
	{ 0x10, 0x14 }, // Clanker's Cavern
	{ 0x15, 0x1E }, // Bubblegloop Swamp
	{ 0x1F, 0x28 }, // Freezeezy Peak
	{ 0x29, 0x32 }, // Gobi's Valley
	{ 0x33, 0x41 }, // Mad Monster Mansion
	{ 0x42, 0x50 }, // Rusty Bucket Bay
	{ 0x5B, 0x73 }  // Click Clock Wood
};

enum class MumboTokens
{
	MUMBOTOKEN_01_MM_STUMP_NEAR_CONGA = 1,
	MUMBOTOKEN_02_MM_BEHIND_RUINS,
	MUMBOTOKEN_03_MM_UNDER_RAMP_TO_MUMBOS_SKULL,
	MUMBOTOKEN_04_MM_BEHIND_PINK_JINJO,
	MUMBOTOKEN_05_MM_INSIDE_TICKERS_TOWER,
	MUMBOTOKEN_06_TTC_INSIDE_SALTY_HIPPO,
	MUMBOTOKEN_07_TTC_INSIDE_LOCKUP_1,
	MUMBOTOKEN_08_TTC_INSIDE_LOCKUP_2,
	MUMBOTOKEN_09_TTC_SALTY_HIPPO_MAST,
	MUMBOTOKEN_0A_TTC_BEHIND_LIGHTHOUSE_DOOR,
	MUMBOTOKEN_0B_TTC_ON_FLOATING_BOX,
	MUMBOTOKEN_0C_TTC_BEHIND_PILLAR_UNDERWATER,
	MUMBOTOKEN_0D_TTC_POND_BOTTOM,
	MUMBOTOKEN_0E_TTC_FLOATING_ABOVE_SHOCKJUMP_PAD,
	MUMBOTOKEN_0F_TTC_BEHIND_NIPPER,
	MUMBOTOKEN_10_CC_CHOMPA_BEHIND_CLANKERS_TAIL,
	MUMBOTOKEN_11_CC_ABOVE_LEVEL_ENTRANCE,
	MUMBOTOKEN_12_CC_TUNNEL_RIGHT_OF_CLANKER,
	MUMBOTOKEN_13_CC_ALCOVE_WITH_OPEN_GRATE,
	MUMBOTOKEN_14_CC_CLANKERS_LEFT_TOOTH,
	MUMBOTOKEN_15_BGS_UNDER_HUT_1,
	MUMBOTOKEN_16_BGS_UNDER_HUT_2,
	MUMBOTOKEN_17_BGS_TOP_OF_POLE_NEXT_TO_BIG_EGG,
	MUMBOTOKEN_18_BGS_BEHIND_YELLOW_JINJO,
	MUMBOTOKEN_19_BGS_FLOATING_BETWEEN_HUT_AND_CROCTUS,
	MUMBOTOKEN_1A_BGS_BEHIND_MUMBOS_SKULL,
	MUMBOTOKEN_1B_BGS_ALCOVE_ABOVE_YELLOW_FLIBBITS,
	MUMBOTOKEN_1C_BGS_BEHIND_TIPTUP,
	MUMBOTOKEN_1D_BGS_INSIDE_MR_VILES,
	MUMBOTOKEN_1E_BGS_INSIDE_MUMBOS_SKULL,
	MUMBOTOKEN_1F_FP_SNOWMAN_LEFT_FOOT,
	MUMBOTOKEN_20_FP_SNOWMAN_RIGHT_FOOT,
	MUMBOTOKEN_21_FP_BEHIND_PILE_PRESENT_BOXES,
	MUMBOTOKEN_22_FP_ABOVE_CHIMNEY_FLIGHT_PAD,
	MUMBOTOKEN_23_FP_INSIDE_SIR_SLUSH_NEAR_SNOWMANS_LEFT_FOOT,
	MUMBOTOKEN_24_FP_INSIDE_SIR_SLUSH_NEAR_SNOWMANS_RIGHT_FOOT,
	MUMBOTOKEN_25_FP_CHRISTMAS_TREE_POT,
	MUMBOTOKEN_26_FP_TIP_OF_SCARF,
	MUMBOTOKEN_27_FP_WATER_BEHIND_SNOWMAN,
	MUMBOTOKEN_28_FP_INSIDE_BOGGYS_IGLOO,
	MUMBOTOKEN_29_GV_TOP_OF_JINXYS_NOSE,
	MUMBOTOKEN_2A_GV_SAND_RIGHT_OF_JINXY,
	MUMBOTOKEN_2B_GV_BOTTOM_OF_MOAT,
	MUMBOTOKEN_2C_GV_TOP_OF_WATER_PYRAMID,
	MUMBOTOKEN_2D_GV_FRONT_OF_STAR_DOOR,
	MUMBOTOKEN_2E_GV_INSIDE_MEMORY_GAME_ROOM,
	MUMBOTOKEN_2F_GV_BEHIND_SANDYBUTTS_SARCOPHAGUS,
	MUMBOTOKEN_30_GV_INSIDE_WATER_PYRAMID,
	MUMBOTOKEN_31_GV_NEXT_TO_RUBEE,
	MUMBOTOKEN_32_GV_INSIDE_JINXY,
	MUMBOTOKEN_33_MMM_HIDDEN_NEAR_FOUNTAIN,
	MUMBOTOKEN_34_MMM_NEAR_FRONT_TUMBLARS_SHED,
	MUMBOTOKEN_35_MMM_ALCOVE_CLOCK_TOWER,
	MUMBOTOKEN_36_MMM_NEXT_TO_MAZE_HEDGE_RAMP,
	MUMBOTOKEN_37_MMM_IN_THE_MAZE,
	MUMBOTOKEN_38_MMM_BEHIND_CHURCH,
	MUMBOTOKEN_39_MMM_INSIDE_WHIPCRACK_IN_FOUNTAIN,
	MUMBOTOKEN_3A_MMM_TOP_OF_RAFTER_INSIDE_CHURCH,
	MUMBOTOKEN_3B_MMM_TOP_OF_STOOL_INSIDE_CHURCH,
	MUMBOTOKEN_3C_MMM_TOP_OF_TUMBLARS_SHED,
	MUMBOTOKEN_3D_MMM_INSIDE_LOGGO_AND_INSIDE_BARREL_IN_CELLAR, // Bug: Two tokens have same ID
	MUMBOTOKEN_3E_MMM_IN_CHIMNEY_INSIDE_DINNING_ROOM,
	MUMBOTOKEN_3F_MMM_INSIDE_WELL,
	MUMBOTOKEN_40_MMM_INSIDE_BEDROOM,
	MUMBOTOKEN_41_MMM_SINK_IN_LOGGOS_ROOM,
	MUMBOTOKEN_42_RBB_TOP_OF_SHIPS_CHIMNEY,
	MUMBOTOKEN_43_RBB_SHIPS_PROW,
	MUMBOTOKEN_44_RBB_ON_LIFE_BOAT,
	MUMBOTOKEN_45_RBB_FRONT_OF_CHOMPA_LEFT_OF_SHIP,
	MUMBOTOKEN_46_RBB_TOXIC_WASTE_POND,
	MUMBOTOKEN_47_RBB_UNDER_CHOMPA_IN_WITCH_SWITCH_TOWER,
	MUMBOTOKEN_48_RBB_UNDER_CHOMPA_INSIDE_FIRST_BLUE_CONTAINER,
	MUMBOTOKEN_49_RBB_TOP_OF_BOX_INSIDE_SECOND_BLUE_CONTAINER,
	MUMBOTOKEN_4A_RBB_TOP_OF_BUNK_IN_CABIN,
	MUMBOTOKEN_4B_RBB_INSIDE_NAVIGATION_ROOM,
	MUMBOTOKEN_4C_RBB_INSIDE_KITCHEN,
	MUMBOTOKEN_4D_RBB_ENGINE_ROOM_LEFT_PIPE,
	MUMBOTOKEN_4E_RBB_ENGINE_ROOM_RIGHT_PIPE,
	MUMBOTOKEN_4F_RBB_ENGINE_ROOM_SPINNING_PLATFORM,
	MUMBOTOKEN_50_RBB_TOP_OF_BOX_INSIDE_TNT_ROOM,
	MUMBOTOKEN_51_LAIR_BEHIND_PURPLE_CAULDRON,
	MUMBOTOKEN_52_LAIR_CCW_JIGGY_PODIUM,
	MUMBOTOKEN_53_LAIR_TOP_OF_PIPE_IN_ORANGE_CAULDRON_ROOM,
	MUMBOTOKEN_54_LAIR_ABOVE_CC_ENTRANCE,
	MUMBOTOKEN_55_LAIR_BEHIND_SARCOPHAGUS,
	MUMBOTOKEN_56_LAIR_ADVENT_TOP_WINDOW,
	MUMBOTOKEN_57_LAIR_BEHIND_MUMBO_IN_CRYPT,
	MUMBOTOKEN_58_LAIR_ABOVE_NOTE_DOOR_WATER_AREA,
	MUMBOTOKEN_59_LAIR_UNDERWATER_RBB_ENTRANCE_AREA,
	MUMBOTOKEN_5A_LAIR_WHIPCRACK_NEAR_MMM_JIGGY_PODIUM,
	MUMBOTOKEN_5B_CCW_SPRING_UNFINISHED_WOOD_TREEHOUSE,
	MUMBOTOKEN_5C_CCW_SPRING_TOP_OF_BRANCH,
	MUMBOTOKEN_5D_CCW_SPRING_FRONT_OF_EYRIE,
	MUMBOTOKEN_5E_CCW_SPRING_THORN_FIELD_FRONT_OF_MUMBOS_SKULL,
	MUMBOTOKEN_5F_CCW_SPRING_SNAREBEAR_NEAR_BIG_FLOWER,
	MUMBOTOKEN_60_CCW_SPRING_SNAREBEAR_NEAR_ENTRANCE,
	MUMBOTOKEN_61_CCW_SPRING_TOP_OF_BEEHIVE,
	MUMBOTOKEN_62_CCW_SPRING_NABNUTS_HOUSE,
	MUMBOTOKEN_63_CCW_SUMMER_PLATFORMS_BEFORE_EYRIES_NEST,
	MUMBOTOKEN_64_CCW_SUMMER_BIG_FLOWER_FIELD,
	MUMBOTOKEN_65_CCW_SUMMER_SNAREBEAR_NEAR_THORN_FIELD,
	MUMBOTOKEN_66_CCW_SUMMER_TOP_OF_BRANCH,
	MUMBOTOKEN_67_CCW_SUMMER_ENTRANCE_GNAWTYS_HOUSE,
	MUMBOTOKEN_68_CCW_SUMMER_FLOATING_ABOVE_BIG_CLUCKER,
	MUMBOTOKEN_69_CCW_SUMMER_INSIDE_MUMBOS_SKULL,
	MUMBOTOKEN_6A_CCW_AUTUMN_FLOATING_ABOVE_BIG_CLUCKER,
	MUMBOTOKEN_6B_CCW_AUTUMN_SNAREBEAR_NEAR_ENTRANCE,
	MUMBOTOKEN_6C_CCW_AUTUMN_SNAREBEAR_TOP_OF_TREE,
	MUMBOTOKEN_6D_CCW_AUTUMN_LEAF_NEAR_TREEHOUSE,
	MUMBOTOKEN_6E_CCW_AUTUMN_TOP_OF_BRANCH,
	MUMBOTOKEN_6F_CCW_WINTER_BIG_FLOWER,
	MUMBOTOKEN_70_CCW_WINTER_FROZEN_RIVER_NEAR_FLIGHT_PAD,
	MUMBOTOKEN_71_CCW_WINTER_BROKEN_BEEHIVE,
	MUMBOTOKEN_72_CCW_WINTER_WALKWAY_FRONT_OF_NABNUTS_HOUSE,
	MUMBOTOKEN_73_CCW_WINTER_SIR_SLUSH_BETWEEN_BIG_FLOWER_AND_MUMBOS_SKULL
};

const char* const MumboTokenNames[ACTUAL_MUMBO_TOKEN_COUNT + 1]
{
	"",
	"Stump near Conga",
	"Behind ruins",
	"Under the ramp to Mumbo's Skull",
	"Behind pink Jinjo",
	"Inside Ticker's tower",
	"Inside Salty Hippo",
	"Inside Lockup 1",
	"Inside Lockup 2",
	"Salty Hippo's mast",
	"Behind lighthouse door",
	"On top of floating box",
	"Underwater behind pillar",
	"Bottom of the Pond",
	"Floating above Shock Jump Pad",
	"Behind Nipper",
	"Chompa behind Clanker's tail",
	"Above the level entrance",
	"Tunnel to the right of Clanker",
	"Alcove with open grate",
	"Clanker's left tooth",
	"Under a hut 1",
	"Under a hut 2",
	"Top of the pole next to big egg",
	"Behind yellow Jinjo",
	"Floating between hut and Croctus",
	"Behind Mumbo's Skull",
	"Alcove above the yellow Flibbits",
	"Behind Tiptup",
	"Inside Mr. Vile's",
	"Inside Mumbo's Skull",
	"Snowman left foot",
	"Snowman right foot",
	"Behind the pile of present boxes",
	"Above the chimney's Flight Pad",
	"Inside the Sir Slush near the Snowman's left foot",
	"Inside the Sir Slush near the Snowman's right foot",
	"Christmas tree's pot",
	"Tip of the scarf",
	"Water behind the Snowman",
	"Inside Boggy's igloo",
	"Top of Jinxy's nose",
	"Sand at the right of Jinxy",
	"Bottom of the moat",
	"Top of the water pyramid",
	"Front of the star door",
	"Inside the memory game room",
	"Behind Sandybutt's sarcophagus",
	"Inside the water pyramid",
	"Next to Rubee",
	"Inside Jinxy",
	"Hidden near the fountain",
	"Near the front of Tumblar's shed",
	"Alcove in the clock tower",
	"Next to the maze's hedge ramp",
	"In the maze",
	"Behind the church",
	"Inside the Whipcrack in the fountain",
	"On top of the wood rafter inside the church",
	"On top of the stool inside the church",
	"On top of Tumblar's shed",
	"Inside Loggo / Inside barrel in cellar (Game bug: these two tokens share the same ID)",
	"In chimney inside the dinning room",
	"Inside the well",
	"Inside the bedroom",
	"Sink in Loggo's room",
	"Top of the ship's chimney",
	"Ship's prow",
	"On a life boat",
	"In front of Chompa at the left of the ship",
	"At the toxic waste pond",
	"Under the Chompa in the tower where the Witch Switch is",
	"Under the Chompa inside the first blue container",
	"On top of the box inside the second blue container",
	"On top of the bunk in the cabin",
	"Inside the navigation room",
	"Inside the oven in the kitchen",
	"Engine room's left pipe",
	"Engine room's right pipe",
	"Engine room's spinning platform",
	"On top of the box inside the TNT room",
	"Behind purple Cauldron",
	"Room of the CCW Jiggy podium",
	"On top of the pipe in the orange Cauldron room",
	"Above the CC entrance",
	"Behind the sarcophagus",
	"Advent's top window",
	"Behind Mumbo in the crypt",
	"Above the note door in the water area",
	"Underwater in the RBB entrance area",
	"Whipcrack near the MMM Jiggy podium",
	"[Spring] Unfinished treehouse",
	"[Spring] On top of a branch",
	"[Spring] In front of Eyrie",
	"[Spring] Thorn field in front of Mumbo's Skull",
	"[Spring] Snarebear near the big flower",
	"[Spring] Snarebear near the entrance",
	"[Spring] On top of the beehive",
	"[Spring] Nabnut's house",
	"[Summer] Platforms before Eyrie's nest",
	"[Summer] Big flower's field",
	"[Summer] Snarebear near the thorn field",
	"[Summer] On top of a branch",
	"[Summer] Entrance to Gnawty's house",
	"[Summer] Floating above a Big Clucker",
	"[Summer] Inside Mumbo's Skull",
	"[Autumn] Floating above a Big Clucker",
	"[Autumn] Snarebear near the entrance",
	"[Autumn] Snarebear at the top of the tree",
	"[Autumn] Leaf near the treehouse",
	"[Autumn] On top of a branch",
	"[Winter] Big flower",
	"[Winter] Frozen river near the Flight Pad",
	"[Winter] Broken beehive",
	"[Winter] Walkway in front of Nabnut's house",
	"[Winter] Sir Slush between the big flower and Mumbo's Skull"
};

#pragma endregion

#pragma region Notes_Data

#define NOTES_COUNT 900
#define MAX_NOTES_PER_LEVEL 100

const bool levelHasNotes[TOTAL_LEVEL_COUNT]
{
	false, false, true, true, true, true, true, true, true, true, true
};

const int8_t levelNotesBitOffsets[TOTAL_LEVEL_COUNT]
{
	-1, -1, 56, 49, 42, 35, 28, 21, 0, 7, 14
};

#pragma endregion

#pragma region Progress Flags

enum class ProgressFlags
{
	FILEPROG_3_MUSIC_NOTE_TEXT = 0x3, // Done
	FILEPROG_4_MUMBO_TOKEN_TEXT, // Done
	FILEPROG_5_BLUE_EGG_TEXT, // Done
	FILEPROG_6_RED_FEATHER_TEXT, // Done
	FILEPROG_7_GOLD_FEATHER_TEXT, // Done
	FILEPROG_8_ORANGE_TEXT, // Done
	FILEPROG_9_GOLD_BULLION_TEXT, // Done
	FILEPROG_A_HONEYCOMB_TEXT, // Done
	FILEPROG_B_EMPTY_HONEYCOMB_TEXT, // Done
	FILEPROG_C_EXTRA_LIFE_TEXT, // Done
	FILEPROG_D_BEEHIVE_TEXT, // Done
	FILEPROG_E_JINJO_TEXT, // Done
	FILEPROG_F_HAS_TOUCHED_PIRAHANA_WATER, // Done
	FILEPROG_10_HAS_TOUCHED_SAND_EEL_SAND, // Done
	FILEPROG_11_HAS_MET_MUMBO, // Done
	FILEPROG_12_HAS_TRANSFORMED_BEFORE, // Done
	FILEPROG_13_COMPLETED_TWINKLIES_MINIGAME, // Done
	FILEPROG_14_HAS_TOUCHED_FP_ICY_WATER, // Done
	FILEPROG_15_ENTER_MMM_TEXT, // Done
	FILEPROG_16_STOOD_ON_JIGSAW_PODIUM, // Done
	FILEPROG_17_HAS_HAD_ENOUGH_JIGSAW_PIECES, // Done
	FILEPROG_18_MM_WITCH_SWITCH_JIGGY_PRESSED, // Done
	FILEPROG_19_MMM_WITCH_SWITCH_JIGGY_PRESSED, // Done
	FILEPROG_1A_TTC_WITCH_SWITCH_JIGGY_PRESSED, // Done
	FILEPROG_1B_MET_YELLOW_FLIBBITS, // Done
	FILEPROG_1C_RBB_WITCH_SWITCH_JIGGY_PRESSED, // Done
	FILEPROG_1D_MMM_DINNING_ROOM_CUTSCENE, // Done
	FILEPROG_1E_LAIR_GRATE_TO_BGS_PUZZLE_OPEN, // Done
	FILEPROG_1F_CC_LOBBY_PIPE_1_RAISED, // Done
	FILEPROG_20_CC_LOBBY_PIPE_2_RAISED, // Done
	FILEPROG_21_CC_LOBBY_PIPE_3_RAISED, // Done
	FILEPROG_22_WATER_SWITCH_1_PRESSED, // Done
	FILEPROG_23_LAIR_WATER_LEVEL_1, // Done
	FILEPROG_24_WATER_SWITCH_2_PRESSED, // Done
	FILEPROG_25_LAIR_WATER_LEVEL_2, // Done
	FILEPROG_26_WATER_SWITCH_3_PRESSED, // Done
	FILEPROG_27_LAIR_WATER_LEVEL_3, // Done

	FILEPROG_31_MM_OPEN = 0x31, // Done
	FILEPROG_32_TTC_OPEN, // Done
	FILEPROG_33_CC_OPEN, // Done
	FILEPROG_34_BGS_OPEN, // Done
	FILEPROG_35_FP_OPEN, // Done
	FILEPROG_36_GV_OPEN, // Done
	FILEPROG_37_MMM_OPEN, // Done
	FILEPROG_38_RBB_OPEN, // Done
	FILEPROG_39_CCW_OPEN, // Done
	FILEPROG_3A_NOTE_DOOR_50_OPEN, // Done
	FILEPROG_3B_NOTE_DOOR_180_OPEN, // Done
	FILEPROG_3C_NOTE_DOOR_260_OPEN, // Done
	FILEPROG_3D_NOTE_DOOR_350_OPEN, // Done
	FILEPROG_3E_NOTE_DOOR_450_OPEN, // Done
	FILEPROG_3F_NOTE_DOOR_640_OPEN, // Done
	FILEPROG_40_NOTE_DOOR_765_OPEN, // Done
	FILEPROG_41_NOTE_DOOR_810_OPEN, // Done
	FILEPROG_42_NOTE_DOOR_828_OPEN, // Done
	FILEPROG_43_NOTE_DOOR_846_OPEN, // Done
	FILEPROG_44_NOTE_DOOR_864_OPEN, // Done
	FILEPROG_45_NOTE_DOOR_882_OPEN, // Done
	FILEPROG_46_CCW_WITCH_SWITCH_JIGGY_PRESSED, // Done
	FILEPROG_47_FP_WITCH_SWITCH_JIGGY_PRESSED, // Done
	FILEPROG_48_FP_WITCH_SWITCH_ADVENT_DOOR_OPEN, // Done
	FILEPROG_49_PINK_CAULDRON_1_ACTIVE, // Done
	FILEPROG_4A_PINK_CAULDRON_2_ACTIVE, // Done
	FILEPROG_4B_GREEN_CAULDRON_1_ACTIVE, // Done
	FILEPROG_4C_GREEN_CAULDRON_2_ACTIVE, // Done
	FILEPROG_4D_RED_CAULDRON_1_ACTIVE, // Done
	FILEPROG_4E_RED_CAULDRON_2_ACTIVE, // Done
	FILEPROG_4F_UNUSED_CAULDRON_1_ACTIVE, // Done
	FILEPROG_50_UNUSED_CAULDRON_2_ACTIVE, // Done
	FILEPROG_51_YELLOW_CAULDRON_1_ACTIVE, // Done
	FILEPROG_52_YELLOW_CAULDRON_2_ACTIVE, // Done
	FILEPROG_53_CCW_PUZZLE_PODIUM_SWITCH_PRESSED, // Done
	FILEPROG_54_CCW_PUZZLE_PODIUM_ACTIVE, // Done
	FILEPROG_55_FF_BK_SQUARE_INSTRUCTIONS, // Done
	FILEPROG_56_FF_PICTURE_SQUARE_INSTRUCTIONS, // Done
	FILEPROG_57_FF_MUSIC_SQAURE_INSTRUCTIONS, // Done
	FILEPROG_58_FF_MINIGAME_SQAURE_INSTRUCTIONS, // Done
	FILEPROG_59_FF_GRUNTY_SQAURE_INSTRUCTIONS, // Done
	FILEPROG_5A_FF_DEATH_SQAURE_INSTRUCTIONS, // Done
	FILEPROG_5B_FF_JOKER_SQAURE_INSTRUCTIONS, // Done
	FILEPROG_5C_FF_PATTERN_SET, // Done
	FILEPROG_5D_MM_PUZZLE_PIECES_PLACED = 0x5D, //1 bit // Done
	FILEPROG_5E_TCC_PUZZLE_PIECES_PLACED = 0x5E, //2 bit // Done
	FILEPROG_60_CC_PUZZLE_PIECES_PLACED = 0x60, //3 bit // Done
	FILEPROG_63_BGS_PUZZLE_PIECES_PLACED = 0x63, //3 bit // Done
	FILEPROG_66_FP_PUZZLE_PIECES_PLACED = 0x66, //4 bit // Done
	FILEPROG_6A_GV_PUZZLE_PIECES_PLACED = 0x6A, //4 bit // Done
	FILEPROG_6E_MMM_PUZZLE_PIECES_PLACED = 0x6E, //4 bit // Done
	FILEPROG_72_RBB_PUZZLE_PIECES_PLACED = 0x72, //4 bit // Done
	FILEPROG_76_CCW_PUZZLE_PIECES_PLACED = 0x76, //4 bit // Done
	FILEPROG_7A_DOG_PUZZLE_PIECES_PLACED = 0x7A, //5 bit // Done
	FILEPROG_7F_DOUBLE_HEALTH_PUZZLE_PIECES_PLACED = 0x7F, //3 bit // Done
	// {index=0x82, unknown}
	FILEPROG_82_MET_TWINKLIES = 0x82, // Done
	FILEPROG_83_MAGIC_GET_WEAK_TEXT, // Done
	FILEPROG_84_MAGIC_ALL_GONE_TEXT, // Done

	FILEPROG_86_HAS_TOUCHED_MMM_THORN_HEDGE = 0x86, // Done

	FILEPROG_88_TRIED_LOGGO_AS_BEAR = 0x88, // Done
	FILEPROG_89_ENTERED_LOGGO_AS_PUMPKIN, // Done
	FILEPROG_8A_EXITED_LOGGO, // Done
	FILEPROG_8B_CCW_SPRING_OPEN, // Done
	FILEPROG_8C_CCW_SUMMER_OPEN, // Done
	FILEPROG_8D_CCW_AUTUMN_OPEN, // Done
	FILEPROG_8E_CCW_WINTER_OPEN, // Done
	FILEPROG_8F_MET_BEE_INFESTED_BEEHIVE, // Done
	FILEPROG_90_PAID_TERMITE_COST, // Done
	FILEPROG_91_PAID_PUMPKIN_COST, // Done
	FILEPROG_92_PAID_WALRUS_COST, // Done
	FILEPROG_93_PAID_CROC_COST, // Done
	FILEPROG_94_PAID_BEE_COST, // Done

	FILEPROG_96_MET_BRENTILDA = 0x96, // Done
	FILEPROG_97_ENTERED_LAIR_TEXT, // Done
	FILEPROG_98_EXITED_LEVEL_TEXT, // Done
	FILEPROG_99_PAST_50_NOTE_DOOR_TEXT, // Done
	FILEPROG_9A_CC_WITCH_SWITCH_PRESSED, // Done
	FILEPROG_9B_LAIR_CC_WITCH_SWITCH_EYES_ACTIVE, // Done
	FILEPROG_9C_LAIR_CC_WITCH_SWITCH_LEFT_EYE_PRESSED, // Done
	FILEPROG_9D_LAIR_CC_WITCH_SWITCH_RIGHT_EYE_PRESSED, // Done
	FILEPROG_9E_CRYPT_COFFIN_LID_OPEN, // Done
	FILEPROG_9F_BGS_WITCH_SWITCH_JIGGY_PRESSED, // Done
	FILEPROG_A0_GV_WITCH_SWITCH_JIGGY_PRESSED, // Done
	FILEPROG_A1_STATUE_HAT_OPEN, // Done
	FILEPROG_A2_GV_LOBBY_COFFIN_OPEN, // Done
	FILEPROG_A3_GV_SNS_SWITCH_PRESSED, // Done
	FILEPROG_A4_GV_SNS_SARCOPHAGUS_OPEN, // Done
	FILEPROG_A5_LAIR_CRYPT_GATE_OPEN, // Done
	FILEPROG_A6_FURNACE_FUN_COMPLETE = 0xA6, // Done
	FILEPROG_A7_NEAR_PUZZLE_PODIUM_TEXT, // Done
	FILEPROG_A8_HAS_DIED, // Done
	FILEPROG_A9_HAS_TOUCHED_RBB_OVEN, // Done
	FILEPROG_AA_HAS_TOUCHED_CCW_BRAMBLE_FIELD, // Done
	FILEPROG_AB_SWIM_OILY_WATER, // Done
	FILEPROG_AC_DIVE_OILY_WATER, // Done
	FILEPROG_AD_CHEATO_BLUEEGGS_UNLOCKED, // Done
	FILEPROG_AE_CHEATO_REDFEATHERS_UNLOCKED, // Done
	FILEPROG_AF_CHEATO_GOLDFEATHERS_UNLOCKED, // Done
	FILEPROG_B0_HAS_ENTERED_MM, // Done
	FILEPROG_B1_HAS_ENTERED_BGS, // Done
	FILEPROG_B2_HAS_ENTERED_TTC, // Done
	FILEPROG_B3_HAS_ENTERED_GV, // Done
	FILEPROG_B4_HAS_ENTERED_RBB, // Done
	FILEPROG_B5_HAS_ENTERED_CCW, // Done
	FILEPROG_B6_HAS_ENTERED_FP, // Done
	FILEPROG_B7_HAS_ENTERED_MMM, // Done
	FILEPROG_B8_HAS_ENTERED_CC, // Done
	FILEPROG_B9_DOUBLE_HEALTH, // Done
	FILEPROG_BA_HAS_SEEN_TREX_TEXT, // Done
	FILEPROG_BB_MUMBO_MISTAKE_INDEX, //2 bits // Done
	FILEPROG_BD_ENTER_LAIR_CUTSCENE = 0xBD, // Done
	FILEPROG_BE_CHEATO_BLUEEGGS, // Done
	FILEPROG_BF_CHEATO_REDFEATHERS, // Done
	FILEPROG_C0_CHEATO_GOLDFEATHERS, // Done
	FILEPROG_C1_BADDIES_ESCAPE_TEXT, // Done
	FILEPROG_C2_GRATE_TO_RBB_PUZZLE_OPEN, // Done
	FILEPROG_C3_ICE_BALL_TO_CHEATO_BROKEN, // Done
	FILEPROG_C4_STATUE_EYE_BROKEN, // Done
	FILEPROG_C5_RAREWARE_BOX_BROKEN, // Done
	FILEPROG_C6_LAIR_JUMP_PAD_SWITCH_PRESSED, // Done
	FILEPROG_C7_LAIR_JUMP_PAD_ACTIVE, // Done
	FILEPROG_C8_LAIR_BRICKWALL_TO_WADINGBOOTS_BROKEN, // Done
	FILEPROG_C9_LAIR_BRICKWALL_TO_SHOCKJUMP_PAD_BROKEN, // Done
	FILEPROG_CA_COBWEB_BLOCKING_PURPLE_CAULDRON_BROKEN, // Done
	FILEPROG_CB_LAIR_COBWEB_OVER_FLIGHTPAD_BROKEN, // Done
	FILEPROG_CC_LAIR_COBWEB_OVER_GREEN_CAULDRON_BROKEN, // Done
	FILEPROG_CD_GRATE_TO_WATER_SWITCH_3_OPEN, // Done
	FILEPROG_CE_GRATE_TO_MMM_PUZZLE_OPEN, // Done
	FILEPROG_CF_HAS_ENTERED_FINAL_FIGHT, // Done

	FILEPROG_D1_HAS_ACTIVATED_A_JINJO_STATUE_IN_FINAL_FIGHT = 0xD1, // Done
	FILEPROG_D2_HAS_SPAWNED_A_JINJO_STATUE_IN_FINAL_FIGHT, // Done
	FILEPROG_D3_FF_PATTERN, //8 bit // Done


	// {index=0xDB, unknown},
	FILEPROG_DB_SKIPPED_TUTORIAL = 0xDB, // Done
	FILEPROG_DC_HAS_HAD_ENOUGH_TOKENS_BEFORE, // Done
	FILEPROG_DD_HAS_TOUCHED_CCW_ICY_WATER, // Done
	FILEPROG_DE_USED_ALL_YOUR_PUZZLE_PIECES, // Done
	FILEPROG_DF_CAN_REMOVE_ALL_PUZZLE_PIECES, // Done
	FILEPROG_E0_CAN_PLACE_ALL_PUZZLE_PIECES, // Done

	FILEPROG_E2_DOOR_OF_GRUNTY_OPEN = 0xE2, // Done
	FILEPROG_E3_CCW_FLOWER_SPRING, // Done
	FILEPROG_E4_CCW_FLOWER_SUMMER, // Done
	FILEPROG_E5_CCW_FLOWER_AUTUMN, // Done
	FILEPROG_E6_SPRING_EYRIE_HATCHED, // Done
	FILEPROG_E7_SUMMER_EYRIE_FED, // Done
	FILEPROG_E8_AUTMN_EYRIE_FED, // Done
	FILEPROG_E9_HEALED_BY_BRENTILDA_1, // Done
	FILEPROG_EA_HEALED_BY_BRENTILDA_2, // Done
	FILEPROG_EB_HEALED_BY_BRENTILDA_3, // Done
	FILEPROG_EC_HEALED_BY_BRENTILDA_4, // Done
	FILEPROG_ED_HEALED_BY_BRENTILDA_5, // Done
	FILEPROG_EE_HEALED_BY_BRENTILDA_6, // Done
	FILEPROG_EF_HEALED_BY_BRENTILDA_7, // Done
	FILEPROG_F0_HEALED_BY_BRENTILDA_8, // Done
	FILEPROG_F1_HEALED_BY_BRENTILDA_9, // Done
	FILEPROG_F2_HEALED_BY_BRENTILDA_10, // Done
	FILEPROG_F3_MET_DINGPOT, // Done
	FILEPROG_F4_ENTER_FF_CUTSCENE, // Done
	FILEPROG_F5_COMPLETED_A_WARP_CAULDRON_SET, // Done
	FILEPROG_F6_SEEN_DOOR_OF_GRUNTY_PUZZLE_PODIUM, // Done
	FILEPROG_F7_HAS_TRANSFORMED_IN_CRYPT, // Done
	FILEPROG_F8_KING_SANDYBUTT_PYRAMID_STATE = 0xF8, //2bit // Done

	FILEPROG_FC_DEFEAT_GRUNTY = 0xFC, // Done
	FILEPROG_FD_CHEATCODES_ENTERED //2 bit  // Done
};

#pragma endregion

#pragma region Items Held

#define MAX_MUMBO_TOKENS 116
#define MAX_EGGS 200
#define MAX_RED_FEATHERS 100
#define MAX_GOLD_FEATHERS 20

enum class HeldItems
{
	MumboTokens = 0,
	Eggs,
	RedFeathers,
	GoldFeathers,
	Jiggies
};

#pragma endregion

#pragma region Abilities

enum class Abilities
{
	ABILITY_0_BARGE = 0x0,
	ABILITY_1_BEAK_BOMB = 0x1,
	ABILITY_2_BEAK_BUSTER = 0x2,
	ABILITY_3_CAMERA_CONTROL = 0x3,
	ABILITY_4_CLAW_SWIPE = 0x4,
	ABILITY_5_CLIMB = 0x5,
	ABILITY_6_EGGS = 0x6,
	ABILITY_7_FEATHERY_FLAP = 0x7,
	ABILITY_8_FLAP_FLIP = 0x8,
	ABILITY_9_FLIGHT = 0x9,
	ABILITY_A_HOLD_A_JUMP_HIGHER = 0xA,
	ABILITY_B_RATATAT_RAP = 0xB,
	ABILITY_C_ROLL = 0xC,
	ABILITY_D_SHOCK_JUMP = 0xD,
	ABILITY_E_WADING_BOOTS = 0xE,
	ABILITY_F_DIVE = 0xF,
	ABILITY_10_TALON_TROT = 0x10,
	ABILITY_11_TURBO_TALON = 0x11,
	ABILITY_12_WONDERWING = 0x12,
	ABILITY_13_1ST_NOTEDOOR = 0x13
};

#pragma endregion

struct SaveSlot
{
private:
	uint8_t Magic;
	uint8_t SlotIndex;
	uint8_t Jiggies[13];
	uint8_t Honeycombs[3];
	uint8_t MumboTokens[16];
	uint8_t NoteScores[0x8];
	uint16_t Times[TOTAL_LEVEL_COUNT];
	uint8_t Flags[0x25];
	uint8_t Items[5];
	uint8_t LearnedAbilities[4];
	uint8_t UsedAbilities[4];
	uint8_t Padding[0x2];
	uint32_t Checksum;

public:
	//uint32_t CalculateChecksum() const;
	//void UpdateChecksum(const bool endianSwap);
	//bool IsValid(const bool endianSwap) const;

	uint8_t GetMagic() const;
	uint8_t GetSlotIndex() const;
	void SetSlotIndex(const uint8_t slotIndex);

	bool GetJiggy(const uint8_t jiggy) const;
	void SetJiggy(const uint8_t jiggy, bool value);

	bool GetHoneycomb(const uint8_t honeycomb) const;
	void SetHoneycomb(const uint8_t honeycomb, bool value);

	bool GetMumboToken(const uint8_t token) const;
	void SetMumboToken(const uint8_t token, bool value);

	uint8_t GetNotes(const uint8_t level) const;
	void SetNotes(const uint8_t level, const uint8_t value) const;

	uint16_t GetPlayTime(const uint8_t level, const bool endianSwap) const;
	void SetPlayTime(const uint8_t level, const uint16_t value, const bool endianSwap);

	bool GetProgressFlag(const ProgressFlags flag) const;
	void SetProgressFlag(const ProgressFlags flag, const bool value);

	uint8_t GetProgressValue(const ProgressFlags flag, const uint8_t bitsCount) const;
	void SetProgressValue(const ProgressFlags flag, const uint8_t bitsCount, const uint8_t value);

	uint8_t GetHeldItem(const HeldItems heldItem) const;
	void SetHeldItem(const HeldItems heldItem, const uint8_t value);

	bool GetLearnedAbility(const Abilities ability) const;
	void SetLearnedAbility(const Abilities ability, const bool value);

	bool GetUsedAbility(const Abilities ability) const;
	void SetUsedAbility(const Abilities ability, const bool value);

	uint32_t GetChecksum(const bool endianSwap) const;
};

struct FileGuid
{
	int32_t fileid;
	uint16_t deviceserial;
};

struct SaveBuffer
{
private:
	uint32_t bitpos;
	uint8_t bytes[SAVE_BUFFER_SIZE];

public:
	SaveBuffer();
	SaveBuffer(const uint8_t* bytes, const uint32_t size);

	uint8_t* GetBytes();

	uint32_t ReadBits(const int32_t numbits);
	void ReadGuid(FileGuid* guid);
	void WriteGuid(FileGuid* guid);
	void ReadString(char* dst);
	void WriteString(char* src);
	void Or(const uint32_t value, const int32_t numbits);
	void Clear();
};

struct BotData
{
public:
	uint8_t type = 0;
	uint8_t difficulty = 0;
	uint8_t mpheadnum = 0;
	uint8_t mpbodynum = 0;
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
	uint32_t deviceserial : 13;
	uint32_t fileid : 7;
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
	char teamNames[TEAM_NAMES_COUNT][MAX_NAME_LENGTH + 1] = {};
	uint8_t tracknum = 255;
	uint8_t multipletracknums[MULTIPLE_TRACKS_SIZE] = {};
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
	uint8_t mpheadnum = 0;
	uint8_t mpbodynum = 0;
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
	uint8_t gunfuncs[NUM_WEAPONS] = {};

public:
	void Load(uint8_t* fileBuffer) override;
};

struct MultiplayerSettings : public PakFile
{
public:
	char name[MAX_NAME_LENGTH + 1] = {};
	uint8_t stagenum = 0;
	uint8_t scenario = 0;
	uint8_t scenarioParams = 0; // Changes depending on scenario (KoH: Hill time)
	uint32_t options = 0;
	BotData botsData[MAX_BOTS] = {};
	uint8_t weaponSlots[NUM_MPWEAPONSLOTS] = {};
	uint8_t timelimit = 0;
	uint8_t scorelimit = 0;
	uint16_t teamscorelimit = 0;
	uint8_t teams[MAX_PLAYERS] = {};

public:
	void Load(uint8_t* fileBuffer) override;
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
	MultiplayerSettings mpSettings[ACTUAL_NUM_FILE_SLOTS] = {};
	Terminator terminator = {};

public:
	void Load(uint8_t* fileBuffer);
	void Save(uint8_t* fileBuffer);

	BossFile* GetBossFile(const uint8_t index) { return &bossFiles[index]; }
	GameFile* GetGameFile(const uint8_t index) { return &gameFiles[index]; }
	MultiplayerProfile* GetMultiplayerProfile(const uint8_t index) { return &mpProfiles[index]; }
	MultiplayerSettings* GetMultiplayerSettings(const uint8_t index) { return &mpSettings[index]; }

	static void CalculateChecksum(uint8_t* start, uint8_t* end, uint16_t* checksum);

private:
	static uint32_t TransformSeed(uint64_t* seed);
};