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
	void				FatBat_Fly(void);
	void				FatBat_Shoot(void);

public:
	static CFatBat*		Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx = 0, int Posy = 0);
	virtual void	Free(void);

private:
	int m_iPreIndex = 0;
	int	m_iCoolTime;

	CTexture*		m_pTextureCom = nullptr;
	CRcTex*			m_pBufferCom = nullptr;
	CCalculator*	m_pCalculatorCom = nullptr;
};
