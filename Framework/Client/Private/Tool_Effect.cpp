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
		{"Type",                     (_int)m_tEffectInfo.eType},
		{"Gravity",                  m_tEffectInfo.bGravity}
		////				             
		//{"Range",                    m_tEffectInfo.fRange.x, m_tEffectInfo.fRange.y, m_tEffectInfo.fRange.z},
		//{"RangeDistance",            m_tEffectInfo.fRangeDistance.x, m_tEffectInfo.fRangeDistance.y},
		////				             
		//{"ScaleSameRate",            m_tEffectInfo.bScaleSameRate},
		//{"ScaleStartMin",            m_tEffectInfo.fScaleStartMin.x, m_tEffectInfo.fScaleStartMin.y, m_tEffectInfo.fScaleStartMin.z},
		//{"ScaleStartMax",            m_tEffectInfo.fScaleStartMax.x, m_tEffectInfo.fScaleStartMax.y, m_tEffectInfo.fScaleStartMax.z},				     
		//{"ScaleChange",              m_tEffectInfo.bScaleChange},
		//{"ScaleChangeStartDelay",    m_tEffectInfo.fScaleChangeStartDelay.x, m_tEffectInfo.fScaleChangeStartDelay.y},				     
		//{"ScaleChangeRandom",        m_tEffectInfo.bScaleChangeRandom},
	 //   {"ScaleChangeTime",          m_tEffectInfo.fScaleChangeTime.x, m_tEffectInfo.fScaleChangeTime.y},					     
		//{"ScaleAdd",                 m_tEffectInfo.bScaleAdd},
		//{"ScaleLoop",                m_tEffectInfo.bScaleLoop},
		//{"ScaleLoopStart",           m_tEffectInfo.bScaleLoopStart},				     
		//{"ScaleSizeMin",             m_tEffectInfo.fScaleSizeMin.x, m_tEffectInfo.fScaleSizeMin.y, m_tEffectInfo.fScaleSizeMin.z},
		//{"ScaleSizeMax",             m_tEffectInfo.fScaleSizeMax.x, m_tEffectInfo.fScaleSizeMax.y, m_tEffectInfo.fScaleSizeMax.z},
		//{"ScaleSpeed",               m_tEffectInfo.fScaleSpeed.x, m_tEffectInfo.fScaleSpeed.y},
		//{"ScaleDirSpeed",            m_tEffectInfo.fScaleDirSpeed.x, m_tEffectInfo.fScaleDirSpeed.y, m_tEffectInfo.fScaleDirSpeed.z},
		////						     
		//{"VelocitySpeed",            m_tEffectInfo.fVelocitySpeed.x, m_tEffectInfo.fVelocitySpeed.y},
		//{"VelocityMinStart",         m_tEffectInfo.vVelocityMinStart.x, m_tEffectInfo.vVelocityMinStart.y, m_tEffectInfo.vVelocityMinStart.z},
		//{"VelocityMaxStart",         m_tEffectInfo.vVelocityMaxStart.x, m_tEffectInfo.vVelocityMaxStart.y, m_tEffectInfo.vVelocityMaxStart.z},
		//{"VelocityChange",           m_tEffectInfo.bVelocityChange},
		//{"VelocityChangeStartDelay", m_tEffectInfo.fVelocityChangeStartDelay.x, m_tEffectInfo.fVelocityChangeStartDelay.y},
		//{"VelocityChangeTime",       m_tEffectInfo.fVelocityChangeTime.x, m_tEffectInfo.fVelocityChangeTime.y},
		////
		//{"Billboard",                m_tEffectInfo.bBillboard},
		//{"RandomAxis",               m_tEffectInfo.bRandomAxis},
		//{"Axis",                     m_tEffectInfo.fAxis.x, m_tEffectInfo.fAxis.y, m_tEffectInfo.fAxis.z},
		//{"RandomAngle",              m_tEffectInfo.bRandomAngle},
		//{"Angle",                    m_tEffectInfo.fAngle},
		//{"RotationChange",           m_tEffectInfo.bRotationChange},
		//{"RotationChangeStartDelay", m_tEffectInfo.fRotationChangeStartDelay.x, m_tEffectInfo.fRotationChangeStartDelay.y},
		//{"RotationSpeed",            m_tEffectInfo.fRotationSpeed.x, m_tEffectInfo.fRotationSpeed.y},
		//{"RotationDir",              m_tEffectInfo.fRotationDir.x, m_tEffectInfo.fRotationDir.y, m_tEffectInfo.fRotationDir.z},
		//{"RotationChangeRandom",     m_tEffectInfo.bRotationChangeRandom},
		//{"RotationChangeTime",       m_tEffectInfo.fRotationChangeTime.x, m_tEffectInfo.fRotationChangeTime.y},
		////
		//{"LifeTime",                 m_tEffectInfo.fLifeTime.x, m_tEffectInfo.fLifeTime.y},
		////
		//{"ModelName",                m_tEffectInfo.strModelName},
		//{"DiffuseTetextureName",     m_tEffectInfo.strDiffuseTetextureName},
		//{"AlphaTexturName",          m_tEffectInfo.strAlphaTexturName},
		//{"TextureIndexDiffuse",      m_tEffectInfo.iTextureIndexDiffuse},
		//{"TextureIndexAlpha",        m_tEffectInfo.iTextureIndexAlpha},
		//{"RandomStartIndex",         m_tEffectInfo.bRandomStartIndex},
		//{"UVIndex",                  m_tEffectInfo.fUVIndex.x, m_tEffectInfo.fUVIndex.y},
		//{},
		//{},
		//{},
		//{},
		//{},
		//{},
		//{},
		//{},
		//{},
		//{},
		//{},
		//{},
		//{},

		});

	wstring strFileName(pFileName, pFileName + strlen(pFileName));
	wstring strFilePath = L"../Bin/DataFiles/Effect/" + strFileName + L".json";
	GI->Json_Save(strFilePath, json);

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
	wstring strFilePath = L"../Bin/DataFiles/Effect/" + strFileName + L".json";
	Json json = GI->Json_Load(strFilePath);

	CEffect::EFFECT_DESC EffectInfo = {}; 
	for (const auto& item : json["EffectInfo"])
	{
		_int iType          = item["Type"];
		EffectInfo.eType    = (CEffect::EFFECT_TYPE)iType;

		EffectInfo.bGravity = item["Gravity"];
	}


	// 적용
	m_tEffectInfo = EffectInfo;
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
