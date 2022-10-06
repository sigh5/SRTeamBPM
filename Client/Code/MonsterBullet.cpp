#include "stdafx.h"
#include "MonsterBullet.h"
#include "Export_Function.h"

#include "AbstractFactory.h"
#include "ObjectMgr.h"
#include "MyCamera.h"

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
	
	m_pAnimationCom->Ready_Animation(13, 0, 0.1f);

	/*CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"TestPlayer", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, E_FAIL);
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	

	
	_vec3 vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_MoveDir = vPlayerPos - vPos;*/

	_vec3 vScale = { 0.5f,0.5f,0.5f };

	Set_MoveDir(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC, &vPos,MONSTER_BULLET, &vScale);



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
	
	m_pAnimationCom->Move_Animation(fTimeDelta);
	
	m_pTransCom->Move_Pos(&(m_MoveDir * 2.f * fTimeDelta));


	Engine::CBaseBullet::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CMonsterBullet::LateUpdate_Object(void)
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
	D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.5f);

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

	// 빌보드 에러 해결
	Engine::CGameObject::LateUpdate_Object();
}

void CMonsterBullet::Render_Obejct(void)
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

HRESULT CMonsterBullet::Add_Component(void)
{
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Fatbat_BulletTexture", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_DynamicTransformCom", m_mapComponent, ID_DYNAMIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	//m_pCubeTexCom = CAbstractFactory<CCubeTex>::Clone_Proto_Component(L"Proto_CubeTexCom", m_mapComponent, ID_STATIC);
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
