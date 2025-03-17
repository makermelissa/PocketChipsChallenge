#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <windows.h>

#define TICKS_PER_SECOND	20
#define SECOND_LENGTH		1100

//Turning Macros.
#define	left(dir)	((((dir) << 1) | ((dir) >> 3)) & 15)
#define	back(dir)	((((dir) << 2) | ((dir) >> 2)) & 15)
#define	right(dir)	((((dir) << 3) | ((dir) >> 1)) & 15)

//Filter Macros
#define creatureid(id)		((id) & ~3)
#define	creaturedirid(id)	(idxdir((id) & 3))
#define	crtile(id, dir)		((id) | diridx(dir))
#define	diridx(dir)			((0x30210 >> ((dir) * 2)) & 3)
#define	idxdir(idx)			(1 << ((idx) & 3))

//Directional Defines
#define NONE				-1
#define NORTH				1
#define WEST				2
#define SOUTH				4
#define EAST				8

//Arrow Pad Defines
#define	UP					0
#define	LEFT				1
#define	DOWN				2
#define	RIGHT				3
#define START				4
#define BUTTON_A			5
#define BUTTON_B			6
#define BUTTON_C			7
#define PAUSE				BUTTON_B

//Key State Defines
#define KS_OFF				0
#define KS_ON				1
#define KS_DOWN				2
#define KS_STRUCK			3
#define KS_PRESSED			4
#define KS_DOWNBUTOFF1		5
#define KS_DOWNBUTOFF2		6
#define KS_REPEATING		7

//Dialog Border Style Defines
#define BS_NONE				0
#define BS_OUTSET			1
#define BS_INSET			2

//Keyboard Defines
#define KB_NULL				0
#define KB_BACKSPACE		8
#define KB_CR				13
#define KB_SPACE			32
#define KB_0				48
#define KB_9				57
#define KB_A				65
#define KB_Z				90
#define KB_a				97
#define KB_z				122

//Field Defines
#define FLD_LEVEL			0
#define FLD_PASSWORD		1

//Key Defines
#define RedKey				0
#define BlueKey				1
#define YellowKey			2
#define GreenKey			3

//Boot Defines
#define IceBoots			0
#define SuctionBoots		1
#define FireBoots			2
#define WaterBoots			3

//Floor State Defines
#define FS_BUTTONDOWN		0x01
#define FS_CLONING			0x02
#define FS_BROKEN			0x08

//Creature State Defines
#define CS_RELEASED			0x01
#define CS_CLONING			0x02
#define CS_HASMOVED			0x04
#define	CS_TURNING			0x08
#define CS_SLIP				0x10
#define CS_SLIDE			0x20

//Can Make Move Defines
#define CMM_NOLEAVECHECK		0x0001
#define CMM_NOEXPOSEWALLS		0x0002
#define CMM_CLONECANTBLOCK		0x0004
#define CMM_NOCOLLAPSEBLOCKS	0x0008

//Status Flag Defines
#define SF_CHIPWAITMASK		0x0007
#define SF_CHIPOKAY			0x0000
#define SF_CHIPBURNED		0x0010
#define SF_CHIPBOMBED		0x0020
#define SF_CHIPDROWNED		0x0030
#define SF_CHIPHIT			0x0040
#define SF_CHIPTIMEUP		0x0050
#define SF_CHIPBLOCKHIT		0x0060
#define SF_CHIPNOTOKAY		0x0070
#define SF_CHIPSTATUSMASK	0x0070
#define SF_DEFERBUTTONS		0x0080
#define SF_COMPLETED		0x0100
#define	SF_SHOWHINT		0x10000000

//Game Mode Defines
#define GM_NONE				0
#define GM_PAUSED			1
#define GM_CHIPDEAD			2
#define GM_SHUTDOWN			3
#define GM_LEVELWON			4
#define GM_LOADING			5
#define GM_TITLE			6
#define GM_MENU				7
#define GM_SOUND			9
#define GM_BESTSCORE		10
#define GM_PASSWORD			11
#define GM_GAMEWON			12
#define GM_LEVELGOTO		13
#define GM_NEWGAME			14
#define GM_NORMAL			15

//Menu Masks
#define MENU_MASK1			0x0F
#define MENU_MASK2			0xF0

//Menu Defines
#define MENU_MAIN			0x01
#define MENU_GAME			0x10
#define MENU_LEVEL			0x20

//Timer Defines
#define GAME_TIMER			1009

//Tile Defines
#define NoTile				-1
#define Empty				0x00
#define Wall				0x01
#define ICChip				0x02
#define Water				0x03
#define Fire				0x04
#define HiddenWall_Perm		0x05
#define Wall_North			0x06
#define Wall_West			0x07
#define Wall_South			0x08
#define Wall_East			0x09
#define Block_Static		0x0A
#define Dirt				0x0B
#define Ice					0x0C
#define Slide_South			0x0D
#define Slide_North			0x12
#define Slide_East			0x13
#define Slide_West			0x14
#define Exit				0x15
#define Door_Blue			0x16
#define Door_Red			0x17
#define Door_Green			0x18
#define Door_Yellow			0x19
#define IceWall_Southeast	0x1A
#define IceWall_Southwest	0x1B
#define IceWall_Northwest	0x1C
#define IceWall_Northeast	0x1D
#define BlueWall_Fake		0x1E
#define BlueWall_Real		0x1F

#define Thief				0x21
#define Socket				0x22
#define Button_Green		0x23
#define Button_Red			0x24
#define SwitchWall_Closed	0x25
#define SwitchWall_Open		0x26
#define Button_Brown		0x27
#define Button_Blue			0x28
#define Teleport			0x29
#define Bomb				0x2A
#define Beartrap			0x2B
#define HiddenWall_Temp		0x2C
#define Gravel				0x2D
#define PopupWall			0x2E
#define HintButton			0x2F
#define Wall_Southeast		0x30
#define CloneMachine		0x31
#define Slide_Random		0x32

#define Chip_Drowned		0x33
#define Chip_Burned			0x34
#define Chip_Bombed			0x35

#define Exited_Chip			0x39
#define Exit_Extra_1		0x3A
#define Exit_Extra_2		0x3B

#define Block				0xD0
#define Chip_Swimming		0x3C
#define Bug					0x40
#define Fireball			0x44
#define Ball				0x48
#define Tank				0x4C
#define	Glider				0x50
#define Teeth				0x54
#define Walker				0x58
#define Blob				0x5C
#define Paramecium			0x60

#define Key_Blue			0x64
#define Key_Red				0x65
#define Key_Green			0x66
#define Key_Yellow			0x67

#define Boots_Water			0x68
#define Boots_Fire			0x69
#define Boots_Ice			0x6A
#define Boots_Slide			0x6B

#define Chip				0x6C

#define Nothing				0xFF