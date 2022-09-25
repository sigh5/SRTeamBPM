#pragma once
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRcTex;
	class CTexture;
	class CCalculator;
	class CScene;
	class CManagement;
}



class CHWPlayer :
	public CGameObject
{
private:
	explicit CHWPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHWPlayer();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);
	void				Key_Input(const _float& fTimeDelta);
	void				Set_OnTerrain(void);
	_vec3				PickUp_OnTerrain(void);
	float				Get_TerrainY(void);		//������ Y������ �����ϴ� ��� ����


private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;


private:
	// _vec3
	_vec3				m_vDirection;
	_vec3				m_vUp;
	_vec3				m_vPos;

	// Alpha Blending �뵵 (���ҽ� ���� �� ���� ����)
	_float				m_fFrame = 0.f;
	// ~Alpha Blending �뵵

public:
	static CHWPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);
};

