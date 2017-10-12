#include "..\..\GAPIDRAW\INCLUDE\GapiDraw.h"	// Added by ClassView
	
class CChipsApp : public CGapiApplication
{
public:
    CChipsApp(const GDAPPCONFIG& config) : CGapiApplication(config) {KeysAssigned = FALSE;};
	virtual HRESULT InitInstance();
	virtual HRESULT ExitInstance();
	virtual HRESULT CreateSurfaces(CGapiDisplay& display, HINSTANCE hInstance);
	virtual HRESULT ProcessNextFrame(CGapiSurface& backbuffer, DWORD dwFlags);
    virtual HRESULT KeyDown(DWORD dwKey, GDKEYLIST& keylist);
	virtual HRESULT KeyUp(DWORD dwKey, GDKEYLIST& keylist);
	virtual HRESULT StylusDown(POINT p);
	virtual HRESULT WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	CGapiInput *GetInput() { return &m_input; };
	CGameState *GetGameState() { return &m_GameState; };
	int GetKeystate(DWORD dwKey) { return KeyStates[dwKey]; };
    virtual ~CChipsApp() {};
	void ResetKeys();
	void SetKeyState(DWORD dwKey, BOOL down);
	void AdvanceKeys();
	void GetTile(POINT ptTile, CGapiSurface *Tile);
	int Input();
	void AssignKeys();
	void ResetKeyStates();
	void DisplayDialog(CGapiSurface* pSurface, int Width, int Height, int CenterX, int CenterY, TCHAR Text[256], int TransparencyFactor = 255, BOOL CenterVert = FALSE, BOOL CenterHorz = TRUE, DWORD BackColor = RGB(192,192,192), DWORD FontColor = RGB(0,0,0), int FontSize = 14, int BorderStyle = BS_OUTSET);
	void DisplayMenu(CGapiSurface* pSurface);
	void DisplayYesNo(CGapiSurface* pSurface);
	BOOL Ask(TCHAR Text[256]);
protected:
	CGapiSurface m_Title;
	CGapiSurface m_TileBack;
	CGapiSurface m_InfoWnd;
	CGapiSurface m_Background;
	CGapiSurface m_Numbers;
	CGapiSurface m_Tiles;
	CGapiTimer m_GameTimer;
	CGameState m_GameState;
	CGapiSurface m_Keyboard;
	CGapiSurface m_KeyboardMask;
	int KeyStates[256];
	DWORD Keys[8];
	BOOL KeysAssigned;
	long CurrentMenu;
	CJoJoSIP m_sip;
	TCHAR strCode[5];
	int nLevel;
	int nCurField;
	TCHAR strError[256];
	BOOL bFileDialog;
};

void CALLBACK GameProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
CChipsApp *pChipsApp;