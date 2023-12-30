#include "VIBuffer_Grass.h"
#include "Utils.h"

CVIBuffer_Grass::CVIBuffer_Grass(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Grass::CVIBuffer_Grass(const CVIBuffer_Grass& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Grass::Initialize_Prototype()
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

HRESULT CVIBuffer_Grass::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	const _uint iMaxCount = 1000;

	m_iStrideInstance = sizeof(VTXINSTANCE);
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iStrideInstance * iMaxCount;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStrideInstance;

	m_pVertices = new VTXINSTANCE[iMaxCount];
	ZeroMemory(m_pVertices, sizeof(VTXINSTANCE) * iMaxCount);

	for (size_t i = 0; i < iMaxCount; ++i)
	{
		m_pVertices[i].vRight = Vec4(1.0f, 0.0f, 0.0f, 0.0f);
		m_pVertices[i].vUp = Vec4(0.0f, 1.0f, 0.0f, 0.0f);
		m_pVertices[i].vLook = Vec4(0.0f, 0.0f, 1.0f, 0.0f);
		m_pVertices[i].vPosition = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	::ZeroMemory(&m_SubResourceData, sizeof(m_SubResourceData));
	m_SubResourceData.pSysMem = m_pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVBInstance)))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer_Grass::Tick(_float fTImeDelta)
{
}

HRESULT CVIBuffer_Grass::Render(_uint iCount)
{
	m_iNumInstance = iCount;

	const _uint iMaxCount = 1000;

	if (m_iNumInstance > iMaxCount)
		m_iNumInstance = iMaxCount;

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

CVIBuffer_Grass* CVIBuffer_Grass::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Grass* pInstance = new CVIBuffer_Grass(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Faild to Created : CVIBuffer_Grass");
		Safe_Release<CVIBuffer_Grass*>(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Grass::Clone(void* pArg)
{
	CVIBuffer_Grass* pInstance = new CVIBuffer_Grass(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Faild to Cloned : CIVBuffer_Glass");
		Safe_Release<CVIBuffer_Grass*>(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Grass::Free()
{
	__super::Free();

	Safe_Delete_Array<VTXINSTANCE*>(m_pVertices);
	Safe_Release<ID3D11Buffer*>(m_pVBInstance);
}
