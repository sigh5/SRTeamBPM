#include "stdafx.h"
#include "..\Header\Box.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "Coin.h"
#include "HealthPotion.h"

USING(Engine)

CBox::CBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CBox::~CBox()
{
}

HRESULT CBox::Ready_Object(_uint iX, _uint iY, CTestPlayer* pPlayer)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransCom->Set_Pos(iX, 1.f, iY);
	m_pTransCom->Compulsion_Update();
	m_pAnimationCom->Ready_Animation(1, 0, 0.2f, 3); // �ִϸ��̼��� �ڵ����� ���ư��� �Լ�. �ٲ���� �Ѵ�.

	m_pPlayer = pPlayer;

	return S_OK;
}

_int CBox::Update_Object(const _float & fTimeDelta)
{
	_uint iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	Set_OnTerrain();


	Add_RenderGroup(RENDER_ALPHA, this);

	return iResult;
}

void CBox::LateUpdate_Object(void)
{

}

void CBox::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CBox::Set_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransCom->Set_Pos(vPos.x, fHeight+0.9f, vPos.z);

	/*
	// HealthPotion
	pGameObject = CHealthPotion::Create(m_pGraphicDev, 5, 10);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"HealthPotion", pGameObject), E_FAIL);

	// Coin
	pGameObject = CCoin::Create(m_pGraphicDev, 22, 10);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Coin", pGameObject), E_FAIL);

	*/
}

void CBox::Collision_Event(CGameObject * pGameObject)
{
	CScene* pScene = ::Get_Scene();
	CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");

	CTransform *pTransform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));

	_vec3 vObjPos;
	_vec3 vPlayerPos;

	pTransform->Get_Info(INFO_POS, &vObjPos);
	m_pTransCom->Get_Info(INFO_POS, &vPlayerPos);
	// ��
	if (m_pColliderCom->Check_Sphere_InterSect(vObjPos, vPlayerPos, 1.f, 1.f) == true && m_pPlayer->Get_BoxOpen() == true)
	{
		_vec3 vPos;

		m_pTransCom->Get_Info(INFO_POS, &vPos);

		// HealthPotion

		CHealthPotion* pPotion = CHealthPotion::Create(m_pGraphicDev, vPos.x, vPos.z);
		NULL_CHECK(pPotion);

		
		/*
		
		CBullet* pBullet = CBullet::Create(m_pGraphicDev, vPos);
		NULL_CHECK(pBullet);

		_tchar*         szFinalName = new _tchar[128];
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

		*/
	
	}

}

HRESULT CBox::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_BoxTexture", m_mapComponent, ID_STATIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);



	return S_OK;
}



CBox * CBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iX, _uint iY, CTestPlayer* pPlayer)
{
	CBox*	pInstance = new CBox(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iX, iY, pPlayer)))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CBox::Free(void)
{
	CGameObject::Free();
}
