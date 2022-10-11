#pragma once
#include "UI_Base.h"

BEGIN(Engine)

class CTexture;
class CCubeTex;
class CTransform;
class CCalculator;
class COrthoTransform;
class CRcTex;

END



class CMetronomeSmallUI : public CUI_Base
{
private:
	explicit CMetronomeSmallUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual  ~CMetronomeSmallUI();
	explicit CMetronomeSmallUI(const CUI_Base& rhs);
public:
	HRESULT			Ready_Object(_float fPosX, _float fPosY, _float fSpeed, int iTexIndex);
	virtual _int	Update_Object(const _float& fTimeDelta)	override;

	virtual	void	LateUpdate_Object(void);
	virtual void	Render_Obejct(void) override;

private:
	HRESULT			Add_Component(void);
public:
	void			init(_float fPosX, _float fPosY, _float fSpeed, int iTexIndex, _int iAlpha)
	{
		m_fMoveX = fPosX;
		m_fMoveY = fPosY;
		m_fSpeed = fSpeed;
		m_iTexIndex = iTexIndex;
		
	}


private:
	_float				m_fFrame = 0.f; // WinZero  도착시간
	_float				m_fMoveX = 0.f; // 움직이는 위치
	_float				m_fMoveY = 0.f; // 움직이는 위치
	_float				m_fSpeed = 0.f; // 방향 


	_float				m_fAlpha = 0.f;

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	COrthoTransform*	m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;



public:
	static CMetronomeSmallUI*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fPosX, _float fPosY, _float fSpeed, int iTexIndex);
	virtual void Free();


};

