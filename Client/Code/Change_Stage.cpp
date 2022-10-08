#include "stdafx.h"
#include "..\Header\Change_Stage.h"
#include "Export_Function.h"

#include "Loading_Image.h"
#include "Stage_Pjw.h"

CChange_Stage::CChange_Stage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CChange_Stage::~CChange_Stage()
{
}

HRESULT CChange_Stage::Ready_Scene(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Ready_Layer_Environment"), E_FAIL);

	return S_OK;
}

_int CChange_Stage::Update_Scene(const _float & fTimeDelta)
{
	_int iResult = Engine::CScene::Update_Scene(fTimeDelta);
	
	++m_iLoadingCount;

	//cout << "How Count : " << m_iLoadingCount << endl;

	if (m_iLoadingCount > 100)
	{
		CScene*		pScene = CStage_Pjw::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pScene, E_FAIL);

		FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);

		_uint ia = 0;
		
		return 0;
	}
	
	return iResult;
}

void CChange_Stage::LateUpdate_Scene(void)
{
	Engine::CScene::LateUpdate_Scene();
}

void CChange_Stage::Render_Scene(void)
{
}

HRESULT CChange_Stage::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	
	CGameObject*		pGameObject = nullptr;

	pGameObject = CLoading_Image::Create(m_pGraphicDev, true);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LoadingImage", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CChange_Stage::Ready_Proto(void)
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ChangeScene_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Logo/Change_Scene/Change_Scene%d.png", TEX_NORMAL, 7)), E_FAIL);
	
	return S_OK;
}

CChange_Stage * CChange_Stage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CChange_Stage*	pInstance = new CChange_Stage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CChange_Stage::Free(void)
{
	Engine::CScene::Free();
}
