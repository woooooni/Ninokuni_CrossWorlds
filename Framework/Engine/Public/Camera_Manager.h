#pragma once

#include "Engine_Macro.h"
#include "Base.h"

#pragma region Camera Define 

#define MAX_BEZIER_POINT 4

static const _float Cam_Dist_Follow_Default = 5.f;

static const _float Cam_Fov_Free_Default = XMConvertToRadians(60.0f);
static const _float Cam_Fov_Follow_Default = XMConvertToRadians(60.0f);
static const _float Cam_Fov_CutScene_Default = XMConvertToRadians(70.0f);

enum CAMERA_EVENT_TYPE { LERP_FOV, LERP_DIST, SHAKE, CAMERA_EVENT_TYPE_END };

typedef struct tagCameraEventDesc
{
							/* LERP_FOV,		LERP_DIST,		SHAKE */
	_float fTag1 = 0.f;		/* fTargetValue,	fTargetValue,	fAmplitude*/
	_float fTag2 = 0.f;		/* fTime,			fTime,			fFrequency */
	_float fTag3 = 0.f;		/*		,				,			fDuration */
	_float fTag4 = 0.f;

	void* ptr1 = nullptr;
	void* ptr2 = nullptr;

	_int iTag1 = 0.f;		/* eMode */
	_int iTag2 = 0.f;

}CAMERA_EVENT_DESC;

#pragma endregion


BEGIN(Engine)

class CCamera;

class ENGINE_DLL CCamera_Manager final : public CBase
{
	DECLARE_SINGLETON(CCamera_Manager)

private:
	CCamera_Manager();
	virtual ~CCamera_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

public:
	/* Access */
	CCamera*	Get_Camera(const _uint& iKey);
	CCamera*	Get_CurCamera() const { return m_pCurCamera; }
	HRESULT		Set_CurCamera(const _uint& iKey);
	const _bool Is_Empty_Camera(const _uint& iKey);
	HRESULT		Add_Camera(const _uint& iKey, CCamera* pCamera);

public:
	HRESULT Start_Action_Shake_Default();

	HRESULT Start_Action_CutScene(const string& strCutSceneName);

private:
	CCamera* Find_Camera(const _uint& iKey);

private:
	CCamera* m_pCurCamera	= { nullptr };
	CCamera* m_pNextCamera	= { nullptr };
	
	map<_uint, CCamera*>	m_pCameras;

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	virtual void Free() override;
};

END
