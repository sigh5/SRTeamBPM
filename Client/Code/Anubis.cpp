#include "stdafx.h"
#include "..\Header\Anubis.h"

#include "Export_Function.h"

#include "AbstractFactory.h"
#include "MyCamera.h"
#include "Player.h"
#include "HitBlood.h"
#include "AnubisThunder.h"
#include "AnubisStormBall.h"
#include "HitEffect.h"

#include "Gun_Screen.h"
#include "AttackEffect.h"
#include "Coin.h"
#include "Key.h"

CAnubis::CAnubis(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonsterBase(pGraphicDev)
{
}


CAnubis::~CAnubis()
{
}

HRESULT CAnubis::Ready_Object(float Posx, float Posy)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_MonsterTexture", m_mapComponent, ID_STATIC);

	m_fHitDelay = 0.f;

	CComponent* pComponent = nullptr;
	
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	
	m_pAttackAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);

	m_pAttackTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Anubis_Attack_Texture", m_mapComponent, ID_STATIC);
	m_pDeadTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Anubis_Dead_Texture", m_mapComponent, ID_STATIC);

	m_iMonsterIndex = MONSTER_ANUBIS;
	m_fAttackDelay = 0.5f;

	_vec3	vScale = { 5.f,6.f,5.f };

	m_pDynamicTransCom->Set_Scale(&vScale);
	
	m_bOldPlayerPos = { 1.0f,1.0f,1.0f };
	m_pInfoCom->Ready_CharacterInfo(5, 10, 5.f);
	m_pAnimationCom->Ready_Animation(6, 1, 0.2f);
	m_iPreHp = (m_pInfoCom->Get_InfoRef()._iHp);
	m_pAttackAnimationCom->Ready_Animation(17, 0, 0.3f);
	m_pDeadAnimationCom->Ready_Animation(12, 0, 0.3f);
	m_iRepeatCharge = 4;
	m_iRepeatShoot = 8;
	if (Posx == 0 && Posy == 0) {}
	else
	{
		m_pDynamicTransCom->Set_Pos((float)Posx, 2.f, (float)Posy);
	}
	Save_OriginPos();
	m_pDynamicTransCom->Update_Component(1.f);
	
	// Control Room
	_vec3 vPos;
	m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);
	vScale = m_pDynamicTransCom->Get_Scale();
	m_pColliderCom->Set_vCenter(&vPos, &vScale);
	
	return S_OK;
}

bool	CAnubis::Dead_Judge(const _float& fTimeDelta)
{
	m_pDynamicTransCom->Set_Y(m_pDynamicTransCom->m_vScale.y * 0.5f);
	if (0 >= m_pInfoCom->Get_Hp())
	{
		if (false == m_bDead)
		{
			_int Hitsound = rand() % 2;
			switch (Hitsound)
			{
			case 0:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Anubis_Death_01.wav", SOUND_MONSTER, g_fSound);
				break;
			case 1:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Anubis_Death_02.wav", SOUND_MONSTER, g_fSound);
				break;
			}
			Drop_Item(rand() % 3);
		m_bDead = true;
		}
		//Safe_Release(m_pAttackAnimationCom);
	}
	if (m_bDead)
	{
		if (m_pDeadAnimationCom->m_iMotion<m_pDeadAnimationCom->m_iMaxMotion)
			m_pDeadAnimationCom->Move_Animation(fTimeDelta);
		Engine::CMonsterBase::Update_Object(fTimeDelta);
		Add_RenderGroup(RENDER_ALPHA, this);
		return true;
	}
	else
	{
		return false;
	}
}

_int CAnubis::Update_Object(const _float & fTimeDelta)
{
	// 맨위에있어야됌 리턴되면 안됌
	_matrix matWorld;
	_vec3 vScale;
	vScale = m_pDynamicTransCom->Get_Scale();
	m_pDynamicTransCom->Get_WorldMatrix(&matWorld);
	m_pColliderCom->Set_HitBoxMatrix_With_Scale(&matWorld, vScale);
	Engine::CMonsterBase::Update_Object(fTimeDelta);
	// 맨위에있어야됌 리턴되면 안됌
	m_pDynamicTransCom->Set_Y(m_pDynamicTransCom->m_vScale.y * 0.5f);
	CMonsterBase::Get_MonsterToPlayer_Distance(&fMtoPDistance);
	
	if (Distance_Over())
	{
		m_pAnimationCom->m_iMotion = 0;
		Engine::CMonsterBase::Update_Object(fTimeDelta);
		Add_RenderGroup(RENDER_ALPHA, this);

		return 0;
	}

	if (Dead_Judge(fTimeDelta))
	{
		return 0;
	}

	m_fTimeDelta = fTimeDelta;

	Hit_Delay_toZero();

	AttackJudge(fTimeDelta);

	if (m_bHit == false)
	{
		NoHit_Loop(fTimeDelta);
	}
	else
	{
		Hit_Loop(fTimeDelta);
	}

	for (auto iter = m_AnubisThunderlist.begin(); iter != m_AnubisThunderlist.end();)
	{
		_int iResult = 0;
		iResult = (*iter)->Update_Object(fTimeDelta);
		if (iResult == 1)
		{
			Safe_Release((*iter));
			iter = m_AnubisThunderlist.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	for (auto iter = m_AnubisStormballList.begin(); iter != m_AnubisStormballList.end();)
	{
		_int iResult = 0;
		iResult = (*iter)->Update_Object(fTimeDelta);
		if (iResult == 1)
		{
			Safe_Release((*iter));
			iter = m_AnubisStormballList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	// Control Room

	
	m_pDynamicTransCom->Update_Component(fTimeDelta);
	
	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CAnubis::LateUpdate_Object(void)
{
	// 빌보드 에러 해결
	if (SCENE_TOOLTEST != Get_Scene()->Get_SceneType())
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
		D3DXMatrixScaling(&matScale,m_pDynamicTransCom->m_vScale.x, m_pDynamicTransCom->m_vScale.y, m_pDynamicTransCom->m_vScale.z);

		_matrix      matRot;
		D3DXMatrixIdentity(&matRot);
		D3DXMatrixRotationY(&matRot, (_float)pCamera->Get_BillBoardDir());

		_vec3 vPos;
		m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);

		D3DXMatrixTranslation(&matTrans,
			vPos.x,
			vPos.y,
			vPos.z);

		D3DXMatrixIdentity(&matWorld);
		matWorld = matScale* matRot * matBill * matTrans;
		m_pDynamicTransCom->Set_WorldMatrix(&(matWorld));

		// 빌보드 에러 해결
	}
	Add_ColliderMonsterlist();

	

	Engine::CMonsterBase::LateUpdate_Object();
}

void CAnubis::Render_Obejct(void)
{
	Set_Light_Obj();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pDynamicTransCom->Get_WorldMatrixPointer());


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

	if (false == m_bDead)
	{
		if (m_bAttacking && false == m_bHit)
		{
			m_pAttackTextureCom->Set_Texture(m_pAttackAnimationCom->m_iMotion);
		}
		else
		{
			m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);	// 텍스처 정보 세팅을 우선적으로 한다.
		}	// 텍스처 정보 세팅을 우선적으로 한다.
	}
	else
	{
		m_pDeadTextureCom->Set_Texture(m_pDeadAnimationCom->m_iMotion);
	}
	SetUp_Material();
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->LightEnable(0, TRUE);
	m_pGraphicDev->LightEnable(1, TRUE);
	m_pGraphicDev->LightEnable(2, TRUE);
	m_pGraphicDev->LightEnable(3, FALSE);


}

void CAnubis::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	NULL_CHECK_RETURN(pLayer, );
	CGameObject *pGameObject = nullptr;
	pGameObject = static_cast<CGun_Screen*>(::Get_GameObject(L"Layer_UI", L"Gun"));
	_vec3	vPos;
	m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);

	if (static_cast<CGun_Screen*>(pGameObject)->Get_Shoot() &&
		fMtoPDistance < MAX_CROSSROAD + g_fRange  &&
		m_pColliderCom->Check_Lay_InterSect(m_pBufferCom, m_pDynamicTransCom, g_hWnd))
	{
		m_bHit = true;
		static_cast<CPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"))->Set_ComboCount(1);
		m_pInfoCom->Receive_Damage(1);
		cout << "Anubis " << m_pInfoCom->Get_InfoRef()._iHp << endl;
		static_cast<CGun_Screen*>(pGameObject)->Set_Shoot(false);
		READY_CREATE_EFFECT_VECTOR(pGameObject, CHitEffect, pLayer, m_pGraphicDev, vPos);
		static_cast<CHitEffect*>(pGameObject)->Set_Effect_INFO(OWNER_ANUBIS, 0, 7, 0.2f);

		if(false==m_bDead)
		{
		_int Hitsound = rand() % 3;
		switch (Hitsound)
		{
		case 0:
			::StopSound(SOUND_MONSTER);
			::PlaySoundW(L"Anubis_pain_01.wav", SOUND_MONSTER, g_fSound);
			break;

		case 1:
			::StopSound(SOUND_MONSTER);
			::PlaySoundW(L"Anubis_pain_02.wav", SOUND_MONSTER, g_fSound);
			break;

		case 2:
			::StopSound(SOUND_MONSTER);
			::PlaySoundW(L"Anubis_pain_03.wav", SOUND_MONSTER, g_fSound);
			break;
		}
		}
	}
}

void CAnubis::NoHit_Loop(const _float& fTimeDelta)
{
	if ( fMtoPDistance > 15.f && m_bAttacking == false)
	{
		m_pDynamicTransCom->Chase_Target_notRot(&m_vPlayerPos, m_pInfoCom->Get_InfoRef()._fSpeed, fTimeDelta);

		m_pAnimationCom->Move_Animation(fTimeDelta);

	}
	else
	{
		if (m_bAttack)
		{

			Attack(fTimeDelta);
		}
		else
		{
			m_pAnimationCom->m_iMotion = 0;
		}
	}
}

void CAnubis::Hit_Loop(const _float& fTimeDelta)
{
	// 피격 시 모션
	m_pAnimationCom->m_iMotion = 7;
	m_fHitDelay += fTimeDelta;
	if (m_fHitDelay > 1.5f)
	{

		m_bHit = false;
		m_fHitDelay = 0.f;
	}
}

void CAnubis::Attack_Thunder(const _float& fTimeDelta)
{
	m_pAttackAnimationCom->Move_Animation(fTimeDelta);
	CCharacterInfo* pPlayerInfo = static_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC));
	CTransform* pPlayerTransform = static_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
	_vec3 AnubisInfo;
	m_pDynamicTransCom->Get_Info(INFO_POS, &AnubisInfo);
	/*if (1 == m_pAttackAnimationCom->m_iMotion)
	{
		pPlayerTransform->Get_Info(INFO_POS, &m_bOldPlayerPos);

	}*/
	if (5 == m_pAttackAnimationCom->m_iMotion)
	{
		if (false == m_bCreateOneThunder)
		{
			_vec3 vDir = m_vPlayerPos - AnubisInfo;
			D3DXVec3Normalize(&vDir, &vDir);
			D3DXVec3Cross(&vDir, &_vec3(0.f, 1.f, 0.f), &vDir);

			CAnubisThunder* pThunder;
			pThunder = CAnubisThunder::Create(m_pGraphicDev, AnubisInfo.x + (vDir.x* 1.2f), m_pDynamicTransCom->m_vScale.y * 0.85f, AnubisInfo.z + (vDir.z* 1.2f));
			::StopSound(SOUND_EFFECT);
			::PlaySoundW(L"thunder_spell_loop.wav", SOUND_EFFECT, g_fSound);

			m_AnubisThunderlist.push_back(pThunder);
			m_bCreateOneThunder = true;
		}
	}
	if (11 == m_pAttackAnimationCom->m_iMotion)
	{
		if (false == m_bCreateTwoThunder)
		{
			_vec3 vDir = m_vPlayerPos - AnubisInfo;
			D3DXVec3Normalize(&vDir, &vDir);
			CAnubisThunder* pThunder;
			pThunder = CAnubisThunder::Create(m_pGraphicDev, m_vPlayerPos.x - vDir.x * 0.5f, 1.0f, m_vPlayerPos.z - vDir.z * 0.5f);
			::StopSound(SOUND_EFFECT);
			::PlaySoundW(L"thunder_spell_cast.wav", SOUND_EFFECT, g_fSound);

			m_AnubisThunderlist.push_back(pThunder);
			m_bCreateTwoThunder = true;
		}
	}
	if (m_pAttackAnimationCom->m_iMotion >= m_pAttackAnimationCom->m_iMaxMotion)
	{
		m_bAttack = false;
		m_bCreateOneThunder = false;
		m_bCreateTwoThunder = false;
	}
}

void CAnubis::Attack_Stormball(const _float& fTimeDelta)
{
	m_pAttackAnimationCom->Move_Animation(fTimeDelta);
	_vec3 AnubisInfo;
	m_pDynamicTransCom->Get_Info(INFO_POS, &AnubisInfo);
	if (5 == m_pAttackAnimationCom->m_iMotion)
	{
		if (false == m_bCreateChargThunder)
		{
			_vec3 vDir = m_vPlayerPos - AnubisInfo;
			D3DXVec3Normalize(&vDir, &vDir);
			D3DXVec3Cross(&vDir, &_vec3(0.f, 1.f, 0.f), &vDir);

			CAnubisThunder* pThunder;
			pThunder = CAnubisThunder::Create(m_pGraphicDev, AnubisInfo.x + (vDir.x* 1.2f), m_pDynamicTransCom->m_vScale.y * 0.85f, AnubisInfo.z + (vDir.z* 1.2f));
			//::StopSound(SOUND_EFFECT);
			::PlaySoundW(L"thunder_spell_loop.wav", SOUND_EFFECT, g_fSound);

			m_AnubisThunderlist.push_back(pThunder);
			m_bCreateChargThunder = true;
		}
	}
	if (8 == m_pAttackAnimationCom->m_iMotion)
	{
		if (m_iRepeatChargeNum < m_iRepeatCharge)
		{
			m_pAttackAnimationCom->m_iMotion = 5;
			++m_iRepeatChargeNum;
			m_bCreateChargThunder = false;
		}
	}
	if (10 == m_pAttackAnimationCom->m_iMotion)
	{
		//스톰볼 생성
		if (false == m_bCreateOneStormball)
		{
			_vec3 vDir = m_vPlayerPos - AnubisInfo;
			D3DXVec3Normalize(&vDir, &vDir);
			D3DXVec3Cross(&vDir, &_vec3(0.f, 1.f, 0.f), &vDir);
			_vec3 vSpawnPos = _vec3((AnubisInfo.x + (vDir.x* 1.2f)), (m_pDynamicTransCom->m_vScale.y * 0.8f), (AnubisInfo.z + (vDir.z * 1.2f)));
			_int Stormballsound = rand() % 3;
			switch (Stormballsound)
			{
			case 0:
				::StopSound(SOUND_EFFECT);
				::PlaySoundW(L"lightning_strike_1.wav", SOUND_EFFECT, g_fSound);
				break;

			case 1:
				::StopSound(SOUND_EFFECT);
				::PlaySoundW(L"lightning_strike_2.wav", SOUND_EFFECT, g_fSound);
				break;

			case 2:
				::StopSound(SOUND_EFFECT);
				::PlaySoundW(L"lightning_strike_3.wav", SOUND_EFFECT, g_fSound);
				break;
			}

			if (m_bStormballLeftRight)
			{
				CAnubisStormBall* pStormball;
				pStormball = CAnubisStormBall::Create(m_pGraphicDev, vSpawnPos, (m_vPlayerPos - vDir * 1.4f));
				m_bStormballLeftRight = false;
				m_AnubisStormballList.push_back(pStormball);
			}
			else
			{
				CAnubisStormBall* pStormball;
				pStormball = CAnubisStormBall::Create(m_pGraphicDev, vSpawnPos, (m_vPlayerPos + vDir * 1.4f));
				m_bStormballLeftRight = true;
				m_AnubisStormballList.push_back(pStormball);
			}
			m_bCreateOneStormball = true;
		}
	}
	if (12 == m_pAttackAnimationCom->m_iMotion)
	{
		if (m_iRepeatShootNum < m_iRepeatShoot)
		{
			m_pAttackAnimationCom->m_iMotion = 9;
			++m_iRepeatShootNum;
			m_bCreateOneStormball = false;
		}
	}
	if (m_pAttackAnimationCom->m_iMotion >= m_pAttackAnimationCom->m_iMaxMotion)
	{
		m_bAttack = false;
		m_iRepeatChargeNum = 0;
		m_iRepeatShootNum = 0;
		m_bCreateChargThunder = false;
	}
}

void				CAnubis::Clear_Blood(const _float& fTimeDelta)
{
	//for (auto iter = m_vecBlood.front(); iter != m_vecBlood.back();)
	//{
	//	//if(static_cast<CHitBlood*>(&(*iter))->Get_Motion())
	//	if (2 == iter->Update_Object(fTimeDelta))
	//	{
	//	}
	//	else
	//	{
	//		++iter;
	//	}
	//} //코드병합으로 잠굼
}

HRESULT CAnubis::SetUp_Material(void)
{

	if (Get_Distance() < 30.f)
	{
		_float fDistance = ((1.f - Get_Distance() / 30.f))*2.f;
	
		max(fDistance, 0.1f);

		D3DMATERIAL9		tMtrl;
		ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

		tMtrl.Diffuse = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tMtrl.Specular = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tMtrl.Ambient = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tMtrl.Emissive = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tMtrl.Power = 0.f;

		m_pGraphicDev->SetMaterial(&tMtrl);

	}
	else
	{
		D3DMATERIAL9		tMtrl;
		ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

		tMtrl.Diffuse = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f);
		tMtrl.Specular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f);
		tMtrl.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f);
		tMtrl.Emissive = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f);
		tMtrl.Power = 0.f;

		m_pGraphicDev->SetMaterial(&tMtrl);
	}

	
	return S_OK;
}

void CAnubis::Set_Light_Obj()
{
	if (Get_Distance() >= 31.f)
	{
		D3DLIGHT9		tLightInfo4;
		ZeroMemory(&tLightInfo4, sizeof(D3DLIGHT9));
		FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo4, 3), );
		return;
	}

	if (Get_Distance() < 30.f)
	{
		_float fDistance = ((1.f - Get_Distance() / 30.f)) * 2.f;

		D3DLIGHT9		tLightInfo4;
		ZeroMemory(&tLightInfo4, sizeof(D3DLIGHT9));
		_vec3 vPos;
		m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);
		tLightInfo4.Type = D3DLIGHT_SPOT;
		tLightInfo4.Diffuse = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tLightInfo4.Specular = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tLightInfo4.Ambient = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);

		tLightInfo4.Position = vPos;
		FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo4, 3), );

	}



}

void		CAnubis::AttackJudge(const _float& fTimeDelta)
{
	if (m_bAttack == false)
	{
		m_fAttackDelayTime += fTimeDelta;
		if (m_fAttackDelay <= m_fAttackDelayTime)
		{
			m_bAttack = true;
			m_iAttackPattern = rand() % 2; //rand
			m_fAttackDelayTime = 0.f;
		}
	}
	if (false == m_bHit)
	{
		if (m_pAttackAnimationCom->m_iMotion<m_pAttackAnimationCom->m_iMaxMotion
			&& m_pAttackAnimationCom->m_iMotion>m_pAttackAnimationCom->m_iMinMotion)
		{
			m_bAttacking = true;
		}
		else
		{
			m_bAttacking = false;
		}
	}
	else
		m_pAttackAnimationCom->m_iMotion = 0;
}

void CAnubis::Attack(const _float& fTimeDelta)
{


	switch (m_iAttackPattern)
	{
	case 0:
		Attack_Thunder(fTimeDelta);
		break;

	case 1:
		Attack_Stormball(fTimeDelta);
		break;

	}

	
}
void	CAnubis::Drop_Item(int ItemType)
{
	CScene  *pScene = ::Get_Scene();
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	CGameObject* pItem = nullptr;
	switch (ItemType)
	{
	case 0:
		pItem = CCoin::Create(m_pGraphicDev, (_uint)m_pDynamicTransCom->m_vInfo[INFO_POS].x, (_uint)m_pDynamicTransCom->m_vInfo[INFO_POS].z);
		pLayer->Add_DropItemList(pItem);
		break;

	case 1:
		pItem = CKey::Create(m_pGraphicDev, (_uint)m_pDynamicTransCom->m_vInfo[INFO_POS].x, (_uint)m_pDynamicTransCom->m_vInfo[INFO_POS].z);
		pLayer->Add_DropItemList(pItem);
		break;

	default:
		break;
	}
}

CAnubis * CAnubis::Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx, int Posy)
{
	CAnubis*	pInstance = new CAnubis(pGraphicDev);


	if (FAILED(pInstance->Ready_Object((_float)Posx, (_float)Posy)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CAnubis::Free(void)
{
	CMonsterBase::Free();
	for (auto& iter : m_AnubisThunderlist)
	{
		Safe_Release(iter);
	}
	m_AnubisThunderlist.clear();
	for (auto& iter : m_AnubisStormballList)
	{
		Safe_Release(iter);
	}
	m_AnubisStormballList.clear();
}
