#pragma once
#include "Base_Effect.h"



namespace Engine
{
	class CTransform;
	class CRcTex;
	class CTexture;
}

class CEffect : public CBase_Effect
{
private:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffect();

public:
	virtual HRESULT Ready_Object(const _vec3& vPos) ;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);
	
private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;


public:
	static CEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev,const _vec3& vPos);
	virtual void	Free(void);
};

