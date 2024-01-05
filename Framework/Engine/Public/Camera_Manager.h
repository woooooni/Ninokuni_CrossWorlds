#pragma once

#include "Engine_Macro.h"
#include "Base.h"

#pragma region Camera Define 

enum CAMERA_EVENT_TYPE { FOV, DISTANCE, SHAKE, CAMERA_EVENT_TYPE_END };

static const char* szCameraEventTypeNames[CAMERA_EVENT_TYPE::CAMERA_EVENT_TYPE_END]{ "FOV", "DISTANCE", "SHAKE" };
static const wstring CameraEventTypeNames[CAMERA_EVENT_TYPE::CAMERA_EVENT_TYPE_END] { L"FOV", L"DISTANCE", L"SHAKE" };

typedef struct tagCameraEventDesc
{
								/* FOV,				DIST,			SHAKE */

	_float	fTag1 = 0.f;		/* fTargetValue,	fTargetValue,	fAmplitude */
	_float	fTag2 = 0.f;		/* fTime,			fTime,			fFrequency */
	_float	fTag3 = 0.f;		/*		,			,				fDuration  */

	_int	iTag1 = 0;			/* eMode,			eMode */

	_int	iTag2 = 0;			/* Anim Camera Event에서 CAMERA_EVENT_TYPE 역할 */

	_bool	bTag1 = false;		/* Anim Camera Event에서 Executed 역할 */

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
	HRESULT		Set_PrevCamera();
	
	const _bool Is_Empty_Camera(const _uint& iKey);
	const _bool Is_Blending_Camera() const { return m_bBlending; }
	
	HRESULT		Add_Camera(const _uint& iKey, CCamera* pCamera);

	Vec4		Get_BlendingPosition() const { return m_tBlendingPosition.vCurVec; }
	Vec4		Get_BlendingLookAt() const { return m_tBlendingLookAt.vCurVec; }

public:
	/* Action */
	HRESULT		Start_Action_Shake_Default();
	HRESULT		Start_Action_Shake(const _float& fAmplitude, const _float& fFrequency, const _float& fDuration);

	HRESULT		Change_Camera(const _uint& iKey, const _float& fBlendingDuration = 1.f, const LERP_MODE& eLerpMode = LERP_MODE::SMOOTHER_STEP);

private:
	CCamera*	Find_Camera(const _uint& iKey);
	void		Tick_Blending(_float fTimeDelta);

private:
	CCamera* m_pPrevCamera	= { nullptr };
	CCamera* m_pCurCamera	= { nullptr };
	CCamera* m_pNextCamera	= { nullptr };
	
	map<_uint, CCamera*> m_pCameras;

	_bool	m_bBlending = false;

	LERP_VEC4_DESC m_tBlendingPosition = {};
	LERP_VEC4_DESC m_tBlendingLookAt = {};

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	virtual void Free() override;
};

END
