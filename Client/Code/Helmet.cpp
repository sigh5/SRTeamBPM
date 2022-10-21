#include "stdafx.h"
#include "..\Header\Helmet.h"
#include "Export_Function.h"

#include "AbstractFactory.h"
#include "MyCamera.h"
#include "Inventory_UI.h"
#include "Status_UI.h"
#include "Player.h"

CHelmet::CHelmet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEquipmentBase(pGraphicDev)
{
}

CHelmet::~CHelmet()
{
}

HRESULT CHelmet::Ready_Object(_uint iX, _uint iZ)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Engine::CEquipmentBase::Ready_EquipInfo(0, 10, 0, 0, WEAPON_SHOTGUN);

	m_RenderID = RENDER_ALPHA;

	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.f, 1.f);
	m_EquipState = EquipState_Equip_End;

	m_pTransCom->Set_Pos((_float)iX, 1.f, (_float)iZ);
	m_pTransCom->Compulsion_Update();

	return S_OK;
}

_int CHelmet::Update_Object(const _float & fTimeDelta)
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
	{
		Set_MouseToInventory();
	}

	PickingMouseUp();
	_uint iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(m_RenderID, this);

	return iResult;
}

void CHelmet::LateUpdate_Object(void)
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

void CHelmet::Render_Obejct(void)
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

void CHelmet::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	CGameObject *pGameObject = nullptr;

	pGameObject = pLayer->Get_GameObject(L"Player");
	NULL_CHECK_RETURN(pGameObject, );

	if (m_pColliderCom->Check_Collision(this, pGameObject, 1, 1) && !m_bOnce)
	{
		if (Key_Down(DIK_F))
		{
			// 인벤토리에 들어감
			CInventory_UI* pInven = static_cast<CInventory_UI*>(Get_GameObject(L"Layer_UI", L"InventoryUI"));
			pInven->Get_WeaponType()->push_back(this);
			m_EquipState = EquipState_Slot;
			m_fX = WINCX * 0.5f;
			m_fY = WINCY * 0.5f;
			m_fSizeX = 100.f;
			m_fSizeY = 100.f;

			_vec3 vScale = { m_fSizeX, m_fSizeY, 1.f };
			m_pTransCom->Set_Scale(&vScale);

			RECT Rc{};
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 9; ++j)
				{
					int iIndex = (i * 9) + j;
					memcpy(&Rc, &pInven->Get_InvenSlot()[iIndex].rcInvenSlot, sizeof(RECT));

					m_fX = (Rc.left + Rc.right) / 2.f;
					m_fY = (Rc.top + Rc.bottom) / 2.f;

					if (pInven->Get_InvenSlot()[iIndex].bSlotEmpty == false)
					{
						m_pTransCom->Set_Pos(m_fX - WINCX * 0.5f,
							(-m_fY + WINCY * 0.5f), 0.0f);

						pInven->Get_InvenSlot()[iIndex].bSlotEmpty = true;
						m_pTransCom->Update_Component(1.f);
						m_bOnce = true;
						m_bIsWorld = false;
						m_bIsInventory = true;
						m_fOriginPosX = m_fX - WINCX * 0.5f;
						m_fOriginPosY = (-m_fY + WINCY * 0.5f);
						return;
					}
				}
			}
		}

	}
}

void CHelmet::Change_Equip()
{
	if (m_EquipState == EquipState_Equip_HELMET)
		return;

	CInventory_UI* pInven = static_cast<CInventory_UI*>(Get_GameObject(L"Layer_UI", L"InventoryUI"));
	pInven->Set_CurrentEquipHelmet(this);

	CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	pPlayer->EquipItem_Add_Stat(0,0,0,0,0,100);	//방어력 올림
	m_bEquip = true;
}


void CHelmet::Set_MouseToInventory()
{
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_vec3		vPoint;
	m_fX = (_float)ptMouse.x;
	m_fY = (_float)ptMouse.y;

	m_pTransCom->Set_Pos(m_fX - WINCX * 0.5f,
		(-m_fY + WINCY * 0.5f), 0.f);
}

void CHelmet::PickingMouseUp()
{
	if (m_bisPicking &&::Mouse_Up(DIM_LB) && !m_bPickingEnd) // 마우스 들었을때
	{
		m_bPickingEnd = true;
	}

	if (m_bPickingEnd == true) // 나중에 장비창에 있는지 슬롯창에있는지 확인하는 조건 필요
	{
		CInventory_UI* pInven = static_cast<CInventory_UI*>(Get_GameObject(L"Layer_UI", L"InventoryUI"));
		SearchInventorySlot(&pInven);

		if (!m_iMouseUpEnd)
		{
			RECT Rc{};
			if (m_EquipState == EquipState_Equip_HELMET)
			{
				memcpy(&Rc, &pInven->Get_EquipSlot()[1].rcInvenSlot, sizeof(RECT));
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

void CHelmet::SearchInventorySlot(CInventory_UI ** pInven)
{
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_vec3		vPoint;
	m_fX = (_float)ptMouse.x;
	m_fY = (_float)ptMouse.y;

	RECT rcMouse = { (LONG)(ptMouse.x - 30.f) ,(LONG)(ptMouse.y - 35.f) ,(LONG)(ptMouse.x + 30.f) ,(LONG)(ptMouse.y + 30.f) };
	RECT Rc{};

	if (m_EquipState != EquipState_Equip_HELMET)
	{
		memcpy(&Rc, &(*pInven)->Get_EquipSlot()[1].rcInvenSlot, sizeof(RECT));
		if (PtInRect(&Rc, ptMouse))
		{
			Change_Equip();
			m_iMouseUpEnd = true;

			if ((*pInven)->Get_CurrentEquipHelmet() == nullptr)
			{
				Change_Equip();
				m_EquipState = EquipState_Equip_HELMET;

				RECT Rc{};
				memcpy(&Rc, &(*pInven)->Get_EquipSlot()[1].rcInvenSlot, sizeof(RECT));
				m_fX = (Rc.left + Rc.right) / 2.f;
				m_fY = (Rc.top + Rc.bottom) / 2.f;

				m_pTransCom->Set_Pos(m_fX - WINCX * 0.5f, (-m_fY + WINCY * 0.5f), 0.f);

				m_fOriginPosX = m_fX;
				m_fOriginPosX = m_fY;
			}
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

_bool CHelmet::EquipIconPicking()
{
	CInventory_UI* pInven = static_cast<CInventory_UI*>(Get_GameObject(L"Layer_UI", L"InventoryUI"));

	const _uint iCurrentPickingID = pInven->Get_Current_Picking_ItemID();

	if (!(iCurrentPickingID == ID_HELMET1 || iCurrentPickingID == 0))
		return false;

	POINT		ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	RECT		rcUI2 = {
		(LONG)(m_fX - m_fSizeX * 0.5f) ,
		(LONG)(m_fY - m_fSizeY * 0.5f) ,
		(LONG)(m_fX + m_fSizeX * 0.5f) ,
		(LONG)(m_fY + m_fSizeY * 0.5f) };

	if (Get_DIMouseState(DIM_LB) & 0x80)
	{
		//cout << ptMouse.x << " " << ptMouse.y << endl;
		if (PtInRect(&rcUI2, ptMouse))
		{
			pInven->Set__Current_Picking_ItemID(ID_HELMET1);
			m_bPickingEnd = false;
			m_bisPicking = true;
			return true;
		}
		m_bisPicking = false;
		return false;
	}

	return false;
}

void CHelmet::Shop_Goods()
{
	CInventory_UI* pInven = static_cast<CInventory_UI*>(Get_GameObject(L"Layer_UI", L"InventoryUI"));
	pInven->Get_WeaponType()->push_back(this);
	m_EquipState = EquipState_Slot;
	m_fX = WINCX * 0.5f;
	m_fY = WINCY * 0.5f;
	m_fSizeX = 100.f;
	m_fSizeY = 100.f;

	_vec3 vScale = { m_fSizeX, m_fSizeY, 1.f };
	m_pTransCom->Set_Scale(&vScale);

	RECT Rc{};
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			int iIndex = (i * 9) + j;
			memcpy(&Rc, &pInven->Get_InvenSlot()[iIndex].rcInvenSlot, sizeof(RECT));

			m_fX = (Rc.left + Rc.right) / 2.f;
			m_fY = (Rc.top + Rc.bottom) / 2.f;

			if (pInven->Get_InvenSlot()[iIndex].bSlotEmpty == false)
			{
				m_pTransCom->Set_Pos(m_fX - WINCX * 0.5f,
					(-m_fY + WINCY * 0.5f), 0.0f);

				pInven->Get_InvenSlot()[iIndex].bSlotEmpty = true;
				m_pTransCom->Update_Component(1.f);
				m_bOnce = true;
				m_bIsWorld = false;
				m_bIsInventory = true;
				m_fOriginPosX = m_fX - WINCX * 0.5f;
				m_fOriginPosY = (-m_fY + WINCY * 0.5f);
				return;
			}
		}
	}
}


HRESULT CHelmet::Add_Component(void)
{
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Helmet1Texture", m_mapComponent, ID_STATIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);

	return S_OK;
}

CHelmet * CHelmet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iX, _uint iZ)
{
	CHelmet* pInstance = new CHelmet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iX, iZ)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CHelmet::Free(void)
{
	CGameObject::Free();
}
