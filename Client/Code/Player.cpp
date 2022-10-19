#include "stdafx.h"
#include "..\Header\Player.h"

#include "AbstractFactory.h"
#include "Export_Function.h"
#include "Gun_Screen.h"

#include "ShotGun.h"

#include "Key.h"
#include "MyCamera.h"
#include "TeleCube.h"

#include "Player_Dead_UI.h"
#include  "Ax.h"
#include "AttackEffect.h"
#include "UI_Effect.h"


CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_vDirection(0.f, 0.f, 0.f)
	, m_fJumpPower(5.0f)
	, m_fDashPower(0.3f)
	, m_tpType(TYPING_END)
	, pEquipItem(nullptr)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_Object(void)
{
	
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pInfoCom->Ready_CharacterInfo(100, 10, 5.f);

	_vec3 vPos = { 20.f, 6.f, 15.f };

	m_pDynamicTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	_vec3 vScale = { 0.1f, 0.1f, 0.1f };
	m_pDynamicTransCom->Set_Scale(&vScale);
	
	m_pColliderCom->Set_HitRadiuos(1.1f);
	m_pColliderCom->Set_vCenter(&vPos, &vScale);


	m_pDynamicTransCom->Update_Component(1.0f);
	m_pColliderCom->Set_HitBoxMatrix(&(m_pDynamicTransCom->m_matWorld));
	m_iOriginHP = m_pInfoCom->Get_Hp();

	return S_OK;
}

_int CPlayer::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		m_bDeadTimer += 1.0f* fTimeDelta;
	
	if (m_bDeadTimer >= 5.f)
	{
		Random_ResurrectionRoom();
		m_bDeadTimer = 0.f;
		m_bDead = false;
	}

	m_fTimeDelta = fTimeDelta;
	m_fFrame += 1.0f * fTimeDelta;
	if (m_fFrame >= 1.0f)
	{
		pEquipItem->Set_ReadyShot(false);
		m_fFrame = 0.f;
	}

	pEquipItem = dynamic_cast<CGun_Screen*>(Get_GameObject(L"Layer_UI", L"Gun"));
	NULL_CHECK_RETURN(pEquipItem, -1);
	m_iOriginHP = m_pInfoCom->Get_Hp();

	if (m_pInfoCom->Get_Hp() <= 0)
	{
		Player_Dead(fTimeDelta);
	}

	Key_Input(fTimeDelta);

	if (m_bJump == TRUE)
	{
		m_pDynamicTransCom->Jumping(m_fJumpPower, fTimeDelta, m_pDynamicTransCom);
		// const _tchar * pLayerTag, const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID, CCalculator* pCalculator, CTransform* pTransform
		m_fJumpPower -= 0.25f;					// const _tchar * pLayerTag, const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID, CCalculator* pCalculator, CTransform* pTransform
		m_fNowPosHeight = m_pDynamicTransCom->Get_TerrainY1(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC, m_pCalculatorCom, m_pDynamicTransCom);
		_vec3 vNowPlayerPos;
		m_pDynamicTransCom->Get_Info(INFO_POS, &vNowPlayerPos);
		if (vNowPlayerPos.y <= m_fNowPosHeight)
		{
			m_bJump = FALSE;
			m_fJumpPower = 5.0f;
		}
	}
	else
	{

	}


	if (m_bDash) // 대쉬 시간 제한
	{
		m_fDashTimer += 1.f*fTimeDelta;
		m_pDynamicTransCom->Dashing(fTimeDelta, m_pDynamicTransCom, m_vUp, m_vDirection, m_tpType);
	}

	if (m_fDashTimer >= 0.2f)
	{
		m_bDash = false;
		m_fDashTimer = 0.f;
		CScene* pScene = ::Get_Scene();
		NULL_CHECK_RETURN(pScene, RETURN_ERR);
		CLayer* pLayer = pScene->GetLayer(L"Layer_UI");
		NULL_CHECK_RETURN(pLayer, RETURN_ERR);
		CUI_Effect* pGameObject = nullptr;

		pGameObject = dynamic_cast<CUI_Effect*>(pLayer->Get_GameObject(L"Dash_Effect"));
		pGameObject->Set_Active(false);
	}





	m_pDynamicTransCom->Set_Y(2.f);
	m_pColliderCom->Set_HitBoxMatrix(&(m_pDynamicTransCom->m_matWorld));
	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CPlayer::LateUpdate_Object(void)
{
	if (m_iOriginHP > m_pInfoCom->Get_Hp())
	{
		CScene* pScene = Get_Scene();
		CLayer* pLayer = pScene->GetLayer(L"Layer_Environment");

		CMyCamera *pCam = dynamic_cast<CMyCamera*> (pLayer->Get_GameObject(L"CMyCamera"));
		
		if (pCam != nullptr)
			 pCam->Set_PlayerHit(true);	
		
	
	}
	
	CGameObject::LateUpdate_Object();
}

void CPlayer::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pDynamicTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pTextureCom->Set_Texture(m_iTexIndex);
	m_pBufferCom->Render_Buffer();
	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pColliderCom->HitBoxWolrdmat());
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	
	m_pColliderCom->Render_Buffer();
	
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	
}

HRESULT CPlayer::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_PlayerTexture", m_mapComponent, ID_STATIC);
	m_pDynamicTransCom = CAbstractFactory<CDynamic_Transform>::Clone_Proto_Component(L"Proto_DynamicTransformCom", m_mapComponent, ID_DYNAMIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pInfoCom = CAbstractFactory<CCharacterInfo>::Clone_Proto_Component(L"Proto_CharacterInfoCom", m_mapComponent, ID_STATIC);
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);

	return S_OK;
}

void CPlayer::Key_Input(const _float & fTimeDelta)
{
	m_pDynamicTransCom->Get_Info(INFO_LOOK, &m_vDirection);
	m_pDynamicTransCom->Get_Info(INFO_UP, &m_vUp);
	m_pDynamicTransCom->Get_Info(INFO_POS, &m_vPos);

	if (Get_DIKeyState(DIK_L) & 0X80)
	{
		cout << m_pInfoCom->Get_Hp() << endl;
	}

	if (Get_DIKeyState(DIK_W) & 0X80)
	{
		if (m_bShopUIActive)
			return;
		m_tpType = TYPING_W;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pDynamicTransCom->Move_Pos(&(m_vDirection * 5.f * fTimeDelta));

	}

	if (Get_DIKeyState(DIK_S) & 0X80)
	{
		if (m_bShopUIActive)
			return;
		m_tpType = TYPING_S;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pDynamicTransCom->Move_Pos(&(m_vDirection * -5.f * fTimeDelta));

		
	}

	if (Get_DIKeyState(DIK_A) & 0X80)
	{
		if (m_bShopUIActive)
			return;
		m_tpType = TYPING_A;
		_vec3	vRight;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		D3DXVec3Normalize(&m_vUp, &m_vUp);
		D3DXVec3Cross(&vRight, &m_vDirection, &m_vUp);
		m_pDynamicTransCom->Move_Pos(&(vRight * 5.f * fTimeDelta));

	}

	if (Get_DIKeyState(DIK_D) & 0X80)
	{
		if (m_bShopUIActive)
			return;
		m_tpType = TYPING_D;
		_vec3	vRight;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		D3DXVec3Normalize(&m_vUp, &m_vUp);
		D3DXVec3Cross(&vRight, &m_vDirection, &m_vUp);
		m_pDynamicTransCom->Move_Pos(&(vRight * -5.f * fTimeDelta));

		
	}

	if (Key_Down(DIK_T))
	{
		// Test -> Shop 추후수정
		m_pDynamicTransCom->Set_Pos(340.f, 2.f, 325.f);
		//
		Excution_Motion();

		CScene* pScene = Get_Scene();
		CLayer* PLayer = pScene->GetLayer(L"Layer_GameLogic");
		
		for (auto iter : PLayer->Get_GameObjectMap())
		{
			CMonsterBase* pMonster = dynamic_cast<CMonsterBase*>(iter.second);

			if(pMonster!= nullptr)
				static_cast<CMonsterBase*>(iter.second)->Excution_Event();
		}


/*
		NULL_CHECK_RETURN(pScene, );
		CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
		NULL_CHECK_RETURN(pLayer, );
		CGameObject *pGameObject = nullptr;
		_vec3	vPos;
		m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);
		READY_CREATE_EFFECT_VECTOR(pGameObject, CAttackEffect, pLayer, m_pGraphicDev, vPos);
		static_cast<CAttackEffect*>(pGameObject)->Set_Effect_INFO(OWNER_PALYER, 0, 12, 0.2f);
*/
	}

	if (Get_DIKeyState(DIK_SPACE) & 0X80)
		m_bJump = TRUE;
	if (::Key_Down(DIK_LSHIFT))
	{
		m_bDash = true;
		
		CScene* pScene = ::Get_Scene();
		NULL_CHECK_RETURN(pScene, );
		CLayer* pLayer = pScene->GetLayer(L"Layer_UI");
		NULL_CHECK_RETURN(pLayer, );
		CUI_Effect* pGameObject = nullptr;

		pGameObject =dynamic_cast<CUI_Effect*>(pLayer->Get_GameObject(L"Dash_Effect"));
		pGameObject->Set_Active(true);

	}

	if (::Mouse_Down(DIM_LB)) // Picking
	{
		if (m_bInventroyActive || m_bShopUIActive)
			return;

		Ready_MonsterShotPicking();
	}

	if (Get_DIKeyState(DIK_R) & 0X80)
	{
		pEquipItem->Set_Magazine(8);
	}

	if (Get_DIKeyState(DIK_P) & 0X80)
	{
		_vec3	vcurrentPos;
		m_pDynamicTransCom->Get_Info(INFO_POS, &vcurrentPos);
	}

	if (Engine::Key_Down(DIK_C))
	{
		Player_Dead(fTimeDelta);
		//m_pInfoCom->Get_InfoRef()._iHp -= 25;
	}
	Engine::Key_InputReset();
}



_vec3 CPlayer::PickUp_OnTerrain(void)
{
	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, _vec3());

	return m_pCalculatorCom->PickingOnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
}

void CPlayer::Ready_MonsterShotPicking()
{
	if (pEquipItem->Get_Magazine() <= 0)
		pEquipItem->Set_ReadyShot(false);

	if (static_cast<CGun_Screen*>(pEquipItem)->Get_ReadyShot())
	{
		static_cast<CGun_Screen*>(pEquipItem)->Add_Magazine(-1);
		pEquipItem->Set_AnimationCheck(true);
		pEquipItem->Set_Shoot(true); // 발사가 true이므로 여기서 총알 생성
		return;
	}

	pEquipItem->GunFailSound();
	m_iComboCount = 0;
}

void CPlayer::ComboCheck()
{
	if (m_bMissCheck)
		m_bMissCheck = false;
	else
		m_iComboCount = 0;
}

void CPlayer::EquipItem_Add_Stat(_int _iAttack , _int _iHp , _int iCoin, _int _iKey, float _fSpeed)  // 현재 각 아이템들 충돌처리 부분이 애매해서 F 누르면 스탯이 다 증가할 거임. 충돌처리를 고치던지 날 잡고 한 번 뜯어봐야 함.
{
	m_pInfoCom->Get_InfoRef()._iAttackPower = _iAttack + 10;		// iOrginAttack =10
	m_pInfoCom->Get_InfoRef()._iHp += _iHp ;		// iOrginAttack =10
	//_int iHp = _iArmor + m_pInfoCom->Get_InfoRef()._iAttackPower;
	//m_pInfoCom->Get_InfoRef()._iHp = iHp;


}

void CPlayer::Loss_Damage()
{
	m_pInfoCom->Get_InfoRef()._iHp -= 25;
	// Test
	m_iHpBarChange -= 1;

}

void CPlayer::Excution_Motion()
{
	static_cast<CMyCamera*>(::Get_GameObject(L"Layer_Environment", L"CMyCamera"))->Set_Excution(true);
	static_cast<CGun_Screen*>(pEquipItem)->Set_Active(true);
	
	static_cast<CAx*>(::Get_GameObject(L"Layer_UI", L"AX"))->Set_Active(true);
}

void CPlayer::Random_ResurrectionRoom()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_CubeCollsion");
	NULL_CHECK_RETURN(pLayer, );

	pLayer->Clear_Stack();
	pLayer->m_iRestRoom = 5;

	random_shuffle((*pLayer->GetRestCube()).begin(), (*pLayer->GetRestCube()).end());

	pLayer->m_iRoomIndex = pLayer->m_iRestRoom--;
	CGameObject* pFirstCubeObj = (*pLayer->GetRestCube())[pLayer->m_iRoomIndex];

	pLayer = pScene->GetLayer(L"Layer_GameLogic");

	CTransform* pFirstCubeTransform = dynamic_cast<CTransform*>(pFirstCubeObj->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));

	_vec3 vFirstCubePos;
	pFirstCubeTransform->Get_Info(INFO_POS, &vFirstCubePos);

	m_pDynamicTransCom->Set_Pos(vFirstCubePos.x + 5.f, vFirstCubePos.y, vFirstCubePos.z + 5.f);

	pLayer->Reset_Monster();
	m_pDynamicTransCom->Update_Component(1.f);
}

void CPlayer::Player_Dead_CaemraAction()
{
	CMyCamera* pCam = dynamic_cast<CMyCamera*>(::Get_GameObject(L"Layer_Environment", L"CMyCamera"));

	pCam->Set_PlayerDeadCam(true);
	pCam->m_fOriginAngle = m_pDynamicTransCom->Get_Angle().x;
}

void CPlayer::Player_Dead(const _float& fTimeDelta)
{
	CScene* pScene1 = ::Get_Scene();
	CLayer* pMyLayer = pScene1->GetLayer(L"Layer_UI");

	CPlayer_Dead_UI* pDead_UI = static_cast<CPlayer_Dead_UI*>(Engine::Get_GameObject(L"Layer_UI", L"Dead_UI"));
	pDead_UI->Set_Render(true);

	Player_Dead_CaemraAction();
	m_pInfoCom->Ready_CharacterInfo(100, 10, 5.f);

	m_bDead = true;
}

void CPlayer::Collision_Event()
{
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer *	pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPlayer::Free(void)
{
	CGameObject::Free();
}
