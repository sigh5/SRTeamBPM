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

	// 사용자가 수동적으로 Key를 누를때마다 이미지 변경
	void	Control_Animation(_uint iCount);	

	void	Open_Box_Animation(_bool bOpen);

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