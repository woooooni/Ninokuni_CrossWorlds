#include "stdafx.h"
#include "Tool_Effect.h"
#include "imgui.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "filesystem"
#include "FileUtils.h"
#include "Utils.h"
#include <Vfx.h>

CTool_Effect::CTool_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CTool(pDevice, pContext)
{
}

HRESULT CTool_Effect::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}


#pragma region Tick
void CTool_Effect::Tick(_float fTimeDelta)
{
	ImGui::Begin("Effect_Tool");
	ImGui::BeginTabBar("Effect_Tab");

	if (ImGui::BeginTabItem("Effect"))
	{
		ImGui::NewLine();
		if (Tick_EffectTool())
			Store_ObjectInfo(TYPE_EFFECT);
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Decal"))
	{
		ImGui::NewLine();
		if(Tick_DecalTool())
			Store_ObjectInfo(TYPE_DECAL);
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("VFX"))
	{
		ImGui::NewLine();
		Tick_VfxTool();
		ImGui::EndTabItem();
	}

	ImGui::EndTabBar();
	ImGui::End();
}

_bool CTool_Effect::Tick_EffectTool()
{
	// CreateDelete
	Tick_CreateDelete(TYPE_EFFECT);
	ImGui::NewLine();

	// Binary
	Tick_Binary(TYPE_EFFECT);
	ImGui::NewLine();

	// Prototype
	Tick_Prototype(TYPE_EFFECT);
	ImGui::NewLine();

	// Transform
	Tick_Transform(TYPE_EFFECT);
	ImGui::NewLine();

#pragma region EffectSystem
	_bool bEffectSystemUse = false;

	if (ImGui::CollapsingHeader("EffectSystem"))
	{
		if (ImGui::Button("Restart"))
			Store_ObjectInfo(TYPE_EFFECT);

		// 기본 정보
		if (ImGui::CollapsingHeader("BasicInfo"))
		{
			// 이펙트 타입
			ImGui::Text("EffectType :");
			ImGui::SameLine();
			if (ImGui::Combo("##EffectType", &m_iEffectTypeIndex, m_cEffectType, IM_ARRAYSIZE(m_cEffectType)))
				bEffectSystemUse = true;

			// 증력 여부
			if(ImGui::Checkbox("Gravity", &m_tEffectInfo.bGravity))
				bEffectSystemUse = true;
			ImGui::NewLine();
		}

		// 위치 (분포 범위)
		if (ImGui::CollapsingHeader("Range/StartPosition"))
		{
			ImGui::Text("Range");
			if(ImGui::InputFloat3("##Range", &m_tEffectInfo.fRange.x))
				bEffectSystemUse = true;
			ImGui::NewLine();

			// 시작 거리
			ImGui::Text("StartDistance");
			if (ImGui::InputFloat2("##StartDistance", &m_tEffectInfo.fRangeDistance.x))
				bEffectSystemUse = true;
			ImGui::NewLine();
		}

		// 크기
		if (ImGui::CollapsingHeader("Scale"))
		{
			if(ImGui::Checkbox("ScaleSameRate", &m_tEffectInfo.bScaleSameRate))
				bEffectSystemUse = true;
			ImGui::NewLine();

			ImGui::Text("ScaleStartMin");
			if(ImGui::InputFloat3("##ScaleStartMin", &m_tEffectInfo.fScaleStartMin.x))
				bEffectSystemUse = true;
			ImGui::Text("ScaleStartMax");
			if(ImGui::InputFloat3("##ScaleStartMax", &m_tEffectInfo.fScaleStartMax.x))
				bEffectSystemUse = true;
			ImGui::NewLine();
			ImGui::NewLine();


			if(ImGui::Checkbox("ScaleChange", &m_tEffectInfo.bScaleChange))
				bEffectSystemUse = true;
			ImGui::NewLine();

			if (m_tEffectInfo.bScaleChange)
			{
				ImGui::Text("ScaleChangeStartDelay");
				if(ImGui::InputFloat2("##ScaleChangeStartDelay", &m_tEffectInfo.fScaleChangeStartDelay.x))
					bEffectSystemUse = true;
				ImGui::NewLine();
				ImGui::NewLine();

				if(ImGui::Checkbox("ScaleChangeRandom", &m_tEffectInfo.bScaleChangeRandom))
					bEffectSystemUse = true;
				ImGui::NewLine();

				if (m_tEffectInfo.bScaleChangeRandom)
				{
					ImGui::Text("ScaleChangeTime");
					if(ImGui::InputFloat2("##ScaleChangeTime", &m_tEffectInfo.fScaleChangeTime.x))
						bEffectSystemUse = true;
					ImGui::NewLine();
				}
				else
				{
					ImGui::Text("ScaleMin");
					if (ImGui::InputFloat3("##ScaleMin", &m_tEffectInfo.fScaleSizeMin.x))
						bEffectSystemUse = true;
					ImGui::NewLine();

					ImGui::Text("ScaleMax");
					if (ImGui::InputFloat3("##ScaleMax", &m_tEffectInfo.fScaleSizeMax.x))
						bEffectSystemUse = true;
					ImGui::NewLine();

					ImGui::Text("ScaleDirSpeed");
					if (ImGui::InputFloat3("##ScaleDirSpeed", &m_tEffectInfo.fScaleDirSpeed.x))
						bEffectSystemUse = true;
					ImGui::NewLine();
					ImGui::NewLine();

					if(ImGui::Checkbox("ScaleAdd", &m_tEffectInfo.bScaleAdd))
						bEffectSystemUse = true;
					ImGui::NewLine();

					if(ImGui::Checkbox("ScaleLoop", &m_tEffectInfo.bScaleLoop))
						bEffectSystemUse = true;
					ImGui::NewLine();

					if(ImGui::Checkbox("ScaleLoopStart", &m_tEffectInfo.bScaleLoopStart))
						bEffectSystemUse = true;
					ImGui::NewLine();
				}

				ImGui::Text("ScaleSpeed");
				if(ImGui::InputFloat2("##ScaleSpeed", &m_tEffectInfo.fScaleSpeed.x))
					bEffectSystemUse = true;
				ImGui::NewLine();
			}
		}

		// 이동(힘)
		if (ImGui::CollapsingHeader("Velocity"))
		{
			ImGui::Text("VelocitySpeed");
			if (ImGui::InputFloat2("##VelocitySpeed", &m_tEffectInfo.fVelocitySpeed.x))
				bEffectSystemUse = true;
			ImGui::NewLine();

			ImGui::Text("VelocityMinStart");
			if (ImGui::InputFloat3("##VelocityMinStart", &m_tEffectInfo.vVelocityMinStart.x))
				bEffectSystemUse = true;
			ImGui::Text("vVelocityMaxStart");
			if (ImGui::InputFloat3("##vVelocityMaxStart", &m_tEffectInfo.vVelocityMaxStart.x))
				bEffectSystemUse = true;
			ImGui::NewLine();

			if (ImGui::Checkbox("VelocityChange", &m_tEffectInfo.bVelocityChange))
				bEffectSystemUse = true;

			if (m_tEffectInfo.bVelocityChange)
			{
				ImGui::Text("VelocityChangeStartDelay");
				if(ImGui::InputFloat2("##VelocityChangeStartDelay", &m_tEffectInfo.fVelocityChangeStartDelay.x))
					bEffectSystemUse = true;
				ImGui::NewLine();

				ImGui::Text("VelocityChangeTime");
				if(ImGui::InputFloat2("##VelocityChangeTim", &m_tEffectInfo.fVelocityChangeTime.x))
					bEffectSystemUse = true;
				ImGui::NewLine();
			}
		}

		// 회전
		if (ImGui::CollapsingHeader("Rotation"))
		{
			if (ImGui::Checkbox("RotationBillboard", &m_tEffectInfo.bBillboard))
				bEffectSystemUse = true;
			ImGui::NewLine();

			if(!m_tEffectInfo.bBillboard)
			{
				if (ImGui::Checkbox("RotationRandomAxis", &m_tEffectInfo.bRandomAxis))
					bEffectSystemUse = true;
				if (!m_tEffectInfo.bRandomAxis)
				{
					ImGui::Text("RotationAxis");
					if(ImGui::InputFloat3("##RotationAxis", &m_tEffectInfo.fAxis.x))
						bEffectSystemUse = true;
					ImGui::NewLine();
				}
				ImGui::NewLine();

				if(ImGui::Checkbox("RotationRandomAngle", &m_tEffectInfo.bRandomAngle))
					bEffectSystemUse = true;
				if (!m_tEffectInfo.bRandomAngle)
				{
					ImGui::Text("RotationAngle");
					if(ImGui::InputFloat("##RangeParticles", &m_tEffectInfo.fAngle))
						bEffectSystemUse = true;
					ImGui::NewLine();
				}
				ImGui::NewLine();

				if(ImGui::Checkbox("RotationChange", &m_tEffectInfo.bRotationChange))
					bEffectSystemUse = true;
				if (m_tEffectInfo.bRotationChange)
				{
					ImGui::Text("RotationChangeStartDelayParticles");
					if(ImGui::InputFloat2("##RotationChangeStartDelayParticles", &m_tEffectInfo.fRotationChangeStartDelay.x))
						bEffectSystemUse = true;
					ImGui::NewLine();

					ImGui::Text("RotationSpeed");
					if (ImGui::InputFloat2("##RotationSpeed", &m_tEffectInfo.fRotationSpeed.x))
						bEffectSystemUse = true;

					ImGui::Text("RotationDir");
					if (ImGui::InputFloat3("##RotationDir", &m_tEffectInfo.fRotationDir.x))
						bEffectSystemUse = true;
					ImGui::NewLine();

					if(ImGui::Checkbox("RotationChangeRandom", &m_tEffectInfo.bRotationChangeRandom))
						bEffectSystemUse = true;
					if (m_tEffectInfo.bRotationChangeRandom)
					{
						ImGui::Text("RotationChangeTime");
						if (ImGui::InputFloat2("##RotationChangeTime", &m_tEffectInfo.fRotationChangeTime.x))
							bEffectSystemUse = true;
					}
				}
				ImGui::NewLine();
			}
		}

		// 지속 시간
		if (ImGui::CollapsingHeader("LifeTime"))
		{
			ImGui::Text("LifeTime");
			if(ImGui::InputFloat2("##LifeTime", &m_tEffectInfo.fLifeTime.x))
				bEffectSystemUse = true;
			ImGui::NewLine();
		}

		// 텍스처 지정
		if (ImGui::CollapsingHeader("Resource"))
		{
			// 모델 원형
			ImGui::Text("ModelName :");
			ImGui::SameLine();
			if(ImGui::InputText("##ModelName", m_cModelName, IM_ARRAYSIZE(m_cModelName)))
				bEffectSystemUse = true;
			ImGui::NewLine();


			// 디퓨즈 텍스처
			ImGui::Text("DiffuseFolderName :");
			ImGui::SameLine();
			if (ImGui::Combo("##DiffuseFolderName", &m_iDiffuseFolderIndex, m_cFolderName, IM_ARRAYSIZE(m_cFolderName)))
				bEffectSystemUse = true;

			if (m_pEffect != nullptr)
			{
				ImGui::Text("DiffuseTextureIndex");
				ImGui::SameLine();
				if (ImGui::InputInt("##DiffuseTextureIndex", &(_int)m_tEffectInfo.iTextureIndexDiffuse))
					bEffectSystemUse = true;
				ImGui::NewLine();

				CTexture* pDiffuseTexture = static_cast<CEffect*>(m_pEffect)->Get_DiffuseTexture();
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
								m_tEffectInfo.iTextureIndexDiffuse = i;
								bEffectSystemUse = true;
							}
						}
						ImGui::EndListBox();
					}
				}
			}


			// 알파 텍스처
			ImGui::Text("AlphaFolderName :");
			ImGui::SameLine();
			if (ImGui::Combo("##AlphaFolderName", &m_iAlphaFolderIndex, m_cFolderName, IM_ARRAYSIZE(m_cFolderName)))
				bEffectSystemUse = true;

			if (m_pEffect != nullptr)
			{
				ImGui::Text("AlphaTextureIndex");
				ImGui::SameLine();
				if (ImGui::InputInt("##AlphaTextureIndex", &(_int)m_tEffectInfo.iTextureIndexAlpha))
					bEffectSystemUse = true;
				ImGui::NewLine();

				CTexture* pAlphaTexture = static_cast<CEffect*>(m_pEffect)->Get_AlphaTexture();
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
								m_tEffectInfo.iTextureIndexAlpha = i;
								bEffectSystemUse = true;
							}
						}
						ImGui::EndListBox();
					}
				}
			}
		}

		// 텍스처 애니메이션
		if (ImGui::CollapsingHeader("UV/Flow"))
		{
			// 디퓨즈 && 알파 UVIndex && UVMaxCount
			if (ImGui::Checkbox("UVIndexRandomStart", &m_tEffectInfo.bRandomStartIndex))
				bEffectSystemUse = true;
			if (!m_tEffectInfo.bRandomStartIndex)
			{
				ImGui::Text("UVIndex");
				if (ImGui::InputFloat2("##UVIndex", &m_tEffectInfo.fUVIndex.x))
					bEffectSystemUse = true;
				ImGui::NewLine();
			}
			ImGui::Text("UVMaxCount");
			if (ImGui::InputFloat2("##UVMaxCount", &m_tEffectInfo.fMaxCount.x))
				bEffectSystemUse = true;
			ImGui::NewLine();

			// UVFlow
			if (ImGui::Checkbox("UVFlowChange", &m_tEffectInfo.bUVFlowChange))
				bEffectSystemUse = true;
			if (m_tEffectInfo.bUVFlowChange)
			{
				ImGui::Text("UVFlowLoop : 0 < UVLoop -> NoLoop");
				if (ImGui::InputInt("##UVFlowLoop", &(_int)m_tEffectInfo.iUVFlowLoop))
					bEffectSystemUse = true;
				ImGui::Text("UVFlowDir");
				if (ImGui::InputFloat2("##UVFlowDir", &m_tEffectInfo.fUVFlowDir.x))
					bEffectSystemUse = true;
				ImGui::Text("UVFlowSpeed");
				if (ImGui::InputFloat2("##UVFlowSpeed", &m_tEffectInfo.fUVFlowSpeed.x))
					bEffectSystemUse = true;
			}
			ImGui::NewLine();
		}

		// 애니메이션
		if (ImGui::CollapsingHeader("Animation"))
		{
			if (ImGui::Checkbox("UseAnimation", &m_tEffectInfo.bAnimation))
				bEffectSystemUse = true;
			if (m_tEffectInfo.bAnimation)
			{
				// 반복 여부
				if (ImGui::Checkbox("LoopAnimation", &m_tEffectInfo.bAnimationLoop))
					bEffectSystemUse = true;

				if(ImGui::Checkbox("IncrementAnimation", &m_tEffectInfo.bIncrement))
					bEffectSystemUse = true;

				ImGui::Text("SpeedAnimation");
				if(ImGui::InputFloat2("##SpeedAnimation", &m_tEffectInfo.fAnimationSpeed.x))
					bEffectSystemUse = true;
			}
		}

		// 색상
		if (ImGui::CollapsingHeader("Color"))
		{
			if (ImGui::Checkbox("ColorRandom", &m_tEffectInfo.bColorRandom))
				bEffectSystemUse = true;
			if (!m_tEffectInfo.bColorRandom)
			{
				ImGui::Text("ColorStart");
				if(ImGui::ColorEdit4("##ColorStart", (float*)&m_tEffectInfo.fColorS, ImGuiColorEditFlags_Float))
					bEffectSystemUse = true;
			}
			ImGui::NewLine();
			ImGui::NewLine();

			if (ImGui::Checkbox("ColorChange", &m_tEffectInfo.bColorChange))
				bEffectSystemUse = true;
			if (m_tEffectInfo.bColorChange)
			{
				if(ImGui::Checkbox("ColorChangeRandom", &m_tEffectInfo.bColorChangeRandom))
					bEffectSystemUse = true;
				ImGui::NewLine();

				// fColorChangeStartDelay
				ImGui::Text("ColorChangeStartDelay");
				if(ImGui::InputFloat2("##ColorChangeStartDelay", &m_tEffectInfo.fColorChangeStartDelay.x))
					bEffectSystemUse = true;
				ImGui::NewLine();

				if (m_tEffectInfo.bColorChangeRandom)
				{
					ImGui::Text("ColorChangeRandomTime");
					if(ImGui::InputFloat2("##ColorChangeRandomTime", &m_tEffectInfo.fColorChangeRandomTime.x))
						bEffectSystemUse = true;
					ImGui::NewLine();
				}
				else
				{
					if(ImGui::Checkbox("ColorLoop", &m_tEffectInfo.bColorLoop))
						bEffectSystemUse = true;
					ImGui::NewLine();

					// fColorChangeStartM
					ImGui::Text("ColorChangeStartM");
					if(ImGui::InputFloat2("##ColorChangeStartM", &m_tEffectInfo.fColorChangeStartM.x))
						bEffectSystemUse = true;

					ImGui::Text("ColorMiddle");
					if(ImGui::ColorEdit4("##ColorMiddle", (float*)&m_tEffectInfo.fColorM, ImGuiColorEditFlags_Float))
						bEffectSystemUse = true;
					ImGui::NewLine();

					// fColorChangeStartF
					ImGui::Text("ColorChangeStartF");
					if(ImGui::InputFloat2("##ColorChangeStartF", &m_tEffectInfo.fColorChangeStartF.x))
						bEffectSystemUse = true;

					ImGui::Text("ColorFinal");
					if(ImGui::ColorEdit4("##ColorFinal", (float*)&m_tEffectInfo.fColorF, ImGuiColorEditFlags_Float))
						bEffectSystemUse = true;
					ImGui::NewLine();
				}

				// ColorDurationTime
				ImGui::Text("ColorDurationTime");
				if(ImGui::InputFloat2("##ColorDurationTime", &m_tEffectInfo.fColorDuration.x))
					bEffectSystemUse = true;
			}
		}

		// 알파
		if (ImGui::CollapsingHeader("Alpha"))
		{
			ImGui::Text("StartAlpha");
			if (ImGui::InputFloat2("##StartAlpha", &m_tEffectInfo.fAlphaStart.x))
				bEffectSystemUse = true;
			ImGui::NewLine();

			if (ImGui::Checkbox("AlphaCreate", &m_tEffectInfo.bAlphaCreate))
				bEffectSystemUse = true;
			if (ImGui::Checkbox("AlphaDelete", &m_tEffectInfo.bAlphaDelete))
				bEffectSystemUse = true;
			ImGui::NewLine();

			ImGui::Text("AlphaSpeed");
			if (ImGui::InputFloat2("##AlphaSpeed", &m_tEffectInfo.fAlphaSpeed.x))
				bEffectSystemUse = true;
			ImGui::NewLine();

			if(ImGui::Checkbox("AlphaChange", &m_tEffectInfo.bAlphaChange))
				bEffectSystemUse = true;
			if (m_tEffectInfo.bAlphaChange)
			{
				ImGui::Text("AlphaChangeStartDelay");
				if(ImGui::InputFloat2("##AlphaChangeStartDelay", &m_tEffectInfo.fAlphaChangeStartDelay.x))
					bEffectSystemUse = true;
				ImGui::NewLine();

				if(ImGui::Checkbox("AlphaIn", &m_tEffectInfo.bAlphaIn))
					bEffectSystemUse = true;
				ImGui::NewLine();
			}
		}

		// 쉐이더 값 셋팅
		if (ImGui::CollapsingHeader("Shader"))
		{
			// 쉐이더 패스
			ImGui::Text("ShaderPass");
			if(ImGui::InputInt("##ShaderPass", &(_int)m_tEffectInfo.iShaderPass))
				bEffectSystemUse = true;
			ImGui::NewLine();

			// 디스카드 값 셋팅
			ImGui::Text("Alpha_Discard");
			if(ImGui::InputFloat("##Alpha_Discard", &m_tEffectInfo.fAlpha_Discard))
				bEffectSystemUse = true;
			ImGui::Text("Black_Discard");
			if(ImGui::InputFloat3("##Black_Discard", &m_tEffectInfo.fBlack_Discard.x))
				bEffectSystemUse = true;
			ImGui::NewLine();

			// 블러 셋팅
			if(ImGui::Checkbox("BloomPowerRandom", &m_tEffectInfo.bBloomPowerRandom))
				bEffectSystemUse = true;
			if (!m_tEffectInfo.bBloomPowerRandom)
			{
				ImGui::Text("BloomPower");
				if(ImGui::ColorEdit4("##BloomPower", (float*)&m_tEffectInfo.fBloomPower, ImGuiColorEditFlags_Float))
					bEffectSystemUse = true;
				ImGui::NewLine();
			}

			if (ImGui::Checkbox("BlurPowerRandom", &m_tEffectInfo.bBlurPowerRandom))
				bEffectSystemUse = true;
			if (!m_tEffectInfo.bBlurPowerRandom)
			{
				if(ImGui::InputFloat("##BlurPower", &m_tEffectInfo.fBlurPower))
					bEffectSystemUse = true;
			}
		}

		// 디스토션
		if (ImGui::CollapsingHeader("Distortion"))
		{
			// 디스토션 텍스처 
			ImGui::Text("DistortionFolderName :");
			ImGui::SameLine();
			if (ImGui::Combo("##DistortionFolderName", &m_iDistortionFolderIndex, m_cFolderName, IM_ARRAYSIZE(m_cFolderName)))
				bEffectSystemUse = true;
			ImGui::NewLine();

			// 디스토션 인덱스
			if (m_pEffect != nullptr)
			{
				ImGui::Text("DistortionTextureIndex");
				ImGui::SameLine();
				if (ImGui::InputInt("##DistortionTextureIndex", &(_int)m_tEffectInfo.iDistortionIndex))
					bEffectSystemUse = true;

				CTexture* pDistortionTexture = static_cast<CEffect*>(m_pEffect)->Get_DistortionTexture();
				if (pDistortionTexture != nullptr)
				{
					ImGui::Text("Distortion Texture");
					if (ImGui::BeginListBox("##Effect_DistortionTexture_List", ImVec2(450.f, 200.f)))
					{
						for (size_t i = 0; i < pDistortionTexture->Get_TextureCount(); ++i)
						{
							if (i % 5 != 0)
								IMGUI_SAME_LINE;

							if (ImGui::ImageButton(pDistortionTexture->Get_Srv(i), ImVec2(50.f, 50.f)))
							{
								m_tEffectInfo.iDistortionIndex = i;
								bEffectSystemUse = true;
							}
						}
						ImGui::EndListBox();
					}
				}
				else
				{
					m_tEffectInfo.vDistortionPower = _float4(0.f, 0.f, 0.f, 0.f);
				}
				ImGui::NewLine();
			}

			// 디스토션 파워
			ImGui::Text("DistortionPower");
			if (ImGui::InputFloat2("##DistortionPower", &m_tEffectInfo.vDistortionPower.x))
				bEffectSystemUse = true;
		}
	}
#pragma endregion

	return bEffectSystemUse;
}

_bool CTool_Effect::Tick_DecalTool()
{
	// CreateDelete
	Tick_CreateDelete(TYPE_DECAL);
	ImGui::NewLine();

	// Binary
	Tick_Binary(TYPE_DECAL);
	ImGui::NewLine();

	// Prototype
	Tick_Prototype(TYPE_DECAL);
	ImGui::NewLine();

	// Transform
	Tick_Transform(TYPE_DECAL);
	ImGui::NewLine();

#pragma region DecalSystem
	_bool bDecalSystemUse = false;

	if (ImGui::CollapsingHeader("DecalSystem"))
	{
		if (ImGui::Button("Restart"))
			Store_ObjectInfo(TYPE_DECAL);

		// 지속 시간
		if (ImGui::CollapsingHeader("LifeTime"))
		{
			ImGui::Text("DecalLifeTime");
			if (ImGui::InputFloat("##DecalLifeTime", &m_tDecalInfo.fLifeTime))
				bDecalSystemUse = true;
			ImGui::NewLine();
		}

		// 텍스처 지정
		if (ImGui::CollapsingHeader("Texture"))
		{
			ImGui::Text("DiffuseTextureIndex");
			ImGui::SameLine();
			if (ImGui::InputInt("##DiffuseTextureIndex", &(_int)m_tDecalInfo.iTextureIndexDiffuse))
				bDecalSystemUse = true;
			ImGui::NewLine();

			if (nullptr != m_pDecal)
			{
				CTexture* pDiffuseTexture = static_cast<CDecal*>(m_pDecal)->Get_DiffuseTexture();
				if (pDiffuseTexture != nullptr)
				{
					ImGui::Text("DiffuseTexture");
					if (ImGui::BeginListBox("##DecalTextureList", ImVec2(450.f, 200.f)))
					{
						for (size_t i = 0; i < pDiffuseTexture->Get_TextureCount(); ++i)
						{
							if (i % 5 != 0)
								IMGUI_SAME_LINE;

							if (ImGui::ImageButton(pDiffuseTexture->Get_Srv(i), ImVec2(50.f, 50.f)))
							{
								m_tDecalInfo.iTextureIndexDiffuse = i;
								bDecalSystemUse = true;
							}
						}
						ImGui::EndListBox();
					}
				}
			}
			ImGui::NewLine();
		}

		// 색상
		if (ImGui::CollapsingHeader("Color"))
		{
			ImGui::Text("Color_Add_01_Alpha");
			if(ImGui::InputFloat("##Color_Add_01_Alpha", &m_tDecalInfo.fColorAdd_01_Alpha))
				bDecalSystemUse = true;
			ImGui::NewLine();

			ImGui::Text("ColorAdd_01");
			if(ImGui::ColorEdit3("##ColorAdd_01", (float*)&m_tDecalInfo.fColorAdd_01, ImGuiColorEditFlags_Float))
				bDecalSystemUse = true;

			ImGui::Text("ColorAdd_02");
			if(ImGui::ColorEdit3("##ColorAdd_02", (float*)&m_tDecalInfo.fColorAdd_02, ImGuiColorEditFlags_Float))
				bDecalSystemUse = true;
			ImGui::NewLine();
		}

		// 알파
		if (ImGui::CollapsingHeader("Alpha"))
		{
			ImGui::Text("AlphaRemove");
			if(ImGui::InputFloat("##AlphaRemove", &m_tDecalInfo.fAlphaRemove))
				bDecalSystemUse = true;
			ImGui::NewLine();

			if(ImGui::Checkbox("AlphaCreate", &m_tDecalInfo.bAlphaCreate))
				bDecalSystemUse = true;

			if(ImGui::Checkbox("AlphaDelete", &m_tDecalInfo.bAlphaDelete))
				bDecalSystemUse = true;

			ImGui::Text("AlphaSpeed");
			if(ImGui::InputFloat("##AlphaSpeed", &m_tDecalInfo.fAlphaSpeed))
				bDecalSystemUse = true;
			ImGui::NewLine();
		}

		// 쉐이더 값 셋팅
		if (ImGui::CollapsingHeader("Shader"))
		{
			// 쉐이더 패스
			ImGui::Text("ShaderPass");
			if(ImGui::InputInt("##ShaderPass", &(_int)m_tDecalInfo.iShaderPass))
				bDecalSystemUse = true;
			ImGui::NewLine();

			// 디스카드 값 셋팅
			ImGui::Text("Alpha_Discard");
			if(ImGui::InputFloat("##Alpha_Discard", &m_tDecalInfo.fAlpha_Discard))
				bDecalSystemUse = true;

			ImGui::Text("Black_Discard");
			if(ImGui::InputFloat3("##Black_Discard", &m_tDecalInfo.fBlack_Discard.x))
				bDecalSystemUse = true;
			ImGui::NewLine();

			// 블룸 셋팅
			ImGui::Text("BloomPower");
			if(ImGui::ColorEdit4("##BloomPower", (float*)&m_tDecalInfo.fBloomPower, ImGuiColorEditFlags_Float))
				bDecalSystemUse = true;
			ImGui::NewLine();

			// 블러 셋팅
			// ImGui::InputFloat("##BlurPower", &m_tDecalInfo.fBlurPower);
		}
	}
#pragma endregion

	return bDecalSystemUse;
}

_bool CTool_Effect::Tick_VfxTool()
{
	// 원형에 적용
	if (ImGui::Button("Load_VfxPrototype"))
		Load_OriginalInfoVfx();
	ImGui::SameLine();

	ImGui::Text("PrototypeVfxName :");
	ImGui::SameLine();

	ImGui::InputText("##PrototypeVfxName", m_cPrototypeName, IM_ARRAYSIZE(m_cPrototypeName));
	ImGui::NewLine();

	if (ImGui::CollapsingHeader("VfxInfo"))
	{
		if (m_pFrameTriger != nullptr && m_pPositionOffset != nullptr && m_pScaleOffset != nullptr && m_pRotationOffset != nullptr)
		{
			for(size_t i = 0; i < m_iVfxMaxCount; ++i)
			{
				std::string frameTrigerString = "FrameTriger" + std::to_string(i);
				ImGui::InputInt(frameTrigerString.c_str(), &m_pFrameTriger[i]);

				std::string positionOffsetString = "PositionOffset " + std::to_string(i);
				ImGui::InputFloat3(positionOffsetString.c_str(), &m_pPositionOffset[i].x);

				std::string scaleOffsetString = "ScaleOffset " + std::to_string(i);
				ImGui::InputFloat3(scaleOffsetString.c_str(), &m_pScaleOffset[i].x);

				std::string rotationOffsetString = "RotationOffset " + std::to_string(i);
				ImGui::InputFloat3(rotationOffsetString.c_str(), &m_pRotationOffset[i].x);

				ImGui::NewLine();
			}
		}
	}

	return true;
}


void CTool_Effect::Tick_CreateDelete(TYPE eType)
{
	if (ImGui::Button("Create"))
		Create_Object(eType);
	ImGui::SameLine();

	if (ImGui::Button("Delete"))
	{
		if (TYPE_EFFECT == eType)
		{
			if (nullptr != m_pEffect)
			{
				m_pEffect->Set_Dead(true);
				m_pEffect = nullptr;
			}
		}
		else if (TYPE_DECAL == eType)
		{
			if (nullptr != m_pDecal)
			{
				m_pDecal->Set_Dead(true);
				m_pDecal = nullptr;
			}
		}
	}
}

void CTool_Effect::Tick_Transform(TYPE eType)
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		if (ImGui::Button("OK"))
			Set_Transform(eType);

		ImGui::Text("Position");
		ImGui::InputFloat3("##Position", &m_fPosition.x);

		ImGui::Text("Rotation");
		ImGui::InputFloat3("##Rotation", &m_fRotation.x);

		ImGui::Text("Scale");
		ImGui::InputFloat3("##Scale", &m_fScale.x);
	}
}

void CTool_Effect::Tick_Prototype(TYPE eType)
{
	if (ImGui::CollapsingHeader("Prototype"))
	{
		if (ImGui::Button("Set_PrototypeObject"))
			Set_OriginalInfo(eType);
		ImGui::SameLine();

		ImGui::Text("PrototypeObject_Name :");
		ImGui::SameLine();
		ImGui::InputText("##PrototypeObject_Name", m_cPrototypeName, IM_ARRAYSIZE(m_cPrototypeName));
	}
}

void CTool_Effect::Tick_Binary(TYPE eType)
{
	if (ImGui::CollapsingHeader("Binary"))
	{
		if (ImGui::Button("Save"))
		{
			switch (eType)
			{
			case TYPE_EFFECT:
				Save_Effect(m_cBinaryName);
				break;
			case TYPE_DECAL:
				Save_Decal(m_cBinaryName);
				break;
			}
		}
		ImGui::SameLine();

		if (ImGui::Button("Load"))
		{
			switch (eType)
			{
			case TYPE_EFFECT:
				Load_Effect(m_cBinaryName);
				break;
			case TYPE_DECAL:
				Load_Decal(m_cBinaryName);
				break;
			}
		}
		ImGui::SameLine();

		ImGui::Text("FileName :");
		ImGui::SameLine();
		ImGui::InputText("##FileName", m_cBinaryName, IM_ARRAYSIZE(m_cBinaryName));
	}
}

#pragma endregion

#pragma region 
void CTool_Effect::Create_Object(TYPE eType)
{
	switch (eType)
	{
	case TYPE_EFFECT:
		if (nullptr != m_pEffect)
			return;
		m_pEffect = GI->Clone_GameObject(TEXT("Prototype_TempMeshEffect"), LAYER_TYPE::LAYER_EFFECT);
		GI->Add_GameObject(LEVEL_TOOL, LAYER_TYPE::LAYER_EFFECT, m_pEffect);
		Load_ObjectInfo(eType);
		break;
		// --------------------------------------------------------------------------------------------
	case TYPE_DECAL:
		if (nullptr != m_pDecal)
			return;
		m_pDecal = GI->Clone_GameObject(TEXT("Prototype_TempDecal"), LAYER_TYPE::LAYER_EFFECT);
		GI->Add_GameObject(LEVEL_TOOL, LAYER_TYPE::LAYER_EFFECT, m_pDecal);
		Load_ObjectInfo(eType);
		break;
		// --------------------------------------------------------------------------------------------
	default:
		break;
	}
}

void CTool_Effect::Set_Transform(TYPE eType)
{
	CTransform* pTransform = nullptr;

	switch (eType)
	{
	case TYPE_EFFECT:
		if (nullptr == m_pEffect)
			return;
		pTransform = m_pEffect->Get_Component<CTransform>(L"Com_Transform");
		break;
		// --------------------------------------------------------------------------------------------
	case TYPE_DECAL:
		if (nullptr == m_pDecal)
			return;
		pTransform = m_pDecal->Get_Component<CTransform>(L"Com_Transform");
		break;
		// --------------------------------------------------------------------------------------------
	default:
		break;
	}

	if (nullptr == pTransform)
		return;

	pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fPosition.x, m_fPosition.y, m_fPosition.z, 1.f));
	pTransform->FixRotation(m_fRotation.x, m_fRotation.y, m_fRotation.z);
	pTransform->Set_Scale(m_fScale);
}

void CTool_Effect::Load_ObjectInfo(TYPE eType)
{
	CTransform* pTransform = nullptr;
	size_t convertedChars = 0;

	switch (eType)
	{
	case TYPE_EFFECT:
		if (nullptr == m_pEffect)
			return;
		pTransform = m_pEffect->Get_Component<CTransform>(L"Com_Transform");
		m_tEffectInfo = static_cast<CEffect*>(m_pEffect)->Get_EffectDesc();

		if (m_tEffectInfo.eType == CEffect::EFFECT_TEXTURE)
			m_iEffectTypeIndex = 0;
		else if (m_tEffectInfo.eType == CEffect::EFFECT_MESH)
			m_iEffectTypeIndex = 1;
		else
			m_iEffectTypeIndex = 2;

		wcstombs_s(&convertedChars, m_cModelName, sizeof(m_cModelName), m_tEffectInfo.strModelName.c_str(), _TRUNCATE);

		m_iDiffuseFolderIndex    = Get_FolderIndex(m_tEffectInfo.strDiffuseTetextureName);
		m_iAlphaFolderIndex      = Get_FolderIndex(m_tEffectInfo.strAlphaTexturName);
		m_iDistortionFolderIndex = Get_FolderIndex(m_tEffectInfo.strDistortionTetextureName);
		break;
		// --------------------------------------------------------------------------------------------
	case TYPE_DECAL:
		if (m_pDecal == nullptr)
			return;
		pTransform = m_pDecal->Get_Component<CTransform>(L"Com_Transform");
		m_tDecalInfo = static_cast<CDecal*>(m_pDecal)->Get_DecalDesc();
		break;
		// --------------------------------------------------------------------------------------------
	default:
		return;
	}

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

void CTool_Effect::Store_ObjectInfo(TYPE eType)
{
	if (TYPE_EFFECT == eType)
	{
		if (nullptr == m_pEffect)
			return;

		if (m_iEffectTypeIndex == 0)
			m_tEffectInfo.eType = CEffect::EFFECT_TEXTURE;
		else if (m_iEffectTypeIndex == 1)
			m_tEffectInfo.eType = CEffect::EFFECT_MESH;
		else
			m_tEffectInfo.eType = CEffect::EFFECT_END;

		wstring strModelName(m_cModelName, m_cModelName + strlen(m_cModelName));
		m_tEffectInfo.strModelName = strModelName;

		m_tEffectInfo.strDiffuseTetextureName    = Select_FolderName(m_iDiffuseFolderIndex);
		m_tEffectInfo.strAlphaTexturName         = Select_FolderName(m_iAlphaFolderIndex);
		m_tEffectInfo.strDistortionTetextureName = Select_FolderName(m_iDistortionFolderIndex);

		static_cast<CEffect*>(m_pEffect)->Set_EffectDesc(m_tEffectInfo);
	}

	else if (TYPE_DECAL == eType)
	{
		if (m_pDecal == nullptr)
			return;

		static_cast<CDecal*>(m_pDecal)->Set_DecalDesc(m_tDecalInfo);
	}
}

void CTool_Effect::Set_OriginalInfo(TYPE eType)
{
	if (TYPE_EFFECT == eType)
	{
		if (nullptr == m_pEffect)
		{
			MSG_BOX("Prototype_Find_Failed!");
			return;
		}

		wstring strFileName(m_cPrototypeName, m_cPrototypeName + strlen(m_cPrototypeName));
		wstring strPropertyName = L"Prototype_" + strFileName;

		CGameObject* pGameObject = GI->Find_Prototype_GameObject(LAYER_TYPE::LAYER_EFFECT, strPropertyName);
		if (pGameObject == nullptr)
		{
			MSG_BOX("Prototype_Find_Failed!");
			return;
		}

		if (m_iEffectTypeIndex == 0)
			m_tEffectInfo.eType = CEffect::EFFECT_TEXTURE;
		else if (m_iEffectTypeIndex == 1)
			m_tEffectInfo.eType = CEffect::EFFECT_MESH;
		else
			m_tEffectInfo.eType = CEffect::EFFECT_END;

		wstring strModelName(m_cModelName, m_cModelName + strlen(m_cModelName));
		m_tEffectInfo.strModelName = strModelName;

		m_tEffectInfo.strDiffuseTetextureName    = Select_FolderName(m_iDiffuseFolderIndex);
		m_tEffectInfo.strAlphaTexturName         = Select_FolderName(m_iAlphaFolderIndex);
		m_tEffectInfo.strDistortionTetextureName = Select_FolderName(m_iDistortionFolderIndex);

		static_cast<CEffect*>(pGameObject)->Set_EffectDesc(m_tEffectInfo);
	}

	else if (TYPE_DECAL == eType)
	{
		if (m_pDecal == nullptr)
		{
			MSG_BOX("Prototype_Find_Failed!");
			return;
		}

		wstring strFileName(m_cPrototypeName, m_cPrototypeName + strlen(m_cPrototypeName));
		wstring strPropertyName = L"Prototype_" + strFileName;

		CGameObject* pGameObject = GI->Find_Prototype_GameObject(LAYER_TYPE::LAYER_EFFECT, strPropertyName);
		if (pGameObject == nullptr)
		{
			MSG_BOX("Prototype_Find_Failed!");
			return;
		}

		static_cast<CDecal*>(pGameObject)->Set_DecalDesc(m_tDecalInfo);
	}

	MSG_BOX("Prototype_Find_Success!");
}

void CTool_Effect::Load_OriginalInfoVfx()
{
	wstring strFileName(m_cPrototypeName, m_cPrototypeName + strlen(m_cPrototypeName));
	wstring strPropertyName = L"Prototype_" + strFileName;

	CGameObject* pGameObject = GI->Find_Prototype_GameObject(LAYER_TYPE::LAYER_EFFECT, strPropertyName);
	if (pGameObject == nullptr)
	{
		MSG_BOX("Prototype_Find_Failed!");
		return;
	}

	CVfx* pVfx = static_cast<CVfx*>(pGameObject);
	if (pVfx == nullptr)
	{
		MSG_BOX("Prototype_Vfx_Failed!");
		return;
	}

	m_iVfxMaxCount    = pVfx->Get_MaxCount();
	m_pFrameTriger    = pVfx->Get_FrameTriger();
	m_pPositionOffset = pVfx->Get_PositionOffset();
	m_pScaleOffset    = pVfx->Get_ScaleOffset();
	m_pRotationOffset = pVfx->Get_RotationOffset();

	MSG_BOX("Prototype_Vfx_Loaded!");
}

wstring CTool_Effect::Select_FolderName(_uint iFolderIndex)  
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

_uint CTool_Effect::Get_FolderIndex(wstring& strName)
{
	_int iIndex = -1;

	_int  iBufferSizeName = WideCharToMultiByte(CP_UTF8, 0, strName.c_str(), -1, nullptr, 0, nullptr, nullptr);
	char* pszName         = new char[iBufferSizeName];
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
#pragma endregion

#pragma region Save && Load
void CTool_Effect::Save_Effect(const char* pFileName)
{
	if (m_pEffect == nullptr)
	{
		MSG_BOX("Effect_Save_Failed!");
		return;
	}

	//m_tEffectInfo = static_cast<CEffect*>(m_pEffect)->Get_EffectDesc();

	Json json;
	json["EffectInfo"].push_back({

		// 이펙트 타입
		{"Type", (_int)m_tEffectInfo.eType},

		// 중력 
		{"Gravity", m_tEffectInfo.bGravity},

		// 위치			             
		{"Range", {
			{"x", m_tEffectInfo.fRange.x},
			{"y", m_tEffectInfo.fRange.y},
			{"z", m_tEffectInfo.fRange.z}}
		},

		{"RangeDistance", {
			{"x", m_tEffectInfo.fRangeDistance.x},
			{"y", m_tEffectInfo.fRangeDistance.y}}
		},

#pragma region 크기		             
		{"ScaleSameRate", m_tEffectInfo.bScaleSameRate},

		{"ScaleStartMin", {
			{"x", m_tEffectInfo.fScaleStartMin.x},
			{"y", m_tEffectInfo.fScaleStartMin.y},
			{"z", m_tEffectInfo.fScaleStartMin.z}}
		},

		{"ScaleStartMax", {
			{"x", m_tEffectInfo.fScaleStartMax.x},
			{"y", m_tEffectInfo.fScaleStartMax.y},
			{"z", m_tEffectInfo.fScaleStartMax.z}}
		},

		{"ScaleChange", m_tEffectInfo.bScaleChange},

		{"ScaleChangeStartDelay", {
			{"x", m_tEffectInfo.fScaleChangeStartDelay.x},
			{"y", m_tEffectInfo.fScaleChangeStartDelay.y}}
		},

		{"ScaleChangeRandom", m_tEffectInfo.bScaleChangeRandom},

		{"ScaleChangeTime", {
			{"x", m_tEffectInfo.fScaleChangeTime.x},
			{"y", m_tEffectInfo.fScaleChangeTime.y}}
		},

		{"ScaleAdd", m_tEffectInfo.bScaleAdd},
		{"ScaleLoop", m_tEffectInfo.bScaleLoop},
		{"ScaleLoopStart", m_tEffectInfo.bScaleLoopStart},

		{"ScaleSizeMin", {
			{"x", m_tEffectInfo.fScaleSizeMin.x},
			{"y", m_tEffectInfo.fScaleSizeMin.y},
			{"z", m_tEffectInfo.fScaleSizeMin.z}}
		},

		{"ScaleSizeMax", {
			{"x", m_tEffectInfo.fScaleSizeMax.x},
			{"y", m_tEffectInfo.fScaleSizeMax.y},
			{"z", m_tEffectInfo.fScaleSizeMax.z}}
		},

		{"ScaleSpeed", {
			{"x", m_tEffectInfo.fScaleSpeed.x},
			{"y", m_tEffectInfo.fScaleSpeed.y}}
		},

		{"ScaleDirSpeed", {
			{"x", m_tEffectInfo.fScaleDirSpeed.x},
			{"y", m_tEffectInfo.fScaleDirSpeed.y},
			{"z", m_tEffectInfo.fScaleDirSpeed.z}}
		},
#pragma endregion

#pragma region 이동
		{"VelocitySpeed", {
			{"x", m_tEffectInfo.fVelocitySpeed.x},
			{"y", m_tEffectInfo.fVelocitySpeed.y}}
		},

		{"VelocityMinStart", {
			{"x", m_tEffectInfo.vVelocityMinStart.x},
			{"y", m_tEffectInfo.vVelocityMinStart.y},
			{"z", m_tEffectInfo.vVelocityMinStart.z}}
		},

		{"VelocityMaxStart", {
			{"x", m_tEffectInfo.vVelocityMaxStart.x},
			{"y", m_tEffectInfo.vVelocityMaxStart.y},
			{"z", m_tEffectInfo.vVelocityMaxStart.z}}
		},

		{"VelocityChange", m_tEffectInfo.bVelocityChange},

		{"VelocityChangeStartDelay", {
			{"x", m_tEffectInfo.fVelocityChangeStartDelay.x},
			{"y", m_tEffectInfo.fVelocityChangeStartDelay.y}}
		},

		{"VelocityChangeTime", {
			{"x", m_tEffectInfo.fVelocityChangeTime.x},
			{"y", m_tEffectInfo.fVelocityChangeTime.y}}
		},
#pragma endregion

#pragma region 회전
		{"Billboard", m_tEffectInfo.bBillboard},

		{"RandomAxis", m_tEffectInfo.bRandomAxis},

		{ "Axis", {
			{"x", m_tEffectInfo.fAxis.x},
			{"y", m_tEffectInfo.fAxis.y},
			{"z", m_tEffectInfo.fAxis.z}}
		},

		{"RandomAngle", m_tEffectInfo.bRandomAngle},

		{"Angle", m_tEffectInfo.fAngle},

		{"RotationChange", m_tEffectInfo.bRotationChange},

		{ "RotationChangeStartDelay", {
			{"x", m_tEffectInfo.fRotationChangeStartDelay.x},
			{"y", m_tEffectInfo.fRotationChangeStartDelay.y}}
		},

		{ "RotationSpeed", {
			{"x", m_tEffectInfo.fRotationSpeed.x},
			{"y", m_tEffectInfo.fRotationSpeed.y}}
		},

		{ "RotationDir", {
			{"x", m_tEffectInfo.fRotationDir.x},
			{"y", m_tEffectInfo.fRotationDir.y},
			{"z", m_tEffectInfo.fRotationDir.z}}
		},

		{"RotationChangeRandom", m_tEffectInfo.bRotationChangeRandom},

		{ "RotationChangeTime", {
			{"x", m_tEffectInfo.fRotationChangeTime.x},
			{"y", m_tEffectInfo.fRotationChangeTime.y}}
		},
#pragma endregion

		// 지속 시간
		{ "LifeTime", {
			{"x", m_tEffectInfo.fLifeTime.x},
			{"y", m_tEffectInfo.fLifeTime.y}}
		},

#pragma region 모델 && 텍스처
		{"ModelName", CUtils::ToString(m_tEffectInfo.strModelName)},
		{"DiffuseTetextureName", CUtils::ToString(m_tEffectInfo.strDiffuseTetextureName)},
		{"AlphaTexturName", CUtils::ToString(m_tEffectInfo.strAlphaTexturName)},

		{"TextureIndexDiffuse", m_tEffectInfo.iTextureIndexDiffuse},
		{"TextureIndexAlpha", m_tEffectInfo.iTextureIndexAlpha},

		{"RandomStartIndex", m_tEffectInfo.bRandomStartIndex},

		{ "UVIndex", {
			{"x", m_tEffectInfo.fUVIndex.x},
			{"y", m_tEffectInfo.fUVIndex.y}}
		},

		{ "MaxCount", {
			{"x", m_tEffectInfo.fMaxCount.x},
			{"y", m_tEffectInfo.fMaxCount.y}}
		},

		{"UVFlowChange", m_tEffectInfo.bUVFlowChange },
		{"UVFlowLoop", m_tEffectInfo.iUVFlowLoop },

		{ "UVFlowDir", {
			{"x", m_tEffectInfo.fUVFlowDir.x},
			{"y", m_tEffectInfo.fUVFlowDir.y}}
		},

		{ "UVFlowSpeed", {
			{"x", m_tEffectInfo.fUVFlowSpeed.x},
			{"y", m_tEffectInfo.fUVFlowSpeed.y}}
		},
#pragma endregion

#pragma region 애니메이션
		{"Animation", m_tEffectInfo.bAnimation},
		{"AnimationLoop", m_tEffectInfo.bAnimationLoop},
		{"Increment", m_tEffectInfo.bIncrement},

		{"AnimationSpeed", {
			{"x", m_tEffectInfo.fAnimationSpeed.x},
			{"y", m_tEffectInfo.fAnimationSpeed.y}}
		},
#pragma endregion

#pragma region 알파
		{ "AlphaStart", {
			{"x", m_tEffectInfo.fAlphaStart.x},
			{"y", m_tEffectInfo.fAlphaStart.y}}
		},

		{"AlphaCreate", m_tEffectInfo.bAlphaCreate},
		{"AlphaDelete", m_tEffectInfo.bAlphaDelete},

		{ "AlphaSpeed", {
			{"x", m_tEffectInfo.fAlphaSpeed.x},
			{"y", m_tEffectInfo.fAlphaSpeed.y}}
		},

		{"AlphaChange", m_tEffectInfo.bAlphaChange},
		{"AlphaIn", m_tEffectInfo.bAlphaIn},

		{ "AlphaChangeStartDelay", {
			{"x", m_tEffectInfo.fAlphaChangeStartDelay.x},
			{"y", m_tEffectInfo.fAlphaChangeStartDelay.y}}
		},
#pragma endregion

#pragma region 색상
		{"ColorRandom", m_tEffectInfo.bColorRandom},

		{ "ColorS", {
			{"x", m_tEffectInfo.fColorS.x},
			{"y", m_tEffectInfo.fColorS.y},
			{"z", m_tEffectInfo.fColorS.z},
			{"w", m_tEffectInfo.fColorS.w}}
		},

		{"ColorChange", m_tEffectInfo.bColorChange},

		{"ColorChangeRandom", m_tEffectInfo.bColorChangeRandom},

		{ "ColorChangeRandomTime", {
			{"x", m_tEffectInfo.fColorChangeRandomTime.x},
			{"y", m_tEffectInfo.fColorChangeRandomTime.y}}
		},

		{"ColorChangeRandom", m_tEffectInfo.bColorLoop},

		{ "ColorChangeStartDelay", {
			{"x", m_tEffectInfo.fColorChangeStartDelay.x},
			{"y", m_tEffectInfo.fColorChangeStartDelay.y}}
		},

		{ "ColorChangeStartM", {
			{"x", m_tEffectInfo.fColorChangeStartM.x},
			{"y", m_tEffectInfo.fColorChangeStartM.y}}
		},

		{ "ColorM", {
			{"x", m_tEffectInfo.fColorM.x},
			{"y", m_tEffectInfo.fColorM.y},
			{"z", m_tEffectInfo.fColorM.z},
			{"w", m_tEffectInfo.fColorM.w}}
		},

		{ "ColorChangeStartF", {
			{"x", m_tEffectInfo.fColorChangeStartF.x},
			{"y", m_tEffectInfo.fColorChangeStartF.y}}
		},

		{ "ColorF", {
			{"x", m_tEffectInfo.fColorF.x},
			{"y", m_tEffectInfo.fColorF.y},
			{"z", m_tEffectInfo.fColorF.z},
			{"w", m_tEffectInfo.fColorF.w}}
		},

		{ "ColorDuration", {
			{"x", m_tEffectInfo.fColorDuration.x},
			{"y", m_tEffectInfo.fColorDuration.y}}
		},
#pragma endregion

#pragma region 블러
		{"BloomPowerRandom", m_tEffectInfo.bBloomPowerRandom },

		{ "BloomPower", {
			{"x", m_tEffectInfo.fBloomPower.x},
			{"y", m_tEffectInfo.fBloomPower.y},
			{"z", m_tEffectInfo.fBloomPower.z},
			{"w", m_tEffectInfo.fBloomPower.w}}
		},

		{"BlurPowerRandom", m_tEffectInfo.bBlurPowerRandom },
		{"BlurPower", m_tEffectInfo.fBlurPower },
#pragma endregion

#pragma region 기타 정보
		{"ShaderPass", m_tEffectInfo.iShaderPass },
		{"Alpha_Discard", m_tEffectInfo.fAlpha_Discard },

		{ "Black_Discard", {
			{"x", m_tEffectInfo.fBlack_Discard.x},
			{"y", m_tEffectInfo.fBlack_Discard.y},
			{"z", m_tEffectInfo.fBlack_Discard.z}}
		},
#pragma endregion

#pragma region 디스토션
		{ "DistortionTetextureName", CUtils::ToString(m_tEffectInfo.strDistortionTetextureName) },
		{ "DistortionIndex", m_tEffectInfo.iDistortionIndex },
		{ "DistortionPower", {
			{"x", m_tEffectInfo.vDistortionPower.x},
			{"y", m_tEffectInfo.vDistortionPower.y},
			{"z", m_tEffectInfo.vDistortionPower.z},
			{"w", m_tEffectInfo.vDistortionPower.w}}
		}
#pragma endregion
		});

	wstring strFileName(pFileName, pFileName + strlen(pFileName));
	wstring strFilePath = L"../Bin/DataFiles/Vfx/Effect/" + strFileName + L".json";
	GI->Json_Save(strFilePath, json);

	MSG_BOX("Effect_Save_Success!");
}

void CTool_Effect::Load_Effect(const char* pFileName)
{
	if (m_pEffect == nullptr)
		Create_Object(TYPE_EFFECT);;

	wstring strFileName(pFileName, pFileName + strlen(pFileName));
	wstring strFilePath = L"../Bin/DataFiles/Vfx/Effect/" + strFileName + L".json";

#pragma region Load
	CEffect::EFFECT_DESC EffectInfo = {};

	Json json = GI->Json_Load(strFilePath);
	for (const auto& item : json["EffectInfo"])
	{
		// 이펙트 타입
		_int iType = item["Type"];
		EffectInfo.eType = (CEffect::EFFECT_TYPE)iType;

		// 중력 
		EffectInfo.bGravity = item["Gravity"];

		// 위치		
		EffectInfo.fRange.x = item["Range"]["x"];
		EffectInfo.fRange.y = item["Range"]["y"];
		EffectInfo.fRange.z = item["Range"]["z"];

		EffectInfo.fRangeDistance.x = item["RangeDistance"]["x"];
		EffectInfo.fRangeDistance.y = item["RangeDistance"]["y"];

#pragma region 크기		  
		EffectInfo.bScaleSameRate = item["ScaleSameRate"];

		EffectInfo.fScaleStartMin.x = item["ScaleStartMin"]["x"];
		EffectInfo.fScaleStartMin.y = item["ScaleStartMin"]["y"];
		EffectInfo.fScaleStartMin.z = item["ScaleStartMin"]["z"];

		EffectInfo.fScaleStartMax.x = item["ScaleStartMax"]["x"];
		EffectInfo.fScaleStartMax.y = item["ScaleStartMax"]["y"];
		EffectInfo.fScaleStartMax.z = item["ScaleStartMax"]["z"];

		EffectInfo.bScaleChange = item["ScaleChange"];

		EffectInfo.fScaleChangeStartDelay.x = item["ScaleChangeStartDelay"]["x"];
		EffectInfo.fScaleChangeStartDelay.y = item["ScaleChangeStartDelay"]["y"];

		EffectInfo.bScaleChangeRandom = item["ScaleChangeRandom"];

		EffectInfo.fScaleChangeTime.x = item["ScaleChangeTime"]["x"];
		EffectInfo.fScaleChangeTime.y = item["ScaleChangeTime"]["y"];

		EffectInfo.bScaleAdd = item["ScaleAdd"];
		EffectInfo.bScaleLoop = item["ScaleLoop"];
		EffectInfo.bScaleLoopStart = item["ScaleLoopStart"];

		EffectInfo.fScaleSizeMin.x = item["ScaleSizeMin"]["x"];
		EffectInfo.fScaleSizeMin.y = item["ScaleSizeMin"]["y"];
		EffectInfo.fScaleSizeMin.z = item["ScaleSizeMin"]["z"];

		EffectInfo.fScaleSizeMax.x = item["ScaleSizeMax"]["x"];
		EffectInfo.fScaleSizeMax.y = item["ScaleSizeMax"]["y"];
		EffectInfo.fScaleSizeMax.z = item["ScaleSizeMax"]["z"];

		EffectInfo.fScaleSpeed.x = item["ScaleSpeed"]["x"];
		EffectInfo.fScaleSpeed.y = item["ScaleSpeed"]["y"];

		EffectInfo.fScaleDirSpeed.x = item["ScaleDirSpeed"]["x"];
		EffectInfo.fScaleDirSpeed.y = item["ScaleDirSpeed"]["y"];
		EffectInfo.fScaleDirSpeed.z = item["ScaleDirSpeed"]["z"];
#pragma endregion

#pragma region 이동
		EffectInfo.fVelocitySpeed.x = item["VelocitySpeed"]["x"];
		EffectInfo.fVelocitySpeed.y = item["VelocitySpeed"]["y"];

		EffectInfo.vVelocityMinStart.x = item["VelocityMinStart"]["x"];
		EffectInfo.vVelocityMinStart.y = item["VelocityMinStart"]["y"];
		EffectInfo.vVelocityMinStart.z = item["VelocityMinStart"]["z"];

		EffectInfo.vVelocityMaxStart.x = item["VelocityMaxStart"]["x"];
		EffectInfo.vVelocityMaxStart.y = item["VelocityMaxStart"]["y"];
		EffectInfo.vVelocityMaxStart.z = item["VelocityMaxStart"]["z"];

		EffectInfo.bVelocityChange = item["VelocityChange"];

		EffectInfo.fVelocityChangeStartDelay.x = item["VelocityChangeStartDelay"]["x"];
		EffectInfo.fVelocityChangeStartDelay.y = item["VelocityChangeStartDelay"]["y"];

		EffectInfo.fVelocityChangeTime.x = item["VelocityChangeTime"]["x"];
		EffectInfo.fVelocityChangeTime.y = item["VelocityChangeTime"]["y"];
#pragma endregion

#pragma region 회전
		EffectInfo.bBillboard = item["Billboard"];

		EffectInfo.bRandomAxis = item["RandomAxis"];

		EffectInfo.fAxis.x = item["Axis"]["x"];
		EffectInfo.fAxis.y = item["Axis"]["y"];
		EffectInfo.fAxis.z = item["Axis"]["z"];

		EffectInfo.bRandomAngle = item["RandomAngle"];

		EffectInfo.fAngle = item["Angle"];

		EffectInfo.bRotationChange = item["RotationChange"];

		EffectInfo.fRotationChangeStartDelay.x = item["RotationChangeStartDelay"]["x"];
		EffectInfo.fRotationChangeStartDelay.y = item["RotationChangeStartDelay"]["y"];

		EffectInfo.fRotationSpeed.x = item["RotationSpeed"]["x"];
		EffectInfo.fRotationSpeed.y = item["RotationSpeed"]["y"];

		EffectInfo.fRotationDir.x = item["RotationDir"]["x"];
		EffectInfo.fRotationDir.y = item["RotationDir"]["y"];
		EffectInfo.fRotationDir.z = item["RotationDir"]["z"];

		EffectInfo.bRotationChangeRandom = item["RotationChangeRandom"];

		EffectInfo.fRotationChangeTime.x = item["RotationChangeTime"]["x"];
		EffectInfo.fRotationChangeTime.y = item["RotationChangeTime"]["y"];
#pragma endregion

		// 지속 시간
		EffectInfo.fLifeTime.x = item["LifeTime"]["x"];
		EffectInfo.fLifeTime.y = item["LifeTime"]["y"];

#pragma region 모델 && 텍스처
		EffectInfo.strModelName = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(item["ModelName"]));
		EffectInfo.strDiffuseTetextureName = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(item["DiffuseTetextureName"]));
		EffectInfo.strAlphaTexturName = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(item["AlphaTexturName"]));

		EffectInfo.iTextureIndexDiffuse = item["TextureIndexDiffuse"];
		EffectInfo.iTextureIndexAlpha = item["TextureIndexAlpha"];

		EffectInfo.bRandomStartIndex = item["RandomStartIndex"];

		EffectInfo.fUVIndex.x = item["UVIndex"]["x"];
		EffectInfo.fUVIndex.y = item["UVIndex"]["y"];

		EffectInfo.fMaxCount.x = item["MaxCount"]["x"];
		EffectInfo.fMaxCount.y = item["MaxCount"]["y"];

		EffectInfo.bUVFlowChange = item["UVFlowChange"];

		EffectInfo.iUVFlowLoop = item["UVFlowLoop"];

		EffectInfo.fUVFlowDir.x = item["UVFlowDir"]["x"];
		EffectInfo.fUVFlowDir.y = item["UVFlowDir"]["y"];

		EffectInfo.fUVFlowSpeed.x = item["UVFlowSpeed"]["x"];
		EffectInfo.fUVFlowSpeed.y = item["UVFlowSpeed"]["y"];
#pragma endregion

#pragma region 애니메이션
		EffectInfo.bAnimation = item["Animation"];

		EffectInfo.bAnimationLoop = item["AnimationLoop"];

		EffectInfo.bIncrement = item["Increment"];

		EffectInfo.fAnimationSpeed.x = item["AnimationSpeed"]["x"];
		EffectInfo.fAnimationSpeed.y = item["AnimationSpeed"]["y"];
#pragma endregion

#pragma region 알파
		EffectInfo.fAlphaStart.x = item["AlphaStart"]["x"];
		EffectInfo.fAlphaStart.y = item["AlphaStart"]["y"];

		EffectInfo.bAlphaCreate = item["AlphaCreate"];

		EffectInfo.bAlphaDelete = item["AlphaDelete"];

		EffectInfo.fAlphaSpeed.x = item["AlphaSpeed"]["x"];
		EffectInfo.fAlphaSpeed.y = item["AlphaSpeed"]["y"];

		EffectInfo.bAlphaChange = item["AlphaChange"];

		EffectInfo.bAlphaIn = item["AlphaIn"];

		EffectInfo.fAlphaChangeStartDelay.x = item["AlphaChangeStartDelay"]["x"];
		EffectInfo.fAlphaChangeStartDelay.y = item["AlphaChangeStartDelay"]["y"];
#pragma endregion

#pragma region 색상
		EffectInfo.bColorRandom = item["ColorRandom"];

		EffectInfo.fColorS.x = item["ColorS"]["x"];
		EffectInfo.fColorS.y = item["ColorS"]["y"];
		EffectInfo.fColorS.z = item["ColorS"]["z"];
		EffectInfo.fColorS.w = item["ColorS"]["w"];

		EffectInfo.bColorChange = item["ColorChange"];

		EffectInfo.bColorChangeRandom = item["ColorChangeRandom"];

		EffectInfo.fColorChangeRandomTime.x = item["ColorChangeRandomTime"]["x"];
		EffectInfo.fColorChangeRandomTime.y = item["ColorChangeRandomTime"]["y"];

		EffectInfo.bColorLoop = item["ColorChangeRandom"];

		EffectInfo.fColorChangeStartDelay.x = item["ColorChangeStartDelay"]["x"];
		EffectInfo.fColorChangeStartDelay.y = item["ColorChangeStartDelay"]["y"];

		EffectInfo.fColorChangeStartM.x = item["ColorChangeStartM"]["x"];
		EffectInfo.fColorChangeStartM.y = item["ColorChangeStartM"]["y"];

		EffectInfo.fColorM.x = item["ColorM"]["x"];
		EffectInfo.fColorM.y = item["ColorM"]["y"];
		EffectInfo.fColorM.z = item["ColorM"]["z"];
		EffectInfo.fColorM.w = item["ColorM"]["w"];

		EffectInfo.fColorChangeStartF.x = item["ColorChangeStartF"]["x"];
		EffectInfo.fColorChangeStartF.y = item["ColorChangeStartF"]["y"];

		EffectInfo.fColorF.x = item["ColorF"]["x"];
		EffectInfo.fColorF.y = item["ColorF"]["y"];
		EffectInfo.fColorF.z = item["ColorF"]["z"];
		EffectInfo.fColorF.w = item["ColorF"]["w"];

		EffectInfo.fColorDuration.x = item["ColorDuration"]["x"];
		EffectInfo.fColorDuration.y = item["ColorDuration"]["y"];
#pragma endregion

#pragma region 블러
		EffectInfo.bBloomPowerRandom = item["BloomPowerRandom"];

		EffectInfo.fBloomPower.x = item["BloomPower"]["x"];
		EffectInfo.fBloomPower.y = item["BloomPower"]["y"];
		EffectInfo.fBloomPower.z = item["BloomPower"]["z"];
		EffectInfo.fBloomPower.w = item["BloomPower"]["w"];

		EffectInfo.bBlurPowerRandom = item["BlurPowerRandom"];

		EffectInfo.fBlurPower = item["BlurPower"];
#pragma endregion

#pragma region 기타 정보
		EffectInfo.iShaderPass = item["ShaderPass"];

		EffectInfo.fAlpha_Discard = item["Alpha_Discard"];

		EffectInfo.fBlack_Discard.x = item["Black_Discard"]["x"];
		EffectInfo.fBlack_Discard.y = item["Black_Discard"]["y"];
		EffectInfo.fBlack_Discard.z = item["Black_Discard"]["z"];
#pragma endregion

#pragma region 디스토션
		EffectInfo.strDistortionTetextureName = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(item["DistortionTetextureName"]));
		EffectInfo.iDistortionIndex = item["DistortionIndex"];
		EffectInfo.vDistortionPower.x = item["DistortionPower"]["x"];
		EffectInfo.vDistortionPower.y = item["DistortionPower"]["y"];
		EffectInfo.vDistortionPower.z = item["DistortionPower"]["z"];
		EffectInfo.vDistortionPower.w = item["DistortionPower"]["w"];
#pragma endregion
	}
#pragma endregion

	// 적용
	static_cast<CEffect*>(m_pEffect)->Set_EffectDesc(EffectInfo);
	Load_ObjectInfo(TYPE_EFFECT);

	MSG_BOX("Effect_Load_Success!");
}

void CTool_Effect::Save_Decal(const char* pFileName)
{
	// 기본 정보
	if (m_pDecal == nullptr)
	{
		MSG_BOX("Decal_Save_Failed!");
		return;
	}

	//m_tDecalInfo = static_cast<CDecal*>(m_pDecal)->Get_DecalDesc();

	Json json;
	json["DecalInfo"].push_back({

		{"Scale", {
			{"x", m_tDecalInfo.fScale.x},
			{"y", m_tDecalInfo.fScale.y},
			{"z", m_tDecalInfo.fScale.z}}
		},

		{"LifeTime", m_tDecalInfo.fLifeTime},

		{"TextureIndexDiffuse", m_tDecalInfo.iTextureIndexDiffuse},

		{"ShaderPass", m_tDecalInfo.iShaderPass},
		{"Alpha_Discard", m_tDecalInfo.fAlpha_Discard},
		{"Black_Discard", {
			{"x", m_tDecalInfo.fBlack_Discard.x},
			{"y", m_tDecalInfo.fBlack_Discard.y},
			{"z", m_tDecalInfo.fBlack_Discard.z}}
		},

		{"BloomPower", {
			{"x", m_tDecalInfo.fBloomPower.x},
			{"y", m_tDecalInfo.fBloomPower.y},
			{"z", m_tDecalInfo.fBloomPower.z}}
		},
		{"BlurPower", m_tDecalInfo.fBlurPower},

		{"ColorAdd_01_Alpha", m_tDecalInfo.fColorAdd_01_Alpha},
		{"ColorAdd_01", {
			{"x", m_tDecalInfo.fColorAdd_01.x},
			{"y", m_tDecalInfo.fColorAdd_01.y},
			{"z", m_tDecalInfo.fColorAdd_01.z}}
		},
		{"ColorAdd_02", {
			{"x", m_tDecalInfo.fColorAdd_02.x},
			{"y", m_tDecalInfo.fColorAdd_02.y},
			{"z", m_tDecalInfo.fColorAdd_02.z}}
		},

		{"AlphaRemove", m_tDecalInfo.fAlphaRemove},
		{"AlphaCreate", m_tDecalInfo.bAlphaCreate},
		{"AlphaDelete", m_tDecalInfo.bAlphaDelete},
		{"AlphaSpeed", m_tDecalInfo.fAlphaSpeed},

		});

	wstring strFileName(pFileName, pFileName + strlen(pFileName));
	wstring strFilePath = L"../Bin/DataFiles/Vfx/Decal/" + strFileName + L".json";
	GI->Json_Save(strFilePath, json);

	MSG_BOX("Decal_Save_Success!");

	// 추가 정보 따로 저장
}

void CTool_Effect::Load_Decal(const char* pFileName)
{
	if (m_pDecal == nullptr)
		Create_Object(TYPE_DECAL);

	wstring strFileName(pFileName, pFileName + strlen(pFileName));
	wstring strFilePath = L"../Bin/DataFiles/Vfx/Decal/" + strFileName + L".json";

#pragma region Load
	CDecal::DECAL_DESC DecalInfo = {};

	Json json = GI->Json_Load(strFilePath);
	for (const auto& item : json["DecalInfo"])
	{
		DecalInfo.fScale.x = item["Scale"]["x"];
		DecalInfo.fScale.y = item["Scale"]["y"];
		DecalInfo.fScale.z = item["Scale"]["z"];

		DecalInfo.fLifeTime = item["LifeTime"];

		DecalInfo.iTextureIndexDiffuse = item["TextureIndexDiffuse"];

		DecalInfo.iShaderPass = item["ShaderPass"];
		DecalInfo.fAlpha_Discard = item["Alpha_Discard"];
		DecalInfo.fBlack_Discard.x = item["Black_Discard"]["x"];
		DecalInfo.fBlack_Discard.y = item["Black_Discard"]["y"];
		DecalInfo.fBlack_Discard.z = item["Black_Discard"]["z"];

		DecalInfo.fBloomPower.x = item["BloomPower"]["x"];
		DecalInfo.fBloomPower.y = item["BloomPower"]["y"];
		DecalInfo.fBloomPower.z = item["BloomPower"]["z"];
		DecalInfo.fBlurPower = item["BlurPower"];

		DecalInfo.fColorAdd_01_Alpha = item["ColorAdd_01_Alpha"];
		DecalInfo.fColorAdd_01.x = item["ColorAdd_01"]["x"];
		DecalInfo.fColorAdd_01.y = item["ColorAdd_01"]["y"];
		DecalInfo.fColorAdd_01.z = item["ColorAdd_01"]["z"];
		DecalInfo.fColorAdd_02.x = item["ColorAdd_02"]["x"];
		DecalInfo.fColorAdd_02.y = item["ColorAdd_02"]["y"];
		DecalInfo.fColorAdd_02.z = item["ColorAdd_02"]["z"];

		DecalInfo.fAlphaRemove = item["AlphaRemove"];
		DecalInfo.bAlphaCreate = item["AlphaCreate"];
		DecalInfo.bAlphaDelete = item["AlphaDelete"];
		DecalInfo.fAlphaSpeed = item["AlphaSpeed"];
	}
#pragma endregion

	// 적용
	static_cast<CDecal*>(m_pDecal)->Set_DecalDesc(DecalInfo);
	Load_ObjectInfo(TYPE_DECAL);

	MSG_BOX("Decal_Load_Success!");
}
#pragma endregion


CTool_Effect* CTool_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Effect* pInstance = new CTool_Effect(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed_Create : CTool_Effect");
		return nullptr;
	}

	return pInstance;
}

void CTool_Effect::Free()
{
	__super::Free();
}
