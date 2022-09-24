#pragma once
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRcTex;
	class CCalculator;
	class CTexture;
}

class CMonster :public CGameObject
{
private:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonster();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);
	void				Set_OnTerrain(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	_vec3				m_vDirection;
	_float				m_fTimeDelta = 0.f;
	_float				m_fSpeed = 0.f;

	int					m_iMotion = 0; //��ǹ�ȣ
	_float				m_fMotionChangeCounter = 0.f;
	_float				m_fBeetweenMotion = 0.f;


public:
	static CMonster*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);
};

