#include "stdafx.h"
#include "..\Header\Player.h"

#include "AbstractFactory.h"
#include "Export_Function.h"
#include "Box.h"





CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_vDirection(0.f, 0.f, 0.f)
	, m_fJumpPower(5.0f)
	, m_fDashPower(0.3f)
	, m_tpType(TYPING_END)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_Object(void)
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pInfoCom->Ready_CharacterInfo(100, 10, 5.f);

	m_pDynamicTransCom->Set_Pos(10.f, 2.f, 10.f);
	_vec3 vScale = { 0.1f, 0.1f, 0.1f };
	m_pDynamicTransCom->Set_Scale(&vScale);
	m_pDynamicTransCom->Update_Component(1.5f);

	return S_OK;
}

_int CPlayer::Update_Object(const _float & fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;
	m_fFrame += 1.0f * fTimeDelta;

	if (m_fFrame >= 1.0f)
	{
		m_bOneShot = false;
		m_fFrame = 0.f;
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

	m_pDynamicTransCom->Set_Y(2.f);

	Engine::CGameObject::Update_Object(fTimeDelta);


	//Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CPlayer::LateUpdate_Object(void)
{

	Set_OnTerrain();

	CGameObject::LateUpdate_Object();
}

void CPlayer::Render_Obejct(void)
{
	/*m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pDynamicTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(m_iTexIndex);
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);*/
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

	if (Get_DIKeyState(DIK_W) & 0X80)
	{
		m_tpType = TYPING_W;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pDynamicTransCom->Move_Pos(&(m_vDirection * 5.f * fTimeDelta));
		m_pDynamicTransCom->Set_CountMovePos(&(m_vDirection * 5.f * fTimeDelta));
		
	}

	if (Get_DIKeyState(DIK_S) & 0X80)
	{
		m_tpType = TYPING_S;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pDynamicTransCom->Move_Pos(&(m_vDirection * -5.f * fTimeDelta));
		m_pDynamicTransCom->Set_CountMovePos(&(m_vDirection * -5.f * fTimeDelta));
		
	}

	if (Get_DIKeyState(DIK_A) & 0X80)
	{
		m_tpType = TYPING_A;
		_vec3	vRight;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		D3DXVec3Normalize(&m_vUp, &m_vUp);
		D3DXVec3Cross(&vRight, &m_vDirection, &m_vUp);
		m_pDynamicTransCom->Move_Pos(&(vRight * 5.f * fTimeDelta));
		m_pDynamicTransCom->Set_CountMovePos(&(vRight * 5.f * fTimeDelta));
		
	}

	if (Get_DIKeyState(DIK_D) & 0X80)
	{
		m_tpType = TYPING_D;
		_vec3	vRight;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		D3DXVec3Normalize(&m_vUp, &m_vUp);
		D3DXVec3Cross(&vRight, &m_vDirection, &m_vUp);
		m_pDynamicTransCom->Move_Pos(&(vRight * -5.f * fTimeDelta));
		m_pDynamicTransCom->Set_CountMovePos(&(vRight * -5.f * fTimeDelta));
		
	}

	if (Get_DIKeyState(DIK_SPACE) & 0X80)
		m_bJump = TRUE;

	if (Get_DIKeyState(DIK_LSHIFT) & 0X80)
	{
		m_bDash = TRUE;

		if (m_bDash)
		{
			m_pDynamicTransCom->Dashing(fTimeDelta, m_pDynamicTransCom, m_vUp, m_vDirection, m_tpType);
			m_bDash = FALSE;
		}
	}


	if (::Mouse_Down(DIM_LB)) // Picking
	{
		if (m_iMagazine <= 0)
			m_bOneShot = FALSE;

		m_bCheckShot = Create_RayCheck(fTimeDelta);

		if (m_bCheckShot == false)
		{
			::PlaySoundW(L"Rythm_Check_Fail.wav", SOUND_EFFECT, 0.1f);
			m_iComboCount = 0;
		}
	}


	if (Get_DIKeyState(DIK_R) & 0X80)
	{
		m_iMagazine = 8;
	}

}

void CPlayer::Set_OnTerrain(void)
{
	_vec3		vPos;
	m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);

	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pDynamicTransCom->Set_Pos(vPos.x, fHeight, vPos.z);
}

_vec3 CPlayer::PickUp_OnTerrain(void)
{
	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, _vec3());

	return m_pCalculatorCom->PickingOnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
}

_bool CPlayer::Create_RayCheck(const _float & fTimeDelta)
{
	if (m_bOneShot)
	{
		m_iMagazine -= 1;
		m_bOneShot = false;
		return true;
	}

	return false;
}

void CPlayer::Collision_Event()
{
	// ������ �����ϴ� �͵��� Player�� 
	//	������ �����Ǵ� �͵��� �� ��ü�� 
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_CubeCollsion");
	NULL_CHECK_RETURN(pLayer, );
	CGameObject *pGameObject = nullptr;

	for (auto iter = pLayer->Get_GameObjectMap().begin(); iter != pLayer->Get_GameObjectMap().end(); ++iter)
	{
		m_pColliderCom->Check_Collision_Wall(iter->second, this);
	}

	pLayer = pScene->GetLayer(L"Layer_GameLogic");
	NULL_CHECK_RETURN(pLayer, );

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