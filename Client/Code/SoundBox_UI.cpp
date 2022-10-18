#include "stdafx.h"
#include "..\Header\SoundBox_UI.h"
#include "Export_Function.h"
#include "AbstractFactory.h"
#include "Setting_Stage.h"

#include "BGMUpBtn.h"
#include "BGMDownBtn.h"

CSoundBox_UI::CSoundBox_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
}

CSoundBox_UI::CSoundBox_UI(const CUI_Base & rhs)
	: CUI_Base(rhs)
{
}

CSoundBox_UI::~CSoundBox_UI()
{
}

HRESULT CSoundBox_UI::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3	vScale = { 0.2f, 0.2f, 0.f };

	m_pTransCom->Set_Scale(&vScale);

	// g_fSound = 1.f , Engine::SetChannelVolume() 함수의 2번째 인자에 쓰기 위함. 0.1f ~ 1f 사이로 설정
	m_iVolume = _uint(g_fSound * 10);

	m_iMaxVol = 100;
	m_iMinVol = 10;
		
	return S_OK;
}

_int CSoundBox_UI::Update_Object(const _float & fTimeDelta)
{
	CBGMUpBtn*		pBGMup = static_cast<CBGMUpBtn*>(Engine::Get_GameObject(L"Ready_Layer_Environment", L"BGM_Up"));
	CBGMDownBtn*	pBGMdown = static_cast<CBGMDownBtn*>(Engine::Get_GameObject(L"Ready_Layer_Environment", L"BGM_Dn"));

	// BGM_Up
	if (pBGMup->Get_Click())
		m_bPlusSound = true;

	if (m_bPlusSound) 
	{
		m_iVolume += 10;
		m_bPlusSound = false;

		if (m_iVolume >= 100)
			m_iVolume = m_iMaxVol;
	
	}
		pBGMup->Set_Click(false);

	// BGM_Down
	if (pBGMdown->Get_Click())
		m_bMinusSound = true;

	if (m_bMinusSound)
	{
		m_iVolume -= 10;
		m_bMinusSound = false;

		if (m_iVolume <= 10)
			m_iVolume = m_iMinVol;

	}
		pBGMdown->Set_Click(false);

	//cout << pBGMup->Get_Click() << endl;

	CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CSoundBox_UI::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CSoundBox_UI::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	_tchar	pStrSound[MAX_PATH];
	swprintf_s(pStrSound, L"%d", m_iVolume);
	m_szVolume = L"";
	m_szVolume += pStrSound;

	Render_Font(L"Ghanachocolate", m_szVolume.c_str(), &_vec2(300.f, 440.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

	m_pTextureCom->Set_Texture(0);	

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	CGameObject::Render_Obejct();

}

HRESULT CSoundBox_UI::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_SoundBox_Texture", m_mapComponent, ID_STATIC);

	return S_OK;
}

CSoundBox_UI * CSoundBox_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY)
{
	CSoundBox_UI* pInstance = new CSoundBox_UI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	pInstance->m_pTransCom->Set_Pos(fX, fY, 0.1f);

	return pInstance;
}

void CSoundBox_UI::Free(void)
{
	CGameObject::Free();
}
