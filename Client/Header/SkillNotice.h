#pragma once
#include "GameObject.h"
BEGIN(Engine)
class CTransform;
class CTexture;
class CRcTex;
END
class CSkillNotice :public CGameObject
{
private:
	explicit CSkillNotice(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkillNotice();

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
	static CSkillNotice*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pMaster, float Posx = 0, float Posz = 0);
	virtual void	Free(void);

private:
	float	m_fLifetime = 0.f;
	int		m_iRenderChange = 0;
	bool	m_bRenderOn = true;
	bool	m_bSoundNotice = true;
};

