#pragma once
#pragma once
#include "GameObject.h"
#include "Export_Function.h"
namespace Engine
{
	class CTransform;
	class CTerrainTex;
	class CTexture;
	class CShader;
	class CCalculator;
	class CTerrainMoveTex;
}

class CMagneticField;

class CStageTerrain :public CGameObject
{
private:
	explicit CStageTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStageTerrain();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;
	

	CMagneticField*		Get_CurrentSphere() {return  m_pCurrentField;}


public:
	_vec3			Get_TerrainCenter() { return m_pBufferCom->Get_Center(); }

private:
	HRESULT				Add_Component(void);

	void				Create_Once();
	

private:
	CTerrainTex*		m_pBufferCom = nullptr;
	CTransform*				m_pTransCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CShader*				m_pShaderCom = nullptr;


	_bool					m_bOnce = false;
	float					m_fFrame = 0.f;

	_int					m_iPreNum = 32;


	_bool					m_bPlayerGetSkillCheck[5] = { false, };


	CMagneticField*			m_pCurrentField = nullptr;

public:
	static CStageTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);
};

