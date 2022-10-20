#pragma once
#include "GameObject.h"
BEGIN(Engine)
class CTransform;
class CTexture;
class CRcTex;
END
class CYetiTalk :public CGameObject
{
private:
	explicit CYetiTalk(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CYetiTalk();

public:
	virtual HRESULT		Ready_Object(float	Posx, float Posz, CTransform* pMaster);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	CTexture* m_pTextureCom = nullptr;
	CTransform* m_pTransform = nullptr;
	CRcTex*		m_pBufferCom = nullptr;

	CTransform*	m_pMasterTransform = nullptr;
public:
	static CYetiTalk*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pMaster, float Posx = 0, float Posz = 0 );
	virtual void	Free(void);

private:
	float	m_bLifetime = 0.f;
};

