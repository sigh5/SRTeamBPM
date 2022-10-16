#include "stdafx.h"
#include "..\Header\SphinxBullet.h"
#include "Export_Function.h"

#include "AbstractFactory.h"
#include "ObjectMgr.h"
#include "MyCamera.h"

CSphinxBullet::CSphinxBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBaseBullet(pGraphicDev)
{
}


CSphinxBullet::~CSphinxBullet()
{
}

HRESULT CSphinxBullet::Ready_Object(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pAnimationCom->Ready_Animation(3, 0, 0.03f);

	m_pDeadAnimationCom->Ready_Animation(9, 0, 0.1f);
	/*CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"TestPlayer", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, E_FAIL);
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);



	_vec3 vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_MoveDir = vPlayerPos - vPos;*/



	Set_MoveDir(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC, &vPos, MONSTER_BULLET, &_vec3(1.f, 1.f, 1.f));

	m_pTransCom->Set_Scale(&_vec3(1.f, 1.f, 1.f));
	m_pTransCom->Update_Component(1.f);
	return S_OK;
}

_int CSphinxBullet::Update_Object(const _float & fTimeDelta)
{
	_vec3 vPos = m_pTransCom->m_vInfo[INFO_POS];
	m_fFrame += fTimeDelta;

	if (m_fFrame > 3.f)
	{
		m_bHitPlayer = false;
		CObjectMgr::GetInstance()->Collect_SphinxBulletObj(this);
		m_fFrame = 0.f;
		return 5;
	}

	m_pAnimationCom->Move_Animation(fTimeDelta);


	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
	CCharacterInfo* pPlayerInfo = static_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC));
	_vec3 vPlayerPos = pPlayerTransformCom->m_vInfo[INFO_POS];

	float fDistance = sqrtf((powf(vPos.x - vPlayerPos.x, 2) + powf(vPos.y - vPlayerPos.y, 2) + powf(vPos.z - vPlayerPos.z, 2)));

	if (fDistance < 1.5f && false == m_bHitPlayer)
	{
		pPlayerInfo->Receive_Damage(10);
		m_bHitPlayer = true;
	}
	if (vPos.y <= 0.f)
	{
		m_pTransCom->Set_Scale(&_vec3(3.f, 3.f, 3.f));
		m_pDeadAnimationCom->Move_Animation(fTimeDelta);
		if (false == m_bExplosionSound)
		{
			::StopSound(SOUND_EXPLOSION);
			::PlaySoundW(L"explosion_2.wav", SOUND_EXPLOSION, 0.3f);
			m_bExplosionSound = true;
		}
		if (m_pDeadAnimationCom->m_iMotion == m_pDeadAnimationCom->m_iMaxMotion)
		{
			m_pDeadAnimationCom->m_iMotion = m_pDeadAnimationCom->m_iMinMotion;
			m_pTransCom->Set_Scale(&_vec3(1.f, 1.f, 1.f));
			m_bHitPlayer = false;
			m_bExplosionSound = false;
			CObjectMgr::GetInstance()->Collect_SphinxBulletObj(this);
			m_fFrame = 0.f;
			return 5;
		}
	}
	else
	{
		m_pTransCom->Move_Pos(&(m_MoveDir * 2.f * fTimeDelta));
	}

	Engine::CBaseBullet::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CSphinxBullet::LateUpdate_Object(void)
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

void CSphinxBullet::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	if (m_pTransCom->m_vInfo[INFO_POS].y <= 0.f)
	{
		m_pDeadTextureCom->Set_Texture(m_pDeadAnimationCom->m_iMotion);
	}
	else
	{
		m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);	// 텍스처 정보 세팅을 우선적으로 한다.
	}
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CSphinxBullet::Add_Component(void)
{
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Sphinx_Bullet_Texture", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_DynamicTransformCom", m_mapComponent, ID_DYNAMIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pDeadTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_explosion_sphinx", m_mapComponent, ID_STATIC);
	m_pDeadAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_Dead_AnimationCom", m_mapComponent, ID_STATIC);
	//m_pCubeTexCom = CAbstractFactory<CCubeTex>::Clone_Proto_Component(L"Proto_CubeTexCom", m_mapComponent, ID_STATIC);
	return S_OK;
}

CSphinxBullet * CSphinxBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CSphinxBullet *	pInstance = new CSphinxBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSphinxBullet::Free(void)
{
	Engine::CBaseBullet::Free();
}

