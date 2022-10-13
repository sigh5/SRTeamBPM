#include "stdafx.h"
#include "..\Header\Inventory_UI.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "TestPlayer.h"
#include "ShotGun.h"
#include "Magnum.h"

#include "Player.h"
#include "MyCamera.h"


USING(Engine)

CInventory_UI::CInventory_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
}

CInventory_UI::CInventory_UI(const CUI_Base & rhs)
	: CUI_Base(rhs)
{
}

CInventory_UI::~CInventory_UI()
{
}

HRESULT CInventory_UI::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_OrthoMatrix(250.f, 320.f, 100.f, 100.f);

	m_vecScale = { m_fSizeX, m_fSizeY, 1.f };

	m_pTransCom->Set_Scale(&m_vecScale);
	m_pTransCom->Set_Pos(m_fX , m_fY , 0.3f);
	//m_pTransCom->Set_Pos(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.2f);
											// 120.f
	return S_OK;
}

_int CInventory_UI::Update_Object(const _float & fTimeDelta)
{
	/*if (0 != m_vecWeaponType.size())
	{
		for (_uint i = 0; i < 5; ++i)
		{
			
		}
	}*/
	//cout << m_vecWeaponType.size() << endl;

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);
	
	return 0;
}

void CInventory_UI::LateUpdate_Object(void)
{
	//RECT		rcUI = { m_fX - m_fSizeX * 0.5f, m_fY - m_fSizeY  *0.5f, m_fX + m_fSizeX * 3.5f, m_fY + m_fSizeY * 2.5f };//rcUI = { m_fX - m_fSizeX, m_fY - m_fSizeY, m_fX + m_fSizeX * 1.5f, m_fY + m_fSizeY * 1.5f };
	////RECT		rcUI = { m_fX - m_fSizeX * 1.5f, m_fY - m_fSizeY  *1.5f, m_fX + m_fSizeX * 1.5f, m_fY + m_fSizeY * 1.5f };
	//
	//POINT		ptMouse;
	//GetCursorPos(&ptMouse);
	//ScreenToClient(g_hWnd, &ptMouse);
	//
	//if (PtInRect(&rcUI, ptMouse) && (Get_DIMouseState(DIM_LB) & 0x80))
	//{
	//	//MSG_BOX("충돌");		
	//	m_pTransCom->Set_Pos((_float)ptMouse.x -(WINCX * 0.5f), (_float)ptMouse.y - (WINCY * 0.5f), 0.2f);
	//
	//	cout << "마우스 위치 : " << ptMouse.x << "//" << ptMouse.y << endl;
	//}
	
}

void CInventory_UI::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	_matrix		OldViewMatrix, OldProjMatrix;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);

	_matrix		ViewMatrix;

	ViewMatrix = *D3DXMatrixIdentity(&ViewMatrix);

	_matrix		matProj;

	Get_ProjMatrix(&matProj);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
		// 텍스처 정보 세팅을 우선적으로 한다.

	if (Key_Down(DIK_I) )
	{
		m_bInvenSwitch = !m_bInvenSwitch;
		
		dynamic_cast<CMyCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"CMyCamera"))->Set_inventroyActive(m_bInvenSwitch);
		dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"))->Set_inventroyActive(m_bInvenSwitch);

		if (dynamic_cast<CShotGun*>(Engine::Get_GameObject(L"Layer_GameLogic", L"ShotGun"))->Get_RenderFalse() == true)
		{
			dynamic_cast<CShotGun*>(Engine::Get_GameObject(L"Layer_GameLogic", L"ShotGun"))->Set_RenderControl(true);

			_uint iA = 0;
		}	
	}

	::Key_InputReset();


	if (m_bInvenSwitch)
	{
		m_pTextureCom->Set_Texture(0);
		m_pBufferCom->Render_Buffer();	
	//	Find_Equip_Item();
	}	

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);
}

void CInventory_UI::Find_Equip_Item(void)
{
		
}


HRESULT CInventory_UI::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<COrthoTransform>::Clone_Proto_Component(L"Proto_OrthoTransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Inventory_UI_Texture", m_mapComponent, ID_STATIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);

	return S_OK;
}

CInventory_UI * CInventory_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CInventory_UI* pInstance = new CInventory_UI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CInventory_UI::Free()
{
	CGameObject::Free();
}
