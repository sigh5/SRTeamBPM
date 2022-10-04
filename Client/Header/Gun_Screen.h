#pragma once
#include "GameObject.h"
#include "Engine_Include.h"
#include "TestPlayer.h"

BEGIN(Engine)

class CDynamic_Transform;
class CRcTex;
class CTexture;
class CCalculator;
class CAnimation;
class COrthoTransform;

END

class CGun_Screen : public CGameObject
{
public:
	explicit CGun_Screen(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGun_Screen(const CGameObject& rhs);
	virtual ~CGun_Screen();

public:
	HRESULT			Ready_Object(CGameObject* pPlayer);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

public:
	HRESULT				Shoot_Motion(void);

	void				Set_Shoot(_bool pShoot) { m_bShootAnimation = pShoot; }

public:
	CRcTex*				m_pBufferCom = nullptr;	
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CDynamic_Transform*	m_pDynamicTransCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;
	COrthoTransform*	m_pOrthoTransCom = nullptr;
	
	CGameObject*		m_pPlayer;

public:
	_bool				m_bShootAnimation = false;
	
public:
	static CGun_Screen*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pPlayer);
	virtual void			Free(void);

};

