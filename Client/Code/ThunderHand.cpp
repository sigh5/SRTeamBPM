#include "stdafx.h"
#include "..\Header\ThunderHand.h"
#include "Export_Function.h"

#include "AbstractFactory.h"
#include "Player.h"
#include "SkillParticle.h"
#include "ControlRoom.h"

CThunderHand::CThunderHand(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
}

CThunderHand::~CThunderHand()
{
}

HRESULT CThunderHand::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_OrthoMatrix(300.f, 300.f, 0.f, 0.f);
	m_vecScale = { m_fSizeX * 1.6f, m_fSizeY * 2.f, 1.f };

	m_pTransCom->Set_Scale(&m_vecScale);
	m_pTransCom->Set_Pos(m_fX + 174.f, m_fY - 300.f, 0.1f);

	m_pAnimationCom->Ready_Animation(4, 0, 0.25f);




	return S_OK;
}

_int CThunderHand::Update_Object(const _float & fTimeDelta)
{
	if (!m_bBuySkill)
		return 0;

	if (m_bOnce)
	{
		CScene* pScene = Get_Scene();
		CLayer* pLayer = pScene->GetLayer(L"Layer_UI");

		_vec3 vPos;
		m_pTransCom->Get_Info(INFO_POS, &vPos);

		m_pParicle = CSkillParticle::Create(m_pGraphicDev, vPos);
		NULL_CHECK_RETURN(m_pParicle, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkillParticle", m_pParicle), E_FAIL);
		m_bOnce = false;
	}


	if (m_bActive)
	{
		m_fActiveTimer += 1.f*fTimeDelta;
		m_pAnimationCom->Move_Animation(fTimeDelta);
		m_pParicle->Set_Active(true);
	}

	if (m_fActiveTimer >= 1.f)
	{
		m_bActive = false;
		m_fActiveTimer = 0.f;

		CScene* pScene = Get_Scene();
		CLayer* pLayer = pScene->GetLayer(L"Layer_Room");

		for (auto iter : pLayer->Get_ControlRoomList())
		{
			static_cast<CControlRoom*>(iter)->Area_of_Effect();
		}
	}

	Engine::CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CThunderHand::LateUpdate_Object(void)
{
	if (!m_bBuySkill)
		return;
	CGameObject::LateUpdate_Object();
}

void CThunderHand::Render_Obejct(void)
{
	if (!m_bBuySkill)
		return;
	if (!m_bActive)
		return;
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
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

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

HRESULT CThunderHand::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<COrthoTransform>::Clone_Proto_Component(L"Proto_OrthoTransformCom", m_mapComponent, ID_DYNAMIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Skill_ScreenTexture", m_mapComponent, ID_STATIC);
	return S_OK;
}

CThunderHand * CThunderHand::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CThunderHand* pInstance = new CThunderHand(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CThunderHand::Free(void)
{
	CGameObject::Free();
}
