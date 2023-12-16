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

				if (m_bParticleType_Pers)
					static_cast<CParticle*>(m_pParticle)->Set_Position_Perspective(_float3(m_fPosition[0], m_fPosition[1], m_fPosition[2]));
				else if(m_bParticleType_Orth)
					static_cast<CParticle*>(m_pParticle)->Set_Position_Orthographic(_float2(m_fPosition[0], m_fPosition[1]));

				pTransform->FixRotation(m_fRotation[0], m_fRotation[1], m_fRotation[2]);
				pTransform->Set_Scale(_float3(m_fScale[0], m_fScale[1], m_fScale[2]));
			}
		}
	}

	// 파티클 정보
	if (ImGui::CollapsingHeader("ParticleSystem"))
	{
		// 투영 타입
		ImGui::Checkbox("PersParticles", &m_bParticleType_Pers);
		ImGui::NewLine();
		if (m_bParticleType_Pers == true)
			m_bParticleType_Orth = false;

		ImGui::Checkbox("OrthParticles", &m_bParticleType_Orth);
		ImGui::NewLine();
		if (m_bParticleType_Orth == true)
			m_bParticleType_Pers = false;

		// 반복 여부
		ImGui::Checkbox("LoopParticles", &m_tParticleInfo.bParticleLoop);
		ImGui::NewLine();

		// 파티클 개수
		ImGui::Text("MaxParticles");
		ImGui::InputInt("##MaxParticles", &(_int)m_tParticleInfo.iNumEffectCount);
		ImGui::NewLine();

		// 위치 (분포 범위)
		ImGui::Text("RangeParticles");
		ImGui::InputFloat3("##RangeParticles", m_fParticleRange);
		ImGui::NewLine();

		// 크기
		if (ImGui::CollapsingHeader("ParticleScale"))
		{
			ImGui::Checkbox("ScaleSameRate", &m_tParticleInfo.bScaleSameRate);
			ImGui::NewLine();

			ImGui::Text("ScaleStartParticles");
			ImGui::InputFloat2("##ScaleStartParticles", m_fParticleScaleStart);
			ImGui::NewLine();

			ImGui::Checkbox("ScaleChange", &m_tParticleInfo.bScaleChange);
			ImGui::NewLine();

			if (m_tParticleInfo.bScaleChange)
			{
				ImGui::Text("ScaleChangeStartDelayParticles");
				ImGui::InputFloat2("##ScaleChangeStartDelayParticles", m_fParticleScaleChangeStartDelay);
				ImGui::NewLine();

				ImGui::Checkbox("ScaleChangeRandom", &m_tParticleInfo.bScaleChangeRandom);
				ImGui::NewLine();

				if (m_tParticleInfo.bScaleChangeRandom)
				{
					ImGui::Text("ScaleChangeTimeParticles");
					ImGui::InputFloat2("##ScaleChangeTimeParticles", m_fParticleScaleChangeTime);
					ImGui::NewLine();
				}
				else
				{
					ImGui::Checkbox("ScaleAdd", &m_tParticleInfo.bScaleAdd);
					ImGui::NewLine();

					ImGui::Checkbox("ScaleLoop", &m_tParticleInfo.bScaleLoop);
					ImGui::NewLine();

					ImGui::Checkbox("ScaleLoopStart", &m_tParticleInfo.bScaleLoopStart);
					ImGui::NewLine();

					ImGui::Text("ScaleMinParticles");
					ImGui::InputFloat2("##ScaleMinParticles", m_fParticleScaleMin);
					ImGui::NewLine();

					ImGui::Text("ScaleMaxParticles");
					ImGui::InputFloat2("##ScaleMaxParticles", m_fParticleScaleMax);
					ImGui::NewLine();
				}

				ImGui::Text("ScaleSpeedParticles");
				ImGui::InputFloat2("##ScaleSpeedParticles", m_fParticleScaleSpeed);
				ImGui::NewLine();
			}
		}

		// 이동(힘)
		if (ImGui::CollapsingHeader("ParticleVelocity"))
		{
			ImGui::Text("VelocitySpeed");
			ImGui::InputFloat2("##VelocitySpeed", m_fParticleSpeed);
			ImGui::NewLine();

			ImGui::Text("VelocityMinParticles");
			ImGui::InputFloat3("##VelocityMinParticles", m_fParticleVelocityMin);
			ImGui::Text("VelocityMaxParticles");
			ImGui::InputFloat3("##VelocityMaxParticles", m_fParticleVelocityMax);
			ImGui::NewLine();

			ImGui::Checkbox("VelocityChange", &m_tParticleInfo.bVelocityChange);
			ImGui::NewLine();

			if (m_tParticleInfo.bVelocityChange)
			{
				ImGui::Text("VelocityChangeStartDelayParticles");
				ImGui::InputFloat2("##VelocityChangeStartDelayParticles", m_fParticleVelocityChangeStartDelay);
				ImGui::NewLine();

				ImGui::Checkbox("VelocityChangeRandom", &m_tParticleInfo.bVelocityChangeRandom);
				ImGui::NewLine();

				if (m_tParticleInfo.bVelocityChangeRandom)
				{
					ImGui::Text("VelocityChangeTimeParticles");
					ImGui::InputFloat2("##VelocityChangeTimeParticles", m_fParticleVelocityChangeTime);
					ImGui::NewLine();
				}
				else
				{
					ImGui::Checkbox("VelocityLoop", &m_tParticleInfo.bVelocityLoop);
					ImGui::NewLine();

					ImGui::Text("VelocityCountCur");
					ImGui::InputInt("##VelocityCountCur", &(_int)m_tParticleInfo.iVelocityCountCur);
					ImGui::NewLine();

					ImGui::Text("iVelocityCountMax");
					if (ImGui::InputInt("##iVelocityCountMax", &(_int)m_tParticleInfo.iVelocityCountMax))
					{
						if (m_tParticleInfo.iVelocityCountMax == 1)
							m_tParticleInfo.bVelocityChange = false;
						else if (m_tParticleInfo.iVelocityCountMax > 10)
							m_tParticleInfo.bVelocityChange = false;
					}
					ImGui::NewLine();

					if (m_tParticleInfo.iVelocityCountMax > 1)
					{
						ImGui::Text("VelocityMinParticles_01");
						ImGui::InputFloat3("##VelocityMinParticles_01", m_fParticleVelocityMin);

						ImGui::Text("VelocityMaxParticles_01");
						ImGui::InputFloat3("##VelocityMaxParticles_01", m_fParticleVelocityMax);

						ImGui::Text("VelocityChangeTimeParticles_01");
						ImGui::InputFloat2("##VelocityChangeTimeParticles_01", m_fParticleVelocityTime_01);


						ImGui::Text("VelocityMinParticles_02");
						ImGui::InputFloat3("##VelocityMinParticles_02", m_fParticleVelocityMin_02);

						ImGui::Text("VelocityMaxParticles_02");
						ImGui::InputFloat3("##VelocityMaxParticles_02", m_fParticleVelocityMax_02);

						ImGui::Text("VelocityChangeTimeParticles_02");
						ImGui::InputFloat2("##VelocityChangeTimeParticles_02", m_fParticleVelocityTime_02);
					}

					if (m_tParticleInfo.iVelocityCountMax > 2)
					{
						ImGui::Text("VelocityMinParticles_03");
						ImGui::InputFloat3("##VelocityMinParticles_03", m_fParticleVelocityMin_03);

						ImGui::Text("VelocityMaxParticles_03");
						ImGui::InputFloat3("##VelocityMaxParticles_03", m_fParticleVelocityMax_03);

						ImGui::Text("VelocityChangeTimeParticles_03");
						ImGui::InputFloat2("##VelocityChangeTimeParticles_03", m_fParticleVelocityTime_03);
					}

					if (m_tParticleInfo.iVelocityCountMax > 3)
					{
						ImGui::Text("VelocityMinParticles_04");
						ImGui::InputFloat3("##VelocityMinParticles_04", m_fParticleVelocityMin_04);

						ImGui::Text("VelocityMaxParticles_04");
						ImGui::InputFloat3("##VelocityMaxParticles_04", m_fParticleVelocityMax_04);

						ImGui::Text("VelocityChangeTimeParticles_04");
						ImGui::InputFloat2("##VelocityChangeTimeParticles_04", m_fParticleVelocityTime_04);
					}

					if (m_tParticleInfo.iVelocityCountMax > 4)
					{
						ImGui::Text("VelocityMinParticles_05");
						ImGui::InputFloat3("##VelocityMinParticles_05", m_fParticleVelocityMin_05);

						ImGui::Text("VelocityMaxParticles_05");
						ImGui::InputFloat3("##VelocityMaxParticles_05", m_fParticleVelocityMax_05);

						ImGui::Text("VelocityChangeTimeParticles_05");
						ImGui::InputFloat2("##VelocityChangeTimeParticles_05", m_fParticleVelocityTime_05);
					}

					if (m_tParticleInfo.iVelocityCountMax > 5)
					{
						ImGui::Text("VelocityMinParticles_06");
						ImGui::InputFloat3("##VelocityMinParticles_06", m_fParticleVelocityMin_06);

						ImGui::Text("VelocityMaxParticles_06");
						ImGui::InputFloat3("##VelocityMaxParticles_06", m_fParticleVelocityMax_06);

						ImGui::Text("VelocityChangeTimeParticles_06");
						ImGui::InputFloat2("##VelocityChangeTimeParticles_06", m_fParticleVelocityTime_06);
					}

					if (m_tParticleInfo.iVelocityCountMax > 6)
					{
						ImGui::Text("VelocityMinParticles_07");
						ImGui::InputFloat3("##VelocityMinParticles_07", m_fParticleVelocityMin_07);

						ImGui::Text("VelocityMaxParticles_07");
						ImGui::InputFloat3("##VelocityMaxParticles_07", m_fParticleVelocityMax_07);

						ImGui::Text("VelocityChangeTimeParticles_07");
						ImGui::InputFloat2("##VelocityChangeTimeParticles_07", m_fParticleVelocityTime_07);
					}

					if (m_tParticleInfo.iVelocityCountMax > 7)
					{
						ImGui::Text("VelocityMinParticles_08");
						ImGui::InputFloat3("##VelocityMinParticles_08", m_fParticleVelocityMin_08);

						ImGui::Text("VelocityMaxParticles_08");
						ImGui::InputFloat3("##VelocityMaxParticles_08", m_fParticleVelocityMax_08);

						ImGui::Text("VelocityChangeTimeParticles_08");
						ImGui::InputFloat2("##VelocityChangeTimeParticles_08", m_fParticleVelocityTime_08);
					}

					if (m_tParticleInfo.iVelocityCountMax > 8)
					{
						ImGui::Text("VelocityMinParticles_09");
						ImGui::InputFloat3("##VelocityMinParticles_09", m_fParticleVelocityMin_09);

						ImGui::Text("VelocityMaxParticles_09");
						ImGui::InputFloat3("##VelocityMaxParticles_09", m_fParticleVelocityMax_09);

						ImGui::Text("VelocityChangeTimeParticles_09");
						ImGui::InputFloat2("##VelocityChangeTimeParticles_09", m_fParticleVelocityTime_09);
					}

					if (m_tParticleInfo.iVelocityCountMax > 9)
					{
						ImGui::Text("VelocityMinParticles_10");
						ImGui::InputFloat3("##VelocityMinParticles_10", m_fParticleVelocityMin_10);

						ImGui::Text("VelocityMaxParticles_10");
						ImGui::InputFloat3("##VelocityMaxParticles_10", m_fParticleVelocityMax_10);

						ImGui::Text("VelocityChangeTimeParticles_10");
						ImGui::InputFloat2("##VelocityChangeTimeParticles_10", m_fParticleVelocityTime_10);
					}
				}

			}
		}

		// 회전
		if (ImGui::CollapsingHeader("ParticleRotation"))
		{
			ImGui::Checkbox("RotationBillboard", &m_tParticleInfo.bBillboard);
			ImGui::NewLine();

			ImGui::Checkbox("RotationRandomAxis", &m_tParticleInfo.bRandomAxis);
			if (!m_tParticleInfo.bRandomAxis)
			{
				ImGui::Text("RotationAxis");
				ImGui::InputFloat3("##RotationAxis", m_fParticleAxis);
				ImGui::NewLine();
			}
			ImGui::NewLine();

			ImGui::Checkbox("RotationRandomAngle", &m_tParticleInfo.bRandomAngle);
			if (!m_tParticleInfo.bRandomAngle)
			{
				ImGui::Text("RotationAngle");
				ImGui::InputFloat("##RangeParticles", &m_tParticleInfo.fAngle);
				ImGui::NewLine();
			}
			ImGui::NewLine();

			ImGui::Checkbox("RotationChange", &m_tParticleInfo.bRotationChange);
			ImGui::NewLine();

			if (m_tParticleInfo.bRotationChange)
			{
				ImGui::Text("RotationChangeStartDelayParticles");
				ImGui::InputFloat2("##RotationChangeStartDelayParticles", m_fParticleRotationChangeStartDelay);
				ImGui::NewLine();

				ImGui::Text("RotationSpeed");
				ImGui::InputFloat2("##RotationSpeed", m_fParticleRotationSpeed);
				ImGui::NewLine();

				ImGui::Checkbox("RotationChangeRandom", &m_tParticleInfo.bRotationChangeRandom);
				ImGui::NewLine();

				if (m_tParticleInfo.bRotationChangeRandom)
				{
					ImGui::Text("RotationChangeTime");
					ImGui::InputFloat2("##RotationChangeTime", m_fParticleRotationChangeTime);
					ImGui::NewLine();
				}
				else
				{
					ImGui::Checkbox("RotationAddRandom", &m_tParticleInfo.bRotationAdd);
					ImGui::NewLine();
				}
			}
		}

		// 지속 시간
		if (ImGui::CollapsingHeader("ParticleLifeTime"))
		{
			ImGui::Text("LifetimeParticles");
			ImGui::InputFloat2("##LifetimeParticles", m_fParticleLifeTime);
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

		// 텍스처 알파
		if (ImGui::CollapsingHeader("ParticleAlpha"))
		{
			// fStartAlpha
			ImGui::Text("StartAlpha");
			ImGui::InputFloat2("##StartAlpha", m_fStartAlpha);

			ImGui::Checkbox("FadeCreate", &m_tParticleInfo.bFadeCreate);
			ImGui::NewLine();

			ImGui::Checkbox("FadeDelete", &m_tParticleInfo.bFadeDelete);
			ImGui::NewLine();

			// fFadeSpeed
			ImGui::Text("FadeSpeed");
			ImGui::InputFloat2("##FadeSpeed", m_fFadeSpeed);
			ImGui::NewLine();
			
			ImGui::Checkbox("FadeChange", &m_tParticleInfo.bFadeChange);
			ImGui::NewLine();

			// StartDelay
			if (m_tParticleInfo.bFadeChange)
			{
				ImGui::Text("FadeChangeStartDelay");
				ImGui::InputFloat2("##FadeChangeStartDelay", m_fFadeChangeStartDelay);
				ImGui::NewLine();

				ImGui::Checkbox("FadeIn", &m_tParticleInfo.bFadeIn);
				ImGui::NewLine();
			}
		}

		// 파티클 색상
		if (ImGui::CollapsingHeader("ParticleColor"))
		{
			ImGui::Checkbox("RandomColor", &m_tParticleInfo.bColorRandom);
			if (!m_tParticleInfo.bColorRandom)
			{
				ImGui::Text("Diffuse Color");
				ImGui::ColorEdit4("##DiffuseColor", (float*)&m_tParticleInfo.vColorS, ImGuiColorEditFlags_Float);
			}
		}

	    // Gravity Modifier 중력 효과

		ImGui::NewLine();
		if (ImGui::Button("Select_ParticleSystem"))
			Store_InfoParticle();
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

		Load_InfoParticle();
	}
}

void CTool_Particle::Load_InfoParticle()
{
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

	if (m_tParticleInfo.eParticleType == CParticle::TYPE_PERSPECTIVE)
		m_bParticleType_Pers = true;
	if (m_tParticleInfo.eParticleType == CParticle::TYPE_ORTHOGRAPHIC)
		m_bParticleType_Orth = false;

	m_fParticleRange[0] = m_tParticleInfo.fRange.x;
	m_fParticleRange[1] = m_tParticleInfo.fRange.y;
	m_fParticleRange[2] = m_tParticleInfo.fRange.z;

	m_fParticleScaleStart[0] = m_tParticleInfo.fScaleStart.x;
	m_fParticleScaleStart[1] = m_tParticleInfo.fScaleStart.y;

	m_fParticleScaleChangeStartDelay[0] = m_tParticleInfo.fScaleChangeStartDelay.x;
	m_fParticleScaleChangeStartDelay[1] = m_tParticleInfo.fScaleChangeStartDelay.y;

	m_fParticleScaleChangeTime[0] = m_tParticleInfo.fScaleChangeTime.x;
	m_fParticleScaleChangeTime[1] = m_tParticleInfo.fScaleChangeTime.y;

	m_fParticleScaleMin[0] = m_tParticleInfo.fScaleMin .x;
	m_fParticleScaleMin[1] = m_tParticleInfo.fScaleMin.y;

	m_fParticleScaleMax[0] = m_tParticleInfo.fScaleMax.x;
	m_fParticleScaleMax[1] = m_tParticleInfo.fScaleMax.y;

	m_fParticleScaleSpeed[0] = m_tParticleInfo.fScaleSpeed.x;
	m_fParticleScaleSpeed[1] = m_tParticleInfo.fScaleSpeed.y;


	m_fParticleVelocityMin[0] = m_tParticleInfo.vVelocityMinStart.x;
	m_fParticleVelocityMin[1] = m_tParticleInfo.vVelocityMinStart.y;
	m_fParticleVelocityMin[2] = m_tParticleInfo.vVelocityMinStart.z;

	m_fParticleVelocityMax[0] = m_tParticleInfo.vVelocityMaxStart.x;
	m_fParticleVelocityMax[1] = m_tParticleInfo.vVelocityMaxStart.y;
	m_fParticleVelocityMax[2] = m_tParticleInfo.vVelocityMaxStart.z;

	m_fParticleVelocityChangeStartDelay[0] = m_tParticleInfo.fVelocityChangeStartDelay.x;
	m_fParticleVelocityChangeStartDelay[1] = m_tParticleInfo.fVelocityChangeStartDelay.y;

	m_fParticleVelocityChangeTime[0] = m_tParticleInfo.fVelocityChangeTime.x;
	m_fParticleVelocityChangeTime[1] = m_tParticleInfo.fVelocityChangeTime.y;

	if (m_tParticleInfo.bVelocityChange && m_tParticleInfo.pVelocityMin != nullptr && m_tParticleInfo.pVelocityMax != nullptr && m_tParticleInfo.pVelocityTime != nullptr)
	{
		if(m_tParticleInfo.iVelocityCountMax > 1) //2
		{
			m_fParticleVelocityMin[0] = m_tParticleInfo.pVelocityMin[0].x;
			m_fParticleVelocityMin[1] = m_tParticleInfo.pVelocityMin[0].y;
			m_fParticleVelocityMin[2] = m_tParticleInfo.pVelocityMin[0].z;

			m_fParticleVelocityMax[0] = m_tParticleInfo.pVelocityMax[0].x;
			m_fParticleVelocityMax[1] = m_tParticleInfo.pVelocityMax[0].y;
			m_fParticleVelocityMax[2] = m_tParticleInfo.pVelocityMax[0].z;

			m_fParticleVelocityTime_01[0] = m_tParticleInfo.pVelocityTime[0].x;
			m_fParticleVelocityTime_01[1] = m_tParticleInfo.pVelocityTime[0].y;


			m_fParticleVelocityMin_02[0] = m_tParticleInfo.pVelocityMin[1].x;
			m_fParticleVelocityMin_02[1] = m_tParticleInfo.pVelocityMin[1].y;
			m_fParticleVelocityMin_02[2] = m_tParticleInfo.pVelocityMin[1].z;

			m_fParticleVelocityMax_02[0] = m_tParticleInfo.pVelocityMax[1].x;
			m_fParticleVelocityMax_02[1] = m_tParticleInfo.pVelocityMax[1].y;
			m_fParticleVelocityMax_02[2] = m_tParticleInfo.pVelocityMax[1].z;

			m_fParticleVelocityTime_02[0] = m_tParticleInfo.pVelocityTime[1].x;
			m_fParticleVelocityTime_02[1] = m_tParticleInfo.pVelocityTime[1].y;
		}

		if (m_tParticleInfo.iVelocityCountMax > 2) //3
		{
			m_fParticleVelocityMin_03[0] = m_tParticleInfo.pVelocityMin[2].x;
			m_fParticleVelocityMin_03[1] = m_tParticleInfo.pVelocityMin[2].y;
			m_fParticleVelocityMin_03[2] = m_tParticleInfo.pVelocityMin[2].z;

			m_fParticleVelocityMax_03[0] = m_tParticleInfo.pVelocityMax[2].x;
			m_fParticleVelocityMax_03[1] = m_tParticleInfo.pVelocityMax[2].y;
			m_fParticleVelocityMax_03[2] = m_tParticleInfo.pVelocityMax[2].z;

			m_fParticleVelocityTime_03[0] = m_tParticleInfo.pVelocityTime[2].x;
			m_fParticleVelocityTime_03[1] = m_tParticleInfo.pVelocityTime[2].y;
		}

		if (m_tParticleInfo.iVelocityCountMax > 3) //4
		{
			m_fParticleVelocityMin_04[0] = m_tParticleInfo.pVelocityMin[3].x;
			m_fParticleVelocityMin_04[1] = m_tParticleInfo.pVelocityMin[3].y;
			m_fParticleVelocityMin_04[2] = m_tParticleInfo.pVelocityMin[3].z;

			m_fParticleVelocityMax_04[0] = m_tParticleInfo.pVelocityMax[3].x;
			m_fParticleVelocityMax_04[1] = m_tParticleInfo.pVelocityMax[3].y;
			m_fParticleVelocityMax_04[2] = m_tParticleInfo.pVelocityMax[3].z;

			m_fParticleVelocityTime_04[0] = m_tParticleInfo.pVelocityTime[3].x;
			m_fParticleVelocityTime_04[1] = m_tParticleInfo.pVelocityTime[3].y;
		}

		if (m_tParticleInfo.iVelocityCountMax > 4) //5
		{
			m_fParticleVelocityMin_05[0] = m_tParticleInfo.pVelocityMin[4].x;
			m_fParticleVelocityMin_05[1] = m_tParticleInfo.pVelocityMin[4].y;
			m_fParticleVelocityMin_05[2] = m_tParticleInfo.pVelocityMin[4].z;

			m_fParticleVelocityMax_05[0] = m_tParticleInfo.pVelocityMax[4].x;
			m_fParticleVelocityMax_05[1] = m_tParticleInfo.pVelocityMax[4].y;
			m_fParticleVelocityMax_05[2] = m_tParticleInfo.pVelocityMax[4].z;

			m_fParticleVelocityTime_05[0] = m_tParticleInfo.pVelocityTime[4].x;
			m_fParticleVelocityTime_05[1] = m_tParticleInfo.pVelocityTime[4].y;
		}

		if (m_tParticleInfo.iVelocityCountMax > 5) //6
		{
			m_fParticleVelocityMin_06[0] = m_tParticleInfo.pVelocityMin[5].x;
			m_fParticleVelocityMin_06[1] = m_tParticleInfo.pVelocityMin[5].y;
			m_fParticleVelocityMin_06[2] = m_tParticleInfo.pVelocityMin[5].z;

			m_fParticleVelocityMax_06[0] = m_tParticleInfo.pVelocityMax[5].x;
			m_fParticleVelocityMax_06[1] = m_tParticleInfo.pVelocityMax[5].y;
			m_fParticleVelocityMax_06[2] = m_tParticleInfo.pVelocityMax[5].z;

			m_fParticleVelocityTime_06[0] = m_tParticleInfo.pVelocityTime[5].x;
			m_fParticleVelocityTime_06[1] = m_tParticleInfo.pVelocityTime[5].y;
		}

		if (m_tParticleInfo.iVelocityCountMax > 6) //7
		{
			m_fParticleVelocityMin_07[0] = m_tParticleInfo.pVelocityMin[6].x;
			m_fParticleVelocityMin_07[1] = m_tParticleInfo.pVelocityMin[6].y;
			m_fParticleVelocityMin_07[2] = m_tParticleInfo.pVelocityMin[6].z;

			m_fParticleVelocityMax_07[0] = m_tParticleInfo.pVelocityMax[6].x;
			m_fParticleVelocityMax_07[1] = m_tParticleInfo.pVelocityMax[6].y;
			m_fParticleVelocityMax_07[2] = m_tParticleInfo.pVelocityMax[6].z;

			m_fParticleVelocityTime_07[0] = m_tParticleInfo.pVelocityTime[6].x;
			m_fParticleVelocityTime_07[1] = m_tParticleInfo.pVelocityTime[6].y;
		}

		if (m_tParticleInfo.iVelocityCountMax > 7) //8
		{
			m_fParticleVelocityMin_08[0] = m_tParticleInfo.pVelocityMin[7].x;
			m_fParticleVelocityMin_08[1] = m_tParticleInfo.pVelocityMin[7].y;
			m_fParticleVelocityMin_08[2] = m_tParticleInfo.pVelocityMin[7].z;

			m_fParticleVelocityMax_08[0] = m_tParticleInfo.pVelocityMax[7].x;
			m_fParticleVelocityMax_08[1] = m_tParticleInfo.pVelocityMax[7].y;
			m_fParticleVelocityMax_08[2] = m_tParticleInfo.pVelocityMax[7].z;

			m_fParticleVelocityTime_08[0] = m_tParticleInfo.pVelocityTime[7].x;
			m_fParticleVelocityTime_08[1] = m_tParticleInfo.pVelocityTime[7].y;
		}

		if (m_tParticleInfo.iVelocityCountMax > 8) //9
		{
			m_fParticleVelocityMin_09[0] = m_tParticleInfo.pVelocityMin[8].x;
			m_fParticleVelocityMin_09[1] = m_tParticleInfo.pVelocityMin[8].y;
			m_fParticleVelocityMin_09[2] = m_tParticleInfo.pVelocityMin[8].z;

			m_fParticleVelocityMax_09[0] = m_tParticleInfo.pVelocityMax[8].x;
			m_fParticleVelocityMax_09[1] = m_tParticleInfo.pVelocityMax[8].y;
			m_fParticleVelocityMax_09[2] = m_tParticleInfo.pVelocityMax[8].z;

			m_fParticleVelocityTime_09[0] = m_tParticleInfo.pVelocityTime[8].x;
			m_fParticleVelocityTime_09[1] = m_tParticleInfo.pVelocityTime[8].y;
		}

		if (m_tParticleInfo.iVelocityCountMax > 9) //10
		{
			m_fParticleVelocityMin_10[0] = m_tParticleInfo.pVelocityMin[9].x;
			m_fParticleVelocityMin_10[1] = m_tParticleInfo.pVelocityMin[9].y;
			m_fParticleVelocityMin_10[2] = m_tParticleInfo.pVelocityMin[9].z;
																		
			m_fParticleVelocityMax_10[0] = m_tParticleInfo.pVelocityMax[9].x;
			m_fParticleVelocityMax_10[1] = m_tParticleInfo.pVelocityMax[9].y;
			m_fParticleVelocityMax_10[2] = m_tParticleInfo.pVelocityMax[9].z;

			m_fParticleVelocityTime_10[0] = m_tParticleInfo.pVelocityTime[9].x;
			m_fParticleVelocityTime_10[1] = m_tParticleInfo.pVelocityTime[9].y;
		}
	}


	m_fParticleAxis[0] = XMVectorGetX(m_tParticleInfo.vAxis);
	m_fParticleAxis[1] = XMVectorGetY(m_tParticleInfo.vAxis);
	m_fParticleAxis[2] = XMVectorGetZ(m_tParticleInfo.vAxis);

	m_fParticleRotationSpeed[0] = m_tParticleInfo.fRotationSpeed.x;
	m_fParticleRotationSpeed[1] = m_tParticleInfo.fRotationSpeed.y;

	m_fParticleRotationChangeStartDelay[0] = m_tParticleInfo.fRotationChangeStartDelay.x;
	m_fParticleRotationChangeStartDelay[1] = m_tParticleInfo.fRotationChangeStartDelay.y;

	m_fParticleRotationChangeTime[0] = m_tParticleInfo.fRotationChangeTime.x;
	m_fParticleRotationChangeTime[1] = m_tParticleInfo.fRotationChangeTime.y;


	m_fParticleLifeTime[0] = m_tParticleInfo.fLifeTime.x;
	m_fParticleLifeTime[1] = m_tParticleInfo.fLifeTime.y;

	m_fParticleSpeed[0] = m_tParticleInfo.fVelocitySpeed.x;
	m_fParticleSpeed[1] = m_tParticleInfo.fVelocitySpeed.y;

	m_fParticleBoxMin[0] = m_tParticleInfo.fBoxMin.x;
	m_fParticleBoxMin[1] = m_tParticleInfo.fBoxMin.y;
	m_fParticleBoxMin[2] = m_tParticleInfo.fBoxMin.z;

	m_fParticleBoxMax[0] = m_tParticleInfo.fBoxMax.x;
	m_fParticleBoxMax[1] = m_tParticleInfo.fBoxMax.y;
	m_fParticleBoxMax[2] = m_tParticleInfo.fBoxMax.z;

	m_fParticleAnimationSpeed[0] = m_tParticleInfo.fAnimationSpeed.x;
	m_fParticleAnimationSpeed[1] = m_tParticleInfo.fAnimationSpeed.y;

	m_fParticleUVIndex[0] = m_tParticleInfo.fUVIndex.x;
	m_fParticleUVIndex[1] = m_tParticleInfo.fUVIndex.y;

	m_fParticleUVMaxCount[0] = m_tParticleInfo.fUVMaxCount.x;
	m_fParticleUVMaxCount[1] = m_tParticleInfo.fUVMaxCount.y;


	m_fFadeChangeStartDelay[0] = m_tParticleInfo.fFadeChangeStartDelay.x;
	m_fFadeChangeStartDelay[1] = m_tParticleInfo.fFadeChangeStartDelay.y;

	m_fStartAlpha[0] = m_tParticleInfo.fStartAlpha.x;
	m_fStartAlpha[1] = m_tParticleInfo.fStartAlpha.y;

	m_fFadeSpeed[0] = m_tParticleInfo.fFadeSpeed.x;
	m_fFadeSpeed[1] = m_tParticleInfo.fFadeSpeed.y;
}

void CTool_Particle::Store_InfoParticle()
{
	if (m_pParticle != nullptr)
	{
		if (m_bParticleType_Pers == true)
			m_tParticleInfo.eParticleType = CParticle::TYPE_PERSPECTIVE;
		else if (m_bParticleType_Orth == true)
			m_tParticleInfo.eParticleType = CParticle::TYPE_ORTHOGRAPHIC;

		m_tParticleInfo.fRange = _float3(m_fParticleRange[0], m_fParticleRange[1], m_fParticleRange[2]);

		m_tParticleInfo.fScaleStart = _float2(m_fParticleScaleStart[0], m_fParticleScaleStart[1]);
		m_tParticleInfo.fScaleChangeStartDelay = _float2(m_fParticleScaleChangeStartDelay[0], m_fParticleScaleChangeStartDelay[1]);
		m_tParticleInfo.fScaleChangeTime = _float2(m_fParticleScaleChangeTime[0], m_fParticleScaleChangeTime[1]);

		m_tParticleInfo.fScaleMin   = _float2(m_fParticleScaleMin[0], m_fParticleScaleMin[1]);
		m_tParticleInfo.fScaleMax   = _float2(m_fParticleScaleMax[0], m_fParticleScaleMax[1]);
		m_tParticleInfo.fScaleSpeed = _float2(m_fParticleScaleSpeed[0], m_fParticleScaleSpeed[1]);

		m_tParticleInfo.vVelocityMinStart = _float3(m_fParticleVelocityMin[0], m_fParticleVelocityMin[1], m_fParticleVelocityMin[2]);
		m_tParticleInfo.vVelocityMaxStart = _float3(m_fParticleVelocityMax[0], m_fParticleVelocityMax[1], m_fParticleVelocityMax[2]);

		m_tParticleInfo.fVelocityChangeStartDelay = _float2(m_fParticleVelocityChangeStartDelay[0], m_fParticleVelocityChangeStartDelay[1]);
		m_tParticleInfo.fVelocityChangeTime = _float2(m_fParticleVelocityChangeTime[0], m_fParticleVelocityChangeTime[1]);

		if (m_tParticleInfo.bVelocityChange && m_tParticleInfo.pVelocityMin != nullptr && m_tParticleInfo.pVelocityMax != nullptr && m_tParticleInfo.pVelocityTime != nullptr)
		{
			if (m_tParticleInfo.iVelocityCountMax > 1) //2
			{
				m_tParticleInfo.pVelocityMin[0] = _float3(m_fParticleVelocityMin[0], m_fParticleVelocityMin[1], m_fParticleVelocityMin[2]);
				m_tParticleInfo.pVelocityMax[0] = _float3(m_fParticleVelocityMax[0], m_fParticleVelocityMax[1], m_fParticleVelocityMax[2]);
				m_tParticleInfo.pVelocityTime[0] = _float2(m_fParticleVelocityTime_01[0], m_fParticleVelocityTime_01[1]);

				m_tParticleInfo.pVelocityMin[1] = _float3(m_fParticleVelocityMin_02[0], m_fParticleVelocityMin_02[1], m_fParticleVelocityMin_02[2]);
				m_tParticleInfo.pVelocityMax[1] = _float3(m_fParticleVelocityMax_02[0], m_fParticleVelocityMax_02[1], m_fParticleVelocityMax_02[2]);
				m_tParticleInfo.pVelocityTime[1] = _float2(m_fParticleVelocityTime_02[0], m_fParticleVelocityTime_02[1]);
			}

			if (m_tParticleInfo.iVelocityCountMax > 2) //3
			{
				m_tParticleInfo.pVelocityMin[2] = _float3(m_fParticleVelocityMin_03[0], m_fParticleVelocityMin_03[1], m_fParticleVelocityMin_03[2]);
				m_tParticleInfo.pVelocityMax[2] = _float3(m_fParticleVelocityMax_03[0], m_fParticleVelocityMax_03[1], m_fParticleVelocityMax_03[2]);
				m_tParticleInfo.pVelocityTime[2] = _float2(m_fParticleVelocityTime_03[0], m_fParticleVelocityTime_03[1]);
			}

			if (m_tParticleInfo.iVelocityCountMax > 3) //4
			{
				m_tParticleInfo.pVelocityMin[3] = _float3(m_fParticleVelocityMin_04[0], m_fParticleVelocityMin_04[1], m_fParticleVelocityMin_04[2]);
				m_tParticleInfo.pVelocityMax[3] = _float3(m_fParticleVelocityMax_04[0], m_fParticleVelocityMax_04[1], m_fParticleVelocityMax_04[2]);
				m_tParticleInfo.pVelocityTime[3] = _float2(m_fParticleVelocityTime_04[0], m_fParticleVelocityTime_04[1]);
			}

			if (m_tParticleInfo.iVelocityCountMax > 4) //5
			{
				m_tParticleInfo.pVelocityMin[4] = _float3(m_fParticleVelocityMin_05[0], m_fParticleVelocityMin_05[1], m_fParticleVelocityMin_05[2]);
				m_tParticleInfo.pVelocityMax[4] = _float3(m_fParticleVelocityMax_05[0], m_fParticleVelocityMax_05[1], m_fParticleVelocityMax_05[2]);
				m_tParticleInfo.pVelocityTime[4] = _float2(m_fParticleVelocityTime_05[0], m_fParticleVelocityTime_05[1]);
			}

			if (m_tParticleInfo.iVelocityCountMax > 5) //6
			{
				m_tParticleInfo.pVelocityMin[5] = _float3(m_fParticleVelocityMin_06[0], m_fParticleVelocityMin_06[1], m_fParticleVelocityMin_06[2]);
				m_tParticleInfo.pVelocityMax[5] = _float3(m_fParticleVelocityMax_06[0], m_fParticleVelocityMax_06[1], m_fParticleVelocityMax_06[2]);
				m_tParticleInfo.pVelocityTime[5] = _float2(m_fParticleVelocityTime_06[0], m_fParticleVelocityTime_06[1]);
			}

			if (m_tParticleInfo.iVelocityCountMax > 6) //7
			{
				m_tParticleInfo.pVelocityMin[6] = _float3(m_fParticleVelocityMin_07[0], m_fParticleVelocityMin_07[1], m_fParticleVelocityMin_07[2]);
				m_tParticleInfo.pVelocityMax[6] = _float3(m_fParticleVelocityMax_07[0], m_fParticleVelocityMax_07[1], m_fParticleVelocityMax_07[2]);
				m_tParticleInfo.pVelocityTime[6] = _float2(m_fParticleVelocityTime_07[0], m_fParticleVelocityTime_07[1]);
			}

			if (m_tParticleInfo.iVelocityCountMax > 7) //8
			{
				m_tParticleInfo.pVelocityMin[7] = _float3(m_fParticleVelocityMin_08[0], m_fParticleVelocityMin_08[1], m_fParticleVelocityMin_08[2]);
				m_tParticleInfo.pVelocityMax[7] = _float3(m_fParticleVelocityMax_08[0], m_fParticleVelocityMax_08[1], m_fParticleVelocityMax_08[2]);
				m_tParticleInfo.pVelocityTime[7] = _float2(m_fParticleVelocityTime_08[0], m_fParticleVelocityTime_08[1]);
			}

			if (m_tParticleInfo.iVelocityCountMax > 8) //9
			{
				m_tParticleInfo.pVelocityMin[8] = _float3(m_fParticleVelocityMin_09[0], m_fParticleVelocityMin_09[1], m_fParticleVelocityMin_09[2]);
				m_tParticleInfo.pVelocityMax[8] = _float3(m_fParticleVelocityMax_09[0], m_fParticleVelocityMax_09[1], m_fParticleVelocityMax_09[2]);
				m_tParticleInfo.pVelocityTime[8] = _float2(m_fParticleVelocityTime_09[0], m_fParticleVelocityTime_09[1]);
			}

			if (m_tParticleInfo.iVelocityCountMax > 9) //10
			{
				m_tParticleInfo.pVelocityMin[9] = _float3(m_fParticleVelocityMin_10[0], m_fParticleVelocityMin_10[1], m_fParticleVelocityMin_10[2]);
				m_tParticleInfo.pVelocityMax[9] = _float3(m_fParticleVelocityMax_10[0], m_fParticleVelocityMax_10[1], m_fParticleVelocityMax_10[2]);
				m_tParticleInfo.pVelocityTime[9] = _float2(m_fParticleVelocityTime_10[0], m_fParticleVelocityTime_10[1]);
			}
		}


		m_tParticleInfo.vAxis               = XMVectorSet(m_fParticleAxis[0], m_fParticleAxis[1], m_fParticleAxis[2], 0.f);
		m_tParticleInfo.fRotationSpeed      = _float2(m_fParticleRotationSpeed[0], m_fParticleRotationSpeed[1]);
		m_tParticleInfo.fRotationChangeStartDelay = _float2(m_fParticleRotationChangeStartDelay[0], m_fParticleRotationChangeStartDelay[1]);
		m_tParticleInfo.fRotationChangeTime = _float2(m_fParticleRotationChangeTime[0], m_fParticleRotationChangeTime[1]);


		m_tParticleInfo.fLifeTime = _float2(m_fParticleLifeTime[0], m_fParticleLifeTime[1]);
		m_tParticleInfo.fVelocitySpeed = _float2(m_fParticleSpeed[0], m_fParticleSpeed[1]);
		m_tParticleInfo.fBoxMin = _float3(m_fParticleBoxMin[0], m_fParticleBoxMin[1], m_fParticleBoxMin[2]);
		m_tParticleInfo.fBoxMax = _float3(m_fParticleBoxMax[0], m_fParticleBoxMax[1], m_fParticleBoxMax[2]);
		m_tParticleInfo.fAnimationSpeed = _float2(m_fParticleAnimationSpeed[0], m_fParticleAnimationSpeed[1]);

		m_tParticleInfo.fUVIndex    = _float2(m_fParticleUVIndex[0], m_fParticleUVIndex[1]);
		m_tParticleInfo.fUVMaxCount = _float2(m_fParticleUVMaxCount[0], m_fParticleUVMaxCount[1]);

		wstring strDiffuseTextureName(m_cDiffuseTextureName, m_cDiffuseTextureName + strlen(m_cDiffuseTextureName));
		m_tParticleInfo.strDiffuseTetextureName = strDiffuseTextureName;
		wstring strDiffuseTexturePath(m_cDiffuseTexturePath, m_cDiffuseTexturePath + strlen(m_cDiffuseTexturePath));
		m_tParticleInfo.strDiffuseTetexturePath = strDiffuseTexturePath;
		wstring strAlphaTextureName(m_cAlphaTextureName, m_cAlphaTextureName + strlen(m_cAlphaTextureName));
		m_tParticleInfo.strAlphaTexturName = strAlphaTextureName;
		wstring strAlphaTexturePath(m_cAlphaTexturePath, m_cAlphaTexturePath + strlen(m_cAlphaTexturePath));
		m_tParticleInfo.strAlphaTexturPath = strAlphaTexturePath;


		m_tParticleInfo.fFadeChangeStartDelay = _float2(m_fFadeChangeStartDelay[0], m_fFadeChangeStartDelay[1]);
		m_tParticleInfo.fStartAlpha = _float2(m_fStartAlpha[0], m_fStartAlpha[1]);
		m_tParticleInfo.fFadeSpeed  = _float2(m_fFadeSpeed[0],  m_fFadeSpeed[1]);


		static_cast<CParticle*>(m_pParticle)->Set_ParticleDesc(m_tParticleInfo);
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
	File->Write<_bool>(m_tParticleInfo.bParticleLoop);

	// 파티클 개수
	File->Write<_uint>(m_tParticleInfo.iNumEffectCount);

	// 분포 범위
	File->Write<_float3>(m_tParticleInfo.fRange);

	// 크기
	File->Write<_bool>(m_tParticleInfo.bScaleSameRate);
	File->Write<_float2>(m_tParticleInfo.fScaleStart);

	// 지속 시간
	File->Write<_float2>(m_tParticleInfo.fLifeTime);

	// 속도
	File->Write<_float2>(m_tParticleInfo.fVelocitySpeed);

	// 움직임
	File->Write<_float3>(m_tParticleInfo.vVelocityMinStart);
	File->Write<_float3>(m_tParticleInfo.vVelocityMaxStart);

	// 박스 범위
	File->Write<_bool>(m_tParticleInfo.bUseBox);
	File->Write<_float3>(m_tParticleInfo.fBoxMin);
	File->Write<_float3>(m_tParticleInfo.fBoxMax);

	// 색상
	File->Write<_bool>(m_tParticleInfo.bColorRandom);
	File->Write<_float4>(m_tParticleInfo.vColorS);

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
	File->Read<_bool>(ParticleInfo.bParticleLoop);

	// 파티클 개수
	File->Read<_uint>(ParticleInfo.iNumEffectCount);

	// 분포 범위
	File->Read<_float3>(ParticleInfo.fRange);

	// 크기
	File->Read<_bool>(ParticleInfo.bScaleSameRate);
	File->Read<_float2>(ParticleInfo.fScaleStart);

	// 지속 시간
	File->Read<_float2>(ParticleInfo.fLifeTime);

	// 속도
	File->Read<_float2>(ParticleInfo.fVelocitySpeed);

	// 움직임
	File->Read<_float3>(ParticleInfo.vVelocityMinStart);
	File->Read<_float3>(ParticleInfo.vVelocityMaxStart);

	// 박스 범위
	File->Read<_bool>(ParticleInfo.bUseBox);
	File->Read<_float3>(ParticleInfo.fBoxMin);
	File->Read<_float3>(ParticleInfo.fBoxMax);

	// 색상
	File->Read<_bool>(ParticleInfo.bColorRandom);
	File->Read<_float4>(ParticleInfo.vColorS);

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

	Load_InfoParticle();

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
