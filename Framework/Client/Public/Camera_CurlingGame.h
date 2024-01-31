#pragma once

#include "Client_Defines.h"
#include "Camera.h"

#include "Camera_Manager.h"

BEGIN(Client)

class CCamera_CurlingGame final : public CCamera
{

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
		const _float	fDampingCoefficient = 0.025f;

		/* Fov */
		const _float	fDampingFov		= XMConvertToRadians(50.f);
		const _float	fFovLerpTime	= 0.5f;

		void Ready(const Vec4& _vCurPos)
		{
			bActive = true;
			vCurPos = vTargetPos = _vCurPos;
		}

		/*void Clear()
		{
			bActive = false;
		}*/

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
	HRESULT Start_Damping();
	HRESULT Finish_Damping();
	const _bool& Is_Damping() const { return m_tDampingDesc.bActive; }

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