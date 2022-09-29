#include "stdafx.h"
#include "..\Header\ColliderStage.h"

#include "Export_Function.h"

#include "HWPlayer.h"
#include "Bullet.h"  // Player's
#include "Terrain.h"
#include "StaticCamera.h"
#include "DynamicCamera.h"

#include "SkyBox.h"
#include "FileIOMgr.h"
#include "ObjectMgr.h"

#include "MyCamera.h"
#include "MetronomeUI.h"

#include "FatBat.h"



CColliderStage::CColliderStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev),m_iCount(0)
{
}

CColliderStage::~CColliderStage()
{
}

HRESULT CColliderStage::Ready_Scene(void)
{
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Light(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);


	
	::PlaySoundW(L"SamTow.wav", SOUND_BGM, 0.1f);
	return S_OK;
}

_int CColliderStage::Update_Scene(const _float & fTimeDelta)
{
	
	m_fFrame += 1.f * fTimeDelta;

	if (m_fFrame >= 1.f)
	{

		CGameObject*		pGameObject = nullptr;

		CLayer *pMyLayer = GetLayer(L"Layer_UI");

		pGameObject = CObjectMgr::GetInstance()->Reuse_MetronomeUI(m_pGraphicDev, -WINCX / 2.f, 0.f, +100.f, 1);
		NULL_CHECK_RETURN(pGameObject, );
		pMyLayer->Add_GameObjectList(pGameObject);

		pGameObject = CObjectMgr::GetInstance()->Reuse_MetronomeUI(m_pGraphicDev, WINCX / 2.f, 0, -100.f, 0);
		NULL_CHECK_RETURN(pGameObject, );
		pMyLayer->Add_GameObjectList(pGameObject);





	}

	if (m_fFrame >= 1.f)
	{

		CGameObject*		pGameObject = nullptr;

		CLayer *pMyLayer = GetLayer(L"Layer_UI");

	
		
		m_fFrame = 0.f;
	}


	return Engine::CScene::Update_Scene(fTimeDelta);
}

void CColliderStage::LateUpdate_Scene(void)
{
	CLayer *pLayer1 = GetLayer(L"Layer_GameLogic");
	CHWPlayer* pPlayer = dynamic_cast<CHWPlayer*>(pLayer1->Get_GameObject(L"TestPlayer"));

	CLayer * pLayer2 = GetLayer(L"Layer_UI");

	pLayer2->Get_GameObjectMap();

	for (auto iter = pLayer2->Get_GameObjectMap().begin(); iter != pLayer2->Get_GameObjectMap().end(); ++iter)
	{
		pPlayer->Collsion_CubeMap(iter->second);
	}


	Engine::CScene::LateUpdate_Scene();
}

void CColliderStage::Render_Scene(void)
{
	

}

HRESULT CColliderStage::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	//// DynamicCamera
	//pGameObject = CDynamicCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);

	// StaticCamera
	//pGameObject = CStaticCamera::Create(m_pGraphicDev, &_vec3(0.f, 1.f, -1.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StaticCamera", pGameObject), E_FAIL);

	pGameObject = CMyCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CMyCamera", pGameObject), E_FAIL);

	// skybox
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);

	// Terrain
	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);


	pGameObject = CStart_Button::Create(m_pGraphicDev, 0.75f, 0.75f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StartButton", pGameObject), E_FAIL);



	m_mapLayer.insert({ pLayerTag, pLayer });

	//CFileIOMgr::GetInstance()->Load_FileData(m_pGraphicDev,
	//	this,
	//	const_cast<_tchar*>(pLayerTag),
	//	L"../../Data/",
	//	L"Map.dat",
	//	L"TestCube",
	//	OBJ_CUBE);


	return S_OK;
}

HRESULT CColliderStage::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	// testPlayer
	pGameObject = CHWPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestPlayer", pGameObject), E_FAIL);


	pGameObject = CFatBat::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestMonster", pGameObject), E_FAIL);

	
	

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CColliderStage::Ready_Layer_UI(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	m_mapLayer.insert({ pLayerTag, pLayer });
	
	CFileIOMgr::GetInstance()->Load_FileData(m_pGraphicDev,
		this,
		const_cast<_tchar*>(pLayerTag),
		L"../../Data/",
		L"Map.dat",
		L"TestCube",
		OBJ_CUBE);

		pGameObject = CObjectMgr::GetInstance()->Reuse_MetronomeUI(m_pGraphicDev, -WINCX / 2.f, 0.f, +100.f, 1);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObjectList(pGameObject), E_FAIL);
	
	/*for (int i = 0; i < 25; ++i)
	{
		pGameObject = CObjectMgr::GetInstance()->Reuse_MetronomeUI(m_pGraphicDev, WINCX / 2.f, 0, -100.f, 0);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObjectList(pGameObject), E_FAIL);
	}
*/



	/*pGameObject = CObjectMgr::GetInstance()->Reuse_MetronomeUI(m_pGraphicDev, -WINCX / 2.f, 0.f, +100.f, 1);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObjectList(pGameObject), E_FAIL);*/


	/*pGameObject = CMetronomeUI::Create(m_pGraphicDev,WINCX/2.f , 0.f, -100.f,0);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestUI", pGameObject), E_FAIL);
	*/

	/*pGameObject = CMetronomeUI::Create(m_pGraphicDev, -WINCX / 2.f, 0.f, +100.f, 1);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestUI2", pGameObject), E_FAIL);*/


	return S_OK;
}

HRESULT CColliderStage::Ready_Proto(void)
{
	return S_OK;
}

HRESULT CColliderStage::Ready_Light(void)
{
	D3DLIGHT9		tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Direction = _vec3(0.f, -1.f, 1.f);

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

CColliderStage * CColliderStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CColliderStage *	pInstance = new CColliderStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CColliderStage::Free(void)
{
	CScene::Free();
}
