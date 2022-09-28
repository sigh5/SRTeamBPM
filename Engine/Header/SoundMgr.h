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

	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];

	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem;

	virtual void Free()override;
};

END