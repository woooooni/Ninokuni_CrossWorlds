#include "stdafx.h"
#include "Tool_Model.h"
#include "imgui.h"
#include "GameInstance.h"
#include "Animation.h"
#include "Utils.h"
#include "Dummy.h"
#include "Model.h"
#include <fstream>

CTool_Model::CTool_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CTool(pDevice, pContext)
{
}

HRESULT CTool_Model::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_pDummy = CDummy::Create(m_pDevice, m_pContext, L"Dummy");

	if (FAILED(m_pDummy->Initialize(nullptr)))
		return E_FAIL;
	return S_OK;
}

void CTool_Model::Tick(_float fTimeDelta)
{
	Tick_Model(fTimeDelta);

	Tick_Animation(fTimeDelta);
}

void CTool_Model::Reset_Transform()
{
	m_pDummy->Get_TransformCom()->Set_State(CTransform::STATE::STATE_RIGHT, XMVectorSet(1.f, 0.f, 0.f, 0.f));
	m_pDummy->Get_TransformCom()->Set_State(CTransform::STATE::STATE_UP, XMVectorSet(0.f, 1.f, 0.f, 0.f));
	m_pDummy->Get_TransformCom()->Set_State(CTransform::STATE::STATE_LOOK, XMVectorSet(0.f, 0.f, 1.f, 0.f));
	m_pDummy->Get_TransformCom()->Set_State(CTransform::STATE::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));

	return;
}

void CTool_Model::Tick_Model(_float fTimeDelta)
{
	ImGui::Begin("Model_Tool");
	{
		/* Exception */
		if (nullptr == m_pDummy) 
		{
			ImGui::End();
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
			IMGUI_SAME_LINE;
			ImGui::Text("Path");

			/* File Name */
			if (ImGui::InputText("##ModelFileText", szFileName, MAX_PATH))
				m_strFileName = CUtils::ToWString(string(szFileName));
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
			IMGUI_SAME_LINE;
			ImGui::Text("Type");

			/* Ibport Btn */
			if (ImGui::Button("Import"))
			{
				if (iSelectedImportModelType != -1)
				{
					if (FAILED(m_pDummy->Ready_ModelCom(iSelectedImportModelType, m_strFilePath, m_strFileName)))
						MSG_BOX("Failed Import.");
					else
					{
						MSG_BOX("Success Import.");
						m_pDummy->Get_ModelCom()->Set_Animation(0);
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
			ImGui::Text("Type");

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
	
		/* Etc */
		{
			IMGUI_NEW_LINE;
			ImGui::Separator();
			ImGui::Text("Etc");

			/* Reset Transform*/
			if (ImGui::Button("Reset Transform"))
				Reset_Transform();
		}
	}
	ImGui::End();

	m_pDummy->Tick(fTimeDelta);
	m_pDummy->LateTick(fTimeDelta);
}


void CTool_Model::Tick_Animation(_float fTimeDelta)
{
	ImGui::Begin("Animation");

	if (nullptr == m_pDummy)
	{
		ImGui::End();
		return;
	}
	
	if (ImGui::Button("Test Animation Play"))
	{

		_uint iCurIndex = m_pDummy->Get_ModelCom()->Get_TweenDesc().cur.iAnimIndex;

		m_pDummy->Get_ModelCom()->Set_Animation(++iCurIndex);
	}


	static char szAnimationName[255];
	if (nullptr != m_pDummy->Get_ModelCom())
	{
	    CModel* pModelCom = m_pDummy->Get_ModelCom();
	    const vector<CAnimation*>& Animations = pModelCom->Get_Animations();
	
	    if (CModel::TYPE::TYPE_NONANIM == pModelCom->Get_ModelType())
	    {
	        ImGui::End();
	        return;
	    }
	        
	    if(ImGui::Button("Export_Animation_Names"))
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
	        
	
	    // AnimationList
	    if (ImGui::BeginListBox("##Animation_List"))
	    {
	        for(size_t i = 0; i< Animations.size(); ++i)
	        {
	            string AnimationName = CUtils::ToString(Animations[i]->Get_AnimationName());
	            if (ImGui::Selectable(AnimationName.c_str(), i == pModelCom->Get_CurrAnimationIndex()))
	            {
					pModelCom->Set_Animation(i);
	                sprintf_s(szAnimationName, CUtils::ToString(Animations[pModelCom->Get_CurrAnimationIndex()]->Get_AnimationName()).c_str());
	            }
	        }
	        ImGui::EndListBox();
	    }
	    IMGUI_SAME_LINE;
	
	    ImGui::BeginGroup();
	    if (ImGui::ArrowButton("##Swap_Animation_Up", ImGuiDir_Up))
	    {
	        pModelCom->Swap_Animation(pModelCom->Get_CurrAnimationIndex(), pModelCom->Get_CurrAnimationIndex() - 1);
	    }
	    IMGUI_SAME_LINE;
	    if (ImGui::ArrowButton("##Swap_Animation_Down", ImGuiDir_Down))
	    {
	        pModelCom->Swap_Animation(pModelCom->Get_CurrAnimationIndex(), pModelCom->Get_CurrAnimationIndex() + 1);
	    }
	
	        
	
	        
	        
	    ImGui::InputText("##Animation_Input_Name", szAnimationName, 255);
	    if(ImGui::Button("Rename"))
	    {
	        wstring NewAnimationName = CUtils::ToWString(string(szAnimationName));
	        if (NewAnimationName.size() > 0)
	            Animations[pModelCom->Get_CurrAnimationIndex()]->Set_AnimationName(NewAnimationName);
	    }

	
	    if (ImGui::Button("Delete") || (KEY_TAP(KEY::DEL) && ImGui::IsWindowFocused()))
	        pModelCom->Delete_Animation(pModelCom->Get_CurrAnimationIndex());
	
	    ImGui::EndGroup();
	
	    // Animation Slider
	    CAnimation* pCurrAnimation = Animations[pModelCom->Get_CurrAnimationIndex()];
	
	    _float fPlayTime = pCurrAnimation->Get_PlayTime();
	    if (ImGui::SliderFloat("##Animation_PlayTime", &fPlayTime, 0.f, pCurrAnimation->Get_Duration()))
	    {
	        pCurrAnimation->Set_AnimationPlayTime(m_pDummy->Get_TransformCom(), fPlayTime, fTimeDelta);
	    }
	
	    _float fAnimationProgress = pCurrAnimation->Get_AnimationProgress();
	
	    ImGui::Text("Progress : ");
	    IMGUI_SAME_LINE;
	        
	    ImGui::Text(to_string(fAnimationProgress).c_str());
	
	    if (ImGui::ArrowButton("##Play_AnimationButton", ImGuiDir_Right))
	    {
	        if(fAnimationProgress >= 1.f)
	            pModelCom->Set_Animation(pModelCom->Get_CurrAnimationIndex());
	
	        pCurrAnimation->Set_Pause(false);
	    }
	            

	
	    IMGUI_SAME_LINE;
	
	    if (ImGui::Button("||"))
	        pCurrAnimation->Set_Pause(true);
	
	    IMGUI_SAME_LINE;
	    if (ImGui::Button("Sort"))
	    {
	        vector<class CAnimation*>& Animations = pModelCom->Get_Animations();
	        sort(Animations.begin(), Animations.end(), [&](CAnimation* pSrcAnimation, CAnimation* pDestAnimation) {
	            return pSrcAnimation->Get_AnimationName() < pDestAnimation->Get_AnimationName();
	        });
	    }
	
	    _float fSpeed = pCurrAnimation->Get_AnimationSpeed();
	    ImGui::Text("Speed");
	    IMGUI_SAME_LINE;
	    if (ImGui::DragFloat("##AnimationSpeed", &fSpeed, 0.01f, 0.f, 100.f))
	    {
	        pCurrAnimation->Set_AnimationSpeed(fSpeed);
	    }
	
	    IMGUI_NEW_LINE;
	
	    _bool bRootAnimation = pCurrAnimation->Is_RootAnimation();
	    ImGui::Text("Root_Animation");
	    IMGUI_SAME_LINE;
	    ImGui::Checkbox("##IsRootAnimation", &bRootAnimation);
	    pCurrAnimation->Set_RootAnimation(bRootAnimation);
	
	    _bool bLoop = pCurrAnimation->Is_Loop();
	    ImGui::Text("Loop");
	    IMGUI_SAME_LINE;
	    if(ImGui::Checkbox("##IsLoop", &bLoop))
	    pCurrAnimation->Set_Loop(bLoop);
	}
	ImGui::End();
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
}
