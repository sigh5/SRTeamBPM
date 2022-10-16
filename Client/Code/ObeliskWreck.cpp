#include "stdafx.h"
#include "..\Header\ObeliskWreck.h"

#include "Export_Function.h"
#include "AbstractFactory.h"

CObeliskWreck::CObeliskWreck(LPDIRECT3DDEVICE9 pGraphicDev)
	:CWreckBase(pGraphicDev)
{
}


CObeliskWreck::~CObeliskWreck()
{
}

HRESULT CObeliskWreck::Ready_Object(_vec3 vPos, _vec3 vDir)
{
	Add_Component();

	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Obelisk_Wreck_Texture", m_mapComponent, ID_STATIC);

	return S_OK;
}

_int CObeliskWreck::Update_Object(const _float & fTimeDelta)
{


	return 0;
}

void CObeliskWreck::LateUpdate_Object(void)
{
}

void CObeliskWreck::Render_Obejct(void)
{
}

CObeliskWreck * CObeliskWreck::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vRight)
{
	return nullptr;
}

void CObeliskWreck::Free(void)
{
}
