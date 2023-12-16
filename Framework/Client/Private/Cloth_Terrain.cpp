#include "stdafx.h"
#include "..\Public\Cloth_Terrain.h"
#include "GameInstance.h"
#include "VIBuffer_Terrain.h"
#include "DebugDraw.h"
#include "PhysicsX_Manager.h"

CCloth_Terrain::CCloth_Terrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext, L"Cloth_Terrain", OBJ_TYPE::OBJ_TERRAIN)
{

}

CCloth_Terrain::CCloth_Terrain(const CGameObject & rhs)
	: CGameObject(rhs)
{

}

HRESULT CCloth_Terrain::Initialize_Prototype()
{



	return S_OK;
}

HRESULT CCloth_Terrain::Initialize(void* pArg)
{
#ifdef _DEBUG

	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);

	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCodes = nullptr;
	size_t			iLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCodes, &iLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCodes, iLength, &m_pInputLayout)))
		return E_FAIL;
#endif

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CCloth_Terrain::Priority_Tick(_float fTimeDelta)
{

}

void CCloth_Terrain::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCloth_Terrain::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONBLEND, this);

}

HRESULT CCloth_Terrain::Render()
{

#ifdef _DEBUG

	if (m_bDraw)
	{
		m_pEffect->SetWorld(m_pTransformCom->Get_WorldMatrix());
		m_pEffect->SetView(GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
		m_pEffect->SetProjection(GI->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));


		m_pEffect->Apply(m_pContext);

		m_pContext->IASetInputLayout(m_pInputLayout);


		m_pBatch->Begin();
		
		PxParticleClothBuffer* userBuffer = GI->Get_TestClothBuffer();
		PxVec4* positions = userBuffer->getPositionInvMasses();
		PxU32* Indices = userBuffer->getTriangles();


		PxVec4 vertices[10000];
		PxU32 triangles[(100 - 1) * (100 - 1) * 2];
		

		const PxU32 numParticles = userBuffer->getNbActiveParticles();

		PxCudaContextManager* pContextManager = GI->Get_CudaContext_Manager();
		pContextManager->acquireContext();

		PxCudaContext* cudaContext = pContextManager->getCudaContext();



		cudaContext->memcpyDtoH(vertices, CUdeviceptr(positions), sizeof(PxVec4) * 10000);
		cudaContext->memcpyDtoH(triangles, CUdeviceptr(Indices), sizeof(PxU32) * ((100 - 1) * (100 - 1) * 2));



		pContextManager->releaseContext();
		for (_uint i = 0; i < ((100 - 1) * (100 - 1) * 2); i += 3)
		{
			_vector vPoint0 = XMVectorSet(vertices[triangles[i]].x, vertices[triangles[i]].y, vertices[triangles[i]].z, vertices[triangles[i]].w);
			_vector vPoint1 = XMVectorSet(vertices[triangles[i + 1]].x, vertices[triangles[i + 1]].y, vertices[triangles[i + 1]].z, vertices[triangles[i + 1]].w);
			_vector vPoint2 = XMVectorSet(vertices[triangles[i + 2]].x, vertices[triangles[i + 2]].y, vertices[triangles[i + 2]].z, vertices[triangles[i + 2]].w);

			DX::DrawTriangle(m_pBatch, vPoint0, vPoint1, vPoint2);
		}

		m_pBatch->End();
	}

	if (FAILED(m_pNavigationCom->Render()))
		return E_FAIL;
	
#endif

	return S_OK;
}


HRESULT CCloth_Terrain::Ready_Components()
{
	/* Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cloth_Terrain"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	///* Com_Texture*/
	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Terrain"),
	//	TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	//	return E_FAIL;



	/* Com_Navigation */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), (CComponent**)&m_pNavigationCom)))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCloth_Terrain::Bind_ShaderResources()
{
	/* 셰이더 전역변수로 던져야 할 값들을 던지자. */
	/*_float4x4 IdentityMatrix;
	XMStoreFloat4x4(&IdentityMatrix, XMMatrixIdentity());
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &IdentityMatrix)))
		return E_FAIL;*/

	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(GI->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::D3DTS_VIEW)))
		return E_FAIL;
	if (FAILED(GI->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::D3DTS_PROJ)))
		return E_FAIL;




	return S_OK;
}

CCloth_Terrain * CCloth_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCloth_Terrain*	pInstance = new CCloth_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCloth_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCloth_Terrain::Clone(void* pArg)
{
	CCloth_Terrain*	pInstance = new CCloth_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCloth_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCloth_Terrain::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pVIBufferCom);


#ifdef _DEBUG
	Safe_Delete(m_pBatch);
	Safe_Delete(m_pEffect);
	Safe_Release(m_pInputLayout);
#endif
}

