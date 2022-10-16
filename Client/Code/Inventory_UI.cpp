#include "stdafx.h"
#include "..\Header\Inventory_UI.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "TestPlayer.h"
#include "ShotGun.h"
#include "Magnum.h"
#include "TestWeapon.h"

#include "MyCamera.h"
#include "Player.h"
#include "Player_Dead_UI.h"

USING(Engine)

CInventory_UI::CInventory_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
	
	ZeroMemory(&m_SlotType, sizeof(ItemSlot));
	
	for (_uint i = 0; i < m_iMaxColumn; ++i)
	{
		for (_uint j = 0; j < m_iMaxRow; ++j)
		{
			m_SlotType.pItem[i][j]= nullptr;
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

	// ���� �ʱⰪ
	m_fSlotX = 45.f; // ���� �� ĭ�� X(����) ũ��
	m_fSlotY = 65.f; // ���� �� ĭ�� Y(����) ũ��
	m_SlotType.ItemSlotSize[0][0] = {-210.5f, 23.5f}; // ù ��° ������ ���� ��ġ
	

	// ~���� �ʱⰪ

	Set_OrthoMatrix(512.f, 512.f, 0.f, 0.f);

	m_vecScale = { m_fSizeX, m_fSizeY, 1.f };

	m_pTransCom->Set_Scale(&m_vecScale);
	m_pTransCom->Set_Pos(m_fX , m_fY , 0.3f);
	
	return S_OK;
}

_int CInventory_UI::Update_Object(const _float & fTimeDelta)
{	
	// ���⸦ ȹ��� 

	if (m_bItemInfoAdd)
	{
		if (!m_vecWeaponType.empty())
		{
			m_SlotType.pItem[iIndexColumn][iIndexRow] = m_stackWeapon.top();

			if (m_SlotType.pItem[iIndexColumn][iIndexRow] != nullptr)
			{
				iIndexRow += 1;

				if (iIndexRow >= 9)
				{
					iIndexColumn += 1;
					iIndexRow = 0;
				}
				m_bItemInfoAdd = false;
				m_vecWeaponType.clear();
			}

		}
	}
		//cout << "���Ϳ� �����°� : " << m_vecWeaponType.size() << "���� top :" << m_stackWeapon.size() << endl;
	
	
	// i�� ����(4) , j�� ����(9) , ������ ���� ���� ���� ��ġ�� �˱� ���� ���� for��
	
	for (_uint i = 0; i < m_iMaxColumn; ++i)
	{
		for (_uint j = 0; j < m_iMaxRow; ++j)
		{		
			if (i != 0 && j == 0)
			{			
				m_SlotType.ItemSlotSize[i][j].y += (_float)(67 * i);
			}

			if (i == 0 && j != 0)
			{				
				m_SlotType.ItemSlotSize[i][j].x += (_float)(47 * j);
			}

			if (m_SlotType.pItem[i][j] != nullptr)
			{
				CTransform* pTransform = nullptr;

				pTransform = dynamic_cast<CTransform*>((m_SlotType.pItem[i][j]->Get_Component(L"Proto_TransformCom", ID_DYNAMIC)));
				
				pTransform->Set_Pos(m_SlotType.ItemSlotSize[i][j].x, m_SlotType.ItemSlotSize[i][j].y, 0.f);

				CShotGun* pShotgun = static_cast<CShotGun*>(Engine::Get_GameObject(L"Layer_GameLogic", L"ShotGun"));
											
				if (pShotgun->Get_bPicking() == true)
				{
					_vec3 vecShotPos;

					CTransform* pShotTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"ShotGun", L"Proto_TransformCom", ID_DYNAMIC));
					pShotTransform->Get_Info(INFO_POS, &vecShotPos);

					pTransform->Set_Pos(vecShotPos.x, vecShotPos.y, 0.f);
				}
				
			}

		}
	}
	

	/*if (m_SlotType.pItem[0][0] != nullptr)
	{
		CTransform* pTransform = nullptr;

		pTransform = dynamic_cast<CTransform*>((m_SlotType.pItem[0][0]->Get_Component(L"Proto_TransformCom", ID_DYNAMIC)));

		CShotGun* pShotgun = static_cast<CShotGun*>(Engine::Get_GameObject(L"Layer_GameLogic", L"ShotGun"));

		if(!pShotgun->Get_bPicking())
		pTransform->Set_Pos(m_SlotType.ItemSlotSize[0][0].x, m_SlotType.ItemSlotSize[0][0].y, 0.f);

		else
		{
			_vec3 vecShotPos;

			CTransform* pShotTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"ShotGun", L"Proto_TransformCom", ID_DYNAMIC));
			pShotTransform->Get_Info(INFO_POS, &vecShotPos);

			pTransform->Set_Pos(vecShotPos.x, vecShotPos.y, 0.f);
		}
			
	}*/

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);
	
	return 0;
}

void CInventory_UI::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CInventory_UI::Render_Obejct(void)
{
	CPlayer_Dead_UI* pDead_UI = static_cast<CPlayer_Dead_UI*>(Engine::Get_GameObject(L"Layer_UI", L"Dead_UI"));

	if (pDead_UI->Get_Render() == false)
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
		// �ؽ�ó ���� ������ �켱������ �Ѵ�.

		if (Key_Down(DIK_I))
		{
			m_bInvenSwitch = !m_bInvenSwitch;

			dynamic_cast<CMyCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"CMyCamera"))->Set_inventroyActive(m_bInvenSwitch);

			// ����
			if (dynamic_cast<CShotGun*>(Engine::Get_GameObject(L"Layer_GameLogic", L"ShotGun"))->Get_RenderFalse() == true)
			{
				dynamic_cast<CShotGun*>(Engine::Get_GameObject(L"Layer_GameLogic", L"ShotGun"))->Set_RenderControl(true);
			}

			// �׽�Ʈ�� ��
			if (dynamic_cast<CTestWeapon*>(Engine::Get_GameObject(L"Layer_GameLogic", L"TestWeapon"))->Get_RenderFalse() == true)
			{
				dynamic_cast<CTestWeapon*>(Engine::Get_GameObject(L"Layer_GameLogic", L"TestWeapon"))->Set_RenderControl(true);
			}


			// Magnum : �⺻ ���� ��
		//	dynamic_cast<CMagnum*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Magnum"))->Set_MagnumRender(true);
		//	dynamic_cast<CMagnum*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Magnum"))->Set_RenderControl(true);

		}
		
		::Key_InputReset();


		if (m_bInvenSwitch)
		{
			m_pTextureCom->Set_Texture(0);
			m_pBufferCom->Render_Buffer();
		}

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);
	}
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
