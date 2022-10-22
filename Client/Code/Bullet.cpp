#include "stdafx.h"
#include "..\Header\Bullet.h"
#include "Export_Function.h"

#include "AbstractFactory.h"
#include "MiniPlayer.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBaseBullet(pGraphicDev)
{
}


CBullet::~CBullet()
{
}

HRESULT CBullet::Ready_Object(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	_vec3 vScale = { 0.1f, 0.1f, 0.1f};
	m_pTransCom->Set_Scale(&vScale);

	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, E_FAIL);

	
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	pPlayerTransformCom->Get_Info(INFO_LOOK, &m_MoveDir);
	pPlayerTransformCom->Get_Info(INFO_RIGHT, &m_vRight);
	
	m_pColliderCom->Set_vCenter(&vPos, &vScale);
	m_pColliderCom->Set_HitRadiuos(0.5f);



	return S_OK;
}

_int CBullet::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fFrame += 1.f * fTimeDelta;

	if (m_fFrame >= 1.f)
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

void CBullet::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CBullet::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTextureCom->Set_Texture(0);
	m_pCubeTexCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void CBullet::Collision_Event()
{
	CScene* pScene = Get_Scene();
	CMiniPlayer* pPlayer = dynamic_cast<CMiniPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"));
	if(pPlayer==nullptr)
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
			m_bDead = true;
			pPlayer->Set_MonsterKillCount();
		}
	}

}

void CBullet::Set_Bullet_Dir(_int iDir)
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


HRESULT CBullet::Add_Component(void)
{

	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_BulletTexture", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pCubeTexCom = CAbstractFactory<CCubeTex>::Clone_Proto_Component(L"Proto_CubeTexCom", m_mapComponent, ID_STATIC);
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);


	return S_OK;
}

CBullet * CBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CBullet *	pInstance = new CBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBullet::Free(void)
{
	Engine::CBaseBullet::Free();
}
