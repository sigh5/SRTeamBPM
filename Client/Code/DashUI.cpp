#include "stdafx.h"
#include "..\Header\DashUI.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "Player.h"
#include "Player_Dead_UI.h"
USING(Engine)

CDashUI::CDashUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
	D3DXVec3Normalize(&m_vecScale, &m_vecScale);
}

CDashUI::CDashUI(const CUI_Base & rhs)
	: CUI_Base(rhs)
{
}

CDashUI::~CDashUI()
{
}

HRESULT CDashUI::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_OrthoMatrix(300.f, 300.f, 0.f, 0.f);

	m_vecScale = { m_fSizeX * 0.3f, m_fSizeY * 0.3f, 1.f };

	m_pTransCom->Set_Scale(&m_vecScale);
	m_pTransCom->Set_Pos(m_fX - 194.f, m_fY - WINCY * 0.416f, 0.1f);

	m_fDelay = 15.f;
	
	return S_OK;
}

_int CDashUI::Update_Object(const _float & fTimeDelta)
{		
	CScene* pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, E_FAIL);
	CLayer* pLayer = pScene->GetLayer(L"Layer_GameLogic");
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	CGameObject* pGameObject = nullptr;

	pGameObject = pLayer->Get_GameObject(L"Player");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);	
	

	if (Key_Down(DIK_LSHIFT))//(Engine::Get_DIKeyState(DIK_LSHIFT) & 0x80)
	{
		m_bSize = true;

		_vec3	vecLSHIFT = { m_vecScale.x * 0.7f, m_vecScale.y * 0.7f, 0.1f };
		m_pTransCom->Set_Scale(&vecLSHIFT);
	}

	::Key_InputReset();

		if(m_bSize)
		m_fDelayTime += 50.f * fTimeDelta;

		if (m_fDelay < m_fDelayTime)
		{
			m_pTransCom->Set_Scale(&m_vecScale);
			m_fDelayTime = 0.f;
			m_bSize = false;
		}
			

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_ICON, this);

	return 0;
}

void CDashUI::LateUpdate_Object(void)
{
	
	CGameObject::LateUpdate_Object();
}

void CDashUI::Render_Obejct(void)
{
	CPlayer_Dead_UI* pDead_UI = static_cast<CPlayer_Dead_UI*>(Engine::Get_GameObject(L"Layer_UI", L"Dead_UI"));

	if (pDead_UI->Get_Render() == false)
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

HRESULT CDashUI::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<COrthoTransform>::Clone_Proto_Component(L"Proto_OrthoTransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Dash_UI_Texture", m_mapComponent, ID_STATIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);

	return S_OK;
}

CDashUI * CDashUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDashUI* pInstance = new CDashUI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CDashUI::Free()
{
	CGameObject::Free();
}
