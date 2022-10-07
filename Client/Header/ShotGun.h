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


class CShotGun : public CEquipmentBase
{
private:
	explicit CShotGun(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShotGun();

public:
	HRESULT				Ready_Object(_uint iX, _uint iZ);
	virtual _int		Update_Object(const _float& fTimeDelta);
	virtual void		LateUpdate_Object(void);
	virtual void		Render_Obejct(void);
	virtual void		Collision_Event()override;
	
	void				 Set_OnTerrain(void);
private:
	CTransform*				m_pTransCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRcTex*					m_pBufferCom = nullptr;
	CAnimation*				m_pAnimationCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;
	

private:
	HRESULT				Add_Component(void);

	// 샷건이 가지는 기본 탄창량
	_uint				m_iShotgunMagazine = 6;
	
	_bool				m_bRenderFalse = false;

public:
	static CShotGun*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iX, _uint iZ);
	virtual void		Free(void);
	
};

