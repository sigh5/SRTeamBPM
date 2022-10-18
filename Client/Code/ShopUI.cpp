#include "stdafx.h"
#include "..\Header\ShopUI.h"

#include "Export_Function.h"


CShopUI::CShopUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
}

CShopUI::~CShopUI()
{
}

HRESULT CShopUI::Ready_Object()
{
	return S_OK;
}

_int CShopUI::Update_Object(const _float & fTimeDelta)
{
	return 0;
}

void CShopUI::LateUpdate_Object(void)
{
}

void CShopUI::Render_Obejct(void)
{
}

void CShopUI::Find_Equip_Item(void)
{
}

HRESULT CShopUI::Add_Component(void)
{
	return S_OK;
}

CShopUI * CShopUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShopUI* pInstance = new CShopUI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CShopUI::Free()
{
	CGameObject::Free();
}
