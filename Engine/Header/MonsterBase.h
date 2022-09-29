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

class ENGINE_DLL CMonsterBase :public CGameObject
{
protected:
	explicit CMonsterBase(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonsterBase(const CMonsterBase& rhs);
	virtual ~CMonsterBase();

public:
	virtual HRESULT		 Ready_Object(void)							override;
	virtual _int		 Update_Object(const _float& fTimeDelta)	override;
	virtual void		 LateUpdate_Object(void)					override;
	bool		Set_SelectGizmo(HWND g_hWn);
	int&			Get_MonsterType() { return m_iMonsterIndex; }
	CharacterInfo&	Get_InfoRef();

protected:
	CCharacterInfo*		m_pInfoCom = nullptr;    // 남길것 
	CAnimation*			m_pAnimationCom = nullptr;	 // 남길것 
	CDynamic_Transform* m_pDynamicTransCom = nullptr;
	


	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;

	


	int m_iMonsterIndex;

protected:
	virtual HRESULT				Add_Component(void);
	virtual bool				Set_TransformPositon(HWND g_hWnd);

public:
	virtual void		Free(void);
};

END