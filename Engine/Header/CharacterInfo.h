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

private:
	CharacterInfo m_Info;

public:

	virtual CCharacterInfo*		Clone(void) override;
	static CCharacterInfo*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	bool		m_bClone;

private:
	virtual void Free(void) override;
};

END