#include "stdafx.h"
#include "..\Header\Inventory_UI.h"
#include "Export_Function.h"
#include "AbstractFactory.h"


#include "ShotGun.h"
#include "Magnum.h"

#include "MyCamera.h"
#include "Player.h"
#include "Player_Dead_UI.h"


USING(Engine)

CInventory_UI::CInventory_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
	ZeroMemory(&m_rcEquipSlot, sizeof(SlotInfo) * 4);
	ZeroMemory(&m_rcInvenSlot, sizeof(SlotInfo) * 36);

	m_rcEquipSlot[0].rcInvenSlot = { 350 - 30, 275 - 30 ,350 + 30, 275 + 30 };
	m_rcEquipSlot[0].bSlotEmpty = true;

	m_rcEquipSlot[1].rcInvenSlot = { 550 - 30, 275 - 30 ,550 + 30, 275 + 30 };
	m_rcEquipSlot[1].bSlotEmpty = false;

	m_rcEquipSlot[2].rcInvenSlot = { 350 - 30, 395 - 30 ,350 + 30, 395 + 30 };
	m_rcEquipSlot[2].bSlotEmpty = false;

	m_rcEquipSlot[3].rcInvenSlot = { 550 - 30, 395 - 30 ,550 + 30, 395 + 30 };
	m_rcEquipSlot[3].bSlotEmpty = false;

	float	fx = 350.f;
	float	fy = 485.f;
	// •û´Â°ª x = 30 y=35
	for (int i = 0; i < 4; ++i)
	{
		if (i != 0)
			fy += 80.f;
		for (int j = 0; j < 9; ++j)
		{
			int iIndex = (i * 9) + j;
			m_rcInvenSlot[iIndex].rcInvenSlot = { LONG((fx + (j*65.f)) - 30), LONG(fy - 35.f), LONG((fx + (j*65.f) + 30)), LONG(fy + 35.f) };
			m_rcInvenSlot[iIndex].bSlotEmpty = false;
		}
	}

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

	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.f, 1.f);

	m_fX = WINCX * 0.5f;
	m_fY = WINCY * 0.5f;
	m_fSizeX = 700.f;
	m_fSizeY = 600.f;

	_vec3 vScale = { m_fSizeX, m_fSizeY, 1.f };
	m_pTransCom->Set_Scale(&vScale);

	m_pTransCom->Set_Pos(m_fX - WINCX * 0.5f,
		(-m_fY + WINCY * 0.5f), 0.0f);
	m_pTransCom->Update_Component(1.f);

	return S_OK;
}

_int CInventory_UI::Update_Object(const _float & fTimeDelta)
{

	if (Key_Down(DIK_I))
	{
		m_bInvenSwitch = !m_bInvenSwitch;
		//Engine::PlaySoundW(L"Inventory_Open.mp3", SOUND_EFFECT, 1.f);
		dynamic_cast<CMyCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"CMyCamera"))->Set_inventroyActive(m_bInvenSwitch);
		dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"))->Set_inventroyActive(m_bInvenSwitch);
	}

	if (Key_Down(DIK_O))
	{
		m_pCurrentEquipPet = nullptr;
	}

	::Key_InputReset();

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CInventory_UI::LateUpdate_Object(void)
{


	Engine::CGameObject::LateUpdate_Object();

}

void CInventory_UI::Render_Obejct(void)
{

	CPlayer_Dead_UI* pDead_UI = static_cast<CPlayer_Dead_UI*>(Engine::Get_GameObject(L"Layer_UI", L"Dead_UI"));
	if (pDead_UI->Get_Render())
		return;

	if (m_bInvenSwitch)
	{
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
	}
}

void CInventory_UI::Find_Equip_Item(void)
{

}

void CInventory_UI::Set_CurrentEquipWeapon(CEquipmentBase * pWeapon)
{
	if (m_pCurrentEquipWeapon == nullptr)
	{
		m_pCurrentEquipWeapon = pWeapon;
		return;
	}
	_vec3 vPreWeaponvPos, vCurrentWeaponPos;


	CTransform* pRreWeaponTrans = static_cast<CTransform*>(m_pCurrentEquipWeapon->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
	pRreWeaponTrans->Get_Info(INFO_POS, &vPreWeaponvPos);

	CTransform* pCurrentvWeaponTrans = static_cast<CTransform*>(pWeapon->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));

	RECT Rc{};
	memcpy(&Rc, &m_rcInvenSlot[pWeapon->m_iInvenSlotIndex].rcInvenSlot, sizeof(RECT));
	float __fX = (Rc.left + Rc.right) / 2.f;
	float __fY = (Rc.top + Rc.bottom) / 2.f;

	vCurrentWeaponPos = { __fX,__fY,0.f };

	m_pCurrentEquipWeapon->Set_EquipState(EquipState_Slot);
	pWeapon->Set_EquipState(EquipState_Equip_Weapon);

	pRreWeaponTrans->Set_Pos(__fX - WINCX * 0.5f, (-__fY + WINCY * 0.5f), 0.f);

	pCurrentvWeaponTrans->Set_Pos(vPreWeaponvPos.x, vPreWeaponvPos.y, vPreWeaponvPos.z);
	m_pCurrentEquipWeapon->m_iInvenSlotIndex = pWeapon->m_iInvenSlotIndex;
	m_pCurrentEquipWeapon->Set_FX_FY(__fX, __fY);

	pRreWeaponTrans->Update_Component(1.f);
	pCurrentvWeaponTrans->Update_Component(1.f);

	m_pCurrentEquipWeapon = pWeapon;

}

void CInventory_UI::Set_CurrentEquipHelmet(CEquipmentBase * pHelmet)
{
	if (m_pCurrentEquipHelmet == nullptr)
	{
		m_pCurrentEquipHelmet = pHelmet;
		return;
	}
	_vec3 vPreWeaponvPos, vCurrentWeaponPos;


	CTransform* pRreWeaponTrans = static_cast<CTransform*>(m_pCurrentEquipHelmet->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
	pRreWeaponTrans->Get_Info(INFO_POS, &vPreWeaponvPos);

	CTransform* pCurrentvWeaponTrans = static_cast<CTransform*>(pHelmet->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));

	RECT Rc{};
	memcpy(&Rc, &m_rcInvenSlot[pHelmet->m_iInvenSlotIndex].rcInvenSlot, sizeof(RECT));
	float __fX = (Rc.left + Rc.right) / 2.f;
	float __fY = (Rc.top + Rc.bottom) / 2.f;

	vCurrentWeaponPos = { __fX,__fY,0.f };

	m_pCurrentEquipHelmet->Set_EquipState(EquipState_Slot);
	pHelmet->Set_EquipState(EquipState_Equip_HELMET);

	pRreWeaponTrans->Set_Pos(__fX - WINCX * 0.5f, (-__fY + WINCY * 0.5f), 0.f);

	pCurrentvWeaponTrans->Set_Pos(vPreWeaponvPos.x, vPreWeaponvPos.y, vPreWeaponvPos.z);
	m_pCurrentEquipHelmet->m_iInvenSlotIndex = pHelmet->m_iInvenSlotIndex;
	m_pCurrentEquipHelmet->Set_FX_FY(__fX, __fY);

	pRreWeaponTrans->Update_Component(1.f);
	pCurrentvWeaponTrans->Update_Component(1.f);

	m_pCurrentEquipHelmet = pHelmet;

}

void CInventory_UI::Set_CurrentEquipPet(CEquipmentBase * pPet)
{
	if (m_pCurrentEquipPet == nullptr)
	{
		m_pCurrentEquipPet = pPet;
		return;
	}
	_vec3 vPreWeaponvPos, vCurrentWeaponPos;


	CTransform* pRreWeaponTrans = static_cast<CTransform*>(m_pCurrentEquipPet->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
	pRreWeaponTrans->Get_Info(INFO_POS, &vPreWeaponvPos);

	CTransform* pCurrentvWeaponTrans = static_cast<CTransform*>(pPet->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));

	RECT Rc{};
	memcpy(&Rc, &m_rcInvenSlot[pPet->m_iInvenSlotIndex].rcInvenSlot, sizeof(RECT));
	float __fX = (Rc.left + Rc.right) / 2.f;
	float __fY = (Rc.top + Rc.bottom) / 2.f;

	vCurrentWeaponPos = { __fX,__fY,0.f };

	m_pCurrentEquipPet->Set_EquipState(EquipState_Slot);
	pPet->Set_EquipState(EquipState_Equip_Pet);

	pRreWeaponTrans->Set_Pos(__fX - WINCX * 0.5f, (-__fY + WINCY * 0.5f), 0.f);

	pCurrentvWeaponTrans->Set_Pos(vPreWeaponvPos.x, vPreWeaponvPos.y, vPreWeaponvPos.z);
	m_pCurrentEquipPet->m_iInvenSlotIndex = pPet->m_iInvenSlotIndex;
	m_pCurrentEquipPet->Set_FX_FY(__fX, __fY);

	pRreWeaponTrans->Update_Component(1.f);
	pCurrentvWeaponTrans->Update_Component(1.f);

	m_pCurrentEquipPet = pPet;
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
