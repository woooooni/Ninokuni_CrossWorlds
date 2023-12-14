#include "stdafx.h"
#include "Particle.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "Utils.h"
#include "VIBuffer_Particle.h"
#include "filesystem"
#include "FileUtils.h"
#include "Utils.h"

CParticle::CParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_PARTICLE)
{
}

CParticle::CParticle(const CParticle& rhs)
	: CGameObject(rhs)
	, m_tParticleDesc(rhs.m_tParticleDesc)
{
}

void CParticle::Set_ParticleDesc(const PARTICLE_DESC& tDesc)
{
	m_tParticleDesc = tDesc;
	 
	// 텍스처 셋팅
	Set_Texture_Diffuse();
	Set_Texture_Alpha();

	// 버퍼 정보 셋팅
	if (m_pVIBufferCom != nullptr)
		m_pVIBufferCom->Set_ParticleBufferDesc(*static_cast<CVIBuffer_Particle::PARTICLE_BUFFER_DESC*>(Get_ParticleBufferInfo()));
}

HRESULT CParticle::Initialize_Prototype(const PARTICLE_DESC* pParticleDesc, const wstring& strParticleFilePath)
{
	if(pParticleDesc != nullptr)
		m_tParticleDesc = *pParticleDesc;
	else
		Load_ParticleData(strParticleFilePath);

	return S_OK;
}

HRESULT CParticle::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CParticle::Tick(_float fTimeDelta)
{
	if(m_pVIBufferCom->Get_Finished())
		Set_Dead(true);
	else
		m_pVIBufferCom->Tick(fTimeDelta);
}

void CParticle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT, this);
}

HRESULT CParticle::Render()
{
	if (FAILED(Bind_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render(m_tParticleDesc.iNumEffectCount);

	return S_OK;
}

HRESULT CParticle::Bind_ShaderResource()
{
	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	// 이펙트 정보
	if (FAILED(m_pShaderCom->Bind_RawValue("g_EffectDesc", m_pVIBufferCom->Get_ParticleShaderInfo().data(), sizeof(CVIBuffer_Particle::PARTICLE_SHADER_DESC) * m_pVIBufferCom->Get_ParticleShaderInfo().size())))
		return E_FAIL;

	// 텍스처
	if (m_pDiffuseTextureCom != nullptr)
	{
		if (FAILED(m_pDiffuseTextureCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}
	if (m_pAlphaTextureCom != nullptr)
	{
		if (FAILED(m_pAlphaTextureCom->Bind_ShaderResources(m_pShaderCom, "g_AlphaTexture")))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CParticle::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Point_Instance"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Texture */
	Set_Texture_Diffuse();
	Set_Texture_Alpha();

	/* For.Com_VIBuffer */
	CVIBuffer_Particle::PARTICLE_BUFFER_DESC pBufferInfo = *static_cast<CVIBuffer_Particle::PARTICLE_BUFFER_DESC*>(Get_ParticleBufferInfo());
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, &pBufferInfo)))
			return E_FAIL;

	return S_OK;
}

void CParticle::Load_ParticleData(const wstring& strFileName)
{
	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strFileName, FileMode::Read);

	// 반복 여부
	File->Read<_bool>(m_tParticleDesc.bLoop);

	// 파티클 개수
	File->Read<_uint>(m_tParticleDesc.iNumEffectCount);

	// 분포 범위
	File->Read<_float3>(m_tParticleDesc.fRange);

	// 크기
	File->Read<_bool>(m_tParticleDesc.bSameRate);
	File->Read<_float2>(m_tParticleDesc.fScale);

	// 지속 시간
	File->Read<_float2>(m_tParticleDesc.fLifeTime);

	// 속도
	File->Read<_float2>(m_tParticleDesc.fSpeed);

	// 움직임
	File->Read<_float3>(m_tParticleDesc.vVelocityMin);
	File->Read<_float3>(m_tParticleDesc.vVelocityMax);

	// 박스 범위
	File->Read<_bool>(m_tParticleDesc.bUseBox);
	File->Read<_float3>(m_tParticleDesc.fBoxMin);
	File->Read<_float3>(m_tParticleDesc.fBoxMax);

	// 색상
	File->Read<_bool>(m_tParticleDesc.bRandomColor);
	File->Read<_float4>(m_tParticleDesc.vDiffuseColor);

	// 텍스처
	File->Read<_bool>(m_tParticleDesc.bAnimation);
	File->Read<_bool>(m_tParticleDesc.bAnimationLoop);
	File->Read<_bool>(m_tParticleDesc.bRandomStartIndex);
	File->Read<_float2>(m_tParticleDesc.fUVIndex);
	File->Read<_float2>(m_tParticleDesc.fUVMaxCount);
	File->Read<_float2>(m_tParticleDesc.fAnimationSpeed);

	m_tParticleDesc.strDiffuseTetextureName = CUtils::ToWString(File->Read<string>());
	m_tParticleDesc.strAlphaTexturName = CUtils::ToWString(File->Read<string>());
	m_tParticleDesc.strDiffuseTetexturePath = CUtils::ToWString(File->Read<string>());
	m_tParticleDesc.strAlphaTexturPath = CUtils::ToWString(File->Read<string>());
}

void* CParticle::Get_ParticleBufferInfo()
{
	CVIBuffer_Particle::PARTICLE_BUFFER_DESC tBufferInfo = {};

	tBufferInfo.bLoop     = m_tParticleDesc.bLoop;
	tBufferInfo.bSameRate = m_tParticleDesc.bSameRate;
	tBufferInfo.fScale = m_tParticleDesc.fScale;
	tBufferInfo.fRange = m_tParticleDesc.fRange;
	tBufferInfo.vVelocityMin = m_tParticleDesc.vVelocityMin;
	tBufferInfo.vVelocityMax = m_tParticleDesc.vVelocityMax;
	tBufferInfo.fSpeed    = m_tParticleDesc.fSpeed;
	tBufferInfo.fLifeTime = m_tParticleDesc.fLifeTime;
	tBufferInfo.bUseBox = m_tParticleDesc.bUseBox;
	tBufferInfo.fBoxMin = m_tParticleDesc.fBoxMin;
	tBufferInfo.fBoxMax = m_tParticleDesc.fBoxMax;
	tBufferInfo.bRandomColor  = m_tParticleDesc.bRandomColor;
	tBufferInfo.vDiffuseColor = m_tParticleDesc.vDiffuseColor;
	tBufferInfo.bAnimation      = m_tParticleDesc.bAnimation;
	tBufferInfo.bAnimationLoop  = m_tParticleDesc.bAnimationLoop;
	tBufferInfo.fAnimationSpeed = m_tParticleDesc.fAnimationSpeed;
	tBufferInfo.bRandomStartIndex    = m_tParticleDesc.bRandomStartIndex;
	tBufferInfo.fUVIndex    = m_tParticleDesc.fUVIndex;
	tBufferInfo.fUVMaxCount = m_tParticleDesc.fUVMaxCount;

	//if (m_pDiffuseTextureCom != nullptr)
	//	tBufferInfo.fDiffuseTextureIndexMax = (_float)m_pDiffuseTextureCom->Get_TextureCount();

	return &tBufferInfo;
}

void CParticle::Set_Texture_Diffuse()
{
	_int  iBufferSizeName = WideCharToMultiByte(CP_UTF8, 0, m_tParticleDesc.strDiffuseTetextureName.c_str(), -1, nullptr, 0, nullptr, nullptr);
	char* pszFileName     = new char[iBufferSizeName];
	WideCharToMultiByte(CP_UTF8, 0, m_tParticleDesc.strDiffuseTetextureName.c_str(), -1, pszFileName, iBufferSizeName, nullptr, nullptr);
	if (strcmp(pszFileName, "") != 0)
	{
		if (m_pDiffuseTextureCom != nullptr)
			Safe_Release(m_pDiffuseTextureCom);

		m_pDiffuseTextureCom = static_cast<CTexture*>(GI->Clone_Component(LEVEL_STATIC, m_tParticleDesc.strDiffuseTetextureName));
	}
	else
	{
		_int  iBufferSizePath = WideCharToMultiByte(CP_UTF8, 0, m_tParticleDesc.strDiffuseTetexturePath.c_str(), -1, nullptr, 0, nullptr, nullptr);
		char* pszFilePath     = new char[iBufferSizePath];
		WideCharToMultiByte(CP_UTF8, 0, m_tParticleDesc.strDiffuseTetexturePath.c_str(), -1, pszFilePath, iBufferSizePath, nullptr, nullptr);
		if (strcmp(pszFilePath, "") != 0)
		{
			if (m_pDiffuseTextureCom != nullptr)
				Safe_Release(m_pDiffuseTextureCom);

			char szFileName[MAX_PATH] = ""; // 파일이름
			_splitpath_s(pszFilePath, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, nullptr, 0);
			if (strcmp(szFileName, "") == 0)
				m_pDiffuseTextureCom = CTexture::Create(m_pDevice, m_pContext, m_tParticleDesc.strDiffuseTetexturePath, 0, true);
			else
				m_pDiffuseTextureCom = CTexture::Create(m_pDevice, m_pContext, m_tParticleDesc.strDiffuseTetexturePath);
		}
		Safe_Delete(pszFilePath);
	}
	Safe_Delete(pszFileName);
}

void CParticle::Set_Texture_Alpha()
{
	_int  iBufferSizeName = WideCharToMultiByte(CP_UTF8, 0, m_tParticleDesc.strAlphaTexturName.c_str(), -1, nullptr, 0, nullptr, nullptr);
	char* pszFileName = new char[iBufferSizeName];
	WideCharToMultiByte(CP_UTF8, 0, m_tParticleDesc.strAlphaTexturName.c_str(), -1, pszFileName, iBufferSizeName, nullptr, nullptr);
	if (strcmp(pszFileName, "") != 0)
	{
		if (m_pAlphaTextureCom != nullptr)
			Safe_Release(m_pAlphaTextureCom);

		m_pAlphaTextureCom = static_cast<CTexture*>(GI->Clone_Component(LEVEL_STATIC, m_tParticleDesc.strAlphaTexturName));
	}
	else
	{
		_int  iBufferSizePath = WideCharToMultiByte(CP_UTF8, 0, m_tParticleDesc.strAlphaTexturPath.c_str(), -1, nullptr, 0, nullptr, nullptr);
		char* pszFilePath = new char[iBufferSizePath];
		WideCharToMultiByte(CP_UTF8, 0, m_tParticleDesc.strAlphaTexturPath.c_str(), -1, pszFilePath, iBufferSizePath, nullptr, nullptr);
		if (strcmp(pszFilePath, "") != 0)
		{
			if (m_pAlphaTextureCom != nullptr)
				Safe_Release(m_pAlphaTextureCom);

			char szFileName[MAX_PATH] = ""; // 파일이름
			_splitpath_s(pszFilePath, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, nullptr, 0);
			if (strcmp(szFileName, "") == 0)
				m_pAlphaTextureCom = CTexture::Create(m_pDevice, m_pContext, m_tParticleDesc.strAlphaTexturPath, 0, true);
			else
				m_pAlphaTextureCom = CTexture::Create(m_pDevice, m_pContext, m_tParticleDesc.strAlphaTexturPath);
		}
		Safe_Delete(pszFilePath);
	}
	Safe_Delete(pszFileName);
}

CParticle* CParticle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const PARTICLE_DESC* pParticleDesc, const wstring& strParticleFilePath)
{
	CParticle* pInstance = new CParticle(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype(pParticleDesc, strParticleFilePath)))
	{
		MSG_BOX("Failed to Created : CParticle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CParticle::Clone(void* pArg)
{
	CParticle* pInstance = new CParticle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CParticle");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CParticle::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pDiffuseTextureCom);
	Safe_Release(m_pAlphaTextureCom);
}