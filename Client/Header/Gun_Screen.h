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

class CCharacterInfo;
class CCollider;

END

class CGun_Screen : public CGameObject
{
private:
	explicit CGun_Screen(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGun_Screen();

public:
	HRESULT			Ready_Object(CGameObject* pPlayer);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

private:
	CRcTex*				m_pBufferCom = nullptr;	
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CDynamic_Transform*	m_pDynamicTransCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;

	CCharacterInfo*		m_pInfoCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;

private:
	_matrix				m_matCameraView;

	CGameObject*		m_pPlayer;

public:
	static CGun_Screen*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pPlayer);
	virtual void			Free(void);

};

