#include "stdafx.h"
#include "..\Header\MetronomeUI.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "Gun_Screen.h"
#include "MetronomeRect.h"


CMetronomeUI::CMetronomeUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev) 
{
	
}

CMetronomeUI::CMetronomeUI(const CGameObject & rhs)
	: CGameObject(rhs)
{

}

CMetronomeUI::~CMetronomeUI()
{
}

HRESULT CMetronomeUI::Ready_Object(_float fPosX, _float fPosY, _float fSpeed, int iTexIndex)
{
	m_fMoveX = fPosX;
	m_fMoveY = fPosY;
	m_fSpeed = fSpeed;
	m_iTexIndex = iTexIndex;


	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CMetronomeUI::Update_Object(const _float & fTimeDelta)
{

	CGameObject*		pGameObject = nullptr;
	CScene* pScene = Get_Scene();
	CLayer *pMyLayer = pScene->GetLayer(L"Layer_UI");

	//m_pTransCom->Set_Scale(&_vec3(2.f, 2.f, 2.f));
	
	if (m_fAlpha >= 245)
		m_fAlpha = 240;
	
	m_fFrame += 1 * fTimeDelta;
	m_fAlpha += 40*fTimeDelta;

	m_pTransCom->OrthoMatrix(100.f, 100.f, m_fMoveX, m_fMoveY, WINCX, WINCY);
	
	if (m_fMoveX < 5.f && m_fMoveX > -5.f)	// m_fMove.x < 10.f && m_fMove.x >-10.f
	{
		static_cast<CGun_Screen*>(Get_GameObject(L"Layer_UI", L"Gun"))
			->Set_ReadyShot(true);
		
	
		pGameObject = CObjectMgr::GetInstance()->Reuse_MetronomeRectUI(m_pGraphicDev,  4);
		NULL_CHECK_RETURN(pGameObject, 0);
		pMyLayer->Add_GameObjectList(pGameObject);

		pGameObject = CObjectMgr::GetInstance()->Reuse_MetronomeRectUI(m_pGraphicDev, 5);
		NULL_CHECK_RETURN(pGameObject, 0);
		pMyLayer->Add_GameObjectList(pGameObject);

		m_fAlpha = 0.f;
		// Create 큰사각형
		CObjectMgr::GetInstance()->Collect_UIObj(this);
		
		return 5; 
	}
	

	m_fMoveX += m_fSpeed * fTimeDelta;
	



	CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_UI, this);
	return 0;
}

void CMetronomeUI::LateUpdate_Object(void)
{


	CGameObject::LateUpdate_Object();
}

void CMetronomeUI::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_pTransCom->m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_pTransCom->m_matOrtho);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB((_int)m_fAlpha, 0, 0, 0));
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->Set_Texture(m_iTexIndex);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	
}

HRESULT CMetronomeUI::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_ArrowTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<COrthoTransform>::Clone_Proto_Component(L"Proto_OrthoTransformCom", m_mapComponent, ID_DYNAMIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);

	return S_OK;
}

CMetronomeUI * CMetronomeUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fPosX, _float fPosY, _float fSpeed, int iTexIndex)
{
	CMetronomeUI*	pInstance = new CMetronomeUI(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(fPosX, fPosY,fSpeed, iTexIndex)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMetronomeUI::Free()
{
	CGameObject::Free();
}
