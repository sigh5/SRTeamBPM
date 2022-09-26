#include "stdafx.h"
#include "..\Header\HpBar.h"

#include "Export_Function.h"

CHpBar::CHpBar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CHpBar::~CHpBar()
{
}

HRESULT CHpBar::Ready_Object(void)
{
	return S_OK;
}

_int CHpBar::Update_Object(const _float & fTimeDelta)
{
	return 0;
}

void CHpBar::LateUpdate_Object(void)
{
}

void CHpBar::Render_Obejct(void)
{
}

CHpBar * CHpBar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHpBar* pInstance = new CHpBar(pGraphicDev);

	return nullptr;
}

void CHpBar::Free(void)
{
}
