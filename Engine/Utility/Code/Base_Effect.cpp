#include "..\..\Header\Base_Effect.h"

USING(Engine)


CBase_Effect::CBase_Effect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	ZeroMemory(&m_eEffect, sizeof(INFO_Effect));
	D3DXMatrixIdentity(&m_matEffectProj);
}

CBase_Effect::CBase_Effect(const CBase_Effect & rhs)
	 : CGameObject(rhs)
	
{
	memcpy(&m_eEffect, &rhs.m_eEffect, sizeof(INFO_Effect));
	memcpy(&m_matEffectProj, &rhs.m_matEffectProj, sizeof(_matrix));
}

CBase_Effect::~CBase_Effect()
{

}

_int CBase_Effect::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	return 0;
}

void CBase_Effect::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

HRESULT CBase_Effect::Set_Effect_INFO(_int iEffcet_Owner, _int iStartAnimNum, _int iAnimMax, _float fIntervel)
{
	m_eEffect.iEffcet_Owner = (OBJ_OWNER)iEffcet_Owner;
	m_eEffect.iStartAnimNum = iStartAnimNum;
	m_eEffect.iAnimMax = iAnimMax;
	m_eEffect.fIntervel = fIntervel;

	return S_OK;
}

void CBase_Effect::Free(void)
{
	CGameObject::Free();
}
