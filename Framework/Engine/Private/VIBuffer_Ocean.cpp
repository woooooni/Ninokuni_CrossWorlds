#include "..\Public\VIBuffer_Ocean.h"

CVIBuffer_Ocean::CVIBuffer_Ocean(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Ocean::CVIBuffer_Ocean(const CVIBuffer_Ocean& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Ocean::Initialize_Prototype(_uint width, _uint height, _float thick)
{
	m_iNumVertexBuffers = 1;

	m_iNumVertices = (width + 1) * (height * 1);
	m_iStride = sizeof(VTXNORTEX);
	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];
	::ZeroMemory(pVertices, sizeof(VTXPOSTEX) * m_iNumVertices);

	for (_uint z = 0; z < height; ++z)
	{
		for (_uint x = 0; x < width; ++x)
		{
			_uint index = (width + 1) * z + x;

			pVertices[index].vPosition.x = static_cast<_float>(x) * thick;
			pVertices[index].vPosition.y = 0.0f;
			pVertices[index].vPosition.z = static_cast<_float>(z) * thick;

			pVertices[index].vTexcoord.x = static_cast<_float>(x) / static_cast<_float>(width);
			pVertices[index].vTexcoord.y = static_cast<_float>(z) / static_cast<_float>(height);
		}
	}

	m_iNumPrimitives = width * height * 6;
	m_iIndexSizeofPrimitive = sizeof(_ulong);
	m_iNumIndicesofPrimitive = 1;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;


	_ulong* pIndices = new _ulong[m_iNumPrimitives];
	::ZeroMemory(pIndices, sizeof(_ulong) * m_iNumPrimitives);

	_uint iIndex = 0;
	for (_uint z = 0; z < height; ++z)
	{
		for (_uint x = 0; x < width; ++x)
		{
			pIndices[iIndex + 0] = (width + 1) * z + x; // 0;
			pIndices[iIndex + 1] = (width + 1) * (z + 1) + x; //1
			pIndices[iIndex + 2] = (width + 1) * z + x + 1; //2 

			_vector vSourDir, vDestDir, vNormal;

			vSourDir = XMLoadFloat3(&pVertices[pIndices[iIndex + 1]].vPosition) - XMLoadFloat3(&pVertices[pIndices[iIndex + 0]].vPosition);
			vDestDir = XMLoadFloat3(&pVertices[pIndices[iIndex + 2]].vPosition) - XMLoadFloat3(&pVertices[pIndices[iIndex + 1]].vPosition);


			pIndices[iIndex + 3] = (width + 1) * z + x + 1; // 2
			pIndices[iIndex + 4] = (width + 1) * (z + 1) + x; // 1;
			pIndices[iIndex + 5] = (width + 1) * (z + 1) + x + 1; // 3

			iIndex += 6;
		}
	}

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumPrimitives * m_iIndexSizeofPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pIndices);


	return S_OK;
}

HRESULT CVIBuffer_Ocean::Initialize(void* pArg)
{

	return S_OK;
}

CVIBuffer_Ocean* CVIBuffer_Ocean::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint width, _uint height, _float thick)
{
	CVIBuffer_Ocean* pInstance = new CVIBuffer_Ocean(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(width, height, thick)))
	{
		MSG_BOX("Failed To Created : CVIBuffer_Ocean");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Ocean::Clone(void* pArg)
{
	CVIBuffer_Ocean* pInstance = new CVIBuffer_Ocean(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CVIBuffer_Ocean");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Ocean::Free()
{
	__super::Free();
}
