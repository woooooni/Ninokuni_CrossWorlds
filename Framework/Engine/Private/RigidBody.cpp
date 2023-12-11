#include "RigidBody.h"
#include "GameInstance.h"

CRigidBody::CRigidBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
	
}

CRigidBody::CRigidBody(CRigidBody& rhs)
	: CComponent(rhs)
#ifdef _DEBUG
	, m_pBatch(rhs.m_pBatch)
	, m_pEffect(rhs.m_pEffect)
	, m_pInputLayout(rhs.m_pInputLayout)
#endif // _DEBUG
{
#ifdef _DEBUG
	Safe_AddRef(m_pInputLayout);
#endif // _DEBUG
}

HRESULT CRigidBody::Initialize_Prototype()
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
	return S_OK;
}

HRESULT CRigidBody::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	RIGID_BODY_DESC* pDesc = static_cast<RIGID_BODY_DESC*>(pArg);
	m_pNavigationCom = pDesc->pNavigation;
	m_pTransformCom = pDesc->pTransform;

	if (nullptr == m_pTransformCom)
		return E_FAIL;

	Safe_AddRef(m_pNavigationCom);
	Safe_AddRef(m_pTransformCom);

	

	PxMaterial* pMaterial = GI->Create_PxMaterial(pDesc->fStaticFriction, pDesc->fDynamicFriction, pDesc->fRestitution);

	m_vExtents.x = abs(pDesc->vExtents.x);
	m_vExtents.y = abs(pDesc->vExtents.y);
	m_vExtents.z = abs(pDesc->vExtents.z);


	m_pXBody = GI->Create_PxBox(m_vExtents, pDesc->fWeight, pDesc->fAngleDump, pMaterial, pDesc->fMaxVelocity);
	pMaterial->release();

	_float4 vQuat;
	XMStoreFloat4(&vQuat, XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&pDesc->vRotation)));
	m_pXBody->setGlobalPose(PxTransform(pDesc->vStartPos.x, pDesc->vStartPos.y, pDesc->vStartPos.z, PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w)));

	m_pOriginal_OBB = new BoundingOrientedBox(_float3(0.f, 0.f, 0.f), 
		_float3(pDesc->vExtents.x * 0.5f, 
			pDesc->vExtents.y * 0.5f, 
			pDesc->vExtents.z * 0.5f), _float4(0.f, 0.f, 0.f, 1.f));

	m_vOffsetPos = pDesc->vOffsetPos;

	m_pOBB = new BoundingOrientedBox(*m_pOriginal_OBB);
	m_bFirst = true;

	return S_OK;
}
#ifdef _DEBUG
HRESULT CRigidBody::Render()
{
	m_pEffect->SetWorld(XMMatrixIdentity());

	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	m_pEffect->SetView(pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));

	RELEASE_INSTANCE(CPipeLine);

	m_pEffect->Apply(m_pContext);

	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pBatch->Begin();
	_float4 vColor = _float4(1.f, 0.f, 0.f, 1.f);
	DX::Draw(m_pBatch, *m_pOBB, XMLoadFloat4(&vColor));


	m_pBatch->End();

	return S_OK;
}

#endif

void CRigidBody::Update_RigidBody(_float fTimeDelta)
{
#ifdef _DEBUG
	m_bCollision = false;
#endif // DEBUG
	if (false == m_bFirst && m_pXBody->isSleeping())
		return;

	// Transform에 vPos만큼 더한 값이 BodyX의 값.
	// BodyX에 vPos만큼 뺀 값이 Transform 값.

	PxTransform PxTransform = m_pXBody->getGlobalPose();
	_matrix WorldMatrix;

	_float3 vScale = m_pTransformCom->Get_Scale();

	WorldMatrix.r[CTransform::STATE_RIGHT] = XMVector3Normalize(XMVectorSet(PxTransform.q.getBasisVector0().x, PxTransform.q.getBasisVector0().y, PxTransform.q.getBasisVector0().z, 0.f)) * vScale.x;
	WorldMatrix.r[CTransform::STATE_UP] = XMVector3Normalize(XMVectorSet(PxTransform.q.getBasisVector1().x, PxTransform.q.getBasisVector1().y, PxTransform.q.getBasisVector1().z, 0.f)) * vScale.x;
	WorldMatrix.r[CTransform::STATE_LOOK] = XMVector3Normalize(XMVectorSet(PxTransform.q.getBasisVector2().x, PxTransform.q.getBasisVector2().y, PxTransform.q.getBasisVector2().z, 0.f)) * vScale.x;
	WorldMatrix.r[CTransform::STATE_POSITION] = XMVectorSet(PxTransform.q.x, PxTransform.q.y, PxTransform.q.z, 1.f);

	m_pOriginal_OBB->Transform(*m_pOBB, WorldMatrix);

	WorldMatrix.r[CTransform::STATE_POSITION] = XMVector3TransformCoord(XMLoadFloat3(&m_vOffsetPos), WorldMatrix);
	m_pTransformCom->Set_WorldMatrix(WorldMatrix);
	m_bFirst = false;
}


CRigidBody* CRigidBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRigidBody* pInstance = new CRigidBody(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Crate Failed. : CRigidBody");
		return nullptr;
	}
	return pInstance;
}

CComponent* CRigidBody::Clone(void* pArg)
{
	CRigidBody* pInstance = new CRigidBody(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Clone Failed. : CRigidBody");
		return nullptr;
	}

	return pInstance;
}

void CRigidBody::Free()
{
	__super::Free();

	if (nullptr != m_pXBody)
		m_pXBody->release();
	
#ifdef _DEBUG
	if (false == m_isCloned)
	{
		Safe_Delete(m_pBatch);
		Safe_Delete(m_pEffect);
	}

	Safe_Release(m_pInputLayout);
#endif

	Safe_Delete(m_pOriginal_OBB);
	Safe_Delete(m_pOBB);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pNavigationCom);
}

