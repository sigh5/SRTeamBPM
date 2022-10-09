#include "stdafx.h"
#include "..\Header\MonsterToolObject.h"

#include "Export_Function.h"
#include "AbstractFactory.h"


CMonsterToolObject::CMonsterToolObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CMonsterToolObject::CMonsterToolObject(const CMonsterToolObject& rhs)
	:CGameObject(rhs)
{

}
CMonsterToolObject::~CMonsterToolObject()
{
}

HRESULT CMonsterToolObject::Ready_Object(int Posx, int Posy)
{
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Monster_Example_Texture", m_mapComponent, ID_STATIC);

	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);

	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);

	m_iMonsterIndex = 0;
	m_iPreIndex = 0;
	if (Posx == 0 && Posy == 0) {}
	else
	{
		Set_TransformPositon(g_hWnd, m_pCalculatorCom);
	}
	return S_OK;
}

_int CMonsterToolObject::Update_Object(const _float & fTimeDelta)
{
	if (SCENE_TOOLTEST == Get_Scene()->Get_SceneType())
	{
		CTexture*	pComponent = nullptr;
		if (m_iPreIndex != m_iMonsterIndex)
		{
			Safe_Release(m_pTextureCom);
			m_iPreIndex = m_iMonsterIndex;
			switch (m_iMonsterIndex)
			{
			case 0:
				pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_MonsterTexture"));
				NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
				m_mapComponent[ID_STATIC].insert({ L"Proto_MonsterTexture", pComponent });
				break;

			case 1:
				pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_MonsterTexture2"));
				NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
				m_mapComponent[ID_STATIC].insert({ L"Proto_MonsterTexture2", pComponent });
				break;

			case 2:
				pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_MonsterTexture3"));
				NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
				m_mapComponent[ID_STATIC].insert({ L"Proto_MonsterTexture3", pComponent });
				break;

			default:
				pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_MonsterTexture"));
				NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
				m_mapComponent[ID_STATIC].insert({ L"Proto_MonsterTexture", pComponent });
				break;

			}
		}
	}
	Engine::CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CMonsterToolObject::LateUpdate_Object(void)
{
}

void CMonsterToolObject::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTextureCom->Set_Texture(m_iMonsterIndex);	// 텍스처 정보 세팅을 우선적으로 한다.

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CMonsterToolObject * CMonsterToolObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx, int Posy)
{
	CMonsterToolObject*	pInstance = new CMonsterToolObject(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(Posx, Posy)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

bool CMonsterToolObject::Set_SelectGizmo(HWND g_hWn, CCalculator* _pCalcul, CRcTex* _pBuffer)
{
	if (_pCalcul->PickingOnTransform_Monster(g_hWnd, _pBuffer, m_pTransCom))
		return true;

	// 스정본

	return false;
}

bool CMonsterToolObject::Set_TransformPositon(HWND g_hWnd, CCalculator* _pCalcul)
{
	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"TestLayer", L"TestMap", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, false);

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"TestLayer", L"TestMap", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, false);

	_vec3 Temp = _pCalcul->PickingOnTerrainCube(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);

	m_pTransCom->Set_Pos(Temp.x, Temp.y, Temp.z);

	return false;
}

void CMonsterToolObject::Free(void)
{
	CGameObject::Free();
}
