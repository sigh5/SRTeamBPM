#pragma once
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRcTex;
	class CTexture;
	class CCalculator;
}

class CTestPlayer :
	public CGameObject
{
private:
	explicit CTestPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTestPlayer();

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
	float				Get_TerrainY(void);		//지형의 Y값으로 설정하는 대신 리턴

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;

	_vec3				m_vDirection;

	float				m_fNowPosHeight = 0.f; //현재 포지션 지형의 Y값

	bool				m_bJump = FALSE;		//지금 점프상태인가?

	float				m_fJumpPower;			//점프력

	bool				m_bDash = FALSE;		//대쉬

	float				m_fDashPower = 0.f;		//대쉬 시 가속력
	float				m_fBuffDashPower = 0.f; //가중된 가속력

public:
	static CTestPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);
};

