#pragma once
#include "GameObject.h"
namespace Engine
{
	class CTransform;
	class CTexture;
	class CRcTex;
}
class CSphinxBody :
	public CGameObject
{
private:
	explicit CSphinxBody(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSphinxBody();

public:
	virtual HRESULT		Ready_Object(float Posx, float Posy, float Size);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	CTransform* m_pTransformCom;
	CTexture*	m_pTextureCom;
	CRcTex*		m_pBufferCom;

public:
	static CSphinxBody*		Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx = 0, float Posy = 0, float Size = 0);
	virtual void	Free(void);
};

