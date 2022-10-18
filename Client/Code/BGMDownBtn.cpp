#include "stdafx.h"
#include "..\Header\BGMDownBtn.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

CBGMDownBtn::CBGMDownBtn(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CBGMDownBtn::~CBGMDownBtn()
{
}

HRESULT CBGMDownBtn::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3		vScale = { 0.13f, 0.13f, 0.f };

	m_pTransCom->Set_Scale(&vScale);

	return S_OK;
}

_int CBGMDownBtn::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	if (Engine::Key_Down(DIK_K))
	{
		m_bClick = true;

		_vec3		vScale = { 0.19f, 0.19f, 0.f };

		m_pTransCom->Set_Scale(&vScale);
	}

	else
	{
		_vec3		vScale = { 0.13f, 0.13f, 0.f };

		m_pTransCom->Set_Scale(&vScale);
	}
	Engine::Key_InputReset();

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CBGMDownBtn::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CBGMDownBtn::Render_Obejct(void)
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

_bool CBGMDownBtn::MouseCheck(void)
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

	if (PtInRect(&rcButton, ptMouse))
	{
		m_bClick = true;

		_vec3		vScale = { 0.19f, 0.19f, 0.f };

		m_pTransCom->Set_Scale(&vScale);

		return m_bClick;
	}

	else
	{
		_vec3		vScale = { 0.13f, 0.13f, 0.f };

		m_pTransCom->Set_Scale(&vScale);

		return false;
	}
}

HRESULT CBGMDownBtn::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_BGM_Dn_BtnTexture", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);

	return S_OK;
}

CBGMDownBtn * CBGMDownBtn::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY)
{
	CBGMDownBtn* pInstance = new CBGMDownBtn(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->m_pTransCom->Set_Pos(fX, fY, 0.1f);

	return pInstance;
}

void CBGMDownBtn::Free(void)
{
	Engine::CGameObject::Free();
}
