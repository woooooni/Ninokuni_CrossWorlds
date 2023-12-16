#include "..\Public\VIBuffer_Particle.h"
#include "Utils.h"

CVIBuffer_Particle::CVIBuffer_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Particle::CVIBuffer_Particle(const CVIBuffer_Particle& rhs)
	: CVIBuffer(rhs)
{
}

void CVIBuffer_Particle::Restart_ParticleBufferDesc()
{
	D3D11_MAPPED_SUBRESOURCE SubResource;
	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		// 위치 (분포 범위)
		((VTXINSTANCE*)SubResource.pData)[i].vPosition = Get_NewPosition_Particle();
		if ((*m_tParticleDesc.pVelocityChangeRandom))
		{
			m_vecParticleInfoDesc[i].fVelocityRanTimeAccs = 0.f;
			m_vecParticleInfoDesc[i].fVelocityRanChange   = CUtils::Random_Float((*m_tParticleDesc.pVelocityChangeTime).x, (*m_tParticleDesc.pVelocityChangeTime).y);
		}

#pragma region 크기
		if ((*m_tParticleDesc.pScaleSameRate))
		{
			_float fScale = CUtils::Random_Float((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y);
			((VTXINSTANCE*)SubResource.pData)[i].vRight = _float4(fScale, 0.f, 0.f, 0.f);
			((VTXINSTANCE*)SubResource.pData)[i].vUp    = _float4(0.f, fScale, 0.f, 0.f);
			((VTXINSTANCE*)SubResource.pData)[i].vLook = _float4(0.f, 0.f, fScale, 0.f);
		}
		else
		{
			((VTXINSTANCE*)SubResource.pData)[i].vRight = _float4(CUtils::Random_Float((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y), 0.f, 0.f, 0.f);
			((VTXINSTANCE*)SubResource.pData)[i].vUp    = _float4(0.f, CUtils::Random_Float((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y), 0.f, 0.f);
			((VTXINSTANCE*)SubResource.pData)[i].vLook  = _float4(0.f, 0.f, CUtils::Random_Float((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y), 0.f);
		}

		if ((*m_tParticleDesc.pScaleChange))
		{
			m_vecParticleInfoDesc[i].bScaleAdd = (*m_tParticleDesc.pScaleAdd);
			m_vecParticleInfoDesc[i].fScaleSpeed = CUtils::Random_Float((*m_tParticleDesc.pScaleSpeed).x, (*m_tParticleDesc.pScaleSpeed).y);

			m_vecParticleInfoDesc[i].fScaleTimeAccs = 0.f;
			m_vecParticleInfoDesc[i].fScaleChange   = CUtils::Random_Float((*m_tParticleDesc.pScaleChangeTime).x, (*m_tParticleDesc.pScaleChangeTime).y);
		}
#pragma endregion

#pragma region 이동 방향
		m_vecParticleInfoDesc[i].fVelocitySpeeds = CUtils::Random_Float((*m_tParticleDesc.pVelocitySpeed).x, (*m_tParticleDesc.pVelocitySpeed).y);

		if (!(*m_tParticleDesc.pVelocityChange))
		{
			m_vecParticleInfoDesc[i].vVelocity = Get_NewVelocity_Particle();
		}
		else
		{
			if (m_vecParticleInfoDesc[i].pVelocity == nullptr)
				m_vecParticleInfoDesc[i].pVelocity = new _float3[(*m_tParticleDesc.pVelocityUse)];
			if (m_vecParticleInfoDesc[i].pVelocityChange == nullptr)
				m_vecParticleInfoDesc[i].pVelocityChange = new _float[(*m_tParticleDesc.pVelocityUse)];

			for (size_t j = 0; j < (*m_tParticleDesc.pVelocityCountMax); ++j)
			{
				_vector vVelocity = XMVector3Normalize(
					XMVectorSet(
						CUtils::Random_Float(m_tParticleDesc.pVelocityMin[j].x, m_tParticleDesc.pVelocityMax[j].x),
						CUtils::Random_Float(m_tParticleDesc.pVelocityMin[j].y, m_tParticleDesc.pVelocityMax[j].y),
						CUtils::Random_Float(m_tParticleDesc.pVelocityMin[j].z, m_tParticleDesc.pVelocityMax[j].z),
						0.f));
				m_vecParticleInfoDesc[i].pVelocity[j]       = _float3(XMVectorGetX(vVelocity), XMVectorGetY(vVelocity), XMVectorGetZ(vVelocity));
				m_vecParticleInfoDesc[i].pVelocityChange[j] = CUtils::Random_Float(m_tParticleDesc.pVelocityTime[j].x, m_tParticleDesc.pVelocityTime[j].y);
			}

			m_vecParticleInfoDesc[i].iVelocityCountCur = 0;
			m_vecParticleInfoDesc[i].fVeloityTimeAccs  = 0.f;
		}
#pragma endregion

		// 시간
		m_vecParticleInfoDesc[i].fTimeAccs  = 0.f;
		m_vecParticleInfoDesc[i].fLifeTimes = CUtils::Random_Float((*m_tParticleDesc.pLifeTime).x, (*m_tParticleDesc.pLifeTime).y);

#pragma region 텍스처 정보
		m_vecParticleInfoDesc[i].bAmimationFinish = false;
		m_vecParticleInfoDesc[i].fAccIndex        = 0.f;
		m_vecParticleInfoDesc[i].fAnimationSpeed  = CUtils::Random_Float((*m_tParticleDesc.pAnimationSpeed).x, (*m_tParticleDesc.pAnimationSpeed).y);

		if ((*m_tParticleDesc.pRandomStartIndex))
			m_vecParticleShaderDesc[i].fUVIndex = _float2(
				(_int)CUtils::Random_Float(0.f, (*m_tParticleDesc.pUVMaxCount).x),
				(_int)CUtils::Random_Float(0.f, (*m_tParticleDesc.pUVMaxCount).y));
		else
			m_vecParticleShaderDesc[i].fUVIndex = (*m_tParticleDesc.pUVIndex);

		m_vecParticleShaderDesc[i].fMaxCount = (*m_tParticleDesc.pUVMaxCount);
#pragma endregion

#pragma region 색상
		if ((*m_tParticleDesc.pRandomColor))
			m_vecParticleShaderDesc[i].vColor = _float4(CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.5f, 1.f));
		else
			m_vecParticleShaderDesc[i].vColor = (*m_tParticleDesc.pColor);
#pragma endregion
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

HRESULT CVIBuffer_Particle::Initialize_Prototype()
{
#pragma region VERTEXBUFFER
	m_iNumVertexBuffers = 2;

	m_iStride = sizeof(VTXPOINT);
	m_iNumVertices = 1;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXPOINT* pVertices = new VTXPOINT;
	ZeroMemory(pVertices, sizeof(VTXPOINT));

	pVertices->vPosition = _float3(0.0f, 0.0f, 0.f);
	pVertices->vSize = _float2(1.f, 1.f);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
#pragma endregion

#pragma region INDEXBUFFER
	m_iNumPrimitives = 1;

	m_iIndexSizeofPrimitive = sizeof(_ushort);
	m_iNumIndicesofPrimitive = 1;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumPrimitives * m_iIndexSizeofPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	_ushort* pIndices = new _ushort[m_iNumPrimitives];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumPrimitives);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Particle::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	m_tParticleDesc = *static_cast<PARTICLE_BUFFER_DESC*>(pArg);

	m_vecParticleInfoDesc.reserve(m_iMaxCount);
	m_vecParticleShaderDesc.reserve(m_iMaxCount);

#pragma region INSTANCEVERTEXBUFFER
	m_iStrideInstance = sizeof(VTXINSTANCE);
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iStrideInstance * m_iMaxCount;
	m_BufferDesc.Usage     = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags           = 0;
	m_BufferDesc.StructureByteStride = m_iStrideInstance;

	m_pVertices = new VTXINSTANCE[m_iMaxCount];
	ZeroMemory(m_pVertices, sizeof(VTXINSTANCE) * m_iMaxCount);

	PARTICLE_INFO_DESC   ParticleInfo       = {};
	PARTICLE_SHADER_DESC ParticleShaderInfo = {};
	for (size_t i = 0; i < m_iMaxCount; i++)
	{
		// 위치 (분포 범위)
		m_pVertices[i].vPosition = Get_NewPosition_Particle();
		if ((*m_tParticleDesc.pVelocityChangeRandom))
		{
			ParticleInfo.fVelocityRanTimeAccs = 0.f;
			ParticleInfo.fVelocityRanChange = CUtils::Random_Float((*m_tParticleDesc.pVelocityChangeTime).x, (*m_tParticleDesc.pVelocityChangeTime).y);
		}

#pragma region 크기
		if ((*m_tParticleDesc.pScaleSameRate))
		{
			_float fScale = CUtils::Random_Float((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y);
			m_pVertices[i].vRight = _float4(fScale, 0.f, 0.f, 0.f);
			m_pVertices[i].vUp    = _float4(0.f, fScale, 0.f, 0.f);
			m_pVertices[i].vLook  = _float4(0.f, 0.f, fScale, 0.f);
		}
		else
		{
			m_pVertices[i].vRight = _float4(CUtils::Random_Float((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y), 0.f, 0.f, 0.f);
			m_pVertices[i].vUp    = _float4(0.f, CUtils::Random_Float((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y), 0.f, 0.f);
			m_pVertices[i].vLook  = _float4(0.f, 0.f, CUtils::Random_Float((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y), 0.f);
		}

		if ((*m_tParticleDesc.pScaleChange))
		{
			ParticleInfo.bScaleAdd = (*m_tParticleDesc.pScaleAdd);
			ParticleInfo.fScaleSpeed = CUtils::Random_Float((*m_tParticleDesc.pScaleSpeed).x, (*m_tParticleDesc.pScaleSpeed).y);

			ParticleInfo.fScaleTimeAccs = 0.f;
			ParticleInfo.fScaleChange   = CUtils::Random_Float((*m_tParticleDesc.pScaleChangeTime).x, (*m_tParticleDesc.pScaleChangeTime).y);
		}
#pragma endregion

#pragma region 이동 방향
		ParticleInfo.fVelocitySpeeds = CUtils::Random_Float((*m_tParticleDesc.pVelocitySpeed).x, (*m_tParticleDesc.pVelocitySpeed).y);

		if (!(*m_tParticleDesc.pVelocityChange))
		{
			ParticleInfo.vVelocity = Get_NewVelocity_Particle();
		}
		else
		{
			ParticleInfo.pVelocity       = new _float3[(*m_tParticleDesc.pVelocityUse)];
			ParticleInfo.pVelocityChange = new _float[(*m_tParticleDesc.pVelocityUse)];

			for (size_t j = 0; j < (*m_tParticleDesc.pVelocityCountMax); ++j)
			{
				_vector vVelocity = XMVector3Normalize(
					XMVectorSet(
						CUtils::Random_Float(m_tParticleDesc.pVelocityMin[j].x, m_tParticleDesc.pVelocityMax[j].x),
						CUtils::Random_Float(m_tParticleDesc.pVelocityMin[j].y, m_tParticleDesc.pVelocityMax[j].y),
						CUtils::Random_Float(m_tParticleDesc.pVelocityMin[j].z, m_tParticleDesc.pVelocityMax[j].z),
						0.f));
				ParticleInfo.pVelocity[j]       = _float3(XMVectorGetX(vVelocity), XMVectorGetY(vVelocity), XMVectorGetZ(vVelocity));
				ParticleInfo.pVelocityChange[j] = CUtils::Random_Float(m_tParticleDesc.pVelocityTime[j].x, m_tParticleDesc.pVelocityTime[j].y);
			}

			ParticleInfo.iVelocityCountCur = 0;
			ParticleInfo.fVeloityTimeAccs  = 0.f;
		}
#pragma endregion

#pragma region 회전
		if ((*m_tParticleDesc.pBillboard))
		{
			ParticleShaderInfo.bBillboard = true;
		}
		else
		{
			ParticleShaderInfo.bBillboard = false;

			if ((*m_tParticleDesc.pRandomAxis))
			{
				_uint iRandomCount = (_uint)CUtils::Random_Float(0.f, 2.9);
				switch (iRandomCount)
				{
				case 0:
					ParticleShaderInfo.vAxis = XMVectorSet(1.f, 0.f, 0.f, 0.f);
					break;
				case 1:
					ParticleShaderInfo.vAxis = XMVectorSet(0.f, 1.f, 0.f, 0.f);
					break;
				default:
					ParticleShaderInfo.vAxis = XMVectorSet(0.f, 0.f, 1.f, 0.f);
					break;
				}
			}
			else
			{
				ParticleShaderInfo.vAxis = (*m_tParticleDesc.pAxis);
			}

			if ((*m_tParticleDesc.pRandomAngle))
				ParticleShaderInfo.fAngle = CUtils::Random_Float(0.f, 360.f);
			else
				ParticleShaderInfo.fAngle = (*m_tParticleDesc.pAngle);

			if ((*m_tParticleDesc.pRotationChange))
			{
				if ((*m_tParticleDesc.pRotationChangeRandom))
				{
					ParticleInfo.fRotationChangeTime = CUtils::Random_Float((*m_tParticleDesc.pRotationChangeTime).x, (*m_tParticleDesc.pRotationChangeTime).y);
					ParticleInfo.fRotationSpeed;
				}
				else
				{

				}
			}
		}
#pragma endregion

		// 시간
		ParticleInfo.fTimeAccs  = 0.f;
		ParticleInfo.fLifeTimes = CUtils::Random_Float((*m_tParticleDesc.pLifeTime).x, (*m_tParticleDesc.pLifeTime).y);

#pragma region 텍스처 정보
		ParticleInfo.bAmimationFinish = false;
		ParticleInfo.fAccIndex        = 0.f;
		ParticleInfo.fAnimationSpeed  = CUtils::Random_Float((*m_tParticleDesc.pAnimationSpeed).x, (*m_tParticleDesc.pAnimationSpeed).y);

		if ((*m_tParticleDesc.pRandomStartIndex))
			ParticleShaderInfo.fUVIndex = _float2(
				(_int)CUtils::Random_Float(0.f, (*m_tParticleDesc.pUVMaxCount).x),
				(_int)CUtils::Random_Float(0.f, (*m_tParticleDesc.pUVMaxCount).y));
		else
			ParticleShaderInfo.fUVIndex = (*m_tParticleDesc.pUVIndex);

		ParticleShaderInfo.fMaxCount = (*m_tParticleDesc.pUVMaxCount);
#pragma endregion

#pragma region 색상
		if ((*m_tParticleDesc.pRandomColor))
			ParticleShaderInfo.vColor = _float4(CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.5f, 1.f));
		else
			ParticleShaderInfo.vColor = (*m_tParticleDesc.pColor);
#pragma endregion

		m_vecParticleInfoDesc.push_back(ParticleInfo);
		m_vecParticleShaderDesc.push_back(ParticleShaderInfo);
	}

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = m_pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVBInstance)))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

void CVIBuffer_Particle::Tick(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE SubResource;
	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	_uint iDieEffect = 0;
	for (size_t i = 0; i < m_iNumInstance; i++)
	{
#pragma region 지속 시간 진행 시
		if (m_vecParticleInfoDesc[i].fTimeAccs < m_vecParticleInfoDesc[i].fLifeTimes) 
		{
			m_vecParticleInfoDesc[i].fTimeAccs += fTimeDelta;

#pragma region 위치 변경
			if (!(*m_tParticleDesc.pVelocityChange))
			{
				// 위치 기본 변경
				((VTXINSTANCE*)SubResource.pData)[i].vPosition.x += m_vecParticleInfoDesc[i].vVelocity.x * m_vecParticleInfoDesc[i].fVelocitySpeeds * fTimeDelta;
				((VTXINSTANCE*)SubResource.pData)[i].vPosition.y += m_vecParticleInfoDesc[i].vVelocity.y * m_vecParticleInfoDesc[i].fVelocitySpeeds * fTimeDelta;
				((VTXINSTANCE*)SubResource.pData)[i].vPosition.z += m_vecParticleInfoDesc[i].vVelocity.z * m_vecParticleInfoDesc[i].fVelocitySpeeds * fTimeDelta;

			}
			else
			{
				// 위치 추가 변경
				if ((*m_tParticleDesc.pVelocityChangeRandom))
				{
					// 자동
					((VTXINSTANCE*)SubResource.pData)[i].vPosition.x += m_vecParticleInfoDesc[i].vVelocity.x * m_vecParticleInfoDesc[i].fVelocitySpeeds * fTimeDelta;
					((VTXINSTANCE*)SubResource.pData)[i].vPosition.y += m_vecParticleInfoDesc[i].vVelocity.y * m_vecParticleInfoDesc[i].fVelocitySpeeds * fTimeDelta;
					((VTXINSTANCE*)SubResource.pData)[i].vPosition.z += m_vecParticleInfoDesc[i].vVelocity.z * m_vecParticleInfoDesc[i].fVelocitySpeeds * fTimeDelta;

					m_vecParticleInfoDesc[i].fVelocityRanTimeAccs += m_vecParticleInfoDesc[i].fVelocitySpeeds * fTimeDelta;
					if (m_vecParticleInfoDesc[i].fVelocityRanTimeAccs >= m_vecParticleInfoDesc[i].fVelocityRanChange)
					{
						m_vecParticleInfoDesc[i].fVelocityRanTimeAccs = 0.f;

						m_vecParticleInfoDesc[i].vVelocity          = Get_NewVelocity_Particle();
						m_vecParticleInfoDesc[i].fVelocityRanChange = CUtils::Random_Float((*m_tParticleDesc.pVelocityChangeTime).x, (*m_tParticleDesc.pVelocityChangeTime).y);
					}
				}
				else
				{
					// 수동
					((VTXINSTANCE*)SubResource.pData)[i].vPosition.x += m_vecParticleInfoDesc[i].pVelocity[m_vecParticleInfoDesc[i].iVelocityCountCur].x * m_vecParticleInfoDesc[i].fVelocitySpeeds * fTimeDelta;
					((VTXINSTANCE*)SubResource.pData)[i].vPosition.y += m_vecParticleInfoDesc[i].pVelocity[m_vecParticleInfoDesc[i].iVelocityCountCur].y * m_vecParticleInfoDesc[i].fVelocitySpeeds * fTimeDelta;
					((VTXINSTANCE*)SubResource.pData)[i].vPosition.z += m_vecParticleInfoDesc[i].pVelocity[m_vecParticleInfoDesc[i].iVelocityCountCur].z * m_vecParticleInfoDesc[i].fVelocitySpeeds * fTimeDelta;

					m_vecParticleInfoDesc[i].fVeloityTimeAccs += fTimeDelta;
					if (m_vecParticleInfoDesc[i].fVeloityTimeAccs >= m_vecParticleInfoDesc[i].pVelocityChange[m_vecParticleInfoDesc[i].iVelocityCountCur])
					{
						if (m_vecParticleInfoDesc[i].iVelocityCountCur < (*m_tParticleDesc.pVelocityCountMax) - 1)
						{
							m_vecParticleInfoDesc[i].iVelocityCountCur++;
							m_vecParticleInfoDesc[i].fVeloityTimeAccs = 0.f;
						}
						else
						{
							if ((*m_tParticleDesc.pVelocityLoop))
							{
								m_vecParticleInfoDesc[i].iVelocityCountCur = 0;
								m_vecParticleInfoDesc[i].fVeloityTimeAccs = 0.f;
							}
						}
					}
				}
			}
#pragma endregion

#pragma region 박스 범위 검사
			if ((*m_tParticleDesc.pUseBox))
			{
				if (((VTXINSTANCE*)SubResource.pData)[i].vPosition.x < (*m_tParticleDesc.pBoxMin).x ||
					((VTXINSTANCE*)SubResource.pData)[i].vPosition.y < (*m_tParticleDesc.pBoxMin).y ||
					((VTXINSTANCE*)SubResource.pData)[i].vPosition.z < (*m_tParticleDesc.pBoxMin).z ||
					((VTXINSTANCE*)SubResource.pData)[i].vPosition.x > (*m_tParticleDesc.pBoxMax).x ||
					((VTXINSTANCE*)SubResource.pData)[i].vPosition.y > (*m_tParticleDesc.pBoxMax).y ||
					((VTXINSTANCE*)SubResource.pData)[i].vPosition.z > (*m_tParticleDesc.pBoxMax).z)
				{
					// 반복X
					if (!m_bFinished && !(*m_tParticleDesc.pParticleLoop))
					{
						iDieEffect++;
						if (iDieEffect == m_iNumInstance)
							m_bFinished = true;

						((VTXINSTANCE*)SubResource.pData)[i].vRight = _float4(0.f, 0.f, 0.f, 0.f);
						((VTXINSTANCE*)SubResource.pData)[i].vUp    = _float4(0.f, 0.f, 0.f, 0.f);
						((VTXINSTANCE*)SubResource.pData)[i].vLook  = _float4(0.f, 0.f, 0.f, 0.f);
					}
					// 반복O
					else
					{
						((VTXINSTANCE*)SubResource.pData)[i].vPosition = Get_NewPosition_Particle();
						m_vecParticleInfoDesc[i].vVelocity             = Get_NewVelocity_Particle();
						m_vecParticleInfoDesc[i].fVelocitySpeeds = CUtils::Random_Float((*m_tParticleDesc.pVelocitySpeed).x, (*m_tParticleDesc.pVelocitySpeed).y);
						m_vecParticleInfoDesc[i].fTimeAccs       = 0.f;
					}
				}
			}
#pragma endregion

#pragma region 크기 변경
			if ((*m_tParticleDesc.pScaleChange))
			{
				if ((*m_tParticleDesc.pScaleChangeRandom))
				{
					m_vecParticleInfoDesc[i].fScaleTimeAccs += m_vecParticleInfoDesc[i].fScaleSpeed * fTimeDelta;
					if (m_vecParticleInfoDesc[i].fScaleTimeAccs >= m_vecParticleInfoDesc[i].fScaleChange)
					{
						m_vecParticleInfoDesc[i].fScaleTimeAccs = 0.f;

						((VTXINSTANCE*)SubResource.pData)[i].vRight.x = CUtils::Random_Float((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y);
						((VTXINSTANCE*)SubResource.pData)[i].vUp.y    = ((VTXINSTANCE*)SubResource.pData)[i].vRight.x;

						m_vecParticleInfoDesc[i].fScaleChange = CUtils::Random_Float((*m_tParticleDesc.pScaleChangeTime).x, (*m_tParticleDesc.pScaleChangeTime).y);
					}
				}
				else
				{
					// 확대
					if (m_vecParticleInfoDesc[i].bScaleAdd)
					{
						((VTXINSTANCE*)SubResource.pData)[i].vRight.x += m_vecParticleInfoDesc[i].fScaleSpeed * fTimeDelta;
						((VTXINSTANCE*)SubResource.pData)[i].vUp.y = ((VTXINSTANCE*)SubResource.pData)[i].vRight.x;

						if (((VTXINSTANCE*)SubResource.pData)[i].vRight.x > (*m_tParticleDesc.pScaleMax).x ||
							((VTXINSTANCE*)SubResource.pData)[i].vUp.y > (*m_tParticleDesc.pScaleMax).y)
						{
							// 반복 O
							if ((*m_tParticleDesc.pScaleLoop))
							{
								// 첫 크기로 반복
								if ((*m_tParticleDesc.pScaleLoopStart))
								{
									((VTXINSTANCE*)SubResource.pData)[i].vRight.x = CUtils::Random_Float((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y);
									((VTXINSTANCE*)SubResource.pData)[i].vUp.y = ((VTXINSTANCE*)SubResource.pData)[i].vRight.x;
								}
								else
								{
									// 반대로 진행
									m_vecParticleInfoDesc[i].bScaleAdd = false;
								}
							}
							// 반복 X
							else
							{
								((VTXINSTANCE*)SubResource.pData)[i].vRight.x = (*m_tParticleDesc.pScaleMax).x;
								((VTXINSTANCE*)SubResource.pData)[i].vUp.y = ((VTXINSTANCE*)SubResource.pData)[i].vRight.x;
							}
						}
					}
					// 축소
					else
					{
						((VTXINSTANCE*)SubResource.pData)[i].vRight.x -= m_vecParticleInfoDesc[i].fScaleSpeed * fTimeDelta;
						((VTXINSTANCE*)SubResource.pData)[i].vUp.y = ((VTXINSTANCE*)SubResource.pData)[i].vRight.x;

						if (((VTXINSTANCE*)SubResource.pData)[i].vRight.x < (*m_tParticleDesc.pScaleMin).x ||
							((VTXINSTANCE*)SubResource.pData)[i].vUp.y < (*m_tParticleDesc.pScaleMin).y)
						{
							// 반복 O
							if ((*m_tParticleDesc.pScaleLoop))
							{
								// 첫 크기로 반복
								if ((*m_tParticleDesc.pScaleLoopStart))
								{
									((VTXINSTANCE*)SubResource.pData)[i].vRight.x = CUtils::Random_Float((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y);
									((VTXINSTANCE*)SubResource.pData)[i].vUp.y = ((VTXINSTANCE*)SubResource.pData)[i].vRight.x;
								}
								else
								{
									m_vecParticleInfoDesc[i].bScaleAdd = true;
								}
							}
							// 반복 X
							else
							{
								((VTXINSTANCE*)SubResource.pData)[i].vRight.x = (*m_tParticleDesc.pScaleMin).x;
								((VTXINSTANCE*)SubResource.pData)[i].vUp.y = ((VTXINSTANCE*)SubResource.pData)[i].vRight.x;
							}
						}
					}
				}
			}
#pragma endregion

#pragma region 회전 변경
			if ((*m_tParticleDesc.pRotationChange))
			{
				if (!(*m_tParticleDesc.pRotationChangeRandom))
				{
					if ((*m_tParticleDesc.pRotationAdd))
						m_vecParticleShaderDesc[i].fAngle += fTimeDelta * 100.f;
					else
						m_vecParticleShaderDesc[i].fAngle -= fTimeDelta * 100.f;
				}
				else
				{
					// 랜덤 체인지

				}
			}
#pragma endregion

#pragma region 텍스처 애니메이션
			if ((*m_tParticleDesc.pAnimation))
			{
				if (!m_vecParticleInfoDesc[i].bAmimationFinish)
				{
					m_vecParticleInfoDesc[i].fAccIndex += m_vecParticleInfoDesc[i].fAnimationSpeed * fTimeDelta;
					if (m_vecParticleInfoDesc[i].fAccIndex >= 1.f)
					{
						m_vecParticleInfoDesc[i].fAccIndex = 0.f;
						m_vecParticleShaderDesc[i].fUVIndex.x++;
						if (m_vecParticleShaderDesc[i].fMaxCount.x <= m_vecParticleShaderDesc[i].fUVIndex.x)
						{
							m_vecParticleShaderDesc[i].fUVIndex.x = 0.f;
							m_vecParticleShaderDesc[i].fUVIndex.y++;
							if (m_vecParticleShaderDesc[i].fMaxCount.y <= m_vecParticleShaderDesc[i].fUVIndex.y)
							{
								if ((*m_tParticleDesc.pAnimationLoop))
									m_vecParticleShaderDesc[i].fUVIndex = _float2(0.f, 0.f);
								else
								{
									m_vecParticleInfoDesc[i].bAmimationFinish = true;
									m_vecParticleShaderDesc[i].fUVIndex       = m_vecParticleShaderDesc[i].fMaxCount;
								}
							}
						}
					}
				}
			}
#pragma endregion

		}
#pragma endregion

#pragma region 지속 시간 소멸 시
		else
		{
			// 반복X
			if (!m_bFinished && !(*m_tParticleDesc.pParticleLoop))
			{
				iDieEffect++;
				if (iDieEffect == m_iNumInstance)
					m_bFinished = true;

				((VTXINSTANCE*)SubResource.pData)[i].vRight = _float4(0.f, 0.f, 0.f, 0.f);
				((VTXINSTANCE*)SubResource.pData)[i].vUp    = _float4(0.f, 0.f, 0.f, 0.f);
				((VTXINSTANCE*)SubResource.pData)[i].vLook  = _float4(0.f, 0.f, 0.f, 0.f);
			}
			// 반복O
			else
			{
				((VTXINSTANCE*)SubResource.pData)[i].vPosition = Get_NewPosition_Particle();
				m_vecParticleInfoDesc[i].vVelocity             = Get_NewVelocity_Particle();
				m_vecParticleInfoDesc[i].fVelocitySpeeds = CUtils::Random_Float((*m_tParticleDesc.pVelocitySpeed).x, (*m_tParticleDesc.pVelocitySpeed).y);
				m_vecParticleInfoDesc[i].fTimeAccs       = 0.f;
			}
		}
#pragma endregion
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

HRESULT CVIBuffer_Particle::Render(_uint iCount)
{
	m_iNumInstance = iCount;
	if (m_iNumInstance > m_iMaxCount)
		m_iNumInstance = m_iMaxCount;

	ID3D11Buffer* pVertexBuffers[] = {
	m_pVB,
	m_pVBInstance
	};

	_uint			iStrides[] = {
		m_iStride,
		m_iStrideInstance,

	};

	_uint			iOffsets[] = {
		0,
		0,
	};

	/* 버텍스 버퍼들을 할당한다. */
	/* 그리기용 정점버퍼 + 상태변환용 정점버퍼 */
	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);

	/* 인덱스 버퍼를 할당한다. */
	/* 그리고자 하는 인스턴스의 갯수만큼 확대되어있는 인덱스 버퍼. */
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);

	/* 해당 정점들을 어떤 방식으로 그릴꺼야. */
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	/* 인덱스가 가르키는 정점을 활용하여 그린다. */
	m_pContext->DrawIndexedInstanced(m_iNumPrimitives * m_iNumIndicesofPrimitive, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

_float4 CVIBuffer_Particle::Get_NewPosition_Particle()
{
	return _float4(
		CUtils::Random_Float(-(*m_tParticleDesc.pRange).x, (*m_tParticleDesc.pRange).x),
		CUtils::Random_Float(-(*m_tParticleDesc.pRange).y, (*m_tParticleDesc.pRange).y),
		CUtils::Random_Float(-(*m_tParticleDesc.pRange).z, (*m_tParticleDesc.pRange).z), 1.f);
}

_float4 CVIBuffer_Particle::Get_NewVelocity_Particle()
{
	_vector vVelocity = XMVector3Normalize(
		XMVectorSet(
			CUtils::Random_Float((*m_tParticleDesc.pVelocityMinStart).x, (*m_tParticleDesc.pVelocityMaxStart).x),
			CUtils::Random_Float((*m_tParticleDesc.pVelocityMinStart).y, (*m_tParticleDesc.pVelocityMaxStart).y),
			CUtils::Random_Float((*m_tParticleDesc.pVelocityMinStart).z, (*m_tParticleDesc.pVelocityMaxStart).z),
			0.f));

	return _float4(XMVectorGetX(vVelocity), XMVectorGetY(vVelocity), XMVectorGetZ(vVelocity), XMVectorGetW(vVelocity));
}

CVIBuffer_Particle* CVIBuffer_Particle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Particle* pInstance = new CVIBuffer_Particle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CVIBuffer_Particle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Particle::Clone(void* pArg)
{
	CVIBuffer_Particle* pInstance = new CVIBuffer_Particle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CVIBuffer_Particle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Particle::Free()
{
	__super::Free();

	for (auto& iter : m_vecParticleInfoDesc)
	{
		if (iter.pVelocity != nullptr)
			Safe_Delete_Array(iter.pVelocity);

		if (iter.pVelocityChange != nullptr)
			Safe_Delete_Array(iter.pVelocityChange);
	}

	Safe_Delete_Array(m_pVertices);
	Safe_Release(m_pVBInstance);
}