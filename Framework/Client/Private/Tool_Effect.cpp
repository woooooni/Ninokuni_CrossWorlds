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

	// 저장하기/ 불러오기
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

		ImGui::Text("ScaleStartMinEffect");
		if (ImGui::InputFloat3("##ScaleStartMinEffect", &m_tEffectInfo.fScaleStartMin.x))
			Store_InfoEffect();
		ImGui::Text("ScaleStartMaxEffect");
		if (ImGui::InputFloat3("##ScaleStartMaxEffect", &m_tEffectInfo.fScaleStartMax.x))
			Store_InfoEffect();
		ImGui::NewLine();

		ImGui::Checkbox("ScaleChange", &m_tEffectInfo.bScaleChange);
		ImGui::NewLine();

		if (m_tEffectInfo.bScaleChange)
		{
			ImGui::Text("ScaleChangeStartDelayEffect");
			ImGui::InputFloat2("##ScaleChangeStartDelayEffect", &m_tEffectInfo.fScaleChangeStartDelay.x);
			ImGui::NewLine();

			ImGui::Checkbox("ScaleChangeRandom", &m_tEffectInfo.bScaleChangeRandom);
			ImGui::NewLine();

			if (m_tEffectInfo.bScaleChangeRandom)
			{
				ImGui::Text("ScaleChangeTimeEffect");
				ImGui::InputFloat2("##ScaleChangeTimeEffect", &m_tEffectInfo.fScaleChangeTime.x);
				ImGui::NewLine();
			}
			else
			{
				ImGui::Checkbox("ScaleAdd", &m_tEffectInfo.bScaleAdd);
				ImGui::NewLine();

				ImGui::Checkbox("ScaleLoop", &m_tEffectInfo.bScaleLoop);
				ImGui::NewLine();

				ImGui::Checkbox("ScaleLoopStart", &m_tEffectInfo.bScaleLoopStart);
				ImGui::NewLine();

				ImGui::Text("ScaleMinEffect");
				ImGui::InputFloat3("##ScaleMinEffect", &m_tEffectInfo.fScaleSizeMin.x);
				ImGui::NewLine();

				ImGui::Text("ScaleMaxEffect");
				ImGui::InputFloat3("##ScaleMaxEffect", &m_tEffectInfo.fScaleSizeMax.x);
				ImGui::NewLine();

				ImGui::Text("ScaleDirSpeed");
				if (ImGui::InputFloat3("##ScaleDirSpeed", &m_tEffectInfo.fScaleDirSpeed.x))
					Store_InfoEffect();
			}

			ImGui::Text("ScaleSpeedEffect");
			ImGui::InputFloat2("##ScaleSpeedEffect", &m_tEffectInfo.fScaleSpeed.x);
			ImGui::NewLine();
		}
	}

	// 이동(힘)
	if (ImGui::CollapsingHeader("EffectVelocity"))
	{
		ImGui::Text("VelocitySpeed");
		if (ImGui::InputFloat2("##VelocitySpeed", &m_tEffectInfo.fVelocitySpeed.x))
			Store_InfoEffect();
		ImGui::Text("VelocityMinStart");
		if (ImGui::InputFloat3("##VelocityMinStart", &m_tEffectInfo.vVelocityMinStart.x))
			Store_InfoEffect();
		ImGui::Text("vVelocityMaxStart");
		if (ImGui::InputFloat3("##vVelocityMaxStart", &m_tEffectInfo.vVelocityMaxStart.x))
			Store_InfoEffect();
		ImGui::NewLine();

		ImGui::Checkbox("VelocityChange", &m_tEffectInfo.bVelocityChange);
		if (m_tEffectInfo.bVelocityChange)
		{
			ImGui::Text("VelocityChangeStartDelay");
			ImGui::InputFloat2("##VelocityChangeStartDelay", &m_tEffectInfo.fVelocityChangeStartDelay.x);

			ImGui::Text("VelocityChangeTime");
			ImGui::InputFloat2("##VelocityChangeTim", &m_tEffectInfo.fVelocityChangeTime.x);
			ImGui::NewLine();
		}
	}

	// 회전
	if (ImGui::CollapsingHeader("EffectRotation"))
	{
		if (ImGui::Checkbox("RotationBillboard", &m_tEffectInfo.bBillboard))
			Store_InfoEffect();
		ImGui::NewLine();

		ImGui::Checkbox("RotationRandomAxis", &m_tEffectInfo.bRandomAxis);
		if (!m_tEffectInfo.bRandomAxis)
		{
			ImGui::Text("RotationAxis");
			ImGui::InputFloat3("##RotationAxis", &m_tEffectInfo.fAxis.x);
			ImGui::NewLine();
		}
		ImGui::NewLine();

		ImGui::Checkbox("RotationRandomAngle", &m_tEffectInfo.bRandomAngle);
		if (!m_tEffectInfo.bRandomAngle)
		{
			ImGui::Text("RotationAngle");
			ImGui::InputFloat("##RangeParticles", &m_tEffectInfo.fAngle);
			ImGui::NewLine();
		}
		ImGui::NewLine();

		ImGui::Checkbox("RotationChange", &m_tEffectInfo.bRotationChange);
		ImGui::NewLine();

		if (m_tEffectInfo.bRotationChange)
		{
			ImGui::Text("RotationChangeStartDelayParticles");
			ImGui::InputFloat2("##RotationChangeStartDelayParticles", &m_tEffectInfo.fRotationChangeStartDelay.x);
			ImGui::NewLine();

			ImGui::Text("RotationSpeed");
			if (ImGui::InputFloat2("##RotationSpeed", &m_tEffectInfo.fRotationSpeed.x))
				Store_InfoEffect();
			ImGui::NewLine();

			ImGui::Text("RotationDir");
			if (ImGui::InputFloat3("##RotationDir", &m_tEffectInfo.fRotationDir.x))
				Store_InfoEffect();

			ImGui::Checkbox("RotationChangeRandom", &m_tEffectInfo.bRotationChangeRandom);
			ImGui::NewLine();

			if (m_tEffectInfo.bRotationChangeRandom)
			{
				ImGui::Text("RotationChangeTime");
				ImGui::InputFloat2("##RotationChangeTime", &m_tEffectInfo.fRotationChangeTime.x);
				ImGui::NewLine();
			}
		}
	}

	// 지속 시간
	if (ImGui::CollapsingHeader("EffectLifeTime"))
	{
		ImGui::Text("EffectLifeTime");
		ImGui::InputFloat2("##EffectLifeTime", &m_tEffectInfo.fLifeTime.x);
		ImGui::NewLine();
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
		if (ImGui::Checkbox("UVFlowChange", &m_tEffectInfo.bUVFlowChange))
			Store_InfoEffect();
		ImGui::Text("UVFlowLoop : 0 < UVLoop -> NoLoop");
		if (ImGui::InputInt("##UVFlowLoop", &(_int)m_tEffectInfo.iUVFlowLoop))
			Store_InfoEffect();
		ImGui::Text("UVFlowDir");
		if (ImGui::InputFloat2("##UVFlowDir", &m_tEffectInfo.fUVFlowDir.x))
			Store_InfoEffect();
		ImGui::Text("UVFlowSpeed");
		if (ImGui::InputFloat2("##UVFlowSpeed", &m_tEffectInfo.fUVFlowSpeed.x))
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
			if (ImGui::Checkbox("LoopAnimation", &m_tEffectInfo.bAnimationLoop))
				Store_InfoEffect();

			if (ImGui::Checkbox("IncrementAnimation", &m_tEffectInfo.bIncrement))
				Store_InfoEffect();

			ImGui::Text("SpeedAnimation");
			if (ImGui::InputFloat2("##SpeedAnimation", &m_tEffectInfo.fAnimationSpeed.x))
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
	if (ImGui::CollapsingHeader("EffectColor"))
	{
		if (!m_tEffectInfo.bColorRandom)
		{
			ImGui::Text("ColorStart");
			if (ImGui::ColorEdit4("##ColorStart", (float*)&m_tEffectInfo.fColorS, ImGuiColorEditFlags_Float))
				Store_InfoEffect();
		}
		if (ImGui::Checkbox("ColorRandom", &m_tEffectInfo.bColorRandom))
			Store_InfoEffect();
		ImGui::NewLine();


		ImGui::Checkbox("ColorChange", &m_tEffectInfo.bColorChange);
		if (m_tEffectInfo.bColorChange)
		{
			ImGui::Checkbox("ColorChangeRandom", &m_tEffectInfo.bColorChangeRandom);
			ImGui::NewLine();

			// fColorChangeStartDelay
			ImGui::Text("ColorChangeStartDelay");
			ImGui::InputFloat2("##ColorChangeStartDelay", &m_tEffectInfo.fColorChangeStartDelay.x);
			ImGui::NewLine();

			if (m_tEffectInfo.bColorChangeRandom)
			{
				ImGui::Text("ColorChangeRandomTime");
				ImGui::InputFloat2("##ColorChangeRandomTime", &m_tEffectInfo.fColorChangeRandomTime.x);
				ImGui::NewLine();
			}
			else
			{
				ImGui::Checkbox("ColorLoop", &m_tEffectInfo.bColorLoop);
				ImGui::NewLine();

				// fColorChangeStartM
				ImGui::Text("ColorChangeStartM");
				ImGui::InputFloat2("##ColorChangeStartM", &m_tEffectInfo.fColorChangeStartM.x);

				ImGui::Text("ColorMiddle");
				ImGui::ColorEdit4("##ColorMiddle", (float*)&m_tEffectInfo.fColorM, ImGuiColorEditFlags_Float);
				ImGui::NewLine();

				// fColorChangeStartF
				ImGui::Text("ColorChangeStartF");
				ImGui::InputFloat2("##ColorChangeStartF", &m_tEffectInfo.fColorChangeStartF.x);

				ImGui::Text("ColorFinal");
				ImGui::ColorEdit4("##ColorFinal", (float*)&m_tEffectInfo.fColorF, ImGuiColorEditFlags_Float);
				ImGui::NewLine();
			}

			// ColorDurationTime
			ImGui::Text("ColorDurationTime");
			ImGui::InputFloat2("##ColorDurationTime", &m_tEffectInfo.fColorDuration.x);
		}
	}

	// 쉐이더 값 셋팅
	if (ImGui::CollapsingHeader("EffectShader"))
	{
		// 쉐이더 패스
		ImGui::Text("ShaderPass");
		if (ImGui::InputInt("##ShaderPass", &(_int)m_tEffectInfo.iShaderPass))
			Store_InfoEffect();
		ImGui::NewLine();

		// 디스카드 값 셋팅
		ImGui::Text("Alpha_Discard");
		if (ImGui::InputFloat("##Alpha_Discard", &m_tEffectInfo.fAlpha_Discard))
			Store_InfoEffect();
		ImGui::Text("Black_Discard");
		if (ImGui::InputFloat3("##Black_Discard", &m_tEffectInfo.fBlack_Discard.x))
			Store_InfoEffect();
		ImGui::NewLine();

		// 블러 셋팅
		ImGui::Checkbox("BloomPowerRandom", &m_tEffectInfo.bBloomPowerRandom);
		ImGui::NewLine();
		if (!m_tEffectInfo.bBloomPowerRandom)
		{
			ImGui::Text("BloomPower");
			if (ImGui::ColorEdit4("##BloomPower", (float*)&m_tEffectInfo.fBloomPower, ImGuiColorEditFlags_Float))
				Store_InfoEffect();
			ImGui::NewLine();
		}

		ImGui::Checkbox("BlurPowerRandom", &m_tEffectInfo.bBlurPowerRandom);
		ImGui::NewLine();
		if (!m_tEffectInfo.bBlurPowerRandom)
		{
			if (ImGui::InputFloat("##BlurPower", &m_tEffectInfo.fBlurPower))
				Store_InfoEffect();
		}
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


	if (m_tEffectInfo.eType == CEffect::EFFECT_TEXTURE)
		m_iEffectTypeIndex = 0;
	else if (m_tEffectInfo.eType == CEffect::EFFECT_MESH)
		m_iEffectTypeIndex = 1;
	else
		m_iEffectTypeIndex = 2;


	m_fEffectRange[0] = m_tEffectInfo.fRange.x;
	m_fEffectRange[1] = m_tEffectInfo.fRange.y;
	m_fEffectRange[2] = m_tEffectInfo.fRange.z;

	m_fEffectRangeDistance[0] = m_tEffectInfo.fRangeDistance.x;
	m_fEffectRangeDistance[1] = m_tEffectInfo.fRangeDistance.y;


	size_t convertedChars = 0;
	wcstombs_s(&convertedChars, m_cModelName, sizeof(m_cModelName), m_tEffectInfo.strModelName.c_str(), _TRUNCATE);

	m_iDiffuseFolderIndex = Get_FolderIndex(m_tEffectInfo.strDiffuseTetextureName);
	m_iAlphaFolderIndex = Get_FolderIndex(m_tEffectInfo.strAlphaTexturName);

	m_ffEffectUVIndex[0] = m_tEffectInfo.fUVIndex.x;
	m_ffEffectUVIndex[1] = m_tEffectInfo.fUVIndex.y;
	m_ffEffectUVMaxCount[0] = m_tEffectInfo.fMaxCount.x;
	m_ffEffectUVMaxCount[1] = m_tEffectInfo.fMaxCount.y;

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


	wstring strModelName(m_cModelName, m_cModelName + strlen(m_cModelName));
	m_tEffectInfo.strModelName = strModelName;

	m_tEffectInfo.strDiffuseTetextureName = Select_FolderName(m_iDiffuseFolderIndex);
	m_tEffectInfo.strAlphaTexturName      = Select_FolderName(m_iAlphaFolderIndex);

	m_tEffectInfo.fUVIndex  = _float2(m_ffEffectUVIndex[0], m_ffEffectUVIndex[1]);
	m_tEffectInfo.fMaxCount = _float2(m_ffEffectUVMaxCount[0], m_ffEffectUVMaxCount[1]);

	m_tEffectInfo.fAlphaStart = _float2(m_fAlphaStart[0], m_fAlphaStart[1]);
	m_tEffectInfo.fAlphaSpeed = _float2(m_fAlphaSpeed[0], m_fAlphaSpeed[1]);
	m_tEffectInfo.fAlphaChangeStartDelay = _float2(m_fAlphaChangeStartDelay[0], m_fAlphaChangeStartDelay[1]);

	static_cast<CEffect*>(m_pEffect)->Set_EffectDesc(m_tEffectInfo);
}




void CTool_Effect::Save_Effect(const char* pFileName)
{
	if (m_pEffect == nullptr)
	{
		MSG_BOX("Effect_Save_Failed!");
		return;
	}

	m_tEffectInfo = static_cast<CEffect*>(m_pEffect)->Get_EffectDesc();

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
		}
#pragma endregion

	});

	wstring strFileName(pFileName, pFileName + strlen(pFileName));
	wstring strFilePath = L"../Bin/DataFiles/Effect/" + strFileName + L".json";
	GI->Json_Save(strFilePath, json);

	MSG_BOX("Effect_Save_Success!");
}

void CTool_Effect::Load_Effect(const char* pFileName)
{
	if (m_pEffect == nullptr)
		Create_Effect();

	wstring strFileName(pFileName, pFileName + strlen(pFileName));
	wstring strFilePath = L"../Bin/DataFiles/Effect/" + strFileName + L".json";
	Json json = GI->Json_Load(strFilePath);

	CEffect::EFFECT_DESC EffectInfo = {}; 
	for (const auto& item : json["EffectInfo"])
	{
		// 이펙트 타입
		_int iType          = item["Type"];
		EffectInfo.eType    = (CEffect::EFFECT_TYPE)iType;

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

		EffectInfo.bScaleAdd  = item["ScaleAdd"];
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
		EffectInfo.strModelName            = CUtils::Utf8_To_Wstring(item["ModelName"]);
		EffectInfo.strDiffuseTetextureName = CUtils::Utf8_To_Wstring(item["DiffuseTetextureName"]);
		EffectInfo.strAlphaTexturName      = CUtils::Utf8_To_Wstring(item["AlphaTexturName"]);

		EffectInfo.iTextureIndexDiffuse = item["TextureIndexDiffuse"];
		EffectInfo.iTextureIndexAlpha   = item["TextureIndexAlpha"];

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
	}

	// 적용
	static_cast<CEffect*>(m_pEffect)->Set_EffectDesc(EffectInfo);
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
