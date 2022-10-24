#pragma once
#include "GameObject.h"
BEGIN(Engine)
class CTransform;
class CTexture;
class CRcTex;
class CAnimation;
class CCharacterInfo;
END
class CMonsterHpBar :public CGameObject
{
private:
	explicit CMonsterHpBar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonsterHpBar();

public:
	virtual HRESULT		Ready_Object(float	Posx, float Posz, CTransform* pMaster, CCharacterInfo* pMasterInfo);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	CTexture* m_pTextureCom = nullptr;
	CTransform* m_pTransform = nullptr;
	CRcTex*		m_pBufferCom = nullptr;
	CAnimation*	m_pAnimationCom = nullptr;

	CTransform*	m_pMasterTransform = nullptr;
	CCharacterInfo* m_pMasterInfoCom = nullptr;

	int	m_iMaxHp = 0;
	int m_iHpBarState = 0;
public:
	static CMonsterHpBar*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pMasterTrans, CCharacterInfo* pMasterInfo, float Posx = 0, float Posz = 0);
	virtual void	Free(void);
};

