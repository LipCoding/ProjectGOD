#include "SoundManager.h"
#include "PathManager.h"
#include "../Component/Transform.h"

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

	SAFE_RELEASE(m_pPlayerTr);
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

	//
	m_strNowPlayed = strKey;

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

bool SoundManager::Update(float fTime)
{
	if (m_bChangeSoundCheck == true)
	{
		if (m_bSwapSoundCheck == false)
		{
			if (m_fVolume <= 0.f)
			{
				m_fVolume = 0.f;
				m_bSwapSoundCheck = true;
				Play(m_strSoundName, m_eNowChannel);
			}
			else
			{
				m_fVolume -= 25.f * fTime;
			}
		}
		else
		{
			if (m_fVolume >= 100.f)
			{
				m_fVolume = 100.f;
				m_bSwapSoundCheck = false;
				m_bChangeSoundCheck = false;
			}
			else
			{
				m_fVolume += 15.f * fTime;
			}
		}
	}
	else
	{
		if (m_pPlayerTr != nullptr)
		{
			for (auto& area : m_mapSoundArea)
			{
				Vector3 vPlayerPos = m_pPlayerTr->GetWorldPos();
				vPlayerPos.y = 0.f;
				Vector3 vAreaCenterPos = area.second.vCenterPos;
				vAreaCenterPos.y = 0.f;

				if (vPlayerPos.Distance(vAreaCenterPos) < area.second.m_fRange)
				{
					if (area.first != m_strNowPlayed)
						ChangeSound_Smooth(area.first);
				}
			}
		}
	}

	Volume(m_eNowChannel, m_fVolume / VolumeMax);

	return true;
}

void SoundManager::SetPlayerTr(CTransform * pTr)
{
	m_pPlayerTr = pTr;
}

void SoundManager::AddSoundArea(const string & musicName, const Vector3 & center, const float & range)
{
	SoundChangeArea area;
	area.vCenterPos = center;
	area.m_fRange = range;
	m_mapSoundArea.insert(make_pair(musicName, area));
}

void SoundManager::EraseSoundArea()
{
	m_mapSoundArea.clear();
}

bool SoundManager::ChangeSound_Smooth(const string & strKey)
{
	unordered_map<string, PSOUNDINFO>::iterator iter = m_mapSound.find(strKey);

	if (iter == m_mapSound.end())
		return false;

	m_strSoundName = strKey;
	m_bChangeSoundCheck = true;

	return true;
}

PSOUNDINFO SoundManager::FindSound(const string & strKey)
{
	unordered_map<string, PSOUNDINFO>::iterator	iter = m_mapSound.find(strKey);

	if (iter == m_mapSound.end())
		return NULL;

	return iter->second;
}
