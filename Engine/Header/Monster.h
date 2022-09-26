#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CMonster :public CGameObject
{
private:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonster();


public:
	virtual HRESULT Ready_Object(void)						override;
	virtual _int Update_Object(const _float& fTimeDelta)	override;
	virtual void LateUpdate_Object(void)					override;

private:
	int		m_iMonsterIndex = 0;
};

END