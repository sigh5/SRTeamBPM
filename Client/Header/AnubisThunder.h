#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CThunderTex;
class CTransform;
class CRcTex;

END
class CAnubisThunder :public CGameObject
{
private:
	explicit CAnubisThunder(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAnubisThunder();

public:
	virtual HRESULT		Ready_Object(int Posx, int Posy);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

public:
	static CAnubisThunder*		Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx = 0, int Posy = 0);
	virtual void	Free(void);

private:
	CTexture*		m_pTextureCom = nullptr;
	CThunderTex*	m_pBufferCom = nullptr;
	//CRcTex*			m_pBufferCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;


};

