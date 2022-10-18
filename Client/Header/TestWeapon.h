#pragma once
#include "EquipmentBase.h"
#include "Engine_Include.h"

	BEGIN(Engine)

	class CTransform;
	class CRcTex;
	class CTexture;
	class CAnimation;
	class CCalculator;
	class CCollider;

	END

class CTestWeapon :
	public CEquipmentBase
{
private:
	explicit CTestWeapon(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTestWeapon();

public:
	HRESULT Ready_Object(_uint iX, _uint iZ);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;
	virtual void	Collision_Event()override;

	_bool				Get_RenderFalse(void) { return m_bRenderFalse; }

	void	Picking(void);

private:
	HRESULT				Add_Component(void);

public:
	HRESULT				Open_Event(CGameObject* pGameObject);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CAnimation*				m_pAnimationCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;
	
private: /* 직교투영행렬. */
	_bool				m_bRenderFalse = false;

	_matrix				m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

	_uint iIndexRow = 0;
	_uint iIndexColumn = 0;

public:
	static CTestWeapon*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iX, _uint iZ);
	virtual void			Free(void);
};

