#pragma once
#include "GameObject.h"
namespace Engine
{
	class CTransform;
	class CRcTex;
	class CTexture;

}
class CSphinxHole :
	public CGameObject
{
private:
	explicit CSphinxHole(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSphinxHole();

public:
	virtual HRESULT Ready_Object(_vec3 vPos, bool billboard);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;

	bool			m_bBillboard = false;

public:
	_float				m_fFrame = 0.f;


public:
	static CSphinxHole*		Create(LPDIRECT3DDEVICE9 pGraphicDev, bool _billborad, _vec3 vPos = _vec3{ 0.f,0.f,0.f } );
	virtual void	Free(void);

};

