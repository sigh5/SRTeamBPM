#include "stdafx.h"
#include "..\Header\PiercingBullet.h"
#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MiniPlayer.h"
#include "StaticCamera.h"

CPiercingBullet::CPiercingBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBaseBullet(pGraphicDev)
{
}


CPiercingBullet::~CPiercingBullet()
{
}
HRESULT CPiercingBullet::Ready_Object(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3 vScale = { 1.f, 1.0f, 1.f };
	m_pTransCom->Set_Scale(&vScale);

	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, E_FAIL);


	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	pPlayerTransformCom->Get_Info(INFO_LOOK, &m_MoveDir);
	m_MoveDir.y = 0.f;
	pPlayerTransformCom->Get_Info(INFO_RIGHT, &m_vRight);
	m_vRight.y = 0.f;
	m_pColliderCom->Set_vCenter(&vPos, &vScale);
	m_pColliderCom->Set_HitRadiuos(0.5f);



	return S_OK;
}

_int CPiercingBullet::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fFrame += 1.f * fTimeDelta;

	if (m_fFrame >= 5.f)
	{
		m_fFrame = 0.f;
		return OBJ_DEAD;
	}

	if (m_iDir == 0)
		m_pTransCom->Move_Pos(&(m_MoveDir * m_fBulletSpeed * fTimeDelta));
	else if (m_iDir == 1)
	{
		m_pTransCom->Move_Pos(&(m_MoveDir * m_fBulletSpeed * fTimeDelta));
	}
	else if (m_iDir == 2)
	{
		m_pTransCom->Rotation(ROT_X, 30.f);
		m_pTransCom->Move_Pos(&(m_MoveDir * m_fBulletSpeed * fTimeDelta));
	}
	else if (m_iDir == 3)
	{
		m_pTransCom->Rotation(ROT_X, 60.f);
		m_pTransCom->Move_Pos(&(m_MoveDir * m_fBulletSpeed * fTimeDelta));
	}
	else if (m_iDir == 4)
	{
		m_pTransCom->Rotation(ROT_X, 90.f);
		m_pTransCom->Move_Pos(&(m_MoveDir * m_fBulletSpeed * fTimeDelta));
	}
	Add_RenderGroup(RENDER_NONALPHA, this);

	_matrix matWorld;
	_vec3 vScale;
	vScale = m_pTransCom->Get_Scale();
	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pColliderCom->Set_HitBoxMatrix_With_Scale(&matWorld, vScale);

	Engine::CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CPiercingBullet::LateUpdate_Object(void)
{

	CStaticCamera* pCamera = static_cast<CStaticCamera*>(Get_GameObject(L"Layer_Environment", L"StaticCamera"));
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

	Engine::CGameObject::LateUpdate_Object();
}

void CPiercingBullet::Render_Obejct(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void CPiercingBullet::Collision_Event()
{
	CScene* pScene = Get_Scene();
	CMiniPlayer* pPlayer = dynamic_cast<CMiniPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"));
	if (pPlayer == nullptr)
	{
		return;
	}
	CLayer* pLayer = pScene->GetLayer(L"Layer_Monster");

	for (auto iter = pLayer->Get_EffectList().begin(); iter != pLayer->Get_EffectList().end(); ++iter)
	{
		CCollider* pCollider = dynamic_cast<CCollider*>((*iter)->Get_Component(L"Proto_ColliderCom", ID_STATIC));
		if (m_pColliderCom->Check_CollisonUseCollider(pCollider, m_pColliderCom))
		{
			(*iter)->Set_ObjDead(true);

			pPlayer->Set_MonsterKillCount();
		}
	}

}

void CPiercingBullet::Set_Bullet_Dir(_int iDir)
{
	if (iDir % 4 == 0)
	{
		m_iDir = 1;
		m_MoveDir = m_vRight;
	}
	else if (iDir % 4 == 1)
	{
		m_iDir = 2;
		m_MoveDir += m_vRight;
		m_MoveDir /= 2;
	}
	else if (iDir % 4 == 2)
	{
		m_iDir = 3;
		m_MoveDir = -m_vRight;
	}
	else if (iDir % 4 == 3)
	{
		m_iDir = 4;
		m_MoveDir -= m_vRight;
		m_MoveDir /= 2;
	}




}


HRESULT CPiercingBullet::Add_Component(void)
{

	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Piercing_Bullet_Texture", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	//m_pCubeTexCom = CAbstractFactory<CCubeTex>::Clone_Proto_Component(L"Proto_CubeTexCom", m_mapComponent, ID_STATIC);

	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);

	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);


	return S_OK;
}

CPiercingBullet * CPiercingBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CPiercingBullet *	pInstance = new CPiercingBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPiercingBullet::Free(void)
{
	Engine::CBaseBullet::Free();
}
