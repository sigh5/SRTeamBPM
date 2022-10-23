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

	void	Receive_Damage(int _Damage);		//�������� �Դ´�.
	void	Receive_DefenseCount(int _iDamage);
	int		Get_AttackPower(void);				//���ݷ�(������)�� ���´�.
	void	Add_Coin(void) { ++m_Info._iCoin; } //���� �߰�
	void	Add_Key(void) { ++m_Info._iKey; }	//Ű �߰�
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