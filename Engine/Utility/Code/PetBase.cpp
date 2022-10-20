#include "..\..\Header\PetBase.h"

#include "Transform.h"
#include "Dynamic_Transform.h"

#include "Export_Utility.h"

USING(Engine)

CPetBase::CPetBase(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_vPetPos(0.f,0.f,0.f)
	, m_vPlayerPos(0.f,0.f,0.f)
{

}

CPetBase::~CPetBase()
{
}

HRESULT CPetBase::Ready_Object(_vec3 vPos)
{
	return E_NOTIMPL;
}

_int CPetBase::Update_Object(const _float & fTimeDelta)
{
	return _int();
}

void CPetBase::LateUpdate_Object(void)
{
}

void CPetBase::Render_Obejct(void)
{
}

void CPetBase::Get_ToPlayerDistance(float * _Distance)
{
	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransformCom);

	pPlayerTransformCom->Get_Info(INFO_POS, &m_vPlayerPos);
	m_pDynamicTransformCom->Get_Info(INFO_POS, &m_vPetPos);

	float fMtoPDistance; // 몬스터와 플레이어 간의 거리

	fMtoPDistance = sqrtf((powf(m_vPetPos.x - m_vPlayerPos.x, 2) + powf(m_vPetPos.y - m_vPlayerPos.y, 2) + powf(m_vPetPos.z - m_vPlayerPos.z, 2)));

	memcpy(_Distance, &fMtoPDistance, sizeof(float));
}

void CPetBase::Free(void)
{
	CGameObject::Free();
}
