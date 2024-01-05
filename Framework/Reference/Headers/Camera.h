#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CGameObject;

#define MAX_UNIT_RAND 30

const static Vec2 v2UnitRand[MAX_UNIT_RAND] =
{
	{ -0.394, -0.919 },
	{ 0.922, 0.388 },
	{ -0.668, -0.744 },
	{ -0.892, -0.451 },
	{ 0.960, 0.281 },
	{ 0.125, -0.992 },
	{ -0.931, -0.366 },
	{ 0.673, 0.740 },
	{ 0.237, 0.972 },
	{ 0.138, 0.990 },
	{ -0.595, 0.803 },
	{ -0.992, 0.124 },
	{ -0.985, 0.174 },
	{ 0.763, -0.646 },
	{ 0.950, 0.313 },
	{ 0.743, -0.669 },
	{ 0.760, 0.650 },
	{ -0.999, 0.051 },
	{ -0.721, -0.693 },
	{ -0.919, 0.395 },
	{ 0.797, 0.603 },
	{ 0.999, -0.039 },
	{ 0.170, -0.985 },
	{ 0.966, -0.257 },
	{ -0.815, 0.579 },
	{ 0.240, -0.971 },
	{ -1.000, -0.027 },
	{ 0.933, -0.361 },
	{ 0.649, 0.761 },
	{ -0.920, -0.391 }
};

const static Vec3 v3UnitRand[MAX_UNIT_RAND] =
{
	{ -0.737, -0.555, 0.385 },
	{ -0.019, -0.959, 0.283 },
	{ -0.807, -0.012, 0.591 },
	{ -0.296, -0.020, -0.955 },
	{ -0.246, -0.402, 0.882 },
	{ 0.330, -0.293, 0.897 },
	{ -0.644, -0.496, 0.582 },
	{ 0.179, 0.929, 0.325 },
	{ -0.955, 0.294, 0.052 },
	{ -0.477, -0.726, -0.495 },
	{ 0.710, 0.704, -0.019 },
	{ 0.160, -0.357, 0.920 },
	{ -0.270, -0.371, 0.889 },
	{ 0.824, -0.416, -0.385 },
	{ -0.279, 0.517, 0.809 },
	{ 0.187, 0.836, -0.516 },
	{ 0.658, 0.512, 0.553 },
	{ 0.842, -0.234, 0.486 },
	{ 0.189, -0.866, 0.464 },
	{ -0.760, 0.602, 0.244 },
	{ 0.502, 0.334, -0.798 },
	{ -0.907, 0.031, 0.420 },
	{ -0.921, 0.388, 0.026 },
	{ -0.628, 0.209, -0.750 },
	{ -0.538, 0.737, -0.408 },
	{ -0.043, -0.210, -0.977 },
	{ -0.740, -0.563, -0.369 },
	{ 0.632, 0.748, 0.206 },
	{ -0.013, 0.919, -0.394 },
	{ -0.327, -0.577, -0.748 }
};

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagProjDesc
	{
		LERP_FLOAT_DESC	tLerpFov = {};

		_float fAspect = 0.f;
		_float fNear = 0.f;
		_float fFar = 0.f;

		_float fPrevFov = 0.f;

	}PROJ_DESC;

	typedef struct tagShakeDesc
	{
		/* User */
		_float fAmplitude = 0.f;
		_float fFrequency = 0.f;

		_float fDuration = 0.f;
		
		_float fBlendInTime		= 0.05f;
		_float fBlendOutTime	= 0.05f;

		/* Check */
		_bool bActive = false;

		_float fAccDuration = 0.f;
		_float fAccFrequency = 0.f;
		
		_float fFreqDelta = 0.f;

		LERP_VEC3_DESC tLerpShakeUnitPos;	/* 선형 보간되는 쉐이크 포지션 (fFrequency만큼의 길이)*/
		
		void Clear()
		{
			bActive = false;

			fAccDuration = 0.f;
			fAccFrequency = 0.f;

			fBlendInTime = 0.05f;
			fBlendOutTime = 0.05f;
		}

		Vec3 Get_ShakeLocalPos()
		{
			return tLerpShakeUnitPos.vCurVec * fAmplitude;
		}

	}SHAKE_DESC;

protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag, _uint iObjectID);
	CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Tick_Blending(const _float fDeltaTime) {};

public:
	/* Default */
	const _bool& Is_Active() const { return m_bActive; }
	void Set_Active(const _bool bActive) { m_bActive = bActive; }

	CTransform* Get_Transform() const { return m_pTransformCom; }

	const _int Get_Key() const { return m_iKey; }
	void Set_Key(const _int& iKey) { m_iKey = iKey; }

	/* Proj */
	const _bool& Is_Lerp_Fov() const { return m_tProjDesc.tLerpFov.bActive; }
	const _float& Get_Fov() const { return m_tProjDesc.tLerpFov.fCurValue; }
	PROJ_DESC Get_ProjDesc() const { return m_tProjDesc; }
	void Set_ProjDesc(const PROJ_DESC& tDesc) { memcpy(&m_tProjDesc, &tDesc, sizeof(PROJ_DESC)); }
	void Set_Fov(const _float& fFov) { memcpy(&m_tProjDesc.tLerpFov, &fFov, sizeof(_float)); }
	void Start_Lerp_Fov(const _float& fStartValue, const _float& fTargetValue, const _float& fTime, const LERP_MODE& eMode = LERP_MODE::SMOOTHER_STEP);
	void Start_Lerp_Fov(const _float& fTargetValue, const _float& fTime, const LERP_MODE& eMode = LERP_MODE::SMOOTHER_STEP);
	
	/* Distance */
	void Set_Distance(const _float& fDistance) { memcpy(&m_tLerpDist.fCurValue, &fDistance, sizeof(_float)); }
	const _float& Get_Distance() const { return m_tLerpDist.fCurValue; }
	const _bool& Is_Lerp_Distance() const { return m_tLerpDist.bActive; }
	void Start_Lerp_Distance(const _float& fStartValue, const _float& fTargetValue, const _float& fTime, const LERP_MODE& eMode = LERP_MODE::SMOOTHER_STEP);
	void Start_Lerp_Distance(const _float& fTargetValue, const _float& fTime, const LERP_MODE& eMode = LERP_MODE::SMOOTHER_STEP);

	/* Target, LookAt - Object */
	CGameObject* Get_TargetObj() const { return m_pTargetObj; }
	CGameObject* Get_LookAtObj() const { return m_pLookAtObj; }

	void Set_TargetObj(CGameObject* pTargetObj) { m_pTargetObj = pTargetObj; }
	void Set_LookAtObj(CGameObject* pTargetObj) { m_pLookAtObj = pTargetObj; }

	void Change_TargetObj(CGameObject* pTargetObj, const _float& fChangeTime, const LERP_MODE& eMode = LERP_MODE::SMOOTHER_STEP);
	void Change_LookAtObj(CGameObject* pLookAtObj, const _float& fChangeTime, const LERP_MODE& eMode = LERP_MODE::SMOOTHER_STEP);

	const _bool& Is_Blending_TargetObj() const { return m_tBlendingTargetPosition.bActive; }
	const _bool& Is_Blending_LookAtObj() const { return m_tBlendingLookAtPosition.bActive; }

	/* Target, LookAt - OffSet */
	const Vec4 Get_TargetOffset() const { return m_tTargetOffset.vCurVec; }
	const Vec4 Get_LookAtOffset() const { return m_tLookAtOffset.vCurVec; }

	void Set_TargetOffSet(const Vec4& vTargetOffset) { memcpy(&m_tTargetOffset.vCurVec, &vTargetOffset, sizeof(Vec4)); }
	void Set_LookAtOffSet(const Vec4& vLookAtOffset) { memcpy(&m_tLookAtOffset.vCurVec, &vLookAtOffset, sizeof(Vec4)); }

	void Change_TargetOffSet(const Vec4& vTargetValue, const _float& fTime, const LERP_MODE& eMode = LERP_MODE::SMOOTHER_STEP);
	void Change_LookAtOffSet(const Vec4& vTargetValue, const _float& fTime, const LERP_MODE& eMode = LERP_MODE::SMOOTHER_STEP);

	void Lerp_TargetOffset(const Vec4& vStartValue, const Vec4& vTargetValue, const _float& fTime, const LERP_MODE& eMode = LERP_MODE::SMOOTHER_STEP);
	void Lerp_LookAtOffSet(const Vec4& vStartValue, const Vec4& vTargetValue, const _float& fTime, const LERP_MODE& eMode = LERP_MODE::SMOOTHER_STEP);

	virtual Vec4 Get_LookAt() { return Vec4(); } /* 룩앳과 룩앳 오프셋이 더해진 최종 룩앳 반환 -> 카메라 간 블렌딩에서 주로 사용 */

	/* Input */
	const Vec2& Get_MouseSensitivity() const { return m_vMouseSensitivity; }

	void Set_MouseSensitivity_X(const _float& fX) { m_vMouseSensitivity.x = fX; }
	void Set_MouseSensitivity_Y(const _float& fY) { m_vMouseSensitivity.y = fY; }

	void Set_CanInput(const _bool& bInput) { m_bCanInput = bInput; }

	/* Shake */
	void Start_Shake(const _float& fAmplitude, const _float& fFrequency, const _float& fDuration);
	const _bool& Is_Shake() const { return m_tShakeDesc.bActive; }
	Vec3 Get_ShakeLocalPos() { return m_tShakeDesc.Get_ShakeLocalPos(); }
	
	/* Blending */
	const _bool& Is_Blending() const { return m_bBlending; }
	void Set_Blending(const _bool& bBlending) { m_bBlending = bBlending; }

protected:
	/* Default  */
	_bool				m_bActive		= { false };
	_int				m_iKey			= { -1 };
	CTransform*			m_pTransformCom = { nullptr };

	/* Proj */
	PROJ_DESC			m_tProjDesc		= {};

	/* Dist */
	LERP_FLOAT_DESC		m_tLerpDist		= {};
	_float				m_fPrevDist		= { 0.f };

	/* Target, Look */
	LERP_VEC4_DESC		m_tTargetOffset = {};
	LERP_VEC4_DESC		m_tLookAtOffset = {};

	CGameObject*		m_pTargetObj	= { nullptr };
	CGameObject*		m_pLookAtObj	= { nullptr };

	LERP_VEC4_DESC		m_tBlendingTargetPosition	= {}; /* 타겟, 룩앳 오브젝트를 체인지하는 동안의 블렌딩 위치 */
	LERP_VEC4_DESC		m_tBlendingLookAtPosition	= {};

	/* Input */
	_bool				m_bCanInput		= { true };
	Vec2				m_vMouseSensitivity = { 1.f, 1.f };

	/* Shake */
	SHAKE_DESC			m_tShakeDesc	= {};

	/* Blendgin */
	_bool				m_bBlending		= { false };

protected:
	virtual HRESULT Ready_Components() override;

private:
	void Tick_Lerp(const _float fDeltaTime);
	void Tick_Shake(const _float fDeltaTime);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free();
};

END