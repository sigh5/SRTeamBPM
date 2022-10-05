#pragma once
#include "MonsterBase.h"


class CFatBat :public CMonsterBase
{
private:
	explicit CFatBat(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFatBat();

public:
	virtual HRESULT		Ready_Object(int Posx, int Posy);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	void				FatBat_Fly(const _float& fTimeDelta);
	void				FatBat_Shoot(void);
	void				FatBat_Dodge(const _float& fTimeDelta, _vec3* _vPlayerPos, _vec3* _vMonsterPos);


public:
	static CFatBat*		Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx = 0, int Posy = 0);
	virtual void	Free(void);

private:
	_float				m_fFrame = 0.f; // WinZero  µµÂø½Ã°£

private:
	int m_iPreIndex = 0;
	float m_fActionDelay;
	float m_fFlyDelayCount;
	float m_fMaxY;
	bool  m_bAltitude;

	float m_fDodgeDelayCount;
	int m_iDodgeDir;
	float m_fDodgeSpeed;
	float m_fDodgeStopper;
	float m_fStopperDelay;
	float m_fStopperDelayCount;


	CTexture*		m_pTextureCom = nullptr;
	CRcTex*			m_pBufferCom = nullptr;
	CCalculator*	m_pCalculatorCom = nullptr;
};
