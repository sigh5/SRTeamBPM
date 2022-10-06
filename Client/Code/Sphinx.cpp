#include "stdafx.h"
#include "..\Header\Sphinx.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"
#include "Player.h"
#include "Gun_Screen.h"
#include "ObjectMgr.h"

CSphinx::CSphinx(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonsterBase(pGraphicDev)
{
}


CSphinx::~CSphinx()
{
}

HRESULT CSphinx::Ready_Object(int Posx, int Posy)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Sphinx_Texture", m_mapComponent, ID_STATIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);

	m_vOldPlayerPos = { 0.f, 0.f, 0.f };
	m_pAnimationCom->Ready_Animation(13, 0, 0.2f);
	m_pInfoCom->Ready_CharacterInfo(100, 10, 8.f);
	m_iPreHp = m_pInfoCom->Get_Hp();
	m_iShootLeftRight = 0;
	m_vScale = { 7.f, 7.f, 1.f };

	if (Posx == 0 && Posy == 0) {}
	else
	{
		m_pDynamicTransCom->Set_Pos((float)Posx, m_vScale.y, (float)Posy);
	}
	m_pDynamicTransCom->Compulsion_Update();
	return S_OK;
}

_int CSphinx::Update_Object(const _float & fTimeDelta)
{
	if (m_iPreHp != m_pInfoCom->Get_Hp())
	{
		m_bBattle = true;
	}
	if (m_bBattle)
	{
		Attack(fTimeDelta);
	}

	CMonsterBase::Get_MonsterToPlayer_Distance(&fMtoPDistance);

	Engine::CMonsterBase::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CSphinx::LateUpdate_Object(void)
{
	if (m_bHeadOff)
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
		D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);

		_matrix      matRot;
		D3DXMatrixIdentity(&matRot);
		D3DXMatrixRotationY(&matRot, (_float)pCamera->Get_BillBoardDir());

		_vec3 vPos;
		m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);

		D3DXMatrixTranslation(&matTrans,
			vPos.x,
			vPos.y,
			vPos.z);

		D3DXMatrixIdentity(&matWorld);
		matWorld = matScale* matRot * matBill * matTrans;
		m_pDynamicTransCom->Set_WorldMatrix(&(matWorld));
	}
	else
	{
		_matrix      matScale, matTrans;
		D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);

		_vec3 vPos;
		m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);

		D3DXMatrixTranslation(&matTrans,
			vPos.x,
			vPos.y,
			vPos.z);

		_matrix		matWorld;
		D3DXMatrixIdentity(&matWorld);

		matWorld = matScale * matTrans;
		m_pDynamicTransCom->Set_WorldMatrix(&(matWorld));

	}
}

void CSphinx::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pDynamicTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void		CSphinx::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	NULL_CHECK_RETURN(pLayer, );
	CGameObject *pGameObject = nullptr;
	pGameObject = static_cast<CGun_Screen*>(::Get_GameObject(L"Layer_UI", L"Gun"));


	if (static_cast<CGun_Screen*>(pGameObject)->Get_Shoot() &&
		fMtoPDistance < MAX_CROSSROAD &&
		m_pColliderCom->Check_Lay_InterSect(m_pBufferCom, m_pDynamicTransCom, g_hWnd))
	{

		static_cast<CPlayer*>(pGameObject)->Set_ComboCount(1);

		m_pInfoCom->Receive_Damage(1);
		cout << "Anubis" << m_pInfoCom->Get_InfoRef()._iHp << endl;
		static_cast<CGun_Screen*>(pGameObject)->Set_Shoot(false);
	}

}

void CSphinx::AttackJudge(const _float & fTimeDelta)
{
}

void CSphinx::Attack(const _float & fTimeDelta)
{
	m_pAnimationCom->Move_Animation(fTimeDelta);
	CTransform* pPlayerTransform = static_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));

	if (m_pAnimationCom->m_iMotion == 5)
	{
		if (0 == m_iShootLeftRight)
		{
			_vec3 vPosOrigin, vPos;
			m_pDynamicTransCom->Get_Info(INFO_POS, &vPosOrigin);

			vPos = vPosOrigin;
			vPos.y += 2.5f;
			vPos.x += 1.f;

			CScene* pScene = ::Get_Scene();
			CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");

			CGameObject* pGameObject = nullptr;
			pGameObject = CObjectMgr::GetInstance()->Reuse_SphinxBulletObj(m_pGraphicDev, vPos);
			NULL_CHECK_RETURN(pGameObject, );
			pMyLayer->Add_GameObjectList(pGameObject);
			++m_iShootLeftRight;
		}
		else if (1 == m_iShootLeftRight)
		{
			_vec3 vPosOrigin, vPos;
			m_pDynamicTransCom->Get_Info(INFO_POS, &vPosOrigin);

			vPos = vPosOrigin;
			vPos.y += 2.5f;
			vPos.x -= 1.f;

			CScene* pScene = ::Get_Scene();
			CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");

			CGameObject* pGameObject = nullptr;
			pGameObject = CObjectMgr::GetInstance()->Reuse_SphinxBulletObj(m_pGraphicDev, vPos);
			NULL_CHECK_RETURN(pGameObject, );
			pMyLayer->Add_GameObjectList(pGameObject);
			++m_iShootLeftRight;
		}

	}
	if (m_pAnimationCom->m_iMotion == 6)
	{
		m_iShootLeftRight = 0;
	}
	//if (m_pAnimationCom->m_iMotion == 8)
	//{
	//	if()
	//}
}

CSphinx * CSphinx::Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx, int Posy)
{
	CSphinx*	pInstance = new CSphinx(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(Posx, Posy)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSphinx::Free(void)
{
	CMonsterBase::Free();
}
