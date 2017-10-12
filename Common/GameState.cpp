#include "gamestate.h"
#include <math.h>

#define IsCompleted()		(GetStatus() & SF_COMPLETED)
#define SetCompleted()		(SetStatus(GetStatus() | SF_COMPLETED))
#define ShowHint()			(SetStatus(GetStatus() | SF_SHOWHINT))
#define HideHint()			(SetStatus(GetStatus() & ~SF_SHOWHINT))

#define SetDeferButtons()	(SetStatus(GetStatus() | SF_DEFERBUTTONS))
#define ResetDeferButtons()	(SetStatus(GetStatus() & ~SF_DEFERBUTTONS))
#define ButtonsDeferred()	(GetStatus() & SF_DEFERBUTTONS)

#define GetChipStatus()		(GetStatus() & SF_CHIPSTATUSMASK)
#define SetChipStatus(s)	(SetStatus((GetStatus() & ~SF_CHIPSTATUSMASK) | (s)))

#define ResetChipWait()		(SetStatus(GetStatus() & ~SF_CHIPWAITMASK))
#define GetChipWait()		(GetStatus() & SF_CHIPWAITMASK)
#define IncrChipWait()		(SetStatus((GetStatus() & ~SF_CHIPWAITMASK) | ((GetStatus() & SF_CHIPWAITMASK) + 1)))

#define CellAt(p)			(CurrentLevel.GetCell(p))

BOOL CGameState::DecLevel()
{
	if (nCurrentLevel > 1)
		SetLevel(--nCurrentLevel);
	return TRUE;
}

BOOL CGameState::IncLevel()
{
	if (nCurrentLevel < nLastLevel)
		SetLevel(++nCurrentLevel);
	return TRUE;
};

BOOL CGameState::SetLevel(int Level)
{
	Reset();
	SetGameMode(GM_LOADING);
	nCurrentLevel = Level;
	CurrentLevel.SetLevel(Level);
	if(!CurrentLevel.LoadLevelData()) 
		return FALSE;
	nLastLevel = CurrentLevel.GetLastLevel();
	TimeLimit = CurrentLevel.GetTime() * TICKS_PER_SECOND;
	ChipsNeeded = CurrentLevel.GetChips();
	ExtractCreatures();
	SetGameMode(GM_NORMAL);
	return TRUE;
}


BOOL CGameState::Possession(int Obj)
{
	switch(Obj)
	{
		case Key_Blue:				return GetKey(BlueKey);
		case Key_Red:				return GetKey(RedKey);
		case Key_Yellow:			return GetKey(YellowKey);
		case Key_Green:				return GetKey(GreenKey);
		case Door_Blue:				return GetKey(BlueKey);
		case Door_Red:				return GetKey(RedKey);
		case Door_Yellow:			return GetKey(YellowKey);
		case Door_Green:			return GetKey(GreenKey);
		case Boots_Water:			return GetBoots(WaterBoots);
		case Boots_Fire:			return GetBoots(FireBoots);
		case Boots_Ice:				return GetBoots(IceBoots);
		case Boots_Slide:			return GetBoots(SuctionBoots);
		case Ice:					return GetBoots(IceBoots);
		case IceWall_Northwest:		return GetBoots(IceBoots);
		case IceWall_Northeast:		return GetBoots(IceBoots);
		case IceWall_Southwest:		return GetBoots(IceBoots);
		case IceWall_Southeast:		return GetBoots(IceBoots);
		case Slide_North:			return GetBoots(SuctionBoots);
		case Slide_West:			return GetBoots(SuctionBoots);
		case Slide_South:			return GetBoots(SuctionBoots);
		case Slide_East:			return GetBoots(SuctionBoots);
		case Slide_Random:			return GetBoots(SuctionBoots);
		case Fire:					return GetBoots(FireBoots);
		case Water:					return GetBoots(WaterBoots);
		default:					return FALSE;
	}
}

void CGameState::Collect(int Obj)
{
	switch(Obj)
	{
		case Key_Blue:				AddKey(BlueKey);				break;
		case Key_Red:				AddKey(RedKey);					break;
		case Key_Yellow:			AddKey(YellowKey);				break;
		case Key_Green:				AddKey(GreenKey);				break;
		case Door_Blue:				AddKey(BlueKey);				break;
		case Door_Red:				AddKey(RedKey);					break;
		case Door_Yellow:			AddKey(YellowKey);				break;
		case Door_Green:			AddKey(GreenKey);				break;
		case Boots_Water:			SetBoots(WaterBoots, TRUE);		break;
		case Boots_Fire:			SetBoots(FireBoots, TRUE);		break;
		case Boots_Ice:				SetBoots(IceBoots, TRUE);		break;
		case Boots_Slide:			SetBoots(SuctionBoots, TRUE);	break;
		case Ice:					SetBoots(IceBoots, TRUE);		break;
		case IceWall_Northwest:		SetBoots(IceBoots, TRUE);		break;
		case IceWall_Northeast:		SetBoots(IceBoots, TRUE);		break;
		case IceWall_Southwest:		SetBoots(IceBoots, TRUE);		break;
		case IceWall_Southeast:		SetBoots(IceBoots, TRUE);		break;
		case Slide_North:			SetBoots(SuctionBoots, TRUE);	break;
		case Slide_West:			SetBoots(SuctionBoots, TRUE);	break;
		case Slide_South:			SetBoots(SuctionBoots, TRUE);	break;
		case Slide_East:			SetBoots(SuctionBoots, TRUE);	break;
		case Slide_Random:			SetBoots(SuctionBoots, TRUE);	break;
		case Fire:					SetBoots(FireBoots, TRUE);		break;
		case Water:					SetBoots(WaterBoots, TRUE);		break;
	}
}

void CGameState::Discard(int Obj)
{
	switch(Obj)
	{
		case Key_Blue:				RemKey(BlueKey);					break;
		case Key_Red:				RemKey(RedKey);						break;
		case Key_Yellow:			RemKey(YellowKey);					break;
		case Door_Blue:				RemKey(BlueKey);					break;
		case Door_Red:				RemKey(RedKey);						break;
		case Door_Yellow:			RemKey(YellowKey);					break;
		case Boots_Water:			SetBoots(WaterBoots, FALSE);		break;
		case Boots_Fire:			SetBoots(FireBoots, FALSE);			break;
		case Boots_Ice:				SetBoots(IceBoots, FALSE);			break;
		case Boots_Slide:			SetBoots(SuctionBoots, FALSE);		break;
		case Ice:					SetBoots(IceBoots, FALSE);			break;
		case IceWall_Northwest:		SetBoots(IceBoots, FALSE);			break;
		case IceWall_Northeast:		SetBoots(IceBoots, FALSE);			break;
		case IceWall_Southwest:		SetBoots(IceBoots, FALSE);			break;
		case IceWall_Southeast:		SetBoots(IceBoots, FALSE);			break;
		case Slide_North:			SetBoots(SuctionBoots, FALSE);		break;
		case Slide_West:			SetBoots(SuctionBoots, FALSE);		break;
		case Slide_South:			SetBoots(SuctionBoots, FALSE);		break;
		case Slide_East:			SetBoots(SuctionBoots, FALSE);		break;
		case Slide_Random:			SetBoots(SuctionBoots, FALSE);		break;
		case Fire:					SetBoots(FireBoots, FALSE);			break;
		case Water:					SetBoots(WaterBoots, FALSE);		break;
	}
}


int CGameState::GetSlideDir(int Floor)
{
	switch(Floor)
	{
		case Slide_North:	return NORTH;
		case Slide_West:	return WEST;
		case Slide_South:	return SOUTH;
		case Slide_East:	return EAST;
		case Slide_Random:	return 1 << Random4();
	}
	return NONE;
}


int CGameState::IceWallTurn(int Floor, int Dir)
{
	switch(Floor)
	{
		case IceWall_Northeast:
			return Dir == SOUTH ? EAST : Dir == WEST ? NORTH : Dir;
		case IceWall_Southwest:
			return Dir == NORTH ? WEST : Dir == EAST ? SOUTH : Dir;
		case IceWall_Northwest:
			return Dir == SOUTH ? WEST : Dir == EAST ? NORTH : Dir;
		case IceWall_Southeast:
			return Dir == NORTH ? EAST : Dir == WEST ? SOUTH : Dir;
	}
	return Dir;
}

//This function lines up the edge of the map to the edge of the screen
POINT CGameState::GetViewPort()
{
	POINT ptTile = GetChip()->GetPos();
	if(ptTile.x <= 4)
		ptTile.x = 4;
	else if(ptTile.x >= 27)
		ptTile.x = 27;
	if(ptTile.y <= 4)
		ptTile.y = 4;
	else if(ptTile.y >= 27)
		ptTile.y = 27;
	return ptTile;
}

BOOL CGameState::CanMakeMove(CCreature* cr, int Dir, int Flags)
{
	int nFloor, nType, nBottomTile, nTopTile, id;
	POINT ptTo = cr->GetTileInDir(Dir);
	if(ptTo.x < 0 || ptTo.y < 0 || ptTo.x > 31 || ptTo.y > 31) return FALSE;

	if(!(Flags & CMM_NOLEAVECHECK))
	{
		nBottomTile = CellAt(cr->GetPos())->GetBottom()->GetID();
		switch(nBottomTile)
		{
			case Wall_North:	if(Dir == NORTH)	return FALSE; break;
			case Wall_West:		if(Dir == WEST)		return FALSE; break;
			case Wall_South:	if(Dir == SOUTH)	return FALSE; break;
			case Wall_East:		if(Dir == EAST)		return FALSE; break;
			case Wall_Southeast:
				if((Dir == SOUTH) || (Dir == EAST))
					return FALSE;
				break;
			case Beartrap:
				if(!(cr->GetState() & CS_RELEASED))
					return FALSE;
				break;
		}
	}

	nFloor = FloorAt(ptTo);
	nType = cr->GetType();
	nTopTile = CellAt(ptTo)->GetTop()->GetID();

	if(nType == Chip)
	{
		if(!(Tileset[nFloor].GetChipWalk() & Dir))
			return FALSE;
		if((nFloor == Socket) && (ChipsNeeded > 0))
			return FALSE;
		if(IsDoor(nFloor) && !Possession(nFloor))
			return FALSE;
		if(IsCreature(nTopTile))
		{
			id = creatureid(nTopTile);
			if(id == Chip || id == Chip_Swimming || id == Block)
				return FALSE;
		}
		if(nFloor == HiddenWall_Temp || nFloor == BlueWall_Real)
		{
			if(!(Flags & CMM_NOEXPOSEWALLS))
				SetFloorAt(ptTo, Wall);
			return FALSE;
		}
		if(nFloor == Block_Static)
		{
			if(PushBlock(ptTo, Dir, !(Flags & CMM_NOCOLLAPSEBLOCKS)))
				return CanMakeMove(cr, Dir, Flags);
			else
				return FALSE;
		}
	}
	else if(nType == Block)
	{
		if(IsCreature(nTopTile))
		{
			id = creatureid(nTopTile);
			return id == Chip || id == Chip_Swimming;
		}
		if(!(Tileset[nFloor].GetBlockMove() & Dir))
			return FALSE;
	}
	else
	{
		if(!(Tileset[nFloor].GetCreatureWalk() & Dir))
			return FALSE;
		if(SomeoneAt(ptTo))
		{
			if(!(Flags & CMM_CLONECANTBLOCK))
				return FALSE;
			if(nTopTile != crtile(nType, cr->GetDirection()))
				return FALSE;
		}
		if(IsBoots(nTopTile))
			return FALSE;
		if((nFloor == Fire) && ((nType == Bug) || (nType == Walker)))
			return FALSE;
	}

	if(CellAt(ptTo)->GetBottom()->GetID() == CloneMachine)
		return FALSE;

	return TRUE;
}

BOOL CGameState::PushBlock(POINT ptPos, int Dir, int Collapse)
{
	CCreature* cr;
	int SlipDir;
	cr = GetBlock(ptPos);
	if(cr == NULL)
		return FALSE;
	if(cr->GetState() & (CS_SLIP | CS_SLIDE))
	{
		SlipDir = GetSlipDir(cr);
		if(Dir == SlipDir || Dir == back(SlipDir))
			return FALSE;
		EndFloorMovement(cr);
	}
	if(Collapse && CurrentLevel.GetCell(ptPos)->GetBottom()->GetID() == Block_Static)
		CurrentLevel.GetCell(ptPos)->GetBottom()->SetID(Empty);
	return AdvanceCreature(cr, Dir);
}


CTile CGameState::MakeTile(int TileID, int State)
{
	CTile Tile;
	Tile.SetID(TileID);
	Tile.SetState(State);
	return Tile;
}

void CGameState::UpdateCreature(CCreature* cr)
{
	CTile*	Tile;
	int		nType, dir;

	if(cr->GetHidden())
		return;
	Tile = CellAt(cr->GetPos())->GetTop();
	nType = cr->GetType();
	if(nType == Block)
	{
		Tile->SetID(Block_Static);
		return;
	}
	else if(nType == Chip)
	{
		if(GetChipStatus())
		{
			switch(GetChipStatus())
			{
				case SF_CHIPBURNED:		Tile->SetID(Chip_Burned);	return;
				case SF_CHIPDROWNED:	Tile->SetID(Chip_Drowned);	return;
				case SF_CHIPBOMBED:		Tile->SetID(Chip_Bombed);	return;
			}
		}
		else if(CellAt(cr->GetPos())->GetBottom()->GetID() == Water)
		{
			nType = Chip_Swimming;
		}
	}

	dir = cr->GetDirection();
	if(cr->GetState() & CS_TURNING)
		dir = right(dir);
	Tile->SetID(crtile(nType, dir));
	Tile->SetState(0);
}

void CGameState::ResetButtons()
{
	POINT ptPos;

	for(ptPos.x=0; ptPos.x<32; ptPos.x++)
	{
		for(ptPos.y=0; ptPos.y<32; ptPos.y++)
		{
			CellAt(ptPos)->GetTop()->SetState(CellAt(ptPos)->GetTop()->GetState() & ~FS_BUTTONDOWN);
			CellAt(ptPos)->GetBottom()->SetState(CellAt(ptPos)->GetBottom()->GetState() & ~FS_BUTTONDOWN);
		}
	}
}

void CGameState::HandleButtons()
{
	POINT ptPos;
	int id;

	for(ptPos.x=0; ptPos.x<32; ptPos.x++)
	{
		for(ptPos.y=0; ptPos.y<32; ptPos.y++)
		{
			if(CellAt(ptPos)->GetTop()->GetState() & FS_BUTTONDOWN)
			{
				CellAt(ptPos)->GetTop()->SetState(CellAt(ptPos)->GetTop()->GetState() & ~FS_BUTTONDOWN);
				id = CellAt(ptPos)->GetTop()->GetID();
			}
			else if(CellAt(ptPos)->GetBottom()->GetState() & FS_BUTTONDOWN)
			{
				CellAt(ptPos)->GetBottom()->SetState(CellAt(ptPos)->GetBottom()->GetState() & ~FS_BUTTONDOWN);
				id = CellAt(ptPos)->GetBottom()->GetID();
			}
			else
				continue;

			switch(id)
			{
				case Button_Blue:
					//SOUND EFFECT **[BUTTON PUSHED]**
					ToggleTanks(NULL);
					break;
				case Button_Green:
					CurrentLevel.ToggleBlocks();
					break;
				case Button_Red:
					//SOUND EFFECT **[BUTTON PUSHED]**
					ActivateCloner(ptPos);
					break;
				case Button_Brown:
					//SOUND EFFECT **[BUTTON PUSHED]**
					SpringTrap(ptPos);
					break;
			}
		}
	}	
}


BOOL CGameState::IsIce(int Tile)
{
	return Tile == Ice || (Tile >= IceWall_Southeast && Tile <= IceWall_Northeast);
}


BOOL CGameState::IsSlide(int Tile)
{
	return Tile == Slide_South || Tile == Slide_Random || (Tile >= Slide_North && Tile <= Slide_West);
}


void CGameState::StartFloorMovement(CCreature* cr, int Floor)
{
	int Dir;
	cr->SetState(cr->GetState() & ~(CS_SLIP | CS_SLIDE));

	if(IsIce(Floor))
		Dir = IceWallTurn(Floor, cr->GetDirection());
	else if(IsSlide(Floor))
		Dir = GetSlideDir(Floor);
	else if(Floor == Teleport)
		Dir = cr->GetDirection();
	else if(Floor == Beartrap && cr->GetType() == Block)
		Dir = cr->GetDirection();
	else
		return;

	if(cr->GetType() == Chip)
	{
		cr->SetState(cr->GetState() | (IsSlide(Floor) ? CS_SLIDE : CS_SLIP));
		PrependToSlipList(cr, Dir);
		cr->SetDirection(Dir);
		UpdateCreature(cr);
	}
	else
	{
		cr->SetState(cr->GetState() | CS_SLIP);
		AppendToSlipList(cr, Dir);
	}
}


void CGameState::EndFloorMovement(CCreature* cr)
{
	cr->SetState(cr->GetState() & ~(CS_SLIP | CS_SLIDE));
	RemoveFromSlipList(cr);
}


void CGameState::FloorMovements()
{
	CCreature*	cr;
	CSlip*		Slip;
	int			nFloor, SlipDir;

	if(nSlipCount > 0)
	{
		Slip = SlipList->GetNextSlip();
		for(;;)
		{
			cr = Slip->GetCr();
			SlipDir = Slip->GetDir();
			if((cr->GetState() & (CS_SLIP | CS_SLIDE)) && (SlipDir != NONE))
			{
				if(AdvanceCreature(cr, SlipDir))
				{
					if(cr->GetType() == Chip)
						cr->SetState(cr->GetState() & ~CS_HASMOVED);
				}
				else
				{
					nFloor = CellAt(cr->GetPos())->GetBottom()->GetID();
					if(IsIce(nFloor) || (nFloor == Teleport && cr->GetType() == Chip))
					{
						SlipDir = IceWallTurn(nFloor, back(SlipDir));
						if(AdvanceCreature(cr, SlipDir))
						{
							if(cr->GetType() == Chip)
								cr->SetState(cr->GetState() & ~CS_HASMOVED);
						}
					}
					else if(IsSlide(nFloor))
					{
						if(cr->GetType() == Chip)
							cr->SetState(cr->GetState() & ~CS_HASMOVED);
					}
					if(cr->GetState() & (CS_SLIP | CS_SLIDE))
					{
						EndFloorMovement(cr);
						StartFloorMovement(cr, CellAt(cr->GetPos())->GetBottom()->GetID());
					}
				}
				if(CheckForEnding())
					return;
			}
			if(Slip == SlipListEnd)
				break;
			Slip = Slip->GetNextSlip();
		}
	}

 	if(nSlipCount > 0)
	{
		Slip = SlipListEnd;
		for(;;)
		{
			if(!(Slip->GetCr()->GetState() & (CS_SLIP | CS_SLIDE)))
			{
				EndFloorMovement(Slip->GetCr());
			}
			Slip = Slip->GetPrevSlip();
			if(Slip == SlipList)
				break;
		}
	}
}


BOOL CGameState::AdvanceCreature(CCreature* pCreature, int Dir)
{
	if(Dir == NONE)
		return TRUE;

	if(pCreature->GetType() == Chip)
	{
		ResetChipWait();
		ResetButtons();
		SetDeferButtons();
	}

	if(!StartMovement(pCreature, Dir))
	{
		if(pCreature->GetType() == Chip)
		{
			//SOUND EFFECT **[CANT MOVE]**
			ResetDeferButtons();
			ResetButtons();
		}
		return FALSE;
	}

	EndMovement(pCreature, Dir);
	if(pCreature->GetType() == Chip)
	{
		ResetDeferButtons();
		HandleButtons();
	}

	return TRUE;
}

BOOL CGameState::StartMovement(CCreature* cr, int Dir)
{
	int nFloor, nType;

	nFloor = CurrentLevel.GetCell(cr->GetPos())->GetBottom()->GetID();
	if(!CanMakeMove(cr, Dir, 0))
	{
		nType = cr->GetType();
		if(nType == Chip || (nFloor != Beartrap && nFloor != CloneMachine && !(cr->GetState() & CS_SLIP)))
		{
			cr->SetDirection(Dir);
			UpdateCreature(cr);
		}
		return FALSE;
	}
	cr->SetState(cr->GetState() & ~CS_RELEASED);

	cr->SetDirection(Dir);

	return TRUE;
}

void CGameState::EndMovement(CCreature* cr, int Dir)
{
	CCell*	Cell;
	CTile*	Tile;
	BOOL	Dead = FALSE;
	POINT	OldPos, NewPos, i;
	int		nFloor, n;

	OldPos = cr->GetPos();
	NewPos = cr->Front();

	Cell = CellAt(NewPos);
	Tile = Cell->GetTop();
	nFloor = Tile->GetID();

	if(cr->GetType() == Chip)
	{
		switch(nFloor)
		{
			case Empty:
			case Dirt:
			case BlueWall_Fake:
				CurrentLevel.PopTile(NewPos);
				break;
			case Water:
				if(!Possession(Boots_Water))
					SetChipStatus(SF_CHIPDROWNED);
				break;
			case Fire:
				if(!Possession(Boots_Fire))
					SetChipStatus(SF_CHIPBURNED);
				break;
			case PopupWall:
				Tile->SetID(Wall);
				break;
			case Door_Red:
			case Door_Blue:
			case Door_Yellow:
			case Door_Green:
				Discard(nFloor);
				CurrentLevel.PopTile(NewPos);
				//SOUND EFFECT **[DOOR OPENED]**
				break;
			case Key_Red:
			case Key_Blue:
			case Key_Yellow:
			case Key_Green:
			case Boots_Ice:
			case Boots_Slide:
			case Boots_Fire:
			case Boots_Water:
				Collect(nFloor);
				CurrentLevel.PopTile(NewPos);
				//SOUND EFFECT **[ITEM COLLECTED]**
				break;
			case Thief:
				for(n=0;n<4;n++)
					bBoots[n] = FALSE;
				//SOUND EFFECT **[BOOTS STOLEN]**
				break;
			case ICChip:
				DecChips();
				CurrentLevel.PopTile(NewPos);
				//SOUND EFFECT **[ITEM COLLECTED]**
				break;
			case Socket:
				CurrentLevel.PopTile(NewPos);
				//SOUND EFFECT **[SOCKET OPENED]**
				break;
			case Bomb:
				SetChipStatus(SF_CHIPBOMBED);
				//SOUND EFFECT **[BOMB EXPLODE]**
				break;
		}
	}
	else if(cr->GetType() == Block)
	{
		switch(nFloor)
		{
			case Empty:
				CurrentLevel.PopTile(NewPos);
				break;
			case Water:
				Tile->SetID(Dirt);
				Dead = TRUE;
				//SOUND EFFECT **[WATER SPLASH]**
				break;
			case Bomb:
				Tile->SetID(Empty);
				Dead = TRUE;
				//SOUND EFFECT **[BOMB EXPLODE]**
				break;
			case Teleport:
				if(!(Tile->GetState() & FS_BROKEN))
					NewPos = TeleportCreature(cr, NewPos);
				break;
		}
	}
	else
	{
		if(IsCreature(nFloor))
		{
			Tile = Cell->GetBottom();
			nFloor = Tile->GetID();
		}
		switch(nFloor)
		{
			case Water:
				if(cr->GetType() != Glider)
					Dead = TRUE;
				break;
			case Fire:
				if(cr->GetType() != Fireball)
					Dead = TRUE;
				break;
			case Bomb:
				Tile->SetID(Empty);
				Dead = TRUE;
				//SOUND EFFECT **[BOMB EXPLODE]**
				break;
			case Teleport:
				if(!(Tile->GetState() & FS_BROKEN))
					NewPos = TeleportCreature(cr, NewPos);
				break;
		}
	}
	if(CellAt(OldPos)->GetBottom()->GetID() != CloneMachine)
		CurrentLevel.PopTile(OldPos);
	if(Dead)
	{
		if(cr->GetType() == Block)
			DeadBlocks++;
		else
			DeadCreatures++;

		cr->SetState(cr->GetState() & ~(CS_SLIP | CS_SLIDE));
		cr->SetHidden(TRUE);
		if(CellAt(OldPos)->GetBottom()->GetID() == CloneMachine)
			CellAt(OldPos)->GetBottom()->SetState(CellAt(OldPos)->GetBottom()->GetState() & ~FS_CLONING);
		return;
	}

	if(cr->GetType() == Chip && nFloor == Teleport && !(Tile->GetState() & FS_BROKEN))
	{
		i = NewPos;
		NewPos = TeleportCreature(cr, NewPos);
		//if(!EqualPoints(NewPos, i))
			//SOUND EFFECT **[TELEPORT]**
	}
	
    cr->SetPos(NewPos);
    AddCreatureToMap(cr);
    cr->SetPos(OldPos);
    Tile = Cell->GetBottom();

	switch(nFloor)
	{
		case Button_Blue:
			if(ButtonsDeferred())
				Tile->SetState(Tile->GetState() | FS_BUTTONDOWN);
			else
				ToggleTanks(cr);
			//SOUND EFFECT **[BUTTON PUSHED]**
			break;
		case Button_Green:
			if(ButtonsDeferred())
				Tile->SetState(Tile->GetState() | FS_BUTTONDOWN);
			else
				CurrentLevel.ToggleBlocks();
			break;
		case Button_Red:
			if(ButtonsDeferred())
				Tile->SetState(Tile->GetState() | FS_BUTTONDOWN);
			else
				ActivateCloner(NewPos);
			//SOUND EFFECT **[BUTTON PUSHED]**
			break;
		case Button_Brown:
			if(ButtonsDeferred())
				Tile->SetState(Tile->GetState() | FS_BUTTONDOWN);
			else
				SpringTrap(NewPos);
			//SOUND EFFECT **[BUTTON PUSHED]**
			break;
	}

	cr->SetPos(NewPos);

	if(CellAt(OldPos)->GetBottom()->GetID() == CloneMachine)
		CellAt(OldPos)->GetBottom()->SetState(CellAt(OldPos)->GetBottom()->GetState() & ~FS_CLONING);
	
	if(nFloor == Beartrap)
	{
		if(IsTrapOpen(NewPos))
			cr->SetState(cr->GetState() | CS_RELEASED);
	}
	else if(CellAt(NewPos)->GetBottom()->GetID() == Beartrap)
	{
		for(n=0; n<CurrentLevel.GetTrapCount(); n++)
		{
			if(EqualPoints(CurrentLevel.GetTrap(n)->GetDevice(), NewPos))
			{
				cr->SetState(cr->GetState() | CS_RELEASED);
				break;
			}
		}
	}

	if(cr->GetType() == Chip)
	{
		if(IsCreature(Cell->GetBottom()->GetID()) && creatureid(Cell->GetBottom()->GetID()) != Chip)
		{
			if(creatureid(Cell->GetBottom()->GetID()) == Block)
				SetChipStatus(SF_CHIPBLOCKHIT);
			else
				SetChipStatus(SF_CHIPHIT);
			return;
		}
		else if(Cell->GetBottom()->GetID() == Exit)
		{
			SetCompleted();
			return;
		}
	}
	else
	{
		if(IsCreature(Cell->GetBottom()->GetID()) && (creatureid(Cell->GetBottom()->GetID()) == Chip || creatureid(Cell->GetBottom()->GetID()) == Chip_Swimming))
		{
			if(cr->GetType() == Block)
				SetChipStatus(SF_CHIPBLOCKHIT);
			else
				SetChipStatus(SF_CHIPHIT);
			return;
		}
	}

	if(nFloor == Teleport)
		StartFloorMovement(cr, nFloor);
	else if(IsIce(nFloor) && (cr->GetType() != Chip || !Possession(Boots_Ice)))
		StartFloorMovement(cr, nFloor);
	else if(IsSlide(nFloor) && (cr->GetType() != Chip || !Possession(Boots_Slide)))
		StartFloorMovement(cr, nFloor);
	else if(nFloor != Beartrap || cr->GetType() != Block)
		cr->SetState(cr->GetState() & ~(CS_SLIP | CS_SLIDE));
}


void CGameState::AddCreatureToMap(CCreature* cr)
{
    if (cr->GetHidden())
		return;
    CurrentLevel.PushTile(cr->GetPos(), MakeTile(Empty, 0));
    UpdateCreature(cr);
}

POINT CGameState::ClonerFromButton(POINT ptPos)
{
	CDevice* Cloner;
	POINT NoCloner;
	int i;

	NoCloner.x = -1;
	NoCloner.y = -1;
	for(i=0; i<CurrentLevel.GetClonerCount(); i++)
	{
		Cloner = CurrentLevel.GetCloner(i);
		if(EqualPoints(Cloner->GetButton(), ptPos))
			return Cloner->GetDevice();
	}
	return NoCloner;
}

BOOL CGameState::IsTrapOpen(POINT ptPos)
{
	CDevice* Trap;
	int i;

	for(i=0; i<CurrentLevel.GetTrapCount(); i++)
	{
		Trap = CurrentLevel.GetTrap(i);
		if(EqualPoints(Trap->GetDevice(), ptPos) && IsOccupied(Trap->GetButton()))
			return TRUE;
	}

	return FALSE;
}


POINT CGameState::TrapFromButton(POINT ptPos)
{
	CDevice* Trap;
	POINT NoTrap;
	int i;

	NoTrap.x = -1;
	NoTrap.y = -1;
	for(i=0; i<CurrentLevel.GetTrapCount(); i++)
	{
		Trap = CurrentLevel.GetTrap(i);
		if(EqualPoints(Trap->GetButton(), ptPos))
			return Trap->GetDevice();
	}
	return NoTrap;
}

void CGameState::ActivateCloner(POINT ptPos)
{
	CCreature* cr;
	POINT ptClonerPos;
	int nTile;
	ptClonerPos = ClonerFromButton(ptPos);

	if(ptClonerPos.x < 0 || ptClonerPos.y < 0)
		return;
	nTile = CellAt(ptClonerPos)->GetTop()->GetID();
	if(creatureid(nTile) == Block)
	{
		cr = GetBlock(ptClonerPos);
		if(cr != NULL)
		{
			AdvanceCreature(cr, cr->GetDirection());
		}
	}
	else
	{
		if(!IsCreature(nTile))
			return;
		if(CellAt(ptClonerPos)->GetBottom()->GetState() & FS_CLONING)
			return;
		cr = AddCreature(ptClonerPos, creaturedirid(nTile), creatureid(nTile));
		if(!CanMakeMove(cr, cr->GetDirection(), CMM_CLONECANTBLOCK))
			return;
		cr->SetState(cr->GetState() | CS_CLONING);
		if(CellAt(ptClonerPos)->GetBottom()->GetID() == CloneMachine)
			CellAt(ptClonerPos)->GetBottom()->SetState(CellAt(ptClonerPos)->GetBottom()->GetState() | FS_CLONING);
	}
}

void CGameState::SpringTrap(POINT ptPos)
{
	CCreature* cr;
	POINT ptTrapPos;
	int nTile;

	ptTrapPos = TrapFromButton(ptPos);
	if(ptTrapPos.x < 0 || ptTrapPos.y < 0)
		return;
	nTile = CellAt(ptTrapPos)->GetTop()->GetID();
	if(nTile == Block_Static)
	{
		cr = GetBlock(ptTrapPos);
		if(cr != NULL)
			cr->SetState(cr->GetState() | CS_RELEASED);
	}
	else if(IsCreature(nTile))
	{
		cr = GetCreature(ptTrapPos, TRUE);
		if(cr != NULL)
			cr->SetState(cr->GetState() | CS_RELEASED);
	}
}

POINT CGameState::TeleportCreature(CCreature* cr, POINT ptPos)
{
	CTile* Tile;
	POINT OrigPos, Dest;
	BOOL f, Defer;

	OrigPos = cr->GetPos();
	Dest = ptPos;

	Defer = ButtonsDeferred();
	ResetDeferButtons();
	for(;;)
	{
		Dest.x--;
		if(Dest.x < 0)
		{
			Dest.y--;
			Dest.x = 31;
		}
		if(Dest.y < 0)
			Dest.y = 31;
		if(EqualPoints(Dest, ptPos))
			break;
		Tile = CellAt(Dest)->GetTop();
		if(Tile->GetID() != Teleport || (Tile->GetState() & FS_BROKEN))
			continue;
		cr->SetPos(Dest);
		f = CanMakeMove(cr, cr->GetDirection(), CMM_NOLEAVECHECK | CMM_NOEXPOSEWALLS | CMM_NOCOLLAPSEBLOCKS);
		cr->SetPos(OrigPos);
		if(f) 
			break;
	}
	if(Defer)
		SetDeferButtons();

	return Dest;
}

int CGameState::GetChipWalkCmd()
{
	int x, y, i, choices[2], m, n;
	POINT ptTile;

	ptTile = GetChip()->GetDestination();
	x = ptTile.x - GetChip()->GetPos().x;
	y = ptTile.y - GetChip()->GetPos().y;
	n = y < 0 ? NORTH : y > 0 ? SOUTH : NONE;
	if(y<0) y = -y;
	m = x < 0 ? WEST : x > 0 ? EAST : NONE;
	if(x<0) x = -x;
	if(x>y)
	{
		choices[0] = m;
		choices[1] = n;
	}
	else
	{
		choices[0] = n;
		choices[1] = m;
	}
	i=0;
	while(i < 2)
	{
		if(choices[i] != NONE  && CanMakeMove(GetChip(), choices[i], 0))
		{
			return diridx(choices[i]);
		}
		i++;
	}
	GetChip()->SetWalking(FALSE);
	return NONE;
}


void CGameState::Reset()
{
	SetGameMode(GM_NORMAL);
	bMoving = FALSE;
	ResetTick();
	for(int i=0;i<4;i++)
	{
		nKeys[i] = 0;
		bBoots[i] = FALSE;
	}
	SetStatus(SF_CHIPOKAY);
	GetChip()->SetWalking(FALSE);
	RemoveAllCreatures();
	RemoveAllBlocks();
	nTick = 0;
	SetButton(NONE);
	DeadCreatures = 0;
	DeadBlocks = 0;
}

void CGameState::ExtractCreatures()
{
	int i, nTile, CreatureType, CreatureDir;
	POINT ptTile;
	for(i = 0; i < CurrentLevel.GetCreatureCount(); i++)
	{
		ptTile = CurrentLevel.GetCreature(i);
		nTile = CellAt(ptTile)->GetTop()->GetID();
		if(IsCreature(nTile))
		{
			CreatureType = creatureid(nTile);
			CreatureDir = creaturedirid(nTile);
			AddCreature(ptTile, CreatureDir, CreatureType);
		}
	}
	for(ptTile.x=0; ptTile.x<32; ptTile.x++)
	{
		for(ptTile.y=0; ptTile.y<32; ptTile.y++)
		{
			nTile = CellAt(ptTile)->GetTop()->GetID();
			if(creatureid(nTile) == Chip)
			{
				GetChip()->SetPos(ptTile);
				GetChip()->SetType(Chip);
				GetChip()->SetDirection(creaturedirid(nTile));
			}
		}
	}
}


CCreature* CGameState::AppendToSlipList(CCreature* cr, int Dir)
{

	CSlip* Slip;
	CSlip* NewSlip;

	cr->SetOnSlipList(TRUE);
	if(nSlipCount > 0)
	{
		Slip = SlipList->GetNextSlip();
		for(;;)
		{
			if(Slip->GetCr() == cr)
			{
				Slip->SetDir(Dir);
				return cr;
			}
			if(Slip == SlipListEnd)
				break;
			Slip = Slip->GetNextSlip();
		}
	}

	NewSlip = new CSlip();
	NewSlip->SetPrevSlip(SlipListEnd);
	SlipListEnd->SetNextSlip(NewSlip);
	SlipListEnd = NewSlip;

	NewSlip->SetCr(cr);
	NewSlip->SetDir(Dir);
	nSlipCount++;

	return cr;

}


CCreature* CGameState::PrependToSlipList(CCreature* cr, int Dir)
{
	CSlip* Slip;
	CSlip* NewSlip;

	cr->SetOnSlipList(TRUE);
	if(nSlipCount > 0)
	{
		Slip = SlipList->GetNextSlip();
		for(;;)
		{
			if(Slip->GetCr() == cr)
			{
				Slip->SetDir(Dir);
				return cr;
			}
			if(Slip == SlipListEnd)
				break;
			Slip = Slip->GetNextSlip();
		}
	}

	NewSlip = new CSlip();
	if(nSlipCount == 0)
	{
		NewSlip->SetPrevSlip(SlipListEnd);
		SlipListEnd->SetNextSlip(NewSlip);
		SlipListEnd = NewSlip;
	}
	else
	{
		NewSlip->SetNextSlip(SlipList->GetNextSlip());
		SlipList->GetNextSlip()->SetPrevSlip(NewSlip);
		NewSlip->SetPrevSlip(SlipList);
		SlipList->SetNextSlip(NewSlip);
	}

	nSlipCount++;
	NewSlip->SetCr(cr);
	NewSlip->SetDir(Dir);
	return cr;

}


void CGameState::RemoveAllSlips()
{
	while(SlipList != SlipListEnd)
	{
		SlipListEnd = SlipListEnd->GetPrevSlip();
		delete SlipListEnd->GetNextSlip();
		SlipListEnd->SetNextSlip(NULL);
	}
	nSlipCount = 0;
}

void CGameState::RemoveSlip(CSlip* Slip)
{
	if(nSlipCount == 0)
		return;

	if(Slip == SlipListEnd)
	{
		SlipListEnd = Slip->GetPrevSlip();
	}
	else
	{
		Slip->GetPrevSlip()->SetNextSlip(Slip->GetNextSlip());
		Slip->GetNextSlip()->SetPrevSlip(Slip->GetPrevSlip());
	}
	delete Slip;
	nSlipCount--;
}


void CGameState::RemoveFromSlipList(CCreature* cr)
{
	CSlip* Slip = SlipList->GetNextSlip();

	if(nSlipCount == 0)
		return;
	for(;;)
	{
		if(Slip->GetCr() == cr)
		{
			cr->SetOnSlipList(FALSE);
			RemoveSlip(Slip);
			break;
		}
		if(Slip == SlipListEnd)
			break;
		Slip = Slip->GetNextSlip();
	}
}


int CGameState::GetSlipDir(CCreature* cr)
{
	CSlip* Slip;

	Slip = SlipList->GetNextSlip();
	for(;;)
	{
		if(Slip->GetCr() == cr)
			return Slip->GetDir();
		if(Slip == SlipListEnd)
			break;
		Slip = Slip->GetNextSlip();
	}
	return NONE;
}


CCreature* CGameState::AddCreature(POINT ptTile, int Dir, int Type)
{
	CCreature* NewCreature;
	NewCreature = new CCreature();
	NewCreature->SetPos(ptTile);
	NewCreature->SetDirection(Dir);
	NewCreature->SetType(Type);

	NewCreature->SetPrevCreature(CreaturePoolEnd);
	CreaturePoolEnd->SetNextCreature(NewCreature);
	CreaturePoolEnd = NewCreature;

	nCreatureCount++;
	return NewCreature;
}


void CGameState::RemoveCreature(CCreature* cr)
{
	if(cr == GetChip() && GetChipStatus() == SF_CHIPOKAY)
	{
		SetChipStatus(SF_CHIPNOTOKAY);
	}

	if(cr == CreaturePoolEnd)
	{
		CreaturePoolEnd = cr->GetPrevCreature();
	}
	else
	{
		cr->GetPrevCreature()->SetNextCreature(cr->GetNextCreature());
		cr->GetNextCreature()->SetPrevCreature(cr->GetPrevCreature());
	}
	delete cr;
}

void CGameState::RemoveAllCreatures()
{
	while(CreaturePool != CreaturePoolEnd)
	{
		CreaturePoolEnd = CreaturePoolEnd->GetPrevCreature();
		delete CreaturePoolEnd->GetNextCreature();
		CreaturePoolEnd->SetNextCreature(NULL);
	}
	nCreatureCount = 0;
}

void CGameState::RemoveDeadCreatures()
{
	CCreature* tempCreature;
	tempCreature = CreaturePoolEnd;
	while(tempCreature != CreaturePool)
	{
		if(tempCreature->GetHidden() && !tempCreature->GetOnSlipList())
		{
			if(tempCreature == CreaturePoolEnd)
			{
				CreaturePoolEnd = tempCreature->GetPrevCreature();
				CreaturePoolEnd->SetNextCreature(NULL);
				delete tempCreature;
			}
			else
			{
				tempCreature->GetPrevCreature()->SetNextCreature(tempCreature->GetNextCreature());
				tempCreature->GetNextCreature()->SetPrevCreature(tempCreature->GetPrevCreature());
				delete tempCreature;
			}
			nCreatureCount--;
			DeadCreatures--;
		}
		tempCreature = tempCreature->GetPrevCreature();
	}
}


CCreature* CGameState::GetCreature(POINT ptPos, BOOL IncludeChip)
{
	int i;
	
	CCreature* tempCr;
	tempCr = CreaturePool;
	for(i=0; i<nCreatureCount; i++)
	{
		tempCr = tempCr->GetNextCreature();
		if(tempCr->GetHidden())
			continue;
		if(EqualPoints(tempCr->GetPos(), ptPos))
			return tempCr;
	}
	if(IncludeChip && EqualPoints(GetChip()->GetPos(), ptPos))
		return GetChip();
	return NULL;
}


CCreature* CGameState::AddBlock(POINT ptTile, int Dir, int Type)
{
	CCreature* NewBlock;
	NewBlock = new CCreature();
	NewBlock->SetPos(ptTile);
	NewBlock->SetDirection(Dir);
	NewBlock->SetType(Type);

	NewBlock->SetPrevCreature(BlockPoolEnd);
	BlockPoolEnd->SetNextCreature(NewBlock);
	BlockPoolEnd = NewBlock;

	nBlockCount++;

	return NewBlock;
}

void CGameState::RemoveBlock(CCreature* cr)
{
	if(cr == BlockPoolEnd)
	{
		BlockPoolEnd = cr->GetPrevCreature();
	}
	else
	{
		cr->GetPrevCreature()->SetNextCreature(cr->GetNextCreature());
		cr->GetNextCreature()->SetPrevCreature(cr->GetPrevCreature());
	}
	delete cr;
	nBlockCount--;
}

void CGameState::RemoveAllBlocks()
{
	while(BlockPool != BlockPoolEnd)
	{
		BlockPoolEnd = BlockPoolEnd->GetPrevCreature();
		delete BlockPoolEnd->GetNextCreature();
		BlockPoolEnd->SetNextCreature(NULL);
	}
	nBlockCount = 0;
}

void CGameState::RemoveDeadBlocks()
{
	CCreature* tempBlock;
	tempBlock = BlockPoolEnd;
	while(tempBlock != BlockPool)
	{
		if(tempBlock->GetHidden() && !tempBlock->GetOnSlipList())
		{
			if(tempBlock == BlockPoolEnd)
			{
				BlockPoolEnd = tempBlock->GetPrevCreature();
				BlockPoolEnd->SetNextCreature(NULL);
				delete tempBlock;
			}
			else
			{
				tempBlock->GetPrevCreature()->SetNextCreature(tempBlock->GetNextCreature());
				tempBlock->GetNextCreature()->SetPrevCreature(tempBlock->GetPrevCreature());
				delete tempBlock;
			}
			nBlockCount--;
			DeadBlocks--;
		}
		tempBlock = tempBlock->GetPrevCreature();
	}
}

CCreature* CGameState::GetBlock(POINT ptPos)
{
	int i;
	CCreature* tempCr;
	tempCr = BlockPool;
	int CreatureDir, nTile;

	for(i=0; i<nBlockCount; i++)
	{
		tempCr = tempCr->GetNextCreature();
		if(EqualPoints(tempCr->GetPos(), ptPos) && !tempCr->GetHidden())
			return tempCr;
	}

	nTile = CellAt(ptPos)->GetTop()->GetID();
	if(creatureid(nTile) == Block)
		CreatureDir = creaturedirid(nTile);
	else if(nTile == Block_Static)
		CreatureDir = NONE;
	else
		return NULL;

	tempCr = AddBlock(ptPos, CreatureDir, Block);
	
	if(CellAt(ptPos)->GetBottom()->GetID() == Beartrap)
	{
		for(i=0; i<CurrentLevel.GetTrapCount(); i++)
		{
			if(EqualPoints(CurrentLevel.GetTrap(i)->GetDevice(), tempCr->GetPos()))
			{
				tempCr->SetState(tempCr->GetState() | CS_RELEASED);
				break;
			}
		}
	}

	return tempCr;
}

void CGameState::RandomP(int* array, int PLevel)
{
	int i, n, t;
	srand(GetTickCount()); 
	for(i= (5 - PLevel); i<=PLevel; i++)
	{
		n = rand() % i;
		t = array[n]; array[n] = array[i - 1]; array[i - 1] = t;
	}
}

int CGameState::Random4()
{
	srand(GetTickCount());
	return rand() % 4;
}
void CGameState::SetFloorAt(POINT ptTile, int Tile)
{
	int TestTile = CellAt(ptTile)->GetTop()->GetID();
	if(!IsKey(TestTile) && !IsBoots(TestTile) && !IsCreature(TestTile))
	{
		CellAt(ptTile)->GetTop()->SetID(Tile);
		return;
	}
	else
		CellAt(ptTile)->GetBottom()->SetID(Tile);
}

int CGameState::FloorAt(POINT ptTile)
{
	int Tile = CellAt(ptTile)->GetTop()->GetID();
	if(!IsKey(Tile) && !IsBoots(Tile) && !IsCreature(Tile))
		return Tile;
	Tile = CellAt(ptTile)->GetBottom()->GetID();
	if(!IsKey(Tile) && !IsBoots(Tile) && !IsCreature(Tile))
		return Tile;
	return Empty;
}


BOOL CGameState::IsOccupied(POINT ptPos)
{
	int id = CellAt(ptPos)->GetTop()->GetID();
	return IsCreature(id) || (id == Block_Static);
}


void CGameState::ToggleTanks(CCreature* InMidMove)
{
	CCreature* pCreature;
	if(nCreatureCount > 0)
	{
		pCreature = CreaturePool->GetNextCreature();
		for(;;)
		{
			if(pCreature->GetType() == Tank && !pCreature->GetHidden() && CellAt(pCreature->GetPos())->GetBottom()->GetID() != CloneMachine)
			{
				pCreature->SetDirection(back(pCreature->GetDirection()));
				if(!(pCreature->GetState() & CS_TURNING))
					pCreature->SetState(pCreature->GetState() | CS_TURNING | CS_HASMOVED);
				if(pCreature != InMidMove)
					UpdateCreature(pCreature);
			}
			if(pCreature == CreaturePoolEnd)
				break;
			pCreature = pCreature->GetNextCreature();
		}
	}
}

BOOL CGameState::SomeoneAt(POINT ptTile)
{
	int id;
	id = CurrentLevel.GetCell(ptTile)->GetTop()->GetID();
	if(creatureid(id) == Chip)
	{
		id = CurrentLevel.GetCell(ptTile)->GetBottom()->GetID();
		if(creatureid(id) == Chip)
			return FALSE;
	}
	return IsCreature(id);
}


void CGameState::SetGameMode(int Mode)
{
	//Don't allow modes to double up;
	if(nGameMode != Mode)
	{
		if(nGameMode != GM_MENU && nGameMode != GM_NEWGAME)
		{
			nPrevMode2 = nPrevMode;
			nPrevMode = nGameMode;
		}
		nGameMode = Mode;
	}
}


void CGameState::ChooseChipMove(CCreature* pCreature, int Discard)
{
	int dir;
	pCreature->SetToDir(NONE);

	if(pCreature->GetHidden())
		return;

	if(!(GetTick() & 3))
		pCreature->SetState(pCreature->GetState() & ~CS_HASMOVED);
	if(pCreature->GetState() & CS_HASMOVED)
		return;

	dir = GetButton();
	SetButton(NONE);
	if(!(dir >= NORTH && dir <= EAST))
		dir = NONE;
	if(dir == NONE || Discard)
		return;
	if((pCreature->GetState() & CS_SLIDE) && dir == pCreature->GetDirection())
		return;
	SetLastMove(dir);
	pCreature->SetToDir(dir);
}

int CGameState::GetControllerDir(CCreature* cr)
{
	CCreature* pCreature;
	if(nCreatureCount > 0)
	{
		pCreature = CreaturePool->GetNextCreature();
		for(;;)
		{
			if(pCreature == cr || pCreature == CreaturePoolEnd)
				break;
			pCreature = pCreature->GetNextCreature();
		}

		pCreature = pCreature->GetPrevCreature();
		for(;;)
		{
			if(!pCreature->GetHidden() && pCreature != GetChip() && CurrentLevel.GetCell(pCreature->GetPos())->GetBottom()->GetID() != CloneMachine)
				return pCreature->GetDirection();
			pCreature = pCreature->GetPrevCreature();
			if(pCreature == CreaturePool)
				break;
		}
	}
	return NONE;
}


void CGameState::ChooseCreatureMove(CCreature* pCreature)
{

	int choices[4] = {NONE, NONE, NONE, NONE};
	int dir, pdir;
	int nFloor, nType;
	int	y, x, m, n;

	pCreature->SetToDir(NONE);
	nType = pCreature->GetType();
	if(pCreature->GetHidden())
		return;
	if(nType == Block)
		return;
	if(GetTick() & ((nType == Teeth) || (nType == Blob) ? 6 : 2))
		return;

	if(pCreature->GetState() & CS_TURNING)
	{
		pCreature->SetState(pCreature->GetState() & ~(CS_TURNING | CS_HASMOVED));
		UpdateCreature(pCreature);
	}
	if(pCreature->GetState() & (CS_HASMOVED | CS_SLIP | CS_SLIDE))
		return;

	nFloor = FloorAt(pCreature->GetPos());
	pdir = dir = pCreature->GetDirection();
	if(nFloor == CloneMachine || nFloor == Beartrap)
	{
		if(nFloor == Beartrap && !(pCreature->GetState() & CS_RELEASED))
			return;
		if(nFloor == CloneMachine && (pCreature->GetState() & CS_CLONING))
			return;
		switch(pCreature->GetType())
		{
			case Tank:
			case Ball:
			case Glider:
			case Fireball:
			case Walker:
				choices[0] = dir;
				break;
			case Blob:
				choices[0] = dir;
				choices[1] = left(dir);
				choices[2] = back(dir);
				choices[3] = right(dir);
				RandomP(choices, 4);
				break;
			case Bug:
			case Paramecium:
			case Teeth:
				choices[0] = GetControllerDir(pCreature);
				pdir = NONE;
				break;
		}
	}
	else
	{
		switch(pCreature->GetType())
		{
			case Tank:
				choices[0] = dir;
				break;
			case Ball:
				choices[0] = dir;
				choices[1] = back(dir);
				break;
			case Glider:
				choices[0] = dir;
				choices[1] = left(dir);
				choices[2] = right(dir);
				choices[3] = back(dir);
				break;
			case Fireball:
				choices[0] = dir;
				choices[1] = right(dir);
				choices[2] = left(dir);
				choices[3] = back(dir);
				break;
			case Walker:
				choices[0] = dir;
				choices[1] = left(dir);
				choices[2] = right(dir);
				choices[3] = back(dir);
				RandomP(choices + 1, 3);
				break;
			case Blob:
				choices[0] = dir;
				choices[1] = left(dir);
				choices[2] = right(dir);
				choices[3] = back(dir);
				RandomP(choices, 4);
				break;
			case Bug:
				choices[0] = left(dir);
				choices[1] = dir;
				choices[2] = right(dir);
				choices[3] = back(dir);
				break;
			case Paramecium:
				choices[0] = right(dir);
				choices[1] = dir;
				choices[2] = left(dir);
				choices[3] = back(dir);
				break;
			case Teeth:
				x = GetChip()->GetPos().x - pCreature->GetPos().x;
				y = GetChip()->GetPos().y - pCreature->GetPos().y;
				n = y < 0 ? NORTH : y > 0 ? SOUTH : NONE;
				if(y<0) y = -y;
				m = x < 0 ? WEST : x > 0 ? EAST : NONE;
				if(x<0) x = -x;
				if(x>y)
				{
					choices[0] = m;
					choices[1] = n;
				}
				else
				{
					choices[0] = n;
					choices[1] = m;
				}
				pdir = choices[0];
				break;
		}
	}
	for(n=0; n<4 && choices[n] != NONE; n++)
	{
		pCreature->SetToDir(choices[n]);
		if(CanMakeMove(pCreature, choices[n], 0))
			return;
	}

	if(pCreature->GetType() == Tank)
		pCreature->SetState(pCreature->GetState() | CS_HASMOVED);

	pCreature->SetToDir(pdir);
}

void CGameState::Prepare()
{
	CCreature* cr;
    if (!(GetTick() & 3))
	{
		if(nCreatureCount)
		{
			cr = CreaturePool->GetNextCreature();
			for (;;)
			{
				if (cr->GetState() & CS_TURNING)
				{
					cr->SetState(cr->GetState() & ~(CS_TURNING | CS_HASMOVED));
					UpdateCreature(cr);
				}
				if(cr == CreaturePoolEnd)
					break;
				cr = cr->GetNextCreature();
			}
		}
		IncrChipWait();
		if (GetChipWait() > 3) 
		{
			ResetChipWait();
			GetChip()->SetDirection(SOUTH);
			UpdateCreature(GetChip());
		}
	}
}

void CGameState::AdvanceGame(int Cmd)
{
	int DirCmd;
	CCreature* cr;

	if(Cmd == NONE)
	{
		if(GetChip()->GetWalking())
		{
			DirCmd = GetChipWalkCmd();
			if(DirCmd != NONE)
				Cmd = DirCmd;
		}
	}
	else
		GetChip()->SetWalking(FALSE);
	
	if(GetTick() || (Cmd >= UP && Cmd <= RIGHT)) IncTick();

	if(Cmd >= UP && Cmd <= RIGHT) 
		SetButton(idxdir(Cmd));
	else
		SetButton(NONE);

	Prepare();

	if(GetTick() && !(GetTick() & 1))
	{
		if(nCreatureCount)
		{
			cr = CreaturePool->GetNextCreature();
			for (;;)
			{
				if(!cr->GetHidden() && !(cr->GetState() & CS_CLONING))
				{
					ChooseCreatureMove(cr);
					if(cr->GetToDir() != NONE)
						AdvanceCreature(cr, cr->GetToDir());
				}
				if(cr == CreaturePoolEnd)
					break;
				cr = cr->GetNextCreature();
			}
		}

		if(CheckForEnding())
		{
			Finalize();
			return;
		}
	}

	if(GetTick() && !(GetTick() & 1))
	{
		FloorMovements();
		if(CheckForEnding())
		{
			Finalize();
			return;
		}
	}

	if(GetTick())
	{
		if((GetTick() >= TimeLimit) && TimeLimit)
		{
			//SOUND EFFECT **[TIME UP]**
			SetChipStatus(SF_CHIPTIMEUP);
			return;
		}
		else if((TimeLimit - GetTick() <= 15 * TICKS_PER_SECOND) && (GetTick() % TICKS_PER_SECOND == 0))
		{
			//SOUND EFFECT **[TIME LOW]**
		}
	}

	cr = GetChip();
	ChooseChipMove(cr, cr->GetState() & CS_SLIP);
	if(cr->GetToDir() != NONE)
	{
		if(AdvanceCreature(cr, cr->GetToDir()))
		{
			if(CheckForEnding())
			{
				Finalize();
				return;
			}
		}
		cr->SetState(cr->GetState() | CS_HASMOVED);
	}

	CreateClones();


	Finalize();
}

void CGameState::CreateClones()
{
	CCreature* cr;
	if(nCreatureCount)
	{
		cr = CreaturePool->GetNextCreature();
		for (;;)
		{
			if(cr->GetState() & CS_CLONING)
				cr->SetState(cr->GetState() & ~CS_CLONING);
			if(cr == CreaturePoolEnd)
				break;
			cr = cr->GetNextCreature();
		}
	}
}

void CGameState::Finalize()
{
	if(DeadCreatures) 
		RemoveDeadCreatures();
	if(DeadBlocks)
		RemoveDeadBlocks();
	if (CellAt(GetChip()->GetPos())->GetBottom()->GetID() == HintButton)
		ShowHint();
	else
		HideHint();
}

BOOL CGameState::CheckForEnding()
{
	if(IsCompleted())
	{
		//SOUND EFFECT **[CHIP WINS]**
		SetGameMode(GM_LEVELWON);
		return TRUE;
	}
	if(GetStatus() & SF_CHIPNOTOKAY)
	{
		//SOUND EFFECT **[CHIP LOSES]**
		SetGameMode(GM_CHIPDEAD);
		return TRUE;
	}
	return FALSE;

}


/*Chip, Block, Creature*/
#define NWSE (NORTH|WEST|SOUTH|EAST)
void CGameState::CreateTileset()
{
	int i;
	Tileset[Empty].SetWalk(NWSE, NWSE, NWSE);
	Tileset[Wall].SetWalk(0, 0, 0);
	Tileset[ICChip].SetWalk(NWSE, 0, 0);
	Tileset[Water].SetWalk(NWSE, NWSE, NWSE);
	Tileset[Fire].SetWalk(NWSE, NWSE, NWSE);
	Tileset[Wall_North].SetWalk(WEST|NORTH|EAST, WEST|NORTH|EAST, WEST|NORTH|EAST);
	Tileset[Wall_West].SetWalk(NORTH|SOUTH|WEST, NORTH|SOUTH|WEST, NORTH|SOUTH|WEST);
	Tileset[Wall_South].SetWalk(SOUTH|WEST|EAST, SOUTH|WEST|EAST, SOUTH|WEST|EAST);
	Tileset[Wall_East].SetWalk(NORTH|EAST|SOUTH, NORTH|EAST|SOUTH, NORTH|EAST|SOUTH);
	Tileset[Wall_Southeast].SetWalk(SOUTH|EAST, SOUTH|EAST, SOUTH|EAST);
	Tileset[Block_Static].SetWalk(NWSE, 0, 0);
	Tileset[Dirt].SetWalk(NWSE, 0, 0);
	Tileset[Ice].SetWalk(NWSE, NWSE, NWSE);
	Tileset[Slide_South].SetWalk(NWSE, NWSE, NWSE);
	Tileset[Slide_North].SetWalk(NWSE, NWSE, NWSE);
	Tileset[Slide_East].SetWalk(NWSE, NWSE, NWSE);
	Tileset[Slide_West].SetWalk(NWSE, NWSE, NWSE);
	Tileset[Exit].SetWalk(NWSE, NWSE, 0);
	Tileset[Door_Blue].SetWalk(NWSE, 0, 0);
	Tileset[Door_Red].SetWalk(NWSE, 0, 0);
	Tileset[Door_Green].SetWalk(NWSE, 0, 0);
	Tileset[Door_Yellow].SetWalk(NWSE, 0, 0);
	Tileset[IceWall_Northwest].SetWalk(SOUTH|EAST, SOUTH|EAST, SOUTH|EAST);
	Tileset[IceWall_Northeast].SetWalk(SOUTH|WEST, SOUTH|WEST, SOUTH|WEST);
	Tileset[IceWall_Southwest].SetWalk(NORTH|EAST, NORTH|EAST, NORTH|EAST);
	Tileset[IceWall_Southeast].SetWalk(NORTH|WEST, NORTH|WEST, NORTH|WEST);
	Tileset[BlueWall_Fake].SetWalk(NWSE, 0, 0);
	Tileset[BlueWall_Real].SetWalk(NWSE, 0, 0);
	Tileset[Thief].SetWalk(NWSE, 0, 0);
	Tileset[Socket].SetWalk(NWSE, 0, 0);
	Tileset[Button_Green].SetWalk(NWSE, NWSE, NWSE);
	Tileset[Button_Red].SetWalk(NWSE, NWSE, NWSE);
	Tileset[SwitchWall_Closed].SetWalk(0, 0, 0);
	Tileset[SwitchWall_Open].SetWalk(NWSE, NWSE, NWSE);
	Tileset[Button_Brown].SetWalk(NWSE, NWSE, NWSE);
	Tileset[Button_Blue].SetWalk(NWSE, NWSE, NWSE);
	Tileset[Teleport].SetWalk(NWSE, NWSE, NWSE);
	Tileset[Bomb].SetWalk(NWSE, NWSE, NWSE);
	Tileset[Beartrap].SetWalk(NWSE, NWSE, NWSE);
	Tileset[HiddenWall_Perm].SetWalk(0, 0, 0);
	Tileset[HiddenWall_Temp].SetWalk(NWSE, 0, 0);
	Tileset[Gravel].SetWalk(NWSE, NWSE, 0);
	Tileset[PopupWall].SetWalk(NWSE, 0, 0);
	Tileset[HintButton].SetWalk(NWSE, NWSE, NWSE);
	Tileset[CloneMachine].SetWalk(0, 0, 0);
	Tileset[Slide_Random].SetWalk(NWSE, NWSE, 0);
	Tileset[Key_Blue].SetWalk(NWSE, NWSE, NWSE);
	Tileset[Key_Red].SetWalk(NWSE, NWSE, NWSE);
	Tileset[Key_Green].SetWalk(NWSE, NWSE, NWSE);
	Tileset[Key_Yellow].SetWalk(NWSE, NWSE, NWSE);
	Tileset[Boots_Water].SetWalk(NWSE, NWSE, NWSE);
	Tileset[Boots_Fire].SetWalk(NWSE, NWSE, NWSE);
	Tileset[Boots_Ice].SetWalk(NWSE, NWSE, NWSE);
	Tileset[Boots_Slide].SetWalk(NWSE, NWSE, NWSE);
	Tileset[0x0E].SetWalk(0, 0, 0);
	Tileset[0x0F].SetWalk(0, 0, 0);
	Tileset[0x10].SetWalk(0, 0, 0);
	Tileset[0x11].SetWalk(0, 0, 0);
	for(i=0x40; i<=0x63; i++)
		Tileset[i].SetWalk(NWSE, 0, 0);
	for(i=0x6c; i<=0x6f; i++)
		Tileset[i].SetWalk(0, NWSE, 0);
}
