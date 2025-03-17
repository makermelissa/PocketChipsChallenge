#include "level.h"
#include "defs.h"
#include <windows.h>
#include <tchar.h>

void CLevel::PushTile(POINT ptPos, CTile Tile)
{
	CCell *Cell;
	Cell = GetCell(ptPos);

	Cell->GetBottom()->SetID(Cell->GetTop()->GetID());
	Cell->GetBottom()->SetState(Cell->GetTop()->GetState());

	Cell->GetTop()->SetID(Tile.GetID());
	Cell->GetTop()->SetState(Tile.GetState());
}

CTile CLevel::PopTile(POINT ptPos)
{
	CTile Tile;
	CCell *Cell;
	Cell = GetCell(ptPos);

	Tile.SetID(Cell->GetTop()->GetID());
	Tile.SetState(Cell->GetTop()->GetState());

	Cell->GetTop()->SetID(Cell->GetBottom()->GetID());
	Cell->GetTop()->SetState(Cell->GetBottom()->GetState());

	Cell->GetBottom()->SetID(Empty);
	Cell->GetTop()->SetState(0);

	return Tile;
}

BOOL CLevel::LoadLevelData()
{
	HANDLE hFile;
	BOOL bRet;
	WORD nNextLevelOffset;
	WORD nField;
	WORD nFieldSize;
	WORD nRemainingLevelSize;
	CHIPSFILEHEADER cfh;
	CHIPSLEVELHEADER clh;
	DWORD dwHeaderSize;
	WORD nCurLevel = 1;
	TCHAR uctext[256];
	TCHAR sFile[1024];
	POINT ptCell, ptButton, ptDevice;
	unsigned char text[256], map[1024], ID;
	unsigned int i;
	int nResult;
	nResult = _tcscmp(strFileName, TEXT(""));
	if(nResult == 0)
		wsprintf(sFile, GetPath(TEXT("chips.dat")));
	else
		wsprintf(sFile, strFileName);
	hFile = CreateFile(sFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	};
	/*****************************************/
	/********** Read Level Set Data **********/
	/*****************************************/

	/********** Uncompressed File Size **********/
	bRet = ReadFile(hFile, &cfh.FileSize, sizeof(DWORD), &dwHeaderSize, NULL);
	if ((bRet == FALSE) || (dwHeaderSize != sizeof(DWORD)))
	{
		CloseHandle(hFile);
		return FALSE;
	}

	/********** Number of levels **********/
	bRet = ReadFile(hFile, &cfh.NumberOfLevels, sizeof(WORD), &dwHeaderSize, NULL);
	if ((bRet == FALSE) || (dwHeaderSize != sizeof(WORD)))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	nLastLevel = cfh.NumberOfLevels;
	//Verify Level Number is in acceptable range
	if ((nLevelNumber < 1) || (nLevelNumber > cfh.NumberOfLevels))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	/*************************************/
	/********** Read Level Data **********/
	/*************************************/
	//Read Level Size in bytes
	bRet = ReadFile(hFile, &nNextLevelOffset, sizeof(WORD), &dwHeaderSize, NULL);
	if ((bRet == FALSE) || (dwHeaderSize != sizeof(WORD)))
	{
		CloseHandle(hFile);
		return FALSE;
	}

	//Keep reading levels until desired position is reached.
	while(nCurLevel < nLevelNumber)
	{
		SetFilePointer(hFile, nNextLevelOffset, NULL, FILE_CURRENT);
		bRet = ReadFile(hFile, &nNextLevelOffset, sizeof(WORD), &dwHeaderSize, NULL);
		if ((bRet == FALSE) || (dwHeaderSize != sizeof(WORD)))
		{
			CloseHandle(hFile);
			return FALSE;
		}
		nCurLevel++;
	}

	/********** Level number **********/
	bRet = ReadFile(hFile, &clh.LevelNumber, sizeof(WORD), &dwHeaderSize, NULL);
	if ((bRet == FALSE) || (dwHeaderSize != sizeof(WORD)))
	{
		CloseHandle(hFile);
		return FALSE;
	}

	/********** Level Time Limit **********/
	bRet = ReadFile(hFile, &clh.TimeLimit, sizeof(WORD), &dwHeaderSize, NULL);
	if ((bRet == FALSE) || (dwHeaderSize != sizeof(WORD)))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	nTime = clh.TimeLimit;
	bNoLimit = (nTime == 0);

	/********** Number of Chips to Collect **********/
	bRet = ReadFile(hFile, &clh.ChipsToCollect, sizeof(WORD), &dwHeaderSize, NULL);
	if ((bRet == FALSE) || (dwHeaderSize != sizeof(WORD)))
	{
		CloseHandle(hFile);
		return FALSE;
	}

	nChips = clh.ChipsToCollect;

	/********** Map Detail (Top Level) **********/
	//Read Data Field Number
	bRet = ReadFile(hFile, &nField, sizeof(WORD), &dwHeaderSize, NULL);
	if ((bRet == FALSE) || (dwHeaderSize != sizeof(WORD) || (nField != 1)))
	{
		CloseHandle(hFile);
		return FALSE;
	}

	//Read Data Field Size
	bRet = ReadFile(hFile, &nFieldSize, sizeof(WORD), &dwHeaderSize, NULL);
	if ((bRet == FALSE) || (dwHeaderSize != sizeof(WORD)))
	{
		CloseHandle(hFile);
		return FALSE;
	}

	//Read Map Detail (Top Level)
	bRet = ReadFile(hFile, &map, nFieldSize, &dwHeaderSize, NULL);
	if ((bRet == FALSE) || (dwHeaderSize != nFieldSize))
	{
		CloseHandle(hFile);
		return FALSE;
	}

	//Store RLE mapdata in uncompressed form
	unsigned int nCurPos = 0, nCurByte = 0;
	int j;
	while(nCurByte < nFieldSize)
	{
		if(map[nCurByte] == 0xFF)
		{
			ID = map[(nCurByte + 2)];
			if(ID >= 0x0E && ID <= 0x11) ID += 0xC2;
			for(j=0; j<map[(nCurByte + 1)]; j++)
			{
				ptCell.x = nCurPos % 32;
				ptCell.y = int(nCurPos / 32);
				GetCell(ptCell)->GetTop()->SetID(ID);
				nCurPos++;
			}
			nCurByte += 3;
		}
		else
		{
			ID = map[nCurByte];
			if(ID >= 0x0E && ID <= 0x11) ID += 0xC2;
			ptCell.x = nCurPos % 32;
			ptCell.y = int(nCurPos / 32);
			GetCell(ptCell)->GetTop()->SetID(ID);
            nCurPos++;
            nCurByte++;
		}
	}

	/********** Map Detail (Bottom Level) **********/

	//Read Data Field Size
	bRet = ReadFile(hFile, &nFieldSize, sizeof(WORD), &dwHeaderSize, NULL);
	if ((bRet == FALSE) || (dwHeaderSize != sizeof(WORD)))
	{
		CloseHandle(hFile);
		return FALSE;
	}

	//Read Map Detail (Bottom Level)
	bRet = ReadFile(hFile, &map, nFieldSize, &dwHeaderSize, NULL);
	if ((bRet == FALSE) || (dwHeaderSize != nFieldSize))
	{
		CloseHandle(hFile);
		return FALSE;
	}

	//Store RLE mapdata in uncompressed form
	nCurPos = 0;
	nCurByte = 0;
	while(nCurByte < nFieldSize)
	{
		if(map[nCurByte] == 0xFF)
		{
			ID = map[(nCurByte + 2)];
			if(ID >= 0x0E && ID <= 0x11) ID += 0xC2;
			for(j=0; j<map[(nCurByte + 1)]; j++)
			{
				ptCell.x = nCurPos % 32;
				ptCell.y = int(nCurPos / 32);
				GetCell(ptCell)->GetBottom()->SetID(ID);
				nCurPos++;
			}
			nCurByte += 3;
		}
		else
		{
			ID = map[nCurByte];
			if(ID >= 0x0E && ID <= 0x11) ID += 0xC2;
			ptCell.x = nCurPos % 32;
			ptCell.y = int(nCurPos / 32);
			GetCell(ptCell)->GetBottom()->SetID(ID);
            nCurPos++;
            nCurByte++;
		}
	}

	//Copy Tile from Bottom Map to Top Map at the location of chip
	//TopMap.SetID(nChipX, nChipY, BottomMap.GetID(nChipX, nChipY));

	/********** Remaining LevelSize (in bytes) **********/
	bRet = ReadFile(hFile, &nRemainingLevelSize, sizeof(WORD), &dwHeaderSize, NULL);
	if ((bRet == FALSE) || (dwHeaderSize != sizeof(WORD)))
	{
		CloseHandle(hFile);
		return FALSE;
	}

	while(nRemainingLevelSize > 0)
	{
		//Read Data Field Number
		bRet = ReadFile(hFile, &nField, sizeof(BYTE), &dwHeaderSize, NULL);
		if ((bRet == FALSE) || (dwHeaderSize != sizeof(BYTE)))
		{
			CloseHandle(hFile);
			return FALSE;
		}
		nRemainingLevelSize--;

		//Read Data Field Size
		bRet = ReadFile(hFile, &nFieldSize, sizeof(BYTE), &dwHeaderSize, NULL);
		if ((bRet == FALSE) || (dwHeaderSize != sizeof(BYTE)))
		{
			CloseHandle(hFile);
			return FALSE;
		}
		nRemainingLevelSize--;
		nRemainingLevelSize -= nFieldSize;

		switch(nField)
		{
		case 3:
			/********** Level Title **********/
			bRet = ReadFile(hFile, &text, nFieldSize, &dwHeaderSize, NULL);
			if ((bRet == FALSE) || (dwHeaderSize != nFieldSize))
			{
				CloseHandle(hFile);
				return FALSE;
			}
			for(i=0;i<nFieldSize;i++)
				uctext[i] = text[i];
			SetTitle(uctext);
			break;
		case 7:
			/********** Level Hint **********/
			bRet = ReadFile(hFile, &text, nFieldSize, &dwHeaderSize, NULL);
			if ((bRet == FALSE) || (dwHeaderSize != nFieldSize))
			{
				CloseHandle(hFile);
				return FALSE;
			}
			for(i=0;i<nFieldSize;i++)
				uctext[i] = text[i];
			SetHint(uctext);
			break;
		case 6:
			/********** Level Password **********/
			bRet = ReadFile(hFile, &text, nFieldSize, &dwHeaderSize, NULL);
			if ((bRet == FALSE) || (dwHeaderSize != nFieldSize))
			{
				CloseHandle(hFile);
				return FALSE;
			}
			for(i=0;i<nFieldSize;i++)
			{
				uctext[i] = text[i] ^ 0x99;
			}
			SetPassword(uctext);
			break;
		case 4:
			/********** Bear Traps **********/
			nTrapCount = nFieldSize / 10;
			for(j=0; j<nTrapCount; j++)
			{
				bRet = ReadFile(hFile, &text, 10, &dwHeaderSize, NULL);
				if ((bRet == FALSE) || (dwHeaderSize != 10))
				{
					CloseHandle(hFile);
					return FALSE;
				}
				ptButton.x = text[0];
				ptButton.y = text[2];
				ptDevice.x = text[4];
				ptDevice.y = text[6];
				Traps[j].SetButton(ptButton);
				Traps[j].SetDevice(ptDevice);
			}
			break;
		case 5:
			/********** Clone Machines **********/
			nClonerCount = nFieldSize / 8;
			for(j=0; j<nClonerCount; j++)
			{
				bRet = ReadFile(hFile, &text, 8, &dwHeaderSize, NULL);
				if ((bRet == FALSE) || (dwHeaderSize != 8))
				{
					CloseHandle(hFile);
					return FALSE;
				}
				ptButton.x = text[0];
				ptButton.y = text[2];
				ptDevice.x = text[4];
				ptDevice.y = text[6];
				Cloners[j].SetButton(ptButton);
				Cloners[j].SetDevice(ptDevice);
			}
			break;
		case 10:
			/********** Moving Creatures **********/
			nCreatureCount = nFieldSize / 2;
			for(j=0; j<nCreatureCount; j++)
			{
				bRet = ReadFile(hFile, &text, 2, &dwHeaderSize, NULL);
				if ((bRet == FALSE) || (dwHeaderSize != 2))
				{
					CloseHandle(hFile);
					return FALSE;
				}
				creatures[j].x = text[0];
				creatures[j].y = text[1];
			}
			break;

		}
	}

	//wsprintf(text, _T("%03i"), strlen(text));

	CloseHandle(hFile);
	return TRUE;
}


CLevel::~CLevel()
{
}

LPCTSTR CLevel::GetPath(TCHAR *filename)
{
	int nStrLen;

	TCHAR *lpFilename;
	TCHAR *pDest;
	int result;
	int ch = '\\';

	lpFilename = new TCHAR[1024];
	nStrLen = GetModuleFileName(NULL, lpFilename, 1024);
	pDest = _tcsrchr(lpFilename, ch);
	if (pDest != NULL)
	{
		result = pDest - lpFilename + 1;
		lpFilename[result] = '\0';
		if (filename != NULL)
			_tcscat(lpFilename, filename);
		return lpFilename;
	}
	else
		return NULL;
}

void CLevel::ToggleBlocks()
{
	POINT ptTile;
	for(ptTile.x=0; ptTile.x<32; ptTile.x++)
	{
		for(ptTile.y=0; ptTile.y<32; ptTile.y++)
		{
			if(GetCell(ptTile)->GetTop()->GetID() == SwitchWall_Open)
				GetCell(ptTile)->GetTop()->SetID(SwitchWall_Closed);
			else if(GetCell(ptTile)->GetTop()->GetID() == SwitchWall_Closed)
				GetCell(ptTile)->GetTop()->SetID(SwitchWall_Open);
			if(GetCell(ptTile)->GetBottom()->GetID() == SwitchWall_Open)
				GetCell(ptTile)->GetBottom()->SetID(SwitchWall_Closed);
			else if(GetCell(ptTile)->GetBottom()->GetID() == SwitchWall_Closed)
				GetCell(ptTile)->GetBottom()->SetID(SwitchWall_Open);
		}
	}
}