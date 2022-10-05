#include "..\..\Header\UI_Base.h"
#include "Export_Utility.h"

USING(Engine)



CUI_Base::CUI_Base(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CUI_Base::CUI_Base(const CUI_Base & rhs)
	: CGameObject(rhs)
{
}

CUI_Base::~CUI_Base()
{
}

HRESULT CUI_Base::Ready_Object(void)
{
	return S_OK;
}

_int CUI_Base::Update_Object(const _float & fTimeDelta)
{
	return 0;
}

void CUI_Base::LateUpdate_Object(void)
{
}

void CUI_Base::Set_OrthoMatrix(_float fSizeX, _float fSizeY, _float fPosX, _float fPosY)
{
	D3DXMatrixOrthoLH(&m_matProjMatrix, WINCX, WINCY, 0.f, 1.f);

	m_fSizeX = fSizeX;
	m_fSizeY = fSizeY; 
	m_fX = fPosX;
	m_fY = fPosY;
}

HRESULT CUI_Base::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pAnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pAnimationCom, E_FAIL);
	//m_pAnimationCom->Ready_Animation(6, 1, 0.2f);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_AnimationCom", pComponent });

	pComponent = m_pInfoCom = dynamic_cast<CCharacterInfo*>(Clone_Proto(L"Proto_CharacterInfoCom"));
	NULL_CHECK_RETURN(m_pInfoCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_CharacterInfoCom", pComponent });

	pComponent = m_pDynamicTransCom = dynamic_cast<CDynamic_Transform*>(Clone_Proto(L"Proto_DynamicTransformCom"));
	NULL_CHECK_RETURN(m_pDynamicTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_DynamicTransformCom" , pComponent });

	pComponent = m_pOrthoTransCom = dynamic_cast<COrthoTransform*>(Clone_Proto(L"Proto_OrthoTransformCom"));
	NULL_CHECK_RETURN(m_pOrthoTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_OrthoTransformCom" , pComponent });


	return S_OK;
}

void CUI_Base::Free(void)
{
	CGameObject::Free();
}
