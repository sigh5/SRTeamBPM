#include "stdafx.h"
#include "..\Header\Sphinx.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"
#include "Player.h"

#include "Gun_Screen.h"
#include "ObjectMgr.h"
#include "SphinxBody.h"
#include "SphinxFlyHead.h"
#include "Obelisk.h"
#include "HitEffect.h"

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
	m_pHeadOffTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Sphinx_HeadOff_Texture", m_mapComponent, ID_STATIC);
	m_pFlyHeadTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Sphinx_FlyHead_Texture", m_mapComponent, ID_STATIC);
	m_pBodyTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Sphinx_body_Texture", m_mapComponent, ID_STATIC);

	m_pHeadOffAnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pHeadOffAnimationCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Sphinx_HeadOff_AnimationCom", m_pHeadOffAnimationCom });

	m_iMonsterIndex = MONSTER_SPHINX;
	m_vOldPlayerPos = { 0.f, 0.f, 0.f };
	m_pAnimationCom->Ready_Animation(13, 0, 0.2f);
	m_pInfoCom->Ready_CharacterInfo(2, 10, 8.f);
	m_pHeadOffAnimationCom->Ready_Animation(19, 0, 0.3f);
	m_iPreHp = m_pInfoCom->Get_Hp();
	m_iShootLeftRight = 0;
	m_vScale = { 14.f, 14.f, 1.f };
	m_iShootCycle = 0;


	if (Posx == 0 && Posy == 0) {}
	else
	{

	}

	//m_pDynamicTransCom->Rotation(ROT_Y, 90.f);
	m_pDynamicTransCom->Update_Component(1.f);
	return S_OK;
}

_int CSphinx::Update_Object(const _float & fTimeDelta)
{
	CMonsterBase::Get_MonsterToPlayer_Distance(&fMtoPDistance);
	if (Distance_Over())
	{
		Engine::CMonsterBase::Update_Object(fTimeDelta);
		Add_RenderGroup(RENDER_ALPHA, this);

		return 0;
	}

	Get_ObeliskState();

	if (!m_bBattle)
	{
		IdleLoop(fTimeDelta);
	}
	if (m_bBattle)
	{
		BattleLoop(fTimeDelta);
	}


	Engine::CMonsterBase::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CSphinx::LateUpdate_Object(void)
{
	_matrix      matScale, matTrans, matRot;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);


	_vec3 vPos;
	m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);

	D3DXMatrixTranslation(&matTrans,
		vPos.x,
		m_vScale.y * 0.5f,
		vPos.z);

	D3DXMatrixIdentity(&matRot);
	D3DXMatrixRotationY(&matRot, (90.f*3.14f / 180.f));

	_matrix		matWorld;
	D3DXMatrixIdentity(&matWorld);

	matWorld = matScale * matRot * matTrans;
	m_pDynamicTransCom->Set_WorldMatrix(&(matWorld));
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

	if (m_bHeadOff_Finish)
	{
		m_pBodyTextureCom->Set_Texture(0);
	}
	else
	{
		if (false == m_bHeadOff)
		{
			m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);
		}
		else
		{
			m_pHeadOffTextureCom->Set_Texture(m_pHeadOffAnimationCom->m_iMotion);
		}
	}
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

	_vec3	vPos;
	m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);

	if (static_cast<CGun_Screen*>(pGameObject)->Get_Shoot() &&
		fMtoPDistance < MAX_CROSSROAD &&
		m_pColliderCom->Check_Lay_InterSect(m_pBufferCom, m_pDynamicTransCom, g_hWnd))
	{

		m_bHit = true;
		static_cast<CPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"))->Set_ComboCount(1);
		m_pInfoCom->Receive_Damage(1);
		cout << "Sphinx" << m_pInfoCom->Get_InfoRef()._iHp << endl;
		static_cast<CGun_Screen*>(pGameObject)->Set_Shoot(false);

		READY_CREATE_EFFECT_VECTOR(pGameObject, CHitEffect, pLayer, m_pGraphicDev, vPos);
		static_cast<CHitEffect*>(pGameObject)->Set_Effect_INFO(OWNER_SPHINX, 0, 1, 50000);
	}

}

void CSphinx::BattleLoop(const _float & fTimeDelta)
{
	if (m_iPreHp != m_pInfoCom->Get_Hp())
	{
		if (m_bUnbreakable)
		{
			m_pInfoCom->Add_Hp(m_iPreHp - m_pInfoCom->Get_Hp());
		}
		m_iPreHp = m_pInfoCom->Get_Hp();
	}
	HeadOff_Judge(fTimeDelta);
	if (false == m_bHeadOff)
	{
		Attack(fTimeDelta);
	}
	else
	{
		if (false == m_bHeadOff_Finish)
		{
			HeadOff_Animation(fTimeDelta);
		}
		else
		{

		}
	}
}

void CSphinx::IdleLoop(const _float & fTimeDelta)
{
	if (m_iPreHp != m_pInfoCom->Get_Hp())
	{
		if (m_bUnbreakable)
		{
			m_pInfoCom->Add_Hp(m_iPreHp - m_pInfoCom->Get_Hp());
		}
		m_iPreHp = m_pInfoCom->Get_Hp();

		m_bBattle = true;
	}

}

void CSphinx::AttackJudge(const _float & fTimeDelta)
{
}

void CSphinx::Attack(const _float & fTimeDelta)
{
	m_pAnimationCom->Move_Animation(fTimeDelta);
	CTransform* pPlayerTransform =
		static_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));

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
	if (m_pAnimationCom->m_iMotion == 8)
	{
		if (2 > m_iShootCycle)
		{
			m_pAnimationCom->m_iMotion = 5;
			++m_iShootCycle;
		}
	}
	if (m_pAnimationCom->m_iMotion == m_pAnimationCom->m_iMaxMotion)
	{
		m_iShootCycle = 0;
	}
}
void	CSphinx::HeadOff_Judge(const _float& fTimeDelta)
{
	if (1 >= m_pInfoCom->Get_Hp())
	{
		m_bHeadOff = true;

	}

}

void CSphinx::HeadOff_Animation(const _float& fTimeDelta)
{
	m_pHeadOffAnimationCom->Move_Animation(fTimeDelta);
	if (m_pHeadOffAnimationCom->m_iMotion >= m_pHeadOffAnimationCom->m_iMaxMotion)
	{
		_vec3 vPos;
		m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);
		//스핑크스 자리에 대가리를 남김
		//CGameObject* pBody = CSphinxBody::Create(m_pGraphicDev, vPos.x, vPos.z, m_vScale.y);
		CScene* pScene = ::Get_Scene();
		CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");

		//pMyLayer->Add_GameObject(L"Sphinx_Body", pBody);
		//m_bHeadOff_Finish = true;

		CGameObject*	pFlyHead = CSphinxFlyHead::Create(m_pGraphicDev, vPos.x, vPos.z, m_vScale.y);

		pMyLayer->Add_GameObject(L"Sphinx_FlyHead", pFlyHead);
		m_bHeadOff_Finish = true;
	}
}
void		CSphinx::Get_ObeliskState()
{
	CScene* pScene = ::Get_Scene();
	CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");

	m_iAliveObelisk = pMyLayer->Get_ObeliskList().size();

	for (auto iter = pMyLayer->Get_ObeliskList().begin(); iter != pMyLayer->Get_ObeliskList().end(); ++iter)
	{
		if (static_cast<CObelisk*>((*iter))->Get_Dead())
		{
			++m_iDeadObelisk;
		}
		if (static_cast<CObelisk*>((*iter))->Get_Hit())
		{
			m_bBattle = true;
		}

	}
	if (0 >= m_iAliveObelisk - m_iDeadObelisk)
	{
		m_bUnbreakable = false;
	}
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
