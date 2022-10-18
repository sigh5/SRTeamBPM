#include "stdafx.h"
#include "..\Header\Magnum.h"
#include "Export_Function.h"

#include "AbstractFactory.h"
#include "MyCamera.h"
#include "Inventory_UI.h"

CMagnum::CMagnum(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEquipmentBase(pGraphicDev)
{
}

CMagnum::~CMagnum()
{
}

HRESULT CMagnum::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Engine::CEquipmentBase::Ready_EquipInfo(5, 0, 0, 7, WEAPON_MAGNUM);
	
	m_RenderID = RENDER_ICON;

	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.f, 1.f);

	m_fSizeX = 100.f;
	m_fSizeY = 100.f;


	m_fX = (350 - 30 + 350 + 30) / 2.f;
	m_fY = (275 - 30 + 275 + 30) / 2.f;

	_vec3 vScale = { m_fSizeX, m_fSizeY, 1.f };
	m_pTransCom->Set_Scale(&vScale);

	m_pTransCom->Set_Pos(m_fX - WINCX * 0.5f,
		(-m_fY + WINCY * 0.5f), 0.0f);
	
	m_bIsInventory = true;
	m_bIsWorld = false;
	m_pTransCom->Update_Component(1.f);

	return S_OK;
}

_int CMagnum::Update_Object(const _float & fTimeDelta)
{
	m_fTimedelta = fTimeDelta;

	if (m_bIsWorld)
		m_RenderID = RENDER_ALPHA;
	else if (m_bIsInventory)
	{
		m_RenderID = RENDER_ICON;
		m_bIsPick = EquipIconPicking();
	}

	if (m_bIsPick)
	{
		Set_MouseToInventory();
	}

	_uint iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(m_RenderID, this);

	return iResult;
}

void CMagnum::LateUpdate_Object(void)
{
	if (m_bIsWorld)
	{
		CMyCamera* pCamera = static_cast<CMyCamera*>(Get_GameObject(L"Layer_Environment", L"CMyCamera"));
		NULL_CHECK(pCamera);

		_matrix		matWorld, matView, matBill;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixIdentity(&matBill);
		memcpy(&matBill, &matView, sizeof(_matrix));
		memset(&matBill._41, 0, sizeof(_vec3));
		D3DXMatrixInverse(&matBill, 0, &matBill);

		_matrix      matScale, matTrans, matRot;
		D3DXMatrixScaling(&matScale, 2.f, 2.f, 2.f);
		D3DXMatrixIdentity(&matRot);
		D3DXMatrixRotationY(&matRot, (_float)pCamera->Get_BillBoardDir());

		_vec3 vPos;
		m_pTransCom->Get_Info(INFO_POS, &vPos);
		D3DXMatrixTranslation(&matTrans,
			vPos.x,
			vPos.y,
			vPos.z);

		D3DXMatrixIdentity(&matWorld);
		matWorld = matScale* matRot * matBill * matTrans;
		m_pTransCom->Set_WorldMatrix(&(matWorld));
	}
	Engine::CGameObject::LateUpdate_Object();
}

void CMagnum::Render_Obejct(void)
{
	if (m_bIsWorld)
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
		return;
	}
	else if (m_bIsInventory)
	{
		CInventory_UI* pInven = static_cast<CInventory_UI*>(Get_GameObject(L"Layer_UI", L"InventoryUI"));

		if (!pInven->Get_InvenSwitch())
			return;

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

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		return;
	}
}

void CMagnum::Collision_Event()
{
	//CScene  *pScene = ::Get_Scene();
	//NULL_CHECK_RETURN(pScene, );
	//CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	//CGameObject *pGameObject = nullptr;

	//pGameObject = pLayer->Get_GameObject(L"Player");
	//NULL_CHECK_RETURN(pGameObject, );

	//if (m_pColliderCom->Check_Collision(this, pGameObject, 1, 1))
	//{
	//	if (Get_DIKeyState(DIK_F) & 0X80)
	//	{

	//	}
	//}
}

void CMagnum::Set_MouseToInventory()
{
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_vec3		vPoint;
	m_fX = ptMouse.x;
	m_fY = ptMouse.y;

	m_pTransCom->Set_Pos(m_fX - WINCX * 0.5f,
		(-m_fY + WINCY * 0.5f), 0.f);
}

_bool CMagnum::EquipIconPicking()
{
	POINT		ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	RECT		rcUI2 = {
		m_fX - m_fSizeX * 0.5f ,
		m_fY - m_fSizeY * 0.5f ,
		m_fX + m_fSizeX * 0.5f ,
		m_fY + m_fSizeY * 0.5f };

	if (Get_DIMouseState(DIM_LB) & 0x80)
	{
		cout << ptMouse.x << " " << ptMouse.y << endl;
		if (PtInRect(&rcUI2, ptMouse))
		{
			return true;
		}
		return false;
	}

	return false;
}

HRESULT CMagnum::Add_Component(void)
{
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_MagnumTexture", m_mapComponent, ID_STATIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);

	return S_OK;
}

CMagnum * CMagnum::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMagnum* pInstance = new CMagnum(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMagnum::Free(void)
{
	CGameObject::Free();
}
