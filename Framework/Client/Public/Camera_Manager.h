#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "Camera.h"

BEGIN(Engine)
class CCamera;
class CGameObject;
class CTransform;
END

#pragma region Distance

static const _float Cam_Dist_Follow_Default = 3.2f;

#pragma endregion

#pragma region Fov

static const _float Cam_Fov_Free_Default = XMConvertToRadians(60.0f);

static const _float Cam_Fov_Follow_Default = XMConvertToRadians(60.0f);

#pragma endregion


BEGIN(Client)
class CCamera_Manager : public CBase
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
	/* Cameras */
	CCamera* Get_Camera(const CAMERA_TYPE& eType);
	CCamera* Get_CurCamera() const { return m_pCurCamera; }

	const _bool Is_Empty_Camera(const CAMERA_TYPE& eType) { return (nullptr == m_Cameras[eType]) ? true : false; }

	HRESULT Set_CurCamera(const CAMERA_TYPE& eType);

	/* Event */
	/* 카메라 베이스 클래스에서 갖고 있는 데이터로 사용 */
	/* 주로 애니메이션 이벤트에서 사용 */
	HRESULT Start_Action_Shake(const CAMERA_EVENT_DESC& tDesc);
	HRESULT Start_Action_Lerp_Fov(const CAMERA_EVENT_DESC& tDesc);
	HRESULT Start_Action_Lerp_Dist(const CAMERA_EVENT_DESC& tDesc);

private:
	/* Defualt */


	/* Cameras */

	CCamera* m_pCurCamera = { nullptr };
	CCamera* m_pNextCamera = { nullptr };
	CCamera* m_Cameras[CAMERA_TYPE::CAMERA_TYPE_END];

	/* Lerp */
	LERP_TIME_DESC m_tLerpTime = {};

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

private:
	HRESULT Ready_Cameras();

public:
	virtual void Free() override;
};

END
