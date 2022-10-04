#include "stdafx.h"
#include "..\Header\MetronomeUI.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "ObjectMgr.h"
#include "Player.h"

CMetronomeUI::CMetronomeUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev) 
{
	
}

CMetronomeUI::CMetronomeUI(const CGameObject & rhs)
	: CGameObject(rhs)
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
	m_pTransCom->Set_Scale(&_vec3(0.5f, 0.5f, 0.5f));
	
	
	m_fFrame += 1 * fTimeDelta;

	m_pTransCom->OrthoMatrix(50.f, 50.f, m_fMoveX, m_fMoveY, WINCX, WINCY);

	if ( m_fMoveX < 22.f && m_fMoveX >-22.f)
	{
		static_cast<CPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"))->Set_OneShot(true);
		static_cast<CPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"))->Set_SoundCheck(true);
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


	m_pTextureCom->Set_Texture(m_iTexIndex);

	m_pBufferCom->Render_Buffer();


	
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
