#pragma once

#include "Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CInputDev :	public CBase
{
	DECLARE_SINGLETON(CInputDev)

private:
	explicit CInputDev();
	virtual ~CInputDev();

public:
	_byte		Get_DIKeyState(_ubyte byKeyID) { return m_byKeyState[byKeyID]; }
	_byte		Get_DIMouseState(MOUSEKEYSTATE byMouseID)
	{
		return m_MouseState.rgbButtons[byMouseID];
	}
	_long		Get_DIMouseMove(MOUSEMOVESTATE eMoveState)
	{
		return *(((long*)&m_MouseState) + eMoveState);
	}
	bool		Key_Pressing(int _iKey); //�ʿ���µ� �׳� ��������
	bool		Key_Down(int _byKeyID); //Ű������ true
	bool		Key_Up(int _iKey);		//���ǹ� ���� ����ħ

public:
	HRESULT			Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void			SetUp_InputDev(void);

private:
	_byte			m_byKeyState[256];
	DIMOUSESTATE	m_MouseState;

	LPDIRECTINPUTDEVICE8		m_pKeyBoard;
	LPDIRECTINPUTDEVICE8		m_pMouse;

	LPDIRECTINPUT8				m_pInputSDK;

	bool m_bKeyState[256]; //Ű����

public:
	virtual void		Free(void);
};

END