#include "Sound_Manager.h"
#include <io.h>
#include <tchar.h>
#include <filesystem>
#include "GameInstance.h"
#include "Utils.h"

namespace fs = std::filesystem;

IMPLEMENT_SINGLETON(CSound_Manager)

CSound_Manager::CSound_Manager()
	: m_pSystem(nullptr)
{

}

CSound_Manager::~CSound_Manager()
{
	Free();
}

HRESULT CSound_Manager::Reserve_Manager()
{
	FMOD_System_Create(&m_pSystem);
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

	for (_uint i = 0; i < MAXCHANNEL; ++i)
	{
		m_fSoundVolumeArr[i] = 1.f;
	}
	
	const string topLevelPath = "../../Client/Bin/Resources/Sound/";

	if (fs::exists(topLevelPath) && fs::is_directory(topLevelPath)) 
		Search_Recursive(topLevelPath);

	/*Load_SoundFile("../../Client/Bin/Resources/Sound/Bgm/");
	Load_SoundFile("../../Client/Bin/Resources/Sound/Battle/");
	Load_SoundFile("../../Client/Bin/Resources/Sound/Foot/");
	Load_SoundFile("../../Client/Bin/Resources/Sound/Skill/");
	Load_SoundFile("../../Client/Bin/Resources/Sound/Slash/");
	Load_SoundFile("../../Client/Bin/Resources/Sound/UI/");
	Load_SoundFile("../../Client/Bin/Resources/Sound/Voice/");*/
	

	return S_OK;
}

void CSound_Manager::Tick(_float fTimeDelta)
{
	if (m_tBgmVloumeDesc.bActive)
		Tick_BgmBlending(fTimeDelta);
}


void CSound_Manager::Play_Sound(wstring pSoundKey, CHANNELID eID, _float fVolume, _bool bStop, _float fCamDistance)
{
	map<wstring, FMOD_SOUND*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
		{
			return pSoundKey == iter.first;
		});

	if (iter == m_mapSound.end())
		return;

	if (true == bStop)
		Stop_Sound(eID);

	_float fFinalVolume = fVolume * m_fSoundVolumeArr[eID] * m_fAllChannelVolume;
	if (eID == CHANNELID::SOUND_VOICE_NPC 
		|| eID == CHANNELID::SOUND_VOICE_MONSTER1 
		|| eID == CHANNELID::SOUND_VOICE_MONSTER2 
		|| eID == CHANNELID::SOUND_VOICE_MONSTER3 
		|| eID == CHANNELID::SOUND_FOOT_MONSTER
		|| eID == CHANNELID::SOUND_VOICE_ANIMAL
		|| eID == CHANNELID::SOUND_VOICE_ANIMAL2
		|| eID == CHANNELID::SOUND_VOICE_ANIMAL3)
	{
		if (fCamDistance > 10.f)		
			fFinalVolume = 0.f;
		else
			fFinalVolume *= 1.f - (fCamDistance / 10.f);
	}

	fFinalVolume = min(fFinalVolume, 1.f);
	FMOD_BOOL bPlay = FALSE;

	if (eID == CHANNELID::SOUND_FOOT_CHARACTER || eID == CHANNELID::SOUND_FOOT_MONSTER || eID == CHANNELID::SOUND_FOOT_VEHICLE)
	{
		if (GI->Get_CurrentLevel() != 99)
		{
			Play_Foot(eID, fFinalVolume);
			return;
		}
	}

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[eID]);
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fFinalVolume);
	FMOD_System_Update(m_pSystem);
}


void CSound_Manager::Play_BGM(wstring pSoundKey, _float fVolume, _bool bStop, _float fFadeDuration)
{
	/* 만약 사운드 스탑 페이드가 진행되고 있다면 false */
	if (m_bStopBgm)
		m_bStopBgm = false;

	/* Set File */
	{
		map<wstring, FMOD_SOUND*>::iterator iter;

		iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
			{
				return pSoundKey == iter.first;
			});

		if (iter == m_mapSound.end())
			return;

		m_pNextBgmKey = iter->second;
	}

	/* Set Volume */
	{
		m_fBgmFadeDuration = fFadeDuration;

		_float fTargetVolume = 0.f;

		if (!m_bInitBgm) /* 최초 실행 (페이드 인 시작) */
		{
			m_bInitBgm = true;

			fTargetVolume = fVolume * m_fSoundVolumeArr[SOUND_BGM_CURR] * m_fAllChannelVolume;

			m_tBgmVloumeDesc.Start(fTargetVolume, m_fBgmFadeDuration, LERP_MODE::EASE_IN);

			fTargetVolume = 0.f;
			FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, m_pNextBgmKey, FALSE, &m_pChannelArr[SOUND_BGM_CURR]);
		}
		else /* 브금 체인지 */
		{
			m_fNextBgmVolume = fVolume * m_fSoundVolumeArr[SOUND_BGM_CURR] * m_fAllChannelVolume;

			if (bStop || fFadeDuration <= 0.001f) /* 페이드 없이 바로 변경*/
			{
				FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, m_pNextBgmKey, FALSE, &m_pChannelArr[SOUND_BGM_CURR]);
				fTargetVolume = m_fNextBgmVolume;
			}
			else if (m_tBgmVloumeDesc.fCurValue < 0.01f) /* (페이드 아웃) 이전에 아무 브금이 없는 상태*/
			{
				m_tBgmVloumeDesc.Start(m_fNextBgmVolume, m_fBgmFadeDuration, LERP_MODE::EASE_IN);
				fTargetVolume = 0.f;

				FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, m_pNextBgmKey, FALSE, &m_pChannelArr[SOUND_BGM_CURR]); /* 바로 브금 변경 */
				m_pNextBgmKey = nullptr;
			}
			else /* (페이드 아웃) 이전에 브금이 있던 상태*/
			{
				m_tBgmVloumeDesc.Start(0.f, m_fBgmFadeDuration, LERP_MODE::EASE_IN);
				fTargetVolume = m_tBgmVloumeDesc.fCurValue;
			}
		}
		FMOD_Channel_SetVolume(m_pChannelArr[SOUND_BGM_CURR], fTargetVolume);
	}

	FMOD_Channel_SetMode(m_pChannelArr[SOUND_BGM_CURR], FMOD_LOOP_NORMAL);
	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::Stop_Sound(CHANNELID eID, const _float fFadeOutDuration)
{
	if (SOUND_BGM_CURR == eID && 0.01f < fFadeOutDuration)
	{
		/* 페이드 아웃 시작*/
		m_tBgmVloumeDesc.Start(0.f, fFadeOutDuration, LERP_MODE::SMOOTHER_STEP);
		m_bStopBgm = true;
	}
	else
		FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSound_Manager::Stop_All()
{
	for (int i = 0; i < MAXCHANNEL; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSound_Manager::Set_ChannelVolume(CHANNELID eID, float fVolume)
{
	m_fSoundVolumeArr[eID] = fVolume;
	FMOD_Channel_SetVolume(m_pChannelArr[eID], m_fSoundVolumeArr[eID] * m_fAllChannelVolume);
	FMOD_System_Update(m_pSystem);
}

_float CSound_Manager::Get_ChannelVolume(CHANNELID eID)
{
	if (CHANNELID::MAXCHANNEL < eID)
		return 0.f;

	return 	m_fSoundVolumeArr[eID];;
}

const _int CSound_Manager::Get_SoundFileIndex(const wstring& pSoundKey)
{
	_int iIndex = 0;
	for (auto& Pair : m_mapSound)
	{
		if (Pair.first.find(pSoundKey) != wstring::npos)
			return iIndex;
		else
			iIndex++;
	}

	return -1;
}

wstring CSound_Manager::Get_SoundFileKey(const _uint iIndex)
{
	_int iKeyIndex = 0;
	for (auto& Pair : m_mapSound)
	{
		if (iIndex == iKeyIndex)
			return Pair.first;
		else
			iKeyIndex++;
	}

	return L"";
}

wstring CSound_Manager::Get_SoundFileKey(string strKey)
{
	for (auto& Pair : m_mapSound)
	{
		if (CUtils::ToString(Pair.first) == strKey)
			return Pair.first;
	}
	return L"";
}

void CSound_Manager::Search_Recursive(const std::string& currentPath)
{
	for (const auto& entry : fs::directory_iterator(currentPath)) 
	{
		if (fs::is_directory(entry)) // 최상위 경로에서 모든 하위 폴더를 탐색한다.
		{
			Search_Recursive(entry.path().string());
		}
		else if (fs::is_regular_file(entry)) // 파일이라면 사운드 리소스를 만든다. 
		{
			char szFullPath[256] = "";
			strcpy_s(szFullPath, sizeof(char) * 128, entry.path().string().c_str());

			std::string fileName = fs::path(szFullPath).filename().stem().string() + fs::path(szFullPath).extension().string();

			FMOD_SOUND* pSound = nullptr;

			FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, 0, &pSound);

			if (eRes == FMOD_OK)
			{
				int iLength = fileName.length() + 1;

				wstring pSoundKey = CUtils::ToWString(fileName);

				m_mapSound.emplace(pSoundKey, pSound);
			}
		}
	}
}

void CSound_Manager::Play_Foot(CHANNELID eID, _float fVolume)
{
	map<wstring, FMOD_SOUND*>::iterator iter;

	wstring strSoundKey = L"";

	if (GI->Get_CurrentLevel() == 5)
	{
		strSoundKey = L"PC_FootPrint_Dirt_1_5.mp3";
	}

	else if (GI->Get_CurrentLevel() == 6)
	{
		strSoundKey = L"PC_FootPrint_Dirt_1_5.mp3";
	}

	else if (GI->Get_CurrentLevel() == 7)
	{
		strSoundKey = L"PC_FootPrint_Dirt_1_5.mp3";
	}

	else if (GI->Get_CurrentLevel() == 8)
	{
		strSoundKey = L"PC_FootPrint_Dirt_1_5.mp3";
	}

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
		{
			return strSoundKey == iter.first;
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[eID]);
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);
	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::Tick_BgmBlending(_float fTimeDelta)
{		
	m_tBgmVloumeDesc.Update(fTimeDelta);

	if (!m_tBgmVloumeDesc.bActive)
	{
		if (m_bStopBgm) /* Bgm Stop의 경우 */
			m_bStopBgm = false;
		else if (m_tBgmVloumeDesc.fCurValue < 0.1f) /* Bgm Change 페이드 아웃 종료*/
		{
			if (nullptr != m_pNextBgmKey)
			{
				/* 페이드 인 시작 */
				m_tBgmVloumeDesc.Start(m_fNextBgmVolume, m_fBgmFadeDuration, LERP_MODE::EASE_IN);

				/* 다음 bgm 세팅 */
				FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, m_pNextBgmKey, FALSE, &m_pChannelArr[SOUND_BGM_CURR]);
				FMOD_System_Update(m_pSystem);

				m_pNextBgmKey = nullptr;
			}
		}
	}

	FMOD_Channel_SetVolume(m_pChannelArr[SOUND_BGM_CURR], m_tBgmVloumeDesc.fCurValue);
}

//void CSound_Manager::Load_SoundFile(const char* szSoundFilePath)
//{
//	_finddata_t fd;
//
//	string Path = szSoundFilePath;
//	string Exe = "*.*";
//	string FullPath = Path + Exe;
//
//	// _findfirst : <io.h>에서 제공하며 사용자가 설정한 경로 내에서 가장 첫 번째 파일을 찾는 함수
//	intptr_t handle = _findfirst(FullPath.c_str(), &fd);
//
//	if (handle == -1)
//		return;
//
//	int iResult = 0;
//
//	char	szCurPath[128];
//	strcpy_s(szCurPath, sizeof(char) * 128, Path.c_str());
//
//	char szFullPath[128] = "";
//
//	while (iResult != -1)
//	{
//		strcpy_s(szFullPath, szCurPath);
//
//		// "../ Sound/Success.wav"
//		strcat_s(szFullPath, fd.name);
//		
//		FMOD_SOUND* pSound = nullptr;
//
//		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, 0, &pSound);
//
//		if (eRes == FMOD_OK)
//		{
//			int iLength = strlen(fd.name) + 1;
//
//			wstring pSoundKey = new TCHAR[iLength];
//			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);
//
//			// 아스키 코드 문자열을 유니코드 문자열로 변환시켜주는 함수
//			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);
//
//			m_mapSound.emplace(pSoundKey, pSound);
//		}
//		//_findnext : <io.h>에서 제공하며 다음 위치의 파일을 찾는 함수, 더이상 없다면 -1을 리턴
//		iResult = _findnext(handle, &fd);
//	}
//
//	FMOD_System_Update(m_pSystem);
//	_findclose(handle);
//}

void CSound_Manager::Free()
{
	for (auto& Mypair : m_mapSound)
	{
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}
