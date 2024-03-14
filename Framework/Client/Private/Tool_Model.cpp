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
#include "Weapon.h"
#include "Part.h"
#include "Character_Manager.h"

#include "Camera_Manager.h"
#include "Camera.h"

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

	if (FAILED(Ready_Weapons()))
		return E_FAIL;

	if (FAILED(Ready_SoundKey()))
		return E_FAIL;

	return S_OK;
}

void CTool_Model::Tick(_float fTimeDelta)
{
	ImGuiWindowFlags WindowFlags = 0;
	if (TRUE)
	{
		// WindowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	}

	ImGui::Begin("Model_Tool", NULL, WindowFlags);
	{
		Tick_Model(fTimeDelta);
		Tick_Animation(fTimeDelta);
		Tick_Event(fTimeDelta);
		Tick_Socket(fTimeDelta);
		Tick_Costume(fTimeDelta);
	}
	ImGui::End();

	Tick_Dummys(fTimeDelta);
}

HRESULT CTool_Model::Render()
{
	if (FAILED(Render_DebugDraw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTool_Model::Clear_ToolAnimationData()
{
	m_bAllAnimLoop = FALSE;
	/*vector<class CAnimation*>& Animations = m_pDummy->Get_ModelCom()->Get_Animations();

	for (auto& pAnim : Animations)
		pAnim->Set_Loop(m_bAllAnimLoop);
	*/
	m_iCurBoneIndex = 0;

	m_iSocketIndex = -1;
	m_iRenderSocketIndex = 0;

	m_AnimTransformsCaches.clear();
	m_AnimTransformsCaches.shrink_to_fit();

	m_bAuto = FALSE;

	m_iAutoAnimIndex = 0;

	m_iSoundEventIndex = -1;
	m_iColliderEventIndex = -1;
	m_fCurEventFrame = 0.f;


	m_bCostumeMode = FALSE;
	m_pDummy->m_bCostumeMode = FALSE;
	for (size_t i = 0; i < PART_TYPE::PART_END; i++)
	{
		m_pDummy->m_pPart[i] = nullptr;
	}

	m_bInitSocket = FALSE;

	return S_OK;
}

HRESULT CTool_Model::Claer_EventData()
{
	m_iSoundEventIndex		= -1;
	m_iColliderEventIndex	= -1;
	m_iCameraEventIndex		= -1;
	
	m_fCurEventFrame		= 0.f;

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
	if (nullptr == m_pDummy->Get_ModelCom())
		return Matrix();

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
		Vec3 vRot = m_pDummy->Get_ModelCom()->Get_CustomSocketPivotRotation(m_iSocketIndex);

		Matrix matCustomPivot = XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(
								XMConvertToRadians(vRot.x),
								XMConvertToRadians(vRot.y),
								XMConvertToRadians(vRot.z)));




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

	/* 회전 행렬 포지션 초기화 */
	Vec4 vOneW = { 0.f, 0.f, 0.f, 1.f };
	memcpy(&matAnimLocal.m[3], &vOneW, sizeof(Vec4));

	/* 회전 행렬에서 피벗 포지션 값 뽑음*/
	Vec3 vPivotPosition = m_pDummy->Get_ModelCom()->Get_CustomSocketPivotPosition(m_iSocketIndex);
	vPivotPosition = XMVector3TransformCoord(vPivotPosition, matAnimLocal);

	/* 회전 행렬 포지션에 기존 로컬 포지션과 피벗 포지션 값을 더함 */
	vAnimLocalPos += Vec4(vPivotPosition.x, vPivotPosition.y, vPivotPosition.z, 0.f);

	/* 포지션 적용  */
	memcpy(&matAnimLocal.m[3], &vAnimLocalPos, sizeof(Vec4));
	
	return matAnimLocal * m_pDummy->Get_TransformCom()->Get_WorldMatrix();
}

const _bool CTool_Model::Is_Exception()
{
	if (nullptr == m_pDummy)
	{
		ImGui::Text(u8"No Dummy");
		return true;
	}

	if (nullptr != m_pDummy->Get_ModelCom())
	{
		CModel* pModelCom = m_pDummy->Get_ModelCom();
		if (CModel::TYPE::TYPE_NONANIM == pModelCom->Get_ModelType())
		{
			ImGui::Text(u8"애니메이션 모델이 로드되지 않았습니다.");
			
			return true;
		}

		if (0 == m_pDummy->Get_ModelCom()->Get_Animations().size())
		{
			ImGui::Text(u8"애니메이션 모델이지만, 애니메이션이 없습니다.");
			return true;
		}
	}
	else if (nullptr == m_pDummy->Get_ModelCom())
	{
		ImGui::Text(u8"어떠한 모델도 로드되지 않았습니다.");
		
		return true;
	}
	else if (CModel::TYPE::TYPE_ANIM == m_pDummy->Get_ModelCom()->Get_ModelType() && m_pDummy->Get_ModelCom()->Get_Animations().empty())
	{
		ImGui::Text(u8"이 모델은 애니메이션을 갖고 있지 않습니다.");
		return true;
	}
	else
	{
		ImGui::Text(u8"애니메이션 모델이 로드되지 않았습니다.");
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

HRESULT CTool_Model::Ready_Weapons()
{
	CWeapon*			pWeapon = nullptr;
	CGameObject*	pGameObject = nullptr;

	/* Prototype_GameObject_TempSword */
	if (FAILED(GI->Add_GameObject(LEVEL_TOOL, _uint(LAYER_WEAPON), TEXT("Prototype_GameObject_TempSword"), nullptr, &pGameObject)))
		return E_FAIL;
	{
		pWeapon = dynamic_cast<CWeapon*>(pGameObject);
		if (nullptr == pWeapon)
			return E_FAIL;

		m_Weapons.push_back(pWeapon);

		pWeapon = nullptr;
		pGameObject = nullptr;
	}

	/* Npc_Halberd*/
	if (FAILED(GI->Add_GameObject(LEVEL_TOOL, _uint(LAYER_WEAPON), TEXT("Prorotype_GameObject_NpcWeapon_Halberd"), nullptr, &pGameObject)))
		return E_FAIL;
	{
		pWeapon = dynamic_cast<CWeapon*>(pGameObject);
		if (nullptr == pWeapon)
			return E_FAIL;

		m_Weapons.push_back(pWeapon);

		pWeapon = nullptr;
		pGameObject = nullptr;
	}

	return S_OK;
}

HRESULT CTool_Model::Ready_SoundKey()
{
	const map<wstring, FMOD_SOUND*>& mapSound = GI->Get_MapSound();

	if (mapSound.empty())
		return S_OK;

	/* 문자열 변환 */
	vector<wstring> strSoundKeyNames;

	for (const auto& PrevData : mapSound)
	{
		strSoundKeyNames.push_back(PrevData.first);
	}

	m_arrSoundKeys = new const char* [strSoundKeyNames.size()];

	if (nullptr == m_arrSoundKeys) 
		return E_FAIL;
	
	int index = 0;

	for (const auto& Name : strSoundKeyNames)
	{
		std::wstring wideStr = Name;
		std::string narrowStr(wideStr.begin(), wideStr.end());

		m_arrSoundKeys[index] = _strdup(narrowStr.c_str());
		++index;
	}

	m_iSoundKeySize = mapSound.size();

	return S_OK;
}

HRESULT CTool_Model::Ready_SocketTransforms()
{
	if (!m_AnimTransformsCaches.empty())
	{
		m_AnimTransformsCaches.clear();
		m_AnimTransformsCaches.shrink_to_fit();
	}
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
	if (ImGui::CollapsingHeader(u8"파일 Import, Eport"))
	{
		/* Exception */
		if (nullptr == m_pDummy)
		{
			ImGui::Text(u8"더미 오브젝트가 존재하지 않습니다.");
			return;
		}

		/* Import */
		if (ImGui::TreeNode(u8"Import"))
		{
			if (ImGui::TreeNode(u8"파일 불러오기(fbx, binary)"))
			{
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
					ImGui::Text(u8"테스트 용 : ../Bin/Export/AnimModel/Character/TempSwordMan/");
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
					ImGui::Text(u8"테스트 용 : TempSwordMan");
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
				if (ImGui::Button("Import Model"))
				{
					if (iSelectedImportModelType != -1)
					{
						if (FAILED(m_pDummy->Ready_ModelCom(iSelectedImportModelType, m_strFilePath, m_strFileName)))
							MSG_BOX("Failed Import.");
						else
						{
							MSG_BOX("Success Import.");
							if (CModel::TYPE::TYPE_ANIM == m_pDummy->Get_ModelCom()->Get_ModelType())
							{
								Clear_ToolAnimationData();
								m_pDummy->Get_ModelCom()->Set_Animation(0);				
							}
						}
					}
					else
						MSG_BOX("모델 타입을 선택해주세요");
				}
				IMGUI_NEW_LINE;
				ImGui::TreePop();
			}
			
			ImGui::TreePop();
		}

		/* Export */
		if (ImGui::TreeNode(u8"Export"))
		{
			/* Export (One File) */
			if (ImGui::TreeNode(u8"바이너리 파일 내보내기"))
			{
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
						Ready_SocketTransforms();
					}
				}
				IMGUI_NEW_LINE;
				ImGui::TreePop();
			}

			/* Export (All File) */
			if (ImGui::TreeNode(u8"FBX 폴더 일괄 임포트와 동시에 바로 바이너리 내보내기"))
			{
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
						if (FAILED(GI->Export_Model_Data_FromPath(iSelectedExportModelType, CUtils::ToWString(szAllObjectExportFolderName))))
							MSG_BOX("Failed Export.");
						else
							MSG_BOX("Save Success");

					}
					else
					{
						MSG_BOX("폴더 경로 혹은 모델 타입 지정을 확인하세요.");
					}
				}
				ImGui::TreePop();
				IMGUI_NEW_LINE;
			}

			ImGui::TreePop();
		}
		IMGUI_NEW_LINE;
	}
}


void CTool_Model::Tick_Animation(_float fTimeDelta)
{
	if (ImGui::CollapsingHeader(u8"애니메이션 편집"))
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
		if (ImGui::BeginListBox("##Animation_List", ImVec2(350.f, 150.f)))
		{
			for(size_t i = 0; i< Animations.size(); ++i)
			{
				string AnimationName = CUtils::ToString(Animations[i]->Get_AnimationName());
				if (ImGui::Selectable(AnimationName.c_str(), i == pModelCom->Get_CurrAnimationIndex()))
				{
					pModelCom->Set_Animation(i);
					sprintf_s(szAnimationName, CUtils::ToString(pModelCom->Get_Animation(i)->Get_AnimationName()).c_str());

					Claer_EventData();
				}
			}
			ImGui::EndListBox();
		}
	
		/* 변경시 다시 익스포트 해야하는 유형 */
		IMGUI_NEW_LINE;
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.f, 0.3f, 0.6f, 1.f), u8"삭제, 정렬, 순서 변경, 이름 변경은 반드시 Apply를 해야 반영됩니다.");
		ImGui::TextColored(ImVec4(1.f, 0.3f, 0.6f, 1.f), u8"모델의 애니메이션이 이상하다면 Apply 버튼을 눌러주세요.");
		ImGui::TextColored(ImVec4(1.f, 0.3f, 0.6f, 1.f), u8"Save 창이 뜨더라도 Apply의 Save 경로는 캐쉬 경로(Export/stash)이므로");
		ImGui::TextColored(ImVec4(1.f, 0.3f, 0.6f, 1.f), u8"모델의 원본은 수정되지 않습니다.");
		ImGui::Text("Prop 1");
		{
			/* Swap */
			{
				if (ImGui::ArrowButton("##Swap_Animation_Up", ImGuiDir_Up))
				{
					pModelCom->Swap_Animation(pModelCom->Get_CurrAnimationIndex(), pModelCom->Get_CurrAnimationIndex() - 1);

					if (m_bInitSocket)
					{
						if (FAILED(Ready_SocketTransforms()))
						{
							MSG_BOX("소켓 트랜스폼 갱신을 실패했습니다.");
							return;
						}
					}
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

					if (m_bInitSocket)
					{
						if (FAILED(Ready_SocketTransforms()))
						{
							MSG_BOX("소켓 트랜스폼 갱신을 실패했습니다.");
							return;
						}
					}
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
				
				wstring strAnimName = pModelCom->Get_Animation(iCurIndex)->Get_AnimationName();
				pModelCom->Delete_Animation(iCurIndex);

				iCurIndex = (0 < iCurIndex - 1) ? 0 : iCurIndex - 1;

				pModelCom->Set_Animation(iCurIndex, -1.f);

				Animations = pModelCom->Get_Animations();

				sprintf_s(szAnimationName, CUtils::ToString(pModelCom->Get_CurrAnimation()->Get_AnimationName()).c_str());
			}
			IMGUI_SAME_LINE;

			/* Sort */
			if (ImGui::Button("Sort By Name"))
			{
				vector<class CAnimation*>& Animations = pModelCom->Get_Animations();
				sort(Animations.begin(), Animations.end(), [&](CAnimation* pSrcAnimation, CAnimation* pDestAnimation) {
					return pSrcAnimation->Get_AnimationName() < pDestAnimation->Get_AnimationName();
					});

				if (m_bInitSocket)
				{
					if (FAILED(Ready_SocketTransforms()))
					{
						MSG_BOX("소켓 트랜스폼 갱신을 실패했습니다.");
						return;
					}
				}
			}
			IMGUI_SAME_LINE;

			/* Apply */
			if (ImGui::Button("Apply"))
			{
				wstring strStashPath = L"stach/";
				if (FAILED(m_pDummy->Export_Model_Bin(strStashPath, m_strFileName)))
					MSG_BOX("Failed Save.");
				else
					MSG_BOX("Save Success");

				if (m_bInitSocket)
				{
					if (FAILED(Ready_SocketTransforms()))
					{
						MSG_BOX("소켓 트랜스폼 갱신을 실패했습니다.");
						return;
					}
				}
			}

			/* Rename */
			{
				//sprintf_s(szAnimationName, CUtils::ToString(pModelCom->Get_CurrAnimation()->Get_AnimationName()).c_str());
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
		ImGui::Text("Prop 2");

		/* Play and Stop Btn*/
		_bool bStop = (m_pDummy->Get_ModelCom()->Is_Stop() || m_pDummy->Get_ModelCom()->Is_Fix()) ? TRUE : FALSE;

		if (bStop)
		{
			if (ImGui::ArrowButton("##Play_AnimationButton", ImGuiDir_Right))
			{
				_float fAnimationProgress = pModelCom->Get_Progress();

				if (fAnimationProgress >= 1.f)
					pModelCom->Set_Animation(pModelCom->Get_CurrAnimationIndex());

				pModelCom->Set_Stop_Animation(false);
			}
		}
		else
		{
			float sz = ImGui::GetFrameHeight();
			if (ImGui::Button("||", ImVec2(sz, sz)))
				pModelCom->Set_Stop_Animation(true);
		}

		IMGUI_SAME_LINE;

		/* Animation Progress  */
		CAnimation* pCurrAnimation = pModelCom->Get_CurrAnimation();
		if (nullptr != pCurrAnimation)
		{
			_float fProgress = m_pDummy->Get_ModelCom()->Get_Progress();
			ImGui::PushItemWidth(250.f);
			if (ImGui::SliderFloat("##Animation_Progress", &fProgress, 0.f, 1.f, "%.3f (progress)"))
			{
				pModelCom->Set_Stop_Animation(true);
				m_pDummy->Get_ModelCom()->Set_KeyFrame_By_Progress(fProgress);
			}
			ImGui::PopItemWidth();
			IMGUI_SAME_LINE;

			ImGui::Text("Frmae : (%0.2f/%d)", m_pDummy->Get_ModelCom()->Get_CurrAnimationFrame_WithRatio(), pCurrAnimation->Get_MaxFrameCount() - 1);
		}

		/* Set Speed */
		{
			_float fSpeed = pCurrAnimation->Get_LiveSpeed();
			ImGui::PushItemWidth(60.f);
			if (ImGui::DragFloat("##AnimationSpeed", &fSpeed, 0.01f, 0.f, 100.f))
			{
				pCurrAnimation->Set_OriginSpeed(fSpeed);
			}
			ImGui::PopItemWidth();
			IMGUI_SAME_LINE;
			ImGui::Text("Origin Speed  ");
		}

		IMGUI_SAME_LINE;

		/* Cur Loop Btn */
		{
			_bool bLoop = pCurrAnimation->Is_Loop();
			if (ImGui::Checkbox("Cur Anim Loop  ", &bLoop))
			{
				pCurrAnimation->Set_Loop(bLoop);

				if (pModelCom->Is_Finish())
				{
					pModelCom->Set_Animation(pModelCom->Get_CurrAnimationIndex());
				}
			}

		}
		IMGUI_SAME_LINE;

		/* All Loop Btn */
		{
			_bool bLoop = m_bAllAnimLoop;
			if (ImGui::Checkbox("All Anim Loop  ", &bLoop))
			{
				m_bAllAnimLoop = bLoop;

				vector<class CAnimation*>& Animations = pModelCom->Get_Animations();

				for (auto& pAnim : Animations)
				{
					pAnim->Set_Loop(m_bAllAnimLoop);
				}
			}
		}		
	

		IMGUI_NEW_LINE;
		ImGui::Separator();
		ImGui::Text(u8"애니메이션 프레임별 속도 조절");
		ImGui::TextColored(ImVec4(1.f, 0.3f, 0.6f, 1.f), u8"애니메이션이 재생되지 않는 경우 Sort 혹은 Delete를 클릭해주세요");

		IMGUI_NEW_LINE;

		/* Add SBK*/
		if (ImGui::Button("Add Desc"))
		{
			ANIM_SPEED_DESC desc = {};

			desc.fStartSpeed = desc.fEndSpeed = m_pDummy->Get_ModelCom()->Get_CurrAnimation()->Get_LiveSpeed();

			m_pDummy->Get_ModelCom()->Get_CurrAnimation()->Add_SpeedDesc(desc);
		}
		IMGUI_SAME_LINE;

		/* Delete All */
		if (ImGui::Button("Delete All Desc"))
		{
			m_pDummy->Get_ModelCom()->Get_CurrAnimation()->Delete_All_SpeedDesc();
		}
		IMGUI_SAME_LINE;

		/* Sort */
		if (ImGui::Button("Sort Desc"))
		{
			m_pDummy->Get_ModelCom()->Get_CurrAnimation()->Sort_SpeedDesces();
		}
		IMGUI_NEW_LINE;

		/* Category */
		ImGui::Text("(1)Start Frame  (2)End Frame  (3)Start Value   (4)End Value");
		

		/* List */
		vector<ANIM_SPEED_DESC> vecDesc = m_pDummy->Get_ModelCom()->Get_CurrAnimation()->Get_SpeedDescs();
		for (size_t i = 0; i < vecDesc.size(); i++)
		{
			/* desc */
			float desc[4] = { vecDesc[i].fStartFrame, vecDesc[i].fEndFrame, vecDesc[i].fStartSpeed, vecDesc[i].fEndSpeed };
			string strDescTag = (to_string(i) + "   ");
			ImGui::PushItemWidth(360.f);
			if (ImGui::DragFloat4(strDescTag.c_str(), desc, 0.01f, 0.f, 100.f, "%.2f"))
			{
				Vec4 vDesc = { desc[0], desc[1], desc[2], desc[3] };

				if (nullptr != pCurrAnimation)
				{
					const _float fMaxCount = _float(pCurrAnimation->Get_MaxFrameCount() - 1);
					std::clamp(vDesc.x, 0.f, fMaxCount);
					std::clamp(vDesc.y, 0.f, fMaxCount);

					const _float fMaxSpeed = 5.f;
					std::clamp(vDesc.z, 0.f, fMaxSpeed);
					std::clamp(vDesc.w, 0.f, fMaxSpeed);
				}
				m_pDummy->Get_ModelCom()->Get_CurrAnimation()->Change_SpeedDesc(i, vDesc);
			}
			ImGui::PopItemWidth();

			IMGUI_SAME_LINE;
			/* Delete */
			string strDelTag = "del(" + (to_string(i)) + ")";
			if (ImGui::Button(strDelTag.c_str()))
			{
				m_pDummy->Get_ModelCom()->Get_CurrAnimation()->Delete_SpeedDesc(i);
			}
		}




		IMGUI_NEW_LINE;
	}
}

void CTool_Model::Tick_Socket(_float fTimeDelta)
{
	if (ImGui::CollapsingHeader(u8"소켓, 무기 연동"))
	{
		if (Is_Exception()) return;

		if (!m_bInitSocket)
		{
			m_bInitSocket = TRUE;
			if (FAILED(Ready_SocketTransforms()))
			{
				MSG_BOX("소켓 트랜스폼 생성에 실패했습니다.");
				ImGui::TreePop();
				return;
			}
		}
		ImGui::TextColored(ImVec4(1.f, 0.3f, 0.6f, 1.f), u8"애니메이션이 편집된 경우 소켓 또한 다시 갱신이 필요합니다.");

		vector<class CHierarchyNode*>& HiearachyNodes = m_pDummy->Get_ModelCom()->Get_HierarchyNodes();
		_uint iHierarchyNodeCount = HiearachyNodes.size();

		/* HierarchyNode List */
		{
			ImGui::Text("HierarchyNode List (count : %d)", iHierarchyNodeCount);

			if (ImGui::BeginListBox("##Bone_List", ImVec2(350.f, 150.f)))
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
		ImGui::Separator();

		/* Prototype Weapon List */
		{
			ImGui::Text("현재 장착 가능한 무기 리스트");

			if (ImGui::BeginListBox("##Weapon Prototypes List", ImVec2(350.f, 80.f)))
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
		ImGui::Separator(); 

		/* Calculated Socket List */
		{
			ImGui::Text("모델에 저장된 소켓 리스트");

			if (ImGui::BeginListBox("##Calculated Socket List", ImVec2(350.f, 80.f)))
			{
				vector<_uint> SocketTransformIndexCache = m_pDummy->Get_ModelCom()->Get_SocketTransformIndexCache();

				for (size_t i = 0; i < SocketTransformIndexCache.size(); i++)
				{
					wstring strBoneName = m_pDummy->Get_ModelCom()->Get_HiearachyNodeName(SocketTransformIndexCache[i]);

					if (ImGui::Selectable(CUtils::ToString(strBoneName).c_str(), i == m_iSocketIndex))
					{
						m_iSocketIndex = i;

						m_iRenderSocketIndex = m_pDummy->Get_ModelCom()->Get_HierarchyNodeIndex(strBoneName);

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
			m_pDummy->Get_ModelCom()->Add_SocketTransformIndexCache(m_iCurBoneIndex);
			m_pDummy->Get_ModelCom()->Add_CustomSocketPivotRotation(Vec3{ 0.f, 0.f, 0.f });
			m_pDummy->Get_ModelCom()->Add_CustomSocketPivotPosition(Vec3{ 0.f, 0.f, 0.f });
			/* 툴 인덱스 반영 */
			m_iSocketIndex = m_pDummy->Get_ModelCom()->Get_SocketTransformIndexCache().size() - 1;
		}
		IMGUI_SAME_LINE;

		/* Delete */
		if (ImGui::Button("Delete Socket"))
		{
			/* 모델 컴포넌트 반영 */
			vector<_uint> SocketTransformIndexCache = m_pDummy->Get_ModelCom()->Get_SocketTransformIndexCache();

			if (!SocketTransformIndexCache.empty())
			{
				wstring strBoneName = m_pDummy->Get_ModelCom()->Get_HiearachyNodeName(SocketTransformIndexCache[m_iSocketIndex]);
				_uint iIndex = m_pDummy->Get_ModelCom()->Get_HierarchyNodeIndex(strBoneName);

				m_pDummy->Get_ModelCom()->Clear_SocketTransformsCache(m_pDummy->Get_ModelCom()->Get_HierarchyNodeIndex(strBoneName));

				/* 툴 인덱스 반영 */
				m_iSocketIndex -= 1;
				if (m_iSocketIndex < 0)
				{
					m_iSocketIndex = -1;
					m_iRenderSocketIndex = m_iCurBoneIndex;
				}
				else
				{
					strBoneName = m_pDummy->Get_ModelCom()->Get_HiearachyNodeName(SocketTransformIndexCache[m_iSocketIndex]);
					m_iRenderSocketIndex = m_pDummy->Get_ModelCom()->Get_HierarchyNodeIndex(strBoneName);
				}
			}
		}
		IMGUI_SAME_LINE;

		/* All Delete */
		if (ImGui::Button("All Delete Socket"))
		{
			/* 모델 컴포넌트 반영 */
			m_pDummy->Get_ModelCom()->Clear_All_SocketTransformsCaches();

			/* 툴 인덱스 반영 */
			m_iSocketIndex -= 1;
			m_iRenderSocketIndex = m_iCurBoneIndex;
		}
		IMGUI_NEW_LINE;


		/* Rotation matrix */
		{
			Vec3 vRot = m_pDummy->Get_ModelCom()->Get_CustomSocketPivotRotation(m_iSocketIndex);

			_float fRot[3] = { vRot.x, vRot.y, vRot.z };
			ImGui::Text("소켓 회전 (SwordMan 기준 Best 값 : {0, -270, 0})");
			ImGui::Text("모델에 소켓을 추가한 후 선택해야 합니다.");
			IMGUI_SAME_LINE;

			/* Clear */
			if (ImGui::Button("Clear Socket Rotation"))
				vRot = Vec3::Zero;

			/* X */
			{
				if (ImGui::InputFloat("X", &fRot[0]))
					vRot.x = fRot[0];

				IMGUI_SAME_LINE;

				if (ImGui::ArrowButton("##Up X", ImGuiDir_Up))
				{
					vRot.x += 90.f;
					if (360 <= vRot.x)
						vRot.x = -270.f;
				}

				IMGUI_SAME_LINE;

				if (ImGui::ArrowButton("##Down X", ImGuiDir_Down))
				{
					vRot.x -= 90.f;
					if (-360 >= vRot.x)
						vRot.x = 270.f;
				}
			}

			/* Y */
			{
				if (ImGui::InputFloat("Y", &fRot[1]))
					vRot.y = fRot[1];

				IMGUI_SAME_LINE;

				if (ImGui::ArrowButton("##Up Y", ImGuiDir_Up))
				{
					vRot.y += 90.f;
					if (360 <= vRot.y)
						vRot.y = -270.f;
				}

				IMGUI_SAME_LINE;

				if (ImGui::ArrowButton("##Down Y", ImGuiDir_Down))
				{
					vRot.y -= 90.f;
					if (-360 >= vRot.y)
						vRot.y = 270.f;
				}
			}

			/* Z */
			{
				if (ImGui::InputFloat("Z", &fRot[2]))
					vRot.z = fRot[2];

				IMGUI_SAME_LINE;

				if (ImGui::ArrowButton("##Up Z", ImGuiDir_Up))
				{
					vRot.z += 90.f;
					if (360 <= vRot.z)
						vRot.z = -270.f;
				}

				IMGUI_SAME_LINE;

				if (ImGui::ArrowButton("##Down Z", ImGuiDir_Down))
				{
					vRot.z -= 90.f;
					if (-360 >= vRot.z)
						vRot.z = 270.f;
				}
			}

			m_pDummy->Get_ModelCom()->Set_CustomSocketPivotRotation(m_iSocketIndex, vRot);
			IMGUI_NEW_LINE;
		}

		/* Position */
		{
			Vec3 vPos = m_pDummy->Get_ModelCom()->Get_CustomSocketPivotPosition(m_iSocketIndex);

			_float fPos[3] = { vPos.x, vPos.y, vPos.z };
			ImGui::Text("모델에 소켓을 추가한 후 선택해야 합니다.");
			IMGUI_SAME_LINE;

			/* Clear */
			if (ImGui::Button("Clear Socket Position"))
				vPos = Vec3::Zero;

			/* X */
			{
				if (ImGui::DragFloat("Pos X", &fPos[0], 0.005f))
					vPos.x = fPos[0];
			}

			/* Y */
			{
				if (ImGui::DragFloat("Pos Y", &fPos[1], 0.005f))
					vPos.y = fPos[1];
			}

			/* Z */
			{
				if (ImGui::DragFloat("Pos Z", &fPos[2], 0.005f))
					vPos.z = fPos[2];
			}

			m_pDummy->Get_ModelCom()->Set_CustomSocketPivotPosition(m_iSocketIndex, vPos);
			IMGUI_NEW_LINE;
		}
	}
}

void CTool_Model::Tick_Event(_float fTimeDelta)
{
	if (ImGui::CollapsingHeader("애니메이션 이벤트"))
	{
		if (Is_Exception())
			return;

		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		
		IMGUI_NEW_LINE;

		/* Data */
		CAnimation*		pCurAnim = m_pDummy->Get_ModelCom()->Get_CurrAnimation();
		_bool			bChangedEventFrame = FALSE;
		if (nullptr == pCurAnim)
			return;
		
		/* Public Slider */
		ImGui::PushItemWidth(250.f);
		{
			if (ImGui::SliderFloat("##Event_Animation_Frame", &m_fCurEventFrame, 0.f, _float(pCurAnim->Get_MaxFrameCount() - 1), "%.2f (Selected Frame)"))
				bChangedEventFrame = TRUE;
		}
		ImGui::PopItemWidth();

		IMGUI_SAME_LINE;
		ImGui::Text(u8"<- 이 프레임에 이벤트가 설정됩니다.");
		IMGUI_NEW_LINE;

		/* Tab Bar */
		if (ImGui::BeginTabBar("Animation Event ", tab_bar_flags))
		{
			CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FREE);

			/* Sound */
			if (ImGui::BeginTabItem("Sound"))
			{
				vector<pair<_float, ANIM_EVENT_SOUND_DESC>> SoundEvents = pCurAnim->Get_SoundEvents();

				IMGUI_NEW_LINE;

				/* 키프레임 수정 여부 반영 */
				if (bChangedEventFrame && !SoundEvents.empty())
				{
					SoundEvents[m_iSoundEventIndex].first = m_fCurEventFrame;
					pCurAnim->Change_EventKeyFrame(m_iSoundEventIndex, SoundEvents[m_iSoundEventIndex].first, ANIM_EVENT_TYPE::SOUND);
				}


				/* Sound Event List */
				ImGui::Text(u8"설정된 사운드 이벤트 (count : %d)", SoundEvents.size());

				if (ImGui::BeginListBox("##Sound_Event_List", ImVec2(450.f, 70.f)))
				{
					for (size_t i = 0; i < SoundEvents.size(); ++i)
					{
						int iSoundKeyCurIndex = GI->Get_SoundFileIndex(CUtils::ToWString(SoundEvents[i].second.strSoundKey));
					
						string strFrame = to_string(SoundEvents[i].first);
						strFrame = strFrame.substr(0, 6);

						string strEventName = "Frame : " + strFrame +
												"    " +
												"Name : " + string(m_arrSoundKeys[iSoundKeyCurIndex]);

						if (ImGui::Selectable(strEventName.c_str(), i == m_iSoundEventIndex))
						{
							m_iSoundEventIndex = i;

						}
					}
					ImGui::EndListBox();
				}
				IMGUI_NEW_LINE;

				/* Prop */
				{
					/* Sound Key */
					ImGui::PushItemWidth(300.f);
					{	
						if (0 <= m_iSoundEventIndex)
						{
							int iSoundKeyCurIndex = GI->Get_SoundFileIndex(CUtils::ToWString(SoundEvents[m_iSoundEventIndex].second.strSoundKey));
							if (0 <= iSoundKeyCurIndex)
							{
								const char* szSoundPreview = m_arrSoundKeys[iSoundKeyCurIndex];
							
								if (nullptr != szSoundPreview)
								{
									if (ImGui::BeginCombo("Sound Key (File Name)", szSoundPreview))
									{
										for (int n = 0; n < m_iSoundKeySize; n++)
										{
											const bool is_selected = (iSoundKeyCurIndex == n);
											if (ImGui::Selectable(m_arrSoundKeys[n], is_selected))
											{
												iSoundKeyCurIndex = n;

												SoundEvents[m_iSoundEventIndex].second.strSoundKey = CUtils::ToString(GI->Get_SoundFileKey(iSoundKeyCurIndex));

												pCurAnim->Change_SoundEvent(m_iSoundEventIndex, SoundEvents[m_iSoundEventIndex].second);
											}

											if (is_selected)
												ImGui::SetItemDefaultFocus();
										}
										ImGui::EndCombo();
									}	
								}
							}
						}
					}
					ImGui::PopItemWidth();

					/* Channel ID */
					ImGui::PushItemWidth(300.f);
					{
						if (0 <= m_iSoundEventIndex)
						{
							int iChannelCurIndex = SoundEvents[m_iSoundEventIndex].second.iChannelID;
							const char* szChannelPreview = szChannelIDNames[iChannelCurIndex];
							if (ImGui::BeginCombo("Channel ID", szChannelPreview))
							{
								for (int n = 0; n < IM_ARRAYSIZE(szChannelIDNames); n++)
								{
									const bool is_selected = (iChannelCurIndex == n);
									if (ImGui::Selectable(szChannelIDNames[n], is_selected))
									{
										iChannelCurIndex = n;
										if (0 <= m_iSoundEventIndex)
										{
											SoundEvents[m_iSoundEventIndex].second.iChannelID = iChannelCurIndex;
											pCurAnim->Change_SoundEvent(m_iSoundEventIndex, SoundEvents[m_iSoundEventIndex].second);
										}
									}

									if (is_selected)
										ImGui::SetItemDefaultFocus();
								}
								ImGui::EndCombo();
							}
						}
					}
					ImGui::PopItemWidth();

					/* Volume */

					if (0 <= m_iSoundEventIndex)
					{
						_float fSoundEventVolume = SoundEvents[m_iSoundEventIndex].second.fVolume;
						ImGui::PushItemWidth(200.f);
						if (ImGui::SliderFloat("##Sound Event Volume", &fSoundEventVolume, 0.f, 1.f, "%.3f"))
						{
								SoundEvents[m_iSoundEventIndex].second.fVolume = fSoundEventVolume;
								pCurAnim->Change_SoundEvent(m_iSoundEventIndex, SoundEvents[m_iSoundEventIndex].second);
						
						}
						ImGui::PopItemWidth();
						IMGUI_SAME_LINE;
						ImGui::Text("Volume      ");
						IMGUI_SAME_LINE;
					}

					/* Stop */
					if (0 <= m_iSoundEventIndex)
					{
						_bool bSoundEventStop = SoundEvents[m_iSoundEventIndex].second.bStop;
						if (ImGui::Checkbox("Stop", &bSoundEventStop))
						{	
							SoundEvents[m_iSoundEventIndex].second.bStop = bSoundEventStop;
							pCurAnim->Change_SoundEvent(m_iSoundEventIndex, SoundEvents[m_iSoundEventIndex].second);
						}
					}

					/* Add */
					IMGUI_NEW_LINE;

					if (ImGui::Button(u8"Add Sound Event"))
					{
						ANIM_EVENT_SOUND_DESC desc;
						desc.strSoundKey = CUtils::ToString(GI->Get_SoundFileKey(0));
						desc.iChannelID = CHANNELID::SOUND_UI;
						desc.fVolume = 0.5f;
						desc.bStop = true;
						pCurAnim->Add_SoundEvent(m_fCurEventFrame, desc);
						++m_iSoundEventIndex;
					}
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Text(u8"현재 프레임에 기본 설정으로 새로운 사운드 이벤트 추가");
						ImGui::EndTooltip();
					}
					IMGUI_SAME_LINE;

					/* Delete */
					
					if (ImGui::Button("Del Sound Event"))
					{
						pCurAnim->Del_SoundEvent(m_iSoundEventIndex);
						--m_iSoundEventIndex;
					}
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Text(u8"현재 설정된 사운드 이벤트 삭제");
						ImGui::EndTooltip();
					}
					IMGUI_SAME_LINE;

					/* Delete All */
					if (ImGui::Button("Del Sound Events"))
					{
						pCurAnim->Del_All_SoundEvent();
						m_iSoundEventIndex = -1;
					}
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Text(u8"모든 사운드 이벤트 삭제");
						ImGui::EndTooltip();
					}
					IMGUI_SAME_LINE;

					/* Sort */
					
					if (ImGui::Button("Sort Sound Events"))
					{
						pCurAnim->Sort_SoundEvents();
					}
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Text(u8"키프레임 순서대로 사운드 이벤트 정렬");
						ImGui::EndTooltip();
					}
				}

				ImGui::EndTabItem();
			}


			/* Effect */
			if (ImGui::BeginTabItem("Effect"))
			{
				ImGui::PushItemWidth(200.f);
				{
					static int iEffectCurIndex = 0;
					const char* szEffectPreview = szAnimEventEffectTypeNames[iEffectCurIndex];
					if (ImGui::BeginCombo("Effect Event Type", szEffectPreview))
					{
						for (int n = 0; n < IM_ARRAYSIZE(szAnimEventEffectTypeNames); n++)
						{
							const bool is_selected = (iEffectCurIndex == n);
							if (ImGui::Selectable(szAnimEventEffectTypeNames[n], is_selected))
								iEffectCurIndex = n;


							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

				}
				ImGui::PopItemWidth();


				IMGUI_NEW_LINE;
				if (ImGui::Button("Add Effect Event"))
				{
					
				}

				ImGui::EndTabItem();
			}

			/* Camera */
			if (ImGui::BeginTabItem("Camera"))
			{
				/* 카메라 팔로우로 변경*/
				if (E_FAIL != CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FOLLOW))
				{
					CCamera_Manager::GetInstance()->Get_CurCamera()->Set_TargetObj(m_pDummy);
					CCamera_Manager::GetInstance()->Get_CurCamera()->Set_LookAtObj(m_pDummy);
				}

				ImGui::PushItemWidth(200.f);
				{
					vector<pair<_float, CAMERA_EVENT_DESC>> CameraEvents = pCurAnim->Get_CameraEvents();

					IMGUI_NEW_LINE;

					/* 키프레임 수정 여부 반영 */
					if (bChangedEventFrame && !CameraEvents.empty())
					{
						CameraEvents[m_iCameraEventIndex].first = m_fCurEventFrame;
						pCurAnim->Change_EventKeyFrame(m_iCameraEventIndex, CameraEvents[m_iCameraEventIndex].first, ANIM_EVENT_TYPE::CAMERA);
					}

					/* Camera Event List */
					ImGui::Text(u8"설정된 카메라 이벤트 (count : %d)", CameraEvents.size());

					if (ImGui::BeginListBox("##Camera_Event_List", ImVec2(450.f, 70.f)))
					{
						for (size_t i = 0; i < CameraEvents.size(); ++i)
						{
							string strFrame = to_string(CameraEvents[i].first);
							strFrame = strFrame.substr(0, 6);
							
							string strEventName = 
								"Frame : " + strFrame +
								"    " +
								"Type : " + CUtils::ToString(CameraEventTypeNames[CameraEvents[i].second.iTag2]);

							if (ImGui::Selectable(strEventName.c_str(), i == m_iCameraEventIndex))
							{
								m_iCameraEventIndex = i;
							}
						}
						ImGui::EndListBox();
					}
					IMGUI_NEW_LINE;

					/* Prop */
					if (0 <= m_iCameraEventIndex)
					{
						/* Camera Event Type */
						int iCameraEventType = CameraEvents[m_iCameraEventIndex].second.iTag2;
						{
							const char* event_preview_value = szCameraEventTypeNames[iCameraEventType];
							if (ImGui::BeginCombo("Camera Event Type", event_preview_value))
							{
								for (int n = 0; n < CAMERA_EVENT_TYPE::CAMERA_EVENT_TYPE_END; n++)
								{
									if (CAMERA_EVENT_TYPE::DISTANCE == n)
										continue;

									const bool is_selected = (iCameraEventType == n);
									if (ImGui::Selectable(szCameraEventTypeNames[n], is_selected))
									{
										CameraEvents[m_iCameraEventIndex].second.iTag2 = iCameraEventType = n;
										pCurAnim->Change_CameraEvent(m_iCameraEventIndex, CameraEvents[m_iCameraEventIndex].second);
									}

									if (is_selected)
										ImGui::SetItemDefaultFocus();
								}
								ImGui::EndCombo();
							}
						}
						IMGUI_NEW_LINE;

						/* 카메라 이벤트 타입에 따라 프로퍼티가 달라져야 한다. */
						switch (iCameraEventType)
						{
						case CAMERA_EVENT_TYPE::FOV :
						{
							/* Target Fov */
							{
								_float fTargetFov = XMConvertToDegrees(CameraEvents[m_iCameraEventIndex].second.fTag1);
								if (ImGui::DragFloat("Target Fov (Default Fov : 60)", &fTargetFov, 0.5f, 40.f, 100.f))
								{
									CameraEvents[m_iCameraEventIndex].second.fTag1 = XMConvertToRadians(fTargetFov);
									pCurAnim->Change_CameraEvent(m_iCameraEventIndex, CameraEvents[m_iCameraEventIndex].second);
								}
	
							}

							/* Time */
							{
								_float fLerpTime = CameraEvents[m_iCameraEventIndex].second.fTag2;
								if (ImGui::DragFloat("Lerp Time", &fLerpTime, 0.05f, 0.f, 1.f))
								{
									CameraEvents[m_iCameraEventIndex].second.fTag2 = fLerpTime;
									pCurAnim->Change_CameraEvent(m_iCameraEventIndex, CameraEvents[m_iCameraEventIndex].second);
								}

							}

							/* Lerp Mode */
							{
								const char* Preview = LerpModeNames[CameraEvents[m_iCameraEventIndex].second.iTag1].c_str();

								if (ImGui::BeginCombo(u8"보간 모드", Preview))
								{
									for (int iCurComboIndex = 0; iCurComboIndex < (_uint)LERP_MODE::TYPEEND; iCurComboIndex++)
									{
										const bool is_selected = (iCurComboIndex == (_uint)CameraEvents[m_iCameraEventIndex].second.iTag1);

										if (ImGui::Selectable(LerpModeNames[iCurComboIndex].c_str(), is_selected))
										{
											CameraEvents[m_iCameraEventIndex].second.iTag1 = iCurComboIndex;
											pCurAnim->Change_CameraEvent(m_iCameraEventIndex, CameraEvents[m_iCameraEventIndex].second);
										}

										if (is_selected)
											ImGui::SetItemDefaultFocus();
									}
									ImGui::EndCombo();
								}
							}
						}
						break;
						case CAMERA_EVENT_TYPE::DISTANCE:
						{
							///* Target Distance */
							//{
							//	_float fTargetDistance = CameraEvents[m_iCameraEventIndex].second.fTag1;
							//	if (ImGui::DragFloat("Target Distance (Default Distance : 5.f)", &fTargetDistance, 0.5f, 0.f, 100.f))
							//	{
							//		CameraEvents[m_iCameraEventIndex].second.fTag1 = fTargetDistance;
							//		pCurAnim->Change_CameraEvent(m_iCameraEventIndex, CameraEvents[m_iCameraEventIndex].second);
							//	}
							//
							//}
							//
							///* Time */
							//{
							//	_float fLerpTime = CameraEvents[m_iCameraEventIndex].second.fTag2;
							//	if (ImGui::DragFloat("Lerp Time", &fLerpTime, 0.05f, 0.f, 1.f))
							//	{
							//		CameraEvents[m_iCameraEventIndex].second.fTag2 = fLerpTime;
							//		pCurAnim->Change_CameraEvent(m_iCameraEventIndex, CameraEvents[m_iCameraEventIndex].second);
							//	}
							//
							//}
							//
							///* Lerp Mode */
							//{
							//	const char* Preview = LerpModeNames[CameraEvents[m_iCameraEventIndex].second.iTag1].c_str();
							//
							//	if (ImGui::BeginCombo(u8"보간 모드", Preview))
							//	{
							//		for (int iCurComboIndex = 0; iCurComboIndex < (_uint)LERP_MODE::TYPEEND; iCurComboIndex++)
							//		{
							//			const bool is_selected = (iCurComboIndex == (_uint)CameraEvents[m_iCameraEventIndex].second.iTag1);
							//
							//			if (ImGui::Selectable(LerpModeNames[iCurComboIndex].c_str(), is_selected))
							//			{
							//				CameraEvents[m_iCameraEventIndex].second.iTag1 = iCurComboIndex;
							//				pCurAnim->Change_CameraEvent(m_iCameraEventIndex, CameraEvents[m_iCameraEventIndex].second);
							//			}
							//
							//			if (is_selected)
							//				ImGui::SetItemDefaultFocus();
							//		}
							//		ImGui::EndCombo();
							//	}
							//}
						}
						break;
						case CAMERA_EVENT_TYPE::SHAKE :
						{
							/* fAmplitude */
							{
								_float fAmplitudeInput = CameraEvents[m_iCameraEventIndex].second.fTag1;
								if (ImGui::DragFloat(u8"Shake Amplitude (쉐이킹 진폭) (적정값 : 0.1 ~ 0.5)", &fAmplitudeInput, 0.01f, 0.f, 5.f))
								{
									CameraEvents[m_iCameraEventIndex].second.fTag1 = fAmplitudeInput;
									pCurAnim->Change_CameraEvent(m_iCameraEventIndex, CameraEvents[m_iCameraEventIndex].second);
								}
							}

							/* fFrequency */
							{
								_float fFrquencyInput = CameraEvents[m_iCameraEventIndex].second.fTag2;
								if (ImGui::DragFloat(u8"Shake Frquency (쉐이킹 빈도) ", &fFrquencyInput, 0.01f, 0.f, 100.f))
								{
									CameraEvents[m_iCameraEventIndex].second.fTag2 = fFrquencyInput;
									pCurAnim->Change_CameraEvent(m_iCameraEventIndex, CameraEvents[m_iCameraEventIndex].second);
								}
							}

							/* fDuration */
							{
								_float fDurationInput = CameraEvents[m_iCameraEventIndex].second.fTag3;
								if (ImGui::DragFloat(u8"Shake Duration (쉐이킹 시간) ", &fDurationInput, 0.01f, 0.f, 10.f))
								{
									CameraEvents[m_iCameraEventIndex].second.fTag3 = fDurationInput;
									pCurAnim->Change_CameraEvent(m_iCameraEventIndex, CameraEvents[m_iCameraEventIndex].second);
								}
							}
						}
						break;
						default:
							break;
						}
					}
				}
				ImGui::PopItemWidth();
				IMGUI_NEW_LINE;

				if (ImGui::Button("Add Camera Event (Fov)"))
				{
					CAMERA_EVENT_DESC desc;
					{
						desc.iTag2 = CAMERA_EVENT_TYPE::FOV;

						desc.fTag1 = XMConvertToRadians(60.0f);
						desc.fTag2 = 0.2f;

						desc.iTag1 = (_uint)LERP_MODE::EASE_OUT;
					}
					pCurAnim->Add_CameraEvent(m_fCurEventFrame, desc);
					++m_iCameraEventIndex;
				}
				IMGUI_SAME_LINE;

				if (ImGui::Button("Add Camera Event (Distnace)"))
				{
					CAMERA_EVENT_DESC desc;
					{
						desc.iTag2 = CAMERA_EVENT_TYPE::DISTANCE;

						desc.fTag1 = Cam_Dist_Follow_Default;
						desc.fTag2 = 0.2f;

						desc.iTag1 = (_uint)LERP_MODE::EASE_OUT;
					}
					pCurAnim->Add_CameraEvent(m_fCurEventFrame, desc);
					++m_iCameraEventIndex;
				}
				IMGUI_SAME_LINE;

				if (ImGui::Button("Add Camera Event (Shake)"))
				{
					CAMERA_EVENT_DESC desc;
					{
						desc.iTag2 = CAMERA_EVENT_TYPE::SHAKE;

						desc.fTag1 = 0.1f;
						desc.fTag2 = 17.f;
						desc.fTag3 = 0.3f;
					}
					pCurAnim->Add_CameraEvent(m_fCurEventFrame, desc);
					++m_iCameraEventIndex;
				}
				IMGUI_NEW_LINE;

				if (ImGui::Button("Del Camera Event"))
				{
					pCurAnim->Del_CameraEvent(m_iCameraEventIndex);
					--m_iCameraEventIndex;
				}
				IMGUI_SAME_LINE;

				if (ImGui::Button("Del Camera Events"))
				{
					pCurAnim->Del_All_CameraEvent();
					m_iCameraEventIndex = -1;
				}
				IMGUI_SAME_LINE;

				if (ImGui::Button("Sort Camera Event"))
				{
					pCurAnim->Sort_CameraEvents();
				}

				ImGui::EndTabItem();
			}

			/* Collider */
			if (ImGui::BeginTabItem("Collider"))
			{
				CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FREE);

				/* Intro */
				{
					ImGui::TextColored(ImVec4(1.f, 0.3f, 0.6f, 1.f), u8"Detection Type당 설정 가능한 콜라이더는 1개 입니다.");
					ImGui::TextColored(ImVec4(1.f, 0.3f, 0.6f, 1.f), u8"더미에서 테스트할 Detection Type별 콜라이더를 선택해 추가해주세요");

					IMGUI_NEW_LINE;

					ImGui::PushItemWidth(200.f);
					{
						/* Collider Type */
						static int iColliderChoice = 0;
						{
							const char* Collideritems[] = { "OBB", "SPHERE" };
							const char* Collider_preview_value = Collideritems[iColliderChoice];
							if (ImGui::BeginCombo("Collider Type", Collider_preview_value))
							{
								for (int n = 0; n < IM_ARRAYSIZE(Collideritems); n++)
								{
									const bool is_selected = (iColliderChoice == n);
									if (ImGui::Selectable(Collideritems[n], is_selected))
									{
										iColliderChoice = n;
									}

									if (is_selected)
										ImGui::SetItemDefaultFocus();
								}
								ImGui::EndCombo();
							}
						}

						/* Decection Type */
						static int iDetectionChoice = 0;
						{
							const char* Detectionitems[] = { "BOUNDARY", "BODY", "ATTACK" };
							const char* Detection_preview_value = Detectionitems[iDetectionChoice];
							if (ImGui::BeginCombo("Collider Detection Type", Detection_preview_value))
							{
								for (int n = 0; n < IM_ARRAYSIZE(Detectionitems); n++)
								{
									const bool is_selected = (iDetectionChoice == n);
									if (ImGui::Selectable(Detectionitems[n], is_selected))
									{
										iDetectionChoice = n;
									}

									if (is_selected)
										ImGui::SetItemDefaultFocus();
								}
								ImGui::EndCombo();
							}
						}

						/* Add Collider */
						if (ImGui::Button("Add Collider"))
						{
							CCollider::COLLIDER_TYPE eColType;
							if (0 == iColliderChoice)
								eColType = CCollider::COLLIDER_TYPE::OBB;
							else
								eColType = CCollider::COLLIDER_TYPE::SPHERE;

							if (FAILED(m_pDummy->Add_Collider(eColType, CCollider::DETECTION_TYPE(iDetectionChoice))))
								MSG_BOX("Collider 생성에 실패했습니다.");
							else
								m_AddedColliderTypeCaches.push_back({ eColType, CCollider::DETECTION_TYPE(iDetectionChoice)});
						}
						IMGUI_NEW_LINE;

						/* 현재 설정된 콜라이더 정보 */
						ImGui::Text(u8"현재 설정된 콜라이더 정보");
						for (size_t i = 0; i < m_AddedColliderTypeCaches.size(); i++)
						{
							string strColType, strDetectionType;

							switch (m_AddedColliderTypeCaches[i].first)
							{
							case CCollider::COLLIDER_TYPE::OBB :
								strColType = "OBB";
								break;
							case CCollider::COLLIDER_TYPE::SPHERE :
								strColType = "SPHERE";
								break;
							default:
								break;
							}

							switch (m_AddedColliderTypeCaches[i].second)
							{
							case CCollider::DETECTION_TYPE::ATTACK :
								strDetectionType = "ATTACK";
								break;
							case CCollider::DETECTION_TYPE::BODY : 
								strDetectionType = "BODY";
								break;
							case CCollider::DETECTION_TYPE::BOUNDARY:
								strDetectionType = "BOUNDARY";
								break;
							default:
								break;
							}

							string strText = "Col Type : " + strColType + "    Detection Type : " + strDetectionType;
							ImGui::Text(strText.c_str());
						}

						IMGUI_NEW_LINE;
						ImGui::TextColored(ImVec4(1.f, 0.3f, 0.6f, 1.f), u8"각 클래스 Ready Collider에서 현재 설정된 콜라이더 정보와 똑같이 콜라이더를 세팅해주어야 합니다.");
						ImGui::TextColored(ImVec4(1.f, 0.3f, 0.6f, 1.f), u8"콜라이더 이벤트가 동작하지 않을시 Detection Type이 일치하는지 확인해주세요 ");

					}
					ImGui::PopItemWidth();
					
				}


				vector<pair<_float, ANIM_EVENT_COLLIDER_DESC>> ColliderEvents = pCurAnim->Get_ColliderEvents();

				IMGUI_NEW_LINE;

				/* 키프레임 수정 여부 반영 */
				if (bChangedEventFrame && !ColliderEvents.empty())
				{
					ColliderEvents[m_iColliderEventIndex].first = m_fCurEventFrame;
					pCurAnim->Change_EventKeyFrame(m_iColliderEventIndex, ColliderEvents[m_iColliderEventIndex].first, ANIM_EVENT_TYPE::COLLIDER);
				}


				/* Collider Event List */
				ImGui::Text(u8"설정된 콜라이더 이벤트 (count : %d)", ColliderEvents.size());

				if (ImGui::BeginListBox("##Collider_Event_List", ImVec2(450.f, 70.f)))
				{
					for (size_t i = 0; i < ColliderEvents.size(); ++i)
					{
						string strIndex = to_string(i);

						string strFrame = to_string(ColliderEvents[i].first);
						strFrame = strFrame.substr(0, 6);
						
						int iDetectionTypeIndex = ColliderEvents[i].second.iDetectionType;
						const char* items[] = { "BOUNDARY", "BODY", "ATTACK" };
						string strType = items[iDetectionTypeIndex];

						string strEventName = 
							"No. " + strIndex + 
							"  " +
							"Frame : " + strFrame +
							"  " +
							"Detection Type : " + strType;

						if (ImGui::Selectable(strEventName.c_str(), i == m_iColliderEventIndex))
						{
							m_iColliderEventIndex = i;
						}
					}
					ImGui::EndListBox();
				}
				IMGUI_NEW_LINE;

				/* Prop */
				ImGui::PushItemWidth(200.f);
				{
					if (0 <= m_iColliderEventIndex)
					{
						/* On Off 여부 */
						_bool bSetCollider = ColliderEvents[m_iColliderEventIndex].second.bOnOff;
						if (ImGui::Checkbox("On Off Collider", &bSetCollider))
						{
							ColliderEvents[m_iColliderEventIndex].second.bOnOff = bSetCollider;
							pCurAnim->Change_ColliderEvent(m_iColliderEventIndex, ColliderEvents[m_iColliderEventIndex].second);
						}

						/* 디텍션 타입 */ 
						{
							int iDetectionTypeIndex = ColliderEvents[m_iColliderEventIndex].second.iDetectionType;
							const char* items[] = { "BOUNDARY", "BODY", "ATTACK"};
							const char* combo_preview_value = items[iDetectionTypeIndex];  
							if (ImGui::BeginCombo("Detection Type", combo_preview_value))
							{
								for (int n = 0; n < IM_ARRAYSIZE(items); n++)
								{
									const bool is_selected = (iDetectionTypeIndex == n);
									if (ImGui::Selectable(items[n], is_selected))
									{
										iDetectionTypeIndex = n;
										if (0 <= m_iColliderEventIndex)
										{
											ColliderEvents[m_iColliderEventIndex].second.iDetectionType = iDetectionTypeIndex;
											pCurAnim->Change_ColliderEvent(m_iColliderEventIndex, ColliderEvents[m_iColliderEventIndex].second);
										}
									}

									if (is_selected)
										ImGui::SetItemDefaultFocus();
								}
								ImGui::EndCombo();
							}

						}

						/* 어택 타입 */
						{
							
							int iAttackType = ColliderEvents[m_iColliderEventIndex].second.iAttackType;
							const char* combo_preview_value = g_szColliderAttackType[iAttackType];
							if (ImGui::BeginCombo("Attack Type", combo_preview_value))
							{
								for (int n = 0; n < IM_ARRAYSIZE(g_szColliderAttackType); n++)
								{
									const bool is_selected = (iAttackType == n);
									if (ImGui::Selectable(g_szColliderAttackType[n], is_selected))
									{
										iAttackType = n;
										if (0 <= m_iColliderEventIndex)
										{
											ColliderEvents[m_iColliderEventIndex].second.iAttackType = iAttackType;
											pCurAnim->Change_ColliderEvent(m_iColliderEventIndex, ColliderEvents[m_iColliderEventIndex].second);
										}
									}

									if (is_selected)
										ImGui::SetItemDefaultFocus();
								}
								ImGui::EndCombo();
							}
						}

						/* 오프셋 */
						Vec4	vColliderOffset = ColliderEvents[m_iColliderEventIndex].second.vOffset;
						_float	fColliderOffset[3] = { vColliderOffset.x, vColliderOffset.y, vColliderOffset.z };
					
						if (ImGui::DragFloat3(u8"Collider OffSet", fColliderOffset, 10.f, -1000.f, 1000.f))
						{
							ColliderEvents[m_iColliderEventIndex].second.vOffset = Vec3{ fColliderOffset[0], fColliderOffset[1], fColliderOffset[2] };
							pCurAnim->Change_ColliderEvent(m_iColliderEventIndex, ColliderEvents[m_iColliderEventIndex].second);
						}

						/* Extent */
						Vec4	vColliderExtent = ColliderEvents[m_iColliderEventIndex].second.vExtents;
						_float	fColliderExtent[3] = { vColliderExtent.x, vColliderExtent.y, vColliderExtent.z };

						if (ImGui::DragFloat3(u8"Collider Extent", fColliderExtent, 10.f, 0.f, 1000.f))
						{
							ColliderEvents[m_iColliderEventIndex].second.vExtents = Vec3{ fColliderExtent[0], fColliderExtent[1], fColliderExtent[2] };
							pCurAnim->Change_ColliderEvent(m_iColliderEventIndex, ColliderEvents[m_iColliderEventIndex].second);
						}
					}
				}
				ImGui::PopItemWidth();
				IMGUI_NEW_LINE;

				if (ImGui::Button("Add Collider Event"))
				{
					ANIM_EVENT_COLLIDER_DESC desc;

					if (0 <= m_iColliderEventIndex)
						desc = ColliderEvents[m_iColliderEventIndex].second;
					else
					{
						desc.bOnOff = true;
						desc.vExtents = Vec3{ 50.f, 50.f, 50.f };
						desc.vOffset = Vec3::Zero;
						desc.iDetectionType = 0;
						desc.iAttackType = 0;
					}

					pCurAnim->Add_ColliderEvent(m_fCurEventFrame, desc);
					++m_iColliderEventIndex;

				}
				IMGUI_SAME_LINE;

				if (ImGui::Button("Del Collider Event"))
				{
					pCurAnim->Del_ColliderEvent(m_iColliderEventIndex);
					--m_iColliderEventIndex;
				}
				IMGUI_SAME_LINE;

				if (ImGui::Button("Del Collider Events"))
				{
					pCurAnim->Del_All_ColliderEvent();
					m_iColliderEventIndex = -1;
				}
				IMGUI_SAME_LINE;

				if (ImGui::Button("Sort Collider Event"))
				{
					pCurAnim->Sort_ColliderEvents();
				}

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
		
		IMGUI_NEW_LINE;
	}
}

void CTool_Model::Tick_Costume(_float fTimeDelta)
{
	if (ImGui::CollapsingHeader(u8"플레이어 파츠"))
	{
		if (Is_Exception())
			return;
	
		/* Start Costume */
		if (ImGui::Checkbox("Start Costume", &m_bCostumeMode))
		{
			m_pDummy->m_bCostumeMode = m_bCostumeMode;

			/* OFF */
			if (!m_bCostumeMode)
			{
				for (size_t i = 0; i < PART_TYPE::PART_END; i++)
				{
					m_pDummy->m_pPart[i] = nullptr;
				}
			}			
		}

		/* Function */
		{
			IMGUI_NEW_LINE;
			/* 플레이어 타입 */
			ImGui::PushItemWidth(200.f);
			{
				const char** items = CUtils::Get_WStrings_To_ConstChar(wstrCharacterTypeNames, CHARACTER_TYPE::CHARACTER_END);
				if (nullptr != items)
				{
					const char* szPartPreview = items[m_eCharacyerType];
					if (ImGui::BeginCombo("Player Type", szPartPreview))
					{
						for (int n = 0; n < CHARACTER_TYPE::CHARACTER_END; n++)
						{
							const bool is_selected = (m_eCharacyerType == n);
							if (ImGui::Selectable(items[n], is_selected))
							{
								m_eCharacyerType = (CHARACTER_TYPE)n;
								m_iPartIndex = 0;
							}

							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}
					CUtils::Release_WString_To_ConstChar(items, CHARACTER_TYPE::CHARACTER_END);
				}
			}
			ImGui::PopItemWidth();

			/* 파츠 타입 */
			ImGui::PushItemWidth(200.f);
			{
				const char** items = CUtils::Get_WStrings_To_ConstChar(wstrPartTypeNames, PART_TYPE::PART_END);
				if (nullptr != items)
				{
					const char* szPartPreview = items[m_ePartType];
					if (ImGui::BeginCombo("Part Type", szPartPreview))
					{
						for (int n = 0; n < PART_TYPE::PART_END; n++)
						{
							const bool is_selected = (m_ePartType == n);
							if (ImGui::Selectable(items[n], is_selected))
							{
								m_ePartType = (PART_TYPE)n;
								m_iPartIndex = 0;
								

							}

							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}
					CUtils::Release_WString_To_ConstChar(items, PART_TYPE::PART_END);
				}
			}
			ImGui::PopItemWidth();

			/* 해당 파츠 모델 리스트 */
			IMGUI_NEW_LINE;
			ImGui::Text(u8"착용 가능한 모델 리스트");

			vector<class CModel*>* pModelList = CCharacter_Manager::GetInstance()->Get_PartModels(m_eCharacyerType, m_ePartType);
			if (nullptr != pModelList)
			{
				if (ImGui::BeginListBox("##Part Model List ", ImVec2(450.f, 70.f)))
				{
					for (size_t i = 0; i < pModelList->size(); ++i)
					{
						string strFrame = CUtils::ToString((*pModelList)[i]->Get_Name());

						if (ImGui::Selectable(strFrame.c_str(), i == m_iPartIndex))
						{
							m_iPartIndex = i;

							CModel* pModel = CCharacter_Manager::GetInstance()->Get_PartModel(m_eCharacyerType, m_ePartType, m_iPartIndex);

							m_pDummy->m_pPart[m_ePartType] = pModel;	
						}
					}
					ImGui::EndListBox();
				}
			}
			else
			{
				ImGui::Text("현재 옵션으로 착용 가능항 모델이 존재하지 않습니다. ");
			}

		}
		
		for (size_t i = 0; i < 3; i++)
		{
			IMGUI_NEW_LINE;
		}

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

	for (int n = 0; n < m_iSoundKeySize; n++)
		free((void*)m_arrSoundKeys[n]);

	delete[] m_arrSoundKeys;

	for (auto& pWeapon : m_Weapons)
		Safe_Release(pWeapon);

	m_Weapons.clear();

	Safe_Release(m_pDummy);

	Safe_Delete(m_pBatch);
	Safe_Delete(m_pEffect);
	Safe_Delete(m_pSphere);
	Safe_Release(m_pInputLayout);
}
