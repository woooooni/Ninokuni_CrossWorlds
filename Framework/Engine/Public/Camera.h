#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CGameObject;

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagProjDesc
	{
		LERP_FLOAT_DESC	tLerpFov = {};

		_float fAspect = 0.f;
		_float fNear = 0.f;
		_float fFar = 0.f;

	}PROJ_DESC;

	typedef struct tagShakeDesc
	{
		_bool	bActive = false;
		_float	fAccTime = 0.f;
		_float	fShakeTime = 0.f;

		void Reset()
		{
			bActive = false;
			fAccTime = 0.f;
			fShakeTime = 0.f;
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
	/* Default */
	const _bool& Is_Active() const { return m_bActive; }
	void Set_Active(const _bool bActive) { m_bActive = bActive; }

	CTransform* Get_Transform() const { return m_pTransformCom; }

	/* Proj */
	const _bool& Is_Lerp_Fov() const { return m_tProjDesc.tLerpFov.bActive; }
	const _float& Get_Fov() const { return m_tProjDesc.tLerpFov.fCurValue; }
	void Set_Fov(const _float& fFov) { memcpy(&m_tProjDesc.tLerpFov, &fFov, sizeof(_float)); }
	const PROJ_DESC& Get_ProjDesc() const { return m_tProjDesc; }
	
	/* Distance */
	const _bool& Is_Lerp_Distance() const { return m_tDist.bActive; }
	const _float& Get_Distance() const { return m_tDist.fCurValue; }
	void Set_Distance(const _float& fDistance) { memcpy(&m_tDist.fCurValue, &fDistance, sizeof(_float)); }

	/* Target, LookAt */
	CGameObject* Get_TargetObj() const { return m_pTargetObj; }
	CGameObject* Get_LookAtObj() const { return m_pLookAtObj; }

	const Vec4 Get_TargetOffset() const { return m_vTargetOffset; }
	const Vec4 Get_LookAtOffset() const { return m_vLookAtOffset; }

	void Set_TargetObj(CGameObject* pTargetObj) { m_pTargetObj = pTargetObj; }
	void Set_LookAtObj(CGameObject* pTargetObj) { m_pLookAtObj = pTargetObj; }

	void Set_TargetOffSet(const Vec4& vTargetOffset) { memcpy(&m_vTargetOffset, &vTargetOffset, sizeof(Vec4)); }
	void Set_LookAtOffSet(const Vec4& vLookAtOffset) { memcpy(&m_vLookAtOffset, &vLookAtOffset, sizeof(Vec4)); }

public:
	/* Framework */
	/*void Set_Transform_To_Pipeline();*/

protected:
	/* Default  */
	_bool				m_bActive = { false };
	CTransform*			m_pTransformCom = { nullptr };

	/* Proj */
	PROJ_DESC			m_tProjDesc		= {};

	/* Fov, Dist */
	LERP_FLOAT_DESC		m_tDist		= {};

	/* Target, Look */
	Vec4				m_vTargetOffset = {};
	Vec4				m_vLookAtOffset = {};

	CGameObject*		m_pTargetObj = { nullptr };
	CGameObject*		m_pLookAtObj = { nullptr };

	/* Shake */
	SHAKE_DESC			m_tShakeDesc = {};

protected:
	virtual HRESULT Ready_Components() override;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free();
};

END