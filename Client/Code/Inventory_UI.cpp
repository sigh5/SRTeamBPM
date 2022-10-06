#include "stdafx.h"
#include "..\Header\Inventory_UI.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "TestPlayer.h"
#include "ShotGun.h"
#include "Magnum.h"

#include "Player.h"

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

	Set_OrthoMatrix(150.f, 150.f, 0.f, 0.f);

	m_vecScale = { m_fSizeX, m_fSizeY, 1.f };

	m_pTransCom->Set_Scale(&m_vecScale);
	m_pTransCom->Set_Pos(m_fX + 50.f, m_fY + WINCY * 0.2f, 0.f);

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
	
	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CInventory_UI::LateUpdate_Object(void)
{
	/*RECT		rcUI = { m_fX - m_fSizeX * 0.5f, m_fY - m_fSizeY * 0.5f, m_fX + m_fSizeX * 0.5f, m_fY + m_fSizeY * 0.5f };

	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_vec3	vecTest = { (_float)ptMouse.x, (_float)ptMouse.y, 1.f};

	if (PtInRect(&rcUI, ptMouse))
	{
	}*/

	CGameObject::LateUpdate_Object();
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

	if (Get_DIKeyState(DIK_I) & 0X80)
	{
		m_bInvenSwitch = true;

		dynamic_cast<CShotGun*>(Engine::Get_GameObject(L"Layer_GameLogic", L"ShotGun"))->Set_RenderControl(true);
	
		_uint iA = 0;
	}
	
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
	/*_matrix matOrtho;
	Get_ProjMatrix(&matOrtho);
	
	D3DXMatrixInverse(&matOrtho, nullptr, &matOrtho);*/ 

	//m_pGraphicDev->GetViewport()

	if (0 != m_vecWeaponType.size())
	{
		//WEAPON_TYPE weapon = dynamic_cast<CEquipmentBase*>(Engine::Get_GameObject(L"Layer_GameLogic", L"ShotGun"))->Get_EquipInfoRef().m_WeaponType;

		//vector<CEquipmentBase*>::iterator iter = find(m_vecWeaponType.begin(), m_vecWeaponType.end(), 1);

		//_uint iWeapon = distance(m_vecWeaponType.begin(), iter);

		for (_uint i = 0; i < 5; ++i)
		{
			_matrix matShotgun;
			D3DXMatrixIdentity(&matShotgun);

			matShotgun = *(dynamic_cast<CTransform*>(m_vecWeaponType[i]->Get_Component(L"Proto_TransformCom", ID_DYNAMIC))->Get_WorldMatrixPointer());
			// 인벤토리에 들어온 객체의 월드 행렬
		
			Get_ProjMatrix(&matShotgun);
			
			matShotgun._11 *= 1000.f;
			matShotgun._22 *= 1000.f;

			_matrix matTest;
			D3DXMatrixIdentity(&matTest);

			m_pTransCom->Get_WorldMatrix(&matTest);

			matShotgun._41 = matTest._41;
			matShotgun._42 = matTest._42;

			matShotgun._41 += 150.f;
			matShotgun._42 += 150.f;
			
			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matShotgun);
			_uint iA = 0;
		}	
			//	인벤토리에서 들어온 객체(샷건)의 불변수 컨트롤해서 렌더 상태 관리
	}

			// m_vecWeaponType[iWeapon]->Get_Component(L"Proto_TransformCom", ID_DYNAMIC);

		/*if (find(m_vecWeaponType.begin(), m_vecWeaponType.end(), (weapon == WEAPON_SHOTGUN)) != m_vecWeaponType.end())
		{

		}*/

	/*CTestPlayer* pPlayer = static_cast<CTestPlayer*>(Get_GameObject(L"Layer_GameLogic", L"TestPlayer"));

	CShotGun* pShotGun = static_cast<CShotGun*>(Get_GameObject(L"Layer_GameLogic", L"ShotGun"));

	if (0 != pPlayer->Get_WeaponType()->size())
	{
		if (find(pPlayer->Get_WeaponType()->begin(), pPlayer->Get_WeaponType()->end(), pShotGun) != pPlayer->Get_WeaponType()->end())
		{			
			vector<IDirect3DBaseTexture9*> vecShotTexture = static_cast<CTexture*>(Engine::Get_Component(L"Layer_GameLogic", L"ShotGun", L"Proto_ShotGunTexture", ID_STATIC))->Get_Texture();
									
			_uint iA = 0;	
		}
	}*/

}

HRESULT CInventory_UI::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<COrthoTransform>::Clone_Proto_Component(L"Proto_OrthoTransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Inventory_UI_Texture", m_mapComponent, ID_STATIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);

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
