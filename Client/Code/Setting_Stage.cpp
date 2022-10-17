#include "stdafx.h"
#include "..\Header\Setting_Stage.h"
#include "Export_Function.h"

#include "Setting_BackGround.h"
#include "Logo.h"

CSetting_Stage::CSetting_Stage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CSetting_Stage::~CSetting_Stage()
{
}

HRESULT CSetting_Stage::Ready_Scene(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Ready_Layer_Environment"), E_FAIL);

	return S_OK;
}

_int CSetting_Stage::Update_Scene(const _float & fTimeDelta)
{	
	m_pBackButton = dynamic_cast<CBackButton*>(Engine::Get_GameObject(L"Ready_Layer_Environment", L"BackButton"));
	m_pBGMdnButton = dynamic_cast<CBGMDownBtn*>(Engine::Get_GameObject(L"Ready_Layer_Environment", L"BGM_Dn"));
	m_pBGMupButton = dynamic_cast<CBGMUpBtn*>(Engine::Get_GameObject(L"Ready_Layer_Environment", L"BGM_Up"));
	
	_int iResult = Engine::CScene::Update_Scene(fTimeDelta);
	
	if (m_pBackButton->Get_Click())	
		m_bCheck = true;
	
	if (m_bCheck)
	{
		FAILED_CHECK_RETURN(Engine::Load_SaveScene(static_cast<CScene*>(this)), E_FAIL);
		CScene* pScene = (Engine::Get_Scene());
		FAILED_CHECK_RETURN(Engine::Current_Scene(pScene), E_FAIL);
		m_bCheck = false;
	}	

	return iResult;
}

void CSetting_Stage::LateUpdate_Scene(void)
{
	Engine::CScene::LateUpdate_Scene();
}

void CSetting_Stage::Render_Scene(void)
{
}

HRESULT CSetting_Stage::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	pGameObject = CSetting_BackGround::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SettingsImage", pGameObject), E_FAIL);

	pGameObject = CBackButton::Create(m_pGraphicDev,-0.77f, -0.77f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackButton", pGameObject), E_FAIL);

	pGameObject = CBGMDownBtn::Create(m_pGraphicDev, -0.75f, 0.08f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BGM_Dn", pGameObject), E_FAIL);

	pGameObject = CBGMUpBtn::Create(m_pGraphicDev, -0.25f, 0.08f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BGM_Up", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CSetting_Stage::Ready_Proto(void)
{
	return S_OK;
}

CSetting_Stage * CSetting_Stage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSetting_Stage* pInstance = new CSetting_Stage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSetting_Stage::Free(void)
{
	Engine::CScene::Free();
}
