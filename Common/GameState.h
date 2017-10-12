#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "level.h"
#include "creature.h"
#include "element.h"
#include "defs.h"
#include "tile.h"
#include "slip.h"
#include <windows.h>

class CGameState
{
public:
	CGameState()
	{
		CreateTileset();
		nGameMode = GM_NONE;
		nCurrentLevel = 1;
		nLastLevel = 149;
		GetChip()->SetType(0);
		SlipList = new CSlip;
		SlipListEnd = SlipList;
		nSlipCount = 0;
		CreaturePool = new CCreature;
		CreaturePoolEnd = CreaturePool;
		nCreatureCount = 0;
		BlockPool = new CCreature;
		BlockPoolEnd = BlockPool;
		nBlockCount = 0;
	};

	BOOL GetGameInProgress() { return bGameInProgress; };
	void SetGameInProgress(BOOL Value) { bGameInProgress = Value; };
	BOOL CheckForEnding();
	void StartFloorMovement(CCreature* cr, int Floor);
	void EndFloorMovement(CCreature* cr);
	void FloorMovements();
	int Random4();
	int IceWallTurn(int Floor, int Dir);
	int GetSlideDir(int Floor);
	BOOL IsTrapOpen(POINT ptPos);
	void Finalize();
	POINT ClonerFromButton(POINT ptPos);
	POINT TrapFromButton(POINT ptPos);
	void HandleButtons();
	void ResetButtons();
	void AddCreatureToMap(CCreature* cr);
	void UpdateCreature(CCreature* cr);
	void ActivateCloner(POINT ptPos);
	void SpringTrap(POINT ptPos);
	BOOL EqualPoints(POINT pos1, POINT pos2) { return (pos1.x == pos2.x) && (pos1.y == pos2.y); };
	BOOL StartMovement(CCreature* cr, int Dir);
	void EndMovement(CCreature* cr, int Dir);
	void Reset();
	POINT TeleportCreature(CCreature* cr, POINT ptPos);
	int GetControllerDir(CCreature* cr);
	void AddKey(int KeyNumber) { nKeys[KeyNumber]++; };
	void RemKey(int KeyNumber) { nKeys[KeyNumber]--; };
	void Collect(int Obj);
	void Discard(int Obj);
	int GetKey(int KeyNumber) { return nKeys[KeyNumber]; };
	void SetBoots(int BootsNumber, BOOL BootsValue) { bBoots[BootsNumber] = BootsValue; };
	BOOL GetBoots(int BootsNumber) { return bBoots[BootsNumber]; };
	void DecChips() { if(ChipsNeeded) ChipsNeeded--; };
	int GetChipsNeeded() { return ChipsNeeded; };
	BOOL DecLevel();
	BOOL IncLevel();
	BOOL SetLevel(int Level);
	POINT GetViewPort();
	CLevel* GetCurrentLevel() { return &CurrentLevel; };
	CCreature* GetChip() { return &crChip; };
	CElement* GetTiles() { return &Tileset[0]; };
	void SetChipXY(POINT ptTile);
	void SetChipDir(int Dir);
	int GetChipWalkCmd();
	void MoveChip(int Dir);
	int GetButton() { return nCurrentInput; };
	void SetButton(int Button) { nCurrentInput = Button; };
	BOOL GetMoving() {return bMoving; };
	void SetMoving(BOOL bValue) { bMoving = bValue; };
	void CreateTileset();
	void ExtractCreatures();
	BOOL CanMakeMove(CCreature* cr, int Dir, int Flags);

	//Functions to maintain Creature List
	int GetCreatureCount() { return nCreatureCount; };
	CCreature* AddCreature(POINT ptTile, int Dir, int Type);
	void RemoveCreature(CCreature* cr);
	void RemoveAllCreatures();
	void RemoveDeadCreatures();
	CCreature* GetCreature(POINT ptPos, BOOL IncludeChip);
	
	//Functions to maintain Slip List
	CCreature* AppendToSlipList(CCreature* cr, int Dir);
	CCreature* PrependToSlipList(CCreature* cr, int Dir);
	void RemoveAllSlips();
	void RemoveSlip(CSlip* Slip);
	void RemoveFromSlipList(CCreature* cr);
	int GetSlipCount() { return nSlipCount; };
	int GetSlipDir(CCreature* cr);

	//Functions to maintain Block List
	int GetBlockCount() { return nBlockCount; };
	CCreature* AddBlock(POINT ptTile, int Dir, int Type);
	void RemoveBlock(CCreature* cr);
	void RemoveAllBlocks();
	void RemoveDeadBlocks();
	CCreature* GetBlock(POINT ptPos);

	long GetTimeLimit() { return TimeLimit; };
	void MoveCreatures();
	void ToggleTanks(CCreature* InMidMove);
	void RandomP(int* array, int PLevel);
	void MoveBlock(CCreature* cr, POINT ptPos);
	BOOL SomeoneAt(POINT xyPoint);
	void SetGameMode(int Mode);
	void SetStatus(int Status) { nStatus = Status; };
	int GetStatus() { return nStatus; };
	void RevertGameMode() { SetGameMode(nPrevMode); nPrevMode = nPrevMode2;};
	int GetGameMode() { return nGameMode; };
	void IncTick() { CurrentTime++; };
	long GetTick() { return CurrentTime; };
	void ResetTick() { CurrentTime = 0; };
	int FloorAt(POINT ptTile);
	void SetFloorAt(POINT ptTile, int Tile);
	void SetLastMove(int Move) { nLastMove = Move; };
	int GetLastMove() { return nLastMove; };
	BOOL IsKey(int Tile) {return ((Tile >= Key_Blue) && (Tile <= Key_Yellow)); };
	BOOL IsBoots(int Tile) {return ((Tile >= Boots_Water) && (Tile <= Boots_Slide)); };
	BOOL IsCreature(int Tile) {return ((Tile >= 0x40) && (Tile <= 0x63)) || ((Tile >= Block) && (Tile <= Block + 3)) || ((Tile >= Chip_Swimming) && (Tile <= Chip_Swimming + 3)) || ((Tile >= Chip) && (Tile <= Chip + 3)); };
	BOOL IsDoor(int Tile) {return ((Tile >= Door_Blue) && (Tile <= Door_Yellow)); };
	BOOL IsIce(int Tile);
	BOOL IsSlide(int Tile);
	BOOL IsOccupied(POINT ptPos);
	BOOL PushBlock(POINT ptPos, int Dir, int Collapse);
	BOOL AdvanceCreature(CCreature *pCreature, int Dir);
	void ChooseChipMove(CCreature *pCreature, int Discard);
	void ChooseCreatureMove(CCreature *pCreature);
	BOOL Possession(int Obj);
	void AdvanceGame(int Cmd);
	void Prepare();
	void CreateClones();
	CTile MakeTile(int TileID, int State);
	~CGameState() 
	{ 
		RemoveAllSlips();
		delete SlipList;
		RemoveAllCreatures(); 
		delete CreaturePool; 
		RemoveAllBlocks();
		delete BlockPool;
	};

private:
	int DeadBlocks;
	int DeadCreatures;
	CElement Tileset[0x70];
	int nStatus;
	BOOL bGameInProgress;
	BOOL bMoving;
	int nLastMove;
	long TimeLimit;
	int ChipsNeeded;
	long CurrentTime;
	int nCurrentInput;
	CLevel CurrentLevel;
	CCreature crChip;
	int nLastLevel;
	int nCurrentLevel;
	int nKeys[4];
	BOOL bBoots[4];
	CCreature* CreaturePool;
	CCreature* CreaturePoolEnd;
	CCreature* BlockPool;
	CCreature* BlockPoolEnd;
	CSlip* SlipList;
	CSlip* SlipListEnd;
	int nSlipCount;
	int nCreatureCount;
	int nBlockCount;
	int nGameMode, nPrevMode, nPrevMode2;
	int nTick;
};