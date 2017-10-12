#ifndef STRICT
#define STRICT
#endif

#define START_LEVEL 1

#include "GapiDraw.h"
#include "GapiDrawApplication.h"
#include "JoJoSip.h"
#include "defs.h"
#include "resource.h"
#include "gamestate.h"
#include "chips.h"
#include "fmod.h"
#include <windows.h>
#include <Commdlg.h>
#include <math.h>

HRESULT CChipsApp::StylusDown(POINT p)
{
	if(bFileDialog) return S_OK;
	RECT menuButton, screenRect;
	POINT ptDest, ptView;
	::SetRect(&menuButton, 192, 262, 216, 286);
	::SetRect(&screenRect, 12, 10, 228, 226);

	//Title Screen Click Locations
	RECT playButton, scoreButton, soundButton, exitButton;
	//fileButton,
	/*::SetRect(&playButton, 61, 134, 178, 160);
	::SetRect(&scoreButton, 56, 164, 183, 184);
	::SetRect(&soundButton, 86, 194, 153, 214);
	::SetRect(&fileButton, 64, 224, 173, 249);
	::SetRect(&exitButton, 97, 254, 143, 274);*/
	::SetRect(&playButton, 55, 159, 184, 187);
	::SetRect(&scoreButton, 52, 190, 190, 212);
	::SetRect(&soundButton, 84, 221, 157, 241);
	::SetRect(&exitButton, 97, 250, 146, 271);

	//Menu Click Locations
	RECT menuGame, menuLevel;
	RECT menuNew, menuPause, menuMain;
	RECT menuRestart, menuNext, menuPrev, menuGoTo;
	::SetRect(&menuGame, 1, 301, 40, 318);
	::SetRect(&menuLevel, 41, 301, 79, 318);

	::SetRect(&menuNew, 1, 257, 78, 271);
	::SetRect(&menuPause, 1, 272, 78, 285);
	::SetRect(&menuMain, 1, 286, 78, 300);

	::SetRect(&menuRestart, 42, 243, 103, 257);
	::SetRect(&menuNext, 42, 258, 103, 271);
	::SetRect(&menuPrev, 42, 272, 103, 285);
	::SetRect(&menuGoTo, 42, 286, 103, 300);

	//Question Button Click Locations
	RECT btnYes, btnNo;
	::SetRect(&btnYes, 71, 107, 108, 122);
	::SetRect(&btnNo, 131, 107, 168, 122);

	//Goto Level Screen
	RECT fldLevel, fldPassword, btnOK, btnCancel;
	::SetRect(&fldLevel, 123, 68, 196, 91);
	::SetRect(&fldPassword, 123, 123, 196, 146);
	::SetRect(&btnOK, 65, 176, 114, 193);
	::SetRect(&btnCancel, 125, 176, 174, 193);

	ptView = m_GameState.GetViewPort();
	switch(m_GameState.GetGameMode())
	{
		case GM_TITLE:
			if(PtInRect(&playButton, p))
			{
				if(!m_GameState.GetGameInProgress())
				{
					if(!m_GameState.SetLevel(m_GameState.GetCurrentLevel()->GetLevel()))
					{
						m_GameState.SetGameMode(GM_SHUTDOWN);
						return SetError(TEXT("Error loading Level. CHIPS.DAT may be missing."));
					}
				}
				else
					m_GameState.RevertGameMode();
				m_GameState.SetGameInProgress(TRUE);
			}
			else if(PtInRect(&scoreButton, p))
			{

			}
			else if(PtInRect(&soundButton, p))
			{

			}
			/*else if(PtInRect(&fileButton, p))
			{
				m_GameState.SetGameMode(GM_NONE);
				//m_display.SuspendDisplay();
				//ToggleFullscreen(GetForegroundWindow(), FALSE);
				OPENFILENAME lpOfn;
				BOOL bResult;
				TCHAR szFile[1024] = TEXT("\0");
				memset(&lpOfn, 0, sizeof(lpOfn));
				lpOfn.lStructSize = sizeof(OPENFILENAME);
				lpOfn.lpstrFilter = TEXT("Chips Data File (*.dat)\0*.dat\0");
				lpOfn.hwndOwner = GetForegroundWindow();
				lpOfn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
				lpOfn.lpstrFile = szFile;
				lpOfn.nMaxFile = 1024;
				bFileDialog = TRUE;
				bResult = GetOpenFileName(&lpOfn);
				if(bResult)
				{
					m_GameState.GetCurrentLevel()->SetFileName(lpOfn.lpstrFile);
					m_GameState.SetGameInProgress(FALSE);
					m_GameState.GetCurrentLevel()->SetLevel(START_LEVEL);
				}
				m_GameState.RevertGameMode();
				bFileDialog = FALSE;
				MessageBox(GetForegroundWindow(), TEXT("Open File"), TEXT("File"), 0);
				//m_display.ResumeDisplay();
				//ToggleFullscreen(GetForegroundWindow(), TRUE);
			}*/
			else if(PtInRect(&exitButton, p))
				Shutdown();
			break;
		case GM_NORMAL:
			if(PtInRect(&screenRect, p))
			{
				ptDest.x = int((p.x - 12) / 24) - 4 + ptView.x;
				ptDest.y = int((p.y - 12) / 24) - 4 + ptView.y;
				m_GameState.GetChip()->SetWalking(TRUE);
				m_GameState.GetChip()->SetDestination(ptDest);
			}
		case GM_PAUSED:
			if(PtInRect(&menuButton, p))
			{
				CurrentMenu = MENU_MAIN;
				m_GameState.SetGameMode(GM_MENU);
			}
			break;
		case GM_CHIPDEAD:
			if(PtInRect(&menuButton, p))
			{
				CurrentMenu = MENU_MAIN;
				m_GameState.SetGameMode(GM_MENU);
			}
			else
			{
				m_GameState.Reset();
				m_GameState.SetLevel(m_GameState.GetCurrentLevel()->GetLevel());
			}
			break;
		case GM_NEWGAME:
			if(PtInRect(&btnYes, p))
			{
				m_GameState.Reset();
				m_GameState.SetLevel(1);
			}
			else if(PtInRect(&btnNo, p))
			{
				m_GameState.RevertGameMode();
			}
			break;
		case GM_MENU:
			if((CurrentMenu & MENU_MASK2) == MENU_GAME)
			{
				if(PtInRect(&menuNew, p))
				{
					m_GameState.SetGameMode(GM_NEWGAME);
					return S_OK;
				}
				else if(PtInRect(&menuPause, p))
				{
					m_GameState.SetGameMode(GM_PAUSED);
					return S_OK;
				}
				else if(PtInRect(&menuMain, p))
				{
					m_GameState.SetGameMode(GM_TITLE);
					return S_OK;
				}
			}
			else if((CurrentMenu & MENU_MASK2) == MENU_LEVEL)
			{
				if(PtInRect(&menuRestart, p))
				{
					m_GameState.Reset();
					m_GameState.SetLevel(m_GameState.GetCurrentLevel()->GetLevel());
					return S_OK;
				}
				else if(PtInRect(&menuPrev, p))
				{
					m_GameState.Reset();
					m_GameState.DecLevel();
					return S_OK;
				}
				else if(PtInRect(&menuNext, p))
				{
					m_GameState.Reset();
					m_GameState.IncLevel();
					return S_OK;
				}
				else if(PtInRect(&menuGoTo, p))
				{
					m_GameState.SetGameMode(GM_LEVELGOTO);
					nLevel = m_GameState.GetCurrentLevel()->GetLevel();
					nCurField = FLD_PASSWORD;
					CJoJoSIP::GetSingleton().SetVisible(TRUE);
					_tcscpy(strCode,TEXT(""));
					_tcscpy(strError,TEXT(""));
					return S_OK;
				}
			}
			if(PtInRect(&menuGame, p))
			{
				if((CurrentMenu & MENU_MASK2) == MENU_GAME)
					CurrentMenu = MENU_MAIN;
				else
					CurrentMenu = MENU_GAME | MENU_MAIN;
			}
			else if(PtInRect(&menuLevel, p))
			{
				if((CurrentMenu & MENU_MASK2) == MENU_LEVEL)
					CurrentMenu = MENU_MAIN;
				else
					CurrentMenu = MENU_LEVEL | MENU_MAIN;
			}
			else
				m_GameState.RevertGameMode();
			break;
		case GM_LEVELGOTO:
			TCHAR t, strLevel[10];
			if(CJoJoSIP::GetSingleton().GetKey(t, p) == JOJOSIPINFO_KEYPRESSED)
			{
				if(nCurField == FLD_LEVEL)
				{
					wsprintf(strLevel, TEXT("%01i"), nLevel);
					if((int)t == 8)
					{
						if (_tcslen(strLevel) > 0)
							strLevel[_tcslen(strLevel) - 1] = '\0';
					}
					else if(_tcslen(strLevel) < 3 && (int)t >= KB_0 && (int)t <= KB_9)
					{
						strLevel[_tcslen(strLevel) + 1] = '\0';
						strLevel[_tcslen(strLevel)] = t;
					}
					nLevel = 0;
					for(int i=0; (unsigned int)i<_tcslen(strLevel); i++)
						nLevel += (((unsigned int)strLevel[i]) - KB_0) * (int)pow(10, (_tcslen(strLevel) - 1) - i);
				}
				else if(nCurField == FLD_PASSWORD)
				{
					if((int)t == 8)
					{
						if (_tcslen(strCode) > 0)
							strCode[_tcslen(strCode) - 1] = '\0';
					}
					else if(_tcslen(strCode) < 4 && (((int)t >= KB_a && (int)t <= KB_z) || ((int)t >= KB_A && (int)t <= KB_Z)))
					{
						strCode[_tcslen(strCode) + 1] = '\0';
						strCode[_tcslen(strCode)] = t;
						wsprintf(strCode, _tcsupr(strCode));
					}
				}
			}
			else if(PtInRect(&fldLevel, p))
				nCurField = FLD_LEVEL;
			else if(PtInRect(&fldPassword, p))
				nCurField = FLD_PASSWORD;
			else if(PtInRect(&btnOK, p))
			{
				if(nLevel <= m_GameState.GetCurrentLevel()->GetLastLevel())
				{
					CJoJoSIP::GetSingleton().SetVisible(FALSE);
					m_GameState.RevertGameMode();
					m_GameState.Reset();
					m_GameState.SetLevel(nLevel);
					return S_OK;
				}
				else
					wsprintf(strError, TEXT("Level does not exist!!!"));
			}
			else if(PtInRect(&btnCancel, p))
			{
				CJoJoSIP::GetSingleton().SetVisible(FALSE);
				m_GameState.RevertGameMode();
			}
			break;
	}
	return S_OK;
}

void CChipsApp::ResetKeyStates()
{
	int i;
	int KeyboardTrans[8] = {
		/* KS_OFF			=> */ KS_OFF,
		/* KS_ON			=> */ KS_ON,
		/* KS_DOWN			=> */ KS_DOWN,
		/* KS_STRUCK		=> */ KS_OFF,
		/* KS_PRESSED		=> */ KS_DOWNBUTOFF1,
		/* KS_DOWNBUTOFF1	=> */ KS_DOWNBUTOFF2,
		/* KS_DOWNBUTOFF2	=> */ KS_DOWN,
		/* KS_REPEATING		=> */ KS_DOWN
	};

	for(i=0; i<8; i++)
		KeyStates[Keys[i]] = KeyboardTrans[KeyStates[Keys[i]]];
}

void CChipsApp::AssignKeys()
{
	GDKEYLIST keylist;
	m_input.GetKeyList(&keylist);

	//Assign Keys for easier translation
	Keys[UP] = keylist.vkUp;
	Keys[LEFT] = keylist.vkLeft;
	Keys[DOWN] = keylist.vkDown;
	Keys[RIGHT] = keylist.vkRight;
	Keys[START] = keylist.vkStart;
	Keys[BUTTON_A] = keylist.vkA;
	Keys[BUTTON_B] = keylist.vkB;
	Keys[BUTTON_C] = keylist.vkC;
	KeysAssigned = TRUE;
}

void CChipsApp::ResetKeys()
{
	int i;
	if(!KeysAssigned)
		return;

	//Set each key to off
	for(i=0; i<8; i++)
		KeyStates[Keys[i]] = KS_OFF;
}

void CChipsApp::SetKeyState(DWORD dwKey, BOOL down)
{
	GDKEYLIST keylist;
	pChipsApp->GetInput()->GetKeyList(&keylist);
	if(down)
		KeyStates[dwKey] = KeyStates[dwKey] == KS_OFF ? KS_PRESSED : KS_REPEATING;
	else
		KeyStates[dwKey] = KeyStates[dwKey] == KS_PRESSED ? KS_STRUCK : KS_OFF;
}

int CChipsApp::Input()
{
	GDKEYLIST keylist;
	m_input.GetKeyList(&keylist);
	int i;

	for(i=0; i<8; i++)
	{
		if(KeyStates[Keys[i]] == KS_PRESSED || KeyStates[Keys[i]] == KS_DOWN)
			return i;
		else if(KeyStates[Keys[i]] == KS_STRUCK || KeyStates[Keys[i]] == KS_REPEATING)
			return i;
	}
	return NONE;
}

HRESULT CChipsApp::KeyDown(DWORD dwKey, GDKEYLIST& keylist)
{
	KeyStates[dwKey] = (KeyStates[dwKey] == KS_OFF) ? KS_PRESSED : KS_REPEATING;
	//int i;

	//for(i=0; i<4; i++)
		//if(dwKey != Keys[i]) KeyStates[Keys[i]] = KS_OFF;
	//SetKeyState(dwKey, TRUE);

    return S_OK;
}

HRESULT CChipsApp::KeyUp(DWORD dwKey, GDKEYLIST& keylist)
{
	KeyStates[dwKey] = (KeyStates[dwKey] == KS_PRESSED) ? KS_STRUCK : KS_OFF;
	//SetKeyState(dwKey, FALSE);
	return S_OK;
}


void CALLBACK GameProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	CGameState *pGameState;
	pGameState = pChipsApp->GetGameState();
	int cmd;
	if(pGameState->GetGameMode() == GM_SHUTDOWN)
		pChipsApp->Shutdown();
	cmd = pChipsApp->Input();
	pChipsApp->ResetKeyStates();
	switch(pGameState->GetGameMode())
	{
		case GM_NORMAL:
			if(cmd == PAUSE)
			{
				pGameState->SetGameMode(GM_PAUSED);
				break;
			}
			pGameState->AdvanceGame(cmd);
			break;
		case GM_CHIPDEAD:
			if(cmd == PAUSE)
			{
				pGameState->SetGameMode(GM_PAUSED);
				break;
			}
			else if(cmd != NONE)
			{
				pGameState->Reset();
				pGameState->SetLevel(pGameState->GetCurrentLevel()->GetLevel());
			}
			break;
		case GM_PAUSED:
			if(cmd == PAUSE)
				pGameState->RevertGameMode();
			break;
		case GM_LEVELWON:
			pGameState->Reset();
			pGameState->IncLevel();
			break;
	}
}


HRESULT CChipsApp::InitInstance()
{
	SetTimer(GetForegroundWindow(), GAME_TIMER, SECOND_LENGTH / TICKS_PER_SECOND, (TIMERPROC)GameProc);
	AssignKeys();
	ResetKeys();
	m_GameState.GetCurrentLevel()->SetLevel(START_LEVEL);
	m_GameState.SetGameMode(GM_TITLE);
	m_GameState.SetGameInProgress(FALSE);
	_tcscpy(strCode,TEXT(""));
	_tcscpy(strError,TEXT(""));
	bFileDialog = FALSE;
	return S_OK;
}


HRESULT CChipsApp::ExitInstance()
{
	KillTimer(GetForegroundWindow(), GAME_TIMER);
	return S_OK;
}


HRESULT CChipsApp::ProcessNextFrame(CGapiSurface& backbuffer, DWORD dwFlags)
{
	if(m_GameState.GetGameMode() == GM_NONE) return S_OK;
	RECT srcRect, destRect;
	DWORD dwXCenter, dwYCenter;
	int nNumColor, nOffSet, i, nCalcNum;
	int j, x, y;
	CGapiSurface Tile;
	POINT ptTile, ptView;
	TCHAR msgDeath[256], msgHint[256];

	// Clear the back buffer
	backbuffer.FillRect(NULL, RGB(128, 128, 128), 0, NULL);

	if(m_GameState.GetGameMode() == GM_TITLE)
	{
		//Draw Title Screen
		::SetRect(&srcRect, 0, 0, 
			m_Title.GetWidth(), m_Title.GetHeight());
		backbuffer.BltFast(0, 0, &m_Title, &srcRect,
		0, NULL);
	}
	if((m_GameState.GetGameMode() == GM_NORMAL) ||
		(m_GameState.GetGameMode() == GM_CHIPDEAD) ||
		(m_GameState.GetGameMode() == GM_LOADING) ||
		(m_GameState.GetGameMode() == GM_LEVELWON) ||
		(m_GameState.GetGameMode() == GM_PAUSED) ||
		(m_GameState.GetGameMode() == GM_MENU) ||
		(m_GameState.GetGameMode() == GM_PAUSED) ||
		(m_GameState.GetGameMode() == GM_GAMEWON) ||
		(m_GameState.GetGameMode() == GM_LEVELGOTO) ||
		(m_GameState.GetGameMode() == GM_NEWGAME))
	{

		//Draw Background
		::SetRect(&srcRect, 0, 0, 
			m_Background.GetWidth(), m_Background.GetHeight());
		backbuffer.BltFast(0, 0, &m_Background, &srcRect,
		0, NULL);

		//Draw Tile Background
		::SetRect(&srcRect, 0, 0, 
			m_TileBack.GetWidth(), m_TileBack.GetHeight());
		backbuffer.BltFast(6, 4, &m_TileBack, &srcRect,
		0, NULL);

		//Draw Information Window
		::SetRect(&srcRect, 0, 0, 
			m_InfoWnd.GetWidth(), m_InfoWnd.GetHeight());
		backbuffer.BltFast(6, 234, &m_InfoWnd, &srcRect,
		0, NULL);

		//Draw Level Number
		nCalcNum = m_GameState.GetCurrentLevel()->GetLevel();
		for(i=0; i<3; i++)
		{
			nNumColor = 276;
			if(((m_GameState.GetCurrentLevel()->GetLevel() < 100) && (i == 0)) || ((m_GameState.GetCurrentLevel()->GetLevel() < 10) && (i == 1)))
				nOffSet = 23;
			else
			{
				nOffSet = (11 - int(nCalcNum / pow(10,2-i))) * 23;
				nCalcNum -= int(int(nCalcNum / pow(10,2-i)) * pow(10,2-i));
			}
			::SetRect(&srcRect, 0, 0 + nNumColor + nOffSet, 15, 23 + nNumColor + nOffSet);
			::SetRect(&destRect, 30 + (i * 15), 262, 45 + (i * 15), 285);
			backbuffer.Blt(&destRect, &m_Numbers, &srcRect, 0, NULL);	
		}

		//Draw Time Left
		int TimeLeft;
		nCalcNum = TimeLeft = int(ceil((double)(m_GameState.GetTimeLimit() - m_GameState.GetTick()) / TICKS_PER_SECOND));
		for(i=0; i<3; i++)
		{
			if(TimeLeft <= 15)
				nNumColor = 0;
			else
				nNumColor = 276;
			if(!m_GameState.GetTimeLimit())
				nOffSet = 0;
			else if(((TimeLeft < 100) && (i == 0)) || ((TimeLeft < 10) && (i == 1)))
				nOffSet = 23;
			else
				nOffSet = (11 - int(nCalcNum / pow(10,2-i))) * 23;
			nCalcNum -= int(int(nCalcNum / pow(10,2-i)) * pow(10,2-i));
			::SetRect(&srcRect, 0, 0 + nNumColor + nOffSet, 15, 23 + nNumColor + nOffSet);
			::SetRect(&destRect, 85 + (i * 15), 262, 100 + (i * 15), 285);
			backbuffer.Blt(&destRect, &m_Numbers, &srcRect, 0, NULL);	
		}

		//Draw Chips Left
		nCalcNum = m_GameState.GetChipsNeeded();
		for(i=0; i<3; i++)
		{
			if(m_GameState.GetChipsNeeded() <= 0)
				nNumColor = 0;
			else
				nNumColor = 276;

			if(((m_GameState.GetChipsNeeded() < 100) && (i == 0)) || ((m_GameState.GetChipsNeeded() < 10) && (i == 1)))
				nOffSet = 23;
			else
			{
				nOffSet = (11 - int(nCalcNum / pow(10,2-i))) * 23;
				nCalcNum -= int(int(nCalcNum / pow(10,2-i)) * pow(10,2-i));
			}
			::SetRect(&srcRect, 0, 0 + nNumColor + nOffSet, 15, 23 + nNumColor + nOffSet);
			::SetRect(&destRect, 140 + (i * 15), 262, 155 + (i * 15), 285);
			backbuffer.Blt(&destRect, &m_Numbers, &srcRect, 0, NULL);	
		}

		//Draw Keys
		for(j=0; j<=3; j++)
		{
			if(m_GameState.GetKey(j))
			{
				x = 144;
				y = (((5 - j) % 4) + 4) * 24; 
				::SetRect(&srcRect, x , y, x + 24, y + 24);
				x = (j * 24) + 24;
				y = 289;
				::SetRect(&destRect, x , y, x + 24, y + 24);
				backbuffer.Blt(&destRect, &m_Tiles, &srcRect, 0, NULL);	
			}
		}

		//Draw Boots
		for(j=0; j<=3; j++)
		{
			if(m_GameState.GetBoots(j))
			{
				switch(j)
				{
				case WaterBoots:
					y = 0;
					break;
				case FireBoots:
					y = 1;
					break;
				case IceBoots:
					y = 2;
					break;
				case SuctionBoots:
					y = 3;
					break;
				}
				x = 144;
				y = (y + 8) * 24;
				::SetRect(&srcRect, x , y, x + 24, y + 24);
				x = (j * 24) + 120;
				y = 289;
				::SetRect(&destRect, x , y, x + 24, y + 24);
				backbuffer.Blt(&destRect, &m_Tiles, &srcRect, 0, NULL);	
			}
		}
	}
	if((m_GameState.GetGameMode() == GM_NORMAL)
		|| (m_GameState.GetGameMode() == GM_CHIPDEAD)
		|| (m_GameState.GetGameMode() == GM_MENU)
		|| (m_GameState.GetGameMode() == GM_NEWGAME))
	{
	
		ptView = m_GameState.GetViewPort();
		Tile.CreateSurface(GDCREATESURFACE_CLEAR, 24, 24);
		for(ptTile.x=(ptView.x - 4); ptTile.x <= (ptView.x + 4); ptTile.x++)
		{
			for(ptTile.y=(ptView.y - 4); ptTile.y <= (ptView.y + 4); ptTile.y++)
			{
				::SetRect(&srcRect, 0, 0, 24, 24);
				//Tile.FillRect(&srcRect, RGB(255,0,0), 0, NULL);
				GetTile(ptTile, &Tile);
				x = ((ptTile.x - (ptView.x - 4)) * 24) + 12;
				y = ((ptTile.y - (ptView.y - 4)) * 24) + 10;
				backbuffer.BltFast(x, y, &Tile, &srcRect, 0, NULL);	
			}
		}
		//Draw Hint Text if necessary
		if(m_GameState.GetStatus() & SF_SHOWHINT)
		{
			wsprintf(msgHint, m_GameState.GetCurrentLevel()->GetHint());
			DisplayDialog(&backbuffer, 228, 84, backbuffer.GetWidth()>>1, 276, msgHint, 192, FALSE, TRUE, RGB(0,0,0), RGB(255,255,0));
		}
		//Display Debug Information
		//wsprintf(msgHint, "CreatureCount: %02i", m_GameState.GetCreatureCount());
		//DisplayDialog(&backbuffer, 240, 20, 120, 270, msgHint, 128);
		//wsprintf(msgHint, "BlockCount: %02i", m_GameState.GetBlockCount());
		//DisplayDialog(&backbuffer, 240, 20, 120, 290, msgHint, 128);
		//wsprintf(msgHint, "SlipCount: %02i", m_GameState.GetSlipCount());
		//DisplayDialog(&backbuffer, 240, 20, 120, 310, msgHint, 128);
	}
	//Display Debug Information
	wsprintf(msgHint, TEXT("GameMode: %02i"), m_GameState.GetGameMode());
	DisplayDialog(&backbuffer, 240, 20, 120, 310, msgHint, 128);
	if(m_GameState.GetGameMode() == GM_PAUSED)
	{
		//Draw Paused Screen
		RECT rectPause;
		::SetRect(&rectPause, 12, 10, 228, 226);
		wsprintf(msgHint, TEXT("Paused"));
		DisplayDialog(&backbuffer, 216, 216, backbuffer.GetWidth()>>1, 118, msgHint, 255, TRUE, TRUE, RGB(0,0,0), RGB(255,0,0), 36, BS_NONE);
	}
	else if(m_GameState.GetGameMode() == GM_LOADING)
	{
		//Display Loading Message
		dwXCenter = (backbuffer.GetWidth()>>1);
		dwYCenter = (backbuffer.GetHeight()>>1);
		backbuffer.DrawText(dwXCenter, 118, TEXT("Loading Level Data..."), RGB(255, 255, 255),
			GDDRAWTEXT_CENTER | GDDRAWTEXT_BORDER, 0, NULL);
	}
	else if(m_GameState.GetGameMode() == GM_CHIPDEAD)
	{
		//Draw Death Dialog and make translucent
		if((m_GameState.GetStatus() & SF_CHIPNOTOKAY) == SF_CHIPDROWNED)
			wsprintf(msgDeath, TEXT("Ooops! Chip can't swim without flippers!"));
		else if((m_GameState.GetStatus() & SF_CHIPNOTOKAY) == SF_CHIPBURNED)
			wsprintf(msgDeath, TEXT("Ooops! Don't step in the fire without fire boots!"));
		else if((m_GameState.GetStatus() & SF_CHIPNOTOKAY) == SF_CHIPBOMBED)
			wsprintf(msgDeath, TEXT("Ooops! Don't touch the bombs!"));
		else if((m_GameState.GetStatus() & SF_CHIPNOTOKAY) == SF_CHIPHIT)
			wsprintf(msgDeath, TEXT("Ooops! Look out for creatures!"));
		else if((m_GameState.GetStatus() & SF_CHIPNOTOKAY) == SF_CHIPBLOCKHIT)
			wsprintf(msgDeath, TEXT("Ooops! Look out for moving blocks!"));
		else if((m_GameState.GetStatus() & SF_CHIPNOTOKAY) == SF_CHIPTIMEUP)
			wsprintf(msgDeath, TEXT("Ooops! Out of time!"));
		DisplayDialog(&backbuffer, 200, 40, backbuffer.GetWidth()>>1, 92, msgDeath, 216);
	}
	else if(m_GameState.GetGameMode() == GM_LEVELGOTO)
	{
		//Level Select Screen
		TCHAR strLabels[256], strLevel[10], strPW[5];
		wsprintf(strLabels, TEXT("\n\n\n         Level:"));
		_tcscat(strLabels, TEXT("\n\n\n  Password:"));
		wsprintf(strLevel, TEXT("%01i"), nLevel);
		wsprintf(strPW, strCode);
		if(nCurField == FLD_LEVEL)
			_tcscat(strLevel, TEXT("|"));
		else if(nCurField == FLD_PASSWORD)
			_tcscat(strPW, TEXT("|"));
		RECT rectPause;
		::SetRect(&rectPause, 12, 10, 228, 226);
		DisplayDialog(&backbuffer, 216, 216, backbuffer.GetWidth()>>1, 118, strLabels, 255, FALSE, FALSE, RGB(0,0,0), RGB(255,0,0), 20, BS_NONE);
		DisplayDialog(&backbuffer, 150, 50, backbuffer.GetWidth()>>1, 40, strError, 255, FALSE, TRUE, RGB(0,0,0), RGB(255,255,0), 14, BS_NONE);

		//Display Fields
		DisplayDialog(&backbuffer, 78, 25, (backbuffer.GetWidth()>>1) + 40, 80, strLevel, 255, TRUE, FALSE, RGB(255,255,255), RGB(0,0,0), 20, BS_INSET);
		DisplayDialog(&backbuffer, 78, 25, (backbuffer.GetWidth()>>1) + 40, 135, strPW, 255, TRUE, FALSE, RGB(255,255,255), RGB(0,0,0), 20, BS_INSET);

		//Display OK and Cancel Button
		DisplayDialog(&backbuffer, 50, 18, (backbuffer.GetWidth()>>1) - 30, 185, TEXT("OK"), 255);
		DisplayDialog(&backbuffer, 50, 18, (backbuffer.GetWidth()>>1) + 30, 185, TEXT("Cancel"), 255);
	}
	if(m_GameState.GetGameMode() == GM_MENU)
		DisplayMenu(&backbuffer);
	else if(m_GameState.GetGameMode() == GM_NEWGAME)
		DisplayYesNo(&backbuffer);
	CJoJoSIP::GetSingleton().Blt(&backbuffer);
    return S_OK;
};

void CChipsApp::GetTile(POINT ptTile, CGapiSurface *Tile)
{
	RECT srcRect;
	::SetRect(&srcRect, 0, 0, 24, 24);
	int xSrc, ySrc, nTopTile, nBottomTile;

	//Bottom Layer
	nBottomTile = m_GameState.GetCurrentLevel()->GetCell(ptTile)->GetBottom()->GetID();
	if(nBottomTile >= 0xD0 && nBottomTile <= 0xD3) nBottomTile -= 0xC2;
	xSrc = (int(nBottomTile / 16)) * 24;
	ySrc = (nBottomTile % 16) * 24;
	::SetRect(&srcRect, xSrc , ySrc, xSrc + 24, ySrc + 24);
	Tile->BltFast(0, 0, &m_Tiles, &srcRect, 0, NULL);

	//Top Layer
	nTopTile = m_GameState.GetCurrentLevel()->GetCell(ptTile)->GetTop()->GetID();
	if(nTopTile >= 0xD0 && nTopTile <= 0xD3) nTopTile -= 0xC2;
	if((nTopTile > 0x40) && (nBottomTile != Empty)) nTopTile += 48;
	xSrc = (int(nTopTile / 16)) * 24;
	ySrc = (nTopTile % 16) * 24;
	::SetRect(&srcRect, xSrc , ySrc, xSrc + 24, ySrc + 24);
	Tile->BltFast(0, 0, &m_Tiles, &srcRect, GDBLT_KEYSRC, NULL);
}

HRESULT CChipsApp::CreateSurfaces(CGapiDisplay& display, HINSTANCE hInstance)
{
	m_Title.CreateSurface(hInstance, IDB_TITLE);
	m_Background.CreateSurface(hInstance, IDB_BACKGROUND);
	m_TileBack.CreateSurface(hInstance, IDB_TILEBACK);
	m_InfoWnd.CreateSurface(hInstance, IDB_INFOWND);
	m_Numbers.CreateSurface(hInstance, IDB_NUMBERS);
	m_Tiles.CreateSurface(hInstance, IDB_TILES);
	m_Tiles.SetColorKey(RGB(166, 202, 240));
	m_Keyboard.CreateSurface(hInstance, IDB_KEYBOARD);
	m_Keyboard.SetColorKey(RGB(255, 0, 0));
	m_KeyboardMask.CreateSurface(hInstance, IDB_KEYBOARDMASK);

	// The standard keyboard will be used
    CJoJoSIP::GetSingleton().CreateKeyboard(&m_Keyboard, &m_KeyboardMask);

	SIPTRANSITION kbTrans;
	POINT kbPos;

	kbTrans.dwFadeInDelay = 3;
	kbTrans.dwFadeOutDelay = 3;
	kbPos.x = 0;
	kbPos.y = 320 - m_Keyboard.GetHeight();

	CJoJoSIP::GetSingleton().SetTransition( SIP_TRANSITION_FADE_IN_OPACITY |
                     SIP_TRANSITION_FADE_OUT_OPACITY |
                     SIP_TRANSITION_FADE_IN_BOTTOM | 
                     SIP_TRANSITION_FADE_OUT_BOTTOM, &kbTrans);
	CJoJoSIP::GetSingleton().SetPosition(kbPos);
    return S_OK;
}

void CChipsApp::DisplayDialog(CGapiSurface* pSurface, int Width, int Height, int CenterX, int CenterY, TCHAR Text[256], int TransparencyFactor, BOOL CenterVert, BOOL CenterHorz, DWORD BackColor, DWORD FontColor, int FontSize, int BorderStyle)
{
	DWORD BorderColorUL, BorderColorBR;
	RECT srcRect, dlgRect;
	CGapiSurface m_Dialog;
	GDBLTFASTFX bltFastFX;
	RECT txtRect;
	HDC hdc;
	int Flags = DT_WORDBREAK;
	LOGFONT lf = {FontSize, 0, 0, 0, FW_BOLD, 0, 0, 0,
				ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("TAHOMA")};
	HFONT hOldFont, hFont;

	::SetRect(&dlgRect, CenterX-(Width>>1), CenterY-(Height>>1), CenterX+(Width>>1), CenterY+(Height>>1));
	m_Dialog.CreateSurface(GDCREATESURFACE_CLEAR, (dlgRect.right - dlgRect.left), (dlgRect.bottom - dlgRect.top));
	::SetRect(&srcRect, 0, 0, m_Dialog.GetWidth(), m_Dialog.GetHeight());
	m_Dialog.FillRect(&srcRect, BackColor, 0, NULL);
	if(BorderStyle != BS_NONE)
	{
		switch(BorderStyle)
		{
			case BS_OUTSET:
				BorderColorUL = RGB(255, 255, 255);
				BorderColorBR = RGB(128, 128, 128);
				break;
			case BS_INSET:
				BorderColorUL = RGB(128, 128, 128);
				BorderColorBR = RGB(192, 192, 192);
				break;
			default:
				BorderColorUL = RGB(255, 255, 255);
				BorderColorBR = RGB(128, 128, 128);
				break;
		}
		m_Dialog.DrawLine(srcRect.left, srcRect.top, srcRect.right - 1, srcRect.top, BorderColorUL, 0, NULL);
		m_Dialog.DrawLine(srcRect.left, srcRect.top, srcRect.left, srcRect.bottom - 1, BorderColorUL, 0, NULL);
		m_Dialog.DrawLine(srcRect.right - 1, srcRect.top, srcRect.right - 1, srcRect.bottom - 1, BorderColorBR, 0, NULL);
		m_Dialog.DrawLine(srcRect.left, srcRect.bottom - 1, srcRect.right - 1, srcRect.bottom - 1, BorderColorBR, 0, NULL);
	}
	bltFastFX.dwOpacity = TransparencyFactor;
	m_Dialog.GetDC(&hdc);
	hFont = CreateFontIndirect(&lf), hOldFont;
	hOldFont = (HFONT)SelectObject(hdc, hFont);
	::SetRect(&txtRect, srcRect.left + 2, srcRect.top + 2, srcRect.right - 2, srcRect.bottom - 2);
	SetTextColor(hdc,FontColor);
	SetBkColor(hdc, BackColor);
	if(CenterHorz) Flags |= DT_CENTER;
	if(CenterVert) Flags |= (DT_VCENTER | DT_SINGLELINE);
	DrawText(hdc, Text, -1, &txtRect, Flags);
	SelectObject(hdc, hOldFont);
	m_Dialog.ReleaseDC(hdc);
	pSurface->BltFast(dlgRect.left, dlgRect.top, &m_Dialog, &srcRect, GDBLTFAST_OPACITY, &bltFastFX);
}

#define MENUHEIGHT 20
void CChipsApp::DisplayMenu(CGapiSurface* pSurface)
{
	TCHAR MenuString[50];
	RECT Level2Menu;
	if(CurrentMenu & MENU_MASK1)
	{
		wsprintf(MenuString, TEXT(" Game   Level"));
		DisplayDialog(pSurface, pSurface->GetWidth(), MENUHEIGHT, pSurface->GetWidth() >> 1, pSurface->GetHeight() - (MENUHEIGHT / 2), MenuString, 255, FALSE, FALSE);
	}
	if(CurrentMenu & MENU_MASK2)
	{
		switch(CurrentMenu & MENU_MASK2)
		{
			case MENU_GAME:
				wsprintf(MenuString, TEXT(" New Game\n Pause\n Main Menu"));
				::SetRect(&Level2Menu, 0, 256, 80, 302);
				break;
			case MENU_LEVEL:
				wsprintf(MenuString, TEXT(" Restart\n Next\n Previous\n Go To..."));
				::SetRect(&Level2Menu, 41, 242, 105, 302);
				break;
		}
		DisplayDialog(pSurface, (Level2Menu.right - Level2Menu.left), (Level2Menu.bottom - Level2Menu.top), Level2Menu.right - ((Level2Menu.right - Level2Menu.left) / 2), Level2Menu.bottom - ((Level2Menu.bottom - Level2Menu.top) / 2), MenuString, 255, FALSE, FALSE);
	}

}

void CChipsApp::DisplayYesNo(CGapiSurface* pSurface)
{
	DisplayDialog(pSurface, 200, 70, pSurface->GetWidth()>>1, 92, TEXT("Start new game, reset all level scores, and remove all passwords?"), 255);
	DisplayDialog(pSurface, 40, 18, (pSurface->GetWidth()>>1) - 30, 115, TEXT("Yes"), 255);
	DisplayDialog(pSurface, 40, 18, (pSurface->GetWidth()>>1) + 30, 115, TEXT("No"), 255);
}

HRESULT CChipsApp::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_KILLFOCUS:
			if(!bFileDialog)
			{
				m_GameState.SetGameMode(GM_PAUSED);
				m_display.SuspendDisplay();
				m_input.CloseInput();
			}
			break;
		case WM_ACTIVATE:
			if(wParam != WA_INACTIVE && !bFileDialog)
			{
				m_input.OpenInput();
				m_display.ResumeDisplay();
				ResetKeys();
				m_GameState.RevertGameMode();
			}
			break;
	}
	return CGapiApplication::WindowProc(hWnd,msg,wParam,lParam);
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
                     LPTSTR pCmdLine, int nCmdShow)
{
    GDAPPCONFIG config;
    ::ZeroMemory(&config, sizeof(GDAPPCONFIG));

    config.hInstance      = hInst;
    config.pAppTitle      = TEXT("Pocket Chip's Challenge");
    config.dwTargetFPS    = 20;
    config.dwWindowIcon   = IDI_APP;
    config.dwDisplayMode  = GDDISPMODE_NORMAL;

    // Create the application
    CChipsApp ChipsApp(config);

	pChipsApp = &ChipsApp;
    // Start main loop
    return ChipsApp.Run();
}
