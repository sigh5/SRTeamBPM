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


class CMetronomeUI :public CGameObject
{

private:
	explicit CMetronomeUI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMetronomeUI(const CGameObject& rhs);

public:
	HRESULT			Ready_Object();
	virtual _int	Update_Object(const _float& fTimeDelta) override;

	virtual	void	LateUpdate_Object(void);
	virtual void	Render_Obejct(void) override;

private:
	HRESULT			Add_Component(void);

public:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	COrthoTransform*	m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;


public:
	static CMetronomeUI*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free();
};

