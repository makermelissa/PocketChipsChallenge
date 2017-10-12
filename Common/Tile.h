#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <windows.h>
#include "defs.h"

class CTile
{
public:
	CTile() { ID = Empty; State = 0; };
	~CTile() {};
	void SetID(int value) { ID = value; };
	int GetID() { return ID; };
	int GetState() { return State; };
	void SetState(int value) { State = value; };
private:
	int ID;
	int State;
};