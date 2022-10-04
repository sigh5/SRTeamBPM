#include "..\..\Header\Animation.h"

#include "Export_Function.h"

USING(Engine)

CAnimation::CAnimation(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
}

Engine::CAnimation::CAnimation(const CAnimation& rhs)
	: CComponent(rhs)
{

}

CAnimation::~CAnimation()
{
}
// �ִϸ��̼� ����� ���� �⺻ ���� �Լ�
HRESULT CAnimation::Ready_Animation(int _iMaxMotion, int _iMinMotion, float _fInterval, _uint _iOrigin)
{
	m_iMaxMotion = _iMaxMotion;
	m_iMinMotion = _iMinMotion;
	m_fIntervalMotion = _fInterval;
	m_iOrigin = _iOrigin;
	return S_OK;
}

// �������� ���ư��� �ִϸ��̼� �Լ�(����, ���� �� ������� ��Ʈ���� �ʿ���� ��� ���ƾ� �ϴ� ���)
void CAnimation::Move_Animation(float fTimeDelta)
{
	m_fMotionChangeCounter += fTimeDelta;

	if (m_fMotionChangeCounter > m_fIntervalMotion)
	{
		m_fMotionChangeCounter = 0;
		if (m_iMotion < m_iMaxMotion)
			++m_iMotion;
		else
			m_iMotion = m_iMinMotion;

	}
	else
	{
		m_fMotionChangeCounter += fTimeDelta;
	}
}
								
// Bull_UIó�� 3�� �̻� �ؽ�ó�� �ְ� �Ű� ���� 1ȸ�� �ؽ�ó�� 1�常 �Ѿ�� �� �� ��� 
void CAnimation::Control_Animation(_uint iCount)
{		
	if (m_iOrigin > iCount)
	{			
		m_iMotion += 1;
	}	 
	
	m_iOrigin = iCount;

	/*if (m_iMotion > (m_iMaxMotion))	
		m_iMotion = m_iMinMotion;*/

	if(iCount == m_iMaxMotion)
		m_iMotion = m_iMinMotion;
	
}
// ����� �ڽ����� ���, �Ű������� 1�� �ް� �״�� �ִϸ��̼��� ���� ��ҵ�(ex: �ڽ��� �����ٰ�->������ ��)
void CAnimation::Open_Box_Animation(_bool bOpen)
{
	_uint iOrig = m_iMotion;

	if (bOpen)
	{
		m_iMotion++;
		iOrig = m_iMotion;
	}
	m_iMotion = iOrig;

}

// Gun_Screen���� �����. _bool Ÿ�� �Ű������� ������ �ѹ��� Ʈ����(Ŭ�� ��)�� name%d.png�� �ѹ��� ���� ��.
void CAnimation::Gun_Animation(_bool* bShoot)
{	
	if (bShoot)
	{
		m_iMotion++;
		
		if (m_iMotion == m_iMaxMotion)
		{
			bool bfalse = false;
			memcpy(bShoot, &bfalse, sizeof(bool));
			m_iMotion = m_iMinMotion;				
		}
	}		
}

CComponent * CAnimation::Clone(void)
{
	return new CAnimation(*this);
}

CAnimation * CAnimation::Create(LPDIRECT3DDEVICE9 pGraphicDev, int _iMaxMotion, int _iMinMotion, float _fInterval)
{
	CAnimation*	pInstance = new CAnimation(pGraphicDev);

	if (FAILED(pInstance->Ready_Animation( _iMaxMotion, _iMinMotion, _fInterval)))
	{
		MSG_BOX("Animation Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CAnimation::Free(void)
{
	CComponent::Free();

	
}

