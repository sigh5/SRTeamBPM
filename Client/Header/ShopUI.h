#pragma once

#include "UI_Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class COrthoTransform;
class CCalculator;
class CAnimation;
class CCollider;
class CEquipmentBase;

END

class CShopUI : public CUI_Base
{
private:
	explicit CShopUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShopUI();


public:
	HRESULT				Ready_Object();
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual	void		LateUpdate_Object(void);
	virtual void		Render_Obejct(void) override;
	void				Find_Equip_Item(void);

private:
	HRESULT				Add_Component(void);

public:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	COrthoTransform*	m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;


public:
	static CShopUI*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free();


};

