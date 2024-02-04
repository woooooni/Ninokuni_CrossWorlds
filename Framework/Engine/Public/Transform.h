#pragma once

#include "Engine_Defines.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	_vector Get_State(STATE eState) {
		return XMLoadFloat4((_float4*)&m_WorldMatrix.m[eState][0]);
	}

	_vector Get_Look() {
		return XMLoadFloat4((_float4*)&m_WorldMatrix.m[STATE_LOOK][0]);
	}

	_vector Get_Right() {
		return XMLoadFloat4((_float4*)&m_WorldMatrix.m[STATE_RIGHT][0]);
	}

	_vector Get_Up() {
		return XMLoadFloat4((_float4*)&m_WorldMatrix.m[STATE_UP][0]);
	}

	_vector Get_Position() {
		return XMLoadFloat4((_float4*)&m_WorldMatrix.m[STATE_POSITION][0]);
	}

	_matrix Get_WorldMatrix() const {
		return XMLoadFloat4x4(&m_WorldMatrix);
	}

	void Set_WorldMatrix(_matrix WorldMatrix) {
		XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);
	}

	_float4x4 Get_WorldFloat4x4() const {
 		return m_WorldMatrix;
	}

	_float4x4 Get_WorldFloat4x4_TransPose() const {
		_float4x4	WorldMatrix;
		XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(Get_WorldMatrix()));
		return WorldMatrix;
	}

	_matrix Get_WorldMatrixInverse() const {
		return XMMatrixInverse(nullptr, Get_WorldMatrix());
	}

	_float4x4 Get_WorldMatrix_Float4x4_Inverse() const {
		_float4x4 WorldMatrixInverse;
		XMStoreFloat4x4(&WorldMatrixInverse, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));
		return WorldMatrixInverse;
	}

	Vec4 Get_RelativeOffset(Vec4 vPos);

public:
	void Set_State(STATE eState, _vector vState);
	void Set_Right(_vector vState);
	void Set_Up(_vector vState);
	void Set_Look(_vector vState);
	
	void Set_Position(_vector vState);

	Vec4 Get_LookAt();
	void Set_LookAtByDir(Vec4 vLook);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT Bind_ShaderResources(class CShader* pShader, const char* pConstantName);

public:
	void Move(_vector vDir, _float fSpeed, _float fTimeDelta, class CNavigation* pNavigation = nullptr);	
	void Translate(const Vec3& vTranslation);
	void Set_Scale(_vector vScale);
	void Set_Scale(_float3 vScale);

	_float3 Get_Scale();

	void Turn(_fvector vAxis, _float fSpeed, _float fTimeDelta);

	void Rotation(_fvector vAxis, _float fRadian);
	void Rotation_Acc(_fvector vAxis, _float fRadian);

	void LookAt(_vector vPosition);
	void LookAt_ForLandObject(_vector vPosition);
	void Rotation_Look(_vector vLook);

	void FixRotation(_float x, _float y, _float z);
	void RevolutionRotation(const Vec3& vPoint, const Vec3 vAxis, const _float& fAngle);
	void ParabolicFormula(const Vec3& startPos, const Vec3& endPos, const _float fTimeDelta,
		const _float& maxHeight = 150.0f, const _float& maxTime = 3.6f);
	XMVECTOR Get_WorldRotation() { return m_vRotation; }
	
	void ResetTime() { m_fTime = 0.0f; }
private:
	_float4x4				m_WorldMatrix;
	XMVECTOR m_vRotation = ::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	_float m_fTime = 0.0f;

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END