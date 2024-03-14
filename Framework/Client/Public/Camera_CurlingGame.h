#pragma once

#include "Client_Defines.h"
#include "Camera.h"

#include "Camera_Manager.h"

BEGIN(Client)

class CCamera_CurlingGame final : public CCamera
{
private:
	typedef struct tagTargetDesc
	{
		_bool			bChangingTarget = false;
		LERP_VEC4_DESC	tLerpPos = {};

	}TARGET_DESC;

	typedef struct tagDampingDesc
	{
		/* Damping */
		_bool			bActive = false;
		Vec4			vCurPos;
		Vec4			vTargetPos;
		const _float	fDampingCoefficient = 0.01f;

		/* Offset, Fov */
		const _float	fOffsetLerpTime		= 1.5f;
		_bool			bLerpOffset			= false;
		const _float	fCheckThreshold		= 2.f;

		void Ready(const Vec4& _vCurPos)
		{
			bActive = true;
			vCurPos = vTargetPos = _vCurPos;
		}

		void Clear()
		{
			bActive = false;

			bLerpOffset = false;
		}

	}DAMPING_DESC;

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
	virtual Vec4 Get_LookAt() override { return m_vPrevLookAt; } 
	virtual void Tick_Blending(const _float fDeltaTime) override;
	virtual void Set_Blending(const _bool& bBlending) override;

public:
	/* Target */
	HRESULT Change_Target(CGameObject* pTarget, const _float& fLerpTime);
	Vec4 Calculate_Position(const Vec4 vTargetPosition);
	Vec4 Calculate_LookAt(const Vec4 vTargetPostion);
	const _bool& Is_ChagingTarget() const { return m_tTargetDesc.bChangingTarget; }

	/* Damping */
	HRESULT Start_StoneAction();
	HRESULT Finish_StoneAction(const _float& fLerpTime);
	const _bool& Is_StoneAction() const { return m_tDampingDesc.bActive; }
	void Check_StoneAction();

private:
	virtual HRESULT Ready_Components() override;

	void Tick_ChangeTarget(const _float& fTimeDelta);

private:
	Vec4			m_vPrevLookAt = {};

	/* Target */
	TARGET_DESC		m_tTargetDesc = {};

	/* Dampint */
	DAMPING_DESC	m_tDampingDesc = {};

public:
	static CCamera_CurlingGame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END