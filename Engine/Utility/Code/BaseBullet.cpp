#include "..\..\Header\BaseBullet.h"


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

void CBaseBullet::Free(void)
{
	Engine::CGameObject::Free();
}
