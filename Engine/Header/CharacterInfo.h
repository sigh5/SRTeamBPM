#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCharacterInfo :public CComponent
{
private:
	explicit CCharacterInfo(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCharacterInfo(const CCharacterInfo& rhs);
	virtual ~CCharacterInfo();

public:
	HRESULT	 Ready_CharacterInfo();

private:
	int		m_iHp;
	int		m_iAttackPower;
	int		m_iCoin;
	int		m_iKey;
	float	m_fSpeed;


public:

	virtual CCharacterInfo*		Clone(void) override;
	static CCharacterInfo*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	bool		m_bClone;
};

END