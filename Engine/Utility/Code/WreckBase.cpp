#include "..\..\Header\WreckBase.h"

#include "RcTex.h"
#include "Transform.h"
#include "Texture.h"

#include "Export_Utility.h"

USING(Engine)

CWreckBase::CWreckBase(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
	ZeroMemory(&m_vDir, sizeof(_vec3));
	ZeroMemory(&m_vUpDown, sizeof(_vec3));
}


CWreckBase::~CWreckBase()
{
}

HRESULT CWreckBase::Ready_Object(_vec3 vPos, _vec3 vDir)
{
	return S_OK;
}

_int CWreckBase::Update_Object(const _float & fTimeDelta)
{
	return 0;
}

void CWreckBase::LateUpdate_Object(void)
{
}

void CWreckBase::Render_Obejct(void)
{
}

HRESULT CWreckBase::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom" , pComponent });

	m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_RcTexCom" , pComponent });
	return S_OK;
}

void CWreckBase::Free(void)
{
	CGameObject::Free();
}
