#include "stdafx.h"
#include "..\Header\Stage_Pjw.h"


#include "Export_Function.h"
#include "SkyBox.h"
#include "Effect.h"
#include "FileIOMgr.h"

#include "MyCamera.h"

CStage_Pjw::CStage_Pjw(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}


CStage_Pjw::~CStage_Pjw()
{
}

HRESULT CStage_Pjw::Ready_Scene(void)
{
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Light(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);

	return S_OK;
}

_int CStage_Pjw::Update_Scene(const _float & fTimeDelta)
{
	_int iResult = 0;
	if (0 != m_BulletList.size())
	{
		for (auto& iter : m_BulletList)
		{
			iResult = iter->Update_Object(fTimeDelta);

			if (iResult & 0x80000000)
				return iResult;
		}
	}

	if (0 != m_MonBulletList.size())
	{
		for (auto& iter : m_MonBulletList)
		{
			iResult = iter->Update_Object(fTimeDelta);

			if (iResult & 0x80000000)
				return iResult;
		}
	}
	return Engine::CScene::Update_Scene(fTimeDelta);
}

void CStage_Pjw::LateUpdate_Scene(void) 
{
	CLayer *pLayer = GetLayer(L"Layer_GameLogic");

	CTestPlayer* pPlayer = dynamic_cast<CTestPlayer*>(pLayer->Get_GameObject(L"TestPlayer"));
	
	// Potion Collision
	CHealthPotion* pPotion = dynamic_cast<CHealthPotion*>(pLayer->Get_GameObject(L"HealthPotion"));
	
	if (pPotion != nullptr)
	{
		pPlayer->Collision_Event(pPotion);
	}

	//// Coin Collision
	//CCoin* pCoin = dynamic_cast<CCoin*>(pLayer->Get_GameObject(L"Coin"));
	//
	//if (pCoin != nullptr)
	//{
	//	pPlayer->Collision_Event(pCoin);
	//}

	CBox* pBox = dynamic_cast<CBox*>(pLayer->Get_GameObject(L"Box"));

	if (pBox != nullptr)
	{
		pPlayer->Collision_Event(pBox);
	}


	Engine::CScene::LateUpdate_Scene();
}

void CStage_Pjw::Render_Scene(void)
{
}

HRESULT CStage_Pjw::Push_Bullet(CBullet * pBullet)
{
	m_BulletList.emplace_back(pBullet);

	return S_OK;
}

HRESULT CStage_Pjw::Push_MonBullet(CMonsterBullet * pBullet)
{
	m_MonBulletList.emplace_back(pBullet);

	return S_OK;
}

HRESULT CStage_Pjw::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	// DynamicCamera
	pGameObject = CDynamicCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);

	//pGameObject = CMyCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CMyCamera", pGameObject), E_FAIL);

	// StaticCamera
	/*pGameObject = CStaticCamera::Create(m_pGraphicDev, &_vec3(0.f, 20.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StaticCamera", pGameObject), E_FAIL);*/

	// skybox
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);

	// Terrain
	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);
		
	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage_Pjw::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	// testPlayer
	pGameObject = CTestPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestPlayer", pGameObject), E_FAIL);
	
	m_mapLayer.insert({ pLayerTag, pLayer });
	/*
	// HealthPotion
	pGameObject = CHealthPotion::Create(m_pGraphicDev, 5, 10);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"HealthPotion", pGameObject), E_FAIL);

	// Coin
	pGameObject = CCoin::Create(m_pGraphicDev, 22, 10);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Coin", pGameObject), E_FAIL);

	*/
	CTestPlayer*		pPlayer = dynamic_cast<CTestPlayer*>(Get_GameObject(L"Layer_GameLogic", L"TestPlayer"));

	pGameObject = CBox::Create(m_pGraphicDev, 5, 10, pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Box", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	//몬스터 테스트용
	/*pGameObject = CMonster::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);*/

	//CFileIOMgr::GetInstance()->Load_FileData(m_pGraphicDev, this, L"Layer_GameLogic", L"../../Data/", L"Monster", L"Monster", OBJ_MONSTER);
	//CFileIOMgr::GetInstance()->Load_FileData(m_pGraphicDev,
	//	this,
	//	const_cast<_tchar*>(pLayerTag),
	//	L"../../Data/",
	//	L"Monster.dat",
	//	L"Monster",
	//	OBJ_MONSTER);

	return S_OK;
}

HRESULT CStage_Pjw::Ready_Layer_UI(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;
	CTestPlayer*		pPlayer = dynamic_cast<CTestPlayer*>(Get_GameObject(L"Layer_GameLogic", L"TestPlayer"));

	pGameObject = CBullet_UI::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Bullet_UI", pGameObject), E_FAIL);

	pGameObject = CWeapon_UI::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Weapon_UI", pGameObject), E_FAIL);

	pGameObject = CHpBar::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"HpBar", pGameObject), E_FAIL);

	pGameObject = CStatus_UI::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Status_UI", pGameObject), E_FAIL);

	pGameObject = CCoinKeyUI::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CoinKey_UI", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage_Pjw::Ready_Proto(void)
{
	return S_OK;
}

HRESULT CStage_Pjw::Ready_Light(void)
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

CStage_Pjw * CStage_Pjw::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage_Pjw*	pInstance = new CStage_Pjw(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStage_Pjw::Free(void)
{
	CScene::Free();

	for_each(m_BulletList.begin(), m_BulletList.end(), CDeleteObj());
	m_BulletList.clear();
	/*for_each(m_MonBulletList.begin(), m_MonBulletList.end(), CDeleteObj());
	m_MonBulletList.clear();*/
}
