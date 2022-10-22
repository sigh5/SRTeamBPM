#include "stdafx.h"
#include "..\Header\Tentacle.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"
#include "FinalBoss.h"
#include "Player.h"

CTentacle::CTentacle(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}


CTentacle::~CTentacle()
{
}

HRESULT CTentacle::Ready_Object(float Posx, float Posy, CGameObject* pMaster, float Wait)
{
	m_pTexture = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Thingy_spike_Texture", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransform = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_STATIC);

	m_pAnimationCom->Ready_Animation(19, 0, 0.17f);
	
	m_pTransform->Set_Scale(&_vec3(2.f, 4.f, 2.f));

	m_pTransform->Set_Pos(Posx, m_pTransform->m_vScale.y * 0.5f, Posy);

	m_pTransform->Update_Component(1.f);
	m_pMaster = pMaster;
	m_pMasterTransform = static_cast<CTransform*>(pMaster->Get_Component(L"Proto_DynamicTransformCom", ID_DYNAMIC));
	m_pMasterInfo = static_cast<CCharacterInfo*>(pMaster->Get_Component(L"Proto_CharacterInfoCom", ID_STATIC));
	
	return S_OK;
}

_int CTentacle::Update_Object(const _float & fTimeDelta)
{
	if (m_bAttackFinish)
	{
		m_pAnimationCom->Move_Animation(fTimeDelta);

		if (m_pAnimationCom->m_iMaxMotion == m_pAnimationCom->m_iMotion)
		{
			m_bDead = true;
			return OBJ_DEAD;
		}
	m_pTransform->Update_Component(1.f);
	Engine::CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
	}
	if (m_pAnimationCom->m_iMaxMotion == m_pAnimationCom->m_iMotion)
	{
		m_bDead = true;
		return OBJ_DEAD;
	}
	m_bAttackFinish = static_cast<CFinalBoss*>(m_pMaster)->Get_Pettern3Finish();
	_vec3 vThunderPos = m_pTransform->m_vInfo[INFO_POS];
	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
	CCharacterInfo* pPlayerInfo = static_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC));
	_vec3 vPlayerPos = pPlayerTransformCom->m_vInfo[INFO_POS];
	CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	m_fToPlayerDistance = sqrtf((powf(vThunderPos.x - vPlayerPos.x, 2) + powf(vThunderPos.y - vPlayerPos.y, 2) + powf(vThunderPos.z - vPlayerPos.z, 2)));
	if (17 < m_pAnimationCom->m_iMotion && false == m_bAttackFinish)
	{
		static_cast<CFinalBoss*>(m_pMaster)->Set_Pettern3Finish();
	}
	if (m_pAnimationCom->m_iMotion < 3 || m_pAnimationCom->m_iMotion > 17)
	{
		m_fToPlayerDistance = 100;
	}
	else if (4 < m_pAnimationCom->m_iMotion && false == m_bOneTime)
	{
		if (m_fToPlayerDistance < 1.5f && false == m_bHitPlayer)
		{
			pPlayerInfo->Receive_Damage(10);
			pPlayer->Set_DefenseToHp(true);
			m_bHitPlayer = true;
			m_bOneTime = true;
		}
	}
	if (m_bHitPlayer && false == m_bAttackFinish)
	{
		//텐타클을 마스터에게
		_vec3 vOriginPos, vMasterPos, vDir;
		m_pTransform->Get_Info(INFO_POS, &vOriginPos);
		m_pMasterTransform->Get_Info(INFO_POS, &vMasterPos);
		vDir = vMasterPos - vOriginPos;
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransform->Move_Pos(&(vDir * 0.1f));
		_vec3 vTentaclePos;
		m_pTransform->Get_Info(INFO_POS, &vTentaclePos);
		
		pPlayerTransformCom->Set_Pos(vTentaclePos.x, vTentaclePos.y, vTentaclePos.z);
		//m_iMasterHp = m_pMasterInfo->Get_Hp();
		//if (m_iMasterPreHp > m_iMasterHp)
		//{
		//	m_iMasterHitNum++;
		//	m_iMasterPreHp = m_iMasterHp;
		//}

	}
	else
	{
		m_pAnimationCom->Move_Animation(fTimeDelta);
	}
	
	m_pTransform->Update_Component(1.f);
	Engine::CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CTentacle::LateUpdate_Object(void)
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
	D3DXMatrixScaling(&matScale, m_pTransform->m_vScale.x, m_pTransform->m_vScale.y, m_pTransform->m_vScale.z);

	_matrix      matRot;
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixRotationY(&matRot, (_float)pCamera->Get_BillBoardDir());

	_vec3 vPos;
	m_pTransform->Get_Info(INFO_POS, &vPos);

	D3DXMatrixTranslation(&matTrans,
		vPos.x,
		vPos.y,
		vPos.z);

	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale* matRot * matBill * matTrans;
	m_pTransform->Set_WorldMatrix(&(matWorld));

	// 빌보드 에러 해결
	Engine::CGameObject::LateUpdate_Object();
}

void CTentacle::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTexture->Set_Texture(m_pAnimationCom->m_iMotion);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CTentacle * CTentacle::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pMaster, float wait, float Posx, float Posy)
{
	CTentacle*	pInstance = new CTentacle(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(Posx, Posy, pMaster, wait)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTentacle::Free(void)
{
	CGameObject::Free();
}
