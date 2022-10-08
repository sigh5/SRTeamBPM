#include "stdafx.h"
#include "..\Header\Key.h"
#include "Export_Function.h"

#include "AbstractFactory.h"
#include "MyCamera.h"
#include "TestPlayer.h"
#include "Player.h"

USING(Engine)


CKey::CKey(LPDIRECT3DDEVICE9 pGraphicDev)
	: CItemBase(pGraphicDev)
{
}

CKey::~CKey()
{
}

HRESULT CKey::Ready_Object(_uint iX, _uint iY)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Pos((_float)iX, 1.f, (_float)iY);
	m_pTransCom->Compulsion_Update();

	return S_OK;
}

_int CKey::Update_Object(const _float & fTimeDelta)
{
	_uint iRssult = Engine::CGameObject::Update_Object(fTimeDelta);

	Set_OnTerrain();

	Add_RenderGroup(RENDER_ALPHA, this);

	return iRssult;
}

void CKey::LateUpdate_Object(void)
{
	//CMyCamera* pCamera = static_cast<CMyCamera*>(Get_GameObject(L"Layer_Environment", L"CMyCamera"));
	//NULL_CHECK(pCamera);

	//// 
	//_matrix		matWorld, matView, matBill;

	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//D3DXMatrixIdentity(&matBill);
	//memcpy(&matBill, &matView, sizeof(_matrix));
	//memset(&matBill._41, 0, sizeof(_vec3));
	//D3DXMatrixInverse(&matBill, 0, &matBill);

	//_matrix      matScale, matTrans;
	//D3DXMatrixScaling(&matScale, 2.f, 2.f, 2.f);

	//_matrix      matRot;
	//D3DXMatrixIdentity(&matRot);
	//D3DXMatrixRotationY(&matRot, (_float)pCamera->Get_BillBoardDir());

	//_vec3 vPos;
	//m_pTransCom->Get_Info(INFO_POS, &vPos);

	//D3DXMatrixTranslation(&matTrans,
	//	vPos.x,
	//	vPos.y,
	//	vPos.z);

	//D3DXMatrixIdentity(&matWorld);
	//matWorld = matScale* matRot * matBill * matTrans;
	//m_pTransCom->Set_WorldMatrix(&(matWorld));


	//CItemBase::LateUpdate_Object();
}

void CKey::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_pTextureCom->Set_Texture(0);	// 텍스처 정보 세팅을 우선적으로 한다.

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

void CKey::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	NULL_CHECK_RETURN(pLayer, );
	CGameObject *pGameObject = nullptr;

	pGameObject = pLayer->Get_GameObject(L"TestPlayer");
	NULL_CHECK_RETURN(pGameObject, );

	if (!m_pColliderCom->Check_Collision(this, pGameObject, 1, 1))
	{
		if (Engine::Key_Down(DIK_F))
		{
			CTestPlayer* pTestPlayer = static_cast<CTestPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"TestPlayer"));

			m_bAddKey = true;
			pTestPlayer->Set_bCurStat(true);
		}
	}
}

HRESULT CKey::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_KeyTexture", m_mapComponent, ID_STATIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);

	return S_OK;
}

void CKey::Set_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransCom->Set_Pos(vPos.x, fHeight, vPos.z);
}

CKey * CKey::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iX, _uint iY)
{
	CKey* pInstance = new CKey(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iX, iY)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CKey::Free(void)
{
	CItemBase::Free();
}
