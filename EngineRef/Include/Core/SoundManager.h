#pragma once

#include "../PGEngine.h"

enum SOUND_CHANNEL
{
	SC_BGM,
	SC_EFFECT,
	SC_MONSTER,
	SC_MAX,
};

typedef struct _tagSoundInfo
{
	Sound* pSound;
	bool bLoop;

}SOUNDINFO, *PSOUNDINFO;

PG_BEGIN

const float VolumeMax = 100.f;

class PG_DLL SoundManager
{
private:
	System*		m_pSystem;
	Channel*	m_pChannel[SC_MAX];
	unordered_map<string, PSOUNDINFO>	m_mapSound;

public:
	bool Init();
	bool LoadSound(const string& strKey, bool bLoop, char* pFileName, const string& strPathKey = SOUND_PATH);
	bool Play(const string& strKey, SOUND_CHANNEL _Channel);
	bool PlayWithDelay(const string& strKey, SOUND_CHANNEL _Channel, const float& delay);
	bool Stop(SOUND_CHANNEL eChannel);
	bool Volume(SOUND_CHANNEL eChannel, float fVolume);

public:
	bool Update(float fTime);

public:
	struct SoundChangeArea
	{
		Vector3 vCenterPos;
		float m_fRange;
	};

	struct DelaySound
	{
		string strDelayedSoundName;
		SOUND_CHANNEL eDelayedChannel;
		float fDelay;
		float fMaxDelay;
	};

	void SetPlayerTr(class CTransform* pTr);
	void AddSoundArea(const string& musicName, const Vector3& center, const float& range);
	void EraseSoundArea();

private:
	bool ChangeSound_Smooth(const string& strKey);

private:
	unordered_map<string, SoundChangeArea>	m_mapSoundArea;
	list<DelaySound> m_listDelaySound;

	class CTransform *m_pPlayerTr = nullptr;

	float m_fVolume = 100.f;
	SOUND_CHANNEL m_eNowChannel = SC_BGM;
	
	bool m_bChangeSoundCheck = false;
	bool m_bSwapSoundCheck = false;
	string m_strSoundName = "";
	string m_strNowPlayed = "";

private:
	PSOUNDINFO FindSound(const string& strKey);

	DECLARE_SINGLE(SoundManager)
};

PG_END