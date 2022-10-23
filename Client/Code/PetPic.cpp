#include "stdafx.h"
#include "..\Header\PetPic.h"
#include "Export_Function.h"
#include "AbstractFactory.h"
#include "ShopUI.h"
#include "Player_Dead_UI.h"

CPetPic::CPetPic(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
}

CPetPic::CPetPic(const CUI_Base & rhs)
	: CUI_Base(rhs)
{
}

CPetPic::~CPetPic()
{
}

HRESULT CPetPic::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_OrthoMatrix(512.f, 512.f, 0.f, 0.f);

	m_vecScale = { m_fSizeX * 0.35f, m_fSizeY * 0.48f, 1.f };

	m_pTransCom->Set_Scale(&m_vecScale);
	m_pTransCom->Set_Pos(m_fX + 279.f, m_fY - 156.f, 0.07f);

	m_pAnimationCom->Ready_Animation(1, 0, 0.1f, 1);

	return S_OK;
}

_int CPetPic::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_pAnimationCom->Control_Animation(m_bSoldOut);


	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CPetPic::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CPetPic::Render_Obejct(void)
{
	CPlayer_Dead_UI* pDead_UI = static_cast<CPlayer_Dead_UI*>(Engine::Get_GameObject(L"Layer_UI", L"Dead_UI"));

	if (pDead_UI->Get_Render() == false && m_bRender)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

		_matrix		OldViewMatrix, OldProjMatrix;

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);

		_matrix		ViewMatrix;

		ViewMatrix = *D3DXMatrixIdentity(&ViewMatrix);

		_matrix		matProj;

		Get_ProjMatrix(&matProj);

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);


		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);

		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
}

HRESULT CPetPic::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Pet_Sold_Texture", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_DYNAMIC);

	return S_OK;
}

CPetPic * CPetPic::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPetPic* pInstance = new CPetPic(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CPetPic::Free(void)
{
	Engine::CGameObject::Free();
}
