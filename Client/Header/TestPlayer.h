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
	float				Get_TerrainY(void);		//������ Y������ �����ϴ� ��� ����

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;

	_vec3				m_vDirection;

	float				m_fNowPosHeight = 0.f; //���� ������ ������ Y��

	bool				m_bJump = FALSE;		//���� ���������ΰ�?

	float				m_fJumpPower;			//������

	bool				m_bDash = FALSE;		//�뽬

	float				m_fDashPower = 0.f;		//�뽬 �� ���ӷ�
	float				m_fBuffDashPower = 0.f; //���ߵ� ���ӷ�

public:
	static CTestPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);
};

