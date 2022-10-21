#pragma once
#include "PSystem.h"
class CSkillParticle :public CPSystem
{
private:
	CSkillParticle(LPDIRECT3DDEVICE9 pGraphicDev, BDBOX* boundingBox, int numParticles, _vec3 vPos);
	void resetParticle(Attribute* attribute);
	virtual ~CSkillParticle();
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual void		update(_float fTimeDelta);
	virtual void		Render_Obejct(void);

public:
	void			Set_Active(_bool bEvnet) { m_bActive = bEvnet; }
	const	_bool	Get_Active()const { return m_bActive; }



public:
	static CSkillParticle*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void	Free(void);



private:
	CTexture* m_pTextureCom = nullptr;
	CTransform* m_pTransform = nullptr;

private:
	_bool			m_bActive = false;
	_float			m_fTimedelta = 0.f;
	_float			m_fFrame = 0.f;

	_int			m_iTemp = -1;


	_matrix				m_ProjMatrix;

	_bool			m_fOnce = true;

};

