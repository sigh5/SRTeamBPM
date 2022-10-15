#include "stdafx.h"
#include "..\Header\TestWeapon.h"

#include "Export_Function.h"

#include "AbstractFactory.h"
#include "TestPlayer.h"
#include "MyCamera.h"
#include "Inventory_UI.h"
#include "Status_UI.h"
#include "Gun_Screen.h"
#include "Player.h"

USING(Engine)

CTestWeapon::CTestWeapon(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEquipmentBase(pGraphicDev)
{
}

CTestWeapon::~CTestWeapon()
{
}

HRESULT CTestWeapon::Ready_Object(_uint iX, _uint iZ)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
		
	Engine::CEquipmentBase::Ready_EquipInfo(10, 0, 0, 10, WEAPON_MAGNUM);

	m_RenderID = RENDER_ALPHA;

	m_pTransCom->Set_Pos((_float)iX, 1.f, (_float)iZ);
	m_pTransCom->Compulsion_Update();

	m_fX = 100.f;
	m_fY = 100.f;

	m_fSizeX = 200.f;
	m_fSizeY = 200.f;

	return S_OK;
}

_int CTestWeapon::Update_Object(const _float & fTimeDelta)
{
	CInventory_UI* pInven = static_cast<CInventory_UI*>(Get_GameObject(L"Layer_UI", L"InventoryUI"));

	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	m_fX = (_float)ptMouse.x;
	m_fY = (_float)ptMouse.y;

	if (pInven->Get_InvenSwitch() == true && m_bRenderFalse == true)
	{
		if (Engine::Get_DIMouseState(DIM_LB)) // Picking
		{
			_uint ia = 0;

			Picking();
		}
	}

	Add_RenderGroup(m_RenderID, this);

	Engine::CGameObject::Update_Object(fTimeDelta);

	return 0;
}

void CTestWeapon::LateUpdate_Object(void)
{	
	if (!m_bRenderControl && !m_bRenderFalse)
	{
		CMyCamera* pCamera = static_cast<CMyCamera*>(Get_GameObject(L"Layer_Environment", L"CMyCamera"));
		NULL_CHECK(pCamera);

		_matrix		matWorld, matView, matBill;

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixIdentity(&matBill);
		memcpy(&matBill, &matView, sizeof(_matrix));
		memset(&matBill._41, 0, sizeof(_vec3));
		D3DXMatrixInverse(&matBill, 0, &matBill);

		_matrix      matScale;
		D3DXMatrixScaling(&matScale, 2.f, 2.f, 2.f);

		_matrix		 matTrans;

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
		m_pTransCom->Set_WorldMatrix(&(matWorld));
	}
}

void CTestWeapon::Render_Obejct(void)
{
	//_bool 변수로 World(필드에 있을 때), Ortho(인벤토리에 들어옴) 여부 판단
	CInventory_UI* pInven = static_cast<CInventory_UI*>(Get_GameObject(L"Layer_UI", L"InventoryUI"));

	if (!m_bRenderControl && !m_bRenderFalse)
	{
		m_RenderID = RENDER_ALPHA;
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
	if (!(pInven->Get_InvenSwitch() == false))
	{
		// 인벤토리(I)키를 눌러둔 상태에서 먹었을 때 직교투영이 되도록 하는 부분
		if (pInven->Get_InvenSwitch() == true && m_bRenderFalse == true)
		{
			m_RenderID = RENDER_ICON;
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

			_matrix		OldViewMatrix, OldProjMatrix;

			m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldViewMatrix);
			m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);

			_matrix		ViewMatrix;

			ViewMatrix = *D3DXMatrixIdentity(&ViewMatrix);

			_matrix		matProj;

			D3DXMatrixOrthoLH(&matProj, WINCX, WINCY, 0.f, 1.f);

			// ★ 아이템 획득하면 인벤토리 칸에 들어가도록 해줘야 함 인벤토리 UI에서

			for (_uint i = iIndexColumn; i < pInven->Get_MaxColumn(); ++i)
			{
				for (_uint j = iIndexRow; j < pInven->Get_MaxRow(); ++j)
				{
					if (pInven->Get_ItemSlot()->pItem[i][j] != nullptr)
					{
						CTransform* pTransform = static_cast<CTransform*>(Engine::Get_Component(L"Layer_UI", L"InventoryUI", L"Proto_TransformCom", ID_DYNAMIC));

						_vec3 vecInvenPos;

						pTransform = dynamic_cast<CTransform*>((pInven->Get_ItemSlot()->pItem[i][j]->Get_Component(L"Proto_TransformCom", ID_DYNAMIC)));

						pTransform->Get_Info(INFO_POS, &vecInvenPos);

						m_pTransCom->Set_Pos(vecInvenPos.x, vecInvenPos.y, 0.1f);

						iIndexRow += 1;
					}

					if (iIndexRow >= 9)
					{
						iIndexColumn += 1;
						iIndexRow = 0;
					}
				}
			}

			_vec3		vecIconScale = { 70.f, 70.f, 0.f };

			m_pTransCom->Set_Scale(&vecIconScale);

			m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

			m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xDF);
			m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


			m_pTextureCom->Set_Texture(0);
			m_pBufferCom->Render_Buffer();

			m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

			m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		}

		// 직교투영이 되는 부분
		if (m_bRenderControl && m_bRenderFalse)
		{
			m_RenderID = RENDER_ICON;
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

			_matrix		OldViewMatrix, OldProjMatrix;

			m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldViewMatrix);
			m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);

			_matrix		ViewMatrix;

			ViewMatrix = *D3DXMatrixIdentity(&ViewMatrix);

			_matrix		matProj;

			D3DXMatrixOrthoLH(&matProj, WINCX, WINCY, 0.f, 1.f);

			// ★ 아이템 획득하면 인벤토리 칸에 들어가도록 해줘야 함 인벤토리 UI에서

			for (_uint i = iIndexColumn; i < pInven->Get_MaxColumn(); ++i)
			{
				for (_uint j = iIndexRow; j < pInven->Get_MaxRow(); ++j)
				{
					if (pInven->Get_ItemSlot()->pItem[i][j] != nullptr)
					{
						CTransform* pTransform = static_cast<CTransform*>(Engine::Get_Component(L"Layer_UI", L"InventoryUI", L"Proto_TransformCom", ID_DYNAMIC));

						_vec3 vecInvenPos;

						pTransform = dynamic_cast<CTransform*>((pInven->Get_ItemSlot()->pItem[i][j]->Get_Component(L"Proto_TransformCom", ID_DYNAMIC)));

						pTransform->Get_Info(INFO_POS, &vecInvenPos);

						m_pTransCom->Set_Pos(vecInvenPos.x, vecInvenPos.y, 0.1f);

						iIndexRow += 1;
					}

					if (iIndexRow >= 9)
					{
						iIndexColumn += 1;
						iIndexRow = 0;
					}
				}
			}

			_vec3		vecIconScale = { 70.f, 70.f, 0.f };

			m_pTransCom->Set_Scale(&vecIconScale);

			m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

			m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xDF);
			m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


			m_pTextureCom->Set_Texture(0);
			m_pBufferCom->Render_Buffer();

			m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

			m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		}

	}

}

void CTestWeapon::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	CGameObject *pGameObject = nullptr;

	pGameObject = pLayer->Get_GameObject(L"Player");
	NULL_CHECK_RETURN(pGameObject, );

	if (m_pColliderCom->Check_Collision(this, pGameObject, 1, 1))
	{
		if (Engine::Key_Down(DIK_F))
		{
			Open_Event(pGameObject);
			
			CInventory_UI* pInven = static_cast<CInventory_UI*>(Get_GameObject(L"Layer_UI", L"InventoryUI"));
			pInven->Set_ItemInfoAdd(true);
		}
	}
}

void CTestWeapon::Picking(void)
{
	_vec3		vecScale = { m_fSizeX, m_fSizeY, 1.f };

	m_pTransCom->Set_Scale(&vecScale);
	m_pTransCom->Set_Pos(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f);  // RECT 있는 곳으로?

	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	RECT		rcUI = { (_long)(m_fX - m_fSizeX * 1.5f), (_long)(m_fY - m_fSizeY  *1.5f), (_long)(m_fX + m_fSizeX * 1.5f), (_long)(m_fY + m_fSizeY * 1.5f) };
	
	if (PtInRect(&rcUI, ptMouse))
	{
		_vec3		vPoint;
		
		vPoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
		vPoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
		vPoint.z = 0.1f;

		_matrix matOrtho;

		D3DXMatrixOrthoLH(&matOrtho, WINCX, WINCY, 0.f, 1.f);  // 직교 투영

		D3DXMatrixInverse(&matOrtho, nullptr, &matOrtho); // 뷰 스페이스(뷰가 항등이라 월드와 같다)
		D3DXVec3TransformCoord(&vPoint, &vPoint, &matOrtho);

		m_pTransCom->Set_Pos(vPoint.x, vPoint.y, 0.f);
				
	}
}

HRESULT CTestWeapon::Open_Event(CGameObject * pGameObject)
{
	CScene* pScene = ::Get_Scene();
	CLayer* pMyLayer = pScene->GetLayer(L"Layer_Icon");

	CDynamic_Transform *pTransform = dynamic_cast<CDynamic_Transform*>(pGameObject->Get_Component(L"Proto_DynamicTransformCom", ID_DYNAMIC));

	_vec3 vObjPos;
	_vec3 vPlayerPos;

	pTransform->Get_Info(INFO_POS, &vObjPos);
	m_pTransCom->Get_Info(INFO_POS, &vPlayerPos);
	// ★
	if (m_pColliderCom->Check_Sphere_InterSect(vObjPos, vPlayerPos, 1.f, 1.f) == true)
	{
		m_bRenderFalse = true;

		// 인벤토리에 들어감
		CInventory_UI* pInven = static_cast<CInventory_UI*>(Get_GameObject(L"Layer_UI", L"InventoryUI"));
		pInven->Get_WeaponType()->push_back(this);
		pInven->Get_Weapon()->push(this);
	}

	return S_OK;
}


HRESULT CTestWeapon::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_DoubleRevolverTexture", m_mapComponent, ID_STATIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);
	
	return S_OK;
}


CTestWeapon * CTestWeapon::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iX, _uint iZ)
{
	CTestWeapon *	pInstance = new CTestWeapon(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iX, iZ)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTestWeapon::Free(void)
{
	CGameObject::Free();
}
