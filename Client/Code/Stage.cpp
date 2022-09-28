#include "stdafx.h"
#include "..\Header\Stage.h"

#include "Export_Function.h"
#include "SkyBox.h"
#include "Effect.h"
#include "FileIOMgr.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene(void)
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

_int CStage::Update_Scene(const _float & fTimeDelta)
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

void CStage::LateUpdate_Scene(void)
{
	Engine::CScene::LateUpdate_Scene();
}

void CStage::Render_Scene(void)
{

}

HRESULT CStage::Ready_Layer_Environment(const _tchar * pLayerTag)
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

HRESULT CStage::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	// testPlayer
	pGameObject = CTestPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestPlayer", pGameObject), E_FAIL);

	


	//몬스터 테스트용
	/*pGameObject = CMonster::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);*/

	//CFileIOMgr::GetInstance()->Load_FileData(m_pGraphicDev, this, L"Layer_GameLogic", L"../../Data/", L"Monster", L"Monster", OBJ_MONSTER);
	CFileIOMgr::GetInstance()->Load_FileData(m_pGraphicDev, 
		this, 
		const_cast<_tchar*>(pLayerTag),
		L"../../Data/",
		L"Monster.dat",
		L"TestMonster",
		OBJ_MONSTER);

	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(const _tchar * pLayerTag)
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

HRESULT CStage::Ready_Proto(void)
{
	return S_OK;
}

CStage * CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage *	pInstance = new CStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStage::Free(void)
{
	CScene::Free();

	for_each(m_BulletList.begin(), m_BulletList.end(), CDeleteObj());
	m_BulletList.clear();
	for_each(m_MonBulletList.begin(), m_MonBulletList.end(), CDeleteObj());
	m_MonBulletList.clear();
	/*
	
	// Effect map에 넣는 방식 변경에 따른 삭제
	for (auto iter : m_szEffectName)
		delete iter;

	if (m_szEffectName.size() == 0)
		m_szEffectName.clear();
	
	*/
}

HRESULT CStage::Ready_Light(void)
{
	D3DLIGHT9		tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type		= D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Direction  = _vec3(0.f, -1.f, 1.f);

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

HRESULT CStage::Push_Bullet(CBullet * pBullet)
{
	m_BulletList.emplace_back(pBullet);

	return S_OK;
}

HRESULT CStage::Push_MonBullet(CMonsterBullet * pBullet)
{
	m_MonBulletList.emplace_back(pBullet);

	return S_OK;
}
