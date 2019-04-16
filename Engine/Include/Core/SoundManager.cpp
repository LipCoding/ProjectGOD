#include "SoundManager.h"
#include "PathManager.h"

PG_USING

DEFINITION_SINGLE(SoundManager)

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
	unordered_map<string, PSOUNDINFO>::iterator	iter;
	for (iter = m_mapSound.begin(); iter != m_mapSound.end(); ++iter)
	{
		iter->second->pSound->release();
		SAFE_DELETE(iter->second);
	}

	m_pSystem->close();
	m_pSystem->release();
}

bool SoundManager::Init()
{
	// fmod system »ý¼º
	System_Create(&m_pSystem);

	m_pSystem->init(10, FMOD_INIT_NORMAL, 0);

	return true;
}

bool SoundManager::LoadSound(const string & strKey, bool bLoop, char * pFileName,
	const string & strPathKey)
{
	if (FindSound(strKey))
		return false;

	const char* pPath = GET_SINGLE(CPathManager)->FindPathToMultiByte(strPathKey);

	string	strPath;

	if (pPath)
		strPath = pPath;

	strPath += pFileName;

	FMOD_MODE	eMode = FMOD_LOOP_NORMAL;

	if (!bLoop)
		eMode = FMOD_DEFAULT;

	PSOUNDINFO	pSound = new SOUNDINFO;

	pSound->bLoop = bLoop;

	m_pSystem->createSound(strPath.c_str(), eMode, NULL, &pSound->pSound);

	m_mapSound.insert(make_pair(strKey, pSound));

	return true;
}

bool SoundManager::Play(const string & strKey, SOUND_CHANNEL _Channel)
{
	PSOUNDINFO	pSound = FindSound(strKey);

	if (!pSound)
		return false;

	m_pSystem->update();

	SOUND_CHANNEL	eChannel = SC_BGM;

	if (!pSound->bLoop)
	{
		eChannel = _Channel;
	}

	m_pSystem->playSound(pSound->pSound, NULL, false, &m_pChannel[eChannel]);

	return true;
}

bool SoundManager::Stop(SOUND_CHANNEL eChannel)
{
	m_pChannel[eChannel]->stop();

	return true;
}

bool SoundManager::Volume(SOUND_CHANNEL eChannel, float fVolume)
{
	m_pChannel[eChannel]->setVolume(fVolume);

	return true;
}

PSOUNDINFO SoundManager::FindSound(const string & strKey)
{
	unordered_map<string, PSOUNDINFO>::iterator	iter = m_mapSound.find(strKey);

	if (iter == m_mapSound.end())
		return NULL;

	return iter->second;
}
