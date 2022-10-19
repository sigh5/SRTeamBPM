#include "stdafx.h"
#include "..\Header\TestChane.h"
#include "Export_Function.h"

#include "Loading_Image.h"
#include "LoadingBackFont.h"
#include "Stage.h"
#include "MiniStage1.h"
#include "TestUI.h"

CTestChane::CTestChane(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CTestChane::~CTestChane()
{
}

HRESULT CTestChane::Ready_Scene(_uint iStageNum)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_iStageIndex = iStageNum;
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Ready_Layer_Environment"), E_FAIL);



	return S_OK;
}

_int CTestChane::Update_Scene(const _float & fTimeDelta)
{
	_int iResult = Engine::CScene::Update_Scene(fTimeDelta);

	++m_iLoadingCount;

	//cout << "How Count : " << m_iLoadingCount << endl;
	// 150~200

	if (m_iStageIndex == 0)
	{
		if (m_iLoadingCount > 100)
		{
			CScene*		pScene = CStage::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pScene, E_FAIL);

			FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);

			return 0;
		}
	}
	else if (m_iStageIndex == 1)
	{
		/*	if (m_iLoadingCount > 100)
		{
		CScene*		pScene = CMiniStage1::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pScene, E_FAIL);

		FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);

		return 0;
		}
		return 0;*/
		return iResult;
	}

	return iResult;
}

void CTestChane::LateUpdate_Scene(void)
{
	Engine::CScene::LateUpdate_Scene();
}

void CTestChane::Render_Scene(void)
{
}

HRESULT CTestChane::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	wstring szImageName = L"";
	wstring szBackGroundName = L"";
	if (m_iStageIndex == 0)
	{
		szImageName = LOADING_IMAGE0;
		szBackGroundName = LOADING_FONT_BACKGROUND0;
	}


	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	pGameObject = CLoading_Image::Create(m_pGraphicDev, true);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(LOADING_IMAGE0, pGameObject), E_FAIL);

	pGameObject = CLoadingBackFont::Create(m_pGraphicDev, 0.58f, -0.75f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(LOADING_FONT_BACKGROUND0, pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CTestChane::Ready_Proto(void)
{
	return S_OK;
}

CTestChane * CTestChane::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iStageNum)
{
	CTestChane*	pInstance = new CTestChane(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene(iStageNum)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTestChane::Free(void)
{
	Engine::CScene::Free();
}
