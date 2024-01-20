#include "..\Public\VIBuffer_LensFlare.h"
#include "GameInstance.h"


CVIBuffer_LensFlare::CVIBuffer_LensFlare(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_LensFlare::CVIBuffer_LensFlare(const CVIBuffer_LensFlare& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_LensFlare::Initialize_Prototype()
{
//	m_iNumVertexBuffers = 2;
//
//	m_iStride = sizeof(VTXPOINT);
//	m_iNumVertices = 1;
//
//	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
//	m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
//	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	m_BufferDesc.CPUAccessFlags = 0;
//	m_BufferDesc.MiscFlags = 0;
//	m_BufferDesc.StructureByteStride = m_iStride;
//
//	VTXPOINT* pVertices = new VTXPOINT;
//	ZeroMemory(pVertices, sizeof(VTXPOINT));
//
//	pVertices->vPosition = _float3(0.0f, 0.0f, 0.f);
//	pVertices->vSize = _float2(1.f, 1.f);
//
//	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
//	m_SubResourceData.pSysMem = pVertices;
//
//	if (FAILED(__super::Create_VertexBuffer()))
//		return E_FAIL;
//
//	Safe_Delete_Array(pVertices);
//#pragma endregion
//
//#pragma region INDEXBUFFER
//	m_iNumPrimitives = 1;
//
//	m_iIndexSizeofPrimitive = sizeof(_ushort);
//	m_iNumIndicesofPrimitive = 1;
//	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
//	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
//
//	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
//	m_BufferDesc.ByteWidth = m_iNumPrimitives * m_iIndexSizeofPrimitive;
//	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	m_BufferDesc.CPUAccessFlags = 0;
//	m_BufferDesc.MiscFlags = 0;
//	m_BufferDesc.StructureByteStride = 0;
//
//	_ushort* pIndices = new _ushort[m_iNumPrimitives];
//	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumPrimitives);
//
//	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
//	m_SubResourceData.pSysMem = pIndices;
//
//	if (FAILED(__super::Create_IndexBuffer()))
//		return E_FAIL;
//
//	Safe_Delete_Array(pIndices);
//#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_LensFlare::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_LensFlare::Render(uint32 vertexCount, uint32 instance_count, uint32 start_vertex_location, uint32 start_instance_location)
{
	if (nullptr == m_pContext)
		return E_FAIL;

	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	if (1 == instance_count) m_pContext->Draw(vertexCount, start_vertex_location);
	else m_pContext->DrawInstanced(vertexCount, instance_count, start_vertex_location, start_instance_location);

	return S_OK;
}

void CVIBuffer_LensFlare::Tick(const Vec3& sunWorldPos, _float fTimeDelta)
{
}


CVIBuffer_LensFlare* CVIBuffer_LensFlare::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_LensFlare* pInstance = new CVIBuffer_LensFlare(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CVIBuffer_LensFlare");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_LensFlare::Clone(void* pArg)
{
	CVIBuffer_LensFlare* pInstance = new CVIBuffer_LensFlare(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CVIBuffer_LensFlare");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_LensFlare::Free()
{
	__super::Free();
}
