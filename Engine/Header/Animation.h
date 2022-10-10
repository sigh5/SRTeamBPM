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
	HRESULT		Ready_Animation(int _iMaxMotion, int _iMinMotion, float _fInterval, _uint _iOrigin = 0); //���� ��������Ʈ, ó�� ��������Ʈ, ���� ����
	void	Move_Animation(float fTimeDelta);


	// ����ڰ� ���������� Key�� ���������� �̹��� 1�� ����
	void	Control_Animation(_uint iCount);	

	void	Control_Event_Animation(_bool bEvent);


	void	Open_Box_Animation(_bool bOpen);

	// Gun_Screen
	void	Gun_Animation(_bool* bShoot);
	// ~Gun_Screen

public:
	
	int		m_iMinMotion = 0;		//���� ��������Ʈ
	int		m_iMaxMotion = 0;		//������ ��������Ʈ
	
	int		m_iMotion = 0;			//���� ��������Ʈ
	_float	m_fMotionChangeCounter = 0.f;
	_float	m_fIntervalMotion = 0.f;	//��������Ʈ �� ����

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