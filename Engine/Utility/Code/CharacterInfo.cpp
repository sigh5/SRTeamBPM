#include "..\..\Header\CharacterInfo.h"



#include "Export_Function.h"

USING(Engine)


CCharacterInfo::CCharacterInfo(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev), m_bClone(false)
{
}


CCharacterInfo::CCharacterInfo(const CCharacterInfo & rhs)
	: CComponent(rhs), m_bClone(true)
{
}

CCharacterInfo::~CCharacterInfo()
{
}

HRESULT CCharacterInfo::Ready_CharacterInfo(int _hp, int _Attack, float _fSpeed)
{
	memcpy(&m_Info._iHp, &_hp, sizeof(int));
	memcpy(&m_Info._iAttackPower, &_Attack, sizeof(int));
	memcpy(&m_Info._fSpeed, &_fSpeed, sizeof(float));
	m_Info._iCoin = 0;
	m_Info._iKey = 0;

	return S_OK;
}

void CCharacterInfo::Receive_Damage(int _Damage)
{
	m_Info._iHp -= _Damage;
}

int CCharacterInfo::Get_AttackPower(void)
{
	return m_Info._iAttackPower;
}

void CCharacterInfo::Add_Hp(_uint iAddHp)
{
	m_Info._iHp += iAddHp;
}

CCharacterInfo * CCharacterInfo::Clone(void)
{
	return new CCharacterInfo(*this);
}

CCharacterInfo * CCharacterInfo::Create(LPDIRECT3DDEVICE9 pGraphicDev, int _hp, int _Attack, float _fSpeed)
{
	CCharacterInfo*	pInstance = new CCharacterInfo(pGraphicDev);

	if (FAILED(pInstance->Ready_CharacterInfo( _hp, _Attack, _fSpeed)))
	{
		MSG_BOX("Collider Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCharacterInfo::Free(void)
{
	CComponent::Free();
}
