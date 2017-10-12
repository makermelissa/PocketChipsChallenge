#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <windows.h>
#include "tile.h"

class CCell
{
public:
	CCell() {};
	~CCell() {};
	CTile *GetTop() { return &Top; };
	CTile *GetBottom() { return &Bottom; };
private:
	CTile Top;
	CTile Bottom;
};