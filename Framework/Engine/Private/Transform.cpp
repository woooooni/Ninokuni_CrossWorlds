#include "..\Public\Transform.h"
#include "Shader.h"
#include "Navigation.h"
#include "GameObject.h"
#include "RigidBody.h"


USING(Engine)
CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{

}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{

}





HRESULT CTransform::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{

	return S_OK;
}

HRESULT CTransform::Bind_ShaderResources(CShader* pShader, const char* pConstantName)
{
	return pShader->Bind_Matrix(pConstantName, &m_WorldMatrix);
}


void CTransform::Move(_vector vDir, _float fSpeed, _float fTimeDelta, CNavigation* pNavigation)
{
	if (XMVectorGetX(XMVector3Length(vDir)) < .99f)
		return;


	_vector	vPosition = Get_State(CTransform::STATE_POSITION);
	vPosition += XMVector3Normalize(vDir) * fSpeed * fTimeDelta;

	_vector vSlidingDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	if (pNavigation == nullptr)
		Set_State(CTransform::STATE_POSITION, vPosition);
	else
	{
		if (true == pNavigation->Is_Movable(vPosition, XMVector3Normalize(vPosition - Get_State(CTransform::STATE_POSITION)), &vSlidingDir))
			Set_State(CTransform::STATE_POSITION, vPosition);
		else
		{
			if (XMVectorGetX(XMVector3Length(vSlidingDir)) > 0.f)
			{
				vSlidingDir = XMVector3Normalize(vSlidingDir);
				_vector vNewPosition = Get_State(CTransform::STATE_POSITION) + vSlidingDir * fSpeed * fTimeDelta;

				if (true == pNavigation->Is_Movable(vNewPosition, XMVector3Normalize(vNewPosition - Get_State(CTransform::STATE_POSITION)), nullptr))
				{
					Set_State(CTransform::STATE_POSITION, vNewPosition);
				}
			}
		}
	}
}

void CTransform::Set_State(STATE eState, _vector vState)
{
	_matrix	WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
	WorldMatrix.r[eState] = vState;
	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);
}

void CTransform::Set_Position(_vector vPosition, _float fTimeDelta, CNavigation* pNavigation, _bool* bMovable)
{
	if (nullptr != bMovable)
		*bMovable = false;

	vPosition = XMVectorSetW(vPosition, 1.f);
	_vector vSlidingDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);

	if (pNavigation == nullptr)
	{
		Set_State(CTransform::STATE_POSITION, vPosition);
		if (nullptr != bMovable)
			*bMovable = true;
	}
	else
	{
		if (true == pNavigation->Is_Movable(vPosition, XMVector3Normalize(vPosition - Get_State(CTransform::STATE_POSITION)), &vSlidingDir))
		{
			Set_State(CTransform::STATE_POSITION, vPosition);
			if (nullptr != bMovable)
				*bMovable = true;
		}
		else
		{
			CRigidBody* pOwnerRigidBody = m_pOwner->Get_Component<CRigidBody>(L"Com_RigidBody");
			if (nullptr != pOwnerRigidBody)
			{
				_float3 vVelocity = pOwnerRigidBody->Get_Velocity();
				vSlidingDir = XMVector3Normalize(XMVectorSetY(vSlidingDir, vVelocity.y));

				_vector vNewPosition = Get_State(CTransform::STATE_POSITION) + vSlidingDir * XMVectorGetX(XMVector3Length(XMVectorSetY(XMLoadFloat3(&vVelocity), 0.f))) * fTimeDelta;
				if (XMVectorGetY(vNewPosition) <= pOwnerRigidBody->Get_RefHeight())
				{
					vNewPosition = XMVectorSetY(vNewPosition, pOwnerRigidBody->Get_RefHeight());
				}

				if (true == pNavigation->Is_Movable(vNewPosition, XMVector3Normalize(vNewPosition - Get_State(CTransform::STATE_POSITION)), nullptr))
				{
					Set_State(CTransform::STATE_POSITION, vNewPosition);
					if (nullptr != bMovable)
						*bMovable = true;
				}
			}
			else
			{
				_float3 vVelocity = pOwnerRigidBody->Get_Velocity();
				vSlidingDir = XMVector3Normalize(vSlidingDir);
				_vector vNewPosition = Get_State(CTransform::STATE_POSITION) + vSlidingDir * vVelocity.y * fTimeDelta;

				if (XMVectorGetY(vNewPosition) <= pOwnerRigidBody->Get_RefHeight())
				{
					vNewPosition = XMVectorSetY(vNewPosition, pOwnerRigidBody->Get_RefHeight());
				}

				if (true == pNavigation->Is_Movable(vNewPosition, XMVector3Normalize(vNewPosition - Get_State(CTransform::STATE_POSITION)), nullptr))
				{
					Set_State(CTransform::STATE_POSITION, vNewPosition);
					if (nullptr != bMovable)
						*bMovable = true;
				}
			}

		}
	}
}


void CTransform::Set_Scale(_vector vScale)
{
	Set_State(CTransform::STATE_RIGHT,
		XMVector3Normalize(Get_State(CTransform::STATE_RIGHT)) * XMVectorGetX(vScale));
	Set_State(CTransform::STATE_UP,
		XMVector3Normalize(Get_State(CTransform::STATE_UP)) * XMVectorGetY(vScale));
	Set_State(CTransform::STATE_LOOK,
		XMVector3Normalize(Get_State(CTransform::STATE_LOOK)) * XMVectorGetZ(vScale));
}


void CTransform::Set_Scale(_float3 vScale)
{
	Set_State(CTransform::STATE_RIGHT,
		XMVector3Normalize(Get_State(CTransform::STATE_RIGHT)) * vScale.x);
	Set_State(CTransform::STATE_UP,
		XMVector3Normalize(Get_State(CTransform::STATE_UP)) * vScale.y);
	Set_State(CTransform::STATE_LOOK,
		XMVector3Normalize(Get_State(CTransform::STATE_LOOK)) * vScale.z);
}


_float3 CTransform::Get_Scale()
{
	return _float3(
		XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_RIGHT))),
		XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_UP))),
		XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_LOOK))));
}




void CTransform::Turn(_fvector vAxis, _float fSpeed, _float fTimeDelta)
{
	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fSpeed * fTimeDelta);

		Set_State(CTransform::STATE_RIGHT, XMVector3TransformNormal(Get_State(CTransform::STATE_RIGHT), RotationMatrix));
	Set_State(CTransform::STATE_UP, XMVector3TransformNormal(Get_State(CTransform::STATE_UP), RotationMatrix));
	Set_State(CTransform::STATE_LOOK, XMVector3TransformNormal(Get_State(CTransform::STATE_LOOK), RotationMatrix));
}

void CTransform::Rotation(_fvector vAxis, _float fRadian)
{
	// Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(60.0f));

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);
	_float3		Scale = Get_Scale();

	Set_State(CTransform::STATE_RIGHT, XMVector3TransformNormal(XMVectorSet(1.f, 0.f, 0.f, 0.f) * Scale.x, RotationMatrix));
	Set_State(CTransform::STATE_UP, XMVector3TransformNormal(XMVectorSet(0.f, 1.f, 0.f, 0.f) * Scale.y, RotationMatrix));
	Set_State(CTransform::STATE_LOOK, XMVector3TransformNormal(XMVectorSet(0.f, 0.f, 1.f, 0.f) * Scale.z, RotationMatrix));

}

void CTransform::Rotation_Acc(_fvector vAxis, _float fRadian)
{
	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	Set_State(CTransform::STATE_RIGHT, XMVector3TransformNormal(Get_State(CTransform::STATE_RIGHT), RotationMatrix));
	Set_State(CTransform::STATE_UP, XMVector3TransformNormal(Get_State(CTransform::STATE_UP), RotationMatrix));
	Set_State(CTransform::STATE_LOOK, XMVector3TransformNormal(Get_State(CTransform::STATE_LOOK), RotationMatrix));
}


void CTransform::LookAt(_vector vAt)
{
	_vector		vLook = vAt - Get_State(CTransform::STATE_POSITION);

	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);

	_vector		vUp = XMVector3Cross(vLook, vRight);

	_float3		vScale = Get_Scale();

	Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

void CTransform::LookAt_ForLandObject(_vector vAt)
{
	_vector		vLook = vAt - Get_State(CTransform::STATE_POSITION);
	if (XMVectorGetX(XMVector3Length(vLook)) < 0.01f)
		return;

	_float3		vScale = Get_Scale();
	_vector		vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)) * vScale.x;	
	vLook = XMVector3Normalize(XMVector3Cross(vRight, Get_State(CTransform::STATE_UP))) * vScale.z;

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_LOOK, vLook);

}

void CTransform::FixRotation(_float x, _float y, _float z)
{
	Vec3 vScaled = Get_Scale();

	XMVECTOR vRight = ::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) * vScaled.x;
	XMVECTOR vUp = ::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) * vScaled.y;
	XMVECTOR vLook = ::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f) * vScaled.z;

	Matrix mRotationMatrix = Matrix::Identity;

	m_vRotation.m128_f32[0] = x;
	m_vRotation.m128_f32[1] = y;
	m_vRotation.m128_f32[2] = z;

	mRotationMatrix *= ::XMMatrixRotationX(::XMConvertToRadians(m_vRotation.m128_f32[0]));
	mRotationMatrix *= ::XMMatrixRotationY(::XMConvertToRadians(m_vRotation.m128_f32[1]));
	mRotationMatrix *= ::XMMatrixRotationZ(::XMConvertToRadians(m_vRotation.m128_f32[2]));

	vRight = ::XMVector4Transform(vRight, mRotationMatrix);
	vUp = ::XMVector4Transform(vUp, mRotationMatrix);
	vLook = ::XMVector4Transform(vLook, mRotationMatrix);

	Set_State(STATE::STATE_RIGHT, vRight);
	Set_State(STATE::STATE_UP, vUp);
	Set_State(STATE::STATE_LOOK, vLook);
}


CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTransform*			pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CTransform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform*			pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CTransform");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CTransform::Free()
{
	__super::Free();
}
