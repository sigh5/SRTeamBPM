#include "stdafx.h"
#include "..\Header\Skill_UI.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "Player.h"
#include "Player_Dead_UI.h"

CSkill_UI::CSkill_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
}

CSkill_UI::CSkill_UI(const CUI_Base & rhs)
	: CUI_Base(rhs)
{
}

CSkill_UI::~CSkill_UI()
{
}

HRESULT CSkill_UI::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_OrthoMatrix(128.f, 128.f, 0.f, 0.f);

	m_vecScale = { m_fSizeX, m_fSizeY, 1.f };

	m_pTransCom->Set_Scale(&m_vecScale);
	m_pTransCom->Set_Pos(m_fX - 87.f, m_fY - 425.f, 0.1f);

	m_fDelay = 100.f;

	return S_OK;
}

_int CSkill_UI::Update_Object(const _float & fTimeDelta)
{
	CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

	if (m_bRshift)
	{
		m_bSize = true;

		_vec3	vecLSHIFT = { m_vecScale.x * 0.7f, m_vecScale.y * 0.7f, 0.1f };
		m_pTransCom->Set_Scale(&vecLSHIFT);

		if (m_bSize)
			m_fDelayTime += 50.f * fTimeDelta;

		if (m_fDelay < m_fDelayTime)
		{
			pPlayer->Set_Skill_CoolTime(true);
			m_pTransCom->Set_Scale(&m_vecScale);
			m_fDelayTime = 0.f;
			m_bSize = false;
			m_bRshift = false;
		}
	}

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_ICON, this);

	return 0;
}

void CSkill_UI::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CSkill_UI::Render_Obejct(void)
{
	CPlayer_Dead_UI* pDead_UI = static_cast<CPlayer_Dead_UI*>(Engine::Get_GameObject(L"Layer_UI", L"Dead_UI"));

	if (pDead_UI->Get_Render() == false)
	{
		if (m_bRB)
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
			m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xDF);
			m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


			m_pTextureCom->Set_Texture(0);	// 텍스처 정보 세팅을 우선적으로 한다.
			m_pBufferCom->Render_Buffer();

			m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

			m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		}
	}
}

HRESULT CSkill_UI::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Skill_Icon_Texture", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<COrthoTransform>::Clone_Proto_Component(L"Proto_OrthoTransformCom", m_mapComponent, ID_DYNAMIC);
	
	return S_OK;
}

CSkill_UI * CSkill_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkill_UI* pInstance = new CSkill_UI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSkill_UI::Free()
{
	Engine::CGameObject::Free();
}
