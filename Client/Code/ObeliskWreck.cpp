#include "stdafx.h"
#include "..\Header\ObeliskWreck.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"

CObeliskWreck::CObeliskWreck(LPDIRECT3DDEVICE9 pGraphicDev)
	:CWreckBase(pGraphicDev)
{
}


CObeliskWreck::~CObeliskWreck()
{
}

HRESULT CObeliskWreck::Ready_Object(_vec3 vPos, _vec3 vDir)
{
	Add_Component();

	m_vDir = vDir;

	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Obelisk_Wreck_Texture", m_mapComponent, ID_STATIC);

	m_fLifeTime = 3.f;
	

	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pTransCom->Set_Scale(&_vec3(0.5f, 0.5f, 0.5f));

	m_iTextureNum = rand() % 3;
	return S_OK;
}

_int CObeliskWreck::Update_Object(const _float & fTimeDelta)
{
	_vec3 vDir, vUp, vResult;
	vDir = m_vDir * 0.3f;
	vUp = _vec3(0.f, -1.f, 0.f) * 0.4f;
	vResult = vDir + vUp;
	if (!(m_pTransCom->m_vScale.y * 0.5f > m_pTransCom->m_vInfo[INFO_POS].y))
		m_pTransCom->Move_Pos(&vResult);
	else
	{

		m_fLifeTimeCount += fTimeDelta;
		if (m_fLifeTimeCount > m_fLifeTime)
		{
			m_bDead = true;
			return OBJ_DEAD;
		}
	}
	Engine::CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CObeliskWreck::LateUpdate_Object(void)
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

void CObeliskWreck::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTextureCom->Set_Texture(m_iTextureNum);	// 텍스처 정보 세팅을 우선적으로 한다.

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CObeliskWreck * CObeliskWreck::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vRight)
{
	CObeliskWreck*	pInstance = new CObeliskWreck(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos, vRight)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CObeliskWreck::Free(void)
{
	CWreckBase::Free();
}
