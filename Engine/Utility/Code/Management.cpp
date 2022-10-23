#include "Export_Function.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
	: m_pScene(nullptr), m_pSaveScene(nullptr)
{
}


CManagement::~CManagement()
{
	Free();
}


CComponent* Engine::CManagement::Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}

HRESULT CManagement::Add_GameObject(const _tchar* pLayerTag, const _tchar * pObjTag, CGameObject * pInstance)
{
	m_pScene->Get_MapLayer()[pLayerTag]->Add_GameObject(pObjTag, pInstance);

	return S_OK;
}

HRESULT CManagement::Set_Scene(CScene * pScene)
{

	Safe_Release(m_pScene);	 // 기존 scene을 삭제
	Engine::Clear_RenderGroup(); // 기존 scene에 그려지고 있던 모든 렌더 요소들을 삭제

	m_pScene = pScene;
	
	return S_OK;
}

CGameObject * CManagement::Get_GameObject(const _tchar * pLayerTag, const _tchar * pObjTag)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_GameObject(pLayerTag, pObjTag);
}										// Stage			Minigame
										// Logo            Setting_Stage
HRESULT CManagement::Change_Scene(CScene * pScene, CScene* pChangeScene)
{
	if (m_pScene == pScene)
	{
		m_pSaveScene = m_pScene;  // 기존 씬에 대한 정보를 저장

		//Engine::Clear_RenderGroup();
		// 기존 scene에 그려지고 있던 모든 렌더 요소들을 삭제하고 
		// scene은 삭제하지 않은 채로 위에 덮는다.
		m_pScene = pChangeScene;
	}
	return S_OK;
}

HRESULT CManagement::Load_SaveScene(CScene* pScene)
{
	if (m_pScene == pScene)
	{
		Safe_Release(m_pScene);	 // 기존 scene을 삭제
		Engine::Clear_RenderGroup(); // 기존 scene에 그려지고 있던 모든 렌더 요소들을 삭제

		m_pScene = m_pSaveScene;
	}
		return S_OK;
}

HRESULT CManagement::Current_Scene(CScene * pScene)
{
	m_pScene = pScene;

	return S_OK;
}


_int CManagement::Update_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pScene)
		return -1;
	
	//int a = 0;
	return m_pScene->Update_Scene(fTimeDelta);  // -99 리턴값을 잡고 그다음에 
}

void CManagement::LateUpdate_Scene(void)
{
	if (nullptr == m_pScene)
		return;

	m_pScene->LateUpdate_Scene();
}

void CManagement::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Engine::Render_GameObject(pGraphicDev);

	if (nullptr == m_pScene)
		return;

	m_pScene->Render_Scene();
}

void Engine::CManagement::Free(void)
{
	Safe_Release(m_pScene);
}

