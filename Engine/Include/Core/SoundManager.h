#pragma once

#include "../PGEngine.h"

enum SOUND_CHANNEL
{
	SC_BGM,
	SC_EFFECT,
	SC_MAX,
};

typedef struct _tagSoundInfo
{
	Sound* pSound;
	bool bLoop;

}SOUNDINFO, *PSOUNDINFO;

PG_BEGIN

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
	bool Stop(SOUND_CHANNEL eChannel);
	bool Volume(SOUND_CHANNEL eChannel, float fVolume);

private:
	PSOUNDINFO FindSound(const string& strKey);

	DECLARE_SINGLE(SoundManager)
};

PG_END