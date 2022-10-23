#include "stdafx.h"
#include "..\Header\ShopUI.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"
#include "Player.h"
#include "MiniStage1.h"
#include "Change_Stage.h"
#include "Helmet.h"
#include "EquipYeti.h"
#include "QuestProcessing_UI.h"
#include "ThunderHand.h"
#include "Skill_UI.h"
#include "ThunderPic.h"
#include "MiniGame1Pic.h"
#include "HelmetPic.h"
#include "PetPic.h"


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
		(-m_fY + WINCY * 0.5f), 0.08f);
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

	if (m_bBuyFalse == true || m_bBuySuccess == true)
		++m_iBFcount;

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

		if (m_bBuyFalse)
		{
			Render_Font(L"LeeSoonSin", L"잔액이 부족합니다.", &_vec2(_float(WINCX / 2 - 400), 30.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

			if (m_iBFcount > 30)
			{
				m_iBFcount = 0;
				m_bBuyFalse = false;
			}
		}

		if (m_bBuySuccess)
		{
			Render_Font(L"LeeSoonSin", L"Buy해줘서 아리가또!", &_vec2(_float(WINCX / 2 - 400), 30.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		
			if (m_iBFcount > 30)
			{
				m_iBFcount = 0;
				m_bBuySuccess = false;
			}
		}

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
				if (i == 0 && !m_bShopingEnd[0])
				{
					CCharacterInfo* pInfo = dynamic_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC));
					// 구매 제한
					if (pInfo->Get_InfoRef()._iCoin < 40)
					{
						Engine::PlaySoundW(L"Moneyless.mp3", SOUND_EFFECT2, 1.f);
						m_bBuyFalse = true;
						return;
					}

					else
					{
						Engine::PlaySoundW(L"Get_ShopItem.mp3", SOUND_EFFECT2, (g_fSound * 3.f));
						CThunderHand* pHand = dynamic_cast<CThunderHand*>(::Get_GameObject(L"Layer_UI", L"SkillHand"));
						pHand->Set_BuySkill(true);

						CSkill_UI* pSkill_UI = dynamic_cast<CSkill_UI*>(Engine::Get_GameObject(L"Layer_Icon", L"Skill_UI"));
						pSkill_UI->Set_m_bRB(true);

						CThunderPic* pThunderPic = dynamic_cast<CThunderPic*>(Engine::Get_GameObject(L"Layer_Icon", L"ThunderPic"));
						pThunderPic->Set_SoldOut(false);

						CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
						pPlayer->Set_Skill_Unlock(true);

						m_bBuySuccess = true;

						pInfo->Get_InfoRef()._iCoin -= 40;
					}					

					m_bShopingEnd[0] = true;
				}

				else if (i == 1)
				{
					CScene*pScene = ::Get_Scene();
					CLayer * pLayer = pScene->GetLayer(L"Layer_UI");
					NULL_CHECK_RETURN(pLayer, );
					/*pLayer = pScene->GetLayer(L"Layer_UI");*/


					CQuestProcessing_UI* pQuestProcessing_UI =
						dynamic_cast<CQuestProcessing_UI*>(pLayer->Get_GameObject(L"QuestProcessing_UI"));
					if (pQuestProcessing_UI != nullptr)
						pQuestProcessing_UI->Set_Quest_Claer(Quest_Index_TWO, true);
				}

				else if (i == 2)
				{
					CScene*pScene = ::Get_Scene();
					CLayer * pLayer = pScene->GetLayer(L"Layer_UI");
					CQuestProcessing_UI* pQuestProcessing_UI =
						dynamic_cast<CQuestProcessing_UI*>(pLayer->Get_GameObject(L"QuestProcessing_UI"));
				
					if (pQuestProcessing_UI == nullptr)
					{
						MSG_BOX("퀘스트를 받으세요!!");
						return;
					}
					
					pQuestProcessing_UI->Set_Quest_Claer(Quest_Index_ONE, true);

					

					
					pScene->Set_SceneChane(true);
					::Set_SaveScene(pScene);

					CScene*		pChangeScene = CChange_Stage::Create(m_pGraphicDev, 1);
					NULL_CHECK_RETURN(pScene, );

					::Change_Scene(pScene, pChangeScene);
					m_iForceSceneReturn = SCENE_CHANGE_RETRURN;

					 pLayer = pScene->GetLayer(L"Layer_GameLogic");
					NULL_CHECK_RETURN(pLayer, );
					
					m_bSelect[2] = true;

					return;	

				

				}
				else if (i == 3 && !m_bSelect[3])
				{
					CScene*pScene = ::Get_Scene();
					CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
					NULL_CHECK_RETURN(pLayer, );
					CPlayer* pPlayer = dynamic_cast<CPlayer*>(pLayer->Get_GameObject(L"Player"));
					CCharacterInfo* pPlayerInfo = dynamic_cast<CCharacterInfo*>(pLayer->Get_Component(L"Player", L"Proto_CharacterInfoCom", ID_STATIC));
					pPlayerInfo->Get_InfoRef()._iCoin += 50;

				
					pScene->Set_SceneChane(true);
					::Set_SaveScene(pScene);
					pLayer = pScene->GetLayer(L"Layer_UI");
					CQuestProcessing_UI* pQuestProcessing_UI =
						dynamic_cast<CQuestProcessing_UI*>(pLayer->Get_GameObject(L"QuestProcessing_UI"));
					if (pQuestProcessing_UI != nullptr)
						pQuestProcessing_UI->Set_Quest_Claer(Quest_Index_TWO, true);
					
					CScene*		pChangeScene = CChange_Stage::Create(m_pGraphicDev, 3);
					NULL_CHECK_RETURN(pScene, );

					::Change_Scene(pScene, pChangeScene);
					m_iForceSceneReturn = SCENE_CHANGE_RETRURN;

				

					m_bSelect[3] = true;
					return;				
				}
				/*else if (i == 4 && !m_bSelect[4] )
				{
					m_iEquipIndex = 4;
					m_bSelect[4] = true;
					return;
				}*/
				/*else if (i == 5 && !m_bSelect[5])
				{
					m_iEquipIndex = 5;
					m_bSelect[5] = true;
					return;
				}*/
				else if (i == 4 && !m_bShopingEnd[4])
				{
					CCharacterInfo* pInfo = dynamic_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC));
					// 구매 제한
					if (pInfo->Get_InfoRef()._iCoin < 40)
					{
						Engine::PlaySoundW(L"Moneyless.mp3", SOUND_EFFECT2, 1.f);
						m_bBuyFalse = true;
						return;				
					}

					else
					{
						Engine::PlaySoundW(L"Get_ShopItem.mp3", SOUND_EFFECT2, (g_fSound * 3.f));
						CHelmet* pHelmet = static_cast<CHelmet*>(Get_GameObject(L"Layer_GameLogic", L"Helmet1"));
						pHelmet->Shop_Goods();

						CHelmetPic* pHelmetPic = dynamic_cast<CHelmetPic*>(Engine::Get_GameObject(L"Layer_Icon", L"HelmetPic"));
						pHelmetPic->Set_SoldOut(false);

						m_bBuySuccess = true;

						pInfo->Get_InfoRef()._iCoin -= 40;
					}
						m_bShopingEnd[4] = true;
				}

				else if (i == 5 && !m_bShopingEnd[5])
				{
					CCharacterInfo* pInfo = dynamic_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC));
					// 구매 제한

					if (pInfo->Get_InfoRef()._iCoin < 20)
					{
						Engine::PlaySoundW(L"Moneyless.mp3", SOUND_EFFECT2, 1.f);
						m_bBuyFalse = true;
						return;
					}

					else
					{
						Engine::PlaySoundW(L"Get_ShopItem.mp3", SOUND_EFFECT2, (g_fSound * 3.f));
						CEquipYeti* pYeti = static_cast<CEquipYeti*>(Get_GameObject(L"Layer_GameLogic", L"EquipYeti"));
						pYeti->Shop_Goods();

						CPetPic* pPetPic = dynamic_cast<CPetPic*>(Engine::Get_GameObject(L"Layer_Icon", L"PetPic"));
						pPetPic->Set_SoldOut(false);

						m_bBuySuccess = true;

						pInfo->Get_InfoRef()._iCoin -= 20;
					}
					m_bShopingEnd[5] = true;					
				}

			}
		}
	}
	
	//if (m_bSelect[5] && !m_bShopingEnd[5])
	//{
	//	CPlayer* pPlayer = static_cast<CPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"));
	//	CCharacterInfo* pPlayerInfo = static_cast<CCharacterInfo*>(pPlayer->Get_Component(L"Proto_CharacterInfoCom", ID_STATIC));
	//	_int iCoin = pPlayerInfo->Get_InfoRef()._iCoin -=40;		// 나중에 가격적으면됌

	//
	//	m_bShopingEnd[5] = true;

	//	
	//	CEquipYeti* pYeti = static_cast<CEquipYeti*>(Get_GameObject(L"Layer_GameLogic", L"EquipYeti"));
	//	pYeti->Shop_Goods();		
	//	
	//}

	//if (m_bSelect[4] && !m_bShopingEnd[4])
	//{
	//	CPlayer* pPlayer = static_cast<CPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"));
	//	CCharacterInfo* pPlayerInfo = static_cast<CCharacterInfo*>(pPlayer->Get_Component(L"Proto_CharacterInfoCom", ID_STATIC));
	//	_int iCoin = pPlayerInfo->Get_InfoRef()._iCoin -= 40;		// 나중에 가격적으면됌

	//	m_bShopingEnd[4] = true;	

	//	CHelmet* pHelmet = static_cast<CHelmet*>(Get_GameObject(L"Layer_GameLogic", L"Helmet1"));
	//	pHelmet->Shop_Goods();
	
	//}

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
