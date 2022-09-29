#include "..\..\Header\BaseBullet.h"
#include "Transform.h"
#include "Dynamic_Transform.h"

#include "Export_Function.h"

USING(Engine)


CBaseBullet::CBaseBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CBaseBullet::CBaseBullet(const CBaseBullet & rhs)
	:CGameObject(rhs)
{
}

CBaseBullet::~CBaseBullet()
{
}

HRESULT CBaseBullet::Ready_Object(_vec3 vPos)
{
	return S_OK;
}

_int CBaseBullet::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	return 0;
}

void CBaseBullet::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CBaseBullet::Render_Obejct(void)
{
}

void CBaseBullet::Set_MoveDir(const wstring& LayerName , const wstring& GameObjectName, const wstring& ComponentName, COMPONENTID eID, _vec3 * vPos)
{
	// 추후에 불렛이 다이나믹을 받으면 dynamicTransform
	CTransform* pTargetTransformCom = static_cast<CTransform*>(Engine::Get_Component(LayerName.c_str(),
		GameObjectName.c_str(),
		ComponentName.c_str(),
		eID));

	NULL_CHECK_RETURN(pTargetTransformCom, );
	
	CTransform* pThisTransformCom = static_cast<CTransform*>(Get_Component(ComponentName.c_str(), eID));
	pThisTransformCom->Set_Pos((*vPos).x, (*vPos).y, (*vPos).z);

	_vec3 vPlayerPos;
	pTargetTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_MoveDir = vPlayerPos - (*vPos);

}

void CBaseBullet::Free(void)
{
	Engine::CGameObject::Free();
}
