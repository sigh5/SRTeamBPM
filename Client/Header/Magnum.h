#pragma once
#include "Engine_Include.h"
#include "EquipmentBase.h"

BEGIN(Engine)

class COrthoTransform;
class CTexture;
class CRcTex;
class CAnimation;
class CCalculator;
class CCollider;

END

class CMagnum : public CEquipmentBase
{
public:
	explicit CMagnum(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMagnum();

public:
	HRESULT					Ready_Object();
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual void			LateUpdate_Object(void);
	virtual void			Render_Obejct(void);
	virtual void			Collision_Event()override {}

	void					Set_MagnumRender(_bool _bRender) { m_bRenderFalse = _bRender; }

	void	Picking(void);
	
private:
	COrthoTransform*		m_pTransCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRcTex*					m_pBufferCom = nullptr;
	CAnimation*				m_pAnimationCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;
	

private:
	HRESULT					Add_Component(void);


	// 매그넘이 가지는 기본 탄창량
	_uint					m_iMagnumMagazine = 8;
	_bool					m_bRenderFalse = false;
	
	_bool					m_bOrthoPos = false;

	_vec3					m_vecOrtho;

	_matrix					m_matProj;

	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;

public:
	static CMagnum*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);
};

