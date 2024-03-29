#pragma once
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CTerrainTex;
	class CTexture;
	class CShader;
	class CCalculator;
	class CTerrainMoveTex;
	class CCollider;
}


class CMiniTerrain : public CGameObject
{
private:
	explicit CMiniTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMiniTerrain();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;
	virtual void		Collision_Event() override;
private:
	HRESULT				Add_Component(void);


public:
	void			InitScaleAndPos(_vec3* vScale, _vec3* vPos, _bool bisPaDo = false);



private:
	CTerrainMoveTex*		m_pBufferCom = nullptr;
	CTransform*				m_pTransCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;

	_bool				m_pCreateControlSphere = false;
	_bool				m_bWireFrame = false;
	_vec3				m_vDirection;
	_vec3				m_vCenter;

	_float				m_fTime = 0.f;
	_bool				m_bTest = false;
	_bool				m_bPado = false;
public:
	static CMiniTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);
};
