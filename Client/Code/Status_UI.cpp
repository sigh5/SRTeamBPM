#include "stdafx.h"
#include "..\Header\Status_UI.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

USING(Engine)



CStatus_UI::CStatus_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CStatus_UI::CStatus_UI(const CGameObject & rhs)
	: CGameObject(rhs)
{
}

CStatus_UI::~CStatus_UI()
{
}

HRESULT CStatus_UI::Ready_Object(CTestPlayer * pPlayer)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pPlayer = pPlayer;

	return S_OK;
}

_int CStatus_UI::Update_Object(const _float & fTimeDelta)
{
	// Tab Ű �Է¿� ���� Status_UI�� ���� Transform�� vScale�� �������ִ� ��� ������ ��

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CStatus_UI::LateUpdate_Object(void)
{							// 1��, 2�� ���ڴ� Scale X, Y. Default�δ� WINCX, WINCY �ۿ� �̹����� �����ϵ��� �ø��� TabŰ�� ������ �پ�� ȭ�鿡 ���̵���
	m_pTransCom->OrthoMatrix(380.f, 350.f, 0.f, 0.f, WINCX, WINCY);

	CGameObject::LateUpdate_Object();
}

void CStatus_UI::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_pTransCom->m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_pTransCom->m_matOrtho);

	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
}

HRESULT CStatus_UI::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<COrthoTransform>::Clone_Proto_Component(L"Proto_OrthoTransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_StatusUI_Texture", m_mapComponent, ID_STATIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);

	return S_OK;
}

CStatus_UI * CStatus_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev, CTestPlayer * pPlayer)
{
	CStatus_UI*	pInstance = new CStatus_UI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pPlayer)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CStatus_UI::Free()
{
	CGameObject::Free();
}
