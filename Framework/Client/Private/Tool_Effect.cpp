#include "stdafx.h"
#include "Tool_Effect.h"
#include "imgui.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "filesystem"
#include "FileUtils.h"
#include "Utils.h"

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

void CTool_Effect::Tick(_float fTimeDelta)
{
	ImGui::Begin("Effect_Tool");

	// 생성/ 삭제
	if (ImGui::Button("EffectCreate"))
		Create_Effect();
	ImGui::SameLine();
	if (ImGui::Button("EffectDelete")) 
	{
		if (m_pEffect != nullptr) 
		{
			m_pEffect->Set_Dead(true);
			m_pEffect = nullptr;
		}
	}
	ImGui::SameLine();

	// 확인(적용)
	if (ImGui::Button("TransformSelect"))
		Store_TransformEffect();
	ImGui::SameLine();
	if (ImGui::Button("EffectInfoSelect"))
		Store_InfoEffect();
	ImGui::NewLine();

	// 저장하기/ 불러오기 -> 추후 구현
	if (ImGui::CollapsingHeader("EffectBinary"))
	{
		if (ImGui::Button("SaveEffect"))
			Save_Effect(m_cBinaryName);
		ImGui::SameLine();
		if (ImGui::Button("LoadEffect"))
			Load_Effect(m_cBinaryName);
		ImGui::SameLine();
		ImGui::Text("FileName :");
		ImGui::SameLine();
		ImGui::InputText("##FileName", m_cBinaryName, IM_ARRAYSIZE(m_cBinaryName));
	}

	// 트랜스폼
	if (ImGui::CollapsingHeader("EffectTransform"))
	{
		ImGui::Text("Position");
		ImGui::InputFloat3("##Position", m_fPosition);

		ImGui::Text("Rotation");
		ImGui::InputFloat3("##Rotation", m_fRotation);

		ImGui::Text("Scale");
		ImGui::InputFloat3("##Scale", m_fScale);
	}


	// 기본 정보
	if (ImGui::CollapsingHeader("EffectBasicInfo"))
	{
		// 이펙트 타입
		ImGui::Text("EffectType :");
		ImGui::SameLine();
		if (ImGui::Combo("##EffectType", &m_iEffectTypeIndex, m_cEffectType, IM_ARRAYSIZE(m_cEffectType)))
			Store_InfoEffect();

		// 증력 여부
		ImGui::Checkbox("EffectGravity", &m_tEffectInfo.bGravity);
		ImGui::NewLine();
	}

	// 위치 (분포 범위)
	if (ImGui::CollapsingHeader("EffectPosition"))
	{
		ImGui::Text("RangeEffect");
		if (ImGui::InputFloat3("##RangeEffect", m_fEffectRange))
			Store_InfoEffect();
		ImGui::NewLine();

		// 시작 거리
		ImGui::Text("RangeDistance");
		if (ImGui::InputFloat2("##RangeDistance", m_fEffectRangeDistance))
			Store_InfoEffect();
		ImGui::NewLine();
	}

	// 크기
	if (ImGui::CollapsingHeader("EffectScale"))
	{
		ImGui::Checkbox("ScaleSameRate", &m_tEffectInfo.bScaleSameRate);
		ImGui::NewLine();

		ImGui::Text("ScaleStartEffect");
		if (ImGui::InputFloat2("##ScaleStartEffect", m_fEffectScaleStart))
			Store_InfoEffect();
		ImGui::NewLine();


		ImGui::Text("ScaleDir");
		if (ImGui::InputFloat3("##ScaleDir", m_fEffectScaleDir))
			Store_InfoEffect();

		ImGui::Text("ScaleSpeed");
		if (ImGui::InputFloat("##ScaleSpeed", &m_tEffectInfo.fScaleSpeed))
			Store_InfoEffect();


		//ImGui::Text("ScaleStartParticles");
		//if (ImGui::InputFloat2("##ScaleStartParticles", m_fParticleScaleStart))
		//	Store_InfoParticle();
		//ImGui::NewLine();

		//ImGui::Checkbox("ScaleChange", &m_tParticleInfo.bScaleChange);
		//ImGui::NewLine();

		//if (m_tParticleInfo.bScaleChange)
		//{
		//	ImGui::Text("ScaleChangeStartDelayParticles");
		//	ImGui::InputFloat2("##ScaleChangeStartDelayParticles", m_fParticleScaleChangeStartDelay);
		//	ImGui::NewLine();

		//	ImGui::Checkbox("ScaleChangeRandom", &m_tParticleInfo.bScaleChangeRandom);
		//	ImGui::NewLine();

		//	if (m_tParticleInfo.bScaleChangeRandom)
		//	{
		//		ImGui::Text("ScaleChangeTimeParticles");
		//		ImGui::InputFloat2("##ScaleChangeTimeParticles", m_fParticleScaleChangeTime);
		//		ImGui::NewLine();
		//	}
		//	else
		//	{
		//		ImGui::Checkbox("ScaleAdd", &m_tParticleInfo.bScaleAdd);
		//		ImGui::NewLine();

		//		ImGui::Checkbox("ScaleLoop", &m_tParticleInfo.bScaleLoop);
		//		ImGui::NewLine();

		//		ImGui::Checkbox("ScaleLoopStart", &m_tParticleInfo.bScaleLoopStart);
		//		ImGui::NewLine();

		//		ImGui::Text("ScaleMinParticles");
		//		ImGui::InputFloat2("##ScaleMinParticles", m_fParticleScaleMin);
		//		ImGui::NewLine();

		//		ImGui::Text("ScaleMaxParticles");
		//		ImGui::InputFloat2("##ScaleMaxParticles", m_fParticleScaleMax);
		//		ImGui::NewLine();
		//	}

		//	ImGui::Text("ScaleSpeedParticles");
		//	ImGui::InputFloat2("##ScaleSpeedParticles", m_fParticleScaleSpeed);
		//	ImGui::NewLine();
		//}
	}

	// 이동(힘)
	if (ImGui::CollapsingHeader("EffectVelocity"))
	{
		ImGui::Text("VelocityDir");
		if (ImGui::InputFloat3("##VelocityDir", m_fEffectMoveDir))
			Store_InfoEffect();

		ImGui::Text("VelocitySpeed");
		if (ImGui::InputFloat("##VelocitySpeed", &m_tEffectInfo.fMoveSpeed))
			Store_InfoEffect();

		//ImGui::Text("VelocitySpeed");
		//ImGui::InputFloat2("##VelocitySpeed", m_fParticleSpeed);
		//ImGui::NewLine();

		//ImGui::Text("VelocityMinParticles");
		//ImGui::InputFloat3("##VelocityMinParticles", m_fParticleVelocityMin);
		//ImGui::Text("VelocityMaxParticles");
		//ImGui::InputFloat3("##VelocityMaxParticles", m_fParticleVelocityMax);
		//ImGui::NewLine();

		//ImGui::Checkbox("VelocityChange", &m_tParticleInfo.bVelocityChange);
		//ImGui::NewLine();

		//if (m_tParticleInfo.bVelocityChange)
		//{
		//	ImGui::Text("VelocityChangeStartDelayParticles");
		//	ImGui::InputFloat2("##VelocityChangeStartDelayParticles", m_fParticleVelocityChangeStartDelay);
		//	ImGui::NewLine();

		//	ImGui::Checkbox("VelocityChangeRandom", &m_tParticleInfo.bVelocityChangeRandom);
		//	ImGui::NewLine();

		//	if (m_tParticleInfo.bVelocityChangeRandom)
		//	{
		//		ImGui::Text("VelocityChangeTimeParticles");
		//		ImGui::InputFloat2("##VelocityChangeTimeParticles", m_fParticleVelocityChangeTime);
		//		ImGui::NewLine();

		//		if (m_tParticleInfo.iVelocityUse != 0)
		//			m_tParticleInfo.iVelocityUse = 0;
		//	}
		//	else
		//	{
		//		ImGui::Text("VelocityUseParticles");
		//		ImGui::InputInt("##VelocityUseParticles", &(_int)m_tParticleInfo.iVelocityUse);

		//		ImGui::Checkbox("VelocityLoop", &m_tParticleInfo.bVelocityLoop);
		//		ImGui::NewLine();

		//		ImGui::Text("VelocityCountCur");
		//		ImGui::InputInt("##VelocityCountCur", &(_int)m_tParticleInfo.iVelocityCountCur);
		//		ImGui::NewLine();

		//		ImGui::Text("iVelocityCountMax");
		//		if (ImGui::InputInt("##iVelocityCountMax", &(_int)m_tParticleInfo.iVelocityCountMax))
		//		{
		//			if (m_tParticleInfo.iVelocityCountMax == 1)
		//				m_tParticleInfo.bVelocityChange = false;
		//			else if (m_tParticleInfo.iVelocityCountMax > 10)
		//				m_tParticleInfo.bVelocityChange = false;
		//		}
		//		ImGui::NewLine();

		//		if (m_tParticleInfo.iVelocityCountMax > 1)
		//		{
		//			ImGui::Text("VelocityMinParticles_01");
		//			ImGui::InputFloat3("##VelocityMinParticles_01", m_fParticleVelocityMin);

		//			ImGui::Text("VelocityMaxParticles_01");
		//			ImGui::InputFloat3("##VelocityMaxParticles_01", m_fParticleVelocityMax);

		//			ImGui::Text("VelocityChangeTimeParticles_01");
		//			ImGui::InputFloat2("##VelocityChangeTimeParticles_01", m_fParticleVelocityTime_01);


		//			ImGui::Text("VelocityMinParticles_02");
		//			ImGui::InputFloat3("##VelocityMinParticles_02", m_fParticleVelocityMin_02);

		//			ImGui::Text("VelocityMaxParticles_02");
		//			ImGui::InputFloat3("##VelocityMaxParticles_02", m_fParticleVelocityMax_02);

		//			ImGui::Text("VelocityChangeTimeParticles_02");
		//			ImGui::InputFloat2("##VelocityChangeTimeParticles_02", m_fParticleVelocityTime_02);
		//		}

		//		if (m_tParticleInfo.iVelocityCountMax > 2)
		//		{
		//			ImGui::Text("VelocityMinParticles_03");
		//			ImGui::InputFloat3("##VelocityMinParticles_03", m_fParticleVelocityMin_03);

		//			ImGui::Text("VelocityMaxParticles_03");
		//			ImGui::InputFloat3("##VelocityMaxParticles_03", m_fParticleVelocityMax_03);

		//			ImGui::Text("VelocityChangeTimeParticles_03");
		//			ImGui::InputFloat2("##VelocityChangeTimeParticles_03", m_fParticleVelocityTime_03);
		//		}

		//		if (m_tParticleInfo.iVelocityCountMax > 3)
		//		{
		//			ImGui::Text("VelocityMinParticles_04");
		//			ImGui::InputFloat3("##VelocityMinParticles_04", m_fParticleVelocityMin_04);

		//			ImGui::Text("VelocityMaxParticles_04");
		//			ImGui::InputFloat3("##VelocityMaxParticles_04", m_fParticleVelocityMax_04);

		//			ImGui::Text("VelocityChangeTimeParticles_04");
		//			ImGui::InputFloat2("##VelocityChangeTimeParticles_04", m_fParticleVelocityTime_04);
		//		}

		//		if (m_tParticleInfo.iVelocityCountMax > 4)
		//		{
		//			ImGui::Text("VelocityMinParticles_05");
		//			ImGui::InputFloat3("##VelocityMinParticles_05", m_fParticleVelocityMin_05);

		//			ImGui::Text("VelocityMaxParticles_05");
		//			ImGui::InputFloat3("##VelocityMaxParticles_05", m_fParticleVelocityMax_05);

		//			ImGui::Text("VelocityChangeTimeParticles_05");
		//			ImGui::InputFloat2("##VelocityChangeTimeParticles_05", m_fParticleVelocityTime_05);
		//		}

		//		if (m_tParticleInfo.iVelocityCountMax > 5)
		//		{
		//			ImGui::Text("VelocityMinParticles_06");
		//			ImGui::InputFloat3("##VelocityMinParticles_06", m_fParticleVelocityMin_06);

		//			ImGui::Text("VelocityMaxParticles_06");
		//			ImGui::InputFloat3("##VelocityMaxParticles_06", m_fParticleVelocityMax_06);

		//			ImGui::Text("VelocityChangeTimeParticles_06");
		//			ImGui::InputFloat2("##VelocityChangeTimeParticles_06", m_fParticleVelocityTime_06);
		//		}

		//		if (m_tParticleInfo.iVelocityCountMax > 6)
		//		{
		//			ImGui::Text("VelocityMinParticles_07");
		//			ImGui::InputFloat3("##VelocityMinParticles_07", m_fParticleVelocityMin_07);

		//			ImGui::Text("VelocityMaxParticles_07");
		//			ImGui::InputFloat3("##VelocityMaxParticles_07", m_fParticleVelocityMax_07);

		//			ImGui::Text("VelocityChangeTimeParticles_07");
		//			ImGui::InputFloat2("##VelocityChangeTimeParticles_07", m_fParticleVelocityTime_07);
		//		}

		//		if (m_tParticleInfo.iVelocityCountMax > 7)
		//		{
		//			ImGui::Text("VelocityMinParticles_08");
		//			ImGui::InputFloat3("##VelocityMinParticles_08", m_fParticleVelocityMin_08);

		//			ImGui::Text("VelocityMaxParticles_08");
		//			ImGui::InputFloat3("##VelocityMaxParticles_08", m_fParticleVelocityMax_08);

		//			ImGui::Text("VelocityChangeTimeParticles_08");
		//			ImGui::InputFloat2("##VelocityChangeTimeParticles_08", m_fParticleVelocityTime_08);
		//		}

		//		if (m_tParticleInfo.iVelocityCountMax > 8)
		//		{
		//			ImGui::Text("VelocityMinParticles_09");
		//			ImGui::InputFloat3("##VelocityMinParticles_09", m_fParticleVelocityMin_09);

		//			ImGui::Text("VelocityMaxParticles_09");
		//			ImGui::InputFloat3("##VelocityMaxParticles_09", m_fParticleVelocityMax_09);

		//			ImGui::Text("VelocityChangeTimeParticles_09");
		//			ImGui::InputFloat2("##VelocityChangeTimeParticles_09", m_fParticleVelocityTime_09);
		//		}

		//		if (m_tParticleInfo.iVelocityCountMax > 9)
		//		{
		//			ImGui::Text("VelocityMinParticles_10");
		//			ImGui::InputFloat3("##VelocityMinParticles_10", m_fParticleVelocityMin_10);

		//			ImGui::Text("VelocityMaxParticles_10");
		//			ImGui::InputFloat3("##VelocityMaxParticles_10", m_fParticleVelocityMax_10);

		//			ImGui::Text("VelocityChangeTimeParticles_10");
		//			ImGui::InputFloat2("##VelocityChangeTimeParticles_10", m_fParticleVelocityTime_10);
		//		}
		//	}
		//}
		//else
		//	m_tParticleInfo.iVelocityUse = 0;
	}

	// 회전
	if (ImGui::CollapsingHeader("EffectRotation"))
	{
		if (ImGui::Checkbox("RotationBillboard", &m_tEffectInfo.bBillboard))
			Store_InfoEffect();
		ImGui::NewLine();

		ImGui::Text("RotationDir");
		if (ImGui::InputFloat3("##RotationDir", m_fEffectRotationDir))
			Store_InfoEffect();

		ImGui::Text("RotationSpeed");
		if (ImGui::InputFloat("##RotationSpeed", &m_tEffectInfo.fTurnSpeed))
			Store_InfoEffect();

		//ImGui::Checkbox("RotationRandomAxis", &m_tParticleInfo.bRandomAxis);
		//if (!m_tParticleInfo.bRandomAxis)
		//{
		//	ImGui::Text("RotationAxis");
		//	ImGui::InputFloat3("##RotationAxis", m_fParticleAxis);
		//	ImGui::NewLine();
		//}
		//ImGui::NewLine();

		//ImGui::Checkbox("RotationRandomAngle", &m_tParticleInfo.bRandomAngle);
		//if (!m_tParticleInfo.bRandomAngle)
		//{
		//	ImGui::Text("RotationAngle");
		//	ImGui::InputFloat("##RangeParticles", &m_tParticleInfo.fAngle);
		//	ImGui::NewLine();
		//}
		//ImGui::NewLine();

		//ImGui::Checkbox("RotationChange", &m_tParticleInfo.bRotationChange);
		//ImGui::NewLine();

		//if (m_tParticleInfo.bRotationChange)
		//{
		//	ImGui::Text("RotationChangeStartDelayParticles");
		//	ImGui::InputFloat2("##RotationChangeStartDelayParticles", m_fParticleRotationChangeStartDelay);
		//	ImGui::NewLine();

		//	ImGui::Text("RotationSpeed");
		//	ImGui::InputFloat2("##RotationSpeed", m_fParticleRotationSpeed);
		//	ImGui::NewLine();

		//	ImGui::Checkbox("RotationChangeRandom", &m_tParticleInfo.bRotationChangeRandom);
		//	ImGui::NewLine();

		//	if (m_tParticleInfo.bRotationChangeRandom)
		//	{
		//		ImGui::Text("RotationChangeTime");
		//		ImGui::InputFloat2("##RotationChangeTime", m_fParticleRotationChangeTime);
		//		ImGui::NewLine();
		//	}
		//	else
		//	{
		//		ImGui::Checkbox("RotationAddRandom", &m_tParticleInfo.bRotationAdd);
		//		ImGui::NewLine();
		//	}
		//}
	}

	// 지속 시간
	if (ImGui::CollapsingHeader("EffectLifeTime"))
	{
		ImGui::Text("EffectLifeTime");
		ImGui::InputFloat("##EffectLifeTime", &m_tEffectInfo.fLifeTime);
		ImGui::NewLine();
	}

	// 박스 범위
	if (ImGui::CollapsingHeader("ParticleBox"))
	{
		//if (ImGui::Checkbox("UseBox", &m_tParticleInfo.bUseBox))
		//	Store_InfoParticle();

		//if (m_tParticleInfo.bUseBox)
		//{
		//	ImGui::Text("BoxMinParticles");
		//	ImGui::InputFloat3("##BoxMinParticles", m_fParticleBoxMin);
		//	ImGui::Text("BoxMaxParticles");
		//	ImGui::InputFloat3("##BoxMaxParticles", m_fParticleBoxMax);
		//	ImGui::NewLine();
		//}
	}

	// 텍스처 지정
	if (ImGui::CollapsingHeader("EffectResource"))
	{
		// 모델 원형
		ImGui::Text("ModelName :");
		ImGui::SameLine();
		ImGui::InputText("##ModelName", m_cModelName, IM_ARRAYSIZE(m_cModelName));
		ImGui::NewLine();

		// 디퓨즈 텍스처
		ImGui::Text("DiffuseFolderName :");
		ImGui::SameLine();
		if(ImGui::Combo("##DiffuseFolderName", &m_iDiffuseFolderIndex, m_cFolderName, IM_ARRAYSIZE(m_cFolderName)))
			Store_InfoEffect();

		ImGui::Text("DiffuseTextureIndex");
		ImGui::SameLine();
		if (ImGui::InputInt("##DiffuseTextureIndex", &(_int)m_tEffectInfo.iTextureIndexDiffuse))
			Store_InfoEffect();
		ImGui::NewLine();

		// 알파 텍스처
		ImGui::Text("AlphaFolderName :");
		ImGui::SameLine();
		if (ImGui::Combo("##AlphaFolderName", &m_iAlphaFolderIndex, m_cFolderName, IM_ARRAYSIZE(m_cFolderName)))
			Store_InfoEffect();

		ImGui::Text("AlphaTextureIndex");
		ImGui::SameLine();
		if (ImGui::InputInt("##AlphaTextureIndex", &(_int)m_tEffectInfo.iTextureIndexAlpha))
			Store_InfoEffect();
		ImGui::NewLine();

		// 디퓨즈 && 알파 UVIndex && UVMaxCount
		ImGui::Checkbox("UVIndexRandomStart", &m_tEffectInfo.bRandomStartIndex);
		if (!m_tEffectInfo.bRandomStartIndex)
		{
			ImGui::Text("UVIndex");
			if (ImGui::InputFloat2("##UVIndex", m_ffEffectUVIndex))
				Store_InfoEffect();
			ImGui::NewLine();
		}
		ImGui::Text("UVMaxCount");
		if (ImGui::InputFloat2("##UVMaxCount", m_ffEffectUVMaxCount))
			Store_InfoEffect();
		ImGui::NewLine();

		// UVFlow
		ImGui::Text("UVFlowDir X Speed");
		if (ImGui::InputFloat2("##UVFlowDir", m_fEffectUVFlow))
			Store_InfoEffect();
		ImGui::NewLine();

		// UVLoop
		ImGui::Text("UVLoop : 0 < UVLoop -> NoLoop");
		if (ImGui::InputInt("##UVLoop", &(_int)m_tEffectInfo.iUVLoop))
			Store_InfoEffect();
		ImGui::NewLine();
	}

	// 텍스처 애니메이션
	if (ImGui::CollapsingHeader("EffectAnimation"))
	{
		if (ImGui::Checkbox("UseAnimation", &m_tEffectInfo.bAnimation))
			Store_InfoEffect();

		if (m_tEffectInfo.bAnimation)
		{
			// 반복 여부
			if (ImGui::Checkbox("LoopAnimation", &m_tEffectInfo.bLoop))
				Store_InfoEffect();

			if (ImGui::Checkbox("IncrementAnimation", &m_tEffectInfo.bIncrement))
				Store_InfoEffect();

			ImGui::Text("SpeedAnimation");
			if (ImGui::InputFloat("##SpeedAnimation", &m_tEffectInfo.fIndexSpeed))
				Store_InfoEffect();
		}
	}

	// 텍스처 알파
	if (ImGui::CollapsingHeader("EffectAlpha"))
	{
		ImGui::Text("StartAlpha");
		if (ImGui::InputFloat2("##StartAlpha", m_fAlphaStart))
			Store_InfoEffect();

		if (ImGui::Checkbox("AlphaCreate", &m_tEffectInfo.bAlphaCreate))
			Store_InfoEffect();
		ImGui::NewLine();

		if (ImGui::Checkbox("AlphaDelete", &m_tEffectInfo.bAlphaDelete))
			Store_InfoEffect();
		ImGui::NewLine();

		ImGui::Text("AlphaSpeed");
		if (ImGui::InputFloat2("##AlphaSpeed", m_fAlphaSpeed))
			Store_InfoEffect();
		ImGui::NewLine();

		ImGui::Checkbox("AlphaChange", &m_tEffectInfo.bAlphaChange);
		ImGui::NewLine();

		// StartDelay
		if (m_tEffectInfo.bAlphaChange)
		{
			ImGui::Text("AlphaChangeStartDelay");
			ImGui::InputFloat2("##AlphaChangeStartDelay", m_fAlphaChangeStartDelay);
			ImGui::NewLine();

			ImGui::Checkbox("AlphaIn", &m_tEffectInfo.bAlphaIn);
			ImGui::NewLine();
		}
	}

	// 파티클 색상
	if (ImGui::CollapsingHeader("ParticleColor"))
	{
		if (!m_tEffectInfo.bColorRandom)
		{
			ImGui::Text("ColorStart");
			if (ImGui::ColorEdit4("##ColorStart", (float*)&m_tEffectInfo.vAdditiveDiffuseColor, ImGuiColorEditFlags_Float))
				Store_InfoEffect();
		}
		if (ImGui::Checkbox("ColorRandom", &m_tEffectInfo.bColorRandom))
			Store_InfoEffect();
		ImGui::NewLine();


		//ImGui::Checkbox("ColorChange", &m_tParticleInfo.bColorChange);
		//if (m_tParticleInfo.bColorChange)
		//{
		//	ImGui::Checkbox("ColorChangeRandom", &m_tParticleInfo.bColorChangeRandom);
		//	ImGui::NewLine();

		//	// fColorChangeStartDelay
		//	ImGui::Text("ColorChangeStartDelay");
		//	ImGui::InputFloat2("##ColorChangeStartDelay", m_fColorChangeStartDelay);
		//	ImGui::NewLine();

		//	if (m_tParticleInfo.bColorChangeRandom)
		//	{
		//		ImGui::Text("ColorChangeRandomTime");
		//		ImGui::InputFloat2("##ColorChangeRandomTime", m_fColorChangeRandomTime);
		//		ImGui::NewLine();
		//	}
		//	else
		//	{
		//		ImGui::Checkbox("ColorLoop", &m_tParticleInfo.bColorLoop);
		//		ImGui::NewLine();

		//		// fColorChangeStartM
		//		ImGui::Text("ColorChangeStartM");
		//		ImGui::InputFloat2("##ColorChangeStartM", m_fColorChangeStartM);

		//		ImGui::Text("ColorMiddle");
		//		ImGui::ColorEdit4("##ColorMiddle", (float*)&m_tParticleInfo.fColorM, ImGuiColorEditFlags_Float);
		//		ImGui::NewLine();

		//		// fColorChangeStartF
		//		ImGui::Text("ColorChangeStartF");
		//		ImGui::InputFloat2("##ColorChangeStartF", m_fColorChangeStartF);

		//		ImGui::Text("ColorFinal");
		//		ImGui::ColorEdit4("##ColorFinal", (float*)&m_tParticleInfo.fColorF, ImGuiColorEditFlags_Float);
		//		ImGui::NewLine();
		//	}

		//	// ColorDurationTime
		//	ImGui::Text("ColorDurationTime");
		//	ImGui::InputFloat2("##ColorDurationTime", m_fColorChangeStartE);
		//}
	}

	// 쉐이더 값 셋팅
	if (ImGui::CollapsingHeader("EffectShader"))
	{
		// 쉐이더 패스
		ImGui::Text("ShaderPass");
		if (ImGui::InputInt("##ShaderPass", &(_int)m_tEffectInfo.iShaderPass))
			Store_InfoEffect();
		ImGui::NewLine();

		//// 디스카드 값 셋팅
		//ImGui::Text("Alpha_Discard");
		//if (ImGui::InputFloat("##Alpha_Discard", &m_tParticleInfo.fAlpha_Discard))
		//	Store_InfoParticle();
		//ImGui::Text("Black_Discard");
		//if (ImGui::InputFloat3("##Black_Discard", m_fBlack_Discard))
		//	Store_InfoParticle();
		//ImGui::NewLine();

		//// 블러 셋팅
		//ImGui::Checkbox("BlurColorRandom", &m_tParticleInfo.bBlurColorRandom);
		//ImGui::NewLine();
		//if (!m_tParticleInfo.bBlurColorRandom)
		//{
		//	ImGui::Text("BlurColor");
		//	if (ImGui::ColorEdit4("##BlurColor", (float*)&m_tParticleInfo.fBlurColor, ImGuiColorEditFlags_Float))
		//		Store_InfoParticle();
		//	ImGui::NewLine();
		//}

		//ImGui::Checkbox("BlurPowerRandom", &m_tParticleInfo.bBlurPowerRandom);
		//ImGui::NewLine();
		//if (!m_tParticleInfo.bBlurPowerRandom)
		//{
		//	if (ImGui::InputFloat("##BlurPower", &m_tParticleInfo.fBlurPower))
		//		Store_InfoParticle();
		//}
	}

	ImGui::End();
}

void CTool_Effect::Create_Effect()
{
	// 생성
	if (m_pEffect != nullptr)
		return;

	m_pEffect = GI->Clone_GameObject(TEXT("Prototype_TempMeshEffect"), LAYER_TYPE::LAYER_EFFECT);
	GI->Add_GameObject(LEVEL_TOOL, LAYER_TYPE::LAYER_EFFECT, m_pEffect);

	Load_InfoEffect();
}

void CTool_Effect::Store_TransformEffect()
{
	if (m_pEffect == nullptr)
		return;

	CTransform* pTransform = m_pEffect->Get_Component<CTransform>(L"Com_Transform");

	pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fPosition[0], m_fPosition[1], m_fPosition[2], 1.f));
	pTransform->FixRotation(m_fRotation[0], m_fRotation[1], m_fRotation[2]);
	pTransform->Set_Scale(_float3(m_fScale[0], m_fScale[1], m_fScale[2]));
}


void CTool_Effect::Load_InfoEffect() // Load
{
	if (m_pEffect == nullptr)
		return;

	CTransform* pTransform = m_pEffect->Get_Component<CTransform>(L"Com_Transform");

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


	m_tEffectInfo = static_cast<CEffect*>(m_pEffect)->Get_EffectDesc();


	if (m_tEffectInfo.eType = CEffect::EFFECT_TEXTURE)
		m_iEffectTypeIndex = 0;
	else if (m_tEffectInfo.eType = CEffect::EFFECT_MESH)
		m_iEffectTypeIndex = 1;
	else
		m_iEffectTypeIndex = 2;


	m_fEffectRange[0] = m_tEffectInfo.fRange.x;
	m_fEffectRange[1] = m_tEffectInfo.fRange.y;
	m_fEffectRange[2] = m_tEffectInfo.fRange.z;

	m_fEffectRangeDistance[0] = m_tEffectInfo.fRangeDistance.x;
	m_fEffectRangeDistance[1] = m_tEffectInfo.fRangeDistance.y;


	m_fEffectScaleStart[0] = m_tEffectInfo.fScaleStart.x;
	m_fEffectScaleStart[1] = m_tEffectInfo.fScaleStart.y;

	m_fEffectScaleDir[0] = m_tEffectInfo.fScaleDir.x;
	m_fEffectScaleDir[1] = m_tEffectInfo.fScaleDir.y;
	m_fEffectScaleDir[2] = m_tEffectInfo.fScaleDir.z;


	m_fEffectMoveDir[0] = m_tEffectInfo.fMoveDir.x;
	m_fEffectMoveDir[1] = m_tEffectInfo.fMoveDir.y;
	m_fEffectMoveDir[2] = m_tEffectInfo.fMoveDir.z;

	m_fEffectRotationDir[0] = m_tEffectInfo.fTurnDir.x;
	m_fEffectRotationDir[1] = m_tEffectInfo.fTurnDir.y;
	m_fEffectRotationDir[2] = m_tEffectInfo.fTurnDir.z;


	size_t convertedChars = 0;
	wcstombs_s(&convertedChars, m_cModelName, sizeof(m_cModelName), m_tEffectInfo.strModelName.c_str(), _TRUNCATE);

	m_iDiffuseFolderIndex = Get_FolderIndex(m_tEffectInfo.strDiffuseTetextureName);
	m_iAlphaFolderIndex = Get_FolderIndex(m_tEffectInfo.strAlphaTexturName);

	m_ffEffectUVIndex[0] = m_tEffectInfo.fUVIndex.x;
	m_ffEffectUVIndex[1] = m_tEffectInfo.fUVIndex.y;
	m_ffEffectUVMaxCount[0] = m_tEffectInfo.fMaxCount.x;
	m_ffEffectUVMaxCount[1] = m_tEffectInfo.fMaxCount.y;

	m_fEffectUVFlow[0] = m_tEffectInfo.fUVFlow.x;
	m_fEffectUVFlow[1] = m_tEffectInfo.fUVFlow.y;

	m_fAlphaStart[0] = m_tEffectInfo.fAlphaStart.x;
	m_fAlphaStart[1] = m_tEffectInfo.fAlphaStart.y;
	m_fAlphaSpeed[0] = m_tEffectInfo.fAlphaSpeed.x;
	m_fAlphaSpeed[1] = m_tEffectInfo.fAlphaSpeed.y;
	m_fAlphaChangeStartDelay[0] = m_tEffectInfo.fAlphaChangeStartDelay.x;
	m_fAlphaChangeStartDelay[1] = m_tEffectInfo.fAlphaChangeStartDelay.y;
}

void CTool_Effect::Store_InfoEffect() // Save
{
	if (m_pEffect == nullptr)
		return;


	if (m_iEffectTypeIndex == 0)
		m_tEffectInfo.eType = CEffect::EFFECT_TEXTURE;
	else if (m_iEffectTypeIndex == 1)
		m_tEffectInfo.eType = CEffect::EFFECT_MESH;
	else
		m_tEffectInfo.eType = CEffect::EFFECT_END;



	m_tEffectInfo.fRange         = _float3(m_fEffectRange[0], m_fEffectRange[1], m_fEffectRange[2]);
	m_tEffectInfo.fRangeDistance = _float2(m_fEffectRangeDistance[0], m_fEffectRangeDistance[1]);


	m_tEffectInfo.fScaleStart = _float2(m_fEffectScaleStart[0], m_fEffectScaleStart[1]);
	m_tEffectInfo.fScaleDir   = _float3(m_fEffectScaleDir[0], m_fEffectScaleDir[1], m_fEffectScaleDir[2]);


	m_tEffectInfo.fMoveDir = _float3(m_fEffectMoveDir[0], m_fEffectMoveDir[1], m_fEffectMoveDir[2]);

	m_tEffectInfo.fTurnDir = _float3(m_fEffectRotationDir[0], m_fEffectRotationDir[1], m_fEffectRotationDir[2]);


	wstring strModelName(m_cModelName, m_cModelName + strlen(m_cModelName));
	m_tEffectInfo.strModelName = strModelName;

	m_tEffectInfo.strDiffuseTetextureName = Select_FolderName(m_iDiffuseFolderIndex);
	m_tEffectInfo.strAlphaTexturName      = Select_FolderName(m_iAlphaFolderIndex);

	m_tEffectInfo.fUVIndex  = _float2(m_ffEffectUVIndex[0], m_ffEffectUVIndex[1]);
	m_tEffectInfo.fMaxCount = _float2(m_ffEffectUVMaxCount[0], m_ffEffectUVMaxCount[1]);

	m_tEffectInfo.fUVFlow = _float2(m_fEffectUVFlow[0], m_fEffectUVFlow[1]);

	m_tEffectInfo.fAlphaStart = _float2(m_fAlphaStart[0], m_fAlphaStart[1]);
	m_tEffectInfo.fAlphaSpeed = _float2(m_fAlphaSpeed[0], m_fAlphaSpeed[1]);
	m_tEffectInfo.fAlphaChangeStartDelay = _float2(m_fAlphaChangeStartDelay[0], m_fAlphaChangeStartDelay[1]);

	static_cast<CEffect*>(m_pEffect)->Set_EffectDesc(m_tEffectInfo);
}




void CTool_Effect::Save_Effect(const char* pFileName)
{
	if (m_pEffect == nullptr)
		MSG_BOX("Effect_Save_Failed!");

	wstring strFileName(pFileName, pFileName + strlen(pFileName));
	wstring strFilePath = L"../Bin/DataFiles/Effect/" + strFileName + L".Effect";

	auto path = filesystem::path(strFilePath);
	filesystem::create_directories(path.parent_path());

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strFilePath, FileMode::Write);


	// 파티클 정보 저장
	m_tEffectInfo = static_cast<CEffect*>(m_pEffect)->Get_EffectDesc();


#pragma region 2023-12-16-ver.1
	//File->Write<_uint>((_uint)m_tParticleInfo.eParticleType);
#pragma endregion

	MSG_BOX("Effect_Save_Success!");
}

void CTool_Effect::Load_Effect(const char* pFileName)
{
	if (m_pEffect != nullptr)
	{
		m_pEffect->Set_Dead(true);
		m_pEffect = nullptr;
	}
	Create_Effect();

	wstring strFileName(pFileName, pFileName + strlen(pFileName));
	wstring strFilePath = L"../Bin/DataFiles/Effect/" + strFileName + L".Effect";

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strFilePath, FileMode::Read);

	CEffect::EFFECT_DESC EffectInfo = {};


#pragma region 2023-12-16-ver.1
	//File->Read<_uint>(iParticleType);
#pragma endregion

	// 적용
	static_cast<CEffect*>(m_pEffect)->Set_EffectDesc(EffectInfo);
	m_tEffectInfo = EffectInfo;
	Load_InfoEffect();

	MSG_BOX("Effect_Load_Success!");
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
