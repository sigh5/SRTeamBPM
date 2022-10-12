#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CThunderTex;
class CTransform;
class CRcTex;
class CAnimation;
END
class CAnubisThunder :public CGameObject
{
private:
	explicit CAnubisThunder(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAnubisThunder();

public:
	virtual HRESULT		Ready_Object(_float Posx, _float Posy);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

	void				Set_Direction(_vec3* _Dirvec);

private:
	HRESULT				Add_Component(void);

public:
	static CAnubisThunder*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _float Posx = 0, _float Posy = 0);
	virtual void	Free(void);

private:
	CTexture*		m_pTextureCom = nullptr;
	CThunderTex*	m_pBufferCom = nullptr;
	//CRcTex*			m_pBufferCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CAnimation*		m_pAnimationCom = nullptr;

};

