#include "stdafx.h"
#include "..\Header\ShopUI.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"
#include "Player.h"
#include "MiniStage1.h"
#include "Change_Stage.h"
#include "Helmet.h"

CShopUI::CShopUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
	
	m_rcShopSlot[0] = { 165 - 110, 544 - 296, 165 + 110, 544 + 296 };
	m_rcShopSlot[1] = { 428 - 116, 530 - 290, 428 + 116, 530 + 290 };
	m_rcShopSlot[2] = { 680 - 90,  380 - 120, 680 + 90,  380 + 120 };
	m_rcShopSlot[3] = { 910 - 90,  380 - 120, 910 + 90,  380 + 120 };
	m_rcShopSlot[4] = { 680 - 90,  680 - 120, 680 + 90,  680 + 120 };
	m_rcShopSlot[5] = { 910 - 90,  674 - 120, 910 + 90,  674 + 120 };
	
}

CShopUI::~CShopUI()
{
}

HRESULT CShopUI::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.f, 1.f);

	m_fX = WINCX * 0.5f;
	m_fY = WINCY * 0.5f;
	m_fSizeX = 1284.f;
	m_fSizeY = 1024.f;
	_vec3 vScale = { m_fSizeX, m_fSizeY, 1.f };
	m_pTransCom->Set_Scale(&vScale);

	m_pTransCom->Set_Pos(m_fX - WINCX * 0.5f,
		(-m_fY + WINCY * 0.5f), 0.0f);
	m_pTransCom->Update_Component(1.f);

	return S_OK;
}

_int CShopUI::Update_Object(const _float & fTimeDelta)
{
	if (m_bActvie)
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"));

		CCharacterInfo* pPlayerInfo = static_cast<CCharacterInfo*>(pPlayer->Get_Component(L"Proto_CharacterInfoCom", ID_STATIC));

		_int iCoin = pPlayerInfo->Get_InfoRef()._iCoin;
		pString = std::to_wstring(iCoin);

		Picking_Rect_Index();
		 	
		
	}
	
	Engine::CUI_Base::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_UI, this);

	if (m_iForceSceneReturn == SCENE_CHANGE_RETRURN)
		return SCENE_CHANGE_RETRURN;


	return 0;
}

void CShopUI::LateUpdate_Object(void)
{
	Engine::CUI_Base::LateUpdate_Object();
}

void CShopUI::Render_Obejct(void)
{	
	if (m_bActvie)
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
		Render_Font(L"LeeSoonSin", pString.c_str(), &_vec2(_float(WINCX / 2 + 300), 30.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		return;
	}
}

HRESULT CShopUI::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Shop_UI_Texture", m_mapComponent, ID_STATIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);

	return S_OK;

}

void CShopUI::Picking_Rect_Index()
{
	if (Get_DIMouseState(DIM_LB) & 0x80)
	{
		POINT		ptMouse{};

		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		_vec3		vPoint;
		m_fX = (_float)ptMouse.x;
		m_fY = (_float)ptMouse.y;

		RECT Rc{};
		for (int i = 0; i < 6; ++i)
		{
			memcpy(&Rc, &m_rcShopSlot[i], sizeof(RECT));

			if (PtInRect(&Rc, ptMouse))
			{
				if (i == 2)
				{
					CScene*pScene = ::Get_Scene();
					pScene->Set_SceneChane(true);
					::Set_SaveScene(pScene);

					CScene*		pChangeScene = CChange_Stage::Create(m_pGraphicDev,1);
					NULL_CHECK_RETURN(pScene, );

					::Change_Scene(pScene, pChangeScene);

					m_iForceSceneReturn = SCENE_CHANGE_RETRURN;
					return;

				}
				else if (i == 3)
				{
					_bool b = false;
				}
				else if (i == 4)
				{
					CScene*pScene = ::Get_Scene();
					pScene->Set_SceneChane(true);
					::Set_SaveScene(pScene);

					CScene*		pChangeScene = CChange_Stage::Create(m_pGraphicDev, 3);
					NULL_CHECK_RETURN(pScene, );

					::Change_Scene(pScene, pChangeScene);

					m_iForceSceneReturn = SCENE_CHANGE_RETRURN;
					return;
				}
				else if (i == 5 && !m_bSelect[5])
				{
					m_iEquipIndex = 5;
					m_bSelect[5] = true;
					return;
				}


			}
		}
	}
	
	if (m_bSelect[5] && !m_bShopingEnd[5])
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"));
		CCharacterInfo* pPlayerInfo = static_cast<CCharacterInfo*>(pPlayer->Get_Component(L"Proto_CharacterInfoCom", ID_STATIC));
		_int iCoin = pPlayerInfo->Get_InfoRef()._iCoin -=1;		// 나중에 가격적으면됌

		m_bShopingEnd[5] = true;

		CHelmet* pHelmet = static_cast<CHelmet*>(Get_GameObject(L"Layer_GameLogic", L"Helmet1"));
		pHelmet->Shop_Goods();


	}



}

CShopUI * CShopUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShopUI* pInstance = new CShopUI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CShopUI::Free()
{
	CUI_Base::Free();
}
