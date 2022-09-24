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
	HRESULT		Ready_Animation(int _iMaxMotion, int _iMinMotion, float _fInterval); //���� ��������Ʈ, ó�� ��������Ʈ, ���� ����
	void	Move_Animation(float fTimeDelta);

public:
	
	int		m_iMinMotion = 0;		//���� ��������Ʈ
	int		m_iMaxMotion = 0;		//������ ��������Ʈ
	
	int		m_iMotion = 0;			//���� ��������Ʈ
	_float	m_fMotionChangeCounter = 0.f;
	_float	m_fIntervalMotion = 0.f;	//��������Ʈ �� ����


public:
	virtual CComponent* Clone(void) override;
	static CAnimation*		Create(LPDIRECT3DDEVICE9 pGraphicDev, int _iMaxMotion, int _iMinMotion, float _fInterval);
private:
	virtual void Free(void) override;
};

END