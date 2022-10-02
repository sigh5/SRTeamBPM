#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CCharacterInfo;
class CTransform;
class CRcTex;
class CTexture;
class CAnimation;
class CCalculator;
class CDynamic_Transform;

class ENGINE_DLL CItemBase : public CGameObject
{
protected:
	explicit CItemBase(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CItemBase(const CItemBase& rhs);
	virtual ~CItemBase();

public:
	virtual HRESULT		 Ready_Object(void)							override;
	virtual _int		 Update_Object(const _float& fTimeDelta)	override;
	virtual void		 LateUpdate_Object(void)					override;

	_uint&			Get_ItemType() { return m_iItemIndex; }
	CharacterInfo&	Get_InfoRef();


protected:
	CCharacterInfo*		m_pInfoCom = nullptr;  
	CAnimation*			m_pAnimationCom = nullptr;	 
	CDynamic_Transform* m_pDynamicTransCom = nullptr;

	_uint m_iItemIndex;

public:
	ITEM_TYPE			m_ItemType;

protected:
	virtual HRESULT				Add_Component(void);
	virtual bool				Set_TransformPos(HWND g_hWnd, CCalculator* _pCalcul, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, const _tchar* pComponentTag1, COMPONENTID eID, COMPONENTID eID1);

public:
	virtual void		Free(void);
};

END