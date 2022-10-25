#include "stdafx.h"
#include "..\Header\EndingScene.h"
#include "Export_Function.h"
#include "EndingImage.h"

CEndingScene::CEndingScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CEndingScene::~CEndingScene()
{
}

HRESULT CEndingScene::Ready_Scene(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Ready_Layer_Environment"), E_FAIL);
	
	Engine::PlaySoundW(L"GTA-V-Welcome-to-Los-Santosx.mp3", SOUND_ENDINGSCENE, 1.f);

	return S_OK;
}

_int CEndingScene::Update_Scene(const _float & fTimeDelta)
{
	if (m_fCount > 200.f)
		--m_fCount;

	else
		m_fCount = 200.f;


	_int iResult = Engine::CScene::Update_Scene(fTimeDelta);
	
	return iResult;
}

void CEndingScene::LateUpdate_Scene(void)
{
	Engine::CScene::LateUpdate_Scene();
}

void CEndingScene::Render_Scene(void)
{
	Render_Font(L"BMJUA_ttf", L"SR TEAM PORTFOLIO", &_vec2(640.f, m_fCount), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

HRESULT CEndingScene::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;
		
	pGameObject = CEndingImage::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EndingImage", pGameObject), E_FAIL);
	
	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;

}

HRESULT CEndingScene::Ready_Proto(void)
{																									// \Bin\Resource\Texture\Logo
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EndingTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Logo/Ending_Scene1.png", TEX_NORMAL)), E_FAIL);

	return S_OK;
}

CEndingScene * CEndingScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEndingScene*	pInstance = new CEndingScene(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEndingScene::Free(void)
{
	Engine::CScene::Free();
}
