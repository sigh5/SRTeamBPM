#include "stdafx.h"
#include "..\Header\PetYeti.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"
#include "YetiTalk.h"
#include "Inventory_UI.h"
#include "EquipYeti.h"

CPetYeti::CPetYeti(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_vPlayerPos(0.f, 0.f,0.f)
	, m_vMonsterPos(0.f, 0.f, 0.f)
{
}


CPetYeti::~CPetYeti()
{
}

HRESULT CPetYeti::Ready_Object(float Posx, float Posz)
{
	
	m_pTransform = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Yeti_TextureCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pDanceTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Yeti_dance_TextureCom", m_mapComponent, ID_STATIC);
	m_pDanceAnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pDanceAnimationCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_LRAttack_AnimationCom", m_pDanceAnimationCom });

	m_pAnimationCom->Ready_Animation(8, 1, 0.2f);
	m_pDanceAnimationCom->Ready_Animation(1, 0, 0.2f);
	m_pTransform->Set_Scale(&_vec3(3.f, 3.f, 3.f));

	m_iState = 0;
	m_fDanceDelay = 1.5f;
	m_bDead = false;
	if (Posx == 0 && Posz == 0) {}
	else
	{
		m_pTransform->Set_Pos((_float)Posx, m_pTransform->m_vScale.y * 0.5f, (_float)Posz);
	}
	m_pTransform->Update_Component(1.f);
	return S_OK;
}

_int CPetYeti::Update_Object(const _float & fTimeDelta)
{
	CInventory_UI* pInven = static_cast<CInventory_UI*>(Get_GameObject(L"Layer_UI", L"InventoryUI"));
	if ( nullptr == dynamic_cast<CEquipYeti*>(pInven->Get_CurrentEquipPet()))
	{
		return 0;
	}

	Get_MonsterToPlayer_Distance(&fMtoPDistance);
	
	Dance_Judge(fTimeDelta);

	if (fMtoPDistance > 10.f)
	{
		m_iState = 0;
		m_pTransform->Chase_Target_notRot(&m_vPlayerPos, 6.f, fTimeDelta);

		m_pAnimationCom->Move_Animation(fTimeDelta);
	}
	else
	{
		if (m_bDance)
		{
			Dance(fTimeDelta);
		}
		else
		{
			m_iState = 0;
			m_pAnimationCom->m_iMotion = 0;
		}
	}


	m_pTransform->Update_Component(fTimeDelta);
	Engine::CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CPetYeti::LateUpdate_Object(void)
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
	D3DXMatrixScaling(&matScale, m_pTransform->m_vScale.x, m_pTransform->m_vScale.y, m_pTransform->m_vScale.z);

	_matrix      matRot;
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixRotationY(&matRot, (_float)pCamera->Get_BillBoardDir());

	_vec3 vPos;
	m_pTransform->Get_Info(INFO_POS, &vPos);

	D3DXMatrixTranslation(&matTrans,
		vPos.x,
		vPos.y,
		vPos.z);

	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale* matRot * matBill * matTrans;
	m_pTransform->Set_WorldMatrix(&(matWorld));

	// 빌보드 에러 해결
	CScene* pScene = ::Get_Scene();
	CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");


	Engine::CGameObject::LateUpdate_Object();
}

void CPetYeti::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	switch (m_iState)
	{
	case 0:
		m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);
		break;
	case 1:
		m_pDanceTextureCom->Set_Texture(m_pDanceAnimationCom->m_iMotion);
		break;
	}

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CPetYeti::Dance_Judge(const _float& fTimeDelta)
{
	if (m_bDance == false)
	{
		m_fDanceTimeCount += fTimeDelta;
		if (m_fDanceDelay <= m_fDanceTimeCount)
		{
			m_bDance = true;
			//m_iPetPattern = rand() % 2; //rand
			m_iPetPattern = 0;
			m_fDanceTimeCount = 0.f;
			m_bTalk = false;
		}
	}
}

void CPetYeti::Dance(const _float & fTimeDelta)
{
	m_iState = 1;
	CScene  *pScene = ::Get_Scene();
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	m_pDanceAnimationCom->Move_Animation(fTimeDelta);

	if (false == m_bTalk)
	{
		CGameObject* pTalk = nullptr;
		pTalk = CYetiTalk::Create(m_pGraphicDev, m_pTransform, m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].z);
		pLayer->Add_EffectList(pTalk);
		m_bTalk = true;
	}
	if (0 == m_pDanceAnimationCom->m_iMotion && m_bAnireset)
	{
		m_iDanceNum++;
		m_bAnireset = false;
	}
	if (m_pDanceAnimationCom->m_iMaxMotion == m_pDanceAnimationCom->m_iMotion)
	{
		m_bAnireset = true;
		if (4 == m_iDanceNum)
		{
			m_bAnireset = false;
			m_iDanceNum = 0;
			m_bDance = false;
		}
	}
}

void CPetYeti::Get_MonsterToPlayer_Distance(float * _Distance)
{
	if (SCENE_TOOLTEST != Get_Scene()->Get_SceneType())
	{
		CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
		NULL_CHECK(pPlayerTransformCom);

		pPlayerTransformCom->Get_Info(INFO_POS, &m_vPlayerPos);
		m_pTransform->Get_Info(INFO_POS, &m_vMonsterPos);

		float fMtoPDistance; // 몬스터와 플레이어 간의 거리

		fMtoPDistance = sqrtf((powf(m_vMonsterPos.x - m_vPlayerPos.x, 2) + powf(m_vMonsterPos.y - m_vPlayerPos.y, 2) + powf(m_vMonsterPos.z - m_vPlayerPos.z, 2)));

		memcpy(_Distance, &fMtoPDistance, sizeof(float));
	}
}

CPetYeti * CPetYeti::Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx, float Posz)
{
	CPetYeti*	pInstance = new CPetYeti(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(Posx, Posz)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPetYeti::Free(void)
{
	CGameObject::Free();
}
