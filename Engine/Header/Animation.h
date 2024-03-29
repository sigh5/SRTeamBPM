#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CAnimation :public CComponent
{
private:
	explicit CAnimation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAnimation(const CAnimation& rhs);
	virtual ~CAnimation();

public:
	HRESULT		Ready_Animation(int _iMaxMotion, int _iMinMotion, float _fInterval, _uint _iOrigin = 0); //시작 스프라이트, 처음 스프라이트, 변경 간격
	void	Move_Animation(float fTimeDelta);

	// 사용자가 수동적으로 Key를 누를때마다 이미지 1장 변경
	void	Control_Animation(_uint iCount);	

	void	Control_Event_Animation(_bool bEvent);
	//주석지우셈
	
	void	Open_Box_Animation(_bool bOpen);

	// Gun_Screen
	bool	Gun_Animation(const _float& fTimeDelta);
	// ~Gun_Screen

	void	Set_Motion(int _iMotion) { m_iOrigin = _iMotion; }

	void	Set_MaxMotion(int _iMaxMotion) { m_iMaxMotion = _iMaxMotion; }

	void	Add_Origin(_uint _iOrigin) { m_iOrigin += _iOrigin; }

	void	Eliminate_Motion(_uint _iMinusMotion) { m_iMotion -= _iMinusMotion; }

public:
	
	int		m_iMinMotion = 0;		//시작 스프라이트
	int		m_iMaxMotion = 0;		//마지막 스프라이트
	
	int		m_iMotion = 0;			//현재 스프라이트
	_float	m_fMotionChangeCounter = 0.f;
	_float	m_fIntervalMotion = 0.f;	//스프라이트 간 간격

	// Pjw's Add
	_uint	m_iOrigin = 0;


	_bool   m_bClone;



public:
	virtual CComponent* Clone(void) override;
	static CAnimation*		Create(LPDIRECT3DDEVICE9 pGraphicDev, int _iMaxMotion, int _iMinMotion, float _fInterval);
private:
	virtual void Free(void) override;
};

END