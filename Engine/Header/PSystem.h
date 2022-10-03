#pragma once

#include "GameObject.h"
#include "Engine_Include.h"
// ��ƼŬ �ý���
// �ý��� �� ��� ��ƼŬ�� ������ �ִ� ���� Ư���� ����
// ��ƼŬ ����, ���÷���, �Ҹ�, ���� ���� ����

BEGIN(Engine)
class CTexture;
class CTransform;
class ENGINE_DLL CPSystem : public CGameObject
{
protected:
	explicit CPSystem(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPSystem(const CPSystem& rhs);
	virtual ~CPSystem();

public:
	virtual		HRESULT		Ready_Object(void);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Obejct(void);
	virtual		void		Free(void);

public:
	virtual _bool	init(IDirect3DDevice9* pGraphicDev, const _tchar * texFileName); // D3D ��ġ �������� �ʱ�ȭ �۾�
	virtual void	reset();									// �ý��� �� ��� ��ƼŬ �Ӽ� ����
	virtual void	resetParticle(Attribute* attribute) = 0;	// �� ��ƼŬ�� �Ӽ��� ����
	virtual void	addParticle();								// ��ƼŬ�� �߰�
	virtual void	update(_float fTimeDelta) = 0;				// ��ƼŬ ����

	virtual void	preRender();		// �������� �ռ� ������ �ʱ� ���� ����
	virtual void	render();			// ������
	virtual void	postRender();		// Ư�� ��ƼŬ �ý����� �������� �� �ִ� ���� ���¸� �����Ҷ� �̿�

public:
	_bool isEmpty();	// ���� �ý��ۿ� ��ƼŬ�� ������ True
	_bool isDead();	// �ý��� �� ��� ��ƼŬ�� ������� True

public:
	inline DWORD FtoDw(float f)
	{
		return *((DWORD*)&f);
	}
	inline _float GetRandomFloat(_float lowBound, _float highBound)
	{
		if (lowBound >= highBound) // �߸��� �Է�
			return lowBound;

		// [0,1] ������ ���� float �� ��´�.
		_float f = (rand() % 10000) * 0.0001f;

		// [lowBound, highBound] ������ float�� ����
		return (f* (highBound - lowBound)) + lowBound;
	}
	inline void GetRandomVector(_vec3* pOut, _vec3* pMin, _vec3* pMax)
	{
		pOut->x = GetRandomFloat(pMin->x, pMax->x);
		pOut->y = GetRandomFloat(pMin->y, pMax->y);
		pOut->z = GetRandomFloat(pMin->z, pMax->z);

	}


protected:
	virtual void removeDeadParticles();	// ���� ��ƼŬ�� ����Ʈ���� ����

protected:
	//IDirect3DDevice9*			m_pGarphicDev;
	_vec3						m_origin;			// �ý����� ��õ. ��ƼŬ�� ���۵Ǵ� ��
	BDBOX						m_bdBox;		// ��ƼŬ�� �̵��� �� �ִ� ���� ����
	_float						m_fEmitRate;		// �ý��ۿ� ���ο� ��ƼŬ�� �߰��Ǵ� ����. (����/s)
	_float						m_fSize;			// �ý��� �� ��� ��ƼŬ�� ũ��

	IDirect3DTexture9*			m_Tex;
	//CTexture*					m_pTextureCom;
	IDirect3DVertexBuffer9*		m_vb;

	list<Attribute>				m_particles;		// �ý��� �� ��ƼŬ �Ӽ� ����Ʈ
													// ��ƼŬ ����/����/���� �� �� ����Ʈ�� �̿�
													// ��ƼŬ�� �׸� �غ� �Ϸ�Ǹ� ����Ʈ ��� �Ϻθ�
													// ���ؽ� ���۷� �����ϰ�, ��ƼŬ ����� ������ ��ħ.
													// ��� ��ƼŬ�� �׸������� �� ������ �ݺ�.
	_int						m_maxParticles;		// �־��� �ð����� �ý����� ���� �� �ִ� �ִ� ��ƼŬ ��

	_ulong						m_vbSize;			// ���ؽ� ���۰� ������ �� �ִ� ��ƼŬ ��
													// ���� ��ƼŬ �ý��� �� ��ƼŬ ���ʹ� ������
	_ulong						m_vbOffset;			// �������� �̿��
													// ���ؽ� ���ۿ��� ���縦 ������ ��ƼŬ �� ���� �ܰ���� ������		
													// ����Ʈ�� �ƴ� ��ƼŬ �����̴�.
													// ex) �ܰ� 1�� 0���� 499���, �ܰ�2���� �������� 500
	_ulong						m_vbBatchSize;		// �������� �̿��
													// �ܰ� �ϳ��� ���ǵ� ��ƼŬ ��
};

END