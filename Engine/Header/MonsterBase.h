#pragma once

#include "GameObject.h"


BEGIN(Engine)

//class CCharacterInfo;
//class CRcTex;
//class CTransform;
//class CCalculator;
//class CTexture;
//class CAnimation;


class ENGINE_DLL CMonsterBase :public CGameObject
{
protected:
	explicit CMonsterBase(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonsterBase();


public:
	virtual HRESULT		 Ready_Object(void)							override;
	virtual _int		 Update_Object(const _float& fTimeDelta)	override;
	virtual void		 LateUpdate_Object(void)					override;
	bool		Set_SelectGizmo();

protected:

	CCharacterInfo*		m_pInfoCom = nullptr;
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;


	int _iMonsterIndex;

protected:
	virtual HRESULT				Add_Component(void);

	virtual void		Free(void);
};

END