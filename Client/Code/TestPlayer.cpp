#include "stdafx.h"
#include "..\Header\TestPlayer.h"

#include "Export_Function.h"
#include "ObjectMgr.h"
#include "Bullet.h"
#include "Bullet_UI.h"
#include "Stage.h"
#include "HpPotion.h"
#include "Coin.h"
#include "Box.h"
#include "MyCamera.h"


CTestPlayer::CTestPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_vDirection(0.f, 0.f, 0.f)
	, m_fJumpPower(5.0f)
	, m_fDashPower(0.3f)
	, m_tpType(TYPING_END)
{
}


CTestPlayer::~CTestPlayer()
{
}

HRESULT CTestPlayer::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
							// int _hp, int _Attack, float _fSpeed
	m_pInfoCom->Ready_CharacterInfo(100, 10, 5.f);

	//m_preItem = m_pInfoCom->Get_InfoRef()._iCoin;
		
	return S_OK;
}

_int CTestPlayer::Update_Object(const _float & fTimeDelta)
{
	++m_iCountDash;
	Key_Input(fTimeDelta);
	
	//if (m_preItem = m_pInfoCom->Get_InfoRef()._iCoin) // Coin 획득했는가를 체크하는 코드
	//{
	//	//system("cls");

	//	/*cout << "체력 : " << m_pInfoCom->Get_InfoRef()._iHp << endl;
	//	m_preItem = m_pInfoCom->Get_InfoRef()._iHp;*/

	//	cout << "코인 : " << m_pInfoCom->Get_InfoRef()._iCoin << endl;

	//}	

		// cout << "총알 수 :" << m_iMagazine << endl;
		

	
	if (m_bJump == TRUE)
	{
		m_pDynamicTransCom->Jumping(m_fJumpPower, fTimeDelta, m_pTransCom);
												// const _tchar * pLayerTag, const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID, CCalculator* pCalculator, CTransform* pTransform
		m_fJumpPower -= 0.25f;					// const _tchar * pLayerTag, const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID, CCalculator* pCalculator, CTransform* pTransform
		m_fNowPosHeight = m_pDynamicTransCom->Get_TerrainY1(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC, m_pCalculatorCom, m_pTransCom); 
		_vec3 vNowPlayerPos;
		m_pTransCom->Get_Info(INFO_POS, &vNowPlayerPos);
		if (vNowPlayerPos.y <= m_fNowPosHeight)
		{
			m_bJump = FALSE;
			m_fJumpPower = 5.0f;
		}
	}
	else
	{
	
	}
	
	m_fFrame += 1.f * fTimeDelta;

	if (m_fFrame >= 1.f)
		m_fFrame = 0.f;
		
	
	Engine::CGameObject::Update_Object(fTimeDelta);
		
	/*_matrix		matWorld, matView, matBill;
	D3DXMatrixIdentity(&matBill);

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);


	m_pTransCom->Set_WorldMatrix(&(matBill * matWorld));*/
	
	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CTestPlayer::LateUpdate_Object(void)
{
	// 빌보드 에러 해결
	CTransform*	pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"TestPlayer", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransform);

	CMyCamera* pCamera = static_cast<CMyCamera*>(Get_GameObject(L"Layer_Environment", L"DynamicCamera"));
	NULL_CHECK(pCamera);

	_matrix		matWorld, matView, matBill;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);
	memcpy(&matBill, &matView, sizeof(_matrix));
	memset(&matBill._41, 0, sizeof(_vec3));
	D3DXMatrixInverse(&matBill, 0, &matBill);

	_matrix      matScale, matTrans;
	D3DXMatrixScaling(&matScale, 2.f, 2.f, 2.f);

	_matrix      matRot;
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixRotationY(&matRot, pCamera->Get_BillBoardDir());

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

void CTestPlayer::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		
	m_pTextureCom->Set_Texture(m_iTexIndex);
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);


	/*m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTextureCom->Set_Texture(m_iTexIndex);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);*/
}

HRESULT CTestPlayer::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	// TestTool?슜
	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_PlayerTexture2"));
	//NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Proto_PlayerTexture2", pComponent });

	// Stage ?슜
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_PlayerTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_PlayerTexture", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pDynamicTransCom = dynamic_cast<CDynamic_Transform*>(Clone_Proto(L"Proto_DynamicTransformCom"));
	NULL_CHECK_RETURN(m_pDynamicTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_DynamicTransformCom", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_CalculatorCom"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", pComponent });

	pComponent = m_pAnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pAnimationCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_AnimationCom", pComponent });

	pComponent = m_pInfoCom = dynamic_cast<CCharacterInfo*>(Clone_Proto(L"Proto_CharacterInfoCom"));
	NULL_CHECK_RETURN(m_pInfoCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CharacterInfoCom", pComponent });

	
	return S_OK;
}

void CTestPlayer::Key_Input(const _float& fTimeDelta)
{
	m_pTransCom->Get_Info(INFO_LOOK, &m_vDirection);	
	m_pTransCom->Get_Info(INFO_UP, &m_vUp);
	m_pTransCom->Get_Info(INFO_POS, &m_vPos);
	
	if (Get_DIKeyState(DIK_W) & 0X80)
	{
		m_tpType = TYPING_W;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pTransCom->Move_Pos(&(m_vDirection * 10.f * fTimeDelta));
	}

	if (Get_DIKeyState(DIK_S) & 0X80 )
	{
		m_tpType = TYPING_S;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pTransCom->Move_Pos(&(m_vDirection * -10.f * fTimeDelta));	
	}

	if (Get_DIKeyState(DIK_A) & 0X80)
	{
		m_tpType = TYPING_A;
		_vec3	vRight;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		D3DXVec3Normalize(&m_vUp, &m_vUp);
		D3DXVec3Cross(&vRight, &m_vDirection, &m_vUp);
		m_pTransCom->Move_Pos(&(vRight * 10.f * fTimeDelta));
	}
	
	if (Get_DIKeyState(DIK_D) & 0X80)
	{
		m_tpType = TYPING_D;
		_vec3	vRight;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		D3DXVec3Normalize(&m_vUp, &m_vUp);
		D3DXVec3Cross(&vRight, &m_vDirection, &m_vUp);
		m_pTransCom->Move_Pos(&(vRight * -10.f * fTimeDelta));
	}

	if (Get_DIKeyState(DIK_SPACE) & 0X80)	
		m_bJump = TRUE;	
	
	if (Get_DIKeyState(DIK_LSHIFT) & 0X80)		
	{
		m_bDash = TRUE;

		if (m_bDash)
		{
			m_pDynamicTransCom->Dashing(fTimeDelta, m_pTransCom, m_vUp, m_vDirection, m_tpType);
			m_bDash = FALSE;
		}
	}

	if (Engine::Get_DIMouseState(DIM_LB) & 0X80) // Picking
	{
		Create_Bullet(m_vPos);
		
		m_bOneShot = TRUE;
	
		// Magazine 0 = Don't Shoot
		if (m_iMagazine == 0)
			m_bOneShot = FALSE;
	
		
	}

	if (Get_DIKeyState(DIK_R) & 0X80)
	{
		m_iMagazine = 8;
	}

	// Test
	// Weapon Image Change
	if (Get_DIKeyState(DIK_Z) & 0X80)
	{
		m_iChangeImage -= 1;
	}

	if (Get_DIKeyState(DIK_X) & 0X80)
	{
		m_iChangeImage = 1;
	}

	// HpBar Change
	if (Get_DIKeyState(DIK_C) & 0X80)
	{		
		m_iHpBarChange -= 1;
	}

	if (Get_DIKeyState(DIK_V) & 0X80)
	{
		m_iHpBarChange = 4;
	}

	if (Get_DIKeyState(DIK_P) & 0X80)
	{
		m_pInfoCom->Get_InfoRef()._iAttackPower += 1;		
	}

	if (Get_DIKeyState(DIK_O) & 0X80)
	{		
		m_pInfoCom->Get_InfoRef()._fSpeed += 1.f;
	}
	
	// ~Test


	// Picking to LeftButton
	/*if (Engine::Get_DIMouseState(DIM_LB) & 0X80)
	{
		_vec3	vPickPos = PickUp_OnTerrain();
		_vec3	vPlayerPos, vDir;
		m_pTransCom->Get_Info(INFO_POS, &vPlayerPos);
		vDir = vPickPos - vPlayerPos;
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransCom->Move_Pos(&(vDir * 5.f * fTimeDelta));
	}*/
}

void CTestPlayer::Set_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	
	 //TestTool ?슜
	//Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"TestLayer", L"TestMap", L"Proto_TerrainTexCom", ID_STATIC));
	//NULL_CHECK(pTerrainTexCom);
	// Stage ?슜
	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);
	
	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransCom->Set_Pos(vPos.x, fHeight, vPos.z);

	
}

Engine::_vec3 CTestPlayer::PickUp_OnTerrain(void)
{
	// Stage ?슜
	
	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, _vec3());
	

	// TestTool ?슜
	/*CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"TestLayer", L"TestMap", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"TestLayer", L"TestMap", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, _vec3());*/

	return m_pCalculatorCom->PickingOnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
}


HRESULT CTestPlayer::Create_Bullet(_vec3 vPos)
{
	++m_iCoolTime;

	if (m_bOneShot && m_iCoolTime > 10)
	{	
		m_bOneShot = FALSE;
	
		m_iCoolTime = 0;

		CScene* pScene = ::Get_Scene();
		CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");

		CGameObject* pGameObject = nullptr;  // Reuse_PlayerBulltObj
		pGameObject = CObjectMgr::GetInstance()->Reuse_PlayerBulltObj(m_pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMyLayer->Add_GameObjectList(pGameObject);

		m_iMagazine -= 1;

	}
	return S_OK;
}

void CTestPlayer::Collision_Event(CGameObject * pGameObject)
{
	CScene* pScene = ::Get_Scene();
	CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");
			
	CTransform *pTransform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
	
	_vec3 vObjPos;
	_vec3 vPlayerPos;


	pTransform->Get_Info(INFO_POS, &vObjPos);
	m_pTransCom->Get_Info(INFO_POS, &vPlayerPos);
	// ★
	if (m_pColliderCom->Check_Sphere_InterSect(vObjPos, vPlayerPos, 1.f, 1.f) == true)
	{
		if (pGameObject == pMyLayer->Get_GameObject(L"HealthPotion"))
		{				
			m_pInfoCom->Add_Hp(25);
			m_iHpBarChange += 1;				
			pMyLayer->Delete_GameObject(L"HealthPotion"); // 이벤트 처리		
		}

		if (pGameObject == pMyLayer->Get_GameObject(L"Coin"))
		{
			m_pInfoCom->Get_InfoRef()._iCoin += 1;
			pMyLayer->Delete_GameObject(L"Coin"); // 이벤트 처리
		}				
	}
	//_uint iPlayerPower = dynamic_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"TestPlayer", L"Proto_CharacterInfoCom", ID_STATIC))->Get_InfoRef()._iAttackPower;
	if (m_pColliderCom->Check_Sphere_InterSect(vObjPos, vPlayerPos, 1.f, 1.f) == true)
	{
		if (pGameObject == pMyLayer->Get_GameObject(L"Box"))
		{
			if (Get_DIKeyState(DIK_F) & 0X80)
			{
				CAnimation* pBoxAnimation = dynamic_cast<CAnimation*>(pGameObject->Get_Component(L"Proto_AnimationCom", ID_STATIC));
				// 박스를 여는 부분, 꼼수(오픈 이미지만 늘림) 수정 필요

				CBox* pBox = dynamic_cast<CBox*> (Engine::Get_GameObject(L"Layer_GameLogic", L"Box"));


				m_bBoxOpen = true;

				pBoxAnimation->Open_Box_Animation(m_bBoxOpen);
				pBox->Open_Event(this);
				m_bBoxOpen = false;
			}
		}
	}
}


CTestPlayer * CTestPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTestPlayer *	pInstance = new CTestPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;

}

void CTestPlayer::Free(void)
{
	
	CGameObject::Free();
}
