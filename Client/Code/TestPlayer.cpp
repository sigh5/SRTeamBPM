#include "stdafx.h"
#include "..\Header\TestPlayer.h"

#include "Export_Function.h"

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

	Add_RenderGroup(RENDER_NONALPHA, this);

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

	m_pTextureCom->Set_Texture(0);	// �ؽ�ó ���� ������ �켱������ �Ѵ�.
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_PlayerTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_PlayerTexture", pComponent });

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

	if (CInputDev::GetInstance()->Key_Pressing(DIK_LSHIFT))
	{
		m_bDash = TRUE;
	}
	else
	{
		m_bDash = FALSE;
	}

	if (m_bDash == FALSE)
	{
		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			if (m_fBuffDashPower > 0.f)
				m_fBuffDashPower -= m_fDashPower;
			else if (m_fBuffDashPower < 0.f)
			{
				m_fBuffDashPower = 0.f;
			}

			D3DXVec3Normalize(&m_vDirection, &m_vDirection);
			m_pTransCom->Move_Pos(&(m_vDirection * (10.f + m_fBuffDashPower) * fTimeDelta));
		}
	}
	else
	{
		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			if (m_fBuffDashPower < 20.f)
				m_fBuffDashPower += m_fDashPower;

			D3DXVec3Normalize(&m_vDirection, &m_vDirection);
			m_pTransCom->Move_Pos(&(m_vDirection * (10.f + m_fBuffDashPower) * fTimeDelta));
		}
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pTransCom->Move_Pos(&(m_vDirection * -10.f * fTimeDelta));
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_pTransCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_pTransCom->Rotation(ROT_Y, D3DXToRadian(-180.f * fTimeDelta));

	if (Engine::Get_DIMouseState(DIM_LB) & 0X80)
	{
		_vec3	vPickPos = PickUp_OnTerrain();
		_vec3	vPlayerPos, vDir;
		m_pTransCom->Get_Info(INFO_POS, &vPlayerPos);
		vDir = vPickPos - vPlayerPos;
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransCom->Move_Pos(&(vDir * 5.f * fTimeDelta));
	}

	if (Engine::Get_DIKeyState(DIK_SPACE) & 0X80)
	{
		m_bJump = TRUE;
	}

}

void CTestPlayer::Set_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);
	
	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransCom->Set_Pos(vPos.x, fHeight, vPos.z);

	
}

Engine::_vec3 CTestPlayer::PickUp_OnTerrain(void)
{
	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, _vec3());


	return m_pCalculatorCom->PickingOnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
}

float CTestPlayer::Get_TerrainY(void)
{
	_vec3		vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);


	return fHeight;
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
