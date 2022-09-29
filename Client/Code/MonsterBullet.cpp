#include "stdafx.h"
#include "MonsterBullet.h"
#include "Export_Function.h"

#include "AbstractFactory.h"
#include "ObjectMgr.h"

CMonsterBullet::CMonsterBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBaseBullet(pGraphicDev)
{
}


CMonsterBullet::~CMonsterBullet()
{
}

HRESULT CMonsterBullet::Ready_Object(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	/*CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"TestPlayer", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, E_FAIL);
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	

	
	_vec3 vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_MoveDir = vPlayerPos - vPos;*/

	_vec3 vScale = { 0.5f,0.5f,0.5f };

	Set_MoveDir(L"Layer_GameLogic", L"TestPlayer", L"Proto_TransformCom", ID_DYNAMIC, &vPos,MONSTER_BULLET, &vScale);



	return S_OK;
}

_int CMonsterBullet::Update_Object(const _float & fTimeDelta)
{
	m_fFrame += 2.f * fTimeDelta;
	_vec3 vScale = { 0.5f,0.5f,0.5f };
	m_pTransCom->Set_Scale(&vScale);


	if (m_fFrame > 2.f)
	{
		CObjectMgr::GetInstance()->Collect_MonsterBulletObj(this);
		m_fFrame = 0.f;
		return 5;
	}
	
	
	
	m_pTransCom->Move_Pos(&(m_MoveDir * 50.f * fTimeDelta));


	Engine::CBaseBullet::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CMonsterBullet::LateUpdate_Object(void)
{
	Engine::CBaseBullet::LateUpdate_Object();
}

void CMonsterBullet::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	m_pTextureCom->Set_Texture(0);
	m_pCubeTexCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMonsterBullet::Add_Component(void)
{
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_BulletTexture", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pCubeTexCom = CAbstractFactory<CCubeTex>::Clone_Proto_Component(L"Proto_CubeTexCom", m_mapComponent, ID_STATIC);
	return S_OK;
}

CMonsterBullet * CMonsterBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CMonsterBullet *	pInstance = new CMonsterBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMonsterBullet::Free(void)
{
	Engine::CBaseBullet::Free();
}
