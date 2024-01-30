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
	, m_vRotation(rhs.m_vRotation)
	, m_fTime(rhs.m_fTime)
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

void CTransform::Translate(const Vec3& vTranslation)
{
	for (_uint i = 0; i < 3; ++i)
		*((_float*)(&m_WorldMatrix.m[3]) + i) += *((_float*)&vTranslation + i);
}

Vec4 CTransform::Get_RelativeOffset(Vec4 vPos)
{
	Matrix matWorld = m_WorldMatrix;

	matWorld.Translation(Vec3::Zero);

	matWorld.Right(XMVector3Normalize(matWorld.Right()));
	matWorld.Up(XMVector3Normalize(matWorld.Up()));
	matWorld.Backward(XMVector3Normalize(matWorld.Backward()));

	Vec3 vRelativePos = XMVector3TransformCoord(vPos, matWorld);

	return Vec4(vRelativePos.x, vRelativePos.y, vRelativePos.z, 1.f);
}

void CTransform::Set_State(STATE eState, _vector vState)
{
	_matrix	WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
	
	if (eState == STATE::STATE_POSITION)	
		XMVectorSetW(vState, 1.f);

	WorldMatrix.r[eState] = vState;
	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);
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
	Vec4		vLook = vAt - Get_State(CTransform::STATE_POSITION);
	vLook.Normalize();
	
	Vec4		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	vRight.Normalize();

	Vec4		vUp = XMVector3Cross(vLook, vRight);
	vUp.Normalize();

	_float3		vScale = Get_Scale();

 	Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
	Set_State(CTransform::STATE_UP, vUp * vScale.y);
	Set_State(CTransform::STATE_LOOK, vLook * vScale.z);
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

void CTransform::Rotation_Look(_vector vLook)
{
	Vec3 vScale = Get_Scale();
	
	_matrix WorldMatrix = Get_WorldMatrix();

	Vec4 vNewLook = XMVector3Normalize(vLook);
	Vec4 vNewRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vNewLook));
	Vec4 vNewUp = XMVector3Normalize(XMVector3Cross(vNewLook, vNewRight));


	WorldMatrix.r[CTransform::STATE_RIGHT] = vNewRight * vScale.x;
	WorldMatrix.r[CTransform::STATE_UP] = vNewUp * vScale.y;
	WorldMatrix.r[CTransform::STATE_LOOK] = vNewLook * vScale.z;

	Set_WorldMatrix(WorldMatrix);

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

void CTransform::RevolutionRotation(const Vec3& vPoint, const Vec3 vAxis, const _float& fAngle)
{
	Quaternion quat;
	Matrix matRotate;
	Vec3 vOut;

	Vec3 vPos = Get_Position();

	quat = ::XMQuaternionRotationAxis(vAxis, fAngle);
	matRotate = ::XMMatrixRotationQuaternion(quat);
	vOut = XMVector3TransformCoord(vPos - vPoint, matRotate);

	for (_uint i = 0; i < STATE::STATE_POSITION; ++i)
		Set_State(static_cast<STATE>(i), ::XMVector3TransformNormal(Get_State(static_cast<STATE>(i)), matRotate));

	Vec3 vResultPos = vOut + vPoint;
	Vec4 vFinalPos = Vec4(vResultPos.x, vResultPos.y, vResultPos.z, 1.0f);
	Set_State(CTransform::STATE_POSITION, vFinalPos);
}

void CTransform::ParabolicFormula(const Vec3& startPos, const Vec3& endPos, const _float fTimeDelta, const _float& maxHeight, const _float& maxTime)
{
	Vec3 vStarPos = startPos; // 시작할 위치
	Vec3 vEndPos = endPos; // 끝날 위치

	_float fVelocityX, fVelocityY, fVelocityZ;

	_float fMaxHeight = maxHeight; // 최대 높이
	_float fTimeToMaxHeight = maxTime; // 최대 높이까지 가는 시간 

	_float fEndHeight = vEndPos.y - vStarPos.y;
	_float fHeight = fMaxHeight - vStarPos.y;
	_float fGravity = 2.0f * fHeight / (fTimeToMaxHeight * fTimeToMaxHeight);

	fVelocityY = ::sqrtf(2 * fGravity * fHeight);

	_float a = fGravity;
	_float b = -2.0f * fVelocityY;
	_float c = 2.0f * fEndHeight;

	
	_float fEndTime = (-b + ::sqrtf(b * b - 4 * a * c)) / (2.0f * a);
	if (true == std::isnan(fEndTime))
		fEndTime = 3.0f;
	

	fVelocityX = -(vStarPos.x - vEndPos.x) / fEndTime;
	fVelocityZ = -(vStarPos.z - vEndPos.z) / fEndTime;

	m_fTime += fTimeDelta;

	Vec4 vPos;
	vPos.x = vStarPos.x + fVelocityX * m_fTime;
	vPos.y = vStarPos.y + (fVelocityY * m_fTime) - (0.5f * fGravity * m_fTime * m_fTime);
	vPos.z = vStarPos.z + fVelocityZ * m_fTime;
	vPos.w = 1.0f;

	Set_State(CTransform::STATE_POSITION, vPos);
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
