#include "VIBuffer_SkyPlane.h"

CVIBuffer_SkyPlane::CVIBuffer_SkyPlane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_SkyPlane::CVIBuffer_SkyPlane(const CVIBuffer_SkyPlane& rhs)
	: CVIBuffer(rhs)

{

}

HRESULT CVIBuffer_SkyPlane::Initialize_Prototype()
{
	// TODO
	Initialize_SkyPlane();
	
#pragma region VERTEXBUFFER
	m_iNumVertexBuffers = 1;
	m_iNumVertices = (m_iSkyPlaneResolution + 1) * (m_iSkyPlaneResolution + 1) * 6;
	m_iStride = sizeof(VTXPOSTEX);
	m_iNumPrimitives = m_iNumVertices;

	VTXPOSTEX* pVertices = new VTXPOSTEX[m_iNumVertices];
	if (nullptr == pVertices)
		return E_FAIL;

	_ulong* pIndices = new _ulong[m_iNumPrimitives];
	if (nullptr == pIndices)
		return E_FAIL;

	_uint index = 0;

	for (_uint j = 0; j < m_iSkyPlaneResolution; ++j)
	{
		for (_uint i = 0; i < m_iSkyPlaneResolution; ++i)
		{
			_uint iIndex[] = {
				j * (m_iSkyPlaneResolution + 1) + i,
				j * (m_iSkyPlaneResolution + 1) + (i + 1),
				(j + 1) * (m_iSkyPlaneResolution + 1) + i,
				(j + 1) * (m_iSkyPlaneResolution + 1) + (i + 1)
			};

			pVertices[index].vPosition = Vec3(m_pSkyPlane[iIndex[0]].vPosition.x, m_pSkyPlane[iIndex[0]].vPosition.y,
				m_pSkyPlane[iIndex[0]].vPosition.z);
			pVertices[index].vTexcoord = Vec2(m_pSkyPlane[iIndex[0]].vUV.x, m_pSkyPlane[iIndex[0]].vUV.y);
			pIndices[index] = index;
			index++;


			pVertices[index].vPosition = Vec3(m_pSkyPlane[iIndex[1]].vPosition.x, m_pSkyPlane[iIndex[1]].vPosition.y,
				m_pSkyPlane[iIndex[1]].vPosition.z);
			pVertices[index].vTexcoord = Vec2(m_pSkyPlane[iIndex[1]].vUV.x, m_pSkyPlane[iIndex[1]].vUV.y);
			pIndices[index] = index;
			index++;

			pVertices[index].vPosition = Vec3(m_pSkyPlane[iIndex[2]].vPosition.x, m_pSkyPlane[iIndex[2]].vPosition.y,
				m_pSkyPlane[iIndex[2]].vPosition.z);
			pVertices[index].vTexcoord = Vec2(m_pSkyPlane[iIndex[2]].vUV.x, m_pSkyPlane[iIndex[2]].vUV.y);
			pIndices[index] = index;
			index++;

			//

			pVertices[index].vPosition = Vec3(m_pSkyPlane[iIndex[2]].vPosition.x, m_pSkyPlane[iIndex[2]].vPosition.y,
				m_pSkyPlane[iIndex[2]].vPosition.z);
			pVertices[index].vTexcoord = Vec2(m_pSkyPlane[iIndex[2]].vUV.x, m_pSkyPlane[iIndex[2]].vUV.y);
			pIndices[index] = index;
			index++;

			pVertices[index].vPosition = Vec3(m_pSkyPlane[iIndex[1]].vPosition.x, m_pSkyPlane[iIndex[1]].vPosition.y,
				m_pSkyPlane[iIndex[1]].vPosition.z);
			pVertices[index].vTexcoord = Vec2(m_pSkyPlane[iIndex[1]].vUV.x, m_pSkyPlane[iIndex[1]].vUV.y);
			pIndices[index] = index;
			index++;

			pVertices[index].vPosition = Vec3(m_pSkyPlane[iIndex[3]].vPosition.x, m_pSkyPlane[iIndex[3]].vPosition.y,
				m_pSkyPlane[iIndex[3]].vPosition.z);
			pVertices[index].vTexcoord = Vec2(m_pSkyPlane[iIndex[3]].vUV.x, m_pSkyPlane[iIndex[3]].vUV.y);
			pIndices[index] = index;
			index++;
		}
	}

	Safe_Delete_Array<SkyPlaneType*>(m_pSkyPlane);


	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;
#pragma endregion

#pragma region INDEXBUFFER
	m_iIndexSizeofPrimitive = sizeof(_ulong);
	m_iNumIndicesofPrimitive = 1;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumPrimitives * m_iIndexSizeofPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;


	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;


	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pIndices);

#pragma endregion


	return S_OK;
}

HRESULT CVIBuffer_SkyPlane::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_SkyPlane::Initialize_SkyPlane()
{
	_int iTextureRepeat = 4;
	_float fSkyPlaneWidth = 10.f;
	_float fSkyPlaneTop = 0.5f;
	_float fSkyPlaneBottom = 0.0f;

	m_pSkyPlane = new SkyPlaneType[(m_iSkyPlaneResolution + 1) * (m_iSkyPlaneResolution + 1)];
	if (nullptr == m_pSkyPlane)
		return E_FAIL;

	_float fQuadSize = fSkyPlaneWidth / static_cast<_float>(m_iSkyPlaneResolution);
	_float fRadius = fSkyPlaneWidth * 0.5f;
	_float fConstant = (fSkyPlaneTop - fSkyPlaneBottom) / (fRadius * fRadius);
	_float fTextureDelta = static_cast<_float>(iTextureRepeat) / static_cast<_float>(m_iSkyPlaneResolution);

	Vec3 vPosition;
	Vec2 vUV;

	for (_uint j = 0; j <= m_iSkyPlaneResolution; ++j)
	{
		for (_uint i = 0; i <= m_iSkyPlaneResolution; ++i)
		{
			vPosition.x = (-0.5f * fSkyPlaneWidth) + (static_cast<_float>(i) * fQuadSize);
			vPosition.z = (-0.5f * fSkyPlaneWidth) + (static_cast<_float>(j) * fQuadSize);
			vPosition.y = fSkyPlaneTop - (fConstant * ((vPosition.x * vPosition.x) +
				(vPosition.z * vPosition.z)));

			vUV.x = static_cast<_float>(i) * fTextureDelta;
			vUV.y = static_cast<_float>(j) * fTextureDelta;

			_int index = j * (m_iSkyPlaneResolution + 1) + i;

			m_pSkyPlane[index].vPosition.x = vPosition.x;
			m_pSkyPlane[index].vPosition.y = vPosition.y;
			m_pSkyPlane[index].vPosition.z = vPosition.z;

			m_pSkyPlane[index].vUV.x = vUV.x;
			m_pSkyPlane[index].vUV.y = vUV.y;
		}
	}

	return S_OK;
}

CVIBuffer_SkyPlane* CVIBuffer_SkyPlane::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_SkyPlane* pInstance = new CVIBuffer_SkyPlane(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CVIBuffer_SkyPlane");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_SkyPlane::Clone(void* pArg)
{
	CVIBuffer_SkyPlane* pInstance = new CVIBuffer_SkyPlane(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CVIBuffer_SkyPlane");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_SkyPlane::Free()
{
	__super::Free();
}
