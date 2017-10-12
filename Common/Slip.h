#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <windows.h>
#include "creature.h"
#include "defs.h"

class CSlip
{
public:
	CSlip() { cr = NULL; Dir = NONE; NextSlip = NULL; PrevSlip = NULL;};
	~CSlip() {};
	void SetCr(CCreature* Creature) { cr = Creature; };
	CCreature* GetCr() { return cr; };
	int GetDir() { return Dir; };
	void SetDir(int value) { Dir = value; };
	CSlip* GetNextSlip() { return NextSlip; };
	CSlip* GetPrevSlip() { return PrevSlip; };
	void SetNextSlip(CSlip* Next) { NextSlip = Next; };
	void SetPrevSlip(CSlip* Prev) { PrevSlip = Prev; };

private:
	CSlip* NextSlip;
	CSlip* PrevSlip;
	CCreature* cr;
	int Dir;
};