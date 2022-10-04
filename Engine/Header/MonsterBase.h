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
	bool		Set_SelectGizmo(HWND g_hWn, CCalculator* _pCalcul, CRcTex* _pBuffer);
	int&			Get_MonsterType() { return m_iMonsterIndex; }
	CharacterInfo&	Get_InfoRef();
	void			Get_MonsterToPlayer_Distance(float* _Distance);

protected:
	CCharacterInfo*		m_pInfoCom = nullptr;    // 남길것 
	CAnimation*			m_pAnimationCom = nullptr;	 // 남길것 
	CDynamic_Transform* m_pDynamicTransCom = nullptr;

	int m_iMonsterIndex;
	bool m_bHit = false;
	float m_fHitDelay;
	int m_iPreHp;

	vector<CGameObject*> m_vecBlood;
protected:
	virtual HRESULT				Add_Component(void);
	virtual bool				Set_TransformPositon(HWND g_hWnd, CCalculator* _pCalcul);

public:
	virtual void		Free(void);
};

END