#include "stdafx.h"
#include "Camera_CutScene_Map.h"

#include "GameInstance.h"

#include "Camera_Manager.h"
#include "Camera_Follow.h"

#include "UI_Manager.h"
#include "UI_Fade.h"

#include "Game_Manager.h"
#include "Character.h"
#include "Player.h"
#include "Whale.h"

CCamera_CutScene_Map::CCamera_CutScene_Map(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
	: CCamera(pDevice, pContext, strObjTag, OBJ_TYPE::OBJ_CAMERA)
{
}

CCamera_CutScene_Map::CCamera_CutScene_Map(const CCamera_CutScene_Map& rhs)
	: CCamera(rhs)
{

}

HRESULT CCamera_CutScene_Map::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_CutScene_Map::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Load_CutSceneDescs()))
		return E_FAIL;

	return S_OK;
}

void CCamera_CutScene_Map::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::Tick(fTimeDelta);

	/* Check Blending */
	if (m_bBlending)
	{
		Tick_Blending(fTimeDelta);
		return;
	}

	/* Fade */
	if (m_tFadeDesc.bReserved)
	{
		Tick_Fade(fTimeDelta);
		if (m_tFadeDesc.Outtro.bPlaying && m_tFadeDesc.Outtro.bFadeOutted)
		{
			m_tFadeDesc.Clear(); /* 페이드가 적용된 경우 컷신 카메라의 마지막 호출 부분 */
			return;
		}

		if (m_tFadeDesc.bPlaying)
		{
			if(m_tFadeDesc.Intro.bPlaying && !m_tFadeDesc.Intro.bFadeOutted)
				return;

			if (m_tFadeDesc.Outtro.bPlaying && !m_tFadeDesc.Outtro.bFadeOutted)
				return;
		}
	}

	/* 컷신 종료*/
	if (!m_tTimeDesc.bActive)
	{
		/* 예약된 컷신이 있다면 실행 */
		if (!m_CutSceneNamesReserved.empty())
		{
			string strCutSceneName = m_CutSceneNamesReserved.front();

			Start_CutScene(strCutSceneName, m_bWillRetruePrevCam);

			m_CutSceneNamesReserved.pop();
		}
		else /* 없다면 이전 카메라로 체인지 */
		{
			/* 이전 브금 볼륨으로 되돌린다.*/
			GI->Set_ChannelVolume(CHANNELID::SOUND_BGM_CURR, m_fBgmPrevVolume);

			if (m_tWhaleDesc.bSetTransform) /* 고래 컷신이었다면 */
			{
				CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
				if (nullptr != pFollowCam)
				{
					pFollowCam->Reset_WideView_To_DefaultView(true);
					pFollowCam->Set_Default_Position();
					CCamera_Manager::GetInstance()->Change_Camera(pFollowCam->Get_Key(), 1.5f, LERP_MODE::SMOOTHER_STEP);
				}
				return;
			}
			else if (m_bWillRetruePrevCam)
			{
				/* 이전 브금 볼륨으로 되돌린다.*/
				GI->Set_ChannelVolume(CHANNELID::SOUND_BGM_CURR, m_fBgmPrevVolume);

				/* 페이드가 예약되어 있다면 */
				if (m_tFadeDesc.bReserved)
				{
					CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, m_tFadeDesc.Outtro.fDuration, m_tFadeDesc.Outtro.bWhite);
					m_tFadeDesc.bPlaying = true;
					m_tFadeDesc.Outtro.bPlaying = true;
				}
				else
				{
					CCamera_Manager::GetInstance()->Set_PrevCamera();
				}
			}	

			return;
		}
	}
	
	/* 컷신 로케이션 갱신 */
	m_tTimeDesc.Update(fTimeDelta);

	const _float fRatio = m_tTimeDesc.fLerpTime / m_tTimeDesc.fEndTime;

	Vec4 vLookAt = Get_Point_In_Bezier(m_pCurCutSceneDesc->vCamLookAts, fRatio);
	Vec4 vCamPosition = Get_Point_In_Bezier(m_pCurCutSceneDesc->vCamPositions, fRatio);

	if (Is_Shake())
		vLookAt += Vec4(Get_ShakeLocalPos());

	m_vPrevLookAt = vLookAt.OneW();
	m_pTransformCom->LookAt(m_vPrevLookAt);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCamPosition.OneW());
}

void CCamera_CutScene_Map::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_CutScene_Map::Render()
{
	return S_OK;
}

HRESULT CCamera_CutScene_Map::Start_CutScene(const string& strCutSceneName, const _bool& bWillRetruePrevCam)
{
	m_pCurCutSceneDesc = Find_CutSceneDesc(strCutSceneName);
	if (nullptr == m_pCurCutSceneDesc)
		return E_FAIL;

	/* 페이드가 예약되어 있고, 여러 컷신 중 첫번째 컷신이라면 */
	if (m_tFadeDesc.bReserved && !m_tFadeDesc.bInit)
	{
		CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, m_tFadeDesc.Intro.fDuration, m_tFadeDesc.Intro.bWhite);
		m_tFadeDesc.bPlaying = true;
		m_tFadeDesc.Intro.bPlaying = true;
		m_tFadeDesc.bInit = true;

		/* 이전 카메라 세팅 그대로 가져온다. */
		/* 인트로 페이드 아웃이 끝날때까지는 이전 카메라 뷰로 진행해야하기 때문에 (현재 카메라는 컷신 카메라고, 뷰는 이전 카메라 뷰를 보여줘야함) */
		{
			CCamera* pPrevCam = CCamera_Manager::GetInstance()->Get_CurCamera();
			if (nullptr != pPrevCam)
			{
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, pPrevCam->Get_Transform()->Get_Position());
				m_pTransformCom->LookAt(pPrevCam->Get_LookAt());

				Set_Fov(pPrevCam->Get_Fov());
			}
			CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::CUTSCENE_MAP);
		}
	}
	else
	{
		if(LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
			CUI_Manager::GetInstance()->OnOff_GamePlaySetting(false);

		CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::CUTSCENE_MAP);

		m_tTimeDesc.Start(m_pCurCutSceneDesc->fDuration, m_pCurCutSceneDesc->eLerpMode);

		Set_Fov(Cam_Fov_CutScene_Map_Default);
	}

	m_bWillRetruePrevCam = bWillRetruePrevCam;
	
	/* Player Input Off */
	CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(false);

	/* 마우스 아이콘 숨기기 */
	CUI_Manager::GetInstance()->Hide_MouseCursor(true);

	return S_OK;
}

HRESULT CCamera_CutScene_Map::Start_CutScenes(vector<string> strCutSceneNames, const _bool& bWillRetruePrevCam)
{
	if (Is_Playing_CutScenc())
		return E_FAIL;

	for (auto& iter : strCutSceneNames)
	{
		if (nullptr != Find_CutSceneDesc(iter))
		{
			m_CutSceneNamesReserved.push(iter);
		}
	}

	if (!m_CutSceneNamesReserved.empty())
	{
		string strCutSceneName = m_CutSceneNamesReserved.front();

		Start_CutScene(strCutSceneName, bWillRetruePrevCam);

		m_CutSceneNamesReserved.pop();
	}
	return S_OK;
}

HRESULT CCamera_CutScene_Map::Stop_CutScene(const _bool& bClearReservedCutScene)
{
	if (bClearReservedCutScene)
	{
		do
		{
			m_CutSceneNamesReserved.pop();

		} while (m_CutSceneNamesReserved.empty());
	}

	return S_OK;
}

HRESULT CCamera_CutScene_Map::Start_CutScene(const LEVELID& eLevelID, const _uint& iType)
{
	/* Exception */
	const _uint iCurLevel = GI->Get_CurrentLevel();
	{
		if (eLevelID != iCurLevel && LEVELID::LEVEL_TOOL != iCurLevel)
			return E_FAIL;
	}

	switch (eLevelID)
	{
	case LEVELID::LEVEL_EVERMORE:
	{
		if (0 == iType)
		{
			/* Reserve Fade */
			Reserve_Fade(1.f, true, 1.f, true);

			/* CutScene - Evermore */
			vector<string> CutSceneNames;
			{
				CutSceneNames.push_back("Evermore_Intro_Short_Flowers");			
				CutSceneNames.push_back("Evermore_Intro_Short_MarketPlace");		
				CutSceneNames.push_back("Evermore_Intro_Short_Armys");				
				CutSceneNames.push_back("Evermore_Intro_Short_Cat");				
				CutSceneNames.push_back("Evermore_Intro_Short_People");				
				CutSceneNames.push_back("Evermore_Intro_Middle_Entire");			
			}
			Start_CutScenes(CutSceneNames, true);

			/* Sound */
			{
				GI->Play_Sound(TEXT("Evermore_CutScene.mp3"), CHANNELID::SOUND_CUTSCENE, 1.f, true);
				m_fBgmPrevVolume = GI->Get_ChannelVolume(CHANNELID::SOUND_BGM_CURR);
				GI->Set_ChannelVolume(CHANNELID::SOUND_BGM_CURR, m_fBgmPrevVolume * 0.6f);
			}
		}
		else if (1 == iType)
		{
			/* Reserve Fade */
			Reserve_Fade(1.f, false, 1.f, false);

			/* CutScene - Evermore */
			vector<string> CutSceneNames;
			{
				//CutSceneNames.push_back("Evermore_Invasion_00");
				CutSceneNames.push_back("Evermore_Invasion_01");
				CutSceneNames.push_back("Evermore_Invasion_04");
				CutSceneNames.push_back("Evermore_Invasion_02");
				CutSceneNames.push_back("Evermore_Invasion_03");
			}
			Start_CutScenes(CutSceneNames, true);

			/* Sound */
			{

			}
		}
		else if (2 == iType)
		{
			/* Reserve Fade */
			//Reserve_Fade(0.7f, true, 0.7f, true);

			/* CutScene - Evermore */
			vector<string> CutSceneNames;
			{
				CutSceneNames.push_back("Evermore_Ending_From_Back_00");
				CutSceneNames.push_back("Evermore_Ending_From_Back_01");
				CutSceneNames.push_back("Evermore_Ending_From_Front_00");
				CutSceneNames.push_back("Evermore_Ending_From_Front_01");

			}
			Start_CutScenes(CutSceneNames, true);

			///* Sound */
			//{
			//	GI->Play_Sound(TEXT("Evermore_CutScene.mp3"), CHANNELID::SOUND_CUTSCENE, 1.f, true);
			//	m_fBgmPrevVolume = GI->Get_ChannelVolume(CHANNELID::SOUND_BGM_CURR);
			//	GI->Set_ChannelVolume(CHANNELID::SOUND_BGM_CURR, m_fBgmPrevVolume * 0.6f);
			//}
		}
	}
		break;
	case LEVELID::LEVEL_KINGDOMHALL:
		break;
	case LEVELID::LEVEL_ICELAND:
		break;
	case LEVELID::LEVEL_WITCHFOREST:
	{
		/* Reserve Fade */
		Reserve_Fade(0.7f, true, 0.7f, true);

		/* CutScene - Evermore */
		vector<string> CutSceneNames;
		{
			CutSceneNames.push_back("Witchforest_Short_Shrub");		
			CutSceneNames.push_back("Witchforest_Short_Campfire");	
			CutSceneNames.push_back("Witchforest_Short_Town_In");			
			CutSceneNames.push_back("Witchforest_Short_Town_Ghosts");			
			CutSceneNames.push_back("Witchforest_Short_Town_Middle");		
			CutSceneNames.push_back("Witchforest_Short_Town_Out");			
		}
		Start_CutScenes(CutSceneNames, true);

		/* Sound */
		{
			/*GI->Play_Sound(TEXT("Evermore_CutScene.mp3"), CHANNELID::SOUND_CUTSCENE, 1.f, true);
			m_fBgmPrevVolume = GI->Get_ChannelVolume(CHANNELID::SOUND_BGM_CURR);
			GI->Set_ChannelVolume(CHANNELID::SOUND_BGM_CURR, m_fBgmPrevVolume * 0.6f);*/
		}

	}
		break;
	default:
		break;
	}
	return S_OK;
}

HRESULT CCamera_CutScene_Map::Save_CutSceneDescs()
{
	Json json;

	for (auto& CutSceneDesc : m_CutSceneDescs)
	{
		json["CutScene Desc"].push_back({

			{"Name", CutSceneDesc.strCutSceneName},
			
			{"Duration", CutSceneDesc.fDuration},

			{"Start Delay Time", CutSceneDesc.fStartDelayTime},
			{"Finish Delay Time", CutSceneDesc.fFinishDelayTime},
			
			{"Start Fov", CutSceneDesc.fStartFov },
			{"Finish Fov", CutSceneDesc.fFinishFov},

			{"Lerp Mode", (_uint)CutSceneDesc.eLerpMode},

			{"Position_0", {
						{"x", CutSceneDesc.vCamPositions[0].x},
						{"y", CutSceneDesc.vCamPositions[0].y},
						{"z", CutSceneDesc.vCamPositions[0].z}},
			},

			{"Position_1", {
						{"x", CutSceneDesc.vCamPositions[1].x},
						{"y", CutSceneDesc.vCamPositions[1].y},
						{"z", CutSceneDesc.vCamPositions[1].z}},
			},

			{"Position_2", {
						{"x", CutSceneDesc.vCamPositions[2].x},
						{"y", CutSceneDesc.vCamPositions[2].y},
						{"z", CutSceneDesc.vCamPositions[2].z}},
			},

			{"Position_3", {
						{"x", CutSceneDesc.vCamPositions[3].x},
						{"y", CutSceneDesc.vCamPositions[3].y},
						{"z", CutSceneDesc.vCamPositions[3].z}},
			},

			{"LookAt_0", {
						{"x", CutSceneDesc.vCamLookAts[0].x},
						{"y", CutSceneDesc.vCamLookAts[0].y},
						{"z", CutSceneDesc.vCamLookAts[0].z}},
			},

			{"LookAt_1", {
						{"x", CutSceneDesc.vCamLookAts[1].x},
						{"y", CutSceneDesc.vCamLookAts[1].y},
						{"z", CutSceneDesc.vCamLookAts[1].z}},
			},

			{"LookAt_2", {
						{"x", CutSceneDesc.vCamLookAts[2].x},
						{"y", CutSceneDesc.vCamLookAts[2].y},
						{"z", CutSceneDesc.vCamLookAts[2].z}},
			},

			{"LookAt_3", {
						{"x", CutSceneDesc.vCamLookAts[3].x},
						{"y", CutSceneDesc.vCamLookAts[3].y},
						{"z", CutSceneDesc.vCamLookAts[3].z}},
			},

		});
	}

	json.dump(2);

	wstring strPath = L"../Bin/DataFiles/Camera/CutScene/CutSceneData";
	GI->Json_Save(strPath + L".json", json);

	return S_OK;
}

HRESULT CCamera_CutScene_Map::Load_CutSceneDescs()
{
	wstring strPath = L"../Bin/DataFiles/Camera/CutScene/CutSceneData.json";
	auto path = filesystem::path(strPath);

	if (!filesystem::exists(strPath))
	{
		MSG_BOX("컷신 데이터 파일이 존재하지 않습니다.");
		return S_OK;
	}

	Json json = GI->Json_Load(strPath);

	for (const auto& item : json["CutScene Desc"])
	{
		CAMERA_CUTSCENE_MAP_DESC desc;
		
		desc.strCutSceneName	= item["Name"];
		desc.fDuration			= item["Duration"];
		desc.fStartDelayTime	= item["Start Delay Time"];
		desc.fFinishDelayTime	= item["Finish Delay Time"];
		desc.fStartFov			= item["Start Fov"];
		desc.fFinishFov			= item["Finish Fov"];
		desc.eLerpMode			= item["Lerp Mode"];

		desc.vCamPositions[0].x = item["Position_0"]["x"];
		desc.vCamPositions[0].y = item["Position_0"]["y"];
		desc.vCamPositions[0].z = item["Position_0"]["z"];

		desc.vCamPositions[1].x = item["Position_1"]["x"];
		desc.vCamPositions[1].y = item["Position_1"]["y"];
		desc.vCamPositions[1].z = item["Position_1"]["z"];

		desc.vCamPositions[2].x = item["Position_2"]["x"];
		desc.vCamPositions[2].y = item["Position_2"]["y"];
		desc.vCamPositions[2].z = item["Position_2"]["z"];

		desc.vCamPositions[3].x = item["Position_3"]["x"];
		desc.vCamPositions[3].y = item["Position_3"]["y"];
		desc.vCamPositions[3].z = item["Position_3"]["z"];

		desc.vCamLookAts[0].x = item["LookAt_0"]["x"];
		desc.vCamLookAts[0].y = item["LookAt_0"]["y"];
		desc.vCamLookAts[0].z = item["LookAt_0"]["z"];

		desc.vCamLookAts[1].x = item["LookAt_1"]["x"];
		desc.vCamLookAts[1].y = item["LookAt_1"]["y"];
		desc.vCamLookAts[1].z = item["LookAt_1"]["z"];

		desc.vCamLookAts[2].x = item["LookAt_2"]["x"];
		desc.vCamLookAts[2].y = item["LookAt_2"]["y"];
		desc.vCamLookAts[2].z = item["LookAt_2"]["z"];

		desc.vCamLookAts[3].x = item["LookAt_3"]["x"];
		desc.vCamLookAts[3].y = item["LookAt_3"]["y"];
		desc.vCamLookAts[3].z = item["LookAt_3"]["z"];

		m_CutSceneDescs.push_back(desc);
	}
	return S_OK;
}

HRESULT CCamera_CutScene_Map::Reserve_Fade(const _float& fIntroDuration, const _bool& bIntroWhite, const _float& fOuttroDuration, const _bool& bOuttroWhite)
{
	if (m_tFadeDesc.bReserved)
		m_tFadeDesc.Clear();

	m_tFadeDesc.bReserved = true;

	m_tFadeDesc.Intro.fDuration = fIntroDuration;
	m_tFadeDesc.Intro.bWhite = bIntroWhite;

	m_tFadeDesc.Outtro.fDuration = fOuttroDuration;
	m_tFadeDesc.Outtro.bWhite = bOuttroWhite;

	return S_OK;
}

HRESULT CCamera_CutScene_Map::Set_CutSceneTransform(const string& strCutSceneName)
{
	CAMERA_CUTSCENE_MAP_DESC* pCutSceneDesc = Find_CutSceneDesc(strCutSceneName);
	if (nullptr == pCutSceneDesc)
		return E_FAIL;
	
	m_vPrevLookAt = Get_Point_In_Bezier(pCutSceneDesc->vCamLookAts, 0.f);
	m_vPrevLookAt.w = 1.f;
	m_pTransformCom->LookAt(m_vPrevLookAt);

	Vec4 vCamPosition = Get_Point_In_Bezier(pCutSceneDesc->vCamPositions, 0.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCamPosition.OneW());

	/* 고래 컷신이라면 */
	if ("Winter_Whale" == strCutSceneName)
	{
		m_tWhaleDesc.bSetTransform = true;
		Set_Fov(Cam_Fov_CutScene_Map_Default);
	}

	return S_OK;
}

Vec4 CCamera_CutScene_Map::Get_Point_In_Bezier(Vec3 vPoints[MAX_BEZIER_POINT], const _float& fRatio)
{
	if (nullptr == vPoints)
		return Vec4::UnitW;

	const _float fNormalizedRatio = min(1.0f, max(0.0f, fRatio));

	const Vec3 v0 = Vec3::Lerp(vPoints[0], vPoints[1], fNormalizedRatio);
	const Vec3 v1 = Vec3::Lerp(vPoints[1], vPoints[2], fNormalizedRatio);
	const Vec3 v2 = Vec3::Lerp(vPoints[2], vPoints[3], fNormalizedRatio);

	const Vec3 r0 = Vec3::Lerp(v0, v1, fNormalizedRatio);
	const Vec3 r1 = Vec3::Lerp(v1, v2, fNormalizedRatio);

	return Vec3::Lerp(r0, r1, fNormalizedRatio);
}

const _bool CCamera_CutScene_Map::Is_LastCutScene()
{
	/* 현재 진행중인 컷신이 예약된 컷신중 마지막 것인지 여부를 반환한다. */
	if (!m_bActive)
		return false;

	if (m_CutSceneNamesReserved.empty())
		return true;
	else
		return false;
}

const _float CCamera_CutScene_Map::Get_Progress()
{
	/* 현재 진행중인 컷신의 진행률을 반환한다.  */
	return m_tTimeDesc.fLerpTime / m_tTimeDesc.fEndTime;
}

const _float CCamera_CutScene_Map::Get_RemainDuration()
{
	return m_tTimeDesc.fEndTime - m_tTimeDesc.fCurTime;
}

Vec4 CCamera_CutScene_Map::Get_LookAt()
{
	return m_vPrevLookAt;
}

void CCamera_CutScene_Map::Set_Blending(const _bool& bBlending)
{
	__super::Set_Blending(bBlending);

	if (!bBlending)
	{
		if (m_tWhaleDesc.bSetTransform && !m_tWhaleDesc.bStarted)
		{
			/* 배면 뒤집기 */
			CGameObject* pWhale = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_DYNAMIC, TEXT("Animal_Whale"));
			if (nullptr != pWhale) 
			{
				static_cast<CWhale*>(pWhale)->Set_Flip(true);

				m_tWhaleDesc.bStarted = true;
			}
		}
		else if (m_tWhaleDesc.bSetTransform && m_tWhaleDesc.bStarted)
		{
			m_tWhaleDesc.Clear();
		}
	}
}

HRESULT CCamera_CutScene_Map::Add_CutSceneDesc(const CAMERA_CUTSCENE_MAP_DESC& desc)
{
	if (nullptr != Find_CutSceneDesc(desc.strCutSceneName))
		return E_FAIL;

	m_CutSceneDescs.push_back(desc);

	return S_OK;
}

HRESULT CCamera_CutScene_Map::Del_CutSceneDesc(const string& strCutSceneName)
{
	if (nullptr == Find_CutSceneDesc(strCutSceneName))
		return E_FAIL;

	for (vector<CAMERA_CUTSCENE_MAP_DESC>::iterator iter = m_CutSceneDescs.begin(); iter != m_CutSceneDescs.end(); ++iter)
	{
		if (strCutSceneName == (*iter).strCutSceneName)
		{
			m_CutSceneDescs.erase(iter);
			return S_OK;
		}
	}

	return E_FAIL;
}

HRESULT CCamera_CutScene_Map::Change_CutSceneDesc(const _int& iIndex, const CAMERA_CUTSCENE_MAP_DESC& desc)
{
	if (0 > iIndex || m_CutSceneDescs.size() <= iIndex)
		return E_FAIL;

	m_CutSceneDescs[iIndex] = desc;

	return S_OK;
}

CAMERA_CUTSCENE_MAP_DESC* CCamera_CutScene_Map::Find_CutSceneDesc(const string& strCutSceneName)
{
	for (auto& iter : m_CutSceneDescs)
	{
		if (strCutSceneName == iter.strCutSceneName)
			return &iter;
	}

	return nullptr;
}

HRESULT CCamera_CutScene_Map::Ready_Components()
{
	return S_OK;
}

void CCamera_CutScene_Map::Tick_Fade(_float fTimeDelta)
{
	/* Fade 진행중 */
	if (m_tFadeDesc.bPlaying)
	{
		if (m_tFadeDesc.Intro.bPlaying && !m_tFadeDesc.Outtro.bPlaying) /* 컷신 인트로 Fade 진행중 */
		{
			if (CUI_Manager::GetInstance()->Is_FadeFinished()) 
			{
				if (!m_tFadeDesc.Intro.bFadeOutted) /* 페이드 아웃 종료, 페이드 인 시작 */
				{
					/* 컷신 시작 */	
					if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
						CUI_Manager::GetInstance()->OnOff_GamePlaySetting(false);

					m_tTimeDesc.Start(m_pCurCutSceneDesc->fDuration, m_pCurCutSceneDesc->eLerpMode);

					Set_Fov(Cam_Fov_CutScene_Map_Default);

					m_tFadeDesc.Intro.bFadeOutted = true;

					CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, m_tFadeDesc.Intro.fDuration, m_tFadeDesc.Intro.bWhite);
				}
				else /* 페이드 인 종료 */
				{
					/* 페이드 데이터 갱신 */
					m_tFadeDesc.bPlaying = false;
					m_tFadeDesc.Intro.bPlaying = false;
				}
			}
		}
		else if (!m_tFadeDesc.Intro.bPlaying && m_tFadeDesc.Outtro.bPlaying) /* 컷신 아웃트로 Fade 진행중 */
		{
			if (CUI_Manager::GetInstance()->Is_FadeFinished())
			{
				if (!m_tFadeDesc.Outtro.bFadeOutted) /* 페이드 아웃 종료, 페이드 인 시작 */
				{
					/* 카메라 체인지 */
					if (CAMERA_TYPE::FOLLOW == m_eReservedNextCameraType)
					{
						CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
						if (nullptr != pFollowCam)
						{
							pFollowCam->Reset_WideView_To_DefaultView(true);
							pFollowCam->Set_Default_Position();
							CCamera_Manager::GetInstance()->Set_CurCamera(pFollowCam->Get_Key());
						}
						m_eReservedNextCameraType = CAMERA_TYPE::CAMERA_TYPE_END;
					}
					else if (CAMERA_TYPE::ACTION == m_eReservedNextCameraType)
					{
						CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::ACTION);
					}
					else
						CCamera_Manager::GetInstance()->Set_PrevCamera();

					CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, m_tFadeDesc.Outtro.fDuration, m_tFadeDesc.Outtro.bWhite);

					m_tFadeDesc.Outtro.bFadeOutted = true;
				}
				else /* 페이드 인 종료 */
				{
					/* 어차피 여기는 안탄다. (위에서 이미 카메라 바뀌었다) */
				}
			}
		}
	}
		
}

void CCamera_CutScene_Map::Tick_Blending(const _float fDeltaTime)
{
	/* 현재는 고래 컷신에서만 사용 */
	/* Position */
	Vec4 vCamPosition = CCamera_Manager::GetInstance()->Get_BlendingPosition();

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vCamPosition.OneW());

	/* LookAt */
	m_vPrevLookAt = CCamera_Manager::GetInstance()->Get_BlendingLookAt();

	m_pTransformCom->LookAt(m_vPrevLookAt.OneW());
}

CCamera_CutScene_Map* CCamera_CutScene_Map::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
{
	CCamera_CutScene_Map* pInstance = new CCamera_CutScene_Map(pDevice, pContext, strObjTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCamera_CutScene_Map");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_CutScene_Map::Clone(void* pArg)
{
	CCamera_CutScene_Map* pInstance = new CCamera_CutScene_Map(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CCamera_CutScene_Map");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_CutScene_Map::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
}
