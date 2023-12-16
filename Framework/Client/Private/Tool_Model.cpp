#pragma region Default 

#include "stdafx.h"
#include "Tool_Model.h"

#include "imgui.h"

#include "GameInstance.h"
#include "Utils.h"
#include <fstream>

#include "Model.h"
#include "Animation.h"
#include "HierarchyNode.h"

#include "Dummy.h"
#include "Part.h"

#pragma endregion

#pragma region include Weapon Prototype


#pragma endregion


CTool_Model::CTool_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CTool(pDevice, pContext)
{
}

HRESULT CTool_Model::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_DebugDraw()))
		return E_FAIL;

	if (FAILED(Ready_Dummy()))
		return E_FAIL;

	if (FAILED(Ready_WeaponPrototypes()))
		return E_FAIL;

	if (FAILED(Ready_AutoAnimData()))
		return E_FAIL;

	return S_OK;
}

void CTool_Model::Tick(_float fTimeDelta)
{
	ImGui::Begin("Model_Tool");
	{
		Tick_Model(fTimeDelta);
		Tick_Animation(fTimeDelta);
		Tick_Socket(fTimeDelta);
		Tick_Event(fTimeDelta);
		Tick_Costume(fTimeDelta);
	}
	ImGui::End();

	Tick_Dummys(fTimeDelta);

	_bool bDemo = FALSE;
	if(bDemo) ImGui::ShowDemoWindow(&bDemo);
}

HRESULT CTool_Model::Render()
{
	if (FAILED(Render_DebugDraw()))
		return E_FAIL;

	return S_OK;
}

void CTool_Model::Reset_Transform()
{
	m_pDummy->Get_TransformCom()->Set_State(CTransform::STATE::STATE_RIGHT, XMVectorSet(1.f, 0.f, 0.f, 0.f));
	m_pDummy->Get_TransformCom()->Set_State(CTransform::STATE::STATE_UP, XMVectorSet(0.f, 1.f, 0.f, 0.f));
	m_pDummy->Get_TransformCom()->Set_State(CTransform::STATE::STATE_LOOK, XMVectorSet(0.f, 0.f, 1.f, 0.f));
	m_pDummy->Get_TransformCom()->Set_State(CTransform::STATE::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));

	return;
}

HRESULT CTool_Model::Clear_ToolAnimationData()
{
	m_bAllAnimLoop = TRUE;
	vector<class CAnimation*>& Animations = m_pDummy->Get_ModelCom()->Get_Animations();

	for (auto& pAnim : Animations)
		pAnim->Set_Loop(m_bAllAnimLoop);
	

	m_iCurBoneIndex = 0;

	m_iSocketIndex = -1;
	m_iRenderSocketIndex = 0;

	m_AnimTransformsCaches.clear();
	m_AnimTransformsCaches.shrink_to_fit();

	m_AddedTransformNames.clear();
	m_AddedTransformNames.shrink_to_fit();

	m_vRotation = { 0.f, -270.f, 0.f };

	m_bAuto = FALSE;

	m_iAutoAnimIndex = 0;

	return S_OK;
}

Vec3 CTool_Model::Calculate_SocketPosition()
{
	Matrix matSocketWorld = Calculate_SocketWorldMatrix();

	Vec3 vWorldPos = { matSocketWorld._41, matSocketWorld._42, matSocketWorld._43 };

	return vWorldPos;
}

Matrix CTool_Model::Calculate_SocketWorldMatrix()
{
	TweenDesc TweenDesc = m_pDummy->Get_ModelCom()->Get_TweenDesc();

	enum STEP { CURR, NEXT, STEP_END };

	Matrix		matAnimLocal;
	Vec4        vAnimLocalPos;
	
	Matrix		matAnim[STEP_END][STEP_END];
	Vec3		vScaleAnim[STEP_END][STEP_END];
	Quaternion  qQuatAnim[STEP_END][STEP_END];
	Vec3		vPosAnim[STEP_END][STEP_END];

	Vec3		vScale[STEP_END];
	Vec4		vRotation[STEP_END];
	Vec3		vPos[STEP_END];

	/* 현재 애니메이션 */
	Matrix matCurAnim;
	Vec4  vCurAnimPos;
	{
		matAnim[CURR][CURR]	= m_AnimTransformsCaches[TweenDesc.cur.iAnimIndex].transforms[TweenDesc.cur.iCurFrame][m_iRenderSocketIndex];
		matAnim[CURR][NEXT]	= m_AnimTransformsCaches[TweenDesc.cur.iAnimIndex].transforms[TweenDesc.cur.iNextFrame][m_iRenderSocketIndex];

		matAnim[CURR][CURR].Decompose(vScaleAnim[CURR][CURR], qQuatAnim[CURR][CURR], vPosAnim[CURR][CURR]);
		matAnim[CURR][NEXT].Decompose(vScaleAnim[CURR][NEXT], qQuatAnim[CURR][NEXT], vPosAnim[CURR][NEXT]);

		XMStoreFloat3(&vScale[CURR], XMVectorLerp(XMLoadFloat3(&vScaleAnim[CURR][CURR]), XMLoadFloat3(&vScaleAnim[CURR][NEXT]), TweenDesc.cur.fRatio));
		XMStoreFloat4(&vRotation[CURR], XMQuaternionSlerp(XMLoadFloat4(&qQuatAnim[CURR][CURR]), XMLoadFloat4(&qQuatAnim[CURR][NEXT]), TweenDesc.cur.fRatio));
		XMStoreFloat3(&vPos[CURR], XMVectorLerp(XMLoadFloat3(&vPosAnim[CURR][CURR]), XMLoadFloat3(&vPosAnim[CURR][NEXT]), TweenDesc.cur.fRatio));

		matCurAnim = XMMatrixAffineTransformation(
			XMLoadFloat3(&vScale[CURR]), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&vRotation[CURR]), XMVectorSetW(XMLoadFloat3(&vPos[CURR]), 1.f));
		
		memcpy(&vCurAnimPos, matCurAnim.m[3], sizeof(Vec4));
	}

	/* 다음 애니메이션 */
	if (0 <= TweenDesc.next.iAnimIndex)
	{
		/* 다음 애니메이션 데이터 계산 */

		matAnim[NEXT][CURR] = m_AnimTransformsCaches[TweenDesc.next.iAnimIndex].transforms[TweenDesc.next.iCurFrame][m_iRenderSocketIndex];
		matAnim[NEXT][NEXT] = m_AnimTransformsCaches[TweenDesc.next.iAnimIndex].transforms[TweenDesc.next.iNextFrame][m_iRenderSocketIndex];

		matAnim[NEXT][CURR].Decompose(vScaleAnim[NEXT][CURR], qQuatAnim[NEXT][CURR], vPosAnim[NEXT][CURR]);
		matAnim[NEXT][NEXT].Decompose(vScaleAnim[NEXT][NEXT], qQuatAnim[NEXT][NEXT], vPosAnim[NEXT][NEXT]);

		XMStoreFloat3(&vScale[NEXT], XMVectorLerp(XMLoadFloat3(&vScaleAnim[NEXT][CURR]), XMLoadFloat3(&vScaleAnim[NEXT][NEXT]), TweenDesc.next.fRatio));
		XMStoreFloat4(&vRotation[NEXT], XMQuaternionSlerp(XMLoadFloat4(&qQuatAnim[NEXT][CURR]), XMLoadFloat4(&qQuatAnim[NEXT][NEXT]), TweenDesc.next.fRatio));
		XMStoreFloat3(&vPos[NEXT], XMVectorLerp(XMLoadFloat3(&vPosAnim[NEXT][CURR]), XMLoadFloat3(&vPosAnim[NEXT][NEXT]), TweenDesc.next.fRatio));

		/* 현재 애니메이션과 다음 애니메이션 보간 */
		{
			Vec3 vLerpScale;
			Vec4 vLerpRot;
			Vec3 vLerpPos;

			XMStoreFloat3(&vLerpScale, XMVectorLerp(XMLoadFloat3(&vScale[CURR]), XMLoadFloat3(&vScale[NEXT]), TweenDesc.fTweenRatio));
			XMStoreFloat4(&vLerpRot, XMQuaternionSlerp(XMLoadFloat4(&vRotation[CURR]), XMLoadFloat4(&vRotation[NEXT]), TweenDesc.fTweenRatio));
			XMStoreFloat3(&vLerpPos, XMVectorLerp(XMLoadFloat3(&vPos[CURR]), XMLoadFloat3(&vPos[NEXT]), TweenDesc.fTweenRatio));

			matAnimLocal = XMMatrixAffineTransformation(
				XMLoadFloat3(&vLerpScale), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&vLerpRot), XMVectorSetW(XMLoadFloat3(&vLerpPos), 1.f));
			
			memcpy(&vAnimLocalPos, matAnimLocal.m[3], sizeof(Vec4));
		}
	}
	else
	{
		memcpy(&matAnimLocal, &matCurAnim, sizeof(Matrix));
		memcpy(&vAnimLocalPos, &vCurAnimPos, sizeof(Vec4));
	}
		

	/* 커스텀 피벗*/
	{
		Matrix matCustomPivot = XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(
								XMConvertToRadians(m_vRotation.x),
								XMConvertToRadians(m_vRotation.y),
								XMConvertToRadians(m_vRotation.z)));

		matAnimLocal = matCustomPivot * matAnimLocal;
	}

	/* 정규화 */
	{
		Vec3 vRight, vUp, vLook;
		memcpy(&vRight, matAnimLocal.m[0], sizeof(Vec3));
		memcpy(&vUp, matAnimLocal.m[1], sizeof(Vec3));
		memcpy(&vLook, matAnimLocal.m[2], sizeof(Vec3));

		vRight.Normalize();
		vUp.Normalize();
		vLook.Normalize();

		matAnimLocal.Right(vRight);
		matAnimLocal.Up(vUp);
		matAnimLocal.Backward(vLook);
	}

	/* 포지션 리셋*/
	memcpy(&matAnimLocal.m[3], &vAnimLocalPos, sizeof(Vec4));

	return matAnimLocal * m_pDummy->Get_TransformCom()->Get_WorldMatrix();
}

const _bool CTool_Model::Is_Exception()
{
	if (nullptr == m_pDummy)
	{
		ImGui::Text(u8"더미 오브젝트가 존재하지 않습니다.");
		return true;
	}

	if (nullptr != m_pDummy->Get_ModelCom())
	{
		CModel* pModelCom = m_pDummy->Get_ModelCom();
		if (CModel::TYPE::TYPE_NONANIM == pModelCom->Get_ModelType())
		{
			ImGui::Text(u8"스태틱 모델은 애니메이션이 존재하지 않습니다.");
			return true;
		}
	}
	else
	{
		ImGui::Text(u8"모델이 로드되지 않았습니다.");
		return true;
	}

	return false;
}

HRESULT CTool_Model::Ready_Dummy()
{
	m_pDummy = CDummy::Create(m_pDevice, m_pContext, L"Dummy");
	
	if (FAILED(m_pDummy->Initialize(nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTool_Model::Ready_DebugDraw()
{
	m_pBatch = new PrimitiveBatch<VertexPositionColor>(GI->Get_Context());
	m_pEffect = new BasicEffect(GI->Get_Device());

	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCodes = nullptr;
	size_t			iLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCodes, &iLength);

	if (FAILED(GI->Get_Device()->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCodes, iLength, &m_pInputLayout)))
		return E_FAIL;

	_float	fRadius = 0.1f;
	Vec3	vOrigin = { 0.f, fRadius * 0.5f, 0.f };
	m_pSphere = new BoundingSphere(vOrigin, fRadius);

	if (nullptr == m_pSphere)
		return E_FAIL;

	return TRUE;
}

HRESULT CTool_Model::Ready_WeaponPrototypes()
{
	CPart*			pWeapon = nullptr;
	CGameObject*	pGameObject = nullptr;

	/* Prototype_GameObject_TempSword */
	if (FAILED(GI->Add_GameObject(LEVEL_TOOL, _uint(LAYER_WEAPON), TEXT("Prototype_GameObject_TempSword"), nullptr, &pGameObject)))
		return E_FAIL;
	{
		pWeapon = dynamic_cast<CPart*>(pGameObject);
		if (nullptr == pWeapon)
			return E_FAIL;

		m_Weapons.push_back(pWeapon);

		pWeapon = nullptr;
		pGameObject = nullptr;
	}

	return S_OK;
}

HRESULT CTool_Model::Ready_AutoAnimData()
{
	vector<_float> vData = { -270.f, -180.f, -90.f, 0.f, 90.f, 180.f, 270.f };

	for (auto x : vData)
		for (auto y : vData)
			for (auto z : vData)
				m_vAutoSocket.push_back(Vec3{ x, y, z });

	return S_OK;
}

HRESULT CTool_Model::Ready_SocketTransforms()
{
	m_AnimTransformsCaches = GI->Create_AnimationTransform_Caches_InTool(m_pDummy->Get_ModelCom());

	return S_OK;
}

HRESULT CTool_Model::Render_DebugDraw()
{
	if (m_iRenderSocketIndex < 0 || m_AnimTransformsCaches.empty())
		return S_OK;

	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(GI->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));

	m_pEffect->Apply(m_pContext);

	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pBatch->Begin();
	{
		m_pSphere->Center = Calculate_SocketPosition();

		DX::Draw(m_pBatch, *m_pSphere, XMLoadFloat4(&m_vColor));
	}

	m_pBatch->End();

	return S_OK;
}

void CTool_Model::Tick_Model(_float fTimeDelta)
{
	if (ImGui::CollapsingHeader("Import and Eport"))
	{
		/* Exception */
		if (nullptr == m_pDummy)
		{
			ImGui::Text(u8"더미 오브젝트가 존재하지 않습니다.");
			return;
		}

		/* Import */
		{
			ImGui::Text("Import File (Fbx)");
			
			char szFilePath[MAX_PATH];
			char szFileName[MAX_PATH];

			sprintf_s(szFilePath, CUtils::ToString(m_strFilePath).c_str());
			sprintf_s(szFileName, CUtils::ToString(m_strFileName).c_str());

			/* Path */
			if (ImGui::InputText("##ModelPathText", szFilePath, MAX_PATH))
				m_strFilePath = CUtils::ToWString(string(szFilePath));
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text(u8"Fbx 파일의 경우 : ../Bin/Resources/AnimModel/Boss/Stellia/");
				ImGui::Text(u8"Binary 파일의 경우 : ../Bin/Export/AnimModel/Boss/Stellia/");
				ImGui::EndTooltip();
			}
			IMGUI_SAME_LINE;
			ImGui::Text("Path");

			/* File Name */
			if (ImGui::InputText("##ModelFileText", szFileName, MAX_PATH))
				m_strFileName = CUtils::ToWString(string(szFileName));
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text(u8"Fbx 파일의 경우 : Stellia.fbx");
				ImGui::Text(u8"Binary 파일의 경우 : Stellia");
				ImGui::EndTooltip();
			}
			IMGUI_SAME_LINE;
			ImGui::Text("File Name");

			/* Model Type */
			static const char* szImportModelTypes[] = { "STATIC", "ANIM" };
			static const char* szImportModelType = NULL;
			static _int iSelectedImportModelType = -1;

			if (ImGui::BeginCombo("##ImportModelType", szImportModelType))
			{
				for (int n = 0; n < IM_ARRAYSIZE(szImportModelTypes); n++)
				{
					bool is_selected = (szImportModelType == szImportModelTypes[n]); // You can store your selection however you want, outside or inside your objects
					if (ImGui::Selectable(szImportModelTypes[n], is_selected))
					{
						szImportModelType = szImportModelTypes[n];
						iSelectedImportModelType = n;
					}

				}

				ImGui::EndCombo();
			}
			m_strFileName = CUtils::ToWString(string(szFileName));
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text(u8"Static vs Animation");
				ImGui::EndTooltip();
			}
			IMGUI_SAME_LINE;
			ImGui::Text("Model Type");

			/* Import Btn */
			if (ImGui::Button("Import"))
			{
				if (iSelectedImportModelType != -1)
				{
					if (FAILED(m_pDummy->Ready_ModelCom(iSelectedImportModelType, m_strFilePath, m_strFileName)))
						MSG_BOX("Failed Import.");
					else
					{
						MSG_BOX("Success Import.");
						Clear_ToolAnimationData();
						m_pDummy->Get_ModelCom()->Set_Animation(0);
						if (CModel::TYPE::TYPE_ANIM == m_pDummy->Get_ModelCom()->Get_ModelType() && FAILED(Ready_SocketTransforms()))
						{
							MSG_BOX("소켓 트랜스폼 생성에 실패했습니다.");
							return;
						}
					}
				}
				else
					MSG_BOX("모델 타입을 선택해주세요");
			}
		}

	
		/* Export (One File) */
		{
			IMGUI_NEW_LINE;
			ImGui::Separator();
			ImGui::Text("Export File (Binary and Vtf)");
			char szFilePath[MAX_PATH];
			char szFileName[MAX_PATH];

			static char szExportFolderName[MAX_PATH];
			ImGui::InputText("##ModelExportFolder", szExportFolderName, MAX_PATH);
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text(u8"Fbx, Binary 파일 공통 : AnimModel/Boss/Stellia/");
				ImGui::EndTooltip();
			}
			IMGUI_SAME_LINE;
			ImGui::Text("Path");

			/* Export Btn*/
			if (ImGui::Button("Export"))
			{
				if (strlen(szExportFolderName) > 0)
				{
					if (FAILED(m_pDummy->Export_Model_Bin(CUtils::ToWString(szExportFolderName), m_strFileName)))
					{
						MSG_BOX("Failed Save.");

					}
					else
					{
						MSG_BOX("Save Success");
					}
				}
			}
		}

		/* Export (All File) */
		{
			IMGUI_NEW_LINE;
			ImGui::Separator();
			ImGui::Text("Export Files (Binary and Vtf)");

			/* Path */
			static char szAllObjectExportFolderName[MAX_PATH] = "";
			ImGui::InputText("##All_ModelExportFolder", szAllObjectExportFolderName, MAX_PATH);
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text(u8"익스포트할 폴더들이 포함된 상위폴더 ex : AnimModel/Boss/");
				ImGui::EndTooltip();
			}
			IMGUI_SAME_LINE;
			ImGui::Text("Path");

			/* Type */
			const char* szExportModelTypes[] = { "STATIC", "ANIM" };
			static const char* szExportObjectModelType = NULL;
			static _int iSelectedExportModelType = -1;
			if (ImGui::BeginCombo("##ExportAllObject_ModelType", szExportObjectModelType))
			{
				for (int n = 0; n < IM_ARRAYSIZE(szExportModelTypes); n++)
				{
					bool is_selected = (szExportObjectModelType == szExportModelTypes[n]); 
					if (ImGui::Selectable(szExportModelTypes[n], is_selected))
					{
						szExportObjectModelType = szExportModelTypes[n];
						iSelectedExportModelType = n;
					}

				}
				ImGui::EndCombo();
			}
			IMGUI_SAME_LINE;
			ImGui::Text("Model Type");

			/* Btn */
			if (ImGui::Button("Export All"))
			{
				if (0 != strcmp(szAllObjectExportFolderName, "") && iSelectedExportModelType != -1)
				{
					if(FAILED(GI->Export_Model_Data_FromPath(iSelectedExportModelType, CUtils::ToWString(szAllObjectExportFolderName))))
						MSG_BOX("Failed Export.");
				
				}
				else
				{
					MSG_BOX("폴더 경로 혹은 모델 타입 지정을 확인하세요.");
				}
			}
		}
	
		///* Etc */
		//{
		//	IMGUI_NEW_LINE;
		//	ImGui::Separator();
		//	ImGui::Text("Etc");

		//	/* Reset Transform*/
		//	if (ImGui::Button("Reset Transform"))
		//		Reset_Transform();
		//}
		IMGUI_NEW_LINE;
	}
}


void CTool_Model::Tick_Animation(_float fTimeDelta)
{
	if (ImGui::CollapsingHeader("Animations"))
	{
		if (Is_Exception())
			return;

		CModel* pModelCom = m_pDummy->Get_ModelCom();
		static char szAnimationName[255];

		vector<CAnimation*>& Animations = pModelCom->Get_Animations();
    
		ImGui::Text("Animation List (count : %d)", Animations.size());
		IMGUI_SAME_LINE;

		/* Export Anim Names */
		if (ImGui::Button("Export Anim Names"))
		{
			//쓰기 전용으로 파일을 오픈(파일이 없으면 생성)
			ofstream fout;
			fout.open("../Bin/Export/ModelAnimations.txt");

			for (auto& iter : Animations)
			{
				wstring strAnimationName = iter->Get_AnimationName();
				if (fout.is_open())
				{
					fout.write(CUtils::ToString(strAnimationName).c_str(), strAnimationName.size());
					fout.write("\n", sizeof(1));
				}

			}
			fout.close();
			MSG_BOX("Export OK.");
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text(u8"../Bin/Export/ModelAnimations.txt 경로로 애니메이션 목록이 파일로 저장됩니다.");
			ImGui::EndTooltip();
		}

		/* Animation List */
		if (ImGui::BeginListBox("##Animation_List"))
		{
			for(size_t i = 0; i< Animations.size(); ++i)
			{
				string AnimationName = CUtils::ToString(Animations[i]->Get_AnimationName());
				if (ImGui::Selectable(AnimationName.c_str(), i == pModelCom->Get_CurrAnimationIndex()))
				{
					pModelCom->Set_Animation(i);
					sprintf_s(szAnimationName, CUtils::ToString(pModelCom->Get_CurrAnimation()->Get_AnimationName()).c_str());
				}
			}
			ImGui::EndListBox();
		}
	
		/* 변경시 다시 익스포트 해야하는 유형 */
		IMGUI_NEW_LINE;
		ImGui::Separator();
		ImGui::Text("Edit 1");
		IMGUI_SAME_LINE;
		ImGui::TextColored(ImVec4(1.f, 0.3f, 0.6f, 1.f), u8"삭제, 정렬, 순서 변경, 이름 변경은 다시 익스포트 해야 반영됩니다. ");
		{
			/* Swap */
			{
				if (ImGui::ArrowButton("##Swap_Animation_Up", ImGuiDir_Up))
				{
					pModelCom->Swap_Animation(pModelCom->Get_CurrAnimationIndex(), pModelCom->Get_CurrAnimationIndex() - 1);
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text(u8"위 애니메이션과 순서를 스왑");
					ImGui::EndTooltip();
				}
				IMGUI_SAME_LINE;
				if (ImGui::ArrowButton("##Swap_Animation_Down", ImGuiDir_Down))
				{
					pModelCom->Swap_Animation(pModelCom->Get_CurrAnimationIndex(), pModelCom->Get_CurrAnimationIndex() + 1);
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text(u8"아래 애니메이션과 순서를 스왑");
					ImGui::EndTooltip();
				}
			}
			IMGUI_SAME_LINE;

			/* Delete */
			if (ImGui::Button("Delete") || (KEY_TAP(KEY::DEL) && ImGui::IsWindowFocused()))
			{
				_int iCurIndex = pModelCom->Get_CurrAnimationIndex();

				pModelCom->Delete_Animation(iCurIndex);

				iCurIndex = (0 < iCurIndex - 1) ? 0 : iCurIndex - 1;

				pModelCom->Set_Animation(iCurIndex, -1.f);

				Animations = pModelCom->Get_Animations();

				sprintf_s(szAnimationName, CUtils::ToString(pModelCom->Get_CurrAnimation()->Get_AnimationName()).c_str());
			}
			IMGUI_SAME_LINE;

			/* Sort */
			if (ImGui::Button("Sort"))
			{
				vector<class CAnimation*>& Animations = pModelCom->Get_Animations();
				sort(Animations.begin(), Animations.end(), [&](CAnimation* pSrcAnimation, CAnimation* pDestAnimation) {
					return pSrcAnimation->Get_AnimationName() < pDestAnimation->Get_AnimationName();
					});
			}

			/* Rename */
			{
				sprintf_s(szAnimationName, CUtils::ToString(pModelCom->Get_CurrAnimation()->Get_AnimationName()).c_str());
				ImGui::InputText("##Animation_Input_Name", szAnimationName, 255);
				IMGUI_SAME_LINE;
				if(ImGui::Button("Rename"))
				{
					wstring NewAnimationName = CUtils::ToWString(string(szAnimationName));
					if (NewAnimationName.size() > 0)
						pModelCom->Get_CurrAnimation()->Set_AnimationName(NewAnimationName);
				}
			}
			IMGUI_NEW_LINE;
		}

		ImGui::Separator();
		ImGui::Text("Edit 2");

		/* Animation Time Slider */
		CAnimation* pCurrAnimation = pModelCom->Get_CurrAnimation();
		{
			_float fPlayTime = 0.f; // pCurrAnimation->Get_PlayTime();
			if (ImGui::SliderFloat("##Animation_PlayTime", &fPlayTime, 0.f, 0.f))//pCurrAnimation->Get_Duration()))
			{
				//pCurrAnimation->Set_AnimationPlayTime(m_pDummy->Get_TransformCom(), fPlayTime, fTimeDelta);
			}
			IMGUI_SAME_LINE;
			ImGui::Text("Play Time");
		}

		/* Animation Progress  */
		pCurrAnimation = pModelCom->Get_CurrAnimation();
		{
			_float fPlayTime = 0.f; // pCurrAnimation->Get_PlayTime();
			if (ImGui::SliderFloat("##Animation_Progress", &fPlayTime, 0.f, 0.f))//pCurrAnimation->Get_Duration()))
			{
				//pCurrAnimation->Set_AnimationPlayTime(m_pDummy->Get_TransformCom(), fPlayTime, fTimeDelta);
			}
			IMGUI_SAME_LINE;
			ImGui::Text("Progress");
		}
		
		/* Set Speed */
		{
			_float fSpeed = pCurrAnimation->Get_AnimationSpeed();
			if (ImGui::DragFloat("##AnimationSpeed", &fSpeed, 0.01f, 0.f, 100.f))
			{
				pCurrAnimation->Set_AnimationSpeed(fSpeed);
			}
			IMGUI_SAME_LINE;
			ImGui::Text("Set Speed");
		}

		/* Play Btn*/
		if (ImGui::ArrowButton("##Play_AnimationButton", ImGuiDir_Right))
		{
			_float fAnimationProgress = pModelCom->Get_Progress();

			if(fAnimationProgress >= 1.f)
				pModelCom->Set_Animation(pModelCom->Get_CurrAnimationIndex());
	
			pModelCom->Set_Stop_Animation(false);
		}
		IMGUI_SAME_LINE;

		/* Stop Btn*/
		{
			if (ImGui::Button("||"))
				pModelCom->Set_Stop_Animation(true);
		}
		//IMGUI_SAME_LINE;

		/* Loop Btn */
		{
			_bool bLoop = pCurrAnimation->Is_Loop();
			if (ImGui::Checkbox("Cur Anim Loop", &bLoop))
				pCurrAnimation->Set_Loop(bLoop);
			//ImGui::SameLine;
			//ImGui::Text("Cur Anim Loop     ");
		}
		IMGUI_SAME_LINE;

		/* All Btn */
		{
			_bool bLoop = m_bAllAnimLoop;
			if (ImGui::Checkbox("All Anim Loop", &bLoop))
			{
				m_bAllAnimLoop = bLoop;

				vector<class CAnimation*>& Animations = pModelCom->Get_Animations();

				for (auto& pAnim : Animations)
				{
					pAnim->Set_Loop(m_bAllAnimLoop);
				}
			}
			//ImGui::SameLine;
			//ImGui::Text("All Anim Loop");
		}
		
		IMGUI_NEW_LINE;
	}
}

void CTool_Model::Tick_Socket(_float fTimeDelta)
{
	if (ImGui::CollapsingHeader("Socket Bone"))
	{
		if (Is_Exception()) return;

		ImGui::TextColored(ImVec4(1.f, 0.3f, 0.6f, 1.f), u8"애니메이션이 편집된 경우 소켓 또한 다시 갱신이 필요합니다.");

		vector<class CHierarchyNode*>& HiearachyNodes = m_pDummy->Get_ModelCom()->Get_HierarchyNodes();
		_uint iHierarchyNodeCount = HiearachyNodes.size();

		/* HierarchyNode List */
		{
			ImGui::Text("HierarchyNode List (count : %d)", iHierarchyNodeCount);

			if (ImGui::BeginListBox("##Bone_List"))
			{
				for (size_t i = 0; i < HiearachyNodes.size(); ++i)
				{
					string strBoneName = CUtils::ToString(HiearachyNodes[i]->Get_Name());
					if (ImGui::Selectable(strBoneName.c_str(), i == m_iCurBoneIndex))
					{
						m_iCurBoneIndex = i;
						m_iRenderSocketIndex = i;
					}
				}
				ImGui::EndListBox();
			}
		}
		IMGUI_NEW_LINE;

		/* Prototype Weapon List */
		{
			ImGui::Text("Weapon Prototypes List");

			if (ImGui::BeginListBox("##Weapon Prototypes List", ImVec2(0, 50)))
			{
				for (size_t i = 0; i < m_Weapons.size(); ++i)
				{
					string strBoneName = CUtils::ToString(m_Weapons[i]->Get_ObjectTag());
					if (ImGui::Selectable(strBoneName.c_str(), i == m_iCurWeaponIndex))
					{
						m_iCurWeaponIndex = i;
					}
				}
				ImGui::EndListBox();
			}
		}
		IMGUI_NEW_LINE;

		/* Calculated Socket List */
		{
			ImGui::Text("Calculated Socket List");

			if (ImGui::BeginListBox("##Calculated Socket List", ImVec2(0, 50)))
			{
				for (size_t i = 0; i < m_AddedTransformNames.size(); i++)
				{
					if (ImGui::Selectable(CUtils::ToString(m_AddedTransformNames[i]).c_str(), i == m_iSocketIndex))
					{
						m_iSocketIndex = i;


						m_iRenderSocketIndex = m_pDummy->Get_ModelCom()->Get_HierarchyNodeIndex(m_AddedTransformNames[i]);

						int k = 0;
					}
				}
				ImGui::EndListBox();
			}
		}

		/* Add */
		if (ImGui::Button("Add Socket"))
		{
			/* 모델 컴포넌트 반영 */
			m_pDummy->Get_ModelCom()->Add_SocketTransforms(
				GI->Create_AnimationSocketTransform(m_pDummy->Get_ModelCom(), m_iCurBoneIndex));

			/* 툴 네임 리스트 반영 */
			m_AddedTransformNames.push_back(HiearachyNodes[m_iCurBoneIndex]->Get_Name());

			/* 툴 인덱스 반영 */
			m_iSocketIndex = m_AddedTransformNames.size() - 1;
		}
		IMGUI_SAME_LINE;

		/* Delete */
		if (ImGui::Button("Delete Socket"))
		{
			/* 모델 컴포넌트 반영 */
			m_pDummy->Get_ModelCom()->Clear_SocketTransforms(m_iSocketIndex);

			/* 툴 네임 리스트 반영 */
			{
				vector<wstring>::iterator iter = m_AddedTransformNames.begin();
				iter += m_iSocketIndex;

				m_AddedTransformNames.erase(iter);
			}

			/* 툴 인덱스 반영 */
			m_iSocketIndex -= 1;
			if (m_iSocketIndex < 0)
			{
				m_iSocketIndex = -1;
				m_iRenderSocketIndex = m_iCurBoneIndex;
			}
			else
				m_iRenderSocketIndex = m_pDummy->Get_ModelCom()->Get_HierarchyNodeIndex(m_AddedTransformNames[m_iSocketIndex]);
		}
		IMGUI_SAME_LINE;

		/* All Delete */
		if (ImGui::Button("All Delete Socket"))
		{
			/* 모델 컴포넌트 반영 */
			m_pDummy->Get_ModelCom()->Clear_All_SocketTransforms();

			/* 툴 네임 리스트 반영 */
			m_AddedTransformNames.clear();
			m_AddedTransformNames.shrink_to_fit();

			/* 툴 인덱스 반영 */
			m_iSocketIndex -= 1;
			m_iRenderSocketIndex = m_iCurBoneIndex;
		}
		IMGUI_NEW_LINE;


		/* Rotation matrix */
		{
			_float fRot[3] = { m_vRotation.x, m_vRotation.y, m_vRotation.z };
			ImGui::Text("Socket Rotation");
			IMGUI_SAME_LINE;

			/* Clear */
			if (ImGui::Button("Clear Socket Rotation"))
				m_vRotation = Vec3::Zero;

			/* X */
			{
				if (ImGui::InputFloat("X", &fRot[0]))
					m_vRotation.x = fRot[0];

				IMGUI_SAME_LINE;

				if (ImGui::ArrowButton("##Up X", ImGuiDir_Up))
				{
					m_vRotation.x += 90.f;
					if (360 <= m_vRotation.x)
						m_vRotation.x = -270.f;
				}

				IMGUI_SAME_LINE;

				if (ImGui::ArrowButton("##Down X", ImGuiDir_Down))
				{
					m_vRotation.x -= 90.f;
					if (-360 >= m_vRotation.x)
						m_vRotation.x = 270.f;
				}
			}

			/* Y */
			{
				if (ImGui::InputFloat("Y", &fRot[1]))
					m_vRotation.y = fRot[1];

				IMGUI_SAME_LINE;

				if (ImGui::ArrowButton("##Up Y", ImGuiDir_Up))
				{
					m_vRotation.y += 90.f;
					if (360 <= m_vRotation.y)
						m_vRotation.y = -270.f;
				}

				IMGUI_SAME_LINE;

				if (ImGui::ArrowButton("##Down Y", ImGuiDir_Down))
				{
					m_vRotation.y -= 90.f;
					if (-360 >= m_vRotation.y)
						m_vRotation.y = 270.f;
				}
			}

			/* Z */
			{
				if (ImGui::InputFloat("Z", &fRot[2]))
					m_vRotation.z = fRot[2];

				IMGUI_SAME_LINE;

				if (ImGui::ArrowButton("##Up Z", ImGuiDir_Up))
				{
					m_vRotation.z += 90.f;
					if (360 <= m_vRotation.z)
						m_vRotation.z = -270.f;
				}

				IMGUI_SAME_LINE;

				if (ImGui::ArrowButton("##Down Z", ImGuiDir_Down))
				{
					m_vRotation.z -= 90.f;
					if (-360 >= m_vRotation.z)
						m_vRotation.z = 270.f;
				}
			}
			IMGUI_NEW_LINE;
		}
	}
}

void CTool_Model::Tick_Event(_float fTimeDelta)
{
	if (ImGui::CollapsingHeader("Event"))
	{
		if (Is_Exception())
			return;

		/* Sound */
		if (ImGui::TreeNode("Sound"))
		{

			ImGui::TreePop();
		}

		/* Effect */
		if (ImGui::TreeNode("Effect"))
		{

			ImGui::TreePop();
		}

		/* Camera */
		if (ImGui::TreeNode("Camera"))
		{

			ImGui::TreePop();
		}

		/* Collider  */
		if (ImGui::TreeNode("Collider"))
		{

			ImGui::TreePop();
		}
		IMGUI_NEW_LINE;
	}
}

void CTool_Model::Tick_Costume(_float fTimeDelta)
{
	if (ImGui::CollapsingHeader("Costume"))
	{
		if (Is_Exception())
			return;

		/* Function */
		{

		}
		IMGUI_NEW_LINE;
	}
}

void CTool_Model::Tick_Dummys(_float fTimeDelta)
{
	m_pDummy->Tick(fTimeDelta);
	m_pDummy->LateTick(fTimeDelta);

	if (0 <= m_iCurWeaponIndex && !m_Weapons.empty() && m_iCurWeaponIndex < m_Weapons.size() && nullptr != m_Weapons[m_iCurWeaponIndex])
	{
		m_Weapons[m_iCurWeaponIndex]->Set_SocketWorld(Calculate_SocketWorldMatrix());
	}
}

CTool_Model* CTool_Model::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Model* pInstance = new CTool_Model(pDevice, pContext);
	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed_Create : CTool_Model");
		return nullptr;
	}
	return pInstance;
}

void CTool_Model::Free()
{
	__super::Free();

	for (auto& pWeapon : m_Weapons)
		Safe_Release(pWeapon);

	m_Weapons.clear();

	Safe_Release(m_pDummy);

	Safe_Delete(m_pBatch);
	Safe_Delete(m_pEffect);
	Safe_Delete(m_pSphere);
	Safe_Release(m_pInputLayout);

}
