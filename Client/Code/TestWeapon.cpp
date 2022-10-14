#include "stdafx.h"
#include "..\Header\TestWeapon.h"

#include "Export_Function.h"

CTestWeapon::CTestWeapon(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CTestWeapon::~CTestWeapon()
{
}

HRESULT CTestWeapon::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.f, 1.f);

	m_pTransCom->Set_Pos(100.f, 100.f, 0.f);  // 객체의 최초 위치

	m_fX = 100.f;
	m_fY = 100.f;
	
	m_fSizeX = 200.f;
	m_fSizeY = 200.f;
			
	_vec3		vecScale = { m_fSizeX, m_fSizeY, 1.f };

	m_pTransCom->Set_Scale(&vecScale);

	return S_OK;
}

_int CTestWeapon::Update_Object(const _float & fTimeDelta)
{
	//_vec3		vecScale = { m_fSizeX, m_fSizeY, 1.f };

	//m_pTransCom->Set_Scale(&vecScale);
	//m_pTransCom->Set_Pos(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f);

	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	m_fX = ptMouse.x;
	m_fY = ptMouse.y;
	//m_fY = m_pTransCom->m_vInfo[INFO_POS].y;

	if (Engine::Get_DIMouseState(DIM_LB) & 0x80)  // picking
	{
		Picking();
	}

	Add_RenderGroup(RENDER_UI, this);

	Engine::CGameObject::Update_Object(fTimeDelta);
	return 0;
}

void CTestWeapon::LateUpdate_Object(void)
{	
	Engine::CGameObject::LateUpdate_Object();
}

void CTestWeapon::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	
	_matrix		OldViewMatrix, OldProjMatrix;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);

	_matrix		ViewMatrix;

	ViewMatrix = *D3DXMatrixIdentity(&ViewMatrix);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
	m_pTextureCom->Set_Texture(0);	// 텍스처 정보 세팅을 우선적으로 한다.
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);
}

void CTestWeapon::Picking(void)
{
	_vec3		vecScale = { m_fSizeX, m_fSizeY, 1.f };

	m_pTransCom->Set_Scale(&vecScale);
	m_pTransCom->Set_Pos(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f);  // RECT 있는 곳으로?

	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	RECT		rcUI = { m_fX - m_fSizeX * 1.5f, m_fY - m_fSizeY  *1.5f, m_fX + m_fSizeX * 1.5f, m_fY + m_fSizeY * 1.5f };
	
	if (PtInRect(&rcUI, ptMouse))
	{
		_vec3		vPoint;
		
		vPoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
		vPoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
		vPoint.z = 0.1f;

		_matrix matOrtho;

		D3DXMatrixOrthoLH(&matOrtho, WINCX, WINCY, 0.f, 1.f);  // 직교 투영

		D3DXMatrixInverse(&matOrtho, nullptr, &matOrtho); // 뷰 스페이스(뷰가 항등이라 월드와 같다)
		D3DXVec3TransformCoord(&vPoint, &vPoint, &matOrtho);

		m_pTransCom->Set_Pos(vPoint.x, vPoint.y, 0.f);
				
	}
}

HRESULT CTestWeapon::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_MagnumTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_MagnumTexture", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });


	return S_OK;
}

CTestWeapon * CTestWeapon::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTestWeapon *	pInstance = new CTestWeapon(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTestWeapon::Free(void)
{
	CGameObject::Free();
}
