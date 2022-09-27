#pragma once
#include "Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CSoundMgr : public CBase
{
	DECLARE_SINGLETON(CSoundMgr)

private:
	explicit CSoundMgr();
	virtual ~CSoundMgr();

public:
	void Initialize();
	void Release();

public:
	void PlaySoundW(TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void PlayBGM(TCHAR* pSoundKey, float fVolume);

	void StopSound(CHANNELID eID);
	void StopAll();

	void PlaySlow(TCHAR * pSoundKey, CHANNELID eID, float fVolume, float fRate);

	void SetChannelVolume(CHANNELID eID, float fVolume);

public:
	void LoadSoundFile();

private:
	map<TCHAR*, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];

	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem;

	virtual void Free()override;
};

END