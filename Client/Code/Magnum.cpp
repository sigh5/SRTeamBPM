#include "stdafx.h"
#include "..\Header\Magnum.h"
#include "Export_Function.h"

#include "AbstractFactory.h"
#include "MyCamera.h"
#include "Inventory_UI.h"
#include "Status_UI.h"
#include "Gun_Screen.h"
#include "Player.h"

CMagnum::CMagnum(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEquipmentBase(pGraphicDev)
{
}

CMagnum::~CMagnum()
{
}

HRESULT CMagnum::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	Engine::CEquipmentBase::Ready_EquipInfo(5, 0, 0, 7, WEAPON_MAGNUM);
	
	m_RenderID = RENDER_ICON;
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.f, 1.f);

	m_fSizeX = 100.f;
	m_fSizeY = 100.f;

	m_fX = (350 - 30 + 350 + 30) / 2.f;
	m_fY = (275 - 30 + 275 + 30) / 2.f;

	_vec3 vScale = { m_fSizeX, m_fSizeY, 1.f };
	m_pTransCom->Set_Scale(&vScale);

	m_pTransCom->Set_Pos(m_fX - WINCX * 0.5f,
		(-m_fY + WINCY * 0.5f), 0.0f);
	
	m_fOriginPosX = m_fX - WINCX * 0.5f;
	m_fOriginPosY = (-m_fY + WINCY * 0.5f);

	m_bIsInventory = true;
	m_bIsWorld = false;
	m_EquipState = EquipState_Equip_Weapon;
	m_pTransCom->Update_Component(1.f);

	return S_OK;
}

_int CMagnum::Update_Object(const _float & fTimeDelta)
{
	
	m_fTimedelta = fTimeDelta;

	if (m_bIsWorld)
		m_RenderID = RENDER_ALPHA;
	else if (m_bIsInventory)
	{
		m_RenderID = RENDER_ICON;
		m_bIsPick = EquipIconPicking();
	}
	if (m_bIsPick)
		Set_MouseToInventory();

	PickingMouseUp();

	if (m_bOnce)
	{
		// 딱한번만 
		CInventory_UI* pInven = static_cast<CInventory_UI*>(Get_GameObject(L"Layer_UI", L"InventoryUI"));
		pInven->Set_CurrentEquipWeapon(this);
		CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
		pPlayer->EquipItem_Add_Stat(m_EquipInfo._iAddAttack);
		m_bOnce = false;
	}


	_uint iResult = Engine::CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(m_RenderID, this);

	return iResult;
}

void CMagnum::LateUpdate_Object(void)
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
	
	m_iMouseUpEnd = false;
}

void CMagnum::Render_Obejct(void)
{
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
	else if (m_bIsInventory)
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
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
		m_pTextureCom->Set_Texture(0);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		return;
	}
}

void CMagnum::Collision_Event()
{
	
}

void CMagnum::Change_Equip()
{
	if (m_EquipState == EquipState_Equip_Weapon)
		return;
	
	CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	pPlayer->EquipItem_Add_Stat(m_EquipInfo._iAddAttack);

	CGun_Screen* pGun_Screen = static_cast<CGun_Screen*>(Engine::Get_GameObject(L"Layer_UI", L"Gun"));
	pGun_Screen->Set_ChaneWeaponUI_ID(true,ID_MAGNUM);

	CInventory_UI* pInven = static_cast<CInventory_UI*>(Get_GameObject(L"Layer_UI", L"InventoryUI"));
	pInven->Set_CurrentEquipWeapon(this);


}

void CMagnum::Set_MouseToInventory() // 누르면 걍 따라오는 함수
{
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_vec3		vPoint;
	m_fX = ptMouse.x;
	m_fY = ptMouse.y;

	m_pTransCom->Set_Pos(m_fX - WINCX * 0.5f,
		(-m_fY + WINCY * 0.5f), 0.f);
}

void CMagnum::PickingMouseUp()
{

	if (m_bisPicking && !m_bPickingEnd) // 마우스 들었을때
	{
		if(::Mouse_Up(DIM_LB))
			m_bPickingEnd = true;
	}

	if (m_bPickingEnd == true) // 나중에 장비창에 있는지 슬롯창에있는지 확인하는 조건 필요
	{
		CInventory_UI* pInven = static_cast<CInventory_UI*>(Get_GameObject(L"Layer_UI", L"InventoryUI"));
		SearchInventorySlot(&pInven);

		 if (!m_iMouseUpEnd )
		{
			RECT Rc{};
			if (m_EquipState == EquipState_Equip_Weapon)
			{
				memcpy(&Rc, &pInven->Get_EquipSlot()[m_iInvenSlotIndex].rcInvenSlot, sizeof(RECT));
				m_fX = (Rc.left + Rc.right) / 2.f;
				m_fY = (Rc.top + Rc.bottom) / 2.f;
				m_pTransCom->Set_Pos(m_fX - WINCX * 0.5f,
					(-m_fY + WINCY * 0.5f), 0.f);
			}

			else if (m_iInvenSlotIndex != -1 && pInven->Get_InvenSlot()[m_iInvenSlotIndex].bSlotEmpty)
			{
				memcpy(&Rc, &pInven->Get_InvenSlot()[m_iInvenSlotIndex].rcInvenSlot, sizeof(RECT));

				m_fX = (Rc.left + Rc.right) / 2.f;
				m_fY = (Rc.top + Rc.bottom) / 2.f;
				m_pTransCom->Set_Pos(m_fX - WINCX * 0.5f,
					(-m_fY + WINCY * 0.5f), 0.f);
			}
		}
		pInven->Set__Current_Picking_ItemID(0);
	}

}

void CMagnum::SearchInventorySlot(CInventory_UI ** pInven)
{
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_vec3		vPoint;
	m_fX = ptMouse.x;
	m_fY = ptMouse.y;

	RECT rcMouse = { ptMouse.x - 30.f ,ptMouse.y - 35.f ,ptMouse.x + 30.f ,ptMouse.y + 30.f };
	RECT Rc{};
	
	if (m_EquipState != EquipState_Equip_Weapon)
	{
		memcpy(&Rc, &(*pInven)->Get_EquipSlot()[0].rcInvenSlot, sizeof(RECT));
		if (PtInRect(&Rc, ptMouse))
		{
			Change_Equip();

			m_iMouseUpEnd = true;
		}

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 9; ++j)
			{
				int iIndex = (i * 9) + j;
				memcpy(&Rc, &(*pInven)->Get_InvenSlot()[iIndex].rcInvenSlot, sizeof(RECT));

				if (PtInRect(&Rc, ptMouse))
				{
					m_iInvenSlotIndex = -1;
					m_fX = (Rc.left + Rc.right) / 2.f;
					m_fY = (Rc.top + Rc.bottom) / 2.f;

					//cout << m_fX - WINCX * 0.5f << " " << (-m_fY + WINCY * 0.5f);

					m_pTransCom->Set_Pos(m_fX - WINCX * 0.5f,
						(-m_fY + WINCY * 0.5f), 0.f);

					m_fOriginPosX = m_fX - WINCX * 0.5f;
					m_fOriginPosY = (-m_fY + WINCY * 0.5f);

					m_pTransCom->Update_Component(1.f);
					m_iMouseUpEnd = true;

					(*pInven)->Get_InvenSlot()[iIndex].bSlotEmpty = true;
					m_iInvenSlotIndex = iIndex;
					break;
				}
			}
		}
	}
	m_bIsPick = false;
	m_bPickingEnd = false;
}

_bool CMagnum::EquipIconPicking()
{
	CInventory_UI* pInven = static_cast<CInventory_UI*>(Get_GameObject(L"Layer_UI", L"InventoryUI"));

	const _uint iCurrentPickingID = pInven->Get_Current_Picking_ItemID();

	if (!(iCurrentPickingID == ID_MAGNUM || iCurrentPickingID == 0))
		return false;

	POINT		ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	RECT		rcUI2 = {
		m_fX - m_fSizeX * 0.5f ,
		m_fY - m_fSizeY * 0.5f ,
		m_fX + m_fSizeX * 0.5f ,
		m_fY + m_fSizeY * 0.5f };

	if (Get_DIMouseState(DIM_LB) & 0x80)
	{
		cout << ptMouse.x << " " << ptMouse.y << endl;
		if (PtInRect(&rcUI2, ptMouse))
		{
			pInven->Set__Current_Picking_ItemID(ID_MAGNUM);
			m_bPickingEnd = false;
			m_bisPicking = true;
			return true;
		}
		m_bisPicking = false;
		return false;

	}


	return false;
}


HRESULT CMagnum::Add_Component(void)
{
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_MagnumTexture", m_mapComponent, ID_STATIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);

	return S_OK;
}

CMagnum * CMagnum::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMagnum* pInstance = new CMagnum(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMagnum::Free(void)
{
	CGameObject::Free();
}
