#include "stdafx.h"
#include "..\Header\MonsterHpBar.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"

CMonsterHpBar::CMonsterHpBar(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}


CMonsterHpBar::~CMonsterHpBar()
{
}

HRESULT CMonsterHpBar::Ready_Object(float Posx, float Posz, CTransform * pMaster, CCharacterInfo* pMasterInfo)
{
	m_pTransform = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_STATIC);

	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_HpBar_Texture", m_mapComponent, ID_STATIC);

	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);

	m_pAnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pAnimationCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_AnimationCom", m_pAnimationCom });

	m_pAnimationCom->Ready_Animation(10, 0, 5000.f);

	m_pMasterTransform = pMaster;

	m_pTransform->Set_Scale(&m_pMasterTransform->m_vScale);

	m_pTransform->Set_Pos(Posx, m_pMasterTransform->m_vInfo[INFO_POS].y * 1.3f, Posz);

	m_pMasterInfoCom = pMasterInfo;
	
	m_iMaxHp = m_pMasterInfoCom->Get_Hp();

	m_pTransform->Update_Component(1.f);

	return 0;
}

_int CMonsterHpBar::Update_Object(const _float & fTimeDelta)
{
	if (0 < m_pMasterInfoCom->Get_Hp())
	{
		m_iHpBarState = m_pMasterInfoCom->Get_Hp() / (m_iMaxHp * 0.1f);
	}
	else
	{
		m_iHpBarState = 0;
	}
	m_pAnimationCom->m_iMotion = abs(m_iHpBarState - 10);

	_vec3 vMasterPos;
	m_pMasterTransform->Get_Info(INFO_POS, &vMasterPos);

	m_pTransform->Set_Pos(vMasterPos.x, m_pMasterTransform->m_vScale.y * 1.2f, vMasterPos.z);

	m_pTransform->Update_Component(fTimeDelta);
	Engine::CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CMonsterHpBar::LateUpdate_Object(void)
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

void CMonsterHpBar::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);
	//애니메이션

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CMonsterHpBar * CMonsterHpBar::Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform * pMasterTrans, CCharacterInfo * pMasterInfo, float Posx, float Posz)
{
	CMonsterHpBar*	pInstance = new CMonsterHpBar(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(Posx, Posz, pMasterTrans, pMasterInfo)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMonsterHpBar::Free(void)
{
	CGameObject::Free();
}
