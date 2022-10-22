#include "stdafx.h"
#include "..\Header\AnubisStormBall.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"
#include "Player.h"

CAnubisStormBall::CAnubisStormBall(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBaseBullet(pGraphicDev)
{
}


CAnubisStormBall::~CAnubisStormBall()
{
}

HRESULT CAnubisStormBall::Ready_Object(_vec3 vPos, _vec3 vTarget)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pAnimationCom->Ready_Animation(8, 0, 0.1f);

	/*CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"TestPlayer", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, E_FAIL);
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);



	_vec3 vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_MoveDir = vPlayerPos - vPos;*/

	_vec3 vScale = { 2.f,2.f,2.f };
	m_pTransCom->Set_Scale(&vScale);
	m_fLifetime = 2.f;
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	//Set_MoveDir(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC, &vPos, MONSTER_BULLET, &vScale);
	m_MoveDir = vTarget - vPos;

	m_pTransCom->Update_Component(1.f);
	return S_OK;
}

_int CAnubisStormBall::Update_Object(const _float & fTimeDelta)
{

	m_fLifetimeCount += fTimeDelta;



	if (m_fLifetime < m_fLifetimeCount)
	{
		m_bDead = true;
		return 1;
	}

	m_pAnimationCom->Move_Animation(fTimeDelta);

	_vec3 vThunderPos = m_pTransCom->m_vInfo[INFO_POS];
	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
	CCharacterInfo* pPlayerInfo = static_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC));
	_vec3 vPlayerPos = pPlayerTransformCom->m_vInfo[INFO_POS];
	CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	float fDistance = sqrtf((powf(vThunderPos.x - vPlayerPos.x, 2) + powf(vThunderPos.y - vPlayerPos.y, 2) + powf(vThunderPos.z - vPlayerPos.z, 2)));

	if (fDistance < 1.5f && false == m_bHitPlayer)
	{
		pPlayerInfo->Receive_Damage(10);
		pPlayer->Set_DefenseToHp(true);
		m_bHitPlayer = true;
	}

	m_pTransCom->Move_Pos(&(m_MoveDir * 3.f * fTimeDelta));

	Render_Obejct();

	m_pTransCom->Update_Component(fTimeDelta);
	Engine::CBaseBullet::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_NONALPHA, this);
	LateUpdate_Object();
	return 0;
}

void CAnubisStormBall::LateUpdate_Object(void)
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

	// 빌보드 에러 해결
	Engine::CGameObject::LateUpdate_Object();
}

void CAnubisStormBall::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->LightEnable(0, FALSE);
	m_pGraphicDev->LightEnable(1, FALSE);
	m_pGraphicDev->LightEnable(2, FALSE);
	m_pGraphicDev->LightEnable(3, TRUE);

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

	m_pGraphicDev->LightEnable(0, TRUE);
	m_pGraphicDev->LightEnable(1, TRUE);
	m_pGraphicDev->LightEnable(2, TRUE);
	m_pGraphicDev->LightEnable(3, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

}

HRESULT CAnubisStormBall::Add_Component(void)
{
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Anubis_Stormball_Texture", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_DynamicTransformCom", m_mapComponent, ID_DYNAMIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	return S_OK;
}

CAnubisStormBall * CAnubisStormBall::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vTarget)
{
	CAnubisStormBall *	pInstance = new CAnubisStormBall(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos, vTarget)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CAnubisStormBall::Free(void)
{
	Engine::CBaseBullet::Free();
}
