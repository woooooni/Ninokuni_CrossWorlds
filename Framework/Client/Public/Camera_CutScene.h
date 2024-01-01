#pragma once

#include "Client_Defines.h"
#include "Camera.h"

#include "Camera_Manager.h"

BEGIN(Client)

typedef struct tagCameraCutSceneDesc
{
	string		strCutSceneName = {};

	Vec3		vCamPositions[MAX_BEZIER_POINT];
	Vec3		vCamLookAts[MAX_BEZIER_POINT];

	_float		fDuration			= 4.f;
	_float		fStartDelayTime		= 0.f;
	_float		fFinishDelayTime	= 0.f;

	_float		fStartFov			= 0.f; // 아직 미사용 
	_float		fFinishFov			= 0.f;

	LERP_MODE	eLerpMode = LERP_MODE::SMOOTHER_STEP;

}CAMERA_CUTSCENE_DESC;

class CCamera_CutScene final : public CCamera
{
private:
	CCamera_CutScene(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	CCamera_CutScene(const CCamera_CutScene& rhs);
	virtual ~CCamera_CutScene() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Start_CutScene(const string& strCutSceneName); /* 단일 컷신 실행 */
	HRESULT Start_CutScenes(vector<string> strCutSceneNames); /* 복수 컷신 실행 */
	const _bool Is_Playing_CutScenc() const { return m_tTimeDesc.bActive; }
public:
	static Vec4 Get_Point_In_Bezier(Vec3 vPoints[MAX_BEZIER_POINT], const _float& fRatio);

public:
	HRESULT Save_CutSceneDescs();
	HRESULT Load_CutSceneDescs();

public:
	/* Tool */
	HRESULT	Add_CutSceneDesc(const CAMERA_CUTSCENE_DESC& desc);
	HRESULT Del_CutSceneDesc(const string& strCutSceneName);
	HRESULT Change_CutSceneDesc(const _int& iIndex, const CAMERA_CUTSCENE_DESC& desc);
	CAMERA_CUTSCENE_DESC* Find_CutSceneDesc(const string& strCutSceneName);
	const vector<CAMERA_CUTSCENE_DESC>& Get_CutSceneDescs() const { return m_CutSceneDescs; }

private:
	virtual HRESULT Ready_Components() override;

private:
	vector<CAMERA_CUTSCENE_DESC>	m_CutSceneDescs;

	CAMERA_CUTSCENE_DESC*			m_pCurCutSceneDesc = nullptr;

	queue<string>					m_CutSceneNamesReserved;

	LERP_TIME_DESC					m_tTimeDesc;
	
public:
	static CCamera_CutScene* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END