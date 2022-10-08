#include "stdafx.h"
#include "..\Header\SphinxFlyHead.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"
#include "Player.h"
#include "Gun_Screen.h"

CSphinxFlyHead::CSphinxFlyHead(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonsterBase(pGraphicDev)
{
}


CSphinxFlyHead::~CSphinxFlyHead()
{
}

HRESULT CSphinxFlyHead::Ready_Object(float Posx, float Posy, float Size)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Sphinx_FlyHead_Texture", m_mapComponent, ID_STATIC);
	m_pActivatedTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Sphinx_Activated_Texture", m_mapComponent, ID_STATIC);
	m_pLRAttackTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Sphinx_lrattack_Texture", m_mapComponent, ID_STATIC);
	

	m_pHeadActivatedAnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pHeadActivatedAnimationCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Head_Active_AnimationCom", m_pHeadActivatedAnimationCom });
	
	m_pLRAttackAnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pLRAttackAnimationCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_LRAttack_AnimationCom", m_pLRAttackAnimationCom });

	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);

	m_pAnimationCom->Ready_Animation(7, 0, 100);
	
	m_pHeadActivatedAnimationCom->Ready_Animation(5, 0, 0.5f);
	m_pLRAttackAnimationCom->Ready_Animation(4, 0, 0.5f);
	m_pInfoCom->Ready_CharacterInfo(5, 10, 2.f);


	m_fAttackDelay = 0.3f;

	if (Posx == 0 && Posy == 0) {}
	else
	{
		m_pDynamicTransCom->Set_Pos(Posx, Size, Posy);
	}
	m_pDynamicTransCom->Set_Scale(&_vec3{ Size, Size, 1.f });
	m_pDynamicTransCom->Update_Component(1.f);
	return S_OK;
}

_int CSphinxFlyHead::Update_Object(const _float & fTimeDelta)
{
	if(false == m_bBattle)
	HeadActive(fTimeDelta);
	if (Dead_Judge(fTimeDelta))
	{
		return 0;
	}

	m_fTimeDelta = fTimeDelta;

	AttackJudge(fTimeDelta);
	CMonsterBase::Get_MonsterToPlayer_Distance(&fMtoPDistance);

	if(m_bBattle)
	BattleLoop(fTimeDelta);

	Engine::CMonsterBase::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CSphinxFlyHead::LateUpdate_Object(void)
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

	Engine::CMonsterBase::LateUpdate_Object();
}

void CSphinxFlyHead::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pDynamicTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (false == m_bBattle)
		m_pActivatedTextureCom->Set_Texture(m_pHeadActivatedAnimationCom->m_iMotion);
	else
		m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CSphinxFlyHead::Collision_Event()
{
}

void		CSphinxFlyHead::HeadActive(const _float& fTimeDelta)
{
	m_pHeadActivatedAnimationCom->Move_Animation(fTimeDelta);
	if (m_pHeadActivatedAnimationCom->m_iMaxMotion <= m_pHeadActivatedAnimationCom->m_iMotion)
	{
		m_bBattle = true;
	}
}

void CSphinxFlyHead::AttackJudge(const _float & fTimeDelta)
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

void CSphinxFlyHead::BattleLoop(const _float & fTimeDelta)
{
	if (m_bAttack)
	{
		Attack(fTimeDelta);
	}

}

void	CSphinxFlyHead::Attack(const _float& fTimeDelta)
{
	AttackLeftRight(fTimeDelta);
}

void		CSphinxFlyHead::AttackLeftRight(const _float& fTimeDelta)
{
	if (m_pLRAttackAnimationCom->m_iMinMotion == m_pLRAttackAnimationCom->m_iMotion)
	{

		m_pLRAttackAnimationCom->m_fMotionChangeCounter += fTimeDelta;

		
		if (false == m_bSelectedLeftRight)
		{
			m_bLRJudge = rand() % 2;
			m_bSelectedLeftRight = true;
		}
		if (m_pLRAttackAnimationCom->m_fIntervalMotion < m_pLRAttackAnimationCom->m_fMotionChangeCounter)
		{
			switch (m_bLRJudge)
			{
			case true:
				m_pLRAttackAnimationCom->m_iMotion = 1;
				break;

			case false:
				m_pLRAttackAnimationCom->m_iMotion = 3;
				break;
			}
		}
	}
	else
	{
		CCharacterInfo* pPlayerInfo = static_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC));
		if (false == m_bGet_PlayerPos)
		{
			m_vPrePlayerPosdir = m_vPlayerPos - m_pDynamicTransCom->m_vInfo[INFO_POS];
			D3DXVec3Normalize(&m_vPrePlayerPosdir, &m_vPrePlayerPosdir);
			m_bGet_PlayerPos = true;
		}
		else
		{
			_vec3 vCurPlayerPosdir;
			vCurPlayerPosdir = m_vPlayerPos - m_pDynamicTransCom->m_vInfo[INFO_POS];
			D3DXVec3Normalize(&vCurPlayerPosdir, &vCurPlayerPosdir);
			float fRadian = acos(D3DXVec3Dot(&m_vPrePlayerPosdir, &vCurPlayerPosdir));
			if (m_bLRJudge)//LR
			{

				if (0 < fRadian)
				{
					pPlayerInfo->Receive_Damage(m_pInfoCom->Get_AttackPower());
				}
			}
			else
			{

				if (0 > fRadian)
				{
					pPlayerInfo->Receive_Damage(m_pInfoCom->Get_AttackPower());
				}
			}
		}
	}
}

CSphinxFlyHead * CSphinxFlyHead::Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx, float Posy, float Size)
{
	CSphinxFlyHead*	pInstance = new CSphinxFlyHead(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(Posx, Posy, Size)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSphinxFlyHead::Free(void)
{
	CMonsterBase::Free();
}
