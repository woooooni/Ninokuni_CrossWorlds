#include "stdafx.h"
#include "Tool_Particle.h"
#include "imgui.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "filesystem"
#include "FileUtils.h"
#include "Utils.h"

CTool_Particle::CTool_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CTool(pDevice, pContext)
{
}

HRESULT CTool_Particle::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CTool_Particle::Tick(_float fTimeDelta)
{
	ImGui::Begin("Particle_Tool");

	// 생성
	if (ImGui::Button("Create"))
		Create_Particle();
	ImGui::SameLine();
	// 삭제
	if (ImGui::Button("Delete"))
	{
		if (m_pParticle != nullptr)
		{
			m_pParticle->Set_Dead(true);
			m_pParticle = nullptr;

			// 초기화
			m_fPosition[0] = 0.f;
			m_fPosition[1] = 0.f;
			m_fPosition[2] = 0.f;

			m_fRotation[0] = 0.f;
			m_fRotation[1] = 0.f;
			m_fRotation[2] = 0.f;

			m_fScale[0] = 0.f;
			m_fScale[1] = 0.f;
			m_fScale[2] = 0.f;
		}
	}

	// 트랜스폼 정보
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::Text("Position");
		ImGui::InputFloat3("##Position", m_fPosition);

		ImGui::Text("Rotation");
		ImGui::InputFloat3("##Rotation", m_fRotation);

		ImGui::Text("Scale");
		ImGui::InputFloat3("##Scale", m_fScale);

		if (ImGui::Button("Select_Transform"))
		{
			if (m_pParticle != nullptr)
			{
				CTransform* pTransform = m_pParticle->Get_Component<CTransform>(L"Com_Transform");

				pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fPosition[0], m_fPosition[1], m_fPosition[2], 1.f));
				pTransform->FixRotation(m_fRotation[0], m_fRotation[1], m_fRotation[2]);
				pTransform->Set_Scale(_float3(m_fScale[0], m_fScale[1], m_fScale[2]));
			}
		}
	}

	// 파티클 정보
	if (ImGui::CollapsingHeader("ParticleSystem"))
	{
		// 반복 여부
		ImGui::Checkbox("LoopParticles", &m_tParticleInfo.bLoop);
		ImGui::NewLine();

		// 파티클 개수
		ImGui::Text("MaxParticles");
		ImGui::InputInt("##MaxParticles", &(_int)m_tParticleInfo.iNumEffectCount);
		ImGui::NewLine();

		// 분포 범위
		ImGui::Text("RangeParticles");
		ImGui::InputFloat3("##RangeParticles", m_fParticleRange);
		ImGui::NewLine();

		// 시작 크기
		ImGui::Checkbox("SameRate", &m_tParticleInfo.bSameRate);
		ImGui::Text("SizeParticles");
		ImGui::InputFloat2("##SizeParticles", m_fParticleScale);
		ImGui::NewLine();

		// 지속 시간
		if (ImGui::CollapsingHeader("ParticleTime"))
		{
			ImGui::Text("LifetimeParticles");
			ImGui::InputFloat2("##LifetimeParticles", m_fParticleLifeTime);
			ImGui::NewLine();
		}

		// 이동(힘)
		if (ImGui::CollapsingHeader("ParticleVelocity"))
		{
			// 이동 속도
			ImGui::Text("SpeedParticles");
			ImGui::InputFloat2("##SpeedParticles", m_fParticleSpeed);
			ImGui::NewLine();

			// 움직임
			ImGui::Text("VelocityMinParticles");
			ImGui::InputFloat3("##VelocityMinParticles", m_fParticleVelocityMin);
			ImGui::Text("VelocityMaxParticles");
			ImGui::InputFloat3("##VelocityMaxParticles", m_fParticleVelocityMax);
			ImGui::NewLine();
		}

		// 박스 범위
		if (ImGui::CollapsingHeader("ParticleBox"))
		{
			ImGui::Checkbox("UseBox", &m_tParticleInfo.bUseBox);
			if (m_tParticleInfo.bUseBox)
			{
				ImGui::Text("BoxMinParticles");
				ImGui::InputFloat3("##BoxMinParticles", m_fParticleBoxMin);
				ImGui::Text("BoxMaxParticles");
				ImGui::InputFloat3("##BoxMaxParticles", m_fParticleBoxMax);
				ImGui::NewLine();
			}
		}

		// 파티클 색상
		if (ImGui::CollapsingHeader("ParticleColor"))
		{
			ImGui::Checkbox("RandomColor", &m_tParticleInfo.bRandomColor);
			if (!m_tParticleInfo.bRandomColor)
			{
				ImGui::Text("Diffuse Color");
				ImGui::ColorEdit4("##DiffuseColor", (float*)&m_tParticleInfo.vDiffuseColor, ImGuiColorEditFlags_Float);
			}
		}

		// 텍스처 지정
		if (ImGui::CollapsingHeader("ParticleTexture"))
		{
			// 디퓨즈 텍스처 지정
			ImGui::Text("DiffuseTextureName :");
			ImGui::SameLine();
			ImGui::InputText("DiffuseTextureName", m_cDiffuseTextureName, IM_ARRAYSIZE(m_cDiffuseTextureName));
			ImGui::Text("DiffuseTexturePath :");
			ImGui::SameLine();
			ImGui::InputText("DiffuseTexturePath", m_cDiffuseTexturePath, IM_ARRAYSIZE(m_cDiffuseTexturePath));
			ImGui::NewLine();

			// 알파 텍스처 지정
			ImGui::Text("AlphaTextureName :");
			ImGui::SameLine();
			ImGui::InputText("AlphaTextureName", m_cAlphaTextureName, IM_ARRAYSIZE(m_cAlphaTextureName));
			ImGui::Text("AlphaTexturePath :");
			ImGui::SameLine();
			ImGui::InputText("AlphaTexturePath", m_cAlphaTexturePath, IM_ARRAYSIZE(m_cAlphaTexturePath));

			// 디퓨즈 텍스처 시작 텍스처 인덱스
			ImGui::Checkbox("RandomStartTextureIndex", &m_tParticleInfo.bRandomStartIndex);
			if (!m_tParticleInfo.bRandomStartIndex)
			{
				ImGui::Text("ParticleUVIndex");
				ImGui::InputFloat2("##ParticleUVIndex", m_fParticleUVIndex);
				ImGui::NewLine();
			}
			ImGui::Text("ParticleUVMaxCount");
			ImGui::InputFloat2("##ParticleUVMaxCount", m_fParticleUVMaxCount);
			ImGui::NewLine();
		}

		// 텍스처 애니메이션
		if (ImGui::CollapsingHeader("ParticleAnimation"))
		{
			ImGui::Checkbox("UseAnimation", &m_tParticleInfo.bAnimation);
			if (m_tParticleInfo.bAnimation)
			{
				ImGui::Checkbox("LoopAnimation", &m_tParticleInfo.bAnimationLoop);

				ImGui::Text("SpeedAnimation");
				ImGui::InputFloat2("##SpeedAnimation", m_fParticleAnimationSpeed);
			}
		}

	    // Gravity Modifier 중력 효과

		if (ImGui::Button("Select_ParticleSystem"))
		{
			if (m_pParticle != nullptr)
			{
				m_tParticleInfo.fScale = _float2(m_fParticleScale[0], m_fParticleScale[1]);
				m_tParticleInfo.fRange = _float3(m_fParticleRange[0], m_fParticleRange[1], m_fParticleRange[2]);
				m_tParticleInfo.fLifeTime = _float2(m_fParticleLifeTime[0], m_fParticleLifeTime[1]);
				m_tParticleInfo.fSpeed = _float2(m_fParticleSpeed[0], m_fParticleSpeed[1]);
				m_tParticleInfo.fBoxMin = _float3(m_fParticleBoxMin[0], m_fParticleBoxMin[1], m_fParticleBoxMin[2]);
				m_tParticleInfo.fBoxMax = _float3(m_fParticleBoxMax[0], m_fParticleBoxMax[1], m_fParticleBoxMax[2]);
				m_tParticleInfo.vVelocityMin = _float3(m_fParticleVelocityMin[0], m_fParticleVelocityMin[1], m_fParticleVelocityMin[2]);
				m_tParticleInfo.vVelocityMax = _float3(m_fParticleVelocityMax[0], m_fParticleVelocityMax[1], m_fParticleVelocityMax[2]);
				m_tParticleInfo.fAnimationSpeed = _float2(m_fParticleAnimationSpeed[0], m_fParticleAnimationSpeed[1]);

				m_tParticleInfo.fUVIndex = _float2(m_fParticleUVIndex[0], m_fParticleUVIndex[1]);
				m_tParticleInfo.fUVMaxCount = _float2(m_fParticleUVMaxCount[0], m_fParticleUVMaxCount[1]);

				wstring strDiffuseTextureName(m_cDiffuseTextureName, m_cDiffuseTextureName + strlen(m_cDiffuseTextureName));
				m_tParticleInfo.strDiffuseTetextureName = strDiffuseTextureName;
				wstring strDiffuseTexturePath(m_cDiffuseTexturePath, m_cDiffuseTexturePath + strlen(m_cDiffuseTexturePath));
				m_tParticleInfo.strDiffuseTetexturePath = strDiffuseTexturePath;
				wstring strAlphaTextureName(m_cAlphaTextureName, m_cAlphaTextureName + strlen(m_cAlphaTextureName));
				m_tParticleInfo.strAlphaTexturName = strAlphaTextureName;
				wstring strAlphaTexturePath(m_cAlphaTexturePath, m_cAlphaTexturePath + strlen(m_cAlphaTexturePath));
				m_tParticleInfo.strAlphaTexturPath = strAlphaTexturePath;

				static_cast<CParticle*>(m_pParticle)->Set_ParticleDesc(m_tParticleInfo);
			}
		}
	}

	if (ImGui::CollapsingHeader("ParticleSaveAndLoad"))
	{
		ImGui::Text("SaveAndLoadName :");
		ImGui::SameLine();
		ImGui::InputText("SaveAndLoadName", m_cSaveAndLoadName, IM_ARRAYSIZE(m_cSaveAndLoadName));

		// 저장하기/ 불러오기
		if (ImGui::Button("Save"))
			Save_Particle(m_cSaveAndLoadName);
		ImGui::SameLine();
		if (ImGui::Button("Load"))
			Load_Particle(m_cSaveAndLoadName);
	}

	ImGui::End();
}

void CTool_Particle::Create_Particle()
{
	if (m_pParticle == nullptr)
	{
		// 생성
		m_pParticle = GI->Clone_GameObject(TEXT("Prototype_GameObject_Particle"), LAYER_TYPE::LAYER_EFFECT);
		GI->Add_GameObject(LEVEL_TOOL, LAYER_TYPE::LAYER_EFFECT, m_pParticle);

		// 기본 값 초기화
		CTransform* pTransform = m_pParticle->Get_Component<CTransform>(L"Com_Transform");
		_vector vPosition = pTransform->Get_State(CTransform::STATE_POSITION);
		m_fPosition[0] = XMVectorGetX(vPosition);
		m_fPosition[1] = XMVectorGetY(vPosition);
		m_fPosition[2] = XMVectorGetZ(vPosition);

		_vector vRotation = pTransform->Get_WorldRotation();
		m_fRotation[0] = XMVectorGetX(vRotation);
		m_fRotation[1] = XMVectorGetY(vRotation);
		m_fRotation[2] = XMVectorGetZ(vRotation);

		_float3 fScale = pTransform->Get_Scale();
		m_fScale[0] = fScale.x;
		m_fScale[1] = fScale.y;
		m_fScale[2] = fScale.z;

		m_tParticleInfo = static_cast<CParticle*>(m_pParticle)->Get_ParticleDesc();

		m_fParticleScale[0] = m_tParticleInfo.fScale.x;
		m_fParticleScale[1] = m_tParticleInfo.fScale.y;

		m_fParticleRange[0] = m_tParticleInfo.fRange.x;
		m_fParticleRange[1] = m_tParticleInfo.fRange.y;
		m_fParticleRange[2] = m_tParticleInfo.fRange.z;

		m_fParticleLifeTime[0] = m_tParticleInfo.fLifeTime.x;
		m_fParticleLifeTime[1] = m_tParticleInfo.fLifeTime.y;

		m_fParticleSpeed[0] = m_tParticleInfo.fSpeed.x;
		m_fParticleSpeed[1] = m_tParticleInfo.fSpeed.y;

		m_fParticleBoxMin[0] = m_tParticleInfo.fBoxMin.x;
		m_fParticleBoxMin[1] = m_tParticleInfo.fBoxMin.y;
		m_fParticleBoxMin[2] = m_tParticleInfo.fBoxMin.z;

		m_fParticleBoxMax[0] = m_tParticleInfo.fBoxMax.x;
		m_fParticleBoxMax[1] = m_tParticleInfo.fBoxMax.y;
		m_fParticleBoxMax[2] = m_tParticleInfo.fBoxMax.z;

		m_fParticleVelocityMin[0] = m_tParticleInfo.vVelocityMin.x;
		m_fParticleVelocityMin[1] = m_tParticleInfo.vVelocityMin.y;
		m_fParticleVelocityMin[2] = m_tParticleInfo.vVelocityMin.z;

		m_fParticleVelocityMax[0] = m_tParticleInfo.vVelocityMax.x;
		m_fParticleVelocityMax[1] = m_tParticleInfo.vVelocityMax.y;
		m_fParticleVelocityMax[2] = m_tParticleInfo.vVelocityMax.z;

		m_fParticleAnimationSpeed[0] = m_tParticleInfo.fAnimationSpeed.x;
		m_fParticleAnimationSpeed[1] = m_tParticleInfo.fAnimationSpeed.y;

		m_fParticleUVIndex[0] = m_tParticleInfo.fUVIndex.x;
		m_fParticleUVIndex[1] = m_tParticleInfo.fUVIndex.y;

		m_fParticleUVMaxCount[0] = m_tParticleInfo.fUVMaxCount.x;
		m_fParticleUVMaxCount[1] = m_tParticleInfo.fUVMaxCount.y;
	}
}

void CTool_Particle::Save_Particle(const char* pFileName)
{
	if (m_pParticle == nullptr)
	{
		MSG_BOX("Particle_Save_Failed!");
		return;
	}

	wstring strFileName(pFileName, pFileName + strlen(pFileName));
	wstring strFilePath = L"../Bin/DataFiles/Particle/" + strFileName + L".Particle";

	auto path = filesystem::path(strFilePath);
	filesystem::create_directories(path.parent_path());
	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strFilePath, FileMode::Write);

	// 파티클 정보 저장
	m_tParticleInfo = static_cast<CParticle*>(m_pParticle)->Get_ParticleDesc();

	// 반복 여부
	File->Write<_bool>(m_tParticleInfo.bLoop);

	// 파티클 개수
	File->Write<_uint>(m_tParticleInfo.iNumEffectCount);

	// 분포 범위
	File->Write<_float3>(m_tParticleInfo.fRange);

	// 크기
	File->Write<_bool>(m_tParticleInfo.bSameRate);
	File->Write<_float2>(m_tParticleInfo.fScale);

	// 지속 시간
	File->Write<_float2>(m_tParticleInfo.fLifeTime);

	// 속도
	File->Write<_float2>(m_tParticleInfo.fSpeed);

	// 움직임
	File->Write<_float3>(m_tParticleInfo.vVelocityMin);
	File->Write<_float3>(m_tParticleInfo.vVelocityMax);

	// 박스 범위
	File->Write<_bool>(m_tParticleInfo.bUseBox);
	File->Write<_float3>(m_tParticleInfo.fBoxMin);
	File->Write<_float3>(m_tParticleInfo.fBoxMax);

	// 색상
	File->Write<_bool>(m_tParticleInfo.bRandomColor);
	File->Write<_float4>(m_tParticleInfo.vDiffuseColor);

	// 텍스처
	File->Write<_bool>(m_tParticleInfo.bAnimation);
	File->Write<_bool>(m_tParticleInfo.bAnimationLoop);
	File->Write<_bool>(m_tParticleInfo.bRandomStartIndex);
	File->Write<_float2>(m_tParticleInfo.fUVIndex);
	File->Write<_float2>(m_tParticleInfo.fUVMaxCount);
	File->Write<_float2>(m_tParticleInfo.fAnimationSpeed);

	File->Write<string>(CUtils::ToString(m_tParticleInfo.strDiffuseTetextureName));
	File->Write<string>(CUtils::ToString(m_tParticleInfo.strAlphaTexturName));
	File->Write<string>(CUtils::ToString(m_tParticleInfo.strDiffuseTetexturePath));
	File->Write<string>(CUtils::ToString(m_tParticleInfo.strAlphaTexturPath));

	MSG_BOX("Particle_Save_Success!");
}

void CTool_Particle::Load_Particle(const char* pFileName)
{
	if (m_pParticle == nullptr)
		Create_Particle();

	wstring strFileName(pFileName, pFileName + strlen(pFileName));
	wstring strFilePath = L"../Bin/DataFiles/Particle/" + strFileName + L".Particle";

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strFilePath, FileMode::Read);

	CParticle::PARTICLE_DESC ParticleInfo = {};

	// 반복 여부
	File->Read<_bool>(ParticleInfo.bLoop);

	// 파티클 개수
	File->Read<_uint>(ParticleInfo.iNumEffectCount);

	// 분포 범위
	File->Read<_float3>(ParticleInfo.fRange);

	// 크기
	File->Read<_bool>(ParticleInfo.bSameRate);
	File->Read<_float2>(ParticleInfo.fScale);

	// 지속 시간
	File->Read<_float2>(ParticleInfo.fLifeTime);

	// 속도
	File->Read<_float2>(ParticleInfo.fSpeed);

	// 움직임
	File->Read<_float3>(ParticleInfo.vVelocityMin);
	File->Read<_float3>(ParticleInfo.vVelocityMax);

	// 박스 범위
	File->Read<_bool>(ParticleInfo.bUseBox);
	File->Read<_float3>(ParticleInfo.fBoxMin);
	File->Read<_float3>(ParticleInfo.fBoxMax);

	// 색상
	File->Read<_bool>(ParticleInfo.bRandomColor);
	File->Read<_float4>(ParticleInfo.vDiffuseColor);

	// 텍스처
	File->Read<_bool>(ParticleInfo.bAnimation);
	File->Read<_bool>(ParticleInfo.bAnimationLoop);
	File->Read<_bool>(ParticleInfo.bRandomStartIndex);
	File->Read<_float2>(ParticleInfo.fUVIndex);
	File->Read<_float2>(ParticleInfo.fUVMaxCount);
	File->Read<_float2>(ParticleInfo.fAnimationSpeed);

	ParticleInfo.strDiffuseTetextureName = CUtils::ToWString(File->Read<string>());
	ParticleInfo.strAlphaTexturName      = CUtils::ToWString(File->Read<string>());
	ParticleInfo.strDiffuseTetexturePath = CUtils::ToWString(File->Read<string>());
	ParticleInfo.strAlphaTexturPath      = CUtils::ToWString(File->Read<string>());

	// 적용
	static_cast<CParticle*>(m_pParticle)->Set_ParticleDesc(ParticleInfo);

	MSG_BOX("Particle_Load_Success!");
}

CTool_Particle* CTool_Particle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Particle* pInstance = new CTool_Particle(pDevice, pContext);
	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed_Create : CTool_Particle");
		return nullptr;
	}
	return pInstance;
}

void CTool_Particle::Free()
{
	__super::Free();
}
