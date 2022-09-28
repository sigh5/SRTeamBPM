#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CCubeTex;

END

class CMonsterBullet :public CGameObject
{
private:
	explicit CMonsterBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonsterBullet();

public:
	HRESULT				Ready_Object(_vec3 vPos);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

private:
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCubeTex*			m_pCubeTexCom = nullptr;

private:
	_float				m_fFrame = 0.f;
	_vec3				m_vBulletDir;
										// ���Ŀ� ī�޶� ��ǥ ���� ���� ����
										//_vec3				m_vCamLook;

										// 3 Sec after, Bullet is Die.
	_float				m_fDeadTime;
	// ~3 Sec after, Bullet is Die.

public:
	static CMonsterBullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void		Free(void);
};

