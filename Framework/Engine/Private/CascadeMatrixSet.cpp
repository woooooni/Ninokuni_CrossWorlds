#include "..\Public\CascadeMatrixSet.h"
#include "Camera.h"
#include "Camera_Manager.h"
#include "GameInstance.h"

CCascadeMatrixSet::CCascadeMatrixSet()
{
}

void CCascadeMatrixSet::Init(int iShadowMapSize)
{
	m_iShadowMapSize = iShadowMapSize;

	//Set the range values
	m_arrCascadeRanges[0] = 0.2f;
	m_arrCascadeRanges[1] = 8.0f;
	m_arrCascadeRanges[2] = 16.0f;
	m_arrCascadeRanges[3] = 1000.0f;

	for (int i = 0; i < m_iTotalCascades; i++)
	{
		m_arrCascadeBoundCenter[i] = Vec3(0.0f, 0.0f, 0.0f);
		m_arrCascadeBoundRadius[i] = 0.0f;
	}
}

void CCascadeMatrixSet::Tick(const Vec3& vDirectionalDir)
{
	// Find the view matrix
	CCamera_Manager* pCameraManager = GET_INSTANCE(CCamera_Manager);
	if (nullptr == pCameraManager)
		return;

	CCamera* pCamera = pCameraManager->Get_CurCamera();
	if (nullptr == pCamera)
		return;

	Vec3 vCamLook = Vec3(pCamera->Get_Transform()->Get_Look());
	Vec3 vCamEye = Vec3(pCamera->Get_Transform()->Get_Position());
	_float fCurFar = pCamera->Get_ProjDesc().fFar;

	Vec3 vWorldCenter = vCamEye + vCamLook * m_fCascadeTotalRange * 0.5f;
	Vec3 vPos = vWorldCenter;
	Vec3 vLookAt = vWorldCenter + vDirectionalDir * fCurFar;
	Vec3 vUp;
	Vec3 vRight = Vec3(1.0f, 0.0f, 0.0f);
	vUp = vDirectionalDir.Cross(vRight);
	vUp.Normalize();
	Matrix mShadowView;
	mShadowView = ::XMMatrixLookAtLH(vPos, vLookAt, vUp);

	Matrix mCameraViewInv = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);
	const _float fFov = pCamera->Get_Fov();
	const _float fAspect = pCamera->Get_ProjDesc().fAspect;
	const _float fFar = pCamera->Get_ProjDesc().fFar;
	const _float fNear = pCamera->Get_ProjDesc().fNear;

	//_float fTanHalfVerticalFov = ::tanf(XMConvertToRadians(fFov * 0.5f));
	//_float fTanHalfHorizonFov = fTanHalfVerticalFov * fAspect;

	//m_fCascadeEnd[0] = fNear;
	//m_fCascadeEnd[1] = 6.0f;
	//m_fCascadeEnd[2] = 25.0f;
	//m_fCascadeEnd[3] = fFar;

	//for (_uint i = 0; i < 3; ++i)
	//{
	//	_float xn = m_fCascadeEnd[i] * fTanHalfHorizonFov;
	//	_float xf = m_fCascadeEnd[i + 1] * fTanHalfHorizonFov;

	//	_float yn = m_fCascadeEnd[i] * fTanHalfVerticalFov;
	//	_float yf = m_fCascadeEnd[i + 1] * fTanHalfVerticalFov;

	//	Vec4 vFrustumCorners[8] =
	//	{
	//		{xn, yn, m_fCascadeEnd[i], 1.0f},
	//		{-xn, yn, m_fCascadeEnd[i], 1.0f},
	//		{xn, -yn, m_fCascadeEnd[i], 1.0f},
	//		{-xn, -yn, m_fCascadeEnd[i], 1.0f},

	//		{xf, yf, m_fCascadeEnd[i + 1], 1.0f},
	//		{-xf, yf, m_fCascadeEnd[i + 1], 1.0f},
	//		{xf, -yf, m_fCascadeEnd[i + 1], 1.0f},
	//		{-xf, -yf, m_fCascadeEnd[i + 1], 1.0f}
	//	};

	//	Vec4 vCenterPos;
	//	for (_uint j = 0; j < 8; ++j)
	//	{
	//		vFrustumCorners[j] = Vec4::Transform(vFrustumCorners[j], mCameraViewInv);
	//		vCenterPos += vFrustumCorners[j];
	//	}

	//	vCenterPos /= 8.0f;
	//	_float fRadius = 0.0f;
	//	for (_uint j = 0; j < 8; ++j)
	//	{
	//		Vec4 vDistanceVector = vFrustumCorners[j] - vCenterPos;
	//		_float fDistance = vDistanceVector.Length();
	//		fRadius = max(fRadius, fDistance);
	//	}

	//	fRadius = ceil(fRadius * 16.0f) / 16.0f;
	//	Vec3 vMaxExtents = Vec3(fRadius, fRadius, fRadius);
	//	Vec3 vMinExtents = -vMaxExtents;
	//	Vec3 vShadowCamPos = Vec3(vCenterPos) + (vLightDirectional * vMinExtents.z);
	//	Matrix lightMatrix = ::XMMatrixLookAtLH(vShadowCamPos, Vec3(vCenterPos), Vec3::Up);
	//	Vec3 vCascadeExtents = vMaxExtents - vMinExtents;

	//	Matrix lightOrthoMatrix;
	//	m_shadowOrthoProj[i] = ::XMMatrixMultiply(lightMatrix, ::XMMatrixOrthographicOffCenterLH(vMinExtents.x, vMaxExtents.x, vMinExtents.y, vMaxExtents.y,
	//		0.0f, vCascadeExtents.z));

	// Get the bounds for the shadow space
	_float fRadius;
	ExtractFrustumBoundSphere(m_arrCascadeRanges[0], m_arrCascadeRanges[3], m_vShadowBoundCenter, fRadius);
	m_fShadowBoundRadius = max(m_fShadowBoundRadius, fRadius); // Expend the radius to compensate for numerical errors

	// Find the projection matrix
	Matrix mShadowProj;
	mShadowProj = ::XMMatrixOrthographicLH(m_fShadowBoundRadius, m_fShadowBoundRadius, -m_fShadowBoundRadius, m_fShadowBoundRadius);

	// The combined transformation from world to shadow space
	m_WorldToShadowSpace = mShadowView * mShadowProj;

	// For each cascade find the transformation from shadow to cascade space
	Matrix mShadowViewInv;
	mShadowViewInv = mShadowView.Invert();
	for (int iCascadeIdx = 0; iCascadeIdx < m_iTotalCascades; iCascadeIdx++)
	{
		Matrix cascadeTrans;
		Matrix cascadeScale;
		if (m_bAntiFlickerOn)
		{
			// To avoid anti flickering we need to make the transformation invariant to camera rotation and translation
			// By encapsulating the cascade frustum with a sphere we achive the rotation invariance
			Vec3 vNewCenter;
			ExtractFrustumBoundSphere(m_arrCascadeRanges[iCascadeIdx], m_arrCascadeRanges[iCascadeIdx + 1], vNewCenter, fRadius);
			m_arrCascadeBoundRadius[iCascadeIdx] = max(m_arrCascadeBoundRadius[iCascadeIdx], fRadius); // Expend the radius to compensate for numerical errors

			// Only update the cascade bounds if it moved at least a full pixel unit
			// This makes the transformation invariant to translation
			Vec3 vOffset;
			if (CascadeNeedsUpdate(mShadowView, iCascadeIdx, vNewCenter, vOffset))
			{
				// To avoid flickering we need to move the bound center in full units
				Vec3 vOffsetOut;
				vOffsetOut = Vec3::TransformNormal(vOffset, mShadowViewInv);
				m_arrCascadeBoundCenter[iCascadeIdx] += vOffsetOut;
			}

			// Get the cascade center in shadow space
			Vec3 vCascadeCenterShadowSpace;
			vCascadeCenterShadowSpace = Vec3::Transform(m_arrCascadeBoundCenter[iCascadeIdx], m_WorldToShadowSpace);

			// Update the translation from shadow to cascade space
			(&m_vToCascadeOffsetX.x)[iCascadeIdx] = -vCascadeCenterShadowSpace.x;
			(&m_vToCascadeOffsetY.x)[iCascadeIdx] = -vCascadeCenterShadowSpace.y;
			cascadeTrans = cascadeTrans.CreateTranslation((&m_vToCascadeOffsetX.x)[iCascadeIdx], (&m_vToCascadeOffsetY.x)[iCascadeIdx], 0.0f);
			//m_vToCascadeOffsetX.m128_f32[iCascadeIdx] = -vCascadeCenterShadowSpace.x;
			//m_vToCascadeOffsetY.m128_f32[iCascadeIdx] = -vCascadeCenterShadowSpace.y;
			//cascadeTrans = cascadeTrans.CreateTranslation(m_vToCascadeOffsetX.m128_f32[iCascadeIdx], m_vToCascadeOffsetY.m128_f32[iCascadeIdx], 0.0f);
	

			// Update the scale from shadow to cascade space
			(&m_vToCascadeScale.x)[iCascadeIdx] = m_fShadowBoundRadius / m_arrCascadeBoundRadius[iCascadeIdx];
			cascadeScale = cascadeScale.CreateScale((&m_vToCascadeScale.x)[iCascadeIdx], (&m_vToCascadeScale.x)[iCascadeIdx], 1.0f);
			//m_vToCascadeScale.m128_f32[iCascadeIdx] = m_fShadowBoundRadius / m_arrCascadeBoundRadius[iCascadeIdx];
			//cascadeScale = cascadeScale.CreateScale(m_vToCascadeScale.m128_f32[iCascadeIdx], m_vToCascadeScale.m128_f32[iCascadeIdx], 1.0f);
		}
		else
		{
			// Since we don't care about flickering we can make the cascade fit tightly around the frustum
			// Extract the bounding box
			Vec3 arrFrustumPoints[8];
			ExtractFrustumPoints(m_arrCascadeRanges[iCascadeIdx], m_arrCascadeRanges[iCascadeIdx + 1], arrFrustumPoints);

			// Transform to shadow space and extract the minimum andn maximum
			Vec3 vMin = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
			Vec3 vMax = Vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
			for (int i = 0; i < 8; i++)
			{
				Vec3 vPointInShadowSpace;
				vPointInShadowSpace = Vec3::Transform(arrFrustumPoints[i], m_WorldToShadowSpace);

				for (int j = 0; j < 3; j++)
				{
					if ((&vMin.x)[j] > (&vPointInShadowSpace.x)[j])
						(&vMin.x)[j] = (&vPointInShadowSpace.x)[j];
					if ((&vMax.x)[j] < (&vPointInShadowSpace.x)[j])
						(&vMax.x)[j] = (&vPointInShadowSpace.x)[j];
				}
			}

			Vec3 vCascadeCenterShadowSpace = 0.5f * (vMin + vMax);

			// Update the translation from shadow to cascade space
			(&m_vToCascadeOffsetX.x)[iCascadeIdx] = -vCascadeCenterShadowSpace.x;
			(&m_vToCascadeOffsetY.x)[iCascadeIdx] = -vCascadeCenterShadowSpace.y;
			cascadeTrans = cascadeTrans.CreateTranslation((&m_vToCascadeOffsetX.x)[iCascadeIdx], (&m_vToCascadeOffsetY.x)[iCascadeIdx], 0.0f);
			//m_vToCascadeOffsetX.m128_f32[iCascadeIdx] = -vCascadeCenterShadowSpace.x;
			//m_vToCascadeOffsetY.m128_f32[iCascadeIdx] = -vCascadeCenterShadowSpace.y;
			//cascadeTrans = cascadeTrans.CreateTranslation(m_vToCascadeOffsetX.m128_f32[iCascadeIdx], m_vToCascadeOffsetY.m128_f32[iCascadeIdx], 0.0f);

			// Update the scale from shadow to cascade space
			(&m_vToCascadeScale.x)[iCascadeIdx] = 2.0f / max(vMax.x - vMin.x, vMax.y - vMin.y);
			cascadeScale = cascadeScale.CreateScale((&m_vToCascadeScale.x)[iCascadeIdx], (&m_vToCascadeScale.x)[iCascadeIdx], 1.0f);
			//m_vToCascadeScale.m128_f32[iCascadeIdx] = 2.0f / max(vMax.x - vMin.x, vMax.y - vMin.y);
			//cascadeScale = cascadeScale.CreateScale(m_vToCascadeScale.m128_f32[iCascadeIdx], m_vToCascadeScale.m128_f32[iCascadeIdx], 1.0f);
		}

		// Combine the matrices to get the transformation from world to cascade space
		m_arrWorldToCascadeProj[iCascadeIdx] = m_WorldToShadowSpace * cascadeTrans * cascadeScale;
	}

	// Set the values for the unused slots to someplace outside the shadow space
	for (int i = m_iTotalCascades; i < 4; i++)
	{
		(&m_vToCascadeOffsetX.x)[i] = 250.0f;
		(&m_vToCascadeOffsetY.x)[i] = 250.0f;
		(&m_vToCascadeScale.x)[i] = 0.1f;
	/*	m_vToCascadeOffsetX.m128_f32[i] = 250.0f;
		m_vToCascadeOffsetY.m128_f32[i] = 250.0f;
		m_vToCascadeScale.m128_f32[i] = 0.1f;*/
	}
}

void CCascadeMatrixSet::SubdivisionFrustum(const Vec3& vLightDirectional)
{
	CCamera_Manager* pCameraManager = GET_INSTANCE(CCamera_Manager);
	if (nullptr == pCameraManager)
		return;

	CCamera* pCamera = pCameraManager->Get_CurCamera();
	if (nullptr == pCamera)
		return;

	// 카메라의 역행렬과 시야각, 화면비, Far, Near
	Matrix mCameraViewInv = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);
	const _float fFov = pCamera->Get_Fov();
	const _float fAspect = pCamera->Get_ProjDesc().fAspect;
	const _float fFar = pCamera->Get_ProjDesc().fFar;
	const _float fNear = pCamera->Get_ProjDesc().fNear;

	_float fTanHalfVerticalFov = ::tanf(XMConvertToRadians(fFov * 0.5f));
	_float fTanHalfHorizonFov = fTanHalfVerticalFov * fAspect;

	m_fCascadeEnd[0] = fNear;
	m_fCascadeEnd[1] = 6.0f;
	m_fCascadeEnd[2] = 25.0f;
	m_fCascadeEnd[3] = fFar;

	for (_uint i = 0; i < 3; ++i)
	{
		_float xn = m_fCascadeEnd[i] * fTanHalfHorizonFov;
		_float xf = m_fCascadeEnd[i + 1] * fTanHalfHorizonFov;

		_float yn = m_fCascadeEnd[i] * fTanHalfVerticalFov;
		_float yf = m_fCascadeEnd[i + 1] * fTanHalfVerticalFov;

		Vec4 vFrustumCorners[8] =
		{
			{xn, yn, m_fCascadeEnd[i], 1.0f},
			{-xn, yn, m_fCascadeEnd[i], 1.0f},
			{xn, -yn, m_fCascadeEnd[i], 1.0f},
			{-xn, -yn, m_fCascadeEnd[i], 1.0f},

			{xf, yf, m_fCascadeEnd[i + 1], 1.0f},
			{-xf, yf, m_fCascadeEnd[i + 1], 1.0f},
			{xf, -yf, m_fCascadeEnd[i + 1], 1.0f},
			{-xf, -yf, m_fCascadeEnd[i + 1], 1.0f}
		};

		Vec4 vCenterPos;
		for (_uint j = 0; j < 8; ++j)
		{
			vFrustumCorners[j] = Vec4::Transform(vFrustumCorners[j], mCameraViewInv);
			vCenterPos += vFrustumCorners[j];
		}

		vCenterPos /= 8.0f;
		_float fRadius = 0.0f;
		for (_uint j = 0; j < 8; ++j)
		{
			Vec4 vDistanceVector = vFrustumCorners[j] - vCenterPos;
			_float fDistance = vDistanceVector.Length();
			fRadius = max(fRadius, fDistance);
		}

		fRadius = ceil(fRadius * 16.0f) / 16.0f;
		Vec3 vMaxExtents = Vec3(fRadius, fRadius, fRadius);
		Vec3 vMinExtents = -vMaxExtents;
		Vec3 vShadowCamPos = Vec3(vCenterPos) + (vLightDirectional * vMinExtents.z);
		Matrix lightMatrix = ::XMMatrixLookAtLH(vShadowCamPos, Vec3(vCenterPos), Vec3::Up);
		Vec3 vCascadeExtents = vMaxExtents - vMinExtents;

		Matrix lightOrthoMatrix;
		m_shadowOrthoProj[i] = ::XMMatrixMultiply(lightMatrix, ::XMMatrixOrthographicOffCenterLH(vMinExtents.x, vMaxExtents.x, vMinExtents.y, vMaxExtents.y,
			0.0f, vCascadeExtents.z));

		//Vec4 vShadowOrigin = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
		//vShadowOrigin = Vec4::Transform(vShadowOrigin, (lightOrthoMatrix * lightMatrix));
		//vShadowOrigin = vShadowOrigin * (1024.0f / 2.0f);
		//
		//Vec4 vRoundedOrigin;
		//vRoundedOrigin.x = ::roundf(vShadowOrigin.x);
		//vRoundedOrigin.y = ::roundf(vShadowOrigin.y);
		//vRoundedOrigin.z = ::roundf(vShadowOrigin.z);
		//vRoundedOrigin.w = ::roundf(vShadowOrigin.w);

		//Vec4 vRoundOffset = vRoundedOrigin - vShadowOrigin;

		//vRoundOffset = vRoundOffset * 2.0f / 1024.0f;
		//vRoundOffset.z = 0.0f;
		//vRoundOffset.w = 0.0f;

		//lightOrthoMatrix._41 += vRoundOffset.x;
		//lightOrthoMatrix._42 += vRoundOffset.y;
		//lightOrthoMatrix._43 += vRoundOffset.z;
		//lightOrthoMatrix._44 += vRoundOffset.w;

		//m_shadowOrthoProj[i] = lightOrthoMatrix * lightMatrix;
	}
}


void CCascadeMatrixSet::ExtractFrustumPoints(_float fNear, _float fFar, Vec3* arrFrustumCorners)
{
	CCamera_Manager* pCameraManager = GET_INSTANCE(CCamera_Manager);
	if (nullptr == pCameraManager)
		return;

	CCamera* pCamera = pCameraManager->Get_CurCamera();
	if (nullptr == pCamera)
		return;

	// Get the camera bases
	const Vec3& camPos = Vec3(pCamera->Get_Transform()->Get_Position());
	const Vec3& camRight = pCamera->Get_Transform()->Get_Right();
	const Vec3& camUp = pCamera->Get_Transform()->Get_Up();
	const Vec3& camForward = pCamera->Get_Transform()->Get_Look();

	_float fAspect = pCamera->Get_ProjDesc().fAspect;
	_float fFov = pCamera->Get_Fov();
	// Calculate the tangent values (this can be cached
	const _float fTanFOVX = tanf(fAspect * fFov);
	const _float fTanFOVY = tanf(fAspect);

	// Calculate the points on the near plane
	arrFrustumCorners[0] = camPos + (-camRight * fTanFOVX + camUp * fTanFOVY + camForward) * fNear;
	arrFrustumCorners[1] = camPos + (camRight * fTanFOVX + camUp * fTanFOVY + camForward) * fNear;
	arrFrustumCorners[2] = camPos + (camRight * fTanFOVX - camUp * fTanFOVY + camForward) * fNear;
	arrFrustumCorners[3] = camPos + (-camRight * fTanFOVX - camUp * fTanFOVY + camForward) * fNear;

	// Calculate the points on the far plane
	arrFrustumCorners[4] = camPos + (-camRight * fTanFOVX + camUp * fTanFOVY + camForward) * fFar;
	arrFrustumCorners[5] = camPos + (camRight * fTanFOVX + camUp * fTanFOVY + camForward) * fFar;
	arrFrustumCorners[6] = camPos + (camRight * fTanFOVX - camUp * fTanFOVY + camForward) * fFar;
	arrFrustumCorners[7] = camPos + (-camRight * fTanFOVX - camUp * fTanFOVY + camForward) * fFar;
}

void CCascadeMatrixSet::ExtractFrustumBoundSphere(_float fNear, _float fFar, Vec3& vBoundCenter, _float& fBoundRadius)
{

	CCamera_Manager* pCameraManager = GET_INSTANCE(CCamera_Manager);
	if (nullptr == pCameraManager)
		return;

	CCamera* pCamera = pCameraManager->Get_CurCamera();
	if (nullptr == pCamera)
		return;

	// Get the camera bases
	const Vec3& camPos = Vec3(pCamera->Get_Transform()->Get_Position());
	const Vec3& camRight = pCamera->Get_Transform()->Get_Right();
	const Vec3& camUp = pCamera->Get_Transform()->Get_Up();
	const Vec3& camForward = pCamera->Get_Transform()->Get_Look();

	_float fAspect = pCamera->Get_ProjDesc().fAspect;
	_float fFov = pCamera->Get_Fov();
	// Calculate the tangent values (this can be cached as long as the FOV doesn't change)
	const _float fTanFOVX = tanf(fAspect * fFov);
	const _float fTanFOVY = tanf(fAspect);

	// The center of the sphere is in the center of the frustum
	vBoundCenter = camPos + camForward * (fNear + 0.5f * (fNear + fFar));

	// Radius is the distance to one of the frustum far corners
	const Vec3 vBoundSpan = camPos + (-camRight * fTanFOVX + camUp * fTanFOVY + camForward) * fFar - vBoundCenter;
	fBoundRadius = vBoundSpan.Length();
}

bool CCascadeMatrixSet::CascadeNeedsUpdate(const Matrix& mShadowView, _int iCascadeIdx, const Vec3& newCenter, Vec3& vOffset)
{
	// Find the offset between the new and old bound ceter
	Vec3 vOldCenterInCascade;
	vOldCenterInCascade = Vec3::Transform(m_arrCascadeBoundCenter[iCascadeIdx], mShadowView);
	Vec3 vNewCenterInCascade;
	vNewCenterInCascade = Vec3::Transform(newCenter, mShadowView);
	Vec3 vCenterDiff = vNewCenterInCascade - vOldCenterInCascade;

	// Find the pixel size based on the diameters and map pixel size
	_float fPixelSize = static_cast<_float>(m_iShadowMapSize) / (2.0f * m_arrCascadeBoundRadius[iCascadeIdx]);

	_float fPixelOffX = vCenterDiff.x * fPixelSize;
	_float fPixelOffY = vCenterDiff.y * fPixelSize;

	// Check if the center moved at least half a pixel unit
	_bool bNeedUpdate = abs(fPixelOffX) > 0.5f || abs(fPixelOffY) > 0.5f;
	if (bNeedUpdate)
	{
		// Round to the 
		vOffset.x = floorf(0.5f + fPixelOffX) / fPixelSize;
		vOffset.y = floorf(0.5f + fPixelOffY) / fPixelSize;
		vOffset.z = vCenterDiff.z;
	}

	return bNeedUpdate;
}

void CCascadeMatrixSet::Free()
{
	__super::Free();
}
