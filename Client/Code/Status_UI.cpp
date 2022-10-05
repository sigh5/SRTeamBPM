#include "stdafx.h"
#include "..\Header\Status_UI.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "Player.h"
USING(Engine)


CStatus_UI::CStatus_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CStatus_UI::CStatus_UI(const CGameObject & rhs)
	: CGameObject(rhs)
{
}

CStatus_UI::~CStatus_UI()
{
}

HRESULT CStatus_UI::Ready_Object(CGameObject * pPlayer)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pPlayer = pPlayer;

	return S_OK;
}

_int CStatus_UI::Update_Object(const _float & fTimeDelta)
{
	// Tab 키 입력에 따라 Status_UI가 갖는 Transform의 vScale을 조절해주는 기능 구현할 것

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CStatus_UI::LateUpdate_Object(void)
{							// 1번, 2번 인자는 Scale X, Y. Default로는 WINCX, WINCY 밖에 이미지가 존재하도록 늘리고 Tab키를 누르면 줄어들어서 화면에 보이도록
	m_pTransCom->OrthoMatrix(1200.f, 400.f, 0.f, 0.f, WINCX, WINCY);

	if (Get_DIKeyState(DIK_TAB) & 0x80)
	{
		m_pTransCom->Set_OrthoScale(0.3f, 0.7f);
	}

	CGameObject::LateUpdate_Object();
}

void CStatus_UI::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_pTransCom->m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_pTransCom->m_matOrtho);

	if (Get_DIKeyState(DIK_TAB) & 0x80)
	{
		//m_pTransCom->Set_OrthoScale(0.5f, 0.5f);		
		
		_uint iPlayerPower = static_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"TestPlayer", L"Proto_CharacterInfoCom", ID_STATIC))->Get_InfoRef()._iAttackPower;

		_uint iPlayerSkillPower = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"TestPlayer"))->Get_Skill();

		_uint iPlayerSpeed = (_uint)static_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"TestPlayer", L"Proto_CharacterInfoCom", ID_STATIC))->Get_InfoRef()._fSpeed;


		// Player's Bullet Power
		_tchar	tBpower[MAX_PATH];
		swprintf_s(tBpower, L"%d", iPlayerPower);
		m_szPower = L"";
		m_szPower += tBpower;

		Render_Font(L"BMYEONSUNG", m_szPower.c_str(), &_vec2(574.f, 150.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		_tchar szBPname[128] = L"피해량";

		Render_Font(L"DalseoHealingBold", szBPname, &_vec2(565.f, 114.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		// ~Player's Bullet Power


		// Player's Skill Power
		_tchar	tSpower[MAX_PATH];
		swprintf_s(tSpower, L"%d", iPlayerSkillPower);
		m_szSkillPower = L"";
		m_szSkillPower += tSpower;

		Render_Font(L"BMYEONSUNG", m_szSkillPower.c_str(), &_vec2(585.f, 430.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		_tchar szSPname[128] = L"능력";

		Render_Font(L"DalseoHealingBold", szSPname, &_vec2(575.f, 394.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		// ~Player's Skill Power


		// Player's Speed
		_tchar	tSspeed[MAX_PATH];
		swprintf_s(tSspeed, L"%d", iPlayerSpeed);
		m_szSPeed = L"";
		m_szSPeed += tSspeed;

		Render_Font(L"BMYEONSUNG", m_szSPeed.c_str(), &_vec2(196.f, 146.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		_tchar szSPeed[128] = L"이동속도";

		Render_Font(L"DalseoHealingBold", szSPeed, &_vec2(165.f, 120.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		// ~Player's Speed


		// Player's Luck

		_tchar szLuck[128] = L"행운";

		Render_Font(L"DalseoHealingBold", szLuck, &_vec2(185.f, 394.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		// ~Player's Luck


		// Player's Gun Range

		_tchar szRange[128] = L"사거리";

		Render_Font(L"DalseoHealingBold", szRange, &_vec2(105.f, 250.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		// ~Player's Gun Range


		// Player's Critical

		_tchar szCritical[128] = L"크리티컬";

		Render_Font(L"DalseoHealingBold", szCritical, &_vec2(627.f, 260.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		// ~Player's Critical
	}
	
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
}

HRESULT CStatus_UI::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<COrthoTransform>::Clone_Proto_Component(L"Proto_OrthoTransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_StatusUI_Texture", m_mapComponent, ID_STATIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);

	return S_OK;
}

CStatus_UI * CStatus_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject * pPlayer)
{
	CStatus_UI*	pInstance = new CStatus_UI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pPlayer)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CStatus_UI::Free()
{
	CGameObject::Free();
}
