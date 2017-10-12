#include "creature.h"
#include "defs.h"

void CCreature::Move(POINT ptDest)
{
	if(ptDest.y < ptCurPos.y)
		Dir = NORTH;
	else if(ptDest.x < ptCurPos.x)
		Dir = WEST;
	else if(ptDest.y > ptCurPos.y)
		Dir = SOUTH;
	else if(ptDest.x > ptCurPos.x)
		Dir = EAST;
	else
		Dir = NONE;
	ptCurPos = ptDest;
}


int CCreature::ImageNum()
{
	int ImageNum = 0;
	switch(nType)
	{
		case Chip:		ImageNum = 0x6c; break;
		case Block:		return 0x0A;
		default:		ImageNum = 0x40 + ((nType - 1) * 4); break;
	}
	switch(Dir)
	{
		case WEST:	ImageNum += LEFT;	break;
		case EAST:	ImageNum += RIGHT;	break;
		case NORTH:	ImageNum += UP;		break;
		case SOUTH:
		case NONE:	ImageNum += DOWN;	break;
	}

	if(nType == Chip)
	{
	/*	if(nStatus & CS_SWIMMING)
			ImageNum -= 0x30;
		else if(nStatus & CS_DROWNED)
			ImageNum = Chip_Drowned;
		else if(nStatus & CS_BURNED)
			ImageNum = Chip_Burned;
		else if(nStatus & CS_BOMBED)
			ImageNum = Chip_Bombed;*/
	}

	return ImageNum;
}


POINT CCreature::GetTileInDir(int Direction)
{
	POINT ptDir = ptCurPos;
	switch(Direction)
	{
		case WEST:	ptDir.x--;	break;
		case EAST:	ptDir.x++;	break;
		case NORTH:	ptDir.y--;	break;
		case SOUTH:	ptDir.y++;	break;
	}
	return ptDir;
}

POINT CCreature::Left()
{
	POINT ptDest = ptCurPos;
	switch(Dir)
	{
		case NORTH:	ptDest.x--;	break;
		case WEST:	ptDest.y++;	break;
		case SOUTH:	ptDest.x++;	break;
		case EAST:	ptDest.y--;	break;
	}
	return ptDest;
}

POINT CCreature::Right()
{
	POINT ptDest = ptCurPos;
	switch(Dir)
	{
		case NORTH:	ptDest.x++;	break;
		case WEST:	ptDest.y--;	break;
		case SOUTH:	ptDest.x--;	break;
		case EAST:	ptDest.y++;	break;
	}
	return ptDest;
}

POINT CCreature::Back()
{
	POINT ptDest = ptCurPos;
	switch(Dir)
	{
		case NORTH:	ptDest.y++;	break;
		case WEST:	ptDest.x++;	break;
		case SOUTH:	ptDest.y--;	break;
		case EAST:	ptDest.x--;	break;
	}
	return ptDest;
}

POINT CCreature::Front()
{
	POINT ptDest = ptCurPos;
	switch(Dir)
	{
		case NORTH:	ptDest.y--;	break;
		case WEST:	ptDest.x--;	break;
		case SOUTH:	ptDest.y++;	break;
		case EAST:	ptDest.x++;	break;
	}
	return ptDest;
}

int CCreature::Reverse()
{
	switch(Dir)
	{
		case NORTH:	return SOUTH;
		case SOUTH:	return NORTH;
		case WEST:	return EAST;
		case EAST:	return WEST;
		default:	return Dir;
	}
}