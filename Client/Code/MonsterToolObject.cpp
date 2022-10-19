#include "stdafx.h"
#include "..\Header\MonsterToolObject.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "Ghul.h"
#include "Obelisk.h"
#include "Sphinx.h"
#include "SphinxFlyHead.h"
#include "EarthShaker.h"
#include "Animation.h"


CMonsterToolObject::CMonsterToolObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CMonsterToolObject::CMonsterToolObject(const CMonsterToolObject& rhs)
	: CGameObject(rhs)
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

	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);

	m_pAnimationCom->Ready_Animation(9, 0, 50000.f);

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
			//Safe_Release(m_pTextureCom);
			m_iPreIndex = m_iMonsterIndex;
			m_pAnimationCom->m_iMotion = m_iMonsterIndex;
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

	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);	// 텍스처 정보 세팅을 우선적으로 한다.

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
	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"TerrainLayer", L"TestMap", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, false);

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"TerrainLayer", L"TestMap", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, false);

	_vec3 Temp = _pCalcul->PickingOnTerrainCube(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);

	m_pTransCom->Set_Pos(Temp.x, Temp.y, Temp.z);

	return false;
}

void CMonsterToolObject::MousePostoScreen()
{
	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(m_LayerName.c_str(),
		m_RoomName.c_str(),
		L"Proto_TerrainTexCom", ID_STATIC));

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(m_LayerName.c_str(),
		m_RoomName.c_str(),
		L"Proto_TransformCom", ID_DYNAMIC));

	if (pTerrainTransformCom == nullptr || pTerrainBufferCom == nullptr)
	{
		MSG_BOX("Choose Terrain");
		return;
	}
	//주석지우셈
	_vec3 Temp = m_pCalculatorCom->PickingOnTerrainCube(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
	m_pTransCom->Set_Pos(Temp.x, Temp.y, Temp.z);
}

HRESULT CMonsterToolObject::InitSetting(_vec2 * vMousPos, const wstring & LayerName, wstring * RoomName)
{
	Set_Layer_Map_Name(LayerName, RoomName);
	CScene* pScene = ::Get_Scene();
	CLayer* pLayer = pScene->GetLayer(LayerName.c_str());

	if ((*vMousPos).x == 0 && (*vMousPos).y == 0) {}
	else
		MousePostoScreen(); // 현재 마우스 더블클릭한 위치로 큐브를 만들어주는 함수

							//임시 코드
	_vec3 vCurretPos;
	m_pTransCom->Get_Info(INFO_POS, &vCurretPos);
	vCurretPos.y += 0.5f;

	m_pTransCom->Set_Y(vCurretPos.y);


	/*_vec3 vScale = { 3.f,3.f,3.f };
	m_pTransCom->Set_Scale(&vScale);
	*/
	//m_pTransCom->Get_Info(INFO_POS, &vCurretPos);


	return S_OK;
}
void CMonsterToolObject::Free(void)
{
	CGameObject::Free();
}
