#include "stdafx.h"
#include "..\Header\BackButton.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

CBackButton::CBackButton(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CBackButton::~CBackButton()
{
}

HRESULT CBackButton::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3		vScale = { 0.18f, 0.18f, 0.f };

	m_pTransCom->Set_Scale(&vScale);

	return S_OK;
}

_int CBackButton::Update_Object(const _float & fTimeDelta)
{	
	if (Engine::Mouse_Down(DIM_LB))
	{
		MouseCheck();
	}

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CBackButton::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CBackButton::Render_Obejct(void)
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


	Engine::CGameObject::Render_Obejct();
}

_bool CBackButton::MouseCheck(void)
{
	POINT ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	
	_long lLeft, lTop, lRight, lBottom;

	lLeft = _long(((WINCX * 0.5f) * (1 + m_pTransCom->m_vInfo[INFO_POS].x)) - ((WINCX * m_pTransCom->m_vScale.x) * 0.5f));
	lRight = _long(((WINCX * 0.5f) * (1 + m_pTransCom->m_vInfo[INFO_POS].x)) + ((WINCX * m_pTransCom->m_vScale.x) * 0.5f));
	lTop = _long(((WINCY * 0.5f) * (1 - m_pTransCom->m_vInfo[INFO_POS].y)) - ((WINCY * m_pTransCom->m_vScale.y) * 0.5f));
	lBottom = _long(((WINCY * 0.5f) * (1 - m_pTransCom->m_vInfo[INFO_POS].y)) + ((WINCY * m_pTransCom->m_vScale.y) * 0.5f));

	 rcButton = { lLeft, lTop, lRight, lBottom };

	cout << "레 : " << rcButton.left << "탑 : " << rcButton.top << "라 : " << rcButton.right << "바 : " << rcButton.bottom << endl;
	cout << "---------------------------------------------------------" << endl;
	cout << "마엑 : " << ptMouse.x << "마와 : " << ptMouse.y << endl;
	if (PtInRect(&rcButton, ptMouse))
	{
		m_bClick = true;

		_vec3		vScale = { 0.25f, 0.25f, 0.f };

		m_pTransCom->Set_Scale(&vScale);

		return m_bClick;
	}

	else
	{
		_vec3		vScale = { 0.18f, 0.18f, 0.f };

		m_pTransCom->Set_Scale(&vScale);
		
		return false;
	}
}

HRESULT CBackButton::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_BackButtonTexture", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);

	return S_OK;
}

CBackButton * CBackButton::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY)
{
	CBackButton* pInstance = new CBackButton(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->m_pTransCom->Set_Pos(fX, fY, 0.1f);

	return pInstance;
}

void CBackButton::Free(void)
{
	Engine::CGameObject::Free();
}
