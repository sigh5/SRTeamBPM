#include "..\..\Header\Dynamic_Transform.h"
#include "Export_Function.h"

USING(Engine)

CDynamic_Transform::CDynamic_Transform()
	: m_bClone(false)
{
}

CDynamic_Transform::CDynamic_Transform(const CDynamic_Transform & rhs)
	: CTransform(rhs), m_bClone(true)
{
}


CDynamic_Transform::~CDynamic_Transform()
{
}

HRESULT CDynamic_Transform::Ready_Dynamic_Transform(void)
{
	FAILED_CHECK_RETURN(CTransform::Ready_Transform(), E_FAIL);

	return S_OK;
}

_int CDynamic_Transform::Update_Component(const _float & fTimeDelta)
{
	CTransform::Update_Component(fTimeDelta);

	return 0;
}

_float CDynamic_Transform::Get_TerrainY1(const _tchar * pLayerTag, const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID, CCalculator* pCalculator, CTransform* pTransform)
{
	_vec3		vPos;
	D3DXVec3Normalize(&vPos, &vPos);
	//vPos = m_vInfo[INFO_POS];
	pTransform->Get_Info(INFO_POS, &vPos);
																				
	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(pLayerTag, pObjTag, pComponentTag, eID));
	NULL_CHECK_RETURN(pTerrainTexCom,0.f);  // ¡Ú

	_float fHeight = pCalculator->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);
	
	return fHeight;
}

void CDynamic_Transform::Jumping(_float _JumpPower, _float _TimeDelta, CTransform* pTransform)
{
	_vec3 vWorldUp = { 0.f, 1.f, 0.f };

	vWorldUp = vWorldUp *_JumpPower * _TimeDelta;

	pTransform->Move_Pos(&vWorldUp);
}
// ¡Ø
void CDynamic_Transform::Dashing(const _float & fTimeDelta, CTransform* pTransform, _vec3 vecUp, _vec3 vecDir, TYPING_TYPE tType)
{


	_vec3	vPos, vUp, vDir, vRight;

	pTransform->Get_Info(INFO_POS, &vPos);
	
	_float fSpeed = 5.f;
	// ¡Ø

	vUp = vecUp;
	vDir = vecDir;

	switch (tType)
	{

		case TYPING_W:	
		{			
				pTransform->Move_Pos(&(vDir * (fSpeed + fTimeDelta) * 0.3f));
			
				break;			
		}
		
		case TYPING_S:
		{			
				pTransform->Move_Pos(&(vDir * (-fSpeed + fTimeDelta)* 0.3f));
			
				break;			
		}

		case TYPING_A:
		{			
				D3DXVec3Normalize(&vDir, &vDir);
				D3DXVec3Normalize(&vUp, &vUp);
				D3DXVec3Cross(&vRight, &vDir, &vUp);
				pTransform->Move_Pos(&(vRight * (fSpeed + fTimeDelta)* 0.3f));
			
				break;			
		}

		case TYPING_D:
		{			
				D3DXVec3Normalize(&vDir, &vDir);
				D3DXVec3Normalize(&vUp, &vUp);
				D3DXVec3Cross(&vRight, &vDir, &vUp);
				pTransform->Move_Pos(&(vRight * (-fSpeed + fTimeDelta)* 0.3f));
			
				break;			
		}

		default:
		break;		
	}

}
//void CDynamic_Transform::Dashing(const _float & fTimeDelta, CTransform* pTransform)
//{
//	_vec3	vecLook, vecUp, vecPos, vecRight;
//
//	_vec3	vecSight;
//
//	pTransform->Get_Info(INFO_LOOK, &vecLook);
//	pTransform->Get_Info(INFO_UP, &vecUp);
//	pTransform->Get_Info(INFO_POS, &vecPos);
//	pTransform->Get_Info(INFO_RIGHT, &vecRight);
//	
//}
void CDynamic_Transform::Monster_Fly(CTransform* _pTransformCom, float _TerrainY, float _MaxY)
{
	/*_vec3 vPos;
	_pTransformCom->Get_Info(INFO_POS, &vPos);*/

	
}

CDynamic_Transform * CDynamic_Transform::Create(void)
{
	CDynamic_Transform* pInstance = new CDynamic_Transform;
	
	if (FAILED(pInstance->Ready_Dynamic_Transform()))
	{
		Safe_Release(pInstance);
		
		return nullptr;
	}
	return pInstance;
}

CComponent * CDynamic_Transform::Clone(void)
{
	return new CDynamic_Transform(*this);
}

void CDynamic_Transform::Free(void)
{
	CComponent::Free();
}
