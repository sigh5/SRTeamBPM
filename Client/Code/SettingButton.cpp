#include "stdafx.h"
#include "..\Header\SettingButton.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

CSettingButton::CSettingButton(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CSettingButton::~CSettingButton()
{
}

HRESULT CSettingButton::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3		vScale = { 0.28f, 0.18f, 0.f };

	m_pTransCom->Set_Scale(&vScale);

	return S_OK;
}

_int CSettingButton::Update_Object(const _float & fTimeDelta)
{	
	if (Engine::Mouse_Down(DIM_LB))
	{
		MouseCheck();
	}
		
	

	
	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CSettingButton::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CSettingButton::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	m_pTextureCom->Set_Texture(0);	

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	CGameObject::Render_Obejct();
}

_bool CSettingButton::MouseCheck(void)
{
	POINT ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_long lLeft, lTop, lRight, lBottom;

	lLeft = _long(((WINCX * 0.5f) * (1 + m_pTransCom->m_vInfo[INFO_POS].x)) - ((WINCX * m_pTransCom->m_vScale.x) * 0.5f));
	lRight = _long(((WINCX * 0.5f) * (1 + m_pTransCom->m_vInfo[INFO_POS].x)) + ((WINCX * m_pTransCom->m_vScale.x) * 0.5f));
	lTop = _long(((WINCY * 0.5f) * (1 - m_pTransCom->m_vInfo[INFO_POS].y)) - ((WINCY * m_pTransCom->m_vScale.y) * 0.5f));
	lBottom = _long(((WINCY * 0.5f) * (1 - m_pTransCom->m_vInfo[INFO_POS].y)) + ((WINCY * m_pTransCom->m_vScale.y) * 0.5f));

	RECT rcButton = { lLeft, lTop, lRight, lBottom };

	if (PtInRect(&rcButton, ptMouse))
	{
		m_bClick = true;
		return m_bClick;
	}

	return false;
}

HRESULT CSettingButton::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_CreditButtonTexture", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	
	return S_OK;
}

CSettingButton * CSettingButton::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY)
{
	CSettingButton* pInstance = new CSettingButton(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->m_pTransCom->Set_Pos(fX, fY, 0.f);

	return pInstance;
}

void CSettingButton::Free(void)
{
	Engine::CGameObject::Free();
}
