#include "stdafx.h"
#include "..\Header\Magnum.h"
#include "Export_Function.h"

#include "AbstractFactory.h"
#include "Player.h"
#include "Inventory_UI.h"

// Player는 기본적으로 이걸 들고 있다.
USING(Engine)

CMagnum::CMagnum(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEquipmentBase(pGraphicDev)
{
	D3DXVec3Normalize(&m_vecOrtho, &m_vecOrtho);
}

CMagnum::~CMagnum()
{
}

HRESULT CMagnum::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	Engine::CEquipmentBase::Ready_EquipInfo(0, 0, 0, 7, WEAPON_MAGNUM);

	//dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_UI", L"InventoryUI", L"Proto_OrthoTransformCom", ID_DYNAMIC))->Get_Info(INFO_POS, &m_vecOrtho);
		
	//m_vecOrtho = { -211.f, 23.5f, 0.f };

	//m_pTransCom->Set_Pos(m_vecOrtho.x, m_vecOrtho.y, m_vecOrtho.z );  // 객체의 최초 위치

	m_fX = 100.f;
	m_fY = 100.f;

	m_fSizeX = 200.f;
	m_fSizeY = 200.f;

	return S_OK;
}

_int CMagnum::Update_Object(const _float & fTimeDelta)
{
	CInventory_UI* pInven = static_cast<CInventory_UI*>(Get_GameObject(L"Layer_UI", L"InventoryUI"));

	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	m_fX = ptMouse.x;
	m_fY = ptMouse.y;

	if (pInven->Get_InvenSwitch() == true && m_bRenderFalse == true)
	{
		if (Engine::Get_DIMouseState(DIM_LB) & 0X80)  // Picking
		{				
			Picking();			
		}
	}		

	Add_RenderGroup(RENDER_ICON, this);

	Engine::CGameObject::Update_Object(fTimeDelta);

	return 0;
}

void CMagnum::LateUpdate_Object(void)
{			
	CInventory_UI* pInven = static_cast<CInventory_UI*>(Get_GameObject(L"Layer_UI", L"InventoryUI"));
	
	m_pTransCom->Set_Pos(pInven->Get_ItemSlot()->ItemSlotSize[0][1].x, pInven->Get_ItemSlot()->ItemSlotSize[0][0].y, 0.f);

	Engine::CGameObject::LateUpdate_Object();
}

void CMagnum::Render_Obejct(void)
{
	CInventory_UI* pInven = static_cast<CInventory_UI*>(Get_GameObject(L"Layer_UI", L"InventoryUI"));

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
			
			_vec3		vecIconScale = { 70.f, 70.f, 0.f };

			m_pTransCom->Set_Scale(&vecIconScale);

			m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

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

			_vec3		vecIconScale = { 70.f, 70.f, 0.f };

			m_pTransCom->Set_Scale(&vecIconScale);

			m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

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

void CMagnum::Picking(void)
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

	RECT		rcUI = { m_fX - m_fSizeX * 1.5f, m_fY - m_fSizeY  *1.5f, m_fX + m_fSizeX * 1.5f, m_fY + m_fSizeY * 1.5f };

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

HRESULT CMagnum::Add_Component(void)
{
	m_pTransCom = CAbstractFactory<COrthoTransform>::Clone_Proto_Component(L"Proto_OrthoTransformCom", m_mapComponent, ID_DYNAMIC);
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
