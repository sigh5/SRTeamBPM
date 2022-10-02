#pragma once
#include "MonsterBase.h"


class CAnubis :public CMonsterBase
{
private:
	explicit CAnubis(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAnubis();

public:
	virtual HRESULT		Ready_Object(int Posx, int Posy);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	CCalculator*		Get_Calculator(void) { return m_pCalculatorCom; }
	CRcTex*				Get_Buffer(void) { return m_pBufferCom; }

public:
	static CAnubis*		Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx = 0, int Posy = 0);
	virtual void	Free(void);

private:
	int m_iPreIndex = 0;

	CTexture*		m_pTextureCom = nullptr;
	CRcTex*			m_pBufferCom = nullptr;
	CCalculator*	m_pCalculatorCom = nullptr;


};

