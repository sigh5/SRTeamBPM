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


class CMetronomeRect :public CUI_Base
{
private:
	explicit CMetronomeRect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual  ~CMetronomeRect();

public:
	HRESULT			Ready_Object(_int iTexIndex);
	virtual _int	Update_Object(const _float& fTimeDelta)	override;

	virtual	void	LateUpdate_Object(void);
	virtual void	Render_Obejct(void) override;

	void			init(_int iTexIndex, _int iAlpha)
	{
		m_iTexIndex = iTexIndex;
		m_iAlpha = iAlpha;
	}



private:
	HRESULT			Add_Component(void);

private:
	_float				m_fFrame = 0.f; // WinZero  도착시간
	_float				m_fMoveX = 0.f; // 움직이는 위치
	_float				m_fMoveY = 0.f; // 움직이는 위치
	_float				m_fSpeed = 0.f; // 방향 

	_float				m_fSizeX=100.f;
	_float				m_fSizeY =100.f;
	_int				m_iAlpha = 150;

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	COrthoTransform*	m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;

private:
	_vec3 m_vScale = { 2.f,2.f,2.f };

public:
	static CMetronomeRect*	Create(LPDIRECT3DDEVICE9 pGraphicDev,_int iTexIndex);
	virtual void Free();


};

