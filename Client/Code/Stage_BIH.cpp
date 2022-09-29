#include "stdafx.h"
#include "..\Header\Stage_BIH.h"

#include "Export_Function.h"
#include "SkyBox.h"
#include "Effect.h"
#include "FileIOMgr.h"



CStage_BIH::CStage_BIH(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}


CStage_BIH::~CStage_BIH()
{
}

HRESULT CStage_BIH::Ready_Scene(void)
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

_int CStage_BIH::Update_Scene(const _float & fTimeDelta)
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

void CStage_BIH::LateUpdate_Scene(void)
{
	Engine::CScene::LateUpdate_Scene();
}

void CStage_BIH::Render_Scene(void)
{

}

HRESULT CStage_BIH::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	// DynamicCamera
	pGameObject = CDynamicCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);

	// StaticCamera
	//pGameObject = CStaticCamera::Create(m_pGraphicDev, &_vec3(0.f, 20.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StaticCamera", pGameObject), E_FAIL);

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

HRESULT CStage_BIH::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	// testPlayer
	pGameObject = CTestPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestPlayer", pGameObject), E_FAIL);


	m_mapLayer.insert({ pLayerTag, pLayer });

	//���� �׽�Ʈ��
	/*pGameObject = CMonster::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);*/

	//CFileIOMgr::GetInstance()->Load_FileData(m_pGraphicDev, this, L"Layer_GameLogic", L"../../Data/", L"Monster", L"Monster", OBJ_MONSTER);
	CFileIOMgr::GetInstance()->Load_FileData(m_pGraphicDev,
		this,
		const_cast<_tchar*>(pLayerTag),
		L"../../Data/",
		L"Monster.dat",
		L"Monster",
		OBJ_MONSTER);

	return S_OK;
}

HRESULT CStage_BIH::Ready_Layer_UI(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;
	CTestPlayer* pPlayer = dynamic_cast<CTestPlayer*>(Get_GameObject(L"Layer_GameLogic", L"TestPlayer"));

	pGameObject = CBullet_UI::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Bullet_UI", pGameObject), E_FAIL);

	pGameObject = CWeapon_UI::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Weapon_UI", pGameObject), E_FAIL);


	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage_BIH::Ready_Proto(void)
{
	return S_OK;
}

CStage_BIH * CStage_BIH::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage_BIH *	pInstance = new CStage_BIH(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStage_BIH::Free(void)
{
	CScene::Free();

	for_each(m_BulletList.begin(), m_BulletList.end(), CDeleteObj());
	m_BulletList.clear();
	for_each(m_MonBulletList.begin(), m_MonBulletList.end(), CDeleteObj());
	m_MonBulletList.clear();
	/*

	// Effect map�� �ִ� ��� ���濡 ���� ����
	for (auto iter : m_szEffectName)
	delete iter;

	if (m_szEffectName.size() == 0)
	m_szEffectName.clear();

	*/
}

HRESULT CStage_BIH::Ready_Light(void)
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

HRESULT CStage_BIH::Push_Bullet(CBullet * pBullet)
{
	m_BulletList.emplace_back(pBullet);

	return S_OK;
}

HRESULT CStage_BIH::Push_MonBullet(CMonsterBullet * pBullet)
{
	m_MonBulletList.emplace_back(pBullet);

	return S_OK;
}
#include "stdafx.h"
#include "..\Header\Stage_BIH.h"

#include "Export_Function.h"
#include "SkyBox.h"
#include "Effect.h"
#include "FileIOMgr.h"



CStage_BIH::CStage_BIH(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}


CStage_BIH::~CStage_BIH()
{
}

HRESULT CStage_BIH::Ready_Scene(void)
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

_int CStage_BIH::Update_Scene(const _float & fTimeDelta)
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

void CStage_BIH::LateUpdate_Scene(void)
{
	Engine::CScene::LateUpdate_Scene();
}

void CStage_BIH::Render_Scene(void)
{

}

HRESULT CStage_BIH::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	// DynamicCamera
	pGameObject = CDynamicCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);

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

HRESULT CStage_BIH::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	// testPlayer
	pGameObject = CTestPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestPlayer", pGameObject), E_FAIL);


	m_mapLayer.insert({ pLayerTag, pLayer });

	//���� �׽�Ʈ��
	/*pGameObject = CMonster::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);*/

	//CFileIOMgr::GetInstance()->Load_FileData(m_pGraphicDev, this, L"Layer_GameLogic", L"../../Data/", L"Monster", L"Monster", OBJ_MONSTER);
	CFileIOMgr::GetInstance()->Load_FileData(m_pGraphicDev,
		this,
		const_cast<_tchar*>(pLayerTag),
		L"../../Data/",
		L"Monster.dat",
		L"Monster",
		OBJ_MONSTER);

	return S_OK;
}

HRESULT CStage_BIH::Ready_Layer_UI(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;
	CTestPlayer* pPlayer = dynamic_cast<CTestPlayer*>(Get_GameObject(L"Layer_GameLogic", L"TestPlayer"));

	pGameObject = CBullet_UI::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Bullet_UI", pGameObject), E_FAIL);

	pGameObject = CWeapon_UI::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Weapon_UI", pGameObject), E_FAIL);


	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage_BIH::Ready_Proto(void)
{
	return S_OK;
}

CStage_BIH * CStage_BIH::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage_BIH *	pInstance = new CStage_BIH(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStage_BIH::Free(void)
{
	CScene::Free();

	for_each(m_BulletList.begin(), m_BulletList.end(), CDeleteObj());
	m_BulletList.clear();
	for_each(m_MonBulletList.begin(), m_MonBulletList.end(), CDeleteObj());
	m_MonBulletList.clear();
	/*

	// Effect map�� �ִ� ��� ���濡 ���� ����
	for (auto iter : m_szEffectName)
	delete iter;

	if (m_szEffectName.size() == 0)
	m_szEffectName.clear();

	*/
}

HRESULT CStage_BIH::Ready_Light(void)
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

HRESULT CStage_BIH::Push_Bullet(CBullet * pBullet)
{
	m_BulletList.emplace_back(pBullet);

	return S_OK;
}

HRESULT CStage_BIH::Push_MonBullet(CMonsterBullet * pBullet)
{
	m_MonBulletList.emplace_back(pBullet);

	return S_OK;
}
