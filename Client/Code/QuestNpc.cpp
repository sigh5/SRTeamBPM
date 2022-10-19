#include "stdafx.h"
#include "..\Header\QuestNpc.h"

#include "Export_Function.h"
#include "MyCamera.h"
#include "Player.h"
#include "QuestTalkingFrame.h"

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
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	

	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);	// 텍스처 정보 세팅을 우선적으로 한다.

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
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

	if (m_pColliderCom->Check_Collision(this, pGameObject, 2, 2))
	{
		if (Key_Down(DIK_F))
		{
			m_bPlayerInterection = !m_bPlayerInterection;	

			pLayer = pScene->GetLayer(L"Layer_UI");
			
			CQuestTalkingFrame *pFrame = dynamic_cast<CQuestTalkingFrame*>(pLayer->Get_GameObject(L"QuestUIFrame"));
			pFrame->Set_Active(m_bPlayerInterection);

		}
	}
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
