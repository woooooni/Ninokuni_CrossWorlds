#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "Camera.h"

BEGIN(Engine)
class CCamera;
class CGameObject;
class CTransform;
END

enum CAMERA_TYPE { FREE, FOLLOW, /* ACTION, */ TYPE_END };
const wstring CameraWstringNames[CAMERA_TYPE::TYPE_END]{ L"Camera_Free", L"Camera_Follow" };
static const char* CameraCharNames[CAMERA_TYPE::TYPE_END]{ "Camera_Free", "Camera_Follow" };


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
	CCamera* Get_Camera(const CAMERA_TYPE& eType);
	CCamera* Get_CurCamera() const { return m_pCurCamera; }
	CCamera** Get_Cameras() { return m_Cameras; }

	HRESULT Set_CurCamera(const CAMERA_TYPE& eType);

private:
	/* Defualt */


	/* Cameras */

	CCamera* m_pCurCamera = { nullptr };
	CCamera* m_pNextCamera = { nullptr };
	CCamera* m_Cameras[CAMERA_TYPE::TYPE_END];

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
