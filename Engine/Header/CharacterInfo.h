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
	HRESULT	 Ready_CharacterInfo(int _hp, int _Attack, float _fSpeed);

	void	Receive_Damage(int _Damage);		//데미지를 입는다.
	void	Receive_DefenseCount(int _iDamage);
	int		Get_AttackPower(void);				//공격력(데이터)을 얻어온다.
	void	Add_Coin(void) { ++m_Info._iCoin; } //코인 추가
	void	Add_Key(void) { ++m_Info._iKey; }	//키 추가
	CharacterInfo&	Get_InfoRef() { return m_Info; }
	int		Get_Hp() { return m_Info._iHp; }

	void	Use_Coin(void) { --m_Info._iCoin; }

	// Test - Pjw
	void	Add_Hp(_uint iAddHp);
	void	Add_Atk(_uint iAddAtk) { m_Info._iAttackPower += iAddAtk; }
	// ~Test - Pjw
private:
	CharacterInfo m_Info;

public:

	virtual CComponent*			Clone(void) override;
	static CCharacterInfo*		Create(LPDIRECT3DDEVICE9 pGraphicDev, int _hp, int _Attack, float _fSpeed);

private:
	bool		m_bClone;

private:
	virtual void Free(void) override;
};

END