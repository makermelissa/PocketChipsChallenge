#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <windows.h>

class CDevice
{
public:
	CDevice() {};
	~CDevice() {};
	POINT GetButton() { return Button; };
	POINT GetDevice() { return Device; };
	void SetButton(POINT ptButton) { Button = ptButton; };
	void SetDevice(POINT ptDevice) { Device = ptDevice; };
private:
	POINT Button;
	POINT Device;
};