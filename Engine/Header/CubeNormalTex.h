#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeNormalTex : public CVIBuffer
{
private:
	explicit		CCubeNormalTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit		CCubeNormalTex(const CCubeNormalTex& rhs);
	virtual			~CCubeNormalTex();

public:
	virtual			HRESULT Ready_Buffer(void) override;
	virtual void	Render_Buffer(void) override;

public:
	static			CCubeNormalTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	CComponent*		Clone(void);
	virtual void	Free(void) override;

	const _vec3*	Get_VtxPos(void) const { return m_vPos; }

private:
	_vec3*		m_vPos;
	_bool				m_bClone;

};

END