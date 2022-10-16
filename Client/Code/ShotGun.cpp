#include "stdafx.h"
#include "..\Header\ShotGun.h"
#include "Export_Function.h"

#include "AbstractFactory.h"
#include "TestPlayer.h"
#include "MyCamera.h"
#include "Inventory_UI.h"
#include "Status_UI.h"
#include "Gun_Screen.h"
#include "Player.h"

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
	
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.f, 1.f);

	m_pTransCom->Set_Pos((_float)iX, 1.f, (_float)iZ);
	m_pTransCom->Compulsion_Update();

	return S_OK;
}
 
_int CShotGun::Update_Object(const _float & fTimeDelta)
{
	_uint iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	if (m_bIsWorld)
		m_RenderID = RENDER_ALPHA;
	else if (m_bIsInventory)
	{
		m_RenderID = RENDER_ICON;
		m_bIsPick = EquipIconPicking();
	}

	if (m_bIsPick)
	{
		Set_MouseToInventory();
	}


	Add_RenderGroup(m_RenderID, this);
	
	return iResult;
}

void CShotGun::LateUpdate_Object(void)
{
	if (m_bIsWorld)
	{
		CMyCamera* pCamera = static_cast<CMyCamera*>(Get_GameObject(L"Layer_Environment", L"CMyCamera"));
		NULL_CHECK(pCamera);
	
		_matrix		matWorld, matView, matBill;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixIdentity(&matBill);
		memcpy(&matBill, &matView, sizeof(_matrix));
		memset(&matBill._41, 0, sizeof(_vec3));
		D3DXMatrixInverse(&matBill, 0, &matBill);

		_matrix      matScale, matTrans, matRot;
		D3DXMatrixScaling(&matScale, 2.f, 2.f, 2.f);
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
		m_pTransCom->Set_WorldMatrix(&(matWorld));
	}
}

void CShotGun::Render_Obejct(void)
{
	//_bool 변수로 World(필드에 있을 때), Ortho(인벤토리에 들어옴) 여부 판단
	if (m_bIsWorld)
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
		return;
	}
	else if (m_bIsInventory )
	{
		CInventory_UI* pInven = static_cast<CInventory_UI*>(Get_GameObject(L"Layer_UI", L"InventoryUI"));

		if (!pInven->Get_InvenSwitch())
			return;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
		
		_matrix		OldViewMatrix, OldProjMatrix;

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);

		_matrix		ViewMatrix;
		ViewMatrix = *D3DXMatrixIdentity(&ViewMatrix);
		/*m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);*/
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
		m_pTextureCom->Set_Texture(0);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);
		/*m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);*/
		return;
	}

}

void CShotGun::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	CGameObject *pGameObject = nullptr;

	pGameObject = pLayer->Get_GameObject(L"Player");
	NULL_CHECK_RETURN(pGameObject, );
			
	if (m_pColliderCom->Check_Collision(this, pGameObject, 1, 1) && !m_bOnce)
	{
		if (Get_DIKeyState(DIK_F) & 0x80)//Engine::Key_Down(DIK_F))
		{
			m_bIsWorld = false;
			m_bIsInventory = true;
			// 인벤토리에 들어감
			CInventory_UI* pInven = static_cast<CInventory_UI*>(Get_GameObject(L"Layer_UI", L"InventoryUI"));
			pInven->Get_WeaponType()->push_back(this);

			m_fX = WINCX/2.f;
			m_fY = WINCY/2.f;
			m_fSizeX = 200.0f;
			m_fSizeY = 200.f;
			_vec3 vScale = { m_fSizeX, m_fSizeY, 1.f };
			m_pTransCom->Set_Scale(&vScale);
			m_pTransCom->Set_Pos(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f);


			m_bOnce = true;
		

			//// 플레이어의 스탯에 관여하기 위함
			//CPlayer* pTestPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
			//pTestPlayer->Set_bCurStat(true);

		/*	CGun_Screen* pGun_Screen = static_cast<CGun_Screen*>(Engine::Get_GameObject(L"Layer_UI", L"Gun"));
			pGun_Screen->Set_ChangeWeaponUI(true);*/
		}

	}
}

void CShotGun::Set_MouseToInventory()
{

	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_vec3		vPoint;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	vPoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vPoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vPoint.z = 0.1f;

	_matrix matProj;
	memcpy(&matProj, &m_ProjMatrix, sizeof(_matrix));
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vPoint, &vPoint, &matProj);

	_vec3	vRayDir, vRayPos;
	vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = vPoint - vRayPos;
	_vec3	vPos = { vPoint.x,vPoint.y, 1.f };

	_matrix		matWorld;
	m_pTransCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);





	cout << vPos.x << " " << vPos.y << endl;



	m_pTransCom->Set_Pos(vPos.x, vPos.y, 0.f);

}

_bool CShotGun::EquipIconPicking()
{	
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (Get_DIMouseState(DIM_LB) & 0x80)
	{
		POINT		ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		_vec3		vPoint;

		D3DVIEWPORT9		ViewPort;
		ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
		m_pGraphicDev->GetViewport(&ViewPort);

		vPoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
		vPoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
		vPoint.z = 0.1f;

		_matrix matProj;
		memcpy(&matProj, &m_ProjMatrix, sizeof(_matrix));
		D3DXMatrixInverse(&matProj, nullptr, &matProj);
		D3DXVec3TransformCoord(&vPoint, &vPoint, &matProj);
		
		_vec3	vRayDir, vRayPos;
		vRayPos = { 0.f, 0.f, 0.f };
		vRayDir = vPoint - vRayPos;

		_matrix		matWorld;
		m_pTransCom->Get_WorldMatrix(&matWorld);
		D3DXMatrixInverse(&matWorld, nullptr, &matWorld);
		D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
		D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

		const _vec3*	pTerrainVtx = m_pBufferCom->Get_VtxPos();

		_vec3 vPos;
		m_pTransCom->Get_Info(INFO_POS, &vPos);


		_ulong	dwVtxIdx[3]{};
		_float	fU, fV, fDist;

		dwVtxIdx[0] = 0;
		dwVtxIdx[1] = 1;
		dwVtxIdx[2] = 2;

		if (D3DXIntersectTri(&(pTerrainVtx[dwVtxIdx[0]]),
			&(pTerrainVtx[dwVtxIdx[1]]),
			&(pTerrainVtx[dwVtxIdx[2]]),
			&(vRayPos+ vPos), &vRayDir,
			&fU, &fV, &fDist))
		{
			return true;
		}
		dwVtxIdx[0] = 0;
		dwVtxIdx[1] = 2;
		dwVtxIdx[2] = 3;

		if (D3DXIntersectTri(&(pTerrainVtx[dwVtxIdx[0]] ),
			&(pTerrainVtx[dwVtxIdx[1]] ),
			&(pTerrainVtx[dwVtxIdx[2]]),
			&(vRayPos+ vPos), &vRayDir,
			&fU, &fV, &fDist))
		{
			return true;
		}

		return false;
	}

		
	return false;


}



HRESULT CShotGun::Add_Component(void)
{ // CDynamic_Transform
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_ShotGunTexture", m_mapComponent, ID_STATIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);
	
	m_pOrthoTransCom = CAbstractFactory<COrthoTransform>::Clone_Proto_Component(L"Proto_OrthoTransformCom", m_mapComponent, ID_DYNAMIC);
	

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
