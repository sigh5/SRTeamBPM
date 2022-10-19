#include "stdafx.h"
#include "TestUI.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

CTestUI::CTestUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
}

CTestUI::~CTestUI()
{
}

HRESULT CTestUI::Ready_Object(_bool _bTrue)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_bChangePNG = _bTrue;

	return S_OK;
}

_int CTestUI::Update_Object(const _float & fTimeDelta)
{
	_vec3 vecScale = { 2.0f, 2.0f, 2.0f };

	m_pTransCom->Set_Scale(&vecScale);

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_PRIORITY, this);

	if (m_bChangePNG)
	{
		m_iChangePNG = rand() % 7;
		m_bChangePNG = false;
	}

	return 0;
}

void CTestUI::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CTestUI::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	m_pTextureCom->Set_Texture(m_iChangePNG);

	m_pBufferCom->Render_Buffer();
}

HRESULT CTestUI::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_ChangeScene_Texture", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);


	return S_OK;
}

CTestUI * CTestUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _bTrue)
{
	CTestUI*	pInstance = new CTestUI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(_bTrue)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->m_pTransCom->Set_Pos(0.f, 0.f, 0.3f);

	return pInstance;
}

void CTestUI::Free(void)
{
	CGameObject::Free();
}
