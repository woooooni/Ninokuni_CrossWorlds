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

void CVIBuffer_Particle::Set_ParticleBufferDesc(const PARTICLE_BUFFER_DESC& tDesc)
{
	m_tParticleDesc = tDesc;

	D3D11_MAPPED_SUBRESOURCE SubResource;
	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		// 스케일
		if (m_tParticleDesc.bSameRate) // 정비율
		{
			_float fScale = CUtils::Random_Float(m_tParticleDesc.fScale.x, m_tParticleDesc.fScale.y);
			((VTXINSTANCE*)SubResource.pData)[i].vRight = _float4(fScale, 0.f, 0.f, 0.f);
			((VTXINSTANCE*)SubResource.pData)[i].vUp = _float4(0.f, fScale, 0.f, 0.f);
			((VTXINSTANCE*)SubResource.pData)[i].vLook = _float4(0.f, 0.f, fScale, 0.f);
		}
		else
		{
			((VTXINSTANCE*)SubResource.pData)[i].vRight = _float4(CUtils::Random_Float(m_tParticleDesc.fScale.x, m_tParticleDesc.fScale.y), 0.f, 0.f, 0.f);
			((VTXINSTANCE*)SubResource.pData)[i].vUp = _float4(0.f, CUtils::Random_Float(m_tParticleDesc.fScale.x, m_tParticleDesc.fScale.y), 0.f, 0.f);
			((VTXINSTANCE*)SubResource.pData)[i].vLook = _float4(0.f, 0.f, CUtils::Random_Float(m_tParticleDesc.fScale.x, m_tParticleDesc.fScale.y), 0.f);
		}

		// 포지션
		((VTXINSTANCE*)SubResource.pData)[i].vPosition = _float4(
			CUtils::Random_Float(-m_tParticleDesc.fRange.x, m_tParticleDesc.fRange.x),
			CUtils::Random_Float(-m_tParticleDesc.fRange.y, m_tParticleDesc.fRange.y),
			CUtils::Random_Float(-m_tParticleDesc.fRange.z, m_tParticleDesc.fRange.z), 1.f);

		// 시간
		m_vecParticleInfoDesc[i].fTimeAccs = 0;
		m_vecParticleInfoDesc[i].fLifeTimes = CUtils::Random_Float(m_tParticleDesc.fLifeTime.x, m_tParticleDesc.fLifeTime.y);

		// 이동 방향
		_vector vVelocity = XMVector3Normalize(
			XMVectorSet(
				CUtils::Random_Float(m_tParticleDesc.vVelocityMin.x, m_tParticleDesc.vVelocityMax.x),
				CUtils::Random_Float(m_tParticleDesc.vVelocityMin.y, m_tParticleDesc.vVelocityMax.y),
				CUtils::Random_Float(m_tParticleDesc.vVelocityMin.z, m_tParticleDesc.vVelocityMax.z),
				0.f));
		m_vecParticleInfoDesc[i].vVelocity = _float4(XMVectorGetX(vVelocity), XMVectorGetY(vVelocity), XMVectorGetZ(vVelocity), XMVectorGetW(vVelocity));

		// 속도
		m_vecParticleInfoDesc[i].fSpeeds = CUtils::Random_Float(m_tParticleDesc.fSpeed.x, m_tParticleDesc.fSpeed.y);
		m_vecParticleInfoDesc[i].fAnimationSpeed = CUtils::Random_Float(m_tParticleDesc.fAnimationSpeed.x, m_tParticleDesc.fAnimationSpeed.y);


		/* 쉐이더 바인딩 정보 */
		// 텍스처 인덱스
		if (m_tParticleDesc.bRandomStartIndex)
			m_vecParticleShaderDesc[i].fUVIndex = _float2(
				(_int)CUtils::Random_Float(0.f, m_tParticleDesc.fUVMaxCount.x),
				(_int)CUtils::Random_Float(0.f, m_tParticleDesc.fUVMaxCount.y));
		else
			m_vecParticleShaderDesc[i].fUVIndex = m_tParticleDesc.fUVIndex;

		m_vecParticleShaderDesc[i].fMaxCount = m_tParticleDesc.fUVMaxCount;

		// 색상
		if (m_tParticleDesc.bRandomColor)
			m_vecParticleShaderDesc[i].vColor = _float4(CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.5f, 1.f));
		else
			m_vecParticleShaderDesc[i].vColor = m_tParticleDesc.vDiffuseColor;
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
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStrideInstance;

	m_pVertices = new VTXINSTANCE[m_iMaxCount];
	ZeroMemory(m_pVertices, sizeof(VTXINSTANCE) * m_iMaxCount);

	PARTICLE_INFO_DESC   ParticleInfo = {};
	PARTICLE_SHADER_DESC ParticleShaderInfo = {};
	for (size_t i = 0; i < m_iMaxCount; i++)
	{
		// 스케일
		if (m_tParticleDesc.bSameRate) // 정비율
		{
			_float fScale = CUtils::Random_Float(m_tParticleDesc.fScale.x, m_tParticleDesc.fScale.y);
			m_pVertices[i].vRight = _float4(fScale, 0.f, 0.f, 0.f);
			m_pVertices[i].vUp = _float4(0.f, fScale, 0.f, 0.f);
			m_pVertices[i].vLook = _float4(0.f, 0.f, fScale, 0.f);
		}
		else
		{
			m_pVertices[i].vRight = _float4(CUtils::Random_Float(m_tParticleDesc.fScale.x, m_tParticleDesc.fScale.y), 0.f, 0.f, 0.f);
			m_pVertices[i].vUp = _float4(0.f, CUtils::Random_Float(m_tParticleDesc.fScale.x, m_tParticleDesc.fScale.y), 0.f, 0.f);
			m_pVertices[i].vLook = _float4(0.f, 0.f, CUtils::Random_Float(m_tParticleDesc.fScale.x, m_tParticleDesc.fScale.y), 0.f);
		}

		// 포지션
		m_pVertices[i].vPosition = _float4(
			CUtils::Random_Float(-m_tParticleDesc.fRange.x, m_tParticleDesc.fRange.x),
			CUtils::Random_Float(-m_tParticleDesc.fRange.y, m_tParticleDesc.fRange.y),
			CUtils::Random_Float(-m_tParticleDesc.fRange.z, m_tParticleDesc.fRange.z), 1.f);

		// 시간
		ParticleInfo.fTimeAccs = 0;
		ParticleInfo.fLifeTimes = CUtils::Random_Float(m_tParticleDesc.fLifeTime.x, m_tParticleDesc.fLifeTime.y);

		// 이동 방향
		_vector vVelocity = XMVector3Normalize(
			XMVectorSet(
				CUtils::Random_Float(m_tParticleDesc.vVelocityMin.x, m_tParticleDesc.vVelocityMax.x),
				CUtils::Random_Float(m_tParticleDesc.vVelocityMin.y, m_tParticleDesc.vVelocityMax.y),
				CUtils::Random_Float(m_tParticleDesc.vVelocityMin.z, m_tParticleDesc.vVelocityMax.z),
				0.f));
		ParticleInfo.vVelocity = _float4(XMVectorGetX(vVelocity), XMVectorGetY(vVelocity), XMVectorGetZ(vVelocity), XMVectorGetW(vVelocity));

		// 속도
		ParticleInfo.fSpeeds = CUtils::Random_Float(m_tParticleDesc.fSpeed.x, m_tParticleDesc.fSpeed.y);
		ParticleInfo.fAnimationSpeed = CUtils::Random_Float(m_tParticleDesc.fAnimationSpeed.x, m_tParticleDesc.fAnimationSpeed.y);

		m_vecParticleInfoDesc.push_back(ParticleInfo);


		/* 쉐이더 바인딩 정보 */
		// 텍스처 인덱스
		if (m_tParticleDesc.bRandomStartIndex)
			ParticleShaderInfo.fUVIndex = _float2(
				(_int)CUtils::Random_Float(0.f, m_tParticleDesc.fUVMaxCount.x),
				(_int)CUtils::Random_Float(0.f, m_tParticleDesc.fUVMaxCount.y));
		else
			ParticleShaderInfo.fUVIndex = m_tParticleDesc.fUVIndex;

		ParticleShaderInfo.fMaxCount = m_tParticleDesc.fUVMaxCount;

		// 색상
		if (m_tParticleDesc.bRandomColor)
			ParticleShaderInfo.vColor = _float4(CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.5f, 1.f));
		else
			ParticleShaderInfo.vColor = m_tParticleDesc.vDiffuseColor;

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
		if (m_vecParticleInfoDesc[i].fTimeAccs < m_vecParticleInfoDesc[i].fLifeTimes)
		{
			m_vecParticleInfoDesc[i].fTimeAccs += fTimeDelta;

			((VTXINSTANCE*)SubResource.pData)[i].vPosition.x += m_vecParticleInfoDesc[i].vVelocity.x * m_vecParticleInfoDesc[i].fSpeeds * fTimeDelta;
			((VTXINSTANCE*)SubResource.pData)[i].vPosition.y += m_vecParticleInfoDesc[i].vVelocity.y * m_vecParticleInfoDesc[i].fSpeeds * fTimeDelta;
			((VTXINSTANCE*)SubResource.pData)[i].vPosition.z += m_vecParticleInfoDesc[i].vVelocity.z * m_vecParticleInfoDesc[i].fSpeeds * fTimeDelta;

			// 박스 범위 검사
			if (m_tParticleDesc.bUseBox)
			{
				if (((VTXINSTANCE*)SubResource.pData)[i].vPosition.x < m_tParticleDesc.fBoxMin.x ||
					((VTXINSTANCE*)SubResource.pData)[i].vPosition.y < m_tParticleDesc.fBoxMin.y ||
					((VTXINSTANCE*)SubResource.pData)[i].vPosition.z < m_tParticleDesc.fBoxMin.z ||
					((VTXINSTANCE*)SubResource.pData)[i].vPosition.x > m_tParticleDesc.fBoxMax.x ||
					((VTXINSTANCE*)SubResource.pData)[i].vPosition.y > m_tParticleDesc.fBoxMax.y ||
					((VTXINSTANCE*)SubResource.pData)[i].vPosition.z > m_tParticleDesc.fBoxMax.z)
				{
					// 반복X
					if (!m_bFinished && !m_tParticleDesc.bLoop)
					{
						iDieEffect++;
						if (iDieEffect == m_iNumInstance)
							m_bFinished = true;

						((VTXINSTANCE*)SubResource.pData)[i].vRight = _float4(0.f, 0.f, 0.f, 0.f);
						((VTXINSTANCE*)SubResource.pData)[i].vUp = _float4(0.f, 0.f, 0.f, 0.f);
						((VTXINSTANCE*)SubResource.pData)[i].vLook = _float4(0.f, 0.f, 0.f, 0.f);
					}
					// 반복O
					else
					{
						((VTXINSTANCE*)SubResource.pData)[i].vPosition = _float4(
							CUtils::Random_Float(-m_tParticleDesc.fRange.x, m_tParticleDesc.fRange.x),
							CUtils::Random_Float(-m_tParticleDesc.fRange.y, m_tParticleDesc.fRange.y),
							CUtils::Random_Float(-m_tParticleDesc.fRange.z, m_tParticleDesc.fRange.z), 1.f);

						_vector vVelocity = XMVector3Normalize(
							XMVectorSet(
								CUtils::Random_Float(m_tParticleDesc.vVelocityMin.x, m_tParticleDesc.vVelocityMax.x),
								CUtils::Random_Float(m_tParticleDesc.vVelocityMin.y, m_tParticleDesc.vVelocityMax.y),
								CUtils::Random_Float(m_tParticleDesc.vVelocityMin.z, m_tParticleDesc.vVelocityMax.z),
								0.f));
						m_vecParticleInfoDesc[i].vVelocity = _float4(XMVectorGetX(vVelocity), XMVectorGetY(vVelocity), XMVectorGetZ(vVelocity), XMVectorGetW(vVelocity));

						m_vecParticleInfoDesc[i].fSpeeds = CUtils::Random_Float(m_tParticleDesc.fSpeed.x, m_tParticleDesc.fSpeed.y);

						m_vecParticleInfoDesc[i].fTimeAccs = 0.f;
					}
				}
			}

			// 텍스처 애니메이션
			if (m_tParticleDesc.bAnimation)
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
							if (m_tParticleDesc.bAnimationLoop)
								m_vecParticleShaderDesc[i].fUVIndex = _float2(0.f, 0.f);
							else
								m_vecParticleShaderDesc[i].fUVIndex.y = m_vecParticleShaderDesc[i].fMaxCount.y - 1;
						}
					}
				}
			}
		}
		else // 지속시간이 다 지났을 시
		{
			// 반복X
			if (!m_bFinished && !m_tParticleDesc.bLoop)
			{
				iDieEffect++;
				if (iDieEffect == m_iNumInstance)
					m_bFinished = true;

				((VTXINSTANCE*)SubResource.pData)[i].vRight = _float4(0.f, 0.f, 0.f, 0.f);
				((VTXINSTANCE*)SubResource.pData)[i].vUp = _float4(0.f, 0.f, 0.f, 0.f);
				((VTXINSTANCE*)SubResource.pData)[i].vLook = _float4(0.f, 0.f, 0.f, 0.f);
			}
			// 반복O
			else
			{
				((VTXINSTANCE*)SubResource.pData)[i].vPosition = _float4(
					CUtils::Random_Float(-m_tParticleDesc.fRange.x, m_tParticleDesc.fRange.x),
					CUtils::Random_Float(-m_tParticleDesc.fRange.y, m_tParticleDesc.fRange.y),
					CUtils::Random_Float(-m_tParticleDesc.fRange.z, m_tParticleDesc.fRange.z), 1.f);

				_vector vVelocity = XMVector3Normalize(
					XMVectorSet(
						CUtils::Random_Float(m_tParticleDesc.vVelocityMin.x, m_tParticleDesc.vVelocityMax.x),
						CUtils::Random_Float(m_tParticleDesc.vVelocityMin.y, m_tParticleDesc.vVelocityMax.y),
						CUtils::Random_Float(m_tParticleDesc.vVelocityMin.z, m_tParticleDesc.vVelocityMax.z),
						0.f));
				m_vecParticleInfoDesc[i].vVelocity = _float4(XMVectorGetX(vVelocity), XMVectorGetY(vVelocity), XMVectorGetZ(vVelocity), XMVectorGetW(vVelocity));

				m_vecParticleInfoDesc[i].fSpeeds = CUtils::Random_Float(m_tParticleDesc.fSpeed.x, m_tParticleDesc.fSpeed.y);

				m_vecParticleInfoDesc[i].fTimeAccs = 0.f;
			}
		}
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

	Safe_Delete_Array(m_pVertices);
	Safe_Release(m_pVBInstance);
}