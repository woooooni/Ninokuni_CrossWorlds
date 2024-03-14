#pragma once

#include "Client_Defines.h"
#include "Camera.h"

#include "Camera_Manager.h"

BEGIN(Client)

class CCamera_Top final : public CCamera
{
public:
	enum VIEW_TYPE { GLANIX_PILLAR_PATTERN, VIEW_TYPEEND};

private:
	CCamera_Top(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	CCamera_Top(const CCamera_Top& rhs);
	virtual ~CCamera_Top() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override { return S_OK; }

public:
	HRESULT Start_TopView(const VIEW_TYPE& eType);

	virtual Vec4 Get_LookAt() override;
	virtual void Tick_Blending(const _float fDeltaTime) override;
	virtual void Set_Blending(const _bool& bBlending) override;

	virtual void Lerp_TargetOffset(const Vec4& vStartValue, const Vec4& vTargetValue, const _float& fTime, const LERP_MODE& eMode = LERP_MODE::SMOOTHER_STEP);

private:
	virtual HRESULT Ready_Components() override;

private:
	void Tick_Glanix_PillarPattern(_float fTimeDelta);

private:
	VIEW_TYPE	m_eCurType = VIEW_TYPE::VIEW_TYPEEND;

	/* Glanix */
	_float		m_fPosOriginHeight = 0.f;
	_float		m_fLookOriginHeight = 0.f;
	Vec4		m_vPrevLookAt = {};
	Vec4		m_vCurPos = {};
	_float		m_fDampingCoefficient = 0.025f;

	LERP_FLOAT_DESC m_tLerpHeight = {};
	_bool			m_bInitLerp = false;

public:
	static CCamera_Top* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END