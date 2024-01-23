#pragma once

#include "Client_Defines.h"
#include "Camera.h"

#include "Camera_Manager.h"

BEGIN(Client)

typedef struct tagCameraCutSceneMapDesc
{
	string		strCutSceneName = {};

	Vec3		vCamPositions[MAX_BEZIER_POINT];
	Vec3		vCamLookAts[MAX_BEZIER_POINT];

	_float		fDuration			= 4.f;
	_float		fStartDelayTime		= 0.f;
	_float		fFinishDelayTime	= 0.f;

	_float		fStartFov			= 0.f; // 아직 미사용 
	_float		fFinishFov			= 0.f; // 아직 미사용

	LERP_MODE	eLerpMode = LERP_MODE::SMOOTHER_STEP;

}CAMERA_CUTSCENE_MAP_DESC;

class CCamera_CutScene_Map final : public CCamera
{
	typedef struct tagFadeDesc
	{
		_float		fDuration = 0.f;
		_bool		bWhite = false;
		_bool		bPlaying = false;
		_bool		bFadeOutted = false;

		void Clear()
		{
			fDuration = 0.f;
			bWhite = false;
			bPlaying = false;
			bFadeOutted = false;
		}
	}FADE_DESC;
	typedef struct tagCutSceneFadeDesc
	{
		FADE_DESC Intro = {};
		FADE_DESC Outtro = {};

		_bool	bReserved = false;
		_bool	bPlaying = false;
		_bool	bInit = false;
		
		void Clear()
		{
			Intro.Clear();
			Outtro.Clear();
			bReserved = false;
			bPlaying = false;
			bInit = false;
		}

	}CUTSCENE_FADE_DESC;

	typedef struct tagSpecialCutSceneDesc
	{
		/* 개별 컷신 데이터 */
		_bool bSetTransform = false;
		_bool bStarted = false;

		void Clear()
		{
			bSetTransform = false;
			bStarted = false;
		}

	}SPECIAL_CUTSCENE_DESC;

private:
	CCamera_CutScene_Map(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	CCamera_CutScene_Map(const CCamera_CutScene_Map& rhs);
	virtual ~CCamera_CutScene_Map() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Tick_Blending(const _float fDeltaTime) override;

public:
	HRESULT Start_CutScene(const LEVELID& eLevelID);
	HRESULT Start_CutScene(const string& strCutSceneName, const _bool& bWillRetruePrevCam = false); /* 단일 컷신 실행 */
	HRESULT Start_CutScenes(vector<string> strCutSceneNames, const _bool& bWillRetruePrevCam = false); /* 복수 컷신 실행 */
	HRESULT Stop_CutScene(const _bool& bClearReservedCutScene = false);
	const _bool Is_Playing_CutScenc() const { return m_tTimeDesc.bActive; }
	HRESULT Reserve_Fade(const _float& fIntroDuration, const _bool& bIntroWhite, const _float& fOuttroDuration, const _bool& bOuttroWhite);
	HRESULT Set_CutSceneTransform(const string& strCutSceneName); /* 블렌딩이 필요한 경우 해당 컷신의 트랜스폼을 미리 세팅해둔다. */
	static Vec4 Get_Point_In_Bezier(Vec3 vPoints[MAX_BEZIER_POINT], const _float& fRatio);
	void Reserve_NextCameraType(const CAMERA_TYPE& eType) { m_eReservedNextCameraType = eType; }

public:
	virtual Vec4 Get_LookAt() override;
	virtual void Set_Blending(const _bool& bBlending) override;

public:
	HRESULT Save_CutSceneDescs();
	HRESULT Load_CutSceneDescs();

public:
	/* Tool */
	HRESULT	Add_CutSceneDesc(const CAMERA_CUTSCENE_MAP_DESC& desc);
	HRESULT Del_CutSceneDesc(const string& strCutSceneName);
	HRESULT Change_CutSceneDesc(const _int& iIndex, const CAMERA_CUTSCENE_MAP_DESC& desc);
	CAMERA_CUTSCENE_MAP_DESC* Find_CutSceneDesc(const string& strCutSceneName);
	const vector<CAMERA_CUTSCENE_MAP_DESC>& Get_CutSceneDescs() const { return m_CutSceneDescs; }

private:
	virtual HRESULT Ready_Components() override;
	void Tick_Fade(_float fTimeDelta);

private:
	vector<CAMERA_CUTSCENE_MAP_DESC>	m_CutSceneDescs;

	CAMERA_CUTSCENE_MAP_DESC*			m_pCurCutSceneDesc = nullptr;

	queue<string>						m_CutSceneNamesReserved;

	LERP_TIME_DESC						m_tTimeDesc;

	Vec4								m_vPrevLookAt = {};

	/* 컷신 이후 체인지 예약 데이터 */
	_bool								m_bWillRetruePrevCam = false;

	/* 페이드 */
	CUTSCENE_FADE_DESC					m_tFadeDesc = {};

	/* 개별 컷신 데이터 */
	SPECIAL_CUTSCENE_DESC				m_tWhaleDesc = {};

	CAMERA_TYPE							m_eReservedNextCameraType = CAMERA_TYPE::CAMERA_TYPE_END;
	
public:
	static CCamera_CutScene_Map* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END