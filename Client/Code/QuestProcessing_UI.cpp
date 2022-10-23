#include "stdafx.h"
#include "..\Header\QuestProcessing_UI.h"

#include "Export_Function.h"


CQuestProcessing_UI::CQuestProcessing_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
}

CQuestProcessing_UI::~CQuestProcessing_UI()
{
}

HRESULT CQuestProcessing_UI::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.f, 1.f);

	m_fSizeX = 200.f;
	m_fSizeY = 300.f;

	m_fX = WINCX / 2.f +500;
	m_fY = WINCY / 2.f -150;

	_vec3 vScale = { m_fSizeX, m_fSizeY, 1.f };
	m_pTransCom->Set_Scale(&vScale);

	m_pTransCom->Set_Pos(m_fX - WINCX * 0.5f,
		(-m_fY + WINCY * 0.5f), 0.0f);


	return S_OK;
}

_int CQuestProcessing_UI::Update_Object(const _float & fTimeDelta)
{
	if (m_bActive)
	{
		m_fFrame += 1.f*fTimeDelta;
	}

	if (m_fFrame >= 3.f)
	{
		return OBJ_DEAD;
	}

	if (m_bMini_Game_Clear_Check[0] && m_bMini_Game_Clear_Check[1] )
	{
		m_bQuestClear = true;
	}



	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CQuestProcessing_UI::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CQuestProcessing_UI::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	_matrix		OldViewMatrix, OldProjMatrix;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);

	_matrix		ViewMatrix;
	ViewMatrix = *D3DXMatrixIdentity(&ViewMatrix);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);


	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	Render_Font(L"DalseoHealingBold", L"미니게임1\n보상 100골드", &_vec2(_float(WINCX / 2 + 400), _float(WINCY /2.f - 280.f)), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
	if(m_bMini_Game_Clear_Check[Quest_Index_ONE])
		Render_Font(L"DalseoHealingBold", L"------\n---------", &_vec2(_float(WINCX / 2 + 400), _float(WINCY / 2.f - 280.f)), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	
	Render_Font(L"DalseoHealingBold", L"미니게임2\n 보상 100골드", &_vec2(_float(WINCX / 2 + 400), _float(WINCY / 2.f - 240.f)), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	if (m_bMini_Game_Clear_Check[Quest_Index_TWO])
		Render_Font(L"DalseoHealingBold", L"------\n---------", &_vec2(_float(WINCX / 2 + 400), _float(WINCY / 2.f - 240.f)), D3DXCOLOR(0.f, 0.f, 1.f, 1.f));


	if(m_bQuestClear)
		Render_Font(L"DalseoHealingBold", L"퀘스트클리어!!\n다시 말걸기", &_vec2(_float(WINCX / 2 + 400), _float(WINCY / 2.f - 200.f)), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));



	m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

 _bool  CQuestProcessing_UI::MiniGameCheck()
{
	if (m_bMini_Game_Clear_Check[0] && m_bMini_Game_Clear_Check[1])
		return true;

	return false;
}

HRESULT CQuestProcessing_UI::Add_Component(void)
{
	ADD_CLONE_PROTO(CRcTex, m_pBufferCom, m_mapComponent, ID_STATIC, L"Proto_RcTexCom");
	ADD_CLONE_PROTO(CTransform, m_pTransCom, m_mapComponent, ID_DYNAMIC, L"Proto_TransformCom");
	ADD_CLONE_PROTO(CCalculator, m_pCalculatorCom, m_mapComponent, ID_STATIC, L"Proto_CalculatorCom");
	ADD_CLONE_PROTO(CTexture, m_pTextureCom, m_mapComponent, ID_STATIC, L"Proto_QuestTalking_Frame_Texture"); //L"Proto_QuestTalking_Frame_Texture");

	return S_OK;
}

void CQuestProcessing_UI::Current_QuestString()
{
	// 현재 폰트적기
}

CQuestProcessing_UI * CQuestProcessing_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CQuestProcessing_UI* pInstance = new CQuestProcessing_UI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}
	return pInstance;
}

void CQuestProcessing_UI::Free(void)
{
	CGameObject::Free();
}
