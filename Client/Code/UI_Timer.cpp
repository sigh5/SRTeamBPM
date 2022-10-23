#include "stdafx.h"
#include "..\Header\UI_Timer.h"

#include "Export_Function.h"
#include "QuestProcessing_UI.h"
#include "MiniPlayer.h"

CUI_Timer::CUI_Timer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
	D3DXVec3Normalize(&m_vecScale, &m_vecScale);
}

CUI_Timer::~CUI_Timer()
{
}

HRESULT CUI_Timer::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.f, 1.f);

	m_fSizeX = 200.f;
	m_fSizeY = 150.f;

	m_fX = WINCX *0.5f;
	m_fY = WINCY*0.5f ;

	_vec3 vScale = { m_fSizeX, m_fSizeY, 1.f };
	m_pTransCom->Set_Scale(&vScale);

	m_pTransCom->Set_Pos(m_fX - WINCX * 0.5f + 500.f,
		(-m_fY + WINCY * 0.5f) +400.f, 0.0f);


	return S_OK;
}

_int CUI_Timer::Update_Object(const _float & fTimeDelta)
{
	m_fFrame += 1.f *fTimeDelta;
	_int Test = (_int)(m_fFrame);

	if (Test / 60 >= 1)
	{
		m_bFontCheck = true;
	}
	
	pString2 = std::to_wstring((Test / 60));
	
	pString = std::to_wstring((Test%60));


	if (m_bActive)
		Time_Over();


	CMiniPlayer*pPlayer = dynamic_cast<CMiniPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"));

	pString3 = std::to_wstring(pPlayer->Get_MonsterKillCount());


	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CUI_Timer::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CUI_Timer::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	_matrix		OldViewMatrix, OldProjMatrix;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);

	_matrix		ViewMatrix;

	ViewMatrix = *D3DXMatrixIdentity(&ViewMatrix);


	m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	/*m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);*/

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
	Render_Font(L"DalseoHealingBold",(L"목표 시간:2분"), &_vec2( WINCX - 230.f, 60.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	if (m_bFontCheck)
	{
		Render_Font(L"DalseoHealingBold", pString2.c_str(), &_vec2(WINCX - 150.f, 80.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		Render_Font(L"DalseoHealingBold", L"분 ", &_vec2(WINCX - 130.f, 80.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	Render_Font(L"DalseoHealingBold", pString.c_str(), &_vec2(WINCX - 100.f, 80.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	Render_Font(L"DalseoHealingBold", L"초 ", &_vec2(WINCX - 70.f, 80.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	
	Render_Font(L"DalseoHealingBold", L"잡은 수 ", &_vec2(WINCX - 230.f, 120.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	Render_Font(L"DalseoHealingBold", pString3.c_str() , &_vec2(WINCX - 100.f, 120.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CUI_Timer::Add_Component(void)
{
	ADD_CLONE_PROTO(CRcTex, m_pBufferCom, m_mapComponent, ID_STATIC, L"Proto_RcTexCom");
	ADD_CLONE_PROTO(CTransform, m_pTransCom, m_mapComponent, ID_DYNAMIC, L"Proto_TransformCom");
	ADD_CLONE_PROTO(CCalculator, m_pCalculatorCom, m_mapComponent, ID_STATIC, L"Proto_CalculatorCom");
	ADD_CLONE_PROTO(CTexture, m_pTextureCom, m_mapComponent, ID_STATIC, L"Proto_QuestTalking_Frame_Texture"); //L"Proto_QuestTalking_Frame_Texture");

	return S_OK;
}

void CUI_Timer::Time_Over()
{

}

CUI_Timer * CUI_Timer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Timer* pInstance = new CUI_Timer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}
	return pInstance;
}

void CUI_Timer::Free(void)
{
	CGameObject::Free();
}
