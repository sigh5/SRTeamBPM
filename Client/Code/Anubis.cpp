#include "stdafx.h"
#include "..\Header\Anubis.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"
#include "HWPlayer.h"
#include "HitBlood.h"

CAnubis::CAnubis(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonsterBase(pGraphicDev)
{
}


CAnubis::~CAnubis()
{
}

HRESULT CAnubis::Ready_Object(int Posx, int Posy)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_fHitDelay = 0.f;


	CComponent* pComponent = nullptr;

	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_MonsterTexture", m_mapComponent, ID_DYNAMIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);

	m_iMonsterIndex = 0;
	_vec3	vScale = { 2.f,2.f,2.f };

	m_pDynamicTransCom->Set_Scale(&vScale);
	m_pDynamicTransCom->Set_Pos(20.f, 0.f, 20.f);

	m_pInfoCom->Ready_CharacterInfo(100, 10, 5.f);
	m_pAnimationCom->Ready_Animation(6, 1, 0.2f);
	m_iPreHp = (m_pInfoCom->Get_InfoRef()._iHp);
	if (Posx == 0 && Posy == 0) {}
	else
	{
		Set_TransformPositon(g_hWnd, m_pCalculatorCom);
	}

	return S_OK;
}

_int CAnubis::Update_Object(const _float & fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;

	if (m_iPreHp > m_pInfoCom->Get_Hp())
	{
		m_bHit = true;
		m_iPreHp = m_pInfoCom->Get_Hp();
		if (m_fHitDelay != 0)
		{
			m_fHitDelay = 0;
		}
	}

//#ifdef _DEBUG
	/*if (SCENE_TOOLTEST == Get_Scene()->Get_SceneType())
	{
		CTexture*	pComponent = nullptr;
		if (m_iPreIndex != m_iMonsterIndex)
		{
			m_iPreIndex = m_iMonsterIndex;
			switch (m_iMonsterIndex)
			{
			case 0:
				pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_MonsterTexture"));
				NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
				m_mapComponent[ID_STATIC].insert({ L"Proto_MonsterTexture", pComponent });
				break;

			case 1:
				pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_MonsterTexture2"));
				NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
				m_mapComponent[ID_STATIC].insert({ L"Proto_MonsterTexture2", pComponent });
				break;

			case 2:
				pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_MonsterTexture3"));
				NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
				m_mapComponent[ID_STATIC].insert({ L"Proto_MonsterTexture3", pComponent });
				break;
			}
		}
	}
	else*/
	{
		CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"TestPlayer", L"Proto_TransformCom", ID_DYNAMIC));
		NULL_CHECK(pPlayerTransformCom);

		////Set_OnTerrain();
		float TerrainY = m_pDynamicTransCom->Get_TerrainY1(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC, m_pCalculatorCom, m_pDynamicTransCom);
		m_pDynamicTransCom->Set_Y(TerrainY + 2.f);
		//지형에 올림

		_vec3		vPlayerPos, vMonsterPos;
		pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
		m_pDynamicTransCom->Get_Info(INFO_POS, &vMonsterPos);

		float fMtoPDistance; // 몬스터와 플레이어 간의 거리

		fMtoPDistance = sqrtf((powf(vMonsterPos.x - vPlayerPos.x, 2) + powf(vMonsterPos.y - vPlayerPos.y, 2) + powf(vMonsterPos.z - vPlayerPos.z, 2)));
		
		

		if (m_bHit == false)
		{
			if (fMtoPDistance > 5.f)
			{
				m_pDynamicTransCom->Chase_Target_notRot(&vPlayerPos, m_pInfoCom->Get_InfoRef()._fSpeed, fTimeDelta);

				m_pAnimationCom->Move_Animation(fTimeDelta);
			}
			else
			{
				m_pAnimationCom->m_iMotion = 0;
			}
		}
		else
		{
			//피격 시 피격모션
			m_pAnimationCom->m_iMotion = 7;
		}
	}
	
	if (m_bHit)
	{
		m_fHitDelay += fTimeDelta;
		if (m_fHitDelay > 1.5f)
		{
			m_bHit = false;
			m_fHitDelay = 0.f;
		}
	}




	_int iResult = Engine::CGameObject::Update_Object(fTimeDelta);



	return 0;
}

void CAnubis::LateUpdate_Object(void)
{
	// 빌보드 에러 해결
	CTransform*	pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"TestPlayer", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransform);

	CMyCamera* pCamera =static_cast<CMyCamera*>(Get_GameObject(L"Layer_Environment", L"StaticCamera"));
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
	D3DXMatrixRotationY(&matRot, pCamera->Get_BillBoardDir());

	_vec3 vPos;
	m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);

	D3DXMatrixTranslation(&matTrans,
		vPos.x,
		vPos.y,
		vPos.z);

	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale* matRot * matBill * matTrans;
	m_pDynamicTransCom->Set_WorldMatrix(&(matWorld));

	// 빌보드 에러 해결
	Engine::CGameObject::LateUpdate_Object();
}

void CAnubis::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pDynamicTransCom->Get_WorldMatrixPointer());
	
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

void CAnubis::Collision_Event(CGameObject * pGameObject)
{
	_vec3 PickPos;
	if (m_pColliderCom->Check_Lay_InterSect(m_pBufferCom,m_pDynamicTransCom,g_hWnd, PickPos))
	{
		m_pInfoCom->Receive_Damage(1.f);
		cout << m_pInfoCom->Get_InfoRef()._iHp << endl;
	}
	
		CGameObject* pHitBlood = CHitBlood::Create(m_pGraphicDev, PickPos);
		m_vecBlood.push_back(pHitBlood);
	}

	dynamic_cast<CHWPlayer*>(pGameObject)->m_bCheckShot = false;
}
void				CAnubis::Clear_Blood(const _float& fTimeDelta)
{
	for (auto iter = m_vecBlood.front(); iter != m_vecBlood.back();)
	{
		//if(static_cast<CHitBlood*>(&(*iter))->Get_Motion())
		if (2 == iter->Update_Object(fTimeDelta))
		{
		}
		else
		{
			++iter;
		}
	}
}

CAnubis * CAnubis::Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx, int Posy)
{
	CAnubis*	pInstance = new CAnubis(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(Posx, Posy)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CAnubis::Free(void)
{
	CGameObject::Free();
}
