#pragma once
#include "EquipmentBase.h"
#include "TestPlayer.h"

BEGIN(Engine)

class CTransform;
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
	virtual HRESULT			Ready_Object(void);
	virtual _int			Update_Object(const _float& fTimeDelta);
	virtual void			LateUpdate_Object(void);
	virtual void			Render_Obejct(void);
	virtual void			Collision_Event()override;

	void				    Set_OnTerrain(void);
private:
	CTransform*				m_pTransCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRcTex*					m_pBufferCom = nullptr;
	CAnimation*				m_pAnimationCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;
	

private:
	HRESULT					Add_Component(void);

	// 매그넘이 가지는 기본 탄창량
	_uint					m_iMagnumMagazine = 8;

public:
	static CMagnum*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);
};

