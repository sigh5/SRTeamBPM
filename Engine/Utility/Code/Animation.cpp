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
// 애니메이션 사용을 위한 기본 세팅 함수
HRESULT CAnimation::Ready_Animation(int _iMaxMotion, int _iMinMotion, float _fInterval, _uint _iOrigin)
{
	m_iMaxMotion = _iMaxMotion;
	m_iMinMotion = _iMinMotion;
	m_fIntervalMotion = _fInterval;
	m_iOrigin = _iOrigin;
	return S_OK;
}

// 쉴새없이 돌아가는 애니메이션 함수(포션, 코인 등 사용자의 컨트롤이 필요없이 계속 돌아야 하는 경우)
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
								
// Bull_UI처럼 3장 이상 텍스처가 있고 매개 변수 1회에 텍스처가 1장만 넘어가야 할 때 사용 
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
// 현재는 박스에서 사용, 매개변수를 1번 받고 그대로 애니메이션이 끝날 요소들(ex: 박스는 닫혔다가->열리고 끝)
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

// Gun_Screen에서 사용중. _bool 타입 매개변수를 받으면 한번의 트리거(클릭 등)에 name%d.png가 한바퀴 돌게 됨.
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

