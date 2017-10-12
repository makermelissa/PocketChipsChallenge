#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <windows.h>
#include "defs.h"

class CElement
{
public:
	CElement() {};
	CElement(int Walkable[3]) {
		nChipWalk = Walkable[0];
		nBlockMove = Walkable[1];
		nCreatureWalk = Walkable[2];
	};
	void SetWalk(int nChWalkValue, int nBlWalkValue, int nCrWalkValue) {
		nChipWalk = nChWalkValue;
		nBlockMove = nBlWalkValue;
		nCreatureWalk = nCrWalkValue;
	};
	void SetChipWalk(int nWalkValue) { nChipWalk = nWalkValue; };
	int GetChipWalk() { return nChipWalk; };
	void SetBlockMove(int nWalkValue) { nBlockMove = nWalkValue; };
	int GetBlockMove() { return nBlockMove; };
	void SetCreatureWalk(int nWalkValue) { nCreatureWalk = nWalkValue; };
	int GetCreatureWalk() { return nCreatureWalk; };
    ~CElement() {};

private:
	int nChipWalk;
	int nBlockMove;
	int nCreatureWalk;
};