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
	HRESULT				Ready_Object(CTestPlayer* pPlayer);
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

	CTestPlayer*		m_pPlayer;

public:
	static CStatus_UI*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CTestPlayer* pPlayer);
	virtual void			Free();


};

