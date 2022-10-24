#include "stdafx.h"
#include "..\Header\FatBat.h"

#include "Export_Function.h"
#include "MonsterBullet.h"
#include "Stage.h"
#include "AbstractFactory.h"
#include "ObjectMgr.h"
#include "MyCamera.h"
#include "Player.h"
#include "HitEffect.h"

#include "Gun_Screen.h"
#include "Special_Effect.h"
#include "Coin.h"
#include "Key.h"

CFatBat::CFatBat(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonsterBase(pGraphicDev)
{
}


CFatBat::~CFatBat()
{
}

HRESULT CFatBat::Ready_Object(int Posx, int Posy)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_MonsterTexture2", m_mapComponent, ID_DYNAMIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pDeadTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Fatbat_Dead_Texture", m_mapComponent, ID_STATIC);

	m_iMonsterIndex = MONSTER_FATBAT;
	m_pInfoCom->Ready_CharacterInfo(10, 10, 5.f);
	m_pAnimationCom->Ready_Animation(6, 0, 0.2f);
	m_pDeadAnimationCom->Ready_Animation(14, 0, 0.2f);
	for (int i = 0; i < 4; ++i)
	{
		m_bArrFalldown[i] = false;
	}
	_vec3	vScale = { 3.f,3.f,3.f };

	m_pDynamicTransCom->Set_Scale(&vScale);

	if (Posx == 0 && Posy == 0) {}
	else
	{
		m_pDynamicTransCom->Set_Pos((float)Posx, 2.f, (float)Posy);
	}

	m_iDodgeDir = rand() % 3;
	m_fActionDelay = 3.5f;
	m_fMaxY = 3.f;
	m_fFlyDelayCount = 0.f;
	m_fDodgeDelayCount = 0.f;
	m_bAltitude = false;
	m_fDodgeSpeed = 3.5f;
	m_fDodgeStopper = 0.15f;
	m_fStopperDelay = 0.1f;
	m_fStopperDelayCount = 0.f;
	m_fHitDelay = 0.f;
	m_fDeadY = 0.f;
	m_fFrame = rand() % 200 * 0.01f;
	m_vDodgeVector = _vec3(0.f, 0.f, 0.f);
	Save_OriginPos();

	m_pDynamicTransCom->Set_Info(INFO_POS, &_vec3((float)Posx, 2.f, (float)Posy));
	

	m_pDynamicTransCom->Update_Component(1.f);


	return S_OK;
}

bool	CFatBat::Dead_Judge(const _float& fTimeDelta)
{
	if (0 >= m_pInfoCom->Get_Hp() && false == m_bDead)
	{
		if (false == m_bDead)
		{
			_int Hitsound = rand() % 3;
			switch (Hitsound)
			{
			case 0:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Bat_death_01.wav", SOUND_MONSTER, g_fSound * 1.5f);
				break;
			case 1:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Bat_death_02.wav", SOUND_MONSTER, g_fSound * 1.5f);
				break;
			case 2:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Bat_death_03.wav", SOUND_MONSTER, g_fSound * 1.5f);
				break;
			}
			Drop_Item(rand() % 3);
			m_bDead = true;
		}
		_vec3 vPos;
		m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);
		m_fDeadY = vPos.y;


	}
	if (m_bDead)
	{
		Dead_Action(fTimeDelta);
		

		m_pDynamicTransCom->Update_Component(fTimeDelta);
		Engine::CMonsterBase::Update_Object(fTimeDelta);
		Add_RenderGroup(RENDER_ALPHA, this);
		return true;
	}
	else
	{
		return false;
	}
}

void CFatBat::Excution_Event(_bool bAOE )
{

	if (!m_bDead && bAOE)
	{
		m_pInfoCom->Receive_Damage(20);
		return;
	}


	if(!m_bDead && 10 >= m_pInfoCom->Get_Hp())
	{
		m_pInfoCom->Receive_Damage(10);
		_vec3	vPos;
		CGameObject *pGameObject = nullptr;
		CScene* pScene = Get_Scene();
		CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
		m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);
		READY_CREATE_EFFECT_VECTOR(pGameObject, CSpecial_Effect, pLayer, m_pGraphicDev, vPos);
		static_cast<CSpecial_Effect*>(pGameObject)->Set_Effect_INFO(OWNER_PALYER, 0, 17, 0.2f);

		::PlaySoundW(L"explosion_1.wav", SOUND_EFFECT, g_fSound); // BGM

	}
}


_int CFatBat::Update_Object(const _float & fTimeDelta)
{

	m_pDynamicTransCom->Update_Component(fTimeDelta);

	// 맨위에있어야됌 리턴되면 안됌
	_matrix matWorld;
	_vec3 vScale;
	vScale = m_pDynamicTransCom->Get_Scale();
	m_pDynamicTransCom->Get_WorldMatrix(&matWorld);
	m_pColliderCom->Set_HitBoxMatrix_With_Scale(&matWorld, vScale);
	// 맨위에있어야됌 리턴되면 안됌

	// 수정 쿨타임 대신 타임
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

	m_fFrame += fTimeDelta;
	Hit_Delay_toZero();
	
	// 수정 쿨타임 대신 타임

	if (m_bHit == false)
	{
		NoHit_Loop(fTimeDelta);
	}
	else
	{
		Hit_Loop(fTimeDelta);
	}



	CMonsterBase::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CFatBat::LateUpdate_Object(void)
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
	D3DXMatrixScaling(&matScale, m_pDynamicTransCom->m_vScale.x, m_pDynamicTransCom->m_vScale.y, m_pDynamicTransCom->m_vScale.z);

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
	Add_ColliderMonsterlist();
	Engine::CGameObject::LateUpdate_Object();

}

void CFatBat::NoHit_Loop(const _float& fTimeDelta)
{
	if (m_fFrame > m_fActionDelay)
	{
		FatBat_Shoot();
		m_fFrame = 0.f;
	}
	FatBat_Fly(fTimeDelta);
	FatBat_Dodge(fTimeDelta, &m_vPlayerPos, &m_vMonsterPos);


	if (fMtoPDistance > 13.f)
	{
		//m_pDynamicTransCom->Chase_Target_notRot(&m_vPlayerPos, m_pInfoCom->Get_InfoRef()._fSpeed, fTimeDelta);
		_vec3 vDir, vMyPos;
		m_pDynamicTransCom->Get_Info(INFO_POS, &vMyPos);
		vDir = m_vPlayerPos - vMyPos;
		D3DXVec3Normalize(&vDir, &vDir);

		m_pDynamicTransCom->Move_Pos(&(vDir * m_pInfoCom->Get_InfoRef()._fSpeed * fTimeDelta + m_vDodgeVector));
	}
	else if (fMtoPDistance < 10.f)
	{
		//m_pDynamicTransCom->Chase_Target_notRot(&m_vPlayerPos, -m_pInfoCom->Get_InfoRef()._fSpeed, fTimeDelta);
		_vec3 vDir, vMyPos;
		m_pDynamicTransCom->Get_Info(INFO_POS, &vMyPos);
		vDir = m_vPlayerPos - vMyPos;
		D3DXVec3Normalize(&vDir, &vDir);

		m_pDynamicTransCom->Move_Pos(&(vDir * -m_pInfoCom->Get_InfoRef()._fSpeed * fTimeDelta + m_vDodgeVector));
	}
	else
	{
		m_pDynamicTransCom->Move_Pos(&(m_vDodgeVector));
	}
	m_pAnimationCom->Move_Animation(fTimeDelta);
}
void CFatBat::Hit_Loop(const _float& fTimeDelta)
{
	m_pAnimationCom->m_iMotion = 7;
	m_fHitDelay += fTimeDelta;
	if (m_fHitDelay > 1.5f)
	{

		m_bHit = false;
		m_fHitDelay = 0.f;
	}
}

HRESULT CFatBat::SetUp_Material(void)
{

	if (Get_Distance() < 30.f)
	{
		_float fDistance = ((1.f - Get_Distance() / 30.f)) * 2;
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

void CFatBat::Set_Light_Obj()
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
		_float fDistance = ((1.f - Get_Distance() / 30.f) )*2.f;;
		max(fDistance, 0.1f);
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

void CFatBat::Render_Obejct(void)
{
	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pDynamicTransCom->Get_WorldMatrixPointer());
	Set_Light_Obj();
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
	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);	// 텍스처 정보 세팅을 우선적으로 한다.
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

void CFatBat::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	NULL_CHECK_RETURN(pLayer, );
	CGameObject *pGameObject = nullptr;
	CCharacterInfo* pPlayerInfo = static_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC));
	pGameObject = static_cast<CGun_Screen*>(::Get_GameObject(L"Layer_UI", L"Gun"));
	_vec3	vPos;
	m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);

	_vec3 PickPos;

	if (static_cast<CGun_Screen*>(pGameObject)->Get_Shoot() == true &&
		fMtoPDistance < MAX_CROSSROAD + g_fRange &&
		m_pColliderCom->Check_Lay_InterSect(m_pBufferCom, m_pDynamicTransCom, g_hWnd))
	{
		m_bHit = true;
		static_cast<CPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"))->Set_ComboCount(1);
		m_pInfoCom->Receive_Damage(pPlayerInfo->Get_AttackPower());
		cout << "FatBat" << m_pInfoCom->Get_InfoRef()._iHp << endl;
		static_cast<CGun_Screen*>(pGameObject)->Set_Shoot(false);

		READY_CREATE_EFFECT_VECTOR(pGameObject, CHitEffect, pLayer, m_pGraphicDev, vPos);
		static_cast<CHitEffect*>(pGameObject)->Set_Effect_INFO(OWNER_FATBAT, 0, 7, 0.2f);
	
		if (false == m_bDead)
		{
			_int Hitsound = rand() % 3;
			switch (Hitsound)
			{
			case 0:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Bat_pain_01.wav", SOUND_MONSTER, g_fSound * 1.5f);
				break;

			case 1:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Bat_pain_02.wav", SOUND_MONSTER, g_fSound * 1.5f);
				break;

			case 2:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Bat_pain_03.wav", SOUND_MONSTER, g_fSound * 1.5f);
				break;
			}
		}
		::PlaySoundW(L"explosion_1.wav", SOUND_EFFECT, 0.05f); // BGM
	}



}


void	CFatBat::FatBat_Fly(const _float& fTimeDelta)
{
	float TerrainY = 0.f;/* m_pDynamicTransCom->Get_TerrainY1(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC, m_pCalculatorCom, m_pDynamicTransCom);*/
	//L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC, m_pCalculatorCom, m_pDynamicTransCom); 
	_vec3 vPos;
	m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);
	m_fFlyDelayCount += fTimeDelta;
	if (m_fFlyDelayCount > m_fActionDelay)
	{
		int Height;
		Height = rand() % 2;
		if (Height == 0)
		{
			m_bAltitude = false;
		}
		else
		{
			m_bAltitude = true;
		}
		m_fFlyDelayCount = 0;
	}
	switch (m_bAltitude)
	{
	case false:
		if (vPos.y < m_fMaxY)
		{
			m_pDynamicTransCom->Add_Y(0.1f);
		}

		break;

	case true:
		if (vPos.y > TerrainY + 1.5f)
		{
			m_pDynamicTransCom->Add_Y(-0.1f);
		}
		break;
	}

	
	//m_pDynamicTransCom->Monster_Fly(m_pDynamicTransCom, TerrainY, 3.f);

}

void CFatBat::FatBat_Shoot(void)
{
	_vec3 vPos;
	m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);

	CScene* pScene = ::Get_Scene();
	CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");

	_int Hitsound = rand() % 3;
	switch (Hitsound)
	{
	case 0:
		::StopSound(SOUND_MONSTER);
		::PlaySoundW(L"Bat_attack_01.wav", SOUND_MONSTER, g_fSound * 1.5f);
		break;
	case 1:
		::StopSound(SOUND_MONSTER);
		::PlaySoundW(L"Bat_attack_02.wav", SOUND_MONSTER, g_fSound * 1.5f);
		break;
	case 2:
		::StopSound(SOUND_MONSTER);
		::PlaySoundW(L"Bat_attack_03.wav", SOUND_MONSTER, g_fSound * 1.5f);
		break;
	}

	CGameObject* pGameObject = nullptr;
	pGameObject = CObjectMgr::GetInstance()->Reuse_MonsterBulltObj(m_pGraphicDev, vPos);
	NULL_CHECK_RETURN(pGameObject, );
	pMyLayer->Add_GameObjectList(pGameObject);
}

void CFatBat::FatBat_Dodge(const _float& fTimeDelta, _vec3* _vPlayerPos, _vec3* _vMonsterPos)
{
	_vec3 vUp, vDirection;
	m_pDynamicTransCom->Get_Info(INFO_UP, &vUp);
	vDirection = *_vPlayerPos - *_vMonsterPos;
	m_fDodgeDelayCount += fTimeDelta;
	if (m_fDodgeDelayCount > m_fActionDelay)
	{
		m_fDodgeDelayCount = 0.f;
		m_iDodgeDir = rand() % 3;
		m_fDodgeStopper = 0.02f;
	}
	m_fStopperDelayCount += fTimeDelta;
	if (m_fStopperDelayCount > m_fStopperDelay)
	{
		m_fDodgeStopper *= 2;
		m_fStopperDelayCount = 0.f;
	}
		_vec3	vRight;
		D3DXVec3Normalize(&vDirection, &vDirection);
		D3DXVec3Normalize(&vUp, &vUp);
		D3DXVec3Cross(&vRight, &vDirection, &vUp);
	switch (m_iDodgeDir)
	{
	case 0:
		if(0<m_fDodgeSpeed-m_fDodgeStopper)
			m_vDodgeVector = (vRight * (m_fDodgeSpeed - m_fDodgeStopper) * 0.1f);
		break;

	case 1:
		m_vDodgeVector = _vec3(0.f, 0.f, 0.f);
		break;

	case 2:
		
		if (0<m_fDodgeSpeed - m_fDodgeStopper)
			m_vDodgeVector = (vRight * -(m_fDodgeSpeed - m_fDodgeStopper) * 0.1f);
		break;

	}

}

void		CFatBat::Dead_Action(const _float& fTimeDelta)
{
	if (m_pDeadAnimationCom->m_iMotion < m_pDeadAnimationCom->m_iMaxMotion)
	{
		m_pDeadAnimationCom->Move_Animation(fTimeDelta);

	}
	if (3 == m_pDeadAnimationCom->m_iMotion)
	{
		if (false == m_bArrFalldown[0])
		{
			m_pDynamicTransCom->Add_Y(-m_fDeadY * 0.2f);
			m_bArrFalldown[0] = true;
		}
	}
	else if (4 == m_pDeadAnimationCom->m_iMotion)
	{
		if (false == m_bArrFalldown[1])
		{
			m_pDynamicTransCom->Add_Y(-m_fDeadY * 0.2f);
			m_bArrFalldown[1] = true;
		}
	}
	else if (5 == m_pDeadAnimationCom->m_iMotion)
	{
		if (false == m_bArrFalldown[2])
		{
			m_pDynamicTransCom->Add_Y(-m_fDeadY * 0.2f);
			m_bArrFalldown[2] = true;
		}
	}
	else if (6 == m_pDeadAnimationCom->m_iMotion)
	{
		if (false == m_bArrFalldown[3])
		{
			//m_pDynamicTransCom->Add_Y(-m_fDeadY * 0.2f);
			m_pDynamicTransCom->Set_Y(m_pDynamicTransCom->m_vScale.y * 0.5f);
			m_bArrFalldown[3] = true;
		}
	}
}

void	CFatBat::Drop_Item(int ItemType)
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

float	CFatBat::Get_Radius(void)
{
	return m_pDynamicTransCom->m_vScale.x * 0.25f;
}

CFatBat * CFatBat::Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx, int Posy)
{
	CFatBat*	pInstance = new CFatBat(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(Posx, Posy)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFatBat::Free(void)
{
	CMonsterBase::Free();
}
