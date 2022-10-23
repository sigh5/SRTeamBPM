#include "stdafx.h"
#include "..\Header\MiniMonster2.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MiniPlayer.h"
#include "StaticCamera.h"

#include "Special_Effect.h"

CMiniMonster2::CMiniMonster2(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CMiniMonster2::~CMiniMonster2()
{
}

HRESULT CMiniMonster2::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimationCom->Ready_Animation(4, 1, 0.2f);
	m_pAttackAnimationCom->Ready_Animation(13, 0, 0.2f);

	_vec3 vPos = { 0.f, 1.f, 0.f };
	_vec3 vScale = { 1.f,1.f,1.f };
	m_pColliderCom->Set_vCenter(&vPos, &vScale);
	m_pColliderCom->Set_HitRadiuos(1.f);
	return S_OK;

}

_int CMiniMonster2::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
	{
		CScene* pScene = Get_Scene();
		CLayer* pLayer = pScene->GetLayer(L"Layer_GameLogic");

		_vec3 vPos;
		m_pTransCom->Get_Info(INFO_POS, &vPos);
		CGameObject* pGameObject = CSpecial_Effect::Create(m_pGraphicDev, vPos);
		if (pGameObject == nullptr)
			return OBJ_DEAD;

		pLayer->Add_EffectList(pGameObject);
		static_cast<CSpecial_Effect*>(pGameObject)->Set_Effect_INFO(OWNER_PALYER, 0, 17, 0.2f);
		::StopSound(SOUND_MONSTER);
		::PlaySoundW(L"Spider_death_01.wav", SOUND_MONSTER, g_fSound * 1.5f);
		return OBJ_DEAD;
	}
	m_fFrame = fTimeDelta;

	if (m_bChase)
		m_pAnimationCom->Move_Animation(fTimeDelta);

	if (m_bAttack)
		m_pAttackAnimationCom->Move_Animation(fTimeDelta);


	Judge_Active(fTimeDelta);

	m_pTransCom->Set_Y(1.f);

	_matrix matWorld;
	_vec3 vScale;
	vScale = m_pTransCom->Get_Scale();
	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pColliderCom->Set_HitBoxMatrix_With_Scale(&matWorld, vScale);

	_int iResult = Engine::CGameObject::Update_Object(fTimeDelta);



	Add_RenderGroup(RENDER_NONALPHA, this);

	return iResult;
}

void CMiniMonster2::LateUpdate_Object(void)
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

void CMiniMonster2::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (m_bChase)
		m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);

	if (m_bAttack)
		m_pAttackTextureCom->Set_Texture(m_pAttackAnimationCom->m_iMotion);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void CMiniMonster2::Collision_Event()
{
	if (!m_bAttack)
		return;

	if (m_pAttackAnimationCom->m_iMotion < 5)
		return;

	CScene* pScene = Get_Scene();
	CLayer* pLayer = pScene->GetLayer(L"Layer_GameLogic");
	CGameObject* pPlayer = pLayer->Get_GameObject(L"Player");
	CCollider* pCollider = dynamic_cast<CCollider*>(pPlayer->Get_Component(L"Proto_ColliderCom", ID_STATIC));

	if (m_pColliderCom->Check_CollisonUseCollider(pCollider, m_pColliderCom))
	{
		CStaticCamera* pCamera = static_cast<CStaticCamera*>(Get_GameObject(L"Layer_Environment", L"StaticCamera"));
		NULL_CHECK(pCamera);
		pCamera->Set_Shaking();
	}
}

_vec3 CMiniMonster2::Get_Pos()
{
	_vec3 vPos; 
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	return vPos;
}

void CMiniMonster2::Move_Pos(_vec3 _move)
{
	m_pTransCom->Move_Pos(&_move);
}


void CMiniMonster2::Judge_Active(const _float & fTimeDelta)
{
	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransformCom);


	_vec3		vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	_vec3		vMonsterPos;
	m_pTransCom->Get_Info(INFO_POS, &vMonsterPos);

	_int iActiveMotion = CalCulate_Distacne(pPlayerTransformCom, fTimeDelta);
	if (iActiveMotion == 0)
	{
		m_bAttack = false;
		m_bChase = true;

	}
	else if (iActiveMotion == 1)
	{
		m_bAttack = true;
		m_bChase = false;
	}
}



_int CMiniMonster2::CalCulate_Distacne(CTransform * pPlayerTransCom, const _float & fTimeDelta)
{
	_vec3		vPlayerPos;
	pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);

	_vec3		vMonsterPos;
	m_pTransCom->Get_Info(INFO_POS, &vMonsterPos);

	float fMtoPDistance; // 몬스터와 플레이어 간의 거리

	fMtoPDistance = sqrtf((powf(vPlayerPos.x - vMonsterPos.x, 2)
		+ powf(vPlayerPos.y - vMonsterPos.y, 2)
		+ powf(vPlayerPos.z - vMonsterPos.z, 2)));


	if (1.f >= fMtoPDistance && 0.f <= fMtoPDistance)
	{
		pPlayerTransCom->Set_Pos(vPlayerPos.x, vPlayerPos.y, vPlayerPos.z);
		return 1; //attack
	}

	m_pTransCom->Chase_Target_notRot(&vPlayerPos, 1.5f, fTimeDelta);
	return 0;	// chase
}

void CMiniMonster2::Set_Pos(const _vec3 & vPos)
{
	{ m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z); m_pTransCom->Update_Component(1.f); }
}

HRESULT CMiniMonster2::Add_Component()
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_MonsterTexture3", m_mapComponent, ID_STATIC);
	m_pGhulDigoutTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Ghul_dig_Texture", m_mapComponent, ID_STATIC);
	m_pAttackTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Spider_Attack_Texture", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pAttackAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);

	m_pAnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pAnimationCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_AnimationCom", m_pAnimationCom });

	return S_OK;
}

CMiniMonster2 * CMiniMonster2::Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx, float Posy)
{
	CMiniMonster2 *	pInstance = new CMiniMonster2(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMiniMonster2::Free(void)
{
	CGameObject::Free();
}
