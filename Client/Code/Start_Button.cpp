#include "stdafx.h"
#include "..\Header\Start_Button.h"
#include "Export_Function.h"

USING(Engine)

CStart_Button::CStart_Button(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CStart_Button::~CStart_Button()
{	
}

HRESULT CStart_Button::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3		vScale = { 0.28f, 0.15f, 0.f };

	m_pTransCom->Set_Scale(&vScale);

	return S_OK;
}

_int CStart_Button::Update_Object(const _float & fTimeDelta)
{	
	if (Engine::Mouse_Down(DIM_LB))
	{
		MouseCheck();
	}
			
	::MouseInputReset();

	CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CStart_Button::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CStart_Button::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	m_pTextureCom->Set_Texture(0);	// 텍스처 정보 세팅을 우선적으로 한다.

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	CGameObject::Render_Obejct();
}

_bool CStart_Button::MouseCheck(void)
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

	/*cout << "스레 : " << rcButton.left << "스탑 : " << rcButton.top << "스라 : " << rcButton.right << "스바 : " << rcButton.bottom << endl;
	cout << "---------------------------------------------------------" << endl;
	cout << "마엑 : " << ptMouse.x << "마와 : " << ptMouse.y << endl;*/

	if (PtInRect(&rcButton, ptMouse))
	{
		m_bClick = true;	
		return m_bClick;
	}

		return false;
	
}

HRESULT CStart_Button::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_ButtonTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ButtonTexture", pComponent });

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	return S_OK;
}

CStart_Button * CStart_Button::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY)
{
	CStart_Button* pInstance = new CStart_Button(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	pInstance->m_pTransCom->Set_Pos(fX, fY, 0.f);

	return pInstance;
}

void CStart_Button::Free(void)
{
	CGameObject::Free();
}
