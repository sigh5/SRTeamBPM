#include "stdafx.h"
#include "..\Header\MiniGame3.h"

#include "Export_Function.h"

#include "MiniStage3PreHeader.h"



CMiniGame3::CMiniGame3(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CMiniGame3::~CMiniGame3()
{
}

HRESULT CMiniGame3::Ready_Scene(void)
{
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;
	m_SceneType = SCENE_MINISTAGE3;

	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);


	


	return S_OK;
}

_int CMiniGame3::Update_Scene(const _float & fTimeDelta)
{
	/*if (Key_Down(DIK_Q))
	{
		CScene* pStage1 = ::Get_SaveScene();
		CLayer* pLayer = pStage1->GetLayer(L"Layer_GameLogic");
		CShopUI* pShopUI = dynamic_cast<CShopUI*>(pLayer->Get_GameObject(L"ShopUI"));
		pShopUI->Set_ForceScene(0);
		pStage1->Set_SceneChane(false);

		Load_SaveScene(this);
		return -1;
	}*/
	// 브금 바꾸기
	Engine::PlaySoundW(L"Paradox.mp3", SOUND_BGM, 0.1f);

	_int iResult = Engine::CScene::Update_Scene(fTimeDelta);

	return iResult;

}

void CMiniGame3::LateUpdate_Scene(void)
{
	Engine::CScene::LateUpdate_Scene();
}

void CMiniGame3::Render_Scene(void)
{
}

HRESULT CMiniGame3::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	pGameObject  = CStaticCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StaticCamera", pGameObject), E_FAIL);

	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox2", pGameObject), E_FAIL);

	pGameObject = CMiniTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MiniTerrain3", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });


	return S_OK;
}

HRESULT CMiniGame3::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	pGameObject = CMiniPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MiniPlayer", pGameObject), E_FAIL);


	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;

}

HRESULT CMiniGame3::Ready_Layer_UI(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);


	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CMiniGame3::Ready_Proto(void)
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainMoveTexCom", CTerrainMoveTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);


	return S_OK;
}

CMiniGame3 * CMiniGame3::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMiniGame3*	pInstance = new CMiniGame3(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMiniGame3::Free(void)
{
	Engine::CScene::Free();
}
