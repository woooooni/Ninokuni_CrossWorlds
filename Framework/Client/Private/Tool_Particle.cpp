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

#pragma region Tick
void CTool_Particle::Tick(_float fTimeDelta)
{
	ImGui::Begin("Particle_Tool");

	// CreateDelete
	Tick_CreateDelete();
	ImGui::NewLine();

	// Binary
	Tick_Binary();
	ImGui::NewLine();

	// Prototype
	Tick_Prototype();
	ImGui::NewLine();

	// Transform
	Tick_Transform();
	ImGui::NewLine();

#pragma region ParticleSystem
	_bool bParticleSystemUse = false;

	if (ImGui::CollapsingHeader("ParticleSystem"))
	{
		if (ImGui::Button("Restart"))
		{
			Store_InfoParticle();
		}

		// 기본 정보
		if (ImGui::CollapsingHeader("BasicInfo"))
		{
			// 파티클 개수 //ImGui::Text("MaxParticles"); //ImGui::InputInt("##MaxParticles", &(_int)m_tParticleInfo.iNumEffectMaxCount);
			ImGui::Text("ParticlesCount");
			if (ImGui::InputInt("##ParticlesCount", &(_int)m_tParticleInfo.iNumEffectCount))
			{
				bParticleSystemUse = true;
				m_tParticleInfo.iNumEffectMaxCount = m_tParticleInfo.iNumEffectCount;
			}
			ImGui::NewLine();

			// 투영 타입
			ImGui::Text("Projection :");
			ImGui::SameLine();
			if (ImGui::Combo("##Projection", &m_iProjectionIndex, m_cProjection, IM_ARRAYSIZE(m_cProjection)))
				bParticleSystemUse = true;
			ImGui::NewLine();

			// 반복 여부
			if (ImGui::Checkbox("Loop", &m_tParticleInfo.bParticleLoop))
				bParticleSystemUse = true;
			ImGui::NewLine();

			// 파티클 Z정렬 (알파 값이 있을 시 사용 권장)
			if (ImGui::Checkbox("SortZ", &m_tParticleInfo.bParticleSortZ))
				bParticleSystemUse = true;
			ImGui::NewLine();
		}

		// 위치 (분포 범위)
		if (ImGui::CollapsingHeader("Range/StartPosition"))
		{
			ImGui::Text("Range");
			if (ImGui::InputFloat3("##Range", &m_tParticleInfo.fRange.x))
				bParticleSystemUse = true;
			ImGui::NewLine();

			ImGui::Text("StartDistance");
			if (ImGui::InputFloat2("##StartDistance", &m_tParticleInfo.fRangeDistance.x))
				bParticleSystemUse = true;
			ImGui::NewLine();
		}

		// 크기
		if (ImGui::CollapsingHeader("Scale"))
		{
			if (ImGui::Checkbox("ScaleSameRate", &m_tParticleInfo.bScaleSameRate))
				bParticleSystemUse = true;
			ImGui::NewLine();

			ImGui::Text("ScaleStart");
			if (ImGui::InputFloat2("##ScaleStart", &m_tParticleInfo.fScaleStart.x))
				bParticleSystemUse = true;
			ImGui::NewLine();

			if (ImGui::Checkbox("ScaleChange", &m_tParticleInfo.bScaleChange))
				bParticleSystemUse = true;
			ImGui::NewLine();
			if (m_tParticleInfo.bScaleChange)
			{
				ImGui::Text("ScaleChangeStartDelayParticles");
				if (ImGui::InputFloat2("##ScaleChangeStartDelayParticles", &m_tParticleInfo.fScaleChangeStartDelay.x))
					bParticleSystemUse = true;
				ImGui::NewLine();

				ImGui::Text("ScaleSpeedParticles");
				if (ImGui::InputFloat2("##ScaleSpeedParticles", &m_tParticleInfo.fScaleSpeed.x))
					bParticleSystemUse = true;
				ImGui::NewLine();

				if (ImGui::Checkbox("ScaleChangeRandom", &m_tParticleInfo.bScaleChangeRandom))
					bParticleSystemUse = true;
				ImGui::NewLine();
				if (m_tParticleInfo.bScaleChangeRandom)
				{
					ImGui::Text("ScaleChangeTimeParticles");
					if (ImGui::InputFloat2("##ScaleChangeTimeParticles", &m_tParticleInfo.fScaleChangeTime.x))
						bParticleSystemUse = true;
					ImGui::NewLine();
				}
				else
				{
					ImGui::Text("ScaleMinParticles");
					if (ImGui::InputFloat2("##ScaleMinParticles", &m_tParticleInfo.fScaleMin.x))
						bParticleSystemUse = true;
					ImGui::NewLine();

					ImGui::Text("ScaleMaxParticles");
					if (ImGui::InputFloat2("##ScaleMaxParticles", &m_tParticleInfo.fScaleMax.x))
						bParticleSystemUse = true;
					ImGui::NewLine();

					if (ImGui::Checkbox("ScaleAdd", &m_tParticleInfo.bScaleAdd))
						bParticleSystemUse = true;
					ImGui::NewLine();

					if (ImGui::Checkbox("ScaleLoop", &m_tParticleInfo.bScaleLoop))
						bParticleSystemUse = true;
					ImGui::NewLine();

					if (ImGui::Checkbox("ScaleLoopStart", &m_tParticleInfo.bScaleLoopStart))
						bParticleSystemUse = true;
					ImGui::NewLine();
				}
			}
		}

		// 이동(힘)
		if (ImGui::CollapsingHeader("Velocity"))
		{
			ImGui::Text("VelocitySpeed");
			if (ImGui::InputFloat2("##VelocitySpeed", &m_tParticleInfo.fVelocitySpeed.x))
				bParticleSystemUse = true;
			ImGui::NewLine();

			ImGui::Text("VelocityMinParticles");
			if (ImGui::InputFloat3("##VelocityMinParticles", &m_tParticleInfo.vVelocityMinStart.x))
				bParticleSystemUse = true;

			ImGui::Text("VelocityMaxParticles");
			if (ImGui::InputFloat3("##VelocityMaxParticles", &m_tParticleInfo.vVelocityMaxStart.x))
				bParticleSystemUse = true;
			ImGui::NewLine();
			ImGui::NewLine();


			if (ImGui::Checkbox("VelocityChange", &m_tParticleInfo.bVelocityChange))
				bParticleSystemUse = true;
			ImGui::NewLine();
			if (m_tParticleInfo.bVelocityChange)
			{
				ImGui::Text("VelocityChangeStartDelayParticles");
				if (ImGui::InputFloat2("##VelocityChangeStartDelayParticles", &m_tParticleInfo.fVelocityChangeStartDelay.x))
					bParticleSystemUse = true;
				ImGui::NewLine();

				if (ImGui::Checkbox("VelocityChangeRandom", &m_tParticleInfo.bVelocityChangeRandom))
					bParticleSystemUse = true;
				ImGui::NewLine();
				if (m_tParticleInfo.bVelocityChangeRandom)
				{
					ImGui::Text("VelocityChangeTimeParticles");
					if (ImGui::InputFloat2("##VelocityChangeTimeParticles", &m_tParticleInfo.fVelocityChangeTime.x))
						bParticleSystemUse = true;
					ImGui::NewLine();

					m_tParticleInfo.iVelocityUse = 0;
				}
				else
				{
					ImGui::Text("VelocityUseParticles");
					if (ImGui::InputInt("##VelocityUseParticles", &(_int)m_tParticleInfo.iVelocityUse))
					{
						bParticleSystemUse = true;
						if (30 < m_tParticleInfo.iVelocityUse)
							m_tParticleInfo.iVelocityUse = 30;
					}

					if (ImGui::Checkbox("VelocityLoop", &m_tParticleInfo.bVelocityLoop))
						bParticleSystemUse = true;
					ImGui::NewLine();

					ImGui::Text("VelocityCountCur");
					if (ImGui::InputInt("##VelocityCountCur", &(_int)m_tParticleInfo.iVelocityCountCur))
						bParticleSystemUse = true;
					ImGui::NewLine();

					ImGui::Text("iVelocityCountMax");
					if (ImGui::InputInt("##iVelocityCountMax", &(_int)m_tParticleInfo.iVelocityCountMax))
						bParticleSystemUse = true;
					ImGui::NewLine();

					if (nullptr != m_tParticleInfo.pVelocityMin && nullptr != m_tParticleInfo.pVelocityMax && nullptr != m_tParticleInfo.pVelocityTime)
					{
						for (size_t i = 0; i < m_tParticleInfo.iVelocityCountMax; ++i)
						{
							std::string scaleOffsetString = "ChangeTime_" + std::to_string(i);
							ImGui::InputFloat3(scaleOffsetString.c_str(), &m_tParticleInfo.pVelocityTime[i].x);

							std::string frameTrigerString = "VelocityMin_" + std::to_string(i);
							ImGui::InputFloat3(frameTrigerString.c_str(), &m_tParticleInfo.pVelocityMin[i].x);

							std::string positionOffsetString = "VelocityMax_" + std::to_string(i);
							ImGui::InputFloat3(positionOffsetString.c_str(), &m_tParticleInfo.pVelocityMax[i].x);

							ImGui::NewLine();
						}
					}
				}
			}
		}

		// 회전
		if (ImGui::CollapsingHeader("Rotation"))
		{
			if (ImGui::Checkbox("RotationBillboard", &m_tParticleInfo.bBillboard))
				bParticleSystemUse = true;
			ImGui::NewLine();

			if (!m_tParticleInfo.bBillboard)
			{
				if (ImGui::Checkbox("RotationRandomAxis", &m_tParticleInfo.bRandomAxis))
					bParticleSystemUse = true;
				if (!m_tParticleInfo.bRandomAxis)
				{
					ImGui::Text("RotationAxis");
					if (ImGui::InputFloat3("##RotationAxis", &m_fParticleAxis.x))
						bParticleSystemUse = true;
				}
				ImGui::NewLine();

				if (ImGui::Checkbox("RotationRandomAngle", &m_tParticleInfo.bRandomAngle))
					bParticleSystemUse = true;
				if (!m_tParticleInfo.bRandomAngle)
				{
					ImGui::Text("RotationAngle");
					if (ImGui::InputFloat("##RangeParticles", &m_tParticleInfo.fAngle))
						bParticleSystemUse = true;
				}
				ImGui::NewLine();

				if (ImGui::Checkbox("RotationChange", &m_tParticleInfo.bRotationChange))
					bParticleSystemUse = true;
				ImGui::NewLine();
				if (m_tParticleInfo.bRotationChange)
				{
					ImGui::Text("RotationChangeStartDelayParticles");
					if (ImGui::InputFloat2("##RotationChangeStartDelayParticles", &m_tParticleInfo.fRotationChangeStartDelay.x))
						bParticleSystemUse = true;
					ImGui::NewLine();

					ImGui::Text("RotationSpeed");
					if (ImGui::InputFloat2("##RotationSpeed", &m_tParticleInfo.fRotationSpeed.x))
						bParticleSystemUse = true;
					ImGui::NewLine();
					ImGui::NewLine();

					if (ImGui::Checkbox("RotationChangeRandom", &m_tParticleInfo.bRotationChangeRandom))
						bParticleSystemUse = true;
					ImGui::NewLine();
					if (m_tParticleInfo.bRotationChangeRandom)
					{
						ImGui::Text("RotationChangeTime");
						if (ImGui::InputFloat2("##RotationChangeTime", &m_tParticleInfo.fRotationChangeTime.x))
							bParticleSystemUse = true;
						ImGui::NewLine();
					}
					else
					{
						if (ImGui::Checkbox("RotationAddRandom", &m_tParticleInfo.bRotationAdd))
							bParticleSystemUse = true;
						ImGui::NewLine();
					}
				}
			}
		}

		// 지속 시간
		if (ImGui::CollapsingHeader("LifeTime"))
		{
			ImGui::Text("LifetimeParticles");
			if (ImGui::InputFloat2("##LifetimeParticles", &m_tParticleInfo.fLifeTime.x))
				bParticleSystemUse = true;
			ImGui::NewLine();
		}

		// 텍스처 지정
		if (ImGui::CollapsingHeader("Texture"))
		{
			// 시작 텍스처 인덱스
			if (ImGui::Checkbox("RandomStartTextureIndex", &m_tParticleInfo.bRandomStartIndex))
				bParticleSystemUse = true;
			if (!m_tParticleInfo.bRandomStartIndex)
			{
				ImGui::Text("ParticleUVIndex");
				if (ImGui::InputFloat2("##ParticleUVIndex", &m_tParticleInfo.fUVIndex.x))
					bParticleSystemUse = true;
				ImGui::NewLine();
			}
			ImGui::Text("ParticleUVMaxCount");
			if (ImGui::InputFloat2("##ParticleUVMaxCount", &m_tParticleInfo.fUVMaxCount.x))
				bParticleSystemUse = true;
			ImGui::NewLine();
			ImGui::NewLine();


			ImGui::Text("DiffuseFolderName :");
			ImGui::SameLine();
			if (ImGui::Combo("##DiffuseFolderName", &m_iDiffuseFolderIndex, m_cFolderName, IM_ARRAYSIZE(m_cFolderName)))
				bParticleSystemUse = true;

			if (m_pParticle != nullptr)
			{
				ImGui::Text("DiffuseTextureIndex");
				ImGui::SameLine();
				if (ImGui::InputInt("##DiffuseTextureIndex", &(_int)m_tParticleInfo.iTextureIndexDiffuse))
					bParticleSystemUse = true;
				ImGui::NewLine();

				CTexture* pDiffuseTexture = static_cast<CParticle*>(m_pParticle)->Get_DiffuseTexture();
				if (pDiffuseTexture != nullptr)
				{
					ImGui::Text("Diffuse Texture");
					if (ImGui::BeginListBox("##Effect_DiffuseTexture_List", ImVec2(450.f, 200.f)))
					{
						for (size_t i = 0; i < pDiffuseTexture->Get_TextureCount(); ++i)
						{
							if (i % 5 != 0)
								IMGUI_SAME_LINE;

							if (ImGui::ImageButton(pDiffuseTexture->Get_Srv(i), ImVec2(50.f, 50.f)))
							{
								m_tParticleInfo.iTextureIndexDiffuse = i;
								bParticleSystemUse = true;
							}
						}
						ImGui::EndListBox();
					}
				}
			}

			// 알파 텍스처 지정
			ImGui::Text("AlphaFolderName :");
			ImGui::SameLine();
			if (ImGui::Combo("##AlphaFolderName", &m_iAlphaFolderIndex, m_cFolderName, IM_ARRAYSIZE(m_cFolderName)))
				bParticleSystemUse = true;

			if (m_pParticle != nullptr)
			{
				ImGui::Text("AlphaTextureIndex");
				ImGui::SameLine();
				if (ImGui::InputInt("##AlphaTextureIndex", &(_int)m_tParticleInfo.iTextureIndexAlpha))
					bParticleSystemUse = true;
				ImGui::NewLine();

				CTexture* pAlphaTexture = static_cast<CParticle*>(m_pParticle)->Get_AlphaTexture();
				if (pAlphaTexture != nullptr)
				{
					ImGui::Text("Alpha Texture");
					if (ImGui::BeginListBox("##Effect_AlphaTexture_List", ImVec2(450.f, 200.f)))
					{
						for (size_t i = 0; i < pAlphaTexture->Get_TextureCount(); ++i)
						{
							if (i % 5 != 0)
								IMGUI_SAME_LINE;

							if (ImGui::ImageButton(pAlphaTexture->Get_Srv(i), ImVec2(50.f, 50.f)))
							{
								m_tParticleInfo.iTextureIndexAlpha = i;
								bParticleSystemUse = true;
							}
						}
						ImGui::EndListBox();
					}
				}
			}
		}

		// 애니메이션
		if (ImGui::CollapsingHeader("Animation"))
		{
			if (ImGui::Checkbox("UseAnimation", &m_tParticleInfo.bAnimation))
				bParticleSystemUse = true;

			if (m_tParticleInfo.bAnimation)
			{
				if (ImGui::Checkbox("LoopAnimation", &m_tParticleInfo.bAnimationLoop))
					bParticleSystemUse = true;

				ImGui::Text("SpeedAnimation");
				if (ImGui::InputFloat2("##SpeedAnimation", &m_tParticleInfo.fAnimationSpeed.x))
					bParticleSystemUse = true;
			}
		}

		// 색상
		if (ImGui::CollapsingHeader("Color"))
		{
			if (ImGui::Checkbox("ColorRandom", &m_tParticleInfo.bColorRandom))
				bParticleSystemUse = true;
			if (!m_tParticleInfo.bColorRandom)
			{
				ImGui::Text("ColorStart");
				if (ImGui::ColorEdit4("##ColorStart", (float*)&m_tParticleInfo.vColorS, ImGuiColorEditFlags_Float))
					bParticleSystemUse = true;
			}
			ImGui::NewLine();

			if (ImGui::Checkbox("ColorChange", &m_tParticleInfo.bColorChange))
				bParticleSystemUse = true;
			if (m_tParticleInfo.bColorChange)
			{
				// fColorChangeStartDelay
				ImGui::Text("ColorChangeStartDelay");
				if (ImGui::InputFloat2("##ColorChangeStartDelay", &m_tParticleInfo.fColorChangeStartDelay.x))
					bParticleSystemUse = true;
				ImGui::NewLine();

				if (ImGui::Checkbox("ColorChangeRandom", &m_tParticleInfo.bColorChangeRandom))
					bParticleSystemUse = true;
				ImGui::NewLine();
				if (m_tParticleInfo.bColorChangeRandom)
				{
					ImGui::Text("ColorChangeRandomTime");
					if (ImGui::InputFloat2("##ColorChangeRandomTime", &m_tParticleInfo.fColorChangeRandomTime.x))
						bParticleSystemUse = true;
					ImGui::NewLine();
				}
				else
				{
					if (ImGui::Checkbox("ColorLoop", &m_tParticleInfo.bColorLoop))
						bParticleSystemUse = true;
					ImGui::NewLine();

					// fColorChangeStartM
					ImGui::Text("ColorChangeStartM");
					if (ImGui::InputFloat2("##ColorChangeStartM", &m_tParticleInfo.fColorChangeStartM.x))
						bParticleSystemUse = true;

					ImGui::Text("ColorMiddle");
					if (ImGui::ColorEdit4("##ColorMiddle", (float*)&m_tParticleInfo.fColorM, ImGuiColorEditFlags_Float))
						bParticleSystemUse = true;
					ImGui::NewLine();

					// fColorChangeStartF
					ImGui::Text("ColorChangeStartF");
					if (ImGui::InputFloat2("##ColorChangeStartF", &m_tParticleInfo.fColorChangeStartF.x))
						bParticleSystemUse = true;

					ImGui::Text("ColorFinal");
					if (ImGui::ColorEdit4("##ColorFinal", (float*)&m_tParticleInfo.fColorF, ImGuiColorEditFlags_Float))
						bParticleSystemUse = true;
					ImGui::NewLine();
				}

				// ColorDurationTime
				ImGui::Text("ColorDurationTime");
				if (ImGui::InputFloat2("##ColorDurationTime", &m_tParticleInfo.fColorDuration.x))
					bParticleSystemUse = true;
			}
		}

		// 알파
		if (ImGui::CollapsingHeader("Alpha"))
		{
			// fStartAlpha
			ImGui::Text("StartAlpha");
			if (ImGui::InputFloat2("##StartAlpha", &m_tParticleInfo.fStartAlpha.x))
				bParticleSystemUse = true;

			if (ImGui::Checkbox("AlphaCreate", &m_tParticleInfo.bFadeCreate))
				bParticleSystemUse = true;
			if (ImGui::Checkbox("AlphaDelete", &m_tParticleInfo.bFadeDelete))
				bParticleSystemUse = true;
			ImGui::NewLine();

			// fFadeSpeed
			ImGui::Text("AlphaSpeed");
			if (ImGui::InputFloat2("##AlphaSpeed", &m_tParticleInfo.fFadeSpeed.x))
				bParticleSystemUse = true;
			ImGui::NewLine();

			// FadeChange
			if (ImGui::Checkbox("FadeChange", &m_tParticleInfo.bFadeChange))
				bParticleSystemUse = true;
			ImGui::NewLine();
			if (m_tParticleInfo.bFadeChange)
			{
				ImGui::Text("FadeChangeStartDelay");
				if (ImGui::InputFloat2("##FadeChangeStartDelay", &m_tParticleInfo.fFadeChangeStartDelay.x))
					bParticleSystemUse = true;
				ImGui::NewLine();

				if (ImGui::Checkbox("FadeIn", &m_tParticleInfo.bFadeIn))
					bParticleSystemUse = true;
				ImGui::NewLine();
			}
		}

		// 쉐이더 값 셋팅
		if (ImGui::CollapsingHeader("Shader"))
		{
			// 쉐이더 패스
			ImGui::Text("ShaderPass");
			if (ImGui::InputInt("##ShaderPass", &(_int)m_tParticleInfo.iShaderPass))
				bParticleSystemUse = true;
			ImGui::NewLine();

			// 디스카드 값 셋팅
			ImGui::Text("Alpha_Discard");
			if (ImGui::InputFloat("##Alpha_Discard", &m_tParticleInfo.fAlpha_Discard))
				bParticleSystemUse = true;
			ImGui::Text("Black_Discard");
			if (ImGui::InputFloat3("##Black_Discard", &m_tParticleInfo.fBlack_Discard.x))
				bParticleSystemUse = true;
			ImGui::NewLine();

			// 블러 셋팅
			if (ImGui::Checkbox("BloomPowerRandom", &m_tParticleInfo.bBloomPowerRandom))
				bParticleSystemUse = true;
			if (!m_tParticleInfo.bBloomPowerRandom)
			{
				ImGui::Text("BloomPower");
				if (ImGui::ColorEdit4("##BloomPower", (float*)&m_tParticleInfo.fBloomPower, ImGuiColorEditFlags_Float))
					bParticleSystemUse = true;
				ImGui::NewLine();
			}

			if (ImGui::Checkbox("BlurPowerRandom", &m_tParticleInfo.bBlurPowerRandom))
				bParticleSystemUse = true;
			if (!m_tParticleInfo.bBlurPowerRandom)
			{
				if (ImGui::InputFloat("##BlurPower", &m_tParticleInfo.fBlurPower))
					bParticleSystemUse = true;
			}
		}

		// 박스 범위
		if (ImGui::CollapsingHeader("Box"))
		{
			if (ImGui::Checkbox("UseBox", &m_tParticleInfo.bUseBox))
				bParticleSystemUse = true;
			ImGui::NewLine();

			if (m_tParticleInfo.bUseBox)
			{
				ImGui::Text("BoxMinParticles");
				if (ImGui::InputFloat3("##BoxMinParticles", &m_tParticleInfo.fBoxMin.x))
					bParticleSystemUse = true;
				ImGui::Text("BoxMaxParticles");
				if (ImGui::InputFloat3("##BoxMaxParticles", &m_tParticleInfo.fBoxMax.x))
					bParticleSystemUse = true;
				ImGui::NewLine();
			}
		}

		// 리지드바디
		if (ImGui::CollapsingHeader("Rigidbody"))
		{
			if (ImGui::Checkbox("RigidbodyUse", &m_tRigidbodyInfo.bRigidbody))
				bParticleSystemUse = true;

			if (ImGui::Checkbox("Gravity", &m_tRigidbodyInfo.bGravity))
				bParticleSystemUse = true;

			if (ImGui::Checkbox("StopZero", &m_tRigidbodyInfo.bStopZero))
				bParticleSystemUse = true;

			if (ImGui::Checkbox("StopStartY", &m_tRigidbodyInfo.bStopStartY))
				bParticleSystemUse = true;

			if (ImGui::Checkbox("GroundSlide", &m_tRigidbodyInfo.bGroundSlide))
				bParticleSystemUse = true;
			ImGui::NewLine();
			ImGui::NewLine();

			if (ImGui::Checkbox("StartJump", &m_tRigidbodyInfo.bStartJump))
				bParticleSystemUse = true;

			ImGui::Text("StartMinVelocity");
			if (ImGui::InputFloat4("##StartMinVelocity", &m_tRigidbodyInfo.vStartMinVelocity.x))
				bParticleSystemUse = true;

			ImGui::Text("StartMaxVelocity");
			if (ImGui::InputFloat4("##StartMaxVelocity", &m_tRigidbodyInfo.vStartMaxVelocity.x))
				bParticleSystemUse = true;

			ImGui::Text("MaxVelocity");
			if (ImGui::InputFloat4("##MaxVelocity", &m_tRigidbodyInfo.vMaxVelocity.x))
				bParticleSystemUse = true;

			ImGui::Text("Mass");
			if (ImGui::InputFloat("##Mass", &m_tRigidbodyInfo.fMass))
				bParticleSystemUse = true;

			ImGui::Text("FricCoeff");
			if (ImGui::InputFloat("##FricCoeff", &m_tRigidbodyInfo.fFricCoeff))
				bParticleSystemUse = true;
			ImGui::NewLine();
			ImGui::NewLine();

			ImGui::Text("## No Save/ Only Test ##");
			ImGui::Text("RigidMinVelocity");
			ImGui::InputFloat4("##RigidMinVelocity", &m_vMinVelocity.x);
			ImGui::Text("RigidMaxVelocity");
			ImGui::InputFloat4("##RigidMaxVelocity", &m_vMaxVelocity.x);
			if (ImGui::Button("AddVelocity"))
			{
				if (nullptr != m_pParticle && m_tRigidbodyInfo.bRigidbody)
					static_cast<CParticle*>(m_pParticle)->Add_Velocity(m_vMinVelocity, m_vMaxVelocity);
			}
			ImGui::NewLine();
		}
	}
#pragma endregion

	if (bParticleSystemUse)
		Store_InfoParticle();

	ImGui::End();
}

void CTool_Particle::Tick_CreateDelete()
{
	if (ImGui::Button("Create"))
		Create_Particle();
	ImGui::SameLine();

	if (ImGui::Button("Delete"))
	{
		if (m_pParticle != nullptr)
		{
			m_pParticle->Set_Dead(true);
			m_pParticle = nullptr;
		}
	}
}

void CTool_Particle::Tick_Transform()
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		if (ImGui::Button("OK"))
		{
			if (nullptr != m_pParticle)
			{
				CTransform* pTransform = m_pParticle->Get_Component<CTransform>(L"Com_Transform");
				if (nullptr != pTransform)
				{
					_matrix IdentityMatrix = XMMatrixIdentity();
					_float4x4 WorldMatrix;
					XMStoreFloat4x4(&WorldMatrix, IdentityMatrix);
					WorldMatrix.m[3][0] = m_fPosition.x;
					WorldMatrix.m[3][1] = m_fPosition.y;
					WorldMatrix.m[3][2] = m_fPosition.z;
					static_cast<CParticle*>(m_pParticle)->Set_Position_Particle(WorldMatrix);

					pTransform->FixRotation(m_fRotation.x, m_fRotation.y, m_fRotation.z);
					pTransform->Set_Scale(m_fScale);
				}
			}
		}

		ImGui::Text("Position");
		ImGui::InputFloat3("##Position", &m_fPosition.x);

		ImGui::Text("Rotation");
		ImGui::InputFloat3("##Rotation", &m_fRotation.x);

		ImGui::Text("Scale");
		ImGui::InputFloat3("##Scale", &m_fScale.x);
	}
}

void CTool_Particle::Tick_Prototype()
{
	if (ImGui::CollapsingHeader("Prototype"))
	{
		if (ImGui::Button("Set_PrototypeObject"))
			Set_OriginalInfoParticle();
		ImGui::SameLine();

		ImGui::Text("PrototypeObject_Name :");
		ImGui::SameLine();
		ImGui::InputText("##PrototypeObject_Name", m_cPrototypeName, IM_ARRAYSIZE(m_cPrototypeName));
	}
}

void CTool_Particle::Tick_Binary()
{
	if (ImGui::CollapsingHeader("Binary"))
	{
		if (ImGui::Button("Save"))
			Save_Particle(m_cSaveAndLoadName);
		ImGui::SameLine();

		if (ImGui::Button("Load"))
			Load_Particle(m_cSaveAndLoadName);
		ImGui::SameLine();

		ImGui::Text("FileName :");
		ImGui::SameLine();
		ImGui::InputText("##FileName", m_cSaveAndLoadName, IM_ARRAYSIZE(m_cSaveAndLoadName));
	}
}
#pragma endregion


void CTool_Particle::Create_Particle()
{
	if (m_pParticle != nullptr)
		return;

	m_pParticle = GI->Clone_GameObject(TEXT("Prototype_TempParticle"), LAYER_TYPE::LAYER_EFFECT);
	GI->Add_GameObject(LEVEL_TOOL, LAYER_TYPE::LAYER_EFFECT, m_pParticle);

	Load_InfoParticle();
}

void CTool_Particle::Load_InfoParticle()
{
	if (nullptr == m_pParticle)
		return;

	CTransform* pTransform = m_pParticle->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr != pTransform)
	{
		_vector vPosition = pTransform->Get_State(CTransform::STATE_POSITION);
		m_fPosition.x = XMVectorGetX(vPosition);
		m_fPosition.y = XMVectorGetY(vPosition);
		m_fPosition.z = XMVectorGetZ(vPosition);

		_vector vRotation = pTransform->Get_WorldRotation();
		m_fRotation.x = XMVectorGetX(vRotation);
		m_fRotation.y = XMVectorGetY(vRotation);
		m_fRotation.z = XMVectorGetZ(vRotation);

		m_fScale = pTransform->Get_Scale();
	}

	m_tParticleInfo = static_cast<CParticle*>(m_pParticle)->Get_ParticleDesc();

	if (m_tParticleInfo.eParticleType == CParticle::TYPE_PERSPECTIVE)
		m_iProjectionIndex = 0;
	else if (m_tParticleInfo.eParticleType == CParticle::TYPE_ORTHOGRAPHIC)
		m_iProjectionIndex = 1;
	else
		m_iProjectionIndex = 2;

	m_fParticleAxis.x = XMVectorGetX(m_tParticleInfo.vAxis);
	m_fParticleAxis.y = XMVectorGetY(m_tParticleInfo.vAxis);
	m_fParticleAxis.z = XMVectorGetZ(m_tParticleInfo.vAxis);

	m_iDiffuseFolderIndex = Get_FolderIndex(m_tParticleInfo.strDiffuseTetextureName);
	m_iAlphaFolderIndex = Get_FolderIndex(m_tParticleInfo.strAlphaTexturName);

	m_tRigidbodyInfo = static_cast<CParticle*>(m_pParticle)->Get_RigidbodyDesc();
}

void CTool_Particle::Store_InfoParticle()
{
	if (m_pParticle == nullptr)
		return;

	if (m_iProjectionIndex == 0)
		m_tParticleInfo.eParticleType = CParticle::TYPE_PERSPECTIVE;
	else if (m_iProjectionIndex == 1)
		m_tParticleInfo.eParticleType = CParticle::TYPE_ORTHOGRAPHIC;
	else
		m_tParticleInfo.eParticleType = CParticle::TYPE_END;

	m_tParticleInfo.vAxis = XMVectorSet(m_fParticleAxis.x, m_fParticleAxis.y, m_fParticleAxis.z, 0.f);

	m_tParticleInfo.strDiffuseTetextureName = Select_FolderName(m_iDiffuseFolderIndex);
	m_tParticleInfo.strAlphaTexturName = Select_FolderName(m_iAlphaFolderIndex);

	static_cast<CParticle*>(m_pParticle)->Set_ParticleDesc(m_tParticleInfo);
	static_cast<CParticle*>(m_pParticle)->Set_RigidbodyDesc(m_tRigidbodyInfo);
}

void CTool_Particle::Set_OriginalInfoParticle()
{
	if (nullptr == m_pParticle)
		return;

	wstring strPropertyName(m_cPrototypeName, m_cPrototypeName + strlen(m_cPrototypeName));

	CGameObject* pGameObject = GI->Find_Prototype_GameObject(LAYER_TYPE::LAYER_EFFECT, strPropertyName);
	if (pGameObject == nullptr)
	{
		MSG_BOX("Prototype_Find_Failed!");
		return;
	}

	if (m_iProjectionIndex == 0)
		m_tParticleInfo.eParticleType = CParticle::TYPE_PERSPECTIVE;
	else if (m_iProjectionIndex == 1)
		m_tParticleInfo.eParticleType = CParticle::TYPE_ORTHOGRAPHIC;
	else
		m_tParticleInfo.eParticleType = CParticle::TYPE_END;

	m_tParticleInfo.strDiffuseTetextureName = Select_FolderName(m_iDiffuseFolderIndex);
	m_tParticleInfo.strAlphaTexturName = Select_FolderName(m_iAlphaFolderIndex);

	static_cast<CParticle*>(pGameObject)->Set_ParticleDesc(m_tParticleInfo);
	static_cast<CParticle*>(pGameObject)->Set_RigidbodyDesc(m_tRigidbodyInfo);
}

wstring CTool_Particle::Select_FolderName(_uint iFolderIndex)
{
	wstring ProtoName;

	switch (iFolderIndex)
	{
	case 0:
		ProtoName = L"Prototype_Component_Texture_Effect_Aura";
		break;
	case 1:
		ProtoName = L"Prototype_Component_Texture_Effect_Crack";
		break;
	case 2:
		ProtoName = L"Prototype_Component_Texture_Effect_Decal";
		break;
	case 3:
		ProtoName = L"Prototype_Component_Texture_Effect_Fire";
		break;
	case 4:
		ProtoName = L"Prototype_Component_Texture_Effect_Flare";
		break;
	case 5:
		ProtoName = L"Prototype_Component_Texture_Effect_Glow";
		break;
	case 6:
		ProtoName = L"Prototype_Component_Texture_Effect_Hit";
		break;
	case 7:
		ProtoName = L"Prototype_Component_Texture_Effect_Ice";
		break;
	case 8:
		ProtoName = L"Prototype_Component_Texture_Effect_Image";
		break;
	case 9:
		ProtoName = L"Prototype_Component_Texture_Effect_Mask";
		break;
	case 10:
		ProtoName = L"Prototype_Component_Texture_Effect_MeshTrail";
		break;
	case 11:
		ProtoName = L"Prototype_Component_Texture_Effect_Noise";
		break;
	case 12:
		ProtoName = L"Prototype_Component_Texture_Effect_Object";
		break;
	case 13:
		ProtoName = L"Prototype_Component_Texture_Effect_Ring";
		break;
	case 14:
		ProtoName = L"Prototype_Component_Texture_Effect_Slash";
		break;
	case 15:
		ProtoName = L"Prototype_Component_Texture_Effect_Smoke";
		break;
	case 16:
		ProtoName = L"Prototype_Component_Texture_Effect_SubUV";
		break;
	case 17:
		ProtoName = L"Prototype_Component_Texture_Effect_SwordTrail";
		break;
	case 18:
		ProtoName = L"Prototype_Component_Texture_Effect_UI";
		break;
	case 19:
		ProtoName = L"Prototype_Component_Texture_Effect_Water";
		break;
	case 20:
		ProtoName = L"Prototype_Component_Texture_Effect_Wind";
		break;
	case 21:
		ProtoName = L"";
		break;
	}

	return ProtoName;
}

_uint CTool_Particle::Get_FolderIndex(wstring& strName)
{
	_int iIndex = -1;

	_int  iBufferSizeName = WideCharToMultiByte(CP_UTF8, 0, strName.c_str(), -1, nullptr, 0, nullptr, nullptr);
	char* pszName = new char[iBufferSizeName];
	WideCharToMultiByte(CP_UTF8, 0, strName.c_str(), -1, pszName, iBufferSizeName, nullptr, nullptr);

	if (strcmp(pszName, "Prototype_Component_Texture_Effect_Aura") == 0)
		iIndex = 0;
	else if (strcmp(pszName, "Prototype_Component_Texture_Effect_Crack") == 0)
		iIndex = 1;
	else if (strcmp(pszName, "Prototype_Component_Texture_Effect_Decal") == 0)
		iIndex = 2;
	else if (strcmp(pszName, "Prototype_Component_Texture_Effect_Fire") == 0)
		iIndex = 3;
	else if (strcmp(pszName, "Prototype_Component_Texture_Effect_Flare") == 0)
		iIndex = 4;
	else if (strcmp(pszName, "Prototype_Component_Texture_Effect_Glow") == 0)
		iIndex = 5;
	else if (strcmp(pszName, "Prototype_Component_Texture_Effect_Hit") == 0)
		iIndex = 6;
	else if (strcmp(pszName, "Prototype_Component_Texture_Effect_Ice") == 0)
		iIndex = 7;
	else if (strcmp(pszName, "Prototype_Component_Texture_Effect_Image") == 0)
		iIndex = 8;
	else if (strcmp(pszName, "Prototype_Component_Texture_Effect_Mask") == 0)
		iIndex = 9;
	else if (strcmp(pszName, "Prototype_Component_Texture_Effect_MeshTrail") == 0)
		iIndex = 10;
	else if (strcmp(pszName, "Prototype_Component_Texture_Effect_Noise") == 0)
		iIndex = 11;
	else if (strcmp(pszName, "Prototype_Component_Texture_Effect_Object") == 0)
		iIndex = 12;
	else if (strcmp(pszName, "Prototype_Component_Texture_Effect_Ring") == 0)
		iIndex = 13;
	else if (strcmp(pszName, "Prototype_Component_Texture_Effect_Slash") == 0)
		iIndex = 14;
	else if (strcmp(pszName, "Prototype_Component_Texture_Effect_Smoke") == 0)
		iIndex = 15;
	else if (strcmp(pszName, "Prototype_Component_Texture_Effect_SubUV") == 0)
		iIndex = 16;
	else if (strcmp(pszName, "Prototype_Component_Texture_Effect_SwordTrail") == 0)
		iIndex = 17;
	else if (strcmp(pszName, "Prototype_Component_Texture_Effect_UI") == 0)
		iIndex = 18;
	else if (strcmp(pszName, "Prototype_Component_Texture_Effect_Water") == 0)
		iIndex = 19;
	else if (strcmp(pszName, "Prototype_Component_Texture_Effect_Wind") == 0)
		iIndex = 20;
	else
		iIndex = 21;
	Safe_Delete(pszName);

	return iIndex;
}


void CTool_Particle::Save_Particle(const char* pFileName)
{
	if (m_pParticle == nullptr)
	{
		MSG_BOX("Particle_Save_Failed!");
		return;
	}

	//m_tParticleInfo = static_cast<CParticle*>(m_pParticle)->Get_ParticleDesc();

	wstring strFileName(pFileName, pFileName + strlen(pFileName));
	wstring strFilePath = L"../Bin/DataFiles/Vfx/Particle/" + strFileName + L".Particle";

#pragma region Save_Particle
	auto path = filesystem::path(strFilePath);
	filesystem::create_directories(path.parent_path());

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strFilePath, FileMode::Write);

	// 파티클 타입
	File->Write<_uint>((_uint)m_tParticleInfo.eParticleType);

	// 정렬 설정
	File->Write<_bool>(m_tParticleInfo.bParticleSortZ);

	// 반복 여부
	File->Write<_bool>(m_tParticleInfo.bParticleLoop);

	// 파티클 개수
	if (m_tParticleInfo.iNumEffectMaxCount > m_tParticleInfo.iNumEffectCount)
		m_tParticleInfo.iNumEffectMaxCount = m_tParticleInfo.iNumEffectCount;
	File->Write<_uint>(m_tParticleInfo.iNumEffectMaxCount);
	File->Write<_uint>(m_tParticleInfo.iNumEffectCount);

	// 분포 범위
	File->Write<_float3>(m_tParticleInfo.fRange);
	File->Write<_float2>(m_tParticleInfo.fRangeDistance);

#pragma region 크기
	File->Write<_bool>(m_tParticleInfo.bScaleSameRate);
	File->Write<_float2>(m_tParticleInfo.fScaleStart);

	File->Write<_bool>(m_tParticleInfo.bScaleChange);
	File->Write<_float2>(m_tParticleInfo.fScaleChangeStartDelay);

	File->Write<_bool>(m_tParticleInfo.bScaleChangeRandom);
	File->Write<_float2>(m_tParticleInfo.fScaleChangeTime);

	File->Write<_bool>(m_tParticleInfo.bScaleAdd);
	File->Write<_bool>(m_tParticleInfo.bScaleLoop);
	File->Write<_bool>(m_tParticleInfo.bScaleLoopStart);

	File->Write<_float2>(m_tParticleInfo.fScaleMin);
	File->Write<_float2>(m_tParticleInfo.fScaleMax);
	File->Write<_float2>(m_tParticleInfo.fScaleSpeed);
#pragma endregion

#pragma region 이동
	File->Write<_float2>(m_tParticleInfo.fVelocitySpeed);

	File->Write<_float3>(m_tParticleInfo.vVelocityMinStart);
	File->Write<_float3>(m_tParticleInfo.vVelocityMaxStart);

	File->Write<_bool>(m_tParticleInfo.bVelocityChange);
	File->Write<_float2>(m_tParticleInfo.fVelocityChangeStartDelay);

	File->Write<_bool>(m_tParticleInfo.bVelocityChangeRandom);
	File->Write<_float2>(m_tParticleInfo.fVelocityChangeTime);

	File->Write<_bool>(m_tParticleInfo.bVelocityLoop);
	File->Write<_uint>(m_tParticleInfo.iVelocityCountCur);
	File->Write<_uint>(m_tParticleInfo.iVelocityCountMax);

	// 
	if (!m_tParticleInfo.bVelocityChange || m_tParticleInfo.bVelocityChangeRandom)
		m_tParticleInfo.iVelocityUse = 0;

	File->Write<_uint>(m_tParticleInfo.iVelocityUse);
	if (m_tParticleInfo.iVelocityUse > 0)
	{
		if (m_tParticleInfo.pVelocityMin != nullptr)
		{
			for (size_t i = 0; i < m_tParticleInfo.iVelocityUse; ++i)
				File->Write<_float3>(m_tParticleInfo.pVelocityMin[i]);
		}

		if (m_tParticleInfo.pVelocityMax != nullptr)
		{
			for (size_t i = 0; i < m_tParticleInfo.iVelocityUse; ++i)
				File->Write<_float3>(m_tParticleInfo.pVelocityMax[i]);
		}

		if (m_tParticleInfo.pVelocityTime != nullptr)
		{
			for (size_t i = 0; i < m_tParticleInfo.iVelocityUse; ++i)
				File->Write<_float2>(m_tParticleInfo.pVelocityTime[i]);
		}
	}
#pragma endregion

#pragma region 회전
	File->Write<_bool>(m_tParticleInfo.bBillboard);

	File->Write<_bool>(m_tParticleInfo.bRandomAxis);
	File->Write<_vector>(m_tParticleInfo.vAxis);

	File->Write<_bool>(m_tParticleInfo.bRandomAngle);
	File->Write<_float>(m_tParticleInfo.fAngle);

	File->Write<_bool>(m_tParticleInfo.bRotationChange);
	File->Write<_float2>(m_tParticleInfo.fRotationChangeStartDelay);

	File->Write<_float2>(m_tParticleInfo.fRotationSpeed);

	File->Write<_bool>(m_tParticleInfo.bRotationChangeRandom);
	File->Write<_float2>(m_tParticleInfo.fRotationChangeTime);

	File->Write<_bool>(m_tParticleInfo.bRotationAdd);
#pragma endregion

	// 지속 시간
	File->Write<_float2>(m_tParticleInfo.fLifeTime);

	// 박스 범위
	File->Write<_bool>(m_tParticleInfo.bUseBox);
	File->Write<_float3>(m_tParticleInfo.fBoxMin);
	File->Write<_float3>(m_tParticleInfo.fBoxMax);

#pragma region 텍스처
	// 문자열 저장
	File->Write<string>(CUtils::ToString(m_tParticleInfo.strDiffuseTetextureName));
	File->Write<string>(CUtils::ToString(m_tParticleInfo.strDiffuseTetexturePath));
	File->Write<string>(CUtils::ToString(m_tParticleInfo.strAlphaTexturName));
	File->Write<string>(CUtils::ToString(m_tParticleInfo.strAlphaTexturPath));

	File->Write<_bool>(m_tParticleInfo.bRandomStartIndex);
	File->Write<_float2>(m_tParticleInfo.fUVIndex);
	File->Write<_float2>(m_tParticleInfo.fUVMaxCount);

	File->Write<_uint>(m_tParticleInfo.iTextureIndexDiffuse);
	File->Write<_uint>(m_tParticleInfo.iTextureIndexAlpha);
#pragma endregion

#pragma region 애니메이션
	File->Write<_bool>(m_tParticleInfo.bAnimation);
	File->Write<_bool>(m_tParticleInfo.bAnimationLoop);
	File->Write<_float2>(m_tParticleInfo.fAnimationSpeed);
#pragma endregion

#pragma region 알파
	File->Write<_float2>(m_tParticleInfo.fStartAlpha);

	File->Write<_bool>(m_tParticleInfo.bFadeCreate);
	File->Write<_bool>(m_tParticleInfo.bFadeDelete);
	File->Write<_float2>(m_tParticleInfo.fFadeSpeed);

	File->Write<_bool>(m_tParticleInfo.bFadeChange);
	File->Write<_bool>(m_tParticleInfo.bFadeIn);
	File->Write<_float2>(m_tParticleInfo.fFadeChangeStartDelay);
#pragma endregion

#pragma region 색상
	File->Write<_bool>(m_tParticleInfo.bColorRandom);
	File->Write<_float4>(m_tParticleInfo.vColorS);

	File->Write<_bool>(m_tParticleInfo.bColorChange);

	File->Write<_bool>(m_tParticleInfo.bColorChangeRandom);
	File->Write<_float2>(m_tParticleInfo.fColorChangeRandomTime);

	File->Write<_bool>(m_tParticleInfo.bColorLoop);
	File->Write<_float2>(m_tParticleInfo.fColorChangeStartDelay);

	File->Write<_float2>(m_tParticleInfo.fColorChangeStartM);
	File->Write<_float4>(m_tParticleInfo.fColorM);

	File->Write<_float2>(m_tParticleInfo.fColorChangeStartF);
	File->Write<_float4>(m_tParticleInfo.fColorF);

	File->Write<_float2>(m_tParticleInfo.fColorDuration);
#pragma endregion

#pragma region 블러
	File->Write<_bool>(m_tParticleInfo.bBloomPowerRandom);
	File->Write<_float4>(m_tParticleInfo.fBloomPower);
	File->Write<_bool>(m_tParticleInfo.bBlurPowerRandom);
	File->Write<_float>(m_tParticleInfo.fBlurPower);
#pragma endregion

#pragma region 기타 정보
	File->Write<_uint>(m_tParticleInfo.iShaderPass);
	File->Write<_float>(m_tParticleInfo.fAlpha_Discard);
	File->Write<_float3>(m_tParticleInfo.fBlack_Discard);
#pragma endregion

	// 이후 추가 시 기존 정보 로드 후 추가 값 셋팅 후 재저장하기

#pragma endregion

	MSG_BOX("Particle_Save_Success!");

	if (m_tRigidbodyInfo.bRigidbody)
		Save_Rigidbody(pFileName);
}

void CTool_Particle::Save_Rigidbody(const char* pFileName)
{
	if (m_pParticle == nullptr)
	{
		MSG_BOX("Particle_Rigidbody_Save_Failed!");
		return;
	}

	wstring strFileName_Rigidbody(pFileName, pFileName + strlen(pFileName));
	wstring strFilePath_Rigidbody = L"../Bin/DataFiles/Vfx/Particle/" + strFileName_Rigidbody + L".Rigidbody";

#pragma region Save_Particle
	auto path = filesystem::path(strFilePath_Rigidbody);
	filesystem::create_directories(path.parent_path());

	shared_ptr<CFileUtils> File_Rigidbody = make_shared<CFileUtils>();
	File_Rigidbody->Open(strFilePath_Rigidbody, FileMode::Write);

	File_Rigidbody->Write<_bool>(m_tRigidbodyInfo.bRigidbody);
	File_Rigidbody->Write<_bool>(m_tRigidbodyInfo.bGravity);
	File_Rigidbody->Write<_bool>(m_tRigidbodyInfo.bStopZero);
	File_Rigidbody->Write<_bool>(m_tRigidbodyInfo.bStopStartY);
	File_Rigidbody->Write<_bool>(m_tRigidbodyInfo.bGroundSlide);

	File_Rigidbody->Write<_bool>(m_tRigidbodyInfo.bStartJump);
	File_Rigidbody->Write<Vec4>(m_tRigidbodyInfo.vStartMinVelocity);
	File_Rigidbody->Write<Vec4>(m_tRigidbodyInfo.vStartMaxVelocity);

	File_Rigidbody->Write<Vec4>(m_tRigidbodyInfo.vMaxVelocity);
	File_Rigidbody->Write<_float>(m_tRigidbodyInfo.fMass);
	File_Rigidbody->Write<_float>(m_tRigidbodyInfo.fFricCoeff);

	MSG_BOX("Particle_Rigidbody_Save_Success!");
}

void CTool_Particle::Load_Particle(const char* pFileName)
{
	if (m_pParticle != nullptr)
	{
		m_pParticle->Set_Dead(true);
		m_pParticle = nullptr;
	}
	Create_Particle();

	wstring strFileName(pFileName, pFileName + strlen(pFileName));
	wstring strParticlePath = L"../Bin/DataFiles/Vfx/Particle/" + strFileName + L".Particle";

#pragma region Basic_Load
	CParticle::PARTICLE_DESC ParticleInfo = {};

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strParticlePath, FileMode::Read);

	// 파티클 타입
	_uint iParticleType = 0;
	File->Read<_uint>(iParticleType);
	ParticleInfo.eParticleType = (CParticle::PARTICLEPROJTYPE)iParticleType;

	// 정렬 설정
	File->Read<_bool>(ParticleInfo.bParticleSortZ);

	// 반복 여부
	File->Read<_bool>(ParticleInfo.bParticleLoop);

	// 파티클 개수
	File->Read<_uint>(ParticleInfo.iNumEffectMaxCount);
	File->Read<_uint>(ParticleInfo.iNumEffectCount);

	// 분포 범위
	File->Read<_float3>(ParticleInfo.fRange);
	File->Read<_float2>(ParticleInfo.fRangeDistance);

#pragma region 크기
	File->Read<_bool>(ParticleInfo.bScaleSameRate);
	File->Read<_float2>(ParticleInfo.fScaleStart);

	File->Read<_bool>(ParticleInfo.bScaleChange);
	File->Read<_float2>(ParticleInfo.fScaleChangeStartDelay);

	File->Read<_bool>(ParticleInfo.bScaleChangeRandom);
	File->Read<_float2>(ParticleInfo.fScaleChangeTime);

	File->Read<_bool>(ParticleInfo.bScaleAdd);
	File->Read<_bool>(ParticleInfo.bScaleLoop);
	File->Read<_bool>(ParticleInfo.bScaleLoopStart);

	File->Read<_float2>(ParticleInfo.fScaleMin);
	File->Read<_float2>(ParticleInfo.fScaleMax);
	File->Read<_float2>(ParticleInfo.fScaleSpeed);
#pragma endregion

#pragma region 이동
	File->Read<_float2>(ParticleInfo.fVelocitySpeed);

	File->Read<_float3>(ParticleInfo.vVelocityMinStart);
	File->Read<_float3>(ParticleInfo.vVelocityMaxStart);

	File->Read<_bool>(ParticleInfo.bVelocityChange);
	File->Read<_float2>(ParticleInfo.fVelocityChangeStartDelay);

	File->Read<_bool>(ParticleInfo.bVelocityChangeRandom);
	File->Read<_float2>(ParticleInfo.fVelocityChangeTime);

	File->Read<_bool>(ParticleInfo.bVelocityLoop);
	File->Read<_uint>(ParticleInfo.iVelocityCountCur);
	File->Read<_uint>(ParticleInfo.iVelocityCountMax);

	File->Read<_uint>(ParticleInfo.iVelocityUse);
	if (ParticleInfo.iVelocityUse > 0)
	{
		ParticleInfo.pVelocityMin = new _float3[ParticleInfo.iVelocityUse];
		ParticleInfo.pVelocityMax = new _float3[ParticleInfo.iVelocityUse];
		ParticleInfo.pVelocityTime = new _float2[ParticleInfo.iVelocityUse];
		if (ParticleInfo.pVelocityMin != nullptr)
		{
			for (size_t i = 0; i < ParticleInfo.iVelocityUse; ++i)
				File->Read<_float3>(ParticleInfo.pVelocityMin[i]);
		}

		if (ParticleInfo.pVelocityMax != nullptr)
		{
			for (size_t i = 0; i < ParticleInfo.iVelocityUse; ++i)
				File->Read<_float3>(ParticleInfo.pVelocityMax[i]);
		}

		if (ParticleInfo.pVelocityTime != nullptr)
		{
			for (size_t i = 0; i < ParticleInfo.iVelocityUse; ++i)
				File->Read<_float2>(ParticleInfo.pVelocityTime[i]);
		}
	}
#pragma endregion

#pragma region 회전
	File->Read<_bool>(ParticleInfo.bBillboard);

	File->Read<_bool>(ParticleInfo.bRandomAxis);
	File->Read<_vector>(ParticleInfo.vAxis);

	File->Read<_bool>(ParticleInfo.bRandomAngle);
	File->Read<_float>(ParticleInfo.fAngle);

	File->Read<_bool>(ParticleInfo.bRotationChange);
	File->Read<_float2>(ParticleInfo.fRotationChangeStartDelay);

	File->Read<_float2>(ParticleInfo.fRotationSpeed);

	File->Read<_bool>(ParticleInfo.bRotationChangeRandom);
	File->Read<_float2>(ParticleInfo.fRotationChangeTime);

	File->Read<_bool>(ParticleInfo.bRotationAdd);
#pragma endregion

	// 지속 시간
	File->Read<_float2>(ParticleInfo.fLifeTime);

	// 박스 범위
	File->Read<_bool>(ParticleInfo.bUseBox);
	File->Read<_float3>(ParticleInfo.fBoxMin);
	File->Read<_float3>(ParticleInfo.fBoxMax);

#pragma region 텍스처
	// 문자열
	ParticleInfo.strDiffuseTetextureName = CUtils::ToWString(File->Read<string>());
	ParticleInfo.strDiffuseTetexturePath = CUtils::ToWString(File->Read<string>());
	ParticleInfo.strAlphaTexturName = CUtils::ToWString(File->Read<string>());
	ParticleInfo.strAlphaTexturPath = CUtils::ToWString(File->Read<string>());

	File->Read<_bool>(ParticleInfo.bRandomStartIndex);
	File->Read<_float2>(ParticleInfo.fUVIndex);
	File->Read<_float2>(ParticleInfo.fUVMaxCount);

	File->Read<_uint>(ParticleInfo.iTextureIndexDiffuse);
	File->Read<_uint>(ParticleInfo.iTextureIndexAlpha);
#pragma endregion

#pragma region 애니메이션
	File->Read<_bool>(ParticleInfo.bAnimation);
	File->Read<_bool>(ParticleInfo.bAnimationLoop);
	File->Read<_float2>(ParticleInfo.fAnimationSpeed);
#pragma endregion

#pragma region 알파
	File->Read<_float2>(ParticleInfo.fStartAlpha);

	File->Read<_bool>(ParticleInfo.bFadeCreate);
	File->Read<_bool>(ParticleInfo.bFadeDelete);
	File->Read<_float2>(ParticleInfo.fFadeSpeed);

	File->Read<_bool>(ParticleInfo.bFadeChange);
	File->Read<_bool>(ParticleInfo.bFadeIn);
	File->Read<_float2>(ParticleInfo.fFadeChangeStartDelay);
#pragma endregion

#pragma region 색상
	File->Read<_bool>(ParticleInfo.bColorRandom);
	File->Read<_float4>(ParticleInfo.vColorS);

	File->Read<_bool>(ParticleInfo.bColorChange);

	File->Read<_bool>(ParticleInfo.bColorChangeRandom);
	File->Read<_float2>(ParticleInfo.fColorChangeRandomTime);

	File->Read<_bool>(ParticleInfo.bColorLoop);
	File->Read<_float2>(ParticleInfo.fColorChangeStartDelay);

	File->Read<_float2>(ParticleInfo.fColorChangeStartM);
	File->Read<_float4>(ParticleInfo.fColorM);

	File->Read<_float2>(ParticleInfo.fColorChangeStartF);
	File->Read<_float4>(ParticleInfo.fColorF);

	File->Read<_float2>(ParticleInfo.fColorDuration);
#pragma endregion

#pragma region 블러
	File->Read<_bool>(ParticleInfo.bBloomPowerRandom);
	File->Read<_float4>(ParticleInfo.fBloomPower);
	File->Read<_bool>(ParticleInfo.bBlurPowerRandom);
	File->Read<_float>(ParticleInfo.fBlurPower);
#pragma endregion

#pragma region 기타 정보
	File->Read<_uint>(ParticleInfo.iShaderPass);
	File->Read<_float>(ParticleInfo.fAlpha_Discard);
	File->Read<_float3>(ParticleInfo.fBlack_Discard);
#pragma endregion

	static_cast<CParticle*>(m_pParticle)->Set_ParticleDesc(ParticleInfo);
	m_tParticleInfo = ParticleInfo;
#pragma endregion

#pragma region Rigidbody_Load
	wstring strRigidbodyPath = L"../Bin/DataFiles/Vfx/Particle/" + strFileName + L".Rigidbody";

	CParticle::PARTICLE_RIGIDBODY_DESC tRigidbodyDesc = {};
	if (true == filesystem::exists(strRigidbodyPath)) // 해당 파일이 존재
	{
		shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
		File->Open(strRigidbodyPath, FileMode::Read);

		File->Read<_bool>(tRigidbodyDesc.bRigidbody);
		File->Read<_bool>(tRigidbodyDesc.bGravity);
		File->Read<_bool>(tRigidbodyDesc.bStopZero);
		File->Read<_bool>(tRigidbodyDesc.bStopStartY);
		File->Read<_bool>(tRigidbodyDesc.bGroundSlide);

		File->Read<_bool>(tRigidbodyDesc.bStartJump);
		File->Read<Vec4>(tRigidbodyDesc.vStartMinVelocity);
		File->Read<Vec4>(tRigidbodyDesc.vStartMaxVelocity);

		File->Read<Vec4>(tRigidbodyDesc.vMaxVelocity);
		File->Read<_float>(tRigidbodyDesc.fMass);
		File->Read<_float>(tRigidbodyDesc.fFricCoeff);

		static_cast<CParticle*>(m_pParticle)->Set_RigidbodyDesc(tRigidbodyDesc);
		m_tRigidbodyInfo = tRigidbodyDesc;
	}
#pragma endregion

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
