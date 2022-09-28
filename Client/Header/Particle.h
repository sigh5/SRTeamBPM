#pragma once
#include "GameObject.h"


static _uint ftoDw(_float f)
{
	return *((_uint*)&f);
}

_float GetRandomFloat(_float  lowBound, _float highBound)
{
	if (lowBound >= highBound)
		return lowBound;

	_float f = (rand() % 10000)* 0.0001f;

	return (f*(highBound - lowBound) + lowBound);
}

void GetRandomVector(_vec3 *out, _vec3* _min, _vec3* _max)
{
	out->x = GetRandomFloat(_min->x, _max->x);
	out->y = GetRandomFloat(_min->y, _max->y);
	out->z = GetRandomFloat(_min->z, _max->z);
}

struct BoundingBox
{
	_vec3		_min;
	_vec3		_max;


	bool isPointInsind(_vec3 p)
	{
		if (p.x >= _min.x && p.y >= _min.y && p.z >= _min.z &&
			p.x <= _max.x && p.y <= _max.y && p.z <= _max.z)
			return true;
		else
			return false;
	}

	BoundingBox()
	{
		_min.x =  INFINITY;
		_min.y =  INFINITY;
		_min.z = INFINITY;
		_max.x =INFINITY;
		_max.y = INFINITY;
		_max.z = INFINITY;
	}

};

struct Attribute
{
	_vec3	 _position;
	_vec3	 _velocity;
	_vec3	 _acceleration;
	float lifeTime;
	float age;
	D3DXCOLOR color;
	D3DXCOLOR colorFade;
	bool isAlive;

};



namespace Engine
{
	class CTransform;
	class CRcTex;
	class CTexture;
	class CDynamic_Transform;
}



class CParticle :public CGameObject
{
private:
	explicit CParticle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CParticle();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;


private:

	void	Test_Snow(BoundingBox* boundingBox, int numPartincles);
	void	reset()
	{
		list<Attribute>::iterator i;
		for (i = particles.begin(); i != particles.end(); i++)
			resetParticle(&(*i));
	}	
	
	void resetParticle(Attribute* attribute)
	{
		attribute->isAlive = true;
		attribute->_position = _origin;
	}

	void addParticle()
	{
		Attribute attribute;

		resetParticle(&attribute);

		particles.push_back(attribute);
	}

	void removeDeadParicles()
	{
		list<Attribute>::iterator i;
		i = particles.begin();

		while (i != particles.end())
		{
			if (i->isAlive == false)
			{
				i = particles.erase(i);
			}
			else
				++i;
		}


	}



private:
	HRESULT				Add_Component(void);
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;

	_float				m_fFrame = 0.f;


	// For Snow
	
	_vec3				_origin;
	BoundingBox			_boundingBox;
	_float				_size;
	_float				_emitRate;
	DWORD				_vbSize;
	DWORD				_vbOffset;
	DWORD				_vbBatchSize;
	std::list<Attribute> particles;
	int maxParticles;
public:
	static CParticle*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);


};

