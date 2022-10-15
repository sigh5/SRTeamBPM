#include "stdafx.h"
#include "..\Header\Obelisk.h"

#include "Export_Function.h"
#include "AbstractFactory.h"


#include "Player.h"
#include "Ghul.h"
#include "ObjectMgr.h"
#include "MyCamera.h"
#include "Gun_Screen.h"
#include "ObeliskSpawnEffect.h"

CObelisk::CObelisk(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonsterBase(pGraphicDev)
{

}


CObelisk::~CObelisk()
{
}

HRESULT CObelisk::Ready_Object(float Posx, float Posy)
{
	Add_Component();

	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);

	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Obelisk_Texture", m_mapComponent, ID_STATIC);

	m_iMonsterIndex = MONSTER_OBELISK;
	m_pAnimationCom->Ready_Animation(4, 0, 100);
	m_pInfoCom->Ready_CharacterInfo(4, 10, 5.f);
	m_iPreHp = (m_pInfoCom->Get_InfoRef()._iHp);

	m_fAttackDelay = 5.f;

	
	m_pDynamicTransCom->Set_Scale(&_vec3(2.f, 6.f, 2.f));
	if (Posx == 0 && Posy == 0) {}
	else
	{
		m_pDynamicTransCom->Set_Pos((float)Posx, m_pDynamicTransCom->m_vScale.y * 0.5f, (float)Posy);
	}
	m_pDynamicTransCom->Update_Component(1.f);
	Save_OriginPos();
	

	// ControlRoom
	_vec3 vPos, vScale;
	m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);
	vScale = m_pDynamicTransCom->Get_Scale();
	m_pColliderCom->Set_vCenter(&vPos, &vScale);
	// ~ControlRoom
	
	return S_OK;
}

_int CObelisk::Update_Object(const _float & fTimeDelta)
{
	m_pDynamicTransCom->Set_Y(m_pDynamicTransCom->m_vScale.y * 0.5f);

	// Cotrol Room
	_matrix matWorld;
	_vec3 vScale;
	vScale = m_pDynamicTransCom->Get_Scale();
	m_pDynamicTransCom->Get_WorldMatrix(&matWorld);
	m_pColliderCom->Set_HitBoxMatrix_With_Scale(&matWorld, vScale);
	// ~Cotrol Room
	
	CMonsterBase::Get_MonsterToPlayer_Distance(&fMtoPDistance);
	if (Distance_Over())
	{
		Engine::CMonsterBase::Update_Object(fTimeDelta);
		Add_RenderGroup(RENDER_ALPHA, this);

		return 0;
	}

	if (Dead_Judge(fTimeDelta))
	{
		return 0;
	}

	m_fTimeDelta = fTimeDelta;
	AttackJudge(fTimeDelta);

	NoHit_Loop(fTimeDelta);
	if (m_iPreHp != m_pInfoCom->Get_Hp())
	{
		Hit_Loop(fTimeDelta);
		m_iPreHp = m_pInfoCom->Get_Hp();
	}
	
	m_pDynamicTransCom->Update_Component(fTimeDelta);
	
	
	
	Engine::CMonsterBase::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CObelisk::LateUpdate_Object(void)
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
	D3DXMatrixScaling(&matScale, m_pDynamicTransCom->m_vScale.x, m_pDynamicTransCom->m_vScale.y, m_pDynamicTransCom->m_vScale.z);

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

		// 빌보드 에러 해결
	Engine::CMonsterBase::LateUpdate_Object();
}

void CObelisk::Render_Obejct(void)
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

void CObelisk::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	NULL_CHECK_RETURN(pLayer, );
	CGameObject *pGameObject = nullptr;
	pGameObject = static_cast<CGun_Screen*>(::Get_GameObject(L"Layer_UI", L"Gun"));


	if (static_cast<CGun_Screen*>(pGameObject)->Get_Shoot() &&
		fMtoPDistance < MAX_CROSSROAD  &&
		m_pColliderCom->Check_Lay_InterSect(m_pBufferCom, m_pDynamicTransCom, g_hWnd))
	{
		m_bHit = true;
		static_cast<CPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"))->Set_ComboCount(1);
		m_pInfoCom->Receive_Damage(1);
		cout << "Obelisk " << m_pInfoCom->Get_InfoRef()._iHp << endl;
		static_cast<CGun_Screen*>(pGameObject)->Set_Shoot(false);
	}
}

bool CObelisk::Dead_Judge(const _float & fTimeDelta)
{


	if (0 >= m_pInfoCom->Get_Hp())
	{
		if (false == m_bDead)
		{
			m_pAnimationCom->m_iMotion = 4;
			m_pDynamicTransCom->Add_Y(m_pDynamicTransCom->m_vInfo[INFO_POS].y *-0.35f);
			m_pDynamicTransCom->Set_Scale(&_vec3{ m_pDynamicTransCom->m_vScale.x, m_pDynamicTransCom->m_vScale.y * 0.7f, m_pDynamicTransCom->m_vScale.z });
			m_bDead = true;
		}
		//Safe_Release(m_pAttackAnimationCom);
	}
	if (m_bDead)
	{
		
		Engine::CMonsterBase::Update_Object(fTimeDelta);
		Add_RenderGroup(RENDER_ALPHA, this);
		return true;
	}
	else
	{
		return false;
	}
}

void CObelisk::AttackJudge(const _float & fTimeDelta)
{
	if (m_bAttack == false)
	{
		m_fAttackDelayTime += fTimeDelta;
		if (m_fAttackDelay <= m_fAttackDelayTime)
		{
			m_bAttack = true;
			m_fAttackDelayTime = 0.f;
		}
	}
}

void CObelisk::Attack(const _float & fTimeDelta)
{
	_vec3 vPos;
	m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);

	CScene* pScene = ::Get_Scene();
	CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");

	CGhul* pGameObject = nullptr;
	vPos.x += 1 + rand() % 4;
	vPos.z += 1 + rand() % 4;
	pGameObject = CGhul::Create(m_pGraphicDev, vPos.x, vPos.z);

	pMyLayer->Add_GhulList(pGameObject);

	CObeliskSpawnEffect*	pEffect = nullptr;
	pEffect = CObeliskSpawnEffect::Create(m_pGraphicDev, vPos);

	pMyLayer->Add_EffectList(pEffect);

	m_bAttack = false;
}

void CObelisk::NoHit_Loop(const _float & fTimeDelta)
{
	if (m_bAttack == true)
	{

		Attack(fTimeDelta);
	}
}


void CObelisk::Hit_Loop(const _float & fTimeDelta)
{
	_vec3 pScale;
	pScale = m_pDynamicTransCom->m_vScale;
	if (4 == m_pInfoCom->Get_Hp())
	{
		m_pAnimationCom->m_iMotion = 0;
	}
	if (3 == m_pInfoCom->Get_Hp())
	{
		m_pAnimationCom->m_iMotion = 1;
	}
	if (2 == m_pInfoCom->Get_Hp())
	{
		m_pAnimationCom->m_iMotion = 2;
	}
	if (1 == m_pInfoCom->Get_Hp())
	{
		m_pAnimationCom->m_iMotion = 3;
		m_pDynamicTransCom->Add_Y(m_pDynamicTransCom->m_vInfo[INFO_POS].y *- 0.5f);
		m_pDynamicTransCom->Set_Scale(&_vec3{ pScale.x, pScale.y * 0.55f, pScale.z });
	}
	m_pDynamicTransCom->Update_Component(fTimeDelta);
}

CObelisk * CObelisk::Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx, float Posy)
{
	CObelisk*	pInstance = new CObelisk(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(Posx, Posy)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CObelisk::Free(void)
{
	CMonsterBase::Free();
}
