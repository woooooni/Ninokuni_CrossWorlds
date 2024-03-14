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
	m_iNumVertexBuffers = 1;
	m_iNumVertices = 3;

	m_iStride = sizeof(VTXCOL);

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXCOL* pVertices = new VTXCOL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXCOL) * m_iNumVertices);

	pVertices[0].vPosition = Vec3(-1.0f, -1.0f, 0.0f);
	pVertices[0].vColor = Vec4(0.0f, 1.0f, 0.0f, 1.0f);

	pVertices[1].vPosition = Vec3(0.0f, 1.0f, 0.0f);
	pVertices[1].vColor = Vec4(0.0f, 1.0f, 0.0f, 1.0f);

	pVertices[2].vPosition = Vec3(1.0f, -1.0f, 0.0f);
	pVertices[2].vColor = Vec4(0.0f, 1.0f, 0.0f, 1.0f);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INDEXBUFFER
	m_iNumPrimitives = 3;
	m_iIndexSizeofPrimitive = sizeof(FACEINDICES16);
	m_iNumIndicesofPrimitive = 1;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
	// TOPOLOGY_4 -> RECT 

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumPrimitives * m_iIndexSizeofPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;


	FACEINDICES16* pIndices = new FACEINDICES16[m_iNumPrimitives];
	ZeroMemory(pIndices, sizeof(FACEINDICES16) * m_iNumPrimitives);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	/*pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;*/

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
	return S_OK;
}

void CVIBuffer_Grass::Tick(_float fTImeDelta)
{
}

HRESULT CVIBuffer_Grass::Render()
{
	__super::Render();

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
}
