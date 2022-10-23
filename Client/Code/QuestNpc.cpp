#include "stdafx.h"
#include "..\Header\QuestNpc.h"

#include "Export_Function.h"
#include "MyCamera.h"
#include "Player.h"
#include "QuestTalkingFrame.h"
#include "QuestProcessing_UI.h"

CQuestNpc::CQuestNpc(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CQuestNpc::~CQuestNpc()
{
}

HRESULT CQuestNpc::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	_vec3 vScale = { 4.f,4.f,4.f };

	m_pTransCom->Set_Scale(&vScale);
	m_pTransCom->Set_Pos(335.f, 2.f, 315.f);
	m_pTransCom->Compulsion_Update();
	m_pAnimationCom->Ready_Animation(7, 0, 0.2f);
	return S_OK;
}

_int CQuestNpc::Update_Object(const _float & fTimeDelta)
{
	m_pAnimationCom->Move_Animation(fTimeDelta);

	_uint iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_ALPHA, this);

	return iResult;
}

void CQuestNpc::LateUpdate_Object(void)
{
	CMyCamera* pCamera = static_cast<CMyCamera*>(Get_GameObject(L"Layer_Environment", L"CMyCamera"));
	NULL_CHECK(pCamera);

	_matrix		matWorld, matView, matBill;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);
	memcpy(&matBill, &matView, sizeof(_matrix));
	memset(&matBill._41, 0, sizeof(_vec3));
	D3DXMatrixInverse(&matBill, 0, &matBill);

	_matrix      matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_pTransCom->m_vScale.x, m_pTransCom->m_vScale.y, m_pTransCom->m_vScale.z);

	_matrix      matRot;
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixRotationY(&matRot, (_float)pCamera->Get_BillBoardDir());

	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	D3DXMatrixTranslation(&matTrans,
		vPos.x,
		vPos.y,
		vPos.z);

	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale* matRot * matBill * matTrans;
	m_pTransCom->Set_WorldMatrix(&(matWorld));
}

void CQuestNpc::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->LightEnable(0, FALSE);
	m_pGraphicDev->LightEnable(1, FALSE);
	m_pGraphicDev->LightEnable(2, FALSE);
	m_pGraphicDev->LightEnable(3, FALSE);
	m_pGraphicDev->LightEnable(4, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	

	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);	// 텍스처 정보 세팅을 우선적으로 한다.
	SetUp_Material();
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	m_pGraphicDev->LightEnable(0, TRUE);
	m_pGraphicDev->LightEnable(1, TRUE);
	m_pGraphicDev->LightEnable(2, TRUE);
	m_pGraphicDev->LightEnable(3, TRUE);
	m_pGraphicDev->LightEnable(4, FALSE);
}

void CQuestNpc::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	NULL_CHECK_RETURN(pLayer, );
	CGameObject *pGameObject = nullptr;

	pGameObject = pLayer->Get_GameObject(L"Player");
	NULL_CHECK_RETURN(pGameObject, );
	CTransform *pTransform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Proto_DynamicTransformCom", ID_DYNAMIC));

	if (m_pColliderCom->Check_Collision(this, pGameObject, 4, 4))
	{
		if (Key_Down(DIK_C))
		{
			pLayer = pScene->GetLayer(L"Layer_UI");
			if (Quest_clear(pLayer))
				return;

			m_bPlayerInterection = !m_bPlayerInterection;
			CQuestTalkingFrame *pFrame = dynamic_cast<CQuestTalkingFrame*>(pLayer->Get_GameObject(L"QuestUIFrame"));
			pFrame->Set_Active(m_bPlayerInterection);
		}
	}
}

HRESULT CQuestNpc::SetUp_Material(void)
{
	_vec3 vPlayerPos, vPos;

	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom,E_FAIL);

	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	_float fMtoPDistance = sqrtf((powf(vPlayerPos.x - vPos.x, 2)
		+ powf(vPlayerPos.y - vPos.y, 2)
		+ powf(vPlayerPos.z - vPos.z, 2)));


	D3DMATERIAL9		tMtrl;
	if (fMtoPDistance < 30.f)
	{
		_float fDistance = ((1.f - fMtoPDistance / 30.f)) + 0.5f;
		max(fDistance, 0.5f);
		ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));
		tMtrl.Diffuse = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tMtrl.Specular = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tMtrl.Ambient = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tMtrl.Emissive = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tMtrl.Power = 0.f;
	}
	else
	{
		_float fDistance = 0.4f;
		ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));
		tMtrl.Diffuse = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tMtrl.Specular = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tMtrl.Ambient = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tMtrl.Emissive = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tMtrl.Power = 0.f;
	}


	m_pGraphicDev->SetMaterial(&tMtrl);

	return S_OK;
}

void CQuestNpc::Set_Light_Obj()
{
	_vec3 vPlayerPos, vPos;

	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
	if (pPlayerTransformCom == nullptr)
		return;

	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	_float fMtoPDistance = sqrtf((powf(vPlayerPos.x - vPos.x, 2)
		+ powf(vPlayerPos.y - vPos.y, 2)
		+ powf(vPlayerPos.z - vPos.z, 2)));

	if (fMtoPDistance < 30.f)
	{
		_float fDistance = ((1.f - fMtoPDistance / 30.f)) * 2;
		max(fDistance, 0.1f);

		D3DLIGHT9		tLightInfo4;
		ZeroMemory(&tLightInfo4, sizeof(D3DLIGHT9));
		_vec3 vPos;
		m_pTransCom->Get_Info(INFO_POS, &vPos);
		tLightInfo4.Type = D3DLIGHT_SPOT;
		tLightInfo4.Diffuse = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tLightInfo4.Specular = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tLightInfo4.Ambient = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tLightInfo4.Position = vPos;
		FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo4, 4), );
	}
	else
	{
		return;
	}
}

bool CQuestNpc::Quest_clear(CLayer* pLayer)
{
	
	CQuestProcessing_UI* pQuestProcessing_UI =
		dynamic_cast<CQuestProcessing_UI*>(pLayer->Get_GameObject(L"QuestProcessing_UI"));

	if (pQuestProcessing_UI == nullptr)
		return false;

	if (pQuestProcessing_UI->MiniGameCheck())
	{
		pQuestProcessing_UI->Set_Active(true);
		CQuestTalkingFrame *pFrame = dynamic_cast<CQuestTalkingFrame*>(pLayer->Get_GameObject(L"QuestUIFrame"));
		pFrame->Set_Active(true);
		pFrame->Quest_Clear();
		return true;
	}



	return false;
}




HRESULT CQuestNpc::Add_Component(void)
{
	ADD_CLONE_PROTO(CRcTex, m_pBufferCom, m_mapComponent, ID_STATIC, L"Proto_RcTexCom");
	ADD_CLONE_PROTO(CTransform, m_pTransCom, m_mapComponent, ID_DYNAMIC, L"Proto_TransformCom");
	ADD_CLONE_PROTO(CCollider, m_pColliderCom, m_mapComponent, ID_STATIC, L"Proto_ColliderCom");
	ADD_CLONE_PROTO(CTexture, m_pTextureCom, m_mapComponent, ID_STATIC, L"Proto_QuestNPCTexture");
	ADD_CLONE_PROTO(CAnimation, m_pAnimationCom, m_mapComponent, ID_STATIC, L"Proto_AnimationCom");


	return S_OK;
}

CQuestNpc * CQuestNpc::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CQuestNpc*	pInstance = new CQuestNpc(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CQuestNpc::Free(void)
{
	CGameObject::Free();
}
