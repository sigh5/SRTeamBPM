#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CEquipInfo;
class CTransform;
class CRcTex;
class CTexture;
class CAnimation;
class CCalculator;
class CDynamic_Transform;

class ENGINE_DLL CEquipmentBase : public CGameObject
{
protected:
	explicit CEquipmentBase(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEquipmentBase(const CEquipmentBase& rhs);
	virtual ~CEquipmentBase();

public:
	virtual HRESULT		 Ready_Object(void)							override;
	virtual _int		 Update_Object(const _float& fTimeDelta)	override;
	virtual void		 LateUpdate_Object(void)					override;

//	CEquipInfo&			Get_EquipInfoRef();

protected:
	CEquipInfo*			m_pEquipInfoCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;
	CDynamic_Transform* m_pDynamicTransCom = nullptr;

protected:
	virtual HRESULT				Add_Component(void);

public:
	virtual void		Free(void);
};

END