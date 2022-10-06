#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CCubeTex;
class CTransform;
class CCalculator;

END

class CWallCube : public CGameObject
{
private:
	explicit			CWallCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual				~CWallCube();
public:
	HRESULT				Ready_Object(_vec2 *vPos);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		Render_Obejct(void) override;

public:
	void				MousePostoScreen(); // 현재 마우스 더블클릭한 위치로 큐브를 만들어주는 함수
	_bool				Set_SelectGizmo();
	_bool*				Get_WireFrame() { return &m_bWireFrame; }
	void				Set_WireFrame(_bool bWireFrame) { m_bWireFrame = bWireFrame; }

private:
	HRESULT				Add_Component(void);

public:
	CCubeTex*			m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;

private:
	_bool				m_bWireFrame = false;
	_bool				m_bIsOnterrrain = false;

public:
	static CWallCube*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec2 *vPos);

	virtual void		Free()override;

};

// 왜깃만 쓰면 이러냐 

