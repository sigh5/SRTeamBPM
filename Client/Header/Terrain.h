#pragma once
#include "GameObject.h"



namespace Engine
{
	class CTransform;
	class CTerrainTex;
	class CTexture;
	class CCalculator;
}
//ÁÖ¼®Áö¿ì¼À
class CTerrain : public CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrain();

public:
	void	Set_WireFrame(_bool bWireFrame) { m_bWireFrame = bWireFrame; }
	_bool*	Get_WireFrame() { return &m_bWireFrame; }

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;


public:
	_bool				Set_SelectGizmo();	//Only Use tool

private:
	HRESULT				Add_Component(void);
	HRESULT				SetUp_Material(void);

public:
	CTerrainTex*		m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*	   m_pCalculatorCom = nullptr;


	_bool				m_bWireFrame = false;
	_vec3				m_vDirection;
	

public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);
};

