#pragma once

#include "Client_Defines.h"
#include "Camera.h"

#include "Camera_Manager.h"

BEGIN(Client)

class CCamera_Quater final : public CCamera
{
	enum VIEW_TYPE { NE, SE, SW, NW };

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
	virtual HRESULT Ready_VirtualTarget();

	void Tick_VirtualTargetTransform(const _float fDeltaTime);
	void Tick_Transform(const _float fDeltaTime);
	void Test(_float fTimeDelta);

private:
	/* Init */
	_bool			m_bSet = false;

	/* Damping */
	const _float	m_fDampingCoefficient = 0.1f;
	const _float	m_fDampingMaxDistance = 5.f;
	Vec4			m_vCurPos = {};

	/* Virtual Target */
	const _float		m_fVirtualTargetMoveSpeed = 20.f;
	class CTransform*	m_pVirtualTargetTransform = nullptr;

	/* Distance, Height */
	const _float		m_fHeightMag = 0.5f; /* 디스턴스에 대한 높이 비율 */
	const _float		m_fInitHeight = 25.f;
	LERP_FLOAT_DESC		m_tHeight = {};

	CCamera_Quater::VIEW_TYPE			m_eViewType = CCamera_Quater::VIEW_TYPE::NE;

public:
	static CCamera_Quater* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END