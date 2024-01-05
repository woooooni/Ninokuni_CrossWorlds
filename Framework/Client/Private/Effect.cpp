#include "stdafx.h"
#include "Effect.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Utils.h"

CEffect::CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_EFFECT)
{
}

CEffect::CEffect(const CEffect& rhs)
	: CGameObject(rhs), m_tEffectDesc(rhs.m_tEffectDesc)
{
}

void CEffect::Set_EffectDesc(const EFFECT_DESC& tDesc)
{
	m_tEffectDesc = tDesc;

	if (m_tEffectDesc.eType == EFFECT_TYPE::EFFECT_MESH)
	{
		Set_Model();
		if (m_pVIBufferCom != nullptr)
			Safe_Release(m_pVIBufferCom);
	}
	else if (m_tEffectDesc.eType == EFFECT_TYPE::EFFECT_TEXTURE)
	{
		if (m_pVIBufferCom == nullptr)
			m_pVIBufferCom = static_cast<CVIBuffer_Rect*>(GI->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect")));
	}

	Set_Texture_Diffuse();
	Set_Texture_Alpha();

	Reset_Effect();
}

void CEffect::Set_Gravity(_bool bGravity)
{

}

void CEffect::Reset_UV()
{
	m_bAccIndexEnd = false;

	m_fAccIndex  = 0.f;
	m_fAccUVFlow = _float2(0.f, 0.f);
}

void CEffect::Reset_Effect()
{
	if (m_pTransformCom == nullptr)
		return;

	m_bEffectDie       = false;
	m_fAccDeletionTime = 0.f;

	// 위치 조정
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vPosition = XMVectorSet(
		XMVectorGetX(vPosition) + CUtils::Random_Float(-m_tEffectDesc.fRange.x, m_tEffectDesc.fRange.x),
		XMVectorGetY(vPosition) + CUtils::Random_Float(-m_tEffectDesc.fRange.y, m_tEffectDesc.fRange.y),
		XMVectorGetZ(vPosition) + CUtils::Random_Float(-m_tEffectDesc.fRange.z, m_tEffectDesc.fRange.z),
		XMVectorGetW(vPosition));

#pragma region 크기
	if (m_tEffectDesc.bScaleSameRate) {
		_float fScale = CUtils::Random_Float(m_tEffectDesc.fScaleStartMin.x, m_tEffectDesc.fScaleStartMax.x);
		m_pTransformCom->Set_Scale(_float3(fScale, fScale, fScale));
	}
	else
	{
		m_pTransformCom->Set_Scale(_float3(
			CUtils::Random_Float(m_tEffectDesc.fScaleStartMin.x, m_tEffectDesc.fScaleStartMax.x),
			CUtils::Random_Float(m_tEffectDesc.fScaleStartMin.y, m_tEffectDesc.fScaleStartMax.y),
			CUtils::Random_Float(m_tEffectDesc.fScaleStartMin.z, m_tEffectDesc.fScaleStartMax.z)));
	}

	if (m_tEffectDesc.bScaleChange)
	{
		m_fScaleSpeed = CUtils::Random_Float(m_tEffectDesc.fScaleSpeed.x, m_tEffectDesc.fScaleSpeed.y);

		m_fScaleTimeAccs = 0.f;
		m_fScaleChange   = CUtils::Random_Float(m_tEffectDesc.fScaleChangeTime.x, m_tEffectDesc.fScaleChangeTime.y);

		m_fScaleChangeStartTime  = 0.f;
		m_fScaleChangeStartDelay = CUtils::Random_Float(m_tEffectDesc.fScaleChangeStartDelay.x, m_tEffectDesc.fScaleChangeStartDelay.y);
	}
#pragma endregion

#pragma region 이동 방향
	m_fVelocitySpeed = CUtils::Random_Float(m_tEffectDesc.fVelocitySpeed.x, m_tEffectDesc.fVelocitySpeed.y);

	m_fVelocity = _float3(CUtils::Random_Float(m_tEffectDesc.vVelocityMinStart.x, m_tEffectDesc.vVelocityMaxStart.x),
		CUtils::Random_Float(m_tEffectDesc.vVelocityMinStart.y, m_tEffectDesc.vVelocityMaxStart.y),
		CUtils::Random_Float(m_tEffectDesc.vVelocityMinStart.z, m_tEffectDesc.vVelocityMaxStart.z));

	if (m_tEffectDesc.bVelocityChange)
	{
		m_fVelocityChangeStartTime = 0.f;
		m_fVelocityChangeStartDelay = CUtils::Random_Float(m_tEffectDesc.fVelocityChangeStartDelay.x, m_tEffectDesc.fVelocityChangeStartDelay.y);

		m_fVelocityRanTimeAccs = 0.f;
		m_fVelocityRanChange = CUtils::Random_Float(m_tEffectDesc.fVelocityChangeTime.x, m_tEffectDesc.fVelocityChangeTime.y);
	}
#pragma endregion

	// 시작 위치 거리 셋팅
	_float fDistance = CUtils::Random_Float(m_tEffectDesc.fRangeDistance.x, m_tEffectDesc.fRangeDistance.y);
	vPosition = XMVectorSet(
		XMVectorGetX(vPosition) + m_fVelocity.x * fDistance,
		XMVectorGetY(vPosition) + m_fVelocity.y * fDistance,
		XMVectorGetZ(vPosition) + m_fVelocity.z * fDistance,
		XMVectorGetW(vPosition));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

#pragma region 회전
	if (!m_tEffectDesc.bBillboard)
	{
		if (m_tEffectDesc.bRandomAxis)
		{
			_uint iRandomCount = (_uint)CUtils::Random_Float(0.f, 2.9);
			switch (iRandomCount)
			{
			case 0:
				m_fAxis = _float3(1.f, 0.f, 0.f);
				break;
			case 1:
				m_fAxis = _float3(0.f, 1.f, 0.f);
				break;
			default:
				m_fAxis = _float3(0.f, 0.f, 1.f);
				break;
			}
		}
		else
			m_fAxis = m_tEffectDesc.fAxis;

		if (m_tEffectDesc.bRandomAngle)
			m_fAngle = CUtils::Random_Float(0.f, 360.f);
		else
			m_fAngle = m_tEffectDesc.fAngle;

		m_pTransformCom->FixRotation(m_fAxis.x * m_fAngle, m_fAxis.y * m_fAngle, m_fAxis.z * m_fAngle);


		if (m_tEffectDesc.bRotationChange)
		{
			m_fRotationSpeed = CUtils::Random_Float(m_tEffectDesc.fRotationSpeed.x, m_tEffectDesc.fRotationSpeed.y);

			m_fRotationChangeStartTime  = 0.f;
			m_fRotationChangeStartDelay = CUtils::Random_Float(m_tEffectDesc.fRotationChangeStartDelay.x, m_tEffectDesc.fRotationChangeStartDelay.y);

			if (m_tEffectDesc.bRotationChangeRandom)
			{
				m_fRotationTime       = 0.f;
				m_fRotationChangeTime = CUtils::Random_Float(m_tEffectDesc.fRotationChangeTime.x, m_tEffectDesc.fRotationChangeTime.y);
			}
		}
	}
#pragma endregion

	// 지속 시간
	m_fLifeTime = CUtils::Random_Float(m_tEffectDesc.fLifeTime.x, m_tEffectDesc.fLifeTime.y);

#pragma region 텍스처 정보
	m_bAccIndexEnd    = false;
	m_fAccIndex       = 0.f;
	m_fAnimationSpeed = CUtils::Random_Float(m_tEffectDesc.fAnimationSpeed.x, m_tEffectDesc.fAnimationSpeed.y);

	if (m_tEffectDesc.bRandomStartIndex) 
	{
		m_tEffectDesc.fUVIndex = _float2(
			(_int)CUtils::Random_Float(0.f, m_tEffectDesc.fMaxCount.x),
			(_int)CUtils::Random_Float(0.f, m_tEffectDesc.fMaxCount.y));
	}

	m_fUVFlowSpeed = CUtils::Random_Float(m_tEffectDesc.fUVFlowSpeed.x, m_tEffectDesc.fUVFlowSpeed.y);
#pragma endregion

#pragma region 알파
	m_fAlpha = CUtils::Random_Float(m_tEffectDesc.fAlphaStart.x, m_tEffectDesc.fAlphaStart.y);
	if (m_tEffectDesc.bAlphaChange)
	{
		m_fAlphaChangeStartTime  = 0.f;
		m_fAlphaChangeStartDelay = CUtils::Random_Float(m_tEffectDesc.fAlphaChangeStartDelay.x, m_tEffectDesc.fAlphaChangeStartDelay.y);
	}

	if (m_tEffectDesc.bAlphaCreate)
	{
		m_fAlpha           = 0.f;
		m_bAlphaCreateSucc = false;
	}
	else if (m_tEffectDesc.bAlphaDelete)
		m_bAlphaCreateSucc = true;

	m_fAlphaSpeed = CUtils::Random_Float(m_tEffectDesc.fAlphaSpeed.x, m_tEffectDesc.fAlphaSpeed.y);
#pragma endregion

#pragma region 색상
	if (m_tEffectDesc.bColorChange)
	{
		m_LerpInfo             = {};
		m_fColorChangeDuration = CUtils::Random_Float(m_tEffectDesc.fColorDuration.x, m_tEffectDesc.fColorDuration.y);

		if (m_tEffectDesc.bColorChangeRandom)
		{
			m_fNextColor = _float3(CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.f, 1.f));
			m_fColorAccs = 0.f;
			m_fColorChangeTime = CUtils::Random_Float(m_tEffectDesc.fColorChangeRandomTime.x, m_tEffectDesc.fColorChangeRandomTime.y);

			m_fColorFade = false;
		}
		else
		{
			m_fColorChangeStartTime  = 0.f;
			m_fColorChangeStartDelay = CUtils::Random_Float(m_tEffectDesc.fColorChangeStartDelay.x, m_tEffectDesc.fColorChangeStartDelay.y);

			m_iColorIndex = 0;
			m_fColorChangeStartM = CUtils::Random_Float(m_tEffectDesc.fColorChangeStartM.x, m_tEffectDesc.fColorChangeStartM.y);
			m_fColorChangeStartF = CUtils::Random_Float(m_tEffectDesc.fColorChangeStartF.x, m_tEffectDesc.fColorChangeStartF.y);
		}
	}

	if (m_tEffectDesc.bColorRandom)
		m_fColor = _float3(CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.f, 1.f));
	else
		m_fColor = _float3(m_tEffectDesc.fColorS.x, m_tEffectDesc.fColorS.y, m_tEffectDesc.fColorS.z);
#pragma endregion

#pragma region 블러
	if (m_tEffectDesc.bBloomPowerRandom)
		m_fBlurColor = _float3(CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.f, 1.f));
	else
		m_fBlurColor = _float3(m_tEffectDesc.fBloomPower.x, m_tEffectDesc.fBloomPower.y, m_tEffectDesc.fBloomPower.z);

	if (m_tEffectDesc.bBlurPowerRandom)
		m_fBlurPower = CUtils::Random_Float(0.1f, 1.f);
	else
		m_fBlurPower = m_tEffectDesc.fBlurPower;
#pragma endregion

	Reset_UV();
}

HRESULT CEffect::Initialize_Prototype(const EFFECT_DESC* pEffectDesc)
{
	if (pEffectDesc != nullptr)
		m_tEffectDesc = *pEffectDesc;

	return S_OK;
}

HRESULT CEffect::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (nullptr != pArg)
		m_pOwnerObject = (CGameObject*)pArg;

	XMStoreFloat4x4(&m_ParentMatrix, XMMatrixIdentity());
	Reset_Effect();

	return S_OK;
}

void CEffect::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return;

	// 지속 시간
	m_fAccDeletionTime += fTimeDelta;
	if (m_bEffectDie || m_fAccDeletionTime >= m_fLifeTime)
	{
		m_bEffectDie = true;
		if (GI->Get_CurrentLevel() != LEVEL_TOOL)
			Set_Dead(true);
    }

	if (m_bDead || m_bEffectDie)
		return;

	// 알파
	Change_Alpha(fTimeDelta);

	// 크기
	Change_Scale(fTimeDelta);

	// 이동
	Change_Move(fTimeDelta);

	// 회전
	Change_Rotation(fTimeDelta);

	// 애니메이션
	if (m_tEffectDesc.bAnimation)
	{
		if (m_tEffectDesc.bIncrement)
			Increment(fTimeDelta);
		else
			Decrement(fTimeDelta);
	}

	// UVFlow
	if (m_tEffectDesc.bUVFlowChange)
	{
		m_fAccUVFlow.x += m_tEffectDesc.fUVFlowDir.x * m_fUVFlowSpeed * fTimeDelta;
		m_fAccUVFlow.y += m_tEffectDesc.fUVFlowDir.y * m_fUVFlowSpeed * fTimeDelta;
	}

	// 색상
	Change_Color(fTimeDelta);

	// m_ParentMatrix
	if (nullptr != m_pOwnerObject)
	{
		CTransform* pOwnerTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");
		if (nullptr != pOwnerTransform)
			XMStoreFloat4x4(&m_ParentMatrix, pOwnerTransform->Get_WorldMatrix());
	}
}

void CEffect::LateTick(_float fTimeDelta)
{
	if (m_bDead || m_bEffectDie)
		return;

	__super::LateTick(fTimeDelta);

	_float4x4 WorldMatrix;
	_matrix OffetMatrix = XMLoadFloat4x4(&m_tEffectDesc.OffsetMatrix);
	XMStoreFloat4x4(&WorldMatrix, m_pTransformCom->Get_WorldMatrix());

	// 빌보드
	if (m_tEffectDesc.bBillboard)
	{
		_matrix Temp         = XMLoadFloat4x4(&WorldMatrix);
		_vector vPosition    = Temp.r[CTransform::STATE_POSITION];
		_vector vCamPosition = XMLoadFloat4(&GI->Get_CamPosition());

		_float fLookScale = XMVectorGetX(XMVector3Length(Temp.r[CTransform::STATE_LOOK]));
		_vector vLook     = XMVectorSetW(XMVector3Normalize(vPosition - vCamPosition), 0.f) * fLookScale;

		_float fRightScale = XMVectorGetX(XMVector3Length(Temp.r[CTransform::STATE_RIGHT]));
		_vector vRight     = XMVectorSetW(XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)), 0.f) * fRightScale;

		_float fUpScale = XMVectorGetX(XMVector3Length(Temp.r[CTransform::STATE_UP]));
		_vector vUp     = XMVectorSetW(XMVector3Normalize(XMVector3Cross(vLook, vRight)), 0.f) * fUpScale;

		Temp.r[CTransform::STATE_RIGHT] = vRight;
		Temp.r[CTransform::STATE_UP]    = vUp;
		Temp.r[CTransform::STATE_LOOK]  = vLook;

		XMStoreFloat4x4(&WorldMatrix, Temp);
	}
	
	// 리지드바디
	if(m_tEffectDesc.bGravity)
		m_pRigidBodyCom->Update_RigidBody(fTimeDelta);

	// 인스턴싱 데이터
	CRenderer::EFFECT_INSTANCE_DESC EffectInstanceDesc;
	ZeroMemory(&EffectInstanceDesc, sizeof CRenderer::EFFECT_INSTANCE_DESC);
	EffectInstanceDesc.g_fUVIndex  = m_tEffectDesc.fUVIndex;
	EffectInstanceDesc.g_fMaxCount = m_tEffectDesc.fMaxCount;
	EffectInstanceDesc.g_fUVFlow   = m_fAccUVFlow;
	EffectInstanceDesc.g_iUVLoop   = m_tEffectDesc.iUVFlowLoop;
	EffectInstanceDesc.g_fAlpha    = m_fAlpha;
	EffectInstanceDesc.g_fAdditiveDiffuseColor = _float4(m_fColor.x, m_fColor.y, m_fColor.z, 1.f); // Add 컬러
	EffectInstanceDesc.g_vBlurColor = m_fBlurColor;
	EffectInstanceDesc.g_fBlurPower = m_fBlurPower;

	// 컬링
	if (true == GI->Intersect_Frustum_World(XMLoadFloat4x4(&WorldMatrix).r[CTransform::STATE_POSITION], 10.f))
	{
		if (m_tEffectDesc.eType == EFFECT_TYPE::EFFECT_MESH)
			m_pRendererCom->Add_RenderGroup_Instancing_Effect(CRenderer::RENDER_EFFECT, CRenderer::INSTANCING_SHADER_TYPE::EFFECT_MODEL, this, WorldMatrix, EffectInstanceDesc);
		else
			m_pRendererCom->Add_RenderGroup_Instancing_Effect(CRenderer::RENDER_EFFECT, CRenderer::INSTANCING_SHADER_TYPE::EFFECT_TEXTURE, this, WorldMatrix, EffectInstanceDesc);
	}
}


HRESULT CEffect::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (m_bDead && GI->Get_CurrentLevel() != LEVEL_TOOL || m_tEffectDesc.eType == EFFECT_TYPE::EFFECT_END)
		return S_OK;

	if (FAILED(Bind_ShaderResource_Instance(pInstancingShader)))
		return E_FAIL;

	if (m_tEffectDesc.eType == EFFECT_TYPE::EFFECT_MESH)
	{
		if(m_pModelCom == nullptr)
			return S_OK;

		_uint iNumMesh = m_pModelCom->Get_NumMeshes();
		for (_uint i = 0; i < iNumMesh; ++i)
		{
			if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, i, pInstancingBuffer, WorldMatrices, m_tEffectDesc.iShaderPass)))
				return E_FAIL;
		}
	}
	else
	{
		pInstancingShader->Begin(m_tEffectDesc.iShaderPass);
		pInstancingBuffer->Render(WorldMatrices, m_pVIBufferCom);
	}

	return S_OK;
}

HRESULT CEffect::Bind_ShaderResource_Instance(CShader* pShader)
{
	if (FAILED(pShader->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(pShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;


	if (FAILED(pShader->Bind_RawValue("g_fAlpha_Discard", &m_tEffectDesc.fAlpha_Discard, sizeof(_float))))
		return E_FAIL;

	if (FAILED(pShader->Bind_RawValue("g_fBlack_Discard", &m_tEffectDesc.fBlack_Discard, sizeof(_float3))))
		return E_FAIL;


	if (FAILED(pShader->Bind_RawValue("g_fBlurPower", &m_tEffectDesc.fBlurPower, sizeof(_float))))
		return E_FAIL;

	if (m_pDiffuseTextureCom != nullptr && -1 < m_tEffectDesc.iTextureIndexDiffuse) {
		if (FAILED(m_pDiffuseTextureCom->Bind_ShaderResource(pShader, "g_DiffuseTexture", m_tEffectDesc.iTextureIndexDiffuse)))
			return E_FAIL;
	}
	if (m_pAlphaTextureCom != nullptr && -1 < m_tEffectDesc.iTextureIndexAlpha) {
		if (FAILED(m_pAlphaTextureCom->Bind_ShaderResource(pShader, "g_AlphaTexture", m_tEffectDesc.iTextureIndexAlpha)))
			return E_FAIL;
	}

	if (m_tEffectDesc.iShaderPass <= 3)
	{
		// 둘다 없다면
		if (m_pDiffuseTextureCom == nullptr && m_pAlphaTextureCom == nullptr)
			m_tEffectDesc.iShaderPass = 0;
		// 디퓨즈 텍스쳐만 있다면
		else if (m_pDiffuseTextureCom != nullptr && m_pAlphaTextureCom == nullptr)
			m_tEffectDesc.iShaderPass = 1;
		// 알파 텍스쳐만 있다면
		else if (m_pDiffuseTextureCom == nullptr && m_pAlphaTextureCom != nullptr)
			m_tEffectDesc.iShaderPass = 2;
		// 둘다 있다면
		else if (m_pDiffuseTextureCom != nullptr && m_pAlphaTextureCom != nullptr)
			m_tEffectDesc.iShaderPass = 3;
	}

	return S_OK;
}


void CEffect::Increment(_float fTimeDelta)
{
	if (m_bAccIndexEnd == true && m_tEffectDesc.bAnimationLoop == false)
		return;

	m_fAccIndex += m_fAnimationSpeed * fTimeDelta;
	if (m_fAccIndex >= 1.f)
	{
		m_fAccIndex = 0.f;
		m_tEffectDesc.fUVIndex.x++;
		if (m_tEffectDesc.fMaxCount.x <= m_tEffectDesc.fUVIndex.x)
		{
			m_tEffectDesc.fUVIndex.x = 0;
			m_tEffectDesc.fUVIndex.y++;
			if (m_tEffectDesc.fMaxCount.y <= m_tEffectDesc.fUVIndex.y)
			{
				m_tEffectDesc.fUVIndex.y = 0;
				if (m_tEffectDesc.bAnimationLoop == false)
					m_bAccIndexEnd = true;
				else
					m_bAccIndexEnd = false;
			}
		}
	}


}

void CEffect::Decrement(_float fTimeDelta)
{
	if (m_bAccIndexEnd == true && m_tEffectDesc.bAnimationLoop == false)
		return;

	m_fAccIndex += m_fAnimationSpeed * fTimeDelta;

	if (m_fAccIndex >= 1.f)
	{
		m_fAccIndex = 0.f;
		m_tEffectDesc.fUVIndex.x--;
		if (0 > m_tEffectDesc.fUVIndex.x)
		{
			m_tEffectDesc.fUVIndex.x = m_tEffectDesc.fMaxCount.x;
			m_tEffectDesc.fUVIndex.y--;
			if (0 > m_tEffectDesc.fUVIndex.y)
			{
				m_tEffectDesc.fUVIndex.y = m_tEffectDesc.fMaxCount.y;
				if (m_tEffectDesc.bAnimationLoop == false)
					m_bAccIndexEnd = true;
				else
					m_bAccIndexEnd = false;
			}
		}
	}
}

void CEffect::Change_Scale(_float fTimeDelta)
{
	if (m_tEffectDesc.bScaleChange)
	{
		m_fScaleChangeStartTime += fTimeDelta;
		if (m_fScaleChangeStartTime >= m_fScaleChangeStartDelay)
		{
			if (m_tEffectDesc.bScaleChangeRandom)
			{
				m_fScaleTimeAccs += m_fScaleSpeed * fTimeDelta;
				if (m_fScaleTimeAccs >= m_fScaleChange)
				{
					m_fScaleTimeAccs = 0.f;

					if (m_tEffectDesc.bScaleSameRate) {
						_float fScale = CUtils::Random_Float(m_tEffectDesc.fScaleStartMin.x, m_tEffectDesc.fScaleStartMax.x);
						m_pTransformCom->Set_Scale(_float3(fScale, fScale, fScale));
					}
					else
					{
						m_pTransformCom->Set_Scale(_float3(
							CUtils::Random_Float(m_tEffectDesc.fScaleStartMin.x, m_tEffectDesc.fScaleStartMax.x),
							CUtils::Random_Float(m_tEffectDesc.fScaleStartMin.y, m_tEffectDesc.fScaleStartMax.y),
							CUtils::Random_Float(m_tEffectDesc.fScaleStartMin.z, m_tEffectDesc.fScaleStartMax.z)));
					}

					m_fScaleChange = CUtils::Random_Float(m_tEffectDesc.fScaleChangeTime.x, m_tEffectDesc.fScaleChangeTime.y);
				}
			}
			else
			{
				_matrix WorldMatrix = m_pTransformCom->Get_WorldMatrix();
				
				// 확대
				if (m_tEffectDesc.bScaleAdd)
				{
					if (
						XMVectorGetX(XMVector3Length(WorldMatrix.r[CTransform::STATE_RIGHT])) > m_tEffectDesc.fScaleSizeMax.x || 
						XMVectorGetX(XMVector3Length(WorldMatrix.r[CTransform::STATE_UP]))    > m_tEffectDesc.fScaleSizeMax.y ||
						XMVectorGetX(XMVector3Length(WorldMatrix.r[CTransform::STATE_LOOK]))  > m_tEffectDesc.fScaleSizeMax.z)
					{
						// 반복 O
						if (m_tEffectDesc.bScaleLoop)
						{
							// 첫 크기로 반복
							if (m_tEffectDesc.bScaleLoopStart)
							{
								if (m_tEffectDesc.bScaleSameRate) {
									_float fScale = CUtils::Random_Float(m_tEffectDesc.fScaleStartMin.x, m_tEffectDesc.fScaleStartMax.x);
									m_pTransformCom->Set_Scale(_float3(fScale, fScale, fScale));
								}
								else
								{
									m_pTransformCom->Set_Scale(_float3(
										CUtils::Random_Float(m_tEffectDesc.fScaleStartMin.x, m_tEffectDesc.fScaleStartMax.x),
										CUtils::Random_Float(m_tEffectDesc.fScaleStartMin.y, m_tEffectDesc.fScaleStartMax.y),
										CUtils::Random_Float(m_tEffectDesc.fScaleStartMin.z, m_tEffectDesc.fScaleStartMax.z)));
								}
							}
							else
							{
								// 반대로 진행
								m_tEffectDesc.bScaleAdd = false;
							}
						}
					}
					else
					{
						//WorldMatrix.r[CTransform::STATE_RIGHT] += XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Normalize(XMLoadFloat3(&m_tEffectDesc.fScaleDir))) * m_fScaleSpeed * fTimeDelta;
						//WorldMatrix.r[CTransform::STATE_UP]    += XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetY(XMVector3Normalize(XMLoadFloat3(&m_tEffectDesc.fScaleDir))) * m_fScaleSpeed * fTimeDelta;
						//WorldMatrix.r[CTransform::STATE_LOOK]  += XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetZ(XMVector3Normalize(XMLoadFloat3(&m_tEffectDesc.fScaleDir))) * m_fScaleSpeed * fTimeDelta;
						WorldMatrix.r[CTransform::STATE_RIGHT] += XMVectorSet(1.f, 0.f, 0.f, 0.f) * m_fScaleSpeed * fTimeDelta * m_tEffectDesc.fScaleDirSpeed.x;
						WorldMatrix.r[CTransform::STATE_UP]    += XMVectorSet(0.f, 1.f, 0.f, 0.f) * m_fScaleSpeed * fTimeDelta * m_tEffectDesc.fScaleDirSpeed.y;
						WorldMatrix.r[CTransform::STATE_LOOK]  += XMVectorSet(0.f, 0.f, 1.f, 0.f) * m_fScaleSpeed * fTimeDelta * m_tEffectDesc.fScaleDirSpeed.z;
						m_pTransformCom->Set_WorldMatrix(WorldMatrix);
					}
				}
				// 축소
				else
				{
					if (
						XMVectorGetX(XMVector3Length(WorldMatrix.r[CTransform::STATE_RIGHT])) < m_tEffectDesc.fScaleSizeMin.x ||
						XMVectorGetX(XMVector3Length(WorldMatrix.r[CTransform::STATE_UP])) < m_tEffectDesc.fScaleSizeMin.y ||
						XMVectorGetX(XMVector3Length(WorldMatrix.r[CTransform::STATE_LOOK])) < m_tEffectDesc.fScaleSizeMin.z)
					{
						// 반복 O
						if (m_tEffectDesc.bScaleLoop)
						{
							// 첫 크기로 반복
							if (m_tEffectDesc.bScaleLoopStart)
							{
								if (m_tEffectDesc.bScaleSameRate) {
									_float fScale = CUtils::Random_Float(m_tEffectDesc.fScaleStartMin.x, m_tEffectDesc.fScaleStartMax.x);
									m_pTransformCom->Set_Scale(_float3(fScale, fScale, fScale));
								}
								else
								{
									m_pTransformCom->Set_Scale(_float3(
										CUtils::Random_Float(m_tEffectDesc.fScaleStartMin.x, m_tEffectDesc.fScaleStartMax.x),
										CUtils::Random_Float(m_tEffectDesc.fScaleStartMin.y, m_tEffectDesc.fScaleStartMax.y),
										CUtils::Random_Float(m_tEffectDesc.fScaleStartMin.z, m_tEffectDesc.fScaleStartMax.z)));
								}
							}
							else
							{
								m_tEffectDesc.bScaleAdd = true;
							}
						}
					}
					else
					{
						WorldMatrix.r[CTransform::STATE_RIGHT] -= XMVectorSet(1.f, 0.f, 0.f, 0.f) * m_fScaleSpeed * fTimeDelta * m_tEffectDesc.fScaleDirSpeed.x;
						WorldMatrix.r[CTransform::STATE_UP]    -= XMVectorSet(0.f, 1.f, 0.f, 0.f) * m_fScaleSpeed * fTimeDelta * m_tEffectDesc.fScaleDirSpeed.y;
						WorldMatrix.r[CTransform::STATE_LOOK]  -= XMVectorSet(0.f, 0.f, 1.f, 0.f) * m_fScaleSpeed * fTimeDelta * m_tEffectDesc.fScaleDirSpeed.z;
						m_pTransformCom->Set_WorldMatrix(WorldMatrix);
					}
				}
			}
		}
	}
}

void CEffect::Change_Move(_float fTimeDelta)
{
	if (!m_tEffectDesc.bVelocityChange)
	{
		// 위치 기본 변경
		m_pTransformCom->Move(XMVector3Normalize(XMLoadFloat3(&m_fVelocity)), m_fVelocitySpeed, fTimeDelta);
	}
	else
	{
		// 위치 추가 변경
		m_fVelocityChangeStartTime += fTimeDelta;
		if (m_fVelocityChangeStartTime >= m_fVelocityChangeStartDelay)
		{
			// 자동
			m_pTransformCom->Move(XMVector3Normalize(XMLoadFloat3(&m_fVelocity)), m_fVelocitySpeed, fTimeDelta);

			m_fVelocityRanTimeAccs += m_fVelocitySpeed * fTimeDelta;
			if (m_fVelocityRanTimeAccs >= m_fVelocityRanChange)
			{
				m_fVelocityRanTimeAccs = 0.f;

				m_fVelocity = _float3(CUtils::Random_Float(m_tEffectDesc.vVelocityMinStart.x, m_tEffectDesc.vVelocityMaxStart.x),
					CUtils::Random_Float(m_tEffectDesc.vVelocityMinStart.y, m_tEffectDesc.vVelocityMaxStart.y),
					CUtils::Random_Float(m_tEffectDesc.vVelocityMinStart.z, m_tEffectDesc.vVelocityMaxStart.z));

				m_fVelocityRanChange = CUtils::Random_Float(m_tEffectDesc.fVelocityChangeTime.x, m_tEffectDesc.fVelocityChangeTime.y);
			}

			// 수동은 부모 클래스 만들어서 내부 제어
		}
	}
}

void CEffect::Change_Rotation(_float fTimeDelta)
{
	if (m_tEffectDesc.bRotationChange)
	{
		m_fRotationChangeStartTime += fTimeDelta;
		if (m_fRotationChangeStartTime >= m_fRotationChangeStartDelay)
		{
			// 회전
			if (m_tEffectDesc.fRotationDir.x != 0.f || m_tEffectDesc.fRotationDir.y != 0.f || m_tEffectDesc.fRotationDir.z != 0.f)
			{
				_vector vTurnDir = XMVector3Normalize(XMLoadFloat3(&m_tEffectDesc.fRotationDir));
				m_pTransformCom->Turn(vTurnDir, m_fRotationSpeed, fTimeDelta);
			}

			// 랜덤 체인지
			if (m_tEffectDesc.bRotationChangeRandom)
			{
				m_fRotationTime += fTimeDelta;
				if (m_fRotationTime >= m_fRotationChangeTime)
				{
					m_fRotationTime       = 0.f;
					m_fRotationChangeTime = CUtils::Random_Float(m_tEffectDesc.fRotationChangeTime.x, m_tEffectDesc.fRotationChangeTime.y);
					m_fRotationSpeed      = CUtils::Random_Float(m_tEffectDesc.fRotationSpeed.x, m_tEffectDesc.fRotationSpeed.y);

					m_tEffectDesc.fRotationDir = _float3(CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.f, 1.f));
				}
			}
		}
	}
}

void CEffect::Change_Alpha(_float fTimeDelta)
{
	// 생성 이벤트 0 -> 1
	if (m_tEffectDesc.bAlphaCreate && !m_bAlphaCreateSucc && m_fAlpha < 1.f)
	{
		m_fAlpha += fTimeDelta * m_fAlphaSpeed;
		if (m_fAlpha >= 1.f)
		{
			m_fAlpha           = 1.f;
			m_bAlphaCreateSucc = true;
		}
	}
	else
	{
		// 삭제 이벤트 1 -> 0
		if (m_tEffectDesc.bAlphaDelete
			&& m_bAlphaCreateSucc
			&& m_fAccDeletionTime > m_fLifeTime - 2.f)
		{
			m_fAlpha -= fTimeDelta * m_fAlphaSpeed;
			if (m_fAlpha < 0.f)
			{
				m_fAlpha     = 0.f;
				m_bEffectDie = true;
			}
		}
		else
		{
			if (m_tEffectDesc.bAlphaChange)
			{
				m_fAlphaChangeStartTime += fTimeDelta;
				if (m_fAlphaChangeStartTime >= m_fAlphaChangeStartDelay)
				{
					if (m_tEffectDesc.bAlphaIn) // 0 -> 1  // 투명 -> 색상
					{
						if (m_fAlpha < 1.f)
							m_fAlpha += fTimeDelta * m_fAlphaSpeed;
						else
							m_fAlpha = 1.f;
					}
					else // 1 -> 0 // 색상 -> 투명
					{
						if (m_fAlpha > 0.f)
							m_fAlpha -= fTimeDelta * m_fAlphaSpeed;
						else
						{
							m_fAlpha     = 0.f;
							m_bEffectDie = true;
						}
					}
				}
			}
		}
	}
}

void CEffect::Change_Color(_float fTimeDelta)
{
	if (m_tEffectDesc.bColorChange)
	{
		m_fColorChangeStartTime += fTimeDelta;
		if (m_fColorChangeStartTime >= m_fColorChangeStartDelay)
		{
			// 랜덤 체인지
			if (m_tEffectDesc.bColorChangeRandom)
			{
				m_fColorAccs += fTimeDelta;
				if (m_fColorAccs >= m_fColorChangeTime)
				{
					if (!m_fColorFade)
					{
						m_LerpInfo.Start(Vec3(m_fColor.x, m_fColor.y, m_fColor.z),
							Vec3(m_fNextColor.x, m_fNextColor.y, m_fNextColor.z), m_fColorChangeDuration);

						m_fColorFade = true;
					}
					else if (m_fColorFade && m_LerpInfo.fCurTime >= m_LerpInfo.fEndTime)
					{
						m_LerpInfo   = {};
						m_fNextColor = _float3(CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.f, 1.f));
						m_fColorAccs = 0.f;
						m_fColorChangeTime = CUtils::Random_Float(m_tEffectDesc.fColorChangeRandomTime.x, m_tEffectDesc.fColorChangeRandomTime.y);
					
						m_fColorFade = false;
					}
				}

				if (m_LerpInfo.bActive)
				{
					Vec3 vNewColor = m_LerpInfo.Update_Lerp(fTimeDelta);
					m_fColor = _float3(vNewColor.x, vNewColor.y, vNewColor.z);
				}
			}
			else
			{
				// S -> M
				if (m_iColorIndex == 0 && m_fColorChangeStartTime >= m_fColorChangeStartM)
				{
					m_LerpInfo.Start(Vec3(m_tEffectDesc.fColorS.x, m_tEffectDesc.fColorS.y, m_tEffectDesc.fColorS.z),
						Vec3(m_tEffectDesc.fColorM.x, m_tEffectDesc.fColorM.y, m_tEffectDesc.fColorM.z), m_fColorChangeDuration);

					m_iColorIndex++;
				}
				// M -> F
				else if (m_iColorIndex == 1 && m_fColorChangeStartTime >= m_fColorChangeStartF)
				{
					if (m_LerpInfo.fCurTime >= m_LerpInfo.fEndTime)
					{
						m_LerpInfo.Start(Vec3(m_tEffectDesc.fColorM.x, m_tEffectDesc.fColorM.y, m_tEffectDesc.fColorM.z),
							Vec3(m_tEffectDesc.fColorF.x, m_tEffectDesc.fColorF.y, m_tEffectDesc.fColorF.z), m_fColorChangeDuration);

						m_iColorIndex++;
					}
				}

				if (m_LerpInfo.bActive)
				{
					Vec3 vNewColor = m_LerpInfo.Update_Lerp(fTimeDelta);
					m_fColor = _float3(vNewColor.x, vNewColor.y, vNewColor.z);

					if (m_iColorIndex >= 2 && m_LerpInfo.fCurTime >= m_LerpInfo.fEndTime)
					{
						if (m_tEffectDesc.bColorLoop)
						{
							if (m_iColorIndex == 2 && m_LerpInfo.fCurTime >= m_LerpInfo.fEndTime)
							{
								m_LerpInfo.Start(Vec3(m_tEffectDesc.fColorF.x, m_tEffectDesc.fColorF.y, m_tEffectDesc.fColorF.z),
									Vec3(m_tEffectDesc.fColorS.x, m_tEffectDesc.fColorS.y, m_tEffectDesc.fColorS.z), m_fColorChangeDuration);

								m_iColorIndex++;
							}
							else if (m_iColorIndex == 3 && m_LerpInfo.fCurTime >= m_LerpInfo.fEndTime)
							{
								m_LerpInfo = {};

								m_fColorChangeStartTime = m_fColorChangeStartM;

								m_iColorIndex = 0;
								m_fColorChangeStartM = CUtils::Random_Float(m_tEffectDesc.fColorChangeStartM.x, m_tEffectDesc.fColorChangeStartM.y);
								m_fColorChangeStartF = CUtils::Random_Float(m_tEffectDesc.fColorChangeStartF.x, m_tEffectDesc.fColorChangeStartF.y);

								m_fColorChangeDuration = CUtils::Random_Float(m_tEffectDesc.fColorDuration.x, m_tEffectDesc.fColorDuration.y);

								if (m_tEffectDesc.bColorRandom)
									m_fColor = _float3(CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.f, 1.f));
								else
									m_fColor = _float3(m_tEffectDesc.fColorS.x, m_tEffectDesc.fColorS.y, m_tEffectDesc.fColorS.z);
							}
						}
						else
						{
							m_tEffectDesc.bColorChange = false;
						}
					}
				}
			}
		}
	}
}

void CEffect::Set_Model()
{
	_int  iBufferSizeName = WideCharToMultiByte(CP_UTF8, 0, m_tEffectDesc.strModelName.c_str(), -1, nullptr, 0, nullptr, nullptr);
	char* pszFileName = new char[iBufferSizeName];
	WideCharToMultiByte(CP_UTF8, 0, m_tEffectDesc.strModelName.c_str(), -1, pszFileName, iBufferSizeName, nullptr, nullptr);
	if (strcmp(pszFileName, "") != 0)
	{
		if (m_pModelCom != nullptr)
			Safe_Release(m_pModelCom);

		m_pModelCom = static_cast<CModel*>(GI->Clone_Component(LEVEL_STATIC, m_tEffectDesc.strModelName));
	}
	Safe_Delete(pszFileName);
}

void CEffect::Set_Texture_Diffuse()
{
	_int  iBufferSizeName = WideCharToMultiByte(CP_UTF8, 0, m_tEffectDesc.strDiffuseTetextureName.c_str(), -1, nullptr, 0, nullptr, nullptr);
	char* pszFileName = new char[iBufferSizeName];
	WideCharToMultiByte(CP_UTF8, 0, m_tEffectDesc.strDiffuseTetextureName.c_str(), -1, pszFileName, iBufferSizeName, nullptr, nullptr);
	if (strcmp(pszFileName, "") != 0)
	{
		if (m_pDiffuseTextureCom != nullptr)
			Safe_Release(m_pDiffuseTextureCom);

		m_pDiffuseTextureCom = static_cast<CTexture*>(GI->Clone_Component(LEVEL_STATIC, m_tEffectDesc.strDiffuseTetextureName));
	}
	Safe_Delete(pszFileName);

	if (m_pDiffuseTextureCom != nullptr && m_tEffectDesc.iTextureIndexDiffuse >= m_pDiffuseTextureCom->Get_TextureCount())
		m_tEffectDesc.iTextureIndexDiffuse = m_pDiffuseTextureCom->Get_TextureCount() - 1;
}

void CEffect::Set_Texture_Alpha()
{
	_int  iBufferSizeName = WideCharToMultiByte(CP_UTF8, 0, m_tEffectDesc.strAlphaTexturName.c_str(), -1, nullptr, 0, nullptr, nullptr);
	char* pszFileName = new char[iBufferSizeName];
	WideCharToMultiByte(CP_UTF8, 0, m_tEffectDesc.strAlphaTexturName.c_str(), -1, pszFileName, iBufferSizeName, nullptr, nullptr);
	if (strcmp(pszFileName, "") != 0)
	{
		if (m_pAlphaTextureCom != nullptr)
			Safe_Release(m_pAlphaTextureCom);

		m_pAlphaTextureCom = static_cast<CTexture*>(GI->Clone_Component(LEVEL_STATIC, m_tEffectDesc.strAlphaTexturName));
	}
	Safe_Delete(pszFileName);

	if (m_pAlphaTextureCom != nullptr && m_tEffectDesc.iTextureIndexAlpha >= m_pAlphaTextureCom->Get_TextureCount())
		m_tEffectDesc.iTextureIndexAlpha = m_pAlphaTextureCom->Get_TextureCount() - 1;
}

HRESULT CEffect::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For. Com_RigidBody */
	CRigidBody::RIGID_BODY_DESC RigidDesc;
	ZeroMemory(&RigidDesc, sizeof RigidDesc);
	RigidDesc.pTransform = m_pTransformCom;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBodyCom, &RigidDesc)))
		return E_FAIL;
	/*m_pRigidBodyCom->Set_RefHeight(-999.f);
	m_pRigidBodyCom->Set_Gravity(m_bGravity);*/

	Set_Texture_Diffuse();
	Set_Texture_Alpha();

	/* For.Com_Model */
	if (m_tEffectDesc.eType == EFFECT_TYPE::EFFECT_MESH)
		Set_Model();
	/* For.Com_VIBuffer */
	else
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
			return E_FAIL;
	}

	return S_OK;
}


CEffect* CEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, 
	const wstring& strObjectTag, const EFFECT_DESC* pEffectDesc)
{
	CEffect* pInstance = new CEffect(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype(pEffectDesc)))
	{
		MSG_BOX("Failed to Created : CEffect");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CEffect::Clone(void* pArg)
{
	CEffect* pInstance = new CEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pDiffuseTextureCom);
	Safe_Release(m_pAlphaTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRigidBodyCom);
	Safe_Release(m_pVIBufferCom);
}





