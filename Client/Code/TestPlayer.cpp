#include "stdafx.h"
#include "..\Header\TestPlayer.h"

#include "Export_Function.h"
#include "Bullet.h"

CTestPlayer::CTestPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_vDirection(0.f, 0.f, 0.f)
	, m_fJumpPower(5.0f)
	, m_fDashPower(0.3f)
{
}


CTestPlayer::~CTestPlayer()
{
}

HRESULT CTestPlayer::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	return S_OK;
}

_int CTestPlayer::Update_Object(const _float & fTimeDelta)
{

	Key_Input(fTimeDelta);


	Engine::CGameObject::Update_Object(fTimeDelta);

	if (m_bJump == TRUE)
	{
		m_pTransCom->Jump(m_fJumpPower, fTimeDelta);
		m_fJumpPower -= 0.25f;
		m_fNowPosHeight = Get_TerrainY();
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
		Set_OnTerrain();
	}


	Dash(fTimeDelta);
	
	m_fFrame += 1.f * fTimeDelta;

	if (m_fFrame >= 1.f)
		m_fFrame = 0.f;
		
	
	Engine::CGameObject::Update_Object(fTimeDelta);

	// ���̳��� ī�޶� �ٶ󺸴� ������
	_matrix		matWorld, matView, matBill;
	D3DXMatrixIdentity(&matBill);

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	// ���� ���� �� �ڵ�� ������ ������ ���߿� ������ �� �� ����
	m_pTransCom->Set_WorldMatrix(&(matBill * matWorld));
	
	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CTestPlayer::LateUpdate_Object(void)
{
	Set_OnTerrain();
}

void CTestPlayer::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//m_pTextureCom->Set_Texture(0);	// Stage 용
	m_pTextureCom->Set_Texture(m_iTexIndex); // TestTool 용
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

HRESULT CTestPlayer::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	// TestTool용
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_PlayerTexture2"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_PlayerTexture2", pComponent });

	// Stage 용
	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_PlayerTexture"));
	//NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Proto_PlayerTexture", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_CalculatorCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", pComponent });

	return S_OK;
}

void CTestPlayer::Key_Input(const _float& fTimeDelta)
{
	m_pTransCom->Get_Info(INFO_LOOK, &m_vDirection);	
	m_pTransCom->Get_Info(INFO_UP, &m_vUp);
	m_pTransCom->Get_Info(INFO_POS, &m_vPos);




	// if (CInputDev::GetInstance()->Key_Pressing(DIK_LSHIFT))
	// {
	// 	m_bDash = TRUE;
	// }
	// else
	// {
	// 	m_bDash = FALSE;
	// }

	// if (m_bDash == FALSE)
	// {
	// 	if (GetAsyncKeyState(VK_UP) & 0x8000)
	// 	{
	// 		if (m_fBuffDashPower > 0.f)
	// 			m_fBuffDashPower -= m_fDashPower;
	// 		else if (m_fBuffDashPower < 0.f)
	// 		{
	// 			m_fBuffDashPower = 0.f;
	// 		}

	// 		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
	// 		m_pTransCom->Move_Pos(&(m_vDirection * (10.f + m_fBuffDashPower) * fTimeDelta));
	// 	}
	// }
	// else
	// {
	// 	if (GetAsyncKeyState(VK_UP) & 0x8000)
	// 	{
	// 		if (m_fBuffDashPower < 20.f)
	// 			m_fBuffDashPower += m_fDashPower;

	// 		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
	// 		m_pTransCom->Move_Pos(&(m_vDirection * (10.f + m_fBuffDashPower) * fTimeDelta));
	// 	}
	// }
	// if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	// {
	// 	D3DXVec3Normalize(&m_vDirection, &m_vDirection);
	// 	m_pTransCom->Move_Pos(&(m_vDirection * -10.f * fTimeDelta));
	// }

	// if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	// 	m_pTransCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));

	// if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	// 	m_pTransCom->Rotation(ROT_Y, D3DXToRadian(-180.f * fTimeDelta));

	// if (Engine::Get_DIMouseState(DIM_LB) & 0X80)
	// {
	// 	_vec3	vPickPos = PickUp_OnTerrain();
	// 	_vec3	vPlayerPos, vDir;
	// 	m_pTransCom->Get_Info(INFO_POS, &vPlayerPos);
	// 	vDir = vPickPos - vPlayerPos;
	// 	D3DXVec3Normalize(&vDir, &vDir);
	// 	m_pTransCom->Move_Pos(&(vDir * 5.f * fTimeDelta));
	// }

	// if (Engine::Get_DIKeyState(DIK_SPACE) & 0X80)
	// {
	// 	m_bJump = TRUE;
	// }

if (Get_DIKeyState(DIK_W) & 0X80)
	{
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pTransCom->Move_Pos(&(m_vDirection * 10.f * fTimeDelta));
	}

	if (Get_DIKeyState(DIK_S) & 0X80)
	{
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pTransCom->Move_Pos(&(m_vDirection * -10.f * fTimeDelta));	
	}

	if (Get_DIKeyState(DIK_A) & 0X80)
	{
		_vec3	vRight;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		D3DXVec3Normalize(&m_vUp, &m_vUp);
		D3DXVec3Cross(&vRight, &m_vDirection, &m_vUp);
		m_pTransCom->Move_Pos(&(vRight * 10.f * fTimeDelta));
	}
	
	if (Get_DIKeyState(DIK_D) & 0X80)
	{
		_vec3	vRight;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		D3DXVec3Normalize(&m_vUp, &m_vUp);
		D3DXVec3Cross(&vRight, &m_vDirection, &m_vUp);
		m_pTransCom->Move_Pos(&(vRight * -10.f * fTimeDelta));
	}

	if (Get_DIKeyState(DIK_SPACE) & 0X80)	
		m_bJump = true;
	

	if (Get_DIKeyState(DIK_LSHIFT) & 0X80)		
		m_bDash = true;


	if (Engine::Get_DIMouseState(DIM_LB) & 0X80) // �� �߻�
	{
		Create_Bullet(m_vPos);
		
		m_bOneShot = true;

		// Magazine 0 = Don't Shoot
		if (m_iMagazine == 0)
			m_bOneShot = false;
	}


	if (Get_DIKeyState(DIK_R) & 0X80)
	{
		m_iMagazine = 8;
	}


	// ��ŷ
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
	
	 //TestTool 용
	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"TestLayer", L"TestMap", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);
	// Stage 용
	/*Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);*/
	
	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransCom->Set_Pos(vPos.x, fHeight, vPos.z);

	
}

Engine::_vec3 CTestPlayer::PickUp_OnTerrain(void)
{
	// Stage 용
	/*
	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, _vec3());
	*/

	// TestTool 용
	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"TestLayer", L"TestMap", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"TestLayer", L"TestMap", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, _vec3());

	return m_pCalculatorCom->PickingOnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
}

float CTestPlayer::Get_TerrainY(void)
{
	_vec3		vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	// TestTool 용
	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"TestLayer", L"TestMap", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	// Stage 용
	/*Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);*/

	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);


	return fHeight;
}

HRESULT CTestPlayer::Create_Bullet(_vec3 vPos)
{
	++m_iCoolTime;

	if (m_bOneShot && m_iCoolTime > 10)
	{
		CBullet* pBullet = CBullet::Create(m_pGraphicDev, vPos);
		NULL_CHECK(pBullet);

		_tchar*         szFinalName = new _tchar[128]; // �����Ⱚ
		wsprintf(szFinalName, L"");

		const _tchar*   szBulletName = L"Bullet_%d";

		wsprintf(szFinalName, szBulletName, m_iCount);

		//_tchar*	szBullet = L"Bullet1";
		//FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", szFinalName, pBullet), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"TestLayer", szFinalName, pBullet), E_FAIL);

		if (szBulletName != nullptr)
			m_iMagazine -= 1;

		m_szBulletName.push_back(szFinalName);
		m_iCount++;

		m_bOneShot = false;
		m_iCoolTime = 0;
	}

#ifdef _DEBUG

	//cout << "�Ѿ� ���� : " << m_iTest << endl;

#endif	// _DEBUG

	/*_vec3		vPlayerPos;
	m_pTransCom->Get_Info(INFO_POS, &vPlayerPos);*/
		
	/*CTransform*		pBulletCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", szFinalName, L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pBulletCom, );
	pBulletCom->Chase_Target(&vPlayerPos, 2.f, 2.f);*/
	//pBulletCom->Set_Pos(vPlayerPos.x, vPlayerPos.y, vPlayerPos.z);  // ���� ��ġ vec3

	//return pBulletCom->Set_Dir(vPos);
	
	return S_OK;
}

void CTestPlayer::Dash(const _float& fTimeDelta)
{	
	++m_iCountDash;

	if (m_bDash && m_iCountDash > 20)
	{
		m_pTransCom->Get_Info(INFO_LOOK, &m_vDirection);
		m_pTransCom->Get_Info(INFO_UP, &m_vUp);
		_float fSpeed = 4.f;

		if (Get_DIKeyState(DIK_W) & 0X80)
		{
			D3DXVec3Normalize(&m_vDirection, &m_vDirection);
			m_pTransCom->Move_Pos(&(m_vDirection * 4.f));
		}

		if (Get_DIKeyState(DIK_S) & 0X80)
		{
			D3DXVec3Normalize(&m_vDirection, &m_vDirection);
			m_pTransCom->Move_Pos(&(m_vDirection * -4.f));
		}

		if (Get_DIKeyState(DIK_A) & 0X80)
		{
			_vec3	vRight;
			D3DXVec3Normalize(&m_vDirection, &m_vDirection);
			D3DXVec3Normalize(&m_vUp, &m_vUp);
			D3DXVec3Cross(&vRight, &m_vDirection, &m_vUp);
			m_pTransCom->Move_Pos(&(vRight * fSpeed * fTimeDelta));
		}

		if (Get_DIKeyState(DIK_D) & 0X80)
		{
			_vec3	vRight;
			D3DXVec3Normalize(&m_vDirection, &m_vDirection);
			D3DXVec3Normalize(&m_vUp, &m_vUp);
			D3DXVec3Cross(&vRight, &m_vDirection, &m_vUp);
			m_pTransCom->Move_Pos(&(vRight * -fSpeed * fTimeDelta));
		}		

		m_bDash = false;
		m_iCountDash = 0;
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
		for (auto iter : m_szBulletName)
		delete iter;

	if(m_szBulletName.size() == 0)
	m_szBulletName.clear();
	CGameObject::Free();
}
