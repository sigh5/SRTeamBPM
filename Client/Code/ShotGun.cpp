#include "stdafx.h"
#include "..\Header\ShotGun.h"
#include "Export_Function.h"

#include "AbstractFactory.h"
#include "TestPlayer.h"
#include "MyCamera.h"
#include "Inventory_UI.h"

CShotGun::CShotGun(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEquipmentBase(pGraphicDev) 
{
}

CShotGun::~CShotGun()
{
}

HRESULT CShotGun::Ready_Object(_uint iX, _uint iZ)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Engine::CEquipmentBase::Ready_EquipInfo(10, 0, 0, 10, WEAPON_SHOTGUN);
	
	m_RenderID = RENDER_ALPHA;

	m_pTransCom->Set_Pos((_float)iX, 1.f, (_float)iZ);
	m_pTransCom->Compulsion_Update();

	return S_OK;
}
 
_int CShotGun::Update_Object(const _float & fTimeDelta)
{
	_uint iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	Set_OnTerrain();

	Add_RenderGroup(m_RenderID, this);

	return iResult;
}

void CShotGun::LateUpdate_Object(void)
{
	// MyCamera를 통한 빌보드
	/*CMyCamera* pCamera = static_cast<CMyCamera*>(Get_GameObject(L"Layer_Environment", L"CMyCamera"));
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
	D3DXMatrixRotationY(&matRot, (_float)pCamera->Get_BillBoardDir());

	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	D3DXMatrixTranslation(&matTrans,
		vPos.x,
		vPos.y,
		vPos.z);

	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale* matRot * matBill * matTrans;
	m_pTransCom->Set_WorldMatrix(&(matWorld));*/

}

void CShotGun::Render_Obejct(void)
{
	//_bool 변수로 World(필드에 있을 때), Ortho(인벤토리에 들어옴) 여부 판단
	
	if (!m_bRenderControl)
	{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	
	
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_pTextureCom->Set_Texture(0);	
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	}

	if (m_bRenderControl)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

		_matrix		OldViewMatrix, OldProjMatrix;

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);

		_matrix		ViewMatrix;

		ViewMatrix = *D3DXMatrixIdentity(&ViewMatrix);

		_matrix		matProj;

		D3DXMatrixOrthoLH(&matProj, WINCX, WINCY, 0.f, 1.f);

		m_pTransCom->Set_Pos(-50.f, -50.f, 0.1f);

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
		
		m_pTextureCom->Set_Texture(0);
		m_pBufferCom->Render_Buffer();	

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);
	}



}

void CShotGun::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	CGameObject *pGameObject = nullptr;

	pGameObject = pLayer->Get_GameObject(L"TestPlayer");
	NULL_CHECK_RETURN(pGameObject, );
	
	if (Get_DIKeyState(DIK_F) & 0X80)
	{
		if (!m_pColliderCom->Check_Collision(this, pGameObject, 1, 1))
		{
			CInventory_UI* pInven = static_cast<CInventory_UI*>(Get_GameObject(L"Layer_UI", L"InventoryUI"));

			//CTestPlayer* pPlayer = static_cast<CTestPlayer*>(Get_GameObject(L"Layer_GameLogic", L"TestPlayer"));

			pInven->Get_WeaponType()->push_back(this);

			//pPlayer->Get_WeaponType()->push_back(this);

			/*for (_uint i = 0; i < WEAPON_END; ++i)
			{
				if (i == (m_EquipInfo.m_WeaponType == WEAPON_SHOTGUN))
				{
					CTestPlayer* pPlayer = static_cast<CTestPlayer*>(Get_GameObject(L"Layer_GameLogic", L"TestPlayer"));

					pPlayer->Get_WeaponType()->push_back(this);
				}
			}*/

			//_uint iAddPower = (_uint)static_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC))->Get_InfoRef()._iAttackPower;

			//pPlayer->

		}
	}

}

void CShotGun::Set_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransCom->Set_Pos(vPos.x, fHeight + 0.8f, vPos.z);

	
}

HRESULT CShotGun::Add_Component(void)
{ // CDynamic_Transform
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_ShotGunTexture", m_mapComponent, ID_STATIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);
	

	return S_OK;
}

CShotGun * CShotGun::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iX, _uint iZ)
{
	CShotGun* pInstance = new CShotGun(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iX, iZ)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CShotGun::Free(void)
{
	CGameObject::Free();
}
