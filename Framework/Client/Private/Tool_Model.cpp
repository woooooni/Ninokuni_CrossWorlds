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
	m_bAllAnimLoop = FALSE;

	m_iCurBoneIndex = 0;

	m_iSocketIndex = -1;
	m_iRenderSocketIndex = 0;

	m_AnimTransformsCaches.clear();
	m_AnimTransformsCaches.shrink_to_fit();

	m_AddedTransformNames.clear();
	m_AddedTransformNames.shrink_to_fit();

	return S_OK;
}

Vec3 CTool_Model::Calculate_SocketPosition()
{
	TweenDesc TweenDesc = m_pDummy->Get_ModelCom()->Get_TweenDesc();

	Matrix matSocketLocal;

	/* 현재 프레임 계산 */
	matSocketLocal = Matrix::Lerp(m_AnimTransformsCaches[TweenDesc.cur.iAnimIndex].transforms[TweenDesc.cur.iCurFrame][m_iRenderSocketIndex],
		m_AnimTransformsCaches[TweenDesc.cur.iAnimIndex].transforms[TweenDesc.cur.iNextFrame][m_iRenderSocketIndex],
		TweenDesc.cur.fRatio);

	/* 다음 프레임이 예약되어 있다면 추가 계산 */
	if (0 <= TweenDesc.next.iAnimIndex)
	{
		Matrix matRootNextLerp = Matrix::Lerp(m_AnimTransformsCaches[TweenDesc.next.iAnimIndex].transforms[TweenDesc.next.iCurFrame][m_iRenderSocketIndex],
			m_AnimTransformsCaches[TweenDesc.next.iAnimIndex].transforms[TweenDesc.next.iNextFrame][m_iRenderSocketIndex],
			TweenDesc.next.fRatio);

		matSocketLocal = Matrix::Lerp(matSocketLocal, matRootNextLerp, TweenDesc.fTweenRatio);
	}

	Matrix matWorld = matSocketLocal * m_pDummy->Get_TransformCom()->Get_WorldMatrix();

	Vec3 vWorldPos = { matWorld._41, matWorld._42, matWorld._43 };

	return vWorldPos;
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
	//m_WeaponPrototypes.push_back()
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

	cout <<
		"뼈 인덱스 : " << m_iCurBoneIndex 
		<< "\t 소켓 인덱스 : " << m_iSocketIndex 
		<< "\t 렌더 인덱스 : " << m_iRenderSocketIndex << endl;

		
	

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

	m_pDummy->Tick(fTimeDelta);
	m_pDummy->LateTick(fTimeDelta);
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
				for (size_t i = 0; i < m_WeaponPrototypes.size(); ++i)
				{
					string strBoneName = CUtils::ToString(m_WeaponPrototypes[i]->Get_ObjectTag());
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

	Safe_Release(m_pDummy);

	Safe_Delete(m_pBatch);
	Safe_Delete(m_pEffect);
	Safe_Delete(m_pSphere);
	Safe_Release(m_pInputLayout);
}
