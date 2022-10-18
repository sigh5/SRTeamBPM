#include "stdafx.h"
#include "..\Header\Setting_BackGround.h"
#include "Export_Function.h"
#include "AbstractFactory.h"


CSetting_BackGround::CSetting_BackGround(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
}

CSetting_BackGround::~CSetting_BackGround()
{
}

HRESULT CSetting_BackGround::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CSetting_BackGround::Update_Object(const _float & fTimeDelta)
{
	
	_vec3 vecScale = { 2.0f, 2.0f, 2.0f };

	m_pTransCom->Set_Scale(&vecScale);
	
	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CSetting_BackGround::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CSetting_BackGround::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

}

HRESULT CSetting_BackGround::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_SettingsScene_Texture", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);

	return S_OK;
}

CSetting_BackGround * CSetting_BackGround::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSetting_BackGround* pInstance = new CSetting_BackGround(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->m_pTransCom->Set_Pos(0.f, 0.f, 0.3f);

	return pInstance;
}

void CSetting_BackGround::Free(void)
{
	Engine::CGameObject::Free();
}
