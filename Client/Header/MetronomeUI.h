#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CCubeTex;
class CTransform;
class CCalculator;
class COrthoTransform;
class CRcTex;

END

enum START_POSION
{
	START_POSION_RIGHT,
	START_POSION_LEFT
};


class CMetronomeUI :public CGameObject
{

private:
	explicit CMetronomeUI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMetronomeUI(const CGameObject& rhs);
	virtual  ~CMetronomeUI();
public:
	HRESULT			Ready_Object(_float fPosX, _float fPosY, _float fSpeed, int iTexIndex);
	virtual _int	Update_Object(const _float& fTimeDelta) override;

	virtual	void	LateUpdate_Object(void);
	virtual void	Render_Obejct(void) override;


private:
	HRESULT			Add_Component(void);
public:
	void			init(_float fPosX, _float fPosY, _float fSpeed, int iTexIndex,_int iAlpha)
	{
		m_fMoveX = fPosX;
		m_fMoveY = fPosY;
		m_fSpeed = fSpeed;
		m_iTexIndex = iTexIndex;
		//m_fAlpha = iAlpha;
	}

public:
	void			Set_RenderOut(_bool bRenderOut) { m_bRenderOut = bRenderOut; }


public:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	COrthoTransform*	m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	
private:
	_float				m_fFrame = 0.f; // WinZero  도착시간
	_float				m_fMoveX = 0.f; // 움직이는 위치
	_float				m_fMoveY = 0.f; // 움직이는 위치
	_float				m_fSpeed = 0.f; // 방향 

	START_POSION		m_ePos;

	_float				m_fAlpha = 0.f;

	_bool				m_bRenderOut = false;

public:
	static CMetronomeUI*	Create(LPDIRECT3DDEVICE9 pGraphicDev,_float fPosX,_float fPosY,_float fSpeed, int iTexIndex);
	virtual void Free();
};

