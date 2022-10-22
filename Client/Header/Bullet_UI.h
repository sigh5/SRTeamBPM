#pragma once
#include "UI_Base.h"

#include "Engine_Include.h"


BEGIN(Engine)

class CTexture;
class CCalculator;
class COrthoTransform;
class CRcTex;
class CAnimation;
END

class CGun_Screen;


class CBullet_UI :
	public CUI_Base
{
public:
	explicit CBullet_UI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBullet_UI(const CUI_Base& rhs);
	virtual ~CBullet_UI();

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

	CTexture*			m_pSG_TextureCom = nullptr;

private:
	wstring				m_szMagazine = L"";		   // 탄창 수 실시간
	wstring				m_szComboCount = L"";		// 실시간 콤보 카운트
	wstring				m_szOriginMagazine = L"";  // 원래 탄창 수를 표시하기 위한 함수

	_vec3				m_vecScale;

	_bool				m_bMagnum = false;
	_bool				m_bShotgun = false;

public:
	static CBullet_UI*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pPlayer);
	virtual void Free();
};

