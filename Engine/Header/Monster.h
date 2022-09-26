#pragma once

#include "GameObject.h"



BEGIN(Engine)

class ENGINE_DLL CMonster :public CGameObject
{
protected:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonster();


public:
	virtual HRESULT		 Ready_Object(void)							override;
	virtual _int		 Update_Object(const _float& fTimeDelta)	override;
	virtual void		 LateUpdate_Object(void)					override;
	virtual bool		Set_SelectGizmo();

protected:
	virtual HRESULT				Add_Component(void);

	CCharacterInfo*		m_pInfoCom = nullptr;
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;

	

	int _iMonsterIndex;

protected:
	virtual void		Free(void);
};

END