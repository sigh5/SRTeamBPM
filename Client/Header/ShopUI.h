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
class CTransform;

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
	

public:
	void				Set_Active(_bool bEvent) { m_bActvie = bEvent; }
	const	_bool&		Get_Active()			 { return m_bActvie; }
	void	Set_ForceScene(_int iNum) { m_iForceSceneReturn = iNum; }
private:
	HRESULT				Add_Component(void);
	void				Picking_Rect_Index();

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;

private:
	_matrix				m_ProjMatrix;
	_bool				m_bActvie = false;
	_bool				m_bShopUISwitch = false;
	_vec3				m_vecScale;
	RECT				m_rcShopSlot[6];
	   wstring			pString = L"";
	_int				m_iRectIndex = 0;

	_int				m_iForceSceneReturn = 0;

	_int				m_iEquipIndex = 0;
	_int				m_bSelect[6] = { false, };
	_int				m_bShopingEnd[6] = { false, };

public:
	static CShopUI*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free();


};

