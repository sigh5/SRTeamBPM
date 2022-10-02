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

class CBullet_UI :
	public CGameObject
{
public:
	explicit CBullet_UI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBullet_UI(const CGameObject& rhs);
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

	CGameObject*		m_pPlayer;

private:
	wstring				m_szMagazine = L"";		   // 탄창 수 실시간 
	wstring				m_szOriginMagazine = L"";  // 원래 탄창 수를 표시하기 위한 함수


public:
	static CBullet_UI*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pPlayer);
	virtual void Free();
};

