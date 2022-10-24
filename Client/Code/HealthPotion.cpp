#include "stdafx.h"
#include "..\Header\HealthPotion.h"
#include "Export_Function.h"

#include "AbstractFactory.h"
#include "MyCamera.h"
#include "HpBar.h"

USING(Engine)

CHealthPotion::CHealthPotion(LPDIRECT3DDEVICE9 pGraphicDev)
	: CItemBase(pGraphicDev)
{
}

CHealthPotion::~CHealthPotion()
{
}

HRESULT CHealthPotion::Ready_Object(_uint iX, _uint iY)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransCom->Set_Pos((_float)iX, 1.f, (_float)iY);
	m_pTransCom->Compulsion_Update();
	m_pAnimationCom->Ready_Animation(3, 0, 0.2f); 

	return S_OK;

	/*if (iX == 0 && iY == 0) {}
	else
	{		
		Engine::CItemBase::Set_TransformPos(g_hWnd, m_pCalculatorCom, L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", L"Proto_TransformCom", ID_STATIC, ID_DYNAMIC);
	}*/

	/*_vec3	vecScale = { 1.0f, 0.f, 1.0f };

	m_pTransCom->Set_Scale(&vecScale);*/
}

_int CHealthPotion::Update_Object(const _float & fTimeDelta)
{	
	if (m_bDead == true)
	{
		CScene  *pScene = ::Get_Scene();
		NULL_CHECK_RETURN(pScene, RETURN_ERR);
		CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
		NULL_CHECK_RETURN(pLayer, RETURN_ERR);
	

		return OBJ_DEAD;
	}


	_uint iResult = Engine::CGameObject::Update_Object(fTimeDelta);
	
	m_pAnimationCom->Move_Animation(fTimeDelta);

	//Set_OnTerrain();
	

	Add_RenderGroup(RENDER_ALPHA, this);

	return iResult;
}

void CHealthPotion::LateUpdate_Object(void)
{
	CMyCamera* pCamera = static_cast<CMyCamera*>(Get_GameObject(L"Layer_Environment", L"CMyCamera"));
	NULL_CHECK(pCamera);

	// 
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


	CItemBase::LateUpdate_Object();
}

void CHealthPotion::Render_Obejct(void)
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

void CHealthPotion::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	NULL_CHECK_RETURN(pLayer, );
	CGameObject *pGameObject = nullptr;

	pGameObject = pLayer->Get_GameObject(L"Player");
	NULL_CHECK_RETURN(pGameObject, );
	CTransform *pTransform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Proto_DynamicTransformCom", ID_DYNAMIC));

	if (m_pColliderCom->Check_Collision(this,pGameObject,1,1))
	{
		
		CCharacterInfo* pInfo   = dynamic_cast<CCharacterInfo*>(pGameObject->Get_Component(L"Proto_CharacterInfoCom", ID_STATIC));
		CAnimation* pHpBarAnimation = dynamic_cast<CAnimation*>(Engine::Get_Component(L"Layer_Icon", L"HpBar", L"Proto_AnimationCom", ID_DYNAMIC));

		if (pInfo->Get_InfoRef()._iHp == 200)
		{
			pHpBarAnimation->Eliminate_Motion(0);
			pHpBarAnimation->Add_Origin(0);
			m_bDead = false;
		}

		else if (pInfo->Get_InfoRef()._iHp >= 190)
		{
			Engine::PlaySoundW(L"Drink_Potion.mp3", SOUND_DROP, 1.f);
			pInfo->Add_Hp(10);
			m_bDead = true;

			pHpBarAnimation->Eliminate_Motion(1);
			pHpBarAnimation->Add_Origin(1);
		}
		
		else
		{
			Engine::PlaySoundW(L"Drink_Potion.mp3", SOUND_DROP, 1.f);
			pInfo->Add_Hp(20);
			m_bDead = true;

			pHpBarAnimation->Eliminate_Motion(2);
			pHpBarAnimation->Add_Origin(2);
		}				
	}
}

HRESULT CHealthPotion::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);	
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_HpPotionTexture", m_mapComponent, ID_STATIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);
	return S_OK;
}

void CHealthPotion::Set_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransCom->Set_Pos(vPos.x, fHeight, vPos.z);

}

CHealthPotion * CHealthPotion::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iX, _uint iY)
{
	CHealthPotion* pInstance = new CHealthPotion(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iX, iY)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CHealthPotion::Free(void)
{
	CItemBase::Free();
}
