#pragma once

#include "Client_Defines.h"
#include "Camera.h"

#include "Camera_Manager.h"

BEGIN(Client)

class CCamera_CurlingGame final : public CCamera
{
private:
	CCamera_CurlingGame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	CCamera_CurlingGame(const CCamera_CurlingGame& rhs);
	virtual ~CCamera_CurlingGame() = default;

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

	HRESULT Change_Target(CGameObject* pTarget);

	Vec4 Calculate_CamPosition(const Vec4 vTargetPosition);
	Vec4 Calculate_LookAt(const Vec4 vTargetPostion);

private:
	virtual HRESULT Ready_Components() override;

	void Tick_ChangeTarget(const _float& fTimeDelta);


private:
	Vec4 m_vPrevLookAt = {};

	/* Change Target */
	_bool m_bChangingTarget = false;
	LERP_VEC4_DESC m_tLerpPos = {};
	const _float m_fLerpTime = 1.f;


	
public:
	static CCamera_CurlingGame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END