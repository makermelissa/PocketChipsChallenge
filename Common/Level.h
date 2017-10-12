#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cell.h"
#include "device.h"
#include <tchar.h>
#include <windows.h>

struct XYCONN
{
	POINT Button;
	POINT Device;
};

struct CHIPSFILEHEADER
{
	DWORD FileSize;
	WORD NumberOfLevels;
};

struct CHIPSLEVELHEADER
{
	WORD LevelSize;
	WORD LevelNumber;
	WORD TimeLimit;
	WORD ChipsToCollect;
};

class CLevel
{
public:
	CLevel() { bLoading = FALSE; wsprintf(strFileName, TEXT("")); };
	void SetLevel(int Level) { nLevelNumber = Level; };
	int GetLevel() { return nLevelNumber; };
	void SetTime(int Time) { nTime = Time; };
	int GetTime() { return nTime; };
	void SetChips(int Chips) { nChips = Chips; };
	int GetChips() { return nChips; };
	void SetBestTime(int BestTime) { nBestTime = BestTime; };
	int GetBestTime() { return nBestTime; };
	void SetTitle(LPCTSTR strTitle) {wsprintf(title, strTitle); };
	POINT GetCreature(int CreatureNum) { return creatures[CreatureNum]; };
	int GetCreatureCount() { return nCreatureCount; };
	CDevice *GetTrap(int TrapNum) { return &Traps[TrapNum]; };
	int GetTrapCount() { return nTrapCount; };
	CDevice *GetCloner(int ClonerNum) { return &Cloners[ClonerNum]; };
	int GetClonerCount() { return nClonerCount; };
	LPCTSTR GetTitle() { return title; };
	void SetHint(LPCTSTR strHint) {
		TCHAR strTemp[262];
		_tcscpy(strTemp, TEXT("Hint:\n"));
		_tcscat(strTemp, strHint);
		wsprintf(hint, strTemp); };
	LPCTSTR GetHint() { return hint; };
	void SetPassword(LPCTSTR strPassword) {wsprintf(passwd, strPassword); };
	LPCTSTR GetPassword() { return passwd; };
	void SetFileName(LPCTSTR FileName) {wsprintf(strFileName, FileName); };
	LPCTSTR GetFileName() { return strFileName; };
	BOOL LoadLevelData();
	BOOL GetNoLimit() { return bNoLimit; };
	CCell *GetCell(POINT xyPoint) { return &LevelMap[xyPoint.x][xyPoint.y]; };
	int GetLastLevel() { return nLastLevel; };
	BOOL GetLoadStatus() { return bLoading; };
	void SetLoadStatus(BOOL bStatus) { bLoading = bStatus; };
	LPCTSTR GetPath(TCHAR *filename);
	void PushTile(POINT ptPos, CTile Tile);
	CTile PopTile(POINT ptPos);
	~CLevel();
	int nChipX, nChipY;
	int nChipDir;
	void ToggleBlocks();
private:
	BOOL bLoading;
	BOOL bNoLimit;
	int nLevelNumber;
	int nTime;
	int nChips;
	int nBestTime;
	WORD sgFlags;
	int	nMapSize1;
	int nMapSize2;
	CCell LevelMap[32][32];
	int nCreatureCount;
	int nTrapCount;
	int nClonerCount;
	int nLastLevel;
	TCHAR title[256];
	TCHAR passwd[256];
	TCHAR hint[256];
	CDevice Traps[256];
	CDevice Cloners[256];
	POINT creatures[256];
	TCHAR strFileName[1024];
};

LPCTSTR GetPath(TCHAR *filename);