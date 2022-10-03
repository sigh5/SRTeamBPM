#include "..\..\Header\PSystem.h"

USING(Engine)

//const DWORD PARTICLE::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

CPSystem::CPSystem(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_origin({ 0.f,0.f,0.f })
	, m_fEmitRate(0.f)
	, m_fSize(0.f)
	//, m_pTextureCom		(nullptr)
	, m_Tex(nullptr)
	, m_vb(nullptr)
	, m_maxParticles(0)
	, m_vbSize(0)
	, m_vbOffset(0)
	, m_vbBatchSize(0)
{
}

CPSystem::CPSystem(const CPSystem & rhs)
	: CGameObject(rhs)
	, m_origin(rhs.m_origin)
	, m_fEmitRate(rhs.m_fEmitRate)
	, m_fSize(rhs.m_fSize)
	//, m_pTextureCom		(rhs.m_pTextureCom)
	, m_Tex(rhs.m_Tex)
	, m_vb(rhs.m_vb)
	, m_maxParticles(rhs.m_maxParticles)
	, m_vbSize(rhs.m_vbSize)
	, m_vbOffset(rhs.m_vbOffset)
	, m_vbBatchSize(rhs.m_vbBatchSize)
{
}

CPSystem::~CPSystem()
{

}

HRESULT CPSystem::Ready_Object(void)
{
	// D3D��ġ ������ �ʱ�ȭ �۾��� ó��
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(
		m_vbSize * sizeof(ParticleInfo),	// ���ؽ� ���� ũ��
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		// _DYNAMIC : ���� ���ؽ����۸� ����Ѵ�.
		// _POINTS	: ���ؽ� ���۰� ����Ʈ ��������Ʈ�� ������ ������ ����
		FVF_Particle,
		D3DPOOL_DEFAULT,
		&m_vb,
		0), E_FAIL);

	return S_OK;
}

_int CPSystem::Update_Object(const _float & fTimeDelta)
{

	return CGameObject::Update_Object(fTimeDelta);
}

void CPSystem::LateUpdate_Object(void)
{
}

void CPSystem::Render_Obejct(void)
{
}

void CPSystem::Free(void)
{
	for (auto iter = m_particles.begin(); iter != m_particles.end();)
	{
		iter = m_particles.erase(iter);
	}
	m_particles.clear();

	Safe_Release(m_vb);
	Safe_Release(m_Tex);

	CGameObject::Free();
}

_bool CPSystem::init(IDirect3DDevice9 * pGraphicDev, const _tchar * texFileName)
{
	if (D3DXCreateTextureFromFile(m_pGraphicDev,
		texFileName,
		&m_Tex))
	{
		MSG_BOX("particle texture file failed");
		return false;

	}

	return true;


}

void CPSystem::reset()
{
	// ��� ��ƼŬ �Ӽ��� ����
	for (auto& iter = m_particles.begin(); iter != m_particles.end(); ++iter)
	{
		resetParticle(&(*iter));
	}
}

void CPSystem::addParticle()
{
	// �ý��ۿ� ��ƼŬ�� �߰�.
	Attribute tAttribute;

	resetParticle(&tAttribute);

	m_particles.push_back(tAttribute);
}

void CPSystem::preRender()
{
	// �������� �ռ� ������ �ʱ� ���� ����
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, false);

	// ���� ������ ��ü �ؽ�ó ����Ʈ ��������Ʈ�� �ؽ�ó ���ο� �̿��� ����
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, true);

	// ����Ʈ ũ�⸦ �佺���̽� ������ �ؼ��� ���ΰ�?
	// �佺���̽� ���� : ī�޶� ������ 3D ����Ʈ. ī�޶���� �Ÿ��� ���� ũ�Ⱑ ������.
	// false : ��ũ�������̽� ����
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, true);

	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE, FtoDw(m_fSize));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));

	// �Ÿ�(����Ʈ�� ī�޶� ���� �Ÿ�)�� ���� ��ƼŬ ũ�� ����
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));

	// �ؽ�ó�� ���ĸ� �̿��Ѵ�.
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x50);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

}

void CPSystem::render()
{
	if (!m_particles.empty())
	{
		// ���� ���� ����
		preRender();


		//m_pGraphicDev->SetTexture(0, m_Tex);
		m_pGraphicDev->SetFVF(FVF_Particle);
		m_pGraphicDev->SetStreamSource(0, m_vb, 0, sizeof(ParticleInfo));

		// ���ؽ� ���۸� ��� ��� ó������ �����Ѵ�.
		if (m_vbOffset >= m_vbSize)
			m_vbOffset = 0;

		ParticleInfo* v = 0;

		m_vb->Lock(
			m_vbOffset		* sizeof(ParticleInfo),
			m_vbBatchSize	* sizeof(ParticleInfo),
			(void**)&v,
			m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD numParticlesInBatch = 0;

		// ��� ��ƼŬ�� ������ �ɶ�����
		for (auto iter = m_particles.begin(); iter != m_particles.end(); ++iter)
		{
			if (iter->_isAlive)
			{
				// �Ѵܰ��� ������ ��ƼŬ�� ���� ���ؽ� ���� ���׸�Ʈ�� ����
				v->_position = iter->_position;
				v->_color = (D3DCOLOR)iter->_color;
				v++; // next element;

				numParticlesInBatch++; // �ܰ� ī���͸� ������Ų��.

									   // ���� �ܰ谡 ��� ä�����ִ°�?
				if (numParticlesInBatch == m_vbBatchSize)
				{
					// ���ؽ� ���۷� ����� ������ �ܰ��� ��ƼŬ���� �׸���.
					m_vb->Unlock();

					m_pGraphicDev->DrawPrimitive(
						D3DPT_POINTLIST,
						m_vbOffset,
						m_vbBatchSize);


					// �ܰ谡 �׷����� ���� ���� �ܰ踦 ��ƼŬ�� ä���.

					// ���� �ܰ��� ó�� ���������� �̵��Ѵ�.
					m_vbOffset += m_vbBatchSize;

					// ���ؽ� ������ ��踦 �Ѵ� �޸𸮷� �������� �������� �����´�.
					// ��踦 ���� ��� ó������ ����
					if (m_vbOffset >= m_vbSize)
						m_vbOffset = 0;

					m_vb->Lock(
						m_vbOffset		* sizeof(ParticleInfo),
						m_vbBatchSize	* sizeof(ParticleInfo),
						(void**)&v,
						m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

					numParticlesInBatch = 0; // �����ܰ踦 ���� ����
				}
			}
		}

		m_vb->Unlock();

		// numParticleInBatch == vbBatchSize ������ �������� �ʾ�
		// �������ܰ谡 ���������� �ʴ� ��찡 �߻��� �� ����.
		// �Ϻθ� ä���� �ܰ�� �̰����� ��������.
		if (numParticlesInBatch)
		{
			m_pGraphicDev->DrawPrimitive(
				D3DPT_POINTLIST,
				m_vbOffset,
				numParticlesInBatch);
		}

		// ��������
		m_vbOffset += m_vbBatchSize;

		postRender();
	}
}

void CPSystem::postRender() // �������¸� �����ϱ� ���� �ʿ��� �Լ�
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, false);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, false);



}

_bool CPSystem::isEmpty()
{
	return m_particles.empty();
}

_bool CPSystem::isDead()
{
	for (auto& pt : m_particles)
	{
		if (true == pt._isAlive)
		{
			return false;
		}
	}

	return true;
}

void CPSystem::removeDeadParticles()
{
	for (auto iter = m_particles.begin(); iter != m_particles.end();)
	{
		if (false == iter->_isAlive)
			m_particles.erase(iter);
		else
			++iter;
	}
}
