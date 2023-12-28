#pragma once
#include "Tool.h"

#include "Camera.h"

BEGIN(Engine)
class CCamera;
END

BEGIN(Client)
class CTool_Camera final : public CTool
{
private:
	CTool_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_Camera() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;

private:
	void Show_Select_Camera();

	void Show_Camera_Prop_Default(CCamera* pCurCam);
	void Show_Camera_Prop_Free(CCamera* pCurCam);
	void Show_Camera_Prop_Follow(CCamera* pCurCam);
	

private:

public:
	static CTool_Camera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END