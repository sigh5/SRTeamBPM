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

HRESULT CAnimation::Ready_Animation(int _iMaxMotion, int _iMinMotion, float _fInterval, _uint _iOrigin)
{
	m_iMaxMotion = _iMaxMotion;
	m_iMinMotion = _iMinMotion;
	m_fIntervalMotion = _fInterval;
	m_iOrigin = _iOrigin;
	return S_OK;
}

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

void CAnimation::Open_Box_Animation(_bool bOpen)
{
	if (bOpen)
		m_bOpen = true;

	if (m_bOpen)
	{
		m_iMotion++;
		m_bOpen = false;
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

