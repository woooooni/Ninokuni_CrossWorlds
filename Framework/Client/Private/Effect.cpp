#include "stdafx.h"
#include "Effect.h"
#include "GameInstance.h"
#include "Effect_Manager.h"



CEffect::CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_EFFECT)
{
}

CEffect::CEffect(const CEffect& rhs)
	: CGameObject(rhs)
	, m_tEffectDesc(rhs.m_tEffectDesc)
	, m_fAccDeletionTime(0.f)
	, m_fAccIndex(0.f)
{
	m_tEffectDesc.fAlpha = 1.f;
}

HRESULT CEffect::Initialize_Prototype(const EFFECT_DESC* tEffectDesc)
{
	if (tEffectDesc != nullptr)
		m_tEffectDesc = *tEffectDesc;

	return S_OK;
}

HRESULT CEffect::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (nullptr != pArg)
		m_pOwnerObject = (CGameObject*)pArg;

	//if(m_pDiffuseTextureCom != nullptr)
	//	m_tEffectDesc.iTextureIndexDiffuse = m_pDiffuseTextureCom->Find_Index(m_tEffectDesc.strDiffuseTetextureName);
	//if (m_pAlphaTextureCom != nullptr)
	//	m_tEffectDesc.iTextureIndexAlpha = m_pAlphaTextureCom->Find_Index(m_tEffectDesc.strAlphaTexturName);
	XMStoreFloat4x4(&m_ParentMatrix, XMMatrixIdentity());

	return S_OK;
}

void CEffect::Tick(_float fTimeDelta)
{
	if (m_bDead && GI->Get_CurrentLevel() != LEVEL_TOOL)
		return;

	m_fAccDeletionTime += fTimeDelta;

	if (m_fAccDeletionTime >= m_tEffectDesc.fLifeTime)
	{
		m_fAccDeletionTime = 0.f;
		//Set_Dead(true);
	}

	if (m_tEffectDesc.bIncrement)
		Increment(fTimeDelta);
	else
		Decrement(fTimeDelta);

	m_tEffectDesc.fAccUVFlow.x += m_tEffectDesc.vUVFlow.x * fTimeDelta;
	m_tEffectDesc.fAccUVFlow.y += m_tEffectDesc.vUVFlow.y * fTimeDelta;
	if(m_tEffectDesc.fAlpha > 0.f)
		m_tEffectDesc.fAlpha -= m_tEffectDesc.fDestAlphaSpeed * fTimeDelta;


	_matrix WorldMatrix = m_pTransformCom->Get_WorldMatrix();
	WorldMatrix.r[CTransform::STATE_RIGHT] += XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Normalize(XMLoadFloat3(&m_tEffectDesc.vScaleDir))) * m_tEffectDesc.fScaleSpeed * fTimeDelta;
	WorldMatrix.r[CTransform::STATE_UP] += XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetY(XMVector3Normalize(XMLoadFloat3(&m_tEffectDesc.vScaleDir))) * m_tEffectDesc.fScaleSpeed * fTimeDelta;
	WorldMatrix.r[CTransform::STATE_LOOK] += XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetZ(XMVector3Normalize(XMLoadFloat3(&m_tEffectDesc.vScaleDir))) * m_tEffectDesc.fScaleSpeed * fTimeDelta;
	m_pTransformCom->Set_WorldMatrix(WorldMatrix);

	
	_vector vMoveDir = XMVector3Normalize(XMLoadFloat3(&m_tEffectDesc.vMoveDir));
	_vector vTurnDir = XMVector3Normalize(XMLoadFloat3(&m_tEffectDesc.vTurnDir));

	m_pTransformCom->Move(vMoveDir, m_tEffectDesc.fMoveSpeed, fTimeDelta);
	m_pTransformCom->Turn(vTurnDir, m_tEffectDesc.fTurnSpeed, fTimeDelta);

	if (nullptr != m_pOwnerObject)
	{
		CTransform* pOwnerTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");

		if (nullptr != pOwnerTransform)
		{
			XMStoreFloat4x4(&m_ParentMatrix, pOwnerTransform->Get_WorldMatrix());
		}
	}
}

void CEffect::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_bDead && GI->Get_CurrentLevel() != LEVEL_TOOL)
		return;

	_float4x4 WorldMatrix;
	_matrix OffetMatrix = XMLoadFloat4x4(&m_tEffectDesc.OffsetMatrix);
	XMStoreFloat4x4(&WorldMatrix, OffetMatrix * m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(&m_ParentMatrix));

	if (m_tEffectDesc.bBillboard)
	{
		_matrix Temp = XMLoadFloat4x4(&WorldMatrix);
		_vector vPosition = Temp.r[CTransform::STATE_POSITION];
		_vector vCamPosition = XMLoadFloat4(&GI->Get_CamPosition());

		_float fLookScale = XMVectorGetX(XMVector3Length(Temp.r[CTransform::STATE_LOOK]));
		_vector vLook = XMVectorSetW(XMVector3Normalize(vPosition - vCamPosition), 0.f) * fLookScale;

		_float fRightScale = XMVectorGetX(XMVector3Length(Temp.r[CTransform::STATE_RIGHT]));
		_vector vRight = XMVectorSetW(XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)), 0.f) * fRightScale;

		_float fUpScale = XMVectorGetX(XMVector3Length(Temp.r[CTransform::STATE_UP]));
		_vector vUp = XMVectorSetW(XMVector3Normalize(XMVector3Cross(vLook, vRight)), 0.f) * fUpScale;

		Temp.r[CTransform::STATE_RIGHT] = vRight;
		Temp.r[CTransform::STATE_UP] = vUp;
		Temp.r[CTransform::STATE_LOOK] = vLook;

		XMStoreFloat4x4(&WorldMatrix, Temp);
	}
	


	m_pRigidBodyCom->Update_RigidBody(fTimeDelta);

	CRenderer::EFFECT_INSTANCE_DESC EffectInstanceDesc;
	ZeroMemory(&EffectInstanceDesc, sizeof CRenderer::EFFECT_INSTANCE_DESC);

	EffectInstanceDesc.g_fMaxCountX = m_tEffectDesc.fMaxCount.x;
	EffectInstanceDesc.g_fMaxCountY = m_tEffectDesc.fMaxCount.y;
	EffectInstanceDesc.g_fAlpha = m_tEffectDesc.fAlpha;
	EffectInstanceDesc.g_fUVIndex = m_tEffectDesc.vUVIndex;
	EffectInstanceDesc.g_fUVFlow = m_tEffectDesc.fAccUVFlow;

	EffectInstanceDesc.g_fAdditiveDiffuseColor = m_tEffectDesc.vAdditiveDiffuseColor;
	EffectInstanceDesc.g_vBloomPower = _float4(m_tEffectDesc.vBloomPower.x, m_tEffectDesc.vBloomPower.y, m_tEffectDesc.vBloomPower.z, 0.f);
	EffectInstanceDesc.g_iCutUV = m_tEffectDesc.bCutUV;



	if (true == GI->Intersect_Frustum_World(XMLoadFloat4x4(&WorldMatrix).r[CTransform::STATE_POSITION], 3.f))
	{
		if (m_tEffectDesc.eType == EFFECT_TYPE::EFFECT_MESH)
			m_pRendererCom->Add_RenderGroup_Instancing_Effect(CRenderer::RENDER_EFFECT, CRenderer::INSTANCING_SHADER_TYPE::EFFECT_MODEL, this, WorldMatrix, EffectInstanceDesc);
		else
			m_pRendererCom->Add_RenderGroup_Instancing_Effect(CRenderer::RENDER_EFFECT, CRenderer::INSTANCING_SHADER_TYPE::EFFECT_TEXTURE, this, WorldMatrix, EffectInstanceDesc);
	}
	
}


HRESULT CEffect::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (m_bDead && GI->Get_CurrentLevel() != LEVEL_TOOL)
		return S_OK;

	if (FAILED(Bind_ShaderResource_Instance(pInstancingShader)))
		return E_FAIL;

	if (m_tEffectDesc.eType == EFFECT_TYPE::EFFECT_MESH)
	{
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

void CEffect::Set_EffectDesc(const EFFECT_DESC& tDesc)
{
	m_tEffectDesc = tDesc;
	Set_Texture_Diffuse();
	Set_Texture_Alpha();
}

void CEffect::Set_Gravity(_bool bGravity)
{

}

void CEffect::Set_MoveDir(_vector vDir)
{
	XMStoreFloat3(&m_tEffectDesc.vMoveDir, XMVector3Normalize(vDir));
}

void CEffect::Reset_Effect()
{


}

void CEffect::Reset_UV()
{
	m_bEnd = false;

	m_fAccIndex = 0.f;
	m_tEffectDesc.vUVIndex   = { 0.f, 0.f };
	m_tEffectDesc.fAccUVFlow = { 0.f, 0.f };
	m_tEffectDesc.vUVFlow    = { 0.f, 0.f };
}

void CEffect::Increment(_float fTimeDelta)
{
	if (m_bEnd == true && m_tEffectDesc.bLoop == false)
		return;

	m_fAccIndex += m_tEffectDesc.fIndexSpeed * fTimeDelta;
	if (m_fAccIndex >= 1.f)
	{
		m_fAccIndex = 0.f;
		m_tEffectDesc.vUVIndex.x++;
		if (m_tEffectDesc.fMaxCount.x <= m_tEffectDesc.vUVIndex.x)
		{
			m_tEffectDesc.vUVIndex.x = 0;
			m_tEffectDesc.vUVIndex.y++;
			if (m_tEffectDesc.fMaxCount.y <= m_tEffectDesc.vUVIndex.y)
			{
				m_tEffectDesc.vUVIndex.y = 0;
				if (m_tEffectDesc.bLoop == false)
					m_bEnd = true;
				else
					m_bEnd = false;
			}
		}
	}


}

void CEffect::Decrement(_float fTimeDelta)
{
	if (m_bEnd == true && m_tEffectDesc.bLoop == false)
		return;

	m_fAccIndex += m_tEffectDesc.fIndexSpeed * fTimeDelta;

	if (m_fAccIndex >= 1.f)
	{
		m_fAccIndex = 0.f;
		m_tEffectDesc.vUVIndex.x--;
		if (0 > m_tEffectDesc.vUVIndex.x)
		{
			m_tEffectDesc.vUVIndex.x = m_tEffectDesc.fMaxCount.x;
			m_tEffectDesc.vUVIndex.y--;
			if (0 > m_tEffectDesc.vUVIndex.y)
			{
				m_tEffectDesc.vUVIndex.y = m_tEffectDesc.fMaxCount.y;
				if (m_tEffectDesc.bLoop == false)
					m_bEnd = true;
				else
					m_bEnd = false;
			}
		}
	}
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

	if (m_pDiffuseTextureCom != nullptr && m_tEffectDesc.iTextureIndexDiffuse > m_pDiffuseTextureCom->Get_TextureCount())
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

	if (m_pAlphaTextureCom != nullptr && m_tEffectDesc.iTextureIndexAlpha > m_pAlphaTextureCom->Get_TextureCount())
		m_tEffectDesc.iTextureIndexAlpha = m_pAlphaTextureCom->Get_TextureCount() - 1;
}

HRESULT CEffect::Bind_ShaderResource_Instance(CShader* pShader)
{
	if (FAILED(pShader->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(pShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;


	if (FAILED(pShader->Bind_RawValue("g_fBlurPower", &m_tEffectDesc.fBlurPower, sizeof(_float))))
		return E_FAIL;

	if (m_pDiffuseTextureCom != nullptr && -1 < m_tEffectDesc.iTextureIndexDiffuse)
	{
		if (FAILED(m_pDiffuseTextureCom->Bind_ShaderResource(pShader, "g_DiffuseTexture", m_tEffectDesc.iTextureIndexDiffuse)))
			return E_FAIL;
	}

	if (m_pAlphaTextureCom  != nullptr && -1 < m_tEffectDesc.iTextureIndexAlpha)
	{
		if (FAILED(m_pAlphaTextureCom->Bind_ShaderResource(pShader, "g_AlphaTexture", m_tEffectDesc.iTextureIndexAlpha)))
			return E_FAIL;
	}

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

	return S_OK;
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
	if(m_tEffectDesc.eType == EFFECT_TYPE::EFFECT_MESH)
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, m_tEffectDesc.strModelName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
	}
	/* For.Com_VIBuffer */
	else
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
			return E_FAIL;
	}

	return S_OK;
}


CEffect* CEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, 
	const wstring& strObjectTag, const EFFECT_DESC* tEffectDesc, const wstring& strEffectFilePath)
{
	CEffect* pInstance = new CEffect(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype(tEffectDesc)))
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





