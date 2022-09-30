#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef	struct tagVertexColor
	{
		_vec3		vPos;
		_ulong		dwColor;

	}VTXCOL;

	const _ulong		FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;


	typedef struct tagVertexTex
	{
		_vec3		vPos;
		_vec3		vNormal;
		_vec2		vTexUV;

	}VTXTEX;

	const _ulong		FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexCubeTex
	{
		_vec3		vPos;
		_vec3		vTexUV;

	}VTXCUBE;

	const _ulong		FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);		// D3DFVF_TEXCOORDSIZE3 : �ؽ�ó�� UV���� FLOAT �� 3���� ũ�⸸ŭ�̸�, ��ȣ���� ���� 0�� �ǹ̴� ���� ���ؽ��� �ؽ�ó UV ���� �������� �� �� �ִµ� ���� 0��° ���� ���� �����ϰڴٴ� �ǹ��̴�.

	typedef	struct tagIndex16
	{
		_ushort			_0;
		_ushort			_1;
		_ushort			_2;

	}INDEX16;

	typedef	struct tagIndex32
	{
		_ulong		_0;
		_ulong		_1;
		_ulong		_2;

	}INDEX32;

	

	typedef struct tagCharacterInfo
	{
		int _iHp;
		int _iAttackPower;
		int _iCoin;
		int _iKey;
		float _fSpeed;
	}CharacterInfo;


	typedef struct tagParticleInfo
	{
		_vec3		_position;
		D3DCOLOR	_color;
		_float		_size;
		//_vec3		vVelocity;
		//_bool		bDead;
	}ParticleInfo;

	const _ulong  FVF_Particle = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	struct Attribute
	{
		D3DXVECTOR3 _position;
		D3DXVECTOR3 _velocity;
		D3DXVECTOR3 _acceleration;
		float		_liftTime;
		float		_age;
		D3DXCOLOR	_color;
		D3DXCOLOR	_colorFade;
		bool		_isAlive;
	};
	struct BDBOX
	{
		_vec3 vMin;
		_vec3 vMax;
	};
}



#endif // Engine_Struct_h__
