#include "stdafx.h"
#include "..\Header\WrongPicFind.h"
#include "Export_Function.h"
#include "AbstractFactory.h"
#include "Coin.h"
#include "Player.h"
#include "CharacterInfo.h"
#include "Currect_Answer.h"


CWrongPicFind::CWrongPicFind(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
	m_rcFindWrong[0] = { _long(1100 - 50), _long(200 - 50), _long(1100 + 50), _long(200 + 50) };
	m_rcFindWrong[1] = { _long(730 - 50), _long(280 - 50), _long(730 + 50), _long(280 + 50) };
	m_rcFindWrong[2] = { _long(840 - 50), _long(966 - 50), _long(840 + 50), _long(966 + 50) };
}

CWrongPicFind::CWrongPicFind(const CUI_Base & rhs)
	: CUI_Base(rhs)
{
}

CWrongPicFind::~CWrongPicFind()
{
}

HRESULT CWrongPicFind::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_OrthoMatrix(WINCX, WINCY, (WINCX * 0.5f), (WINCY * 0.5f));

	_vec3	vScale = { m_fSizeX, m_fSizeY, 1.f };
	m_pTransCom->Set_Scale(&vScale);

	m_pTransCom->Set_Pos(m_fX- WINCX * 0.5f, (-m_fY + WINCY * 0.5f), 0.1f);
	//m_pTransCom->Update_Component(1.f);

	return S_OK;
}

_int CWrongPicFind::Update_Object(const _float & fTimeDelta)
{		
	Picking_WrongPoint();

	//if (m_bFinalSuccess)
	//{
	//	++iChange;

	//	if (iChange > 30)
	//	{
	//		CScene* pScene = ::Get_Scene();
	//
	//	}
	//}

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CWrongPicFind::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CWrongPicFind::Render_Obejct(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
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

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CWrongPicFind::Picking_WrongPoint(void)
{
	if (Get_DIMouseState(DIM_LB) & 0x80)
	{
		POINT	ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		_vec3	vPoint;
		m_fX = (_float)ptMouse.x;
		m_fY = (_float)ptMouse.y;
		
		cout << m_fX << ": o :" << m_fY << endl;

		RECT RcFind{};

		for (_uint i = 0; i < 3; ++i)
		{
			memcpy(&RcFind, &m_rcFindWrong[i], sizeof(RECT));

			if (PtInRect(&RcFind, ptMouse))
			{
				if (i == 0)
				{
					// Event

					//MSG_BOX("8bit");
					Engine::PlaySoundW(L"Currect_Mark.wav", SOUND_EFFECT, 1.f);				
					m_bSuccess0 = true;	
				}		

				if (i == 1)
				{
					// Event

					//MSG_BOX("상남자");
					Engine::PlaySoundW(L"Currect_Mark.wav", SOUND_EFFECT, 1.f);
					m_bSuccess1 = true;
				}

				if (i == 2)
				{
					//MSG_BOX("나만의 작은 침");
					Engine::PlaySoundW(L"Currect_Mark.wav", SOUND_EFFECT, 1.f);
					m_bSuccess2 = true;
				}				
			}

			if (m_bSuccess0 && m_bSuccess1 && m_bSuccess2)
			{
				CScene* pStageA = ::Get_SaveScene();
				CLayer* pLayer = pStageA->GetLayer(L"Layer_GameLogic");
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(pLayer->Get_GameObject(L"Player"));
				CCharacterInfo* pPlayerInfo = dynamic_cast<CCharacterInfo*>(pLayer->Get_Component(L"Player", L"Proto_CharacterInfoCom", ID_STATIC));

				pPlayerInfo->Get_InfoRef()._iCoin += 50;
				m_bFinalSuccess = true;
				m_bSuccess0 = false;
			}
		}
	
	}

}

HRESULT CWrongPicFind::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Chim_Texture", m_mapComponent, ID_STATIC);

	return S_OK;
}

HRESULT CWrongPicFind::Create_CurrectMark(_float fX, _float fY)
{
	CScene*			pScene = Engine::Get_Scene();
	CLayer*			pMyLayer = pScene->GetLayer(L"Ready_Layer_Environment");

	CGameObject*	pGameObject = nullptr;
	pGameObject = CCurrect_Answer::Create(m_pGraphicDev, fX, fY);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pMyLayer->Add_GameObject(L"Currect", pGameObject), E_FAIL);

	return S_OK;
}

CWrongPicFind * CWrongPicFind::Create(LPDIRECT3DDEVICE9 pGrhaphicDev)
{
	CWrongPicFind* pInstance = new CWrongPicFind(pGrhaphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CWrongPicFind::Free(void)
{
	CGameObject::Free();
}
