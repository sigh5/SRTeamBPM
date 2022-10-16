#include "stdafx.h"
#include "..\Header\BulletShell.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"
#include "Player.h"

CBulletShell::CBulletShell(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}


CBulletShell::~CBulletShell()
{
}

HRESULT CBulletShell::Ready_Object(_vec3 vPos, _vec3 vDir)
{
	Add_Component();

	m_vDir = vDir;

	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pTransCom->Set_Scale(&_vec3(0.5f, 0.5f, 0.5f));
	m_vUpDown = _vec3(0.f, 1.f, 0.f);
	return S_OK;
}

_int CBulletShell::Update_Object(const _float & fTimeDelta)
{
	_vec3 vRight, vUp, vResult;
	vRight = m_vDir * 0.4f;
	vUp = m_vUpDown * 0.5f;
	vResult = vRight + vUp;
	if (m_iDropGround < 3)
	m_pTransCom->Move_Pos(&vResult);
	if (m_pTransCom->m_vScale.y * 0.5f > m_pTransCom->m_vInfo[INFO_POS].y && m_iDropGround < 3)
	{
		m_vUpDown = -m_vUpDown * 0.5f;
		switch (m_iDropGround)
		{
		case 0:
			::StopSound(SOUND_OBJECT);
			::PlaySoundW(L"case_pistol_1.wav", SOUND_OBJECT, 0.5f);
			break;
		case 1:
			::StopSound(SOUND_OBJECT);
			::PlaySoundW(L"case_pistol_2.wav", SOUND_OBJECT, 0.5f);
			break;
		case 2:
			::StopSound(SOUND_OBJECT);
			::PlaySoundW(L"case_pistol_3.wav", SOUND_OBJECT, 0.5f);
			break;
		}
		++m_iDropGround;
	}
	m_vUpDown += _vec3(0.f, -0.1f, 0.f);
	Engine::CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CBulletShell::LateUpdate_Object(void)
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

void CBulletShell::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTextureCom->Set_Texture(0);	// 텍스처 정보 세팅을 우선적으로 한다.

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CBulletShell::Add_Component(void)
{
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_bullet_shell", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	
	return S_OK;
}

CBulletShell * CBulletShell::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vRight)
{
	CBulletShell *	pInstance = new CBulletShell(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos, vRight)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBulletShell::Free(void)
{
	CGameObject::Free();
}
