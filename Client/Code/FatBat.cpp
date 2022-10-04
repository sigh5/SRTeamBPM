#include "stdafx.h"
#include "..\Header\FatBat.h"

#include "Export_Function.h"
#include "MonsterBullet.h"
#include "Stage.h"
#include "AbstractFactory.h"
#include "ObjectMgr.h"
#include "MyCamera.h"
#include "Player.h"

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
	
	m_iMonsterIndex = 1;
	m_pInfoCom->Ready_CharacterInfo(100, 10, 5.f);
	m_pAnimationCom->Ready_Animation(6, 0, 0.2f);

	_vec3	vScale = { 0.5f,0.5f,0.5f };

	m_pDynamicTransCom->Set_Scale(&vScale);

	if (Posx == 0 && Posy == 0) {}
	else
	{
		Set_TransformPositon(g_hWnd, m_pCalculatorCom);
	}

	m_iDodgeDir = 0;
	m_fActionDelay = 2.f;
	m_fMaxY = 3.f;
	m_fFlyDelayCount = 0.f;
	m_fDodgeDelayCount = 0.f;
	m_bAltitude = false;
	m_fDodgeSpeed = 3.5f;
	m_fDodgeStopper = 0.15f;
	m_fStopperDelay = 0.1f;
	m_fStopperDelayCount = 0.f;
	return S_OK;
}

_int CFatBat::Update_Object(const _float & fTimeDelta)
{
	// 수정 쿨타임 대신 타임
	m_fFrame += fTimeDelta;
	if (m_fFrame > m_fActionDelay)
	{
		FatBat_Shoot();
		m_fFrame = 0.f;
	}
	// 수정 쿨타임 대신 타임

	CMonsterBase::Calculator_Distance();

	FatBat_Fly(fTimeDelta);
	FatBat_Dodge(fTimeDelta, &m_vPlayerPos, &m_vMonsterPos);

	
	//Set_OnTerrain();
	//지형에 올림
	if (fMtoPDistance > 13.f)
	{
		m_pDynamicTransCom->Chase_Target_notRot(&m_vPlayerPos, m_pInfoCom->Get_InfoRef()._fSpeed, fTimeDelta);
		
	}
	else if(fMtoPDistance < 10.f)
	{
		m_pDynamicTransCom->Chase_Target_notRot(&m_vPlayerPos, -m_pInfoCom->Get_InfoRef()._fSpeed, fTimeDelta);
	}
	else
	{

	}
	m_pAnimationCom->Move_Animation(fTimeDelta);

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
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

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
	Engine::CGameObject::LateUpdate_Object();

}

void CFatBat::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pDynamicTransCom->Get_WorldMatrixPointer());
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
}

void CFatBat::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	NULL_CHECK_RETURN(pLayer, );
	CGameObject *pGameObject = nullptr;
	pGameObject = static_cast<CPlayer*>(::Get_GameObject(L"Layer_GameLogic", L"Player"));

	_vec3 PickPos;

	if (static_cast<CPlayer*>(pGameObject)->Get_CheckShot() == true &&
		fMtoPDistance < MAX_CROSSROAD &&
		m_pColliderCom->Check_Lay_InterSect(m_pBufferCom, m_pDynamicTransCom, g_hWnd))
	{
		m_bHit = true;
		static_cast<CPlayer*>(pGameObject)->Set_ComboCount(1);

		m_pInfoCom->Receive_Damage(1);
		cout << "FatBat" << m_pInfoCom->Get_InfoRef()._iHp << endl;
	}

}

void CFatBat::Excution_Event()
{
	// 추후 로직추가
}

void	CFatBat::FatBat_Fly(const _float& fTimeDelta)
{
	float TerrainY = m_pDynamicTransCom->Get_TerrainY1(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC, m_pCalculatorCom, m_pDynamicTransCom);
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
		m_pDynamicTransCom->Move_Pos(&(vRight * (m_fDodgeSpeed - m_fDodgeStopper) * 0.1f));
		break;

	case 1:
		break;

	case 2:
		
		if (0<m_fDodgeSpeed - m_fDodgeStopper)
		m_pDynamicTransCom->Move_Pos(&(vRight * -(m_fDodgeSpeed - m_fDodgeStopper) * 0.1f));
		break;

	}

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
