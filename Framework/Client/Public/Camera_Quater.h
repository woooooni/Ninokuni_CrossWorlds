#pragma once

#include "Client_Defines.h"
#include "Camera.h"

#include "Camera_Manager.h"

BEGIN(Client)

class CCamera_Quater final : public CCamera
{
private:
	CCamera_Quater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	CCamera_Quater(const CCamera_Quater& rhs);
	virtual ~CCamera_Quater() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override { return S_OK; }

public:
	virtual Vec4 Get_LookAt() override;
	virtual void Tick_Blending(const _float fDeltaTime) override;
	virtual void Set_Blending(const _bool& bBlending) override;
	virtual void Set_Active(const _bool bActive) override;

private:
	virtual HRESULT Ready_Components() override;

	void Tick_Transform(const _float fDeltaTime);
	void Test(_float fTimeDelta);

private:
	_float m_fMoveSpeed = 20.f;
	
public:
	static CCamera_Quater* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END