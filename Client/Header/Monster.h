#pragma once
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRcTex;
	class CCalculator;
	class CTexture;
	class CAnimation;
}

class CMonster :public CGameObject
{
private:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonster();

public:
	virtual HRESULT Ready_Object(int Posx, int Posy);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;
	virtual void	Set_Pos(_vec3 vPos);


private:
	HRESULT				Add_Component(void);
	void				Set_OnTerrain(void);
	void				Set_TransformPositon();

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;

	//_vec3				m_vDirection;
	_float				m_fTimeDelta = 0.f;
	_float				m_fSpeed = 0.f;

	//int					m_iMotion = 0; //모션번호
	//_float				m_fMotionChangeCounter = 0.f;
	//_float				m_fBeetweenMotion = 0.f;


public:
	static CMonster*		Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx = 0, int Posy = 0);
	virtual void	Free(void);
};

