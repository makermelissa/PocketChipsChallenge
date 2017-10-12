#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <windows.h>
#include "defs.h"

class CCreature
{
public:
	CCreature() { 
		nState = 0x00;
		prevCreature = NULL;
		nextCreature = NULL;
		ptCurPos.x = 0;
		ptCurPos.y = 0;
		Dir = NONE;
		nType = Block;
		bHidden = FALSE;
		OnSlipList = FALSE;
		};
	void SetPos(POINT ptTile) {	ptCurPos = ptTile; };
	POINT GetPos() { return ptCurPos; };
	int GetDirection() { return Dir; };
	void SetDirection(int NewDir) { Dir = NewDir; };
	int GetType() { return nType; };
	void SetType(int Type) { nType = Type; };
	void SetWalking(BOOL bValue) { bWalking = bValue; };
	BOOL GetWalking() { return bWalking; };
	void SetHidden(BOOL bValue) { bHidden = bValue; };
	BOOL GetHidden() { return bHidden; };
	void SetState(int State) { nState = State; };
	int GetState() { return nState; };
	CCreature* GetNextCreature() { return nextCreature; };
	CCreature* GetPrevCreature() { return prevCreature; };
	void SetNextCreature(CCreature* Next) { nextCreature = Next; };
	void SetPrevCreature(CCreature* Prev) { prevCreature = Prev; };
	void Move(POINT ptDest);
	void SetDestination(POINT ptTile) { ptDestination = ptTile; };
	int GetToDir() { return nToDir; };
	void SetToDir(int Dir) { nToDir = Dir; };
	BOOL GetOnSlipList() { return OnSlipList; };
	void SetOnSlipList(BOOL OnList) { OnSlipList = OnList; };
	POINT GetDestination() { return ptDestination; };
	POINT Left();
	POINT Right();
	POINT Back();
	POINT Front();
	POINT GetTileInDir(int Direction);
	int Reverse();
	int ImageNum();
    ~CCreature() {};

private:
	int nToDir;
	int nType;
	int Dir;
	int nState;
	POINT ptCurPos;
	POINT ptDestination;
	BOOL bWalking;
	BOOL bHidden;
	BOOL OnSlipList;

	CCreature* prevCreature;
	CCreature* nextCreature;
};