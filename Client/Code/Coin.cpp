#include "stdafx.h"
#include "..\Header\Coin.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "MyCamera.h"

#include "Player.h"

USING(Engine)

CCoin::CCoin(LPDIRECT3DDEVICE9 pGraphicDev)
	:CItemBase(pGraphicDev)
{
}

CCoin::~CCoin()
{
}

HRESULT CCoin::Ready_Object(_uint iX, _uint iY)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransCom->Set_Pos((_float)iX, 1.f, (_float)iY);
	
	m_pTransCom->Compulsion_Update();
	m_pAnimationCom->Ready_Animation(5, 0, 0.2f);

	return S_OK;
}

_int CCoin::Update_Object(const _float & fTimeDelta)
{

	if (m_bDead == true)
	{
		CScene  *pScene = ::Get_Scene();
		NULL_CHECK_RETURN(pScene, RETURN_ERR );
		CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
		NULL_CHECK_RETURN(pLayer, RETURN_ERR);
		return OBJ_DEAD;
	}

	_uint iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	m_pAnimationCom->Move_Animation(fTimeDelta);


	Add_RenderGroup(RENDER_ALPHA, this);

	return iResult;
}

void CCoin::LateUpdate_Object(void)
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
	D3DXMatrixScaling(&matScale, m_pTransCom->m_vScale.x, m_pTransCom->m_vScale.y, m_pTransCom->m_vScale.z);

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

	// 빌보드 에러 해결
	Engine::CGameObject::LateUpdate_Object();
}

void CCoin::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);	// 텍스처 정보 세팅을 우선적으로 한다.

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

void CCoin::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	NULL_CHECK_RETURN(pLayer, );
	CGameObject *pGameObject = nullptr;

	pGameObject = pLayer->Get_GameObject(L"Player");
	NULL_CHECK_RETURN(pGameObject, );
	CTransform *pTransform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Proto_DynamicTransformCom", ID_DYNAMIC));

	if (!m_bAddCoin && m_pColliderCom->Check_Collision(this, pGameObject , 1, 1))
	{
		Engine::PlaySoundW(L"Get_DropItem.mp3", SOUND_EFFECT, 1.f);
		m_bAddCoin = true;	
		CCharacterInfo *pCharInfo = dynamic_cast<CCharacterInfo*>(pGameObject->Get_Component(L"Proto_CharacterInfoCom", ID_STATIC));
		pCharInfo->Add_Coin();
		m_bDead = true;
	}
	

}

HRESULT CCoin::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_CoinTexture", m_mapComponent, ID_STATIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);

	return S_OK;
}

void CCoin::Set_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransCom->Set_Pos(vPos.x, fHeight, vPos.z);
}

CCoin * CCoin::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iX, _uint iY)
{
	CCoin* pInstance = new CCoin(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iX, iY)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCoin::Free(void)
{
	CItemBase::Free();
}
