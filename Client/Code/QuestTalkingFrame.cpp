#include "stdafx.h"
#include "..\Header\QuestTalkingFrame.h"

#include "Export_Function.h"




CQuestTalkingFrame::CQuestTalkingFrame(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
	D3DXVec3Normalize(&m_vecScale, &m_vecScale);
}

CQuestTalkingFrame::~CQuestTalkingFrame()
{
}

HRESULT CQuestTalkingFrame::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.f, 1.f);

	m_fSizeX = 600.f;
	m_fSizeY = 250.f;

	m_fX = WINCX / 2.f;
	m_fY = WINCY / 2.f +300;

	_vec3 vScale = { m_fSizeX, m_fSizeY, 1.f };
	m_pTransCom->Set_Scale(&vScale);

	m_pTransCom->Set_Pos(m_fX - WINCX * 0.5f,
		(-m_fY + WINCY * 0.5f), 0.0f);


	return S_OK;
}

_int CQuestTalkingFrame::Update_Object(const _float & fTimeDelta)
{

	if (m_bActive)
		Create_Quest();

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CQuestTalkingFrame::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CQuestTalkingFrame::Render_Obejct(void)
{
	if (m_bActive == false)
		return;

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

	
	Render_Font(L"LeeSoonSin", pString, &_vec2(_float(WINCX / 2 - 300), _float(WINCY / 2 + 200.f)), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));


	m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CQuestTalkingFrame::Add_Component(void)
{
	ADD_CLONE_PROTO(CRcTex, m_pBufferCom, m_mapComponent, ID_STATIC, L"Proto_RcTexCom");
	ADD_CLONE_PROTO(CTransform, m_pTransCom, m_mapComponent, ID_DYNAMIC, L"Proto_TransformCom");
	ADD_CLONE_PROTO(CCalculator, m_pCalculatorCom, m_mapComponent, ID_STATIC, L"Proto_CalculatorCom");
	ADD_CLONE_PROTO(CTexture, m_pTextureCom, m_mapComponent, ID_STATIC, L"Proto_QuestTalking_Frame_Texture"); //L"Proto_QuestTalking_Frame_Texture");

	return S_OK;
}

void CQuestTalkingFrame::Create_Quest()
{
	if (Key_Down(DIK_X))
	{
		++m_iTalkNum;
	}


	if (m_iTalkNum == 0)
		pString = L"어이!";

	else if (m_iTalkNum == 1)
		pString = L"미니게임을 통해 돈을 얻어서\n 스킬을 살 수 있어!";

	else if (m_iTalkNum == 2)
		pString = L"나 대신 해줘!!";

	else if (m_iTalkNum == 3)
		pString = L"안돼 해줘!";


	else if (m_iTalkNum == 4)
		pString = L"해줘..";


	else if (m_iTalkNum == 5)
	{
		m_iTalkNum = 0;

		m_bActive = false;

		CScene  *pScene = ::Get_Scene();
		NULL_CHECK_RETURN(pScene, );
		CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
		NULL_CHECK_RETURN(pLayer, );
		pLayer = pScene->GetLayer(L"Layer_UI");
		CQuestTalkingFrame *pFrame = dynamic_cast<CQuestTalkingFrame*>(pLayer->Get_GameObject(L"QuestUIFrame"));
	}

}

CQuestTalkingFrame * CQuestTalkingFrame::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CQuestTalkingFrame* pInstance = new CQuestTalkingFrame(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}
	return pInstance;
}


void CQuestTalkingFrame::Free(void)
{
	CGameObject::Free();
}
