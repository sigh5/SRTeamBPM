#pragma once
#include "GameObject.h"
#include "Engine_Include.h"
#include "TestPlayer.h"

BEGIN(Engine)

class CTexture;
class CCalculator;
class COrthoTransform;
class CRcTex;
class CAnimation;

END

class CStatus_UI : public CGameObject
{
public:
	explicit CStatus_UI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStatus_UI(const CGameObject& rhs);
	virtual ~CStatus_UI();

public:
	HRESULT				Ready_Object(CGameObject* pPlayer);
	virtual _int		Update_Object(const _float& fTimeDelta) override;

	virtual	void		LateUpdate_Object(void);
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

public:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	COrthoTransform*	m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;

	CGameObject*		m_pPlayer;

private: // Status 각 요소에 쓰는 폰트
	wstring				m_szPower = L"";		  // Player's Bullet Power
	wstring				m_szSkillPower = L"";	  // Player's Acquired Skill's Power (if Library Open, Get Random Skill Book. Skill is a Click to DIK_LB)
	wstring				m_szSPeed = L"";		  // Player's Speed (Boots Item Acquired -> Speed Increase)
	wstring				m_szLuck = L"";			  // Item's Drop Percentage Increase
	wstring				m_szCritical = L"";		  // empty 
	wstring				m_szRange = L"";		  // Gun's Max Range 


public:
	static CStatus_UI*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pPlayer);
	virtual void			Free();


};

