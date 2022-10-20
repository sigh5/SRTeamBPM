#include "stdafx.h"
#include "..\Header\Gacha_Machine.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "MyCamera.h"
#include "Player.h"

// 슬롯 돌려서 나올 수 있는 것
#include "Coin.h"
#include "Key.h"
#include "FatBat.h"

CGacha_Machine::CGacha_Machine(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CGacha_Machine::~CGacha_Machine()
{
}

HRESULT CGacha_Machine::Ready_Object(_uint iX, _uint iY)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransCom->Set_Pos((_float)iX, 1.f, (_float)iY);
	m_pTransCom->Compulsion_Update();
	m_pAnimationCom->Ready_Animation(2, 0, 0.2f, 2);

	return S_OK;
}

_int CGacha_Machine::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	
	// m_pAnimationCom-> 플레이어가 소유한 코인을 1개 받으면 애니메이션이 돌다.

	CDynamic_Transform* pPlayerTrans = dynamic_cast<CDynamic_Transform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
	
	_vec3	vecPlayerPos, vecCoinSlotPos;

	pPlayerTrans->Get_Info(INFO_POS, &vecPlayerPos);

	m_pTransCom->Get_Info(INFO_POS, &vecCoinSlotPos);

	_vec3	vecDir = vecPlayerPos - vecCoinSlotPos;

	_float	fDistance = D3DXVec3Length(&vecDir);

	if (fDistance <= 4.f)
		m_bTextRender = true;

	else
		m_bTextRender = false;


	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CGacha_Machine::LateUpdate_Object(void)
{
	CMyCamera* pCamera = static_cast<CMyCamera*>(Get_GameObject(L"Layer_Environment", L"CMyCamera"));
	NULL_CHECK(pCamera);
 
	_matrix		matWorld, matView, matBill;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);
	memcpy(&matBill, &matView, sizeof(_matrix));
	memset(&matBill._41, 0, sizeof(_vec3));
	D3DXMatrixInverse(&matBill, 0, &matBill);

	_matrix      matScale, matTrans;
	D3DXMatrixScaling(&matScale, 2.f, 2.f, 2.f);

	_matrix      matRot;
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
	

	Engine::CGameObject::LateUpdate_Object();
}

void CGacha_Machine::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (m_bTextRender)
	{
		_tchar	tCoinSlotEvent[MAX_PATH] = L"Press 'F' Button (-1 Coin)";
		
		Render_Font(L"LeeSoonSin", tCoinSlotEvent, &_vec2(_float(WINCX / 2 - 240), _float(WINCY / 2 + 200.f)), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}

	m_pTextureCom->Set_Texture(0);//m_pAnimationCom->m_iMotion);	// 텍스처 정보 세팅을 우선적으로 한다.

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CGacha_Machine::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	NULL_CHECK_RETURN(pLayer, );
	CGameObject *pGameObject = nullptr;

	pGameObject = pLayer->Get_GameObject(L"Player");
	NULL_CHECK_RETURN(pGameObject, );
	CTransform *pTransform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Proto_DynamicTransformCom", ID_DYNAMIC));

	if (m_pColliderCom->Check_Collision(this, pGameObject, 2, 2))
	{
		
		Open_Event(pGameObject);
	}
}

HRESULT CGacha_Machine::Open_Event(CGameObject * pPlayer)
{


	return S_OK;
}

HRESULT CGacha_Machine::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_CoinSlotTexture", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);

	return S_OK;
}

CGacha_Machine * CGacha_Machine::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iX, _uint iY)
{
	CGacha_Machine*	pInstance = new CGacha_Machine(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iX, iY)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CGacha_Machine::Free(void)
{
	Engine::CGameObject::Free();
}
