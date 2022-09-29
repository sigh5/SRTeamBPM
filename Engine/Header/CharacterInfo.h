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
	int		Get_AttackPower(void);				//���ݷ�(������)�� ���´�.
	void	Add_Coin(void) { ++m_Info._iCoin; } //���� �߰�
	void	Add_Key(void) { ++m_Info._iKey; }	//Ű �߰�
	CharacterInfo&	Get_InfoRef() { return m_Info; }

	// Test - Pjw
	void	Add_Hp(_uint iAddHp);

	// ~Test - Pjw
private:
	CharacterInfo m_Info;

public:

	virtual CCharacterInfo*		Clone(void) override;
	static CCharacterInfo*		Create(LPDIRECT3DDEVICE9 pGraphicDev, int _hp, int _Attack, float _fSpeed);

private:
	bool		m_bClone;

private:
	virtual void Free(void) override;
};

END