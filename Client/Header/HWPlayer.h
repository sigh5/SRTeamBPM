#pragma once
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CDynamic_Transform;
	class CRcTex;
	class CTexture;
	class CCalculator;
	class CScene;
	class CManagement;
	class CCollider;
	class CAnimation;
}



class CHWPlayer :
	public CGameObject
{
private:
	explicit CHWPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHWPlayer();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);
	void				Key_Input(const _float& fTimeDelta);
	void				Set_OnTerrain(void);
	_vec3				PickUp_OnTerrain(void);
	float				Get_TerrainY(void);		//지형의 Y값으로 설정하는 대신 리턴

	
public:
	void				Collsion_CubeMap(CGameObject* pGameObject);
	_uint				Get_Magazine(void) { return m_iMagazine; }



	void				Set_OneShot(_bool bMetro) { m_bOneShot = bMetro; }


	void				Penalty_ComBo();
	void				Miss_ClickMouseLB(const _float & fTimeDelta);

private:
	HRESULT				Create_bullet(_vec3 pos, const _float & fTimeDelta);

	_uint			m_iCount = 0;
	_uint			m_iCoolTime = 0;
	_bool			m_bOneShot = false;
	_uint			m_iMagazine = 8;

public:
	_bool			m_bComboPenalty = true;
	_float			m_ComboTimer = 0.f;
	_float			m_fMissClick = 0.f;
	_bool			m_bMissCheck = false;

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;
	CDynamic_Transform*	m_pDynamicTransCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;

private:
	// _vec3
	_vec3				m_vDirection;
	_vec3				m_vUp;
	_vec3				m_vPos;

	// Alpha Blending 용도 (리소스 얻은 뒤 쓰일 예정)
	_float				m_fFrame = 0.f;
	// ~Alpha Blending 용도
	
	DIR					m_eDirType = DIR_END;

public:
	static CHWPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);
};

