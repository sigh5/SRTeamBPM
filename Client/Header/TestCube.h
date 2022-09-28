#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CCubeTex;
class CTransform;
class CCalculator;

END

class CTestCube : public CGameObject
{
private:
	explicit CTestCube(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTestCube(const CGameObject& rhs);

public:
	HRESULT Ready_Object(int Posx, int Posy);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	Render_Obejct(void) override;

	void				Set_TransformPositon();
	_bool				Set_SelectGizmo();
	CREATECUBE			Set_CreateCubePlane();



	_bool*	Get_WireFrame() { return &m_bWireFrame; }
	void	Set_WireFrame(_bool bWireFrame) { m_bWireFrame = bWireFrame; }

	
private:
	HRESULT				Add_Component(void);
	
public:
	CCubeTex*			m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;

	//CTexture*			m_pTextureCom2 = nullptr;

private:
	_bool				m_bWireFrame = false;
	_bool				m_bIsOnterrrain = false;

	//_vec3	vRayDir, vRayPos;
public:
	static CTestCube*		Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx = 0, int Posy = 0);
};

