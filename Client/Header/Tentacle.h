#pragma once
#include "GameObject.h"
BEGIN(Engine)
class CTexture;
class CAnimation;
class CRcTex;
class CTransform;
class CCharacterInfo;
END
class CTentacle :public CGameObject
{
private:
	explicit CTentacle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTentacle();

public:
	virtual HRESULT		Ready_Object(float Posx, float Posy, CGameObject* pMaster, float Wait);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

	float				Get_Distance(void) { return m_fToPlayerDistance; }

public:
	static CTentacle*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pMaster, float wait, float Posx = 0, float Posy = 0);
	virtual void		Free(void);
private:
	CTransform*	m_pTransform = nullptr;
	CTexture*	 m_pTexture = nullptr;
	CRcTex*		m_pBufferCom = nullptr;
	CAnimation*	m_pAnimationCom = nullptr;

	CGameObject*	m_pMaster = nullptr;
	CCharacterInfo*	m_pMasterInfo = nullptr;
	CTransform*		m_pMasterTransform = nullptr;
	float		m_fToPlayerDistance = 0.f;
	bool		m_bHitPlayer = false;
	int			m_iMasterHp = 0;
	int			m_iMasterPreHp = 0;

	int			m_iMasterHitNum = 0;
	bool		m_bOneTime = false;
	bool		m_bAttackFinish = false;
};

