#pragma once
#include "GameObject.h"
#include "Engine_Include.h"
#include "TestPlayer.h"

BEGIN(Engine)

	class CTransform;
	class CRcTex;
	class CTexture;
	class CCalculator;
	class CAnimation;
	class CCollider;

END

class CBox : public CGameObject
{
private:
	explicit CBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBox();

public:
	HRESULT				Ready_Object(_uint iX, _uint iY, CTestPlayer* pPlayer);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);
	void				Set_OnTerrain(void);

public:
	HRESULT				Open_Event(CGameObject* pGameObject);
	
private:
	CTransform*			m_pTransCom = nullptr;
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;	
	CAnimation*			m_pAnimationCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;

	CTestPlayer*		m_pPlayer;

public:
	static CBox*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iX, _uint iY, CTestPlayer* pPlayer);
	virtual void		Free(void);

};

 