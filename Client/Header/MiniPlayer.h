#pragma once
#include "GameObject.h"
#include "Engine_Include.h"

namespace Engine
{
	class CDynamic_Transform;
	class CRcTex;
	class CTexture;
	class CCalculator;
	class CAnimation;
	class CCharacterInfo;
	class CCollider;
}

class CMiniPlayer :public CGameObject
{
private:
	explicit CMiniPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMiniPlayer();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;
	virtual void	Collision_Event();

private:
	HRESULT				Add_Component(void);
	void				Key_Input(const _float& fTimeDelta);

	void				Set_OnTerrain();



private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CDynamic_Transform*	m_pDynamicTransCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;


private:		
	_vec3				m_vDirection;
	_vec3				m_vUp;
	_vec3				m_vPos;
	_float				m_fFrame = 0.f;

public:
	static CMiniPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);


};

