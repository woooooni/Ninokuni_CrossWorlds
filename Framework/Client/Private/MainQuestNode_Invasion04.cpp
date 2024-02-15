#include "stdafx.h"
#include "MainQuestNode_Invasion04.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_Fade.h"
#include "UI_PopupQuest.h"

#include "Utils.h"
#include <FileUtils.h>

CMainQuestNode_Invasion04::CMainQuestNode_Invasion04()
{
}

HRESULT CMainQuestNode_Invasion04::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("파괴된 에스타나비아");
	m_strQuestContent = TEXT("루슬란과 대화하기");

	return S_OK;
}

void CMainQuestNode_Invasion04::Start()
{
	CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 1.f);

	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);
	CUI_PopupQuest::QUEST_INFO QuestDesc = {};
	QuestDesc.strType = m_strQuestTag;
	QuestDesc.strTitle = m_strQuestName;
	QuestDesc.strContents = m_strQuestContent;
	CUI_Manager::GetInstance()->Set_QuestPopup(&QuestDesc);

	// Delete_Npc();

	if (FAILED(Load_Npc()))
		MSG_BOX("Fail MainQuest_Invasion04 : Npc Load");

	OBJECT_INIT_DESC NpcDesc = {};
	NpcDesc.vStartPosition = { 0.f, 10.f, 140.f, 1.f };
	//if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, _uint(LAYER_NPC), TEXT("Prorotype_GameObject_Ruslan"), &NpcDesc, &m_pRuslan)))
	//{
	//	MSG_BOX("Fail AddGameObj : Quest Ruslan");
	//}

	m_pRuslan = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Ruslan"));

	if (m_pRuslan != nullptr)
	{
		Vec4 vSpotPos = Set_DestSpot(m_pRuslan);
		m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_ETC), &vSpotPos));

		CUI_PopupQuest::QUEST_INFO QuestDesc = {};
		QuestDesc.strType = m_strQuestTag;
		QuestDesc.strTitle = m_strQuestName;
		QuestDesc.strContents = m_strQuestContent;
		QuestDesc.bCreateSpot = true;
		QuestDesc.vDestPosition = vSpotPos;
		CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);
	}

}

CBTNode::NODE_STATE CMainQuestNode_Invasion04::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_EVERMORE)
	{
		if (m_pQuestDestSpot != nullptr)
		{
			m_pQuestDestSpot->Tick(fTimeDelta);
			m_pQuestDestSpot->LateTick(fTimeDelta);

			if (m_pQuestDestSpot != nullptr)
			{
				if (m_pQuestDestSpot->Get_IsCol())
				{
					m_bIsClear = true;
					CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName); // 루슬란과 대화하기로 되어있다. 잘 확인하자
					m_pQuestDestSpot->Set_ReadyDelete(true);
					Safe_Release(m_pQuestDestSpot);
					return NODE_STATE::NODE_FAIL;
				}
			}
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_Invasion04::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_Invasion04::Delete_Npc()
{
	list<CGameObject*>& NpcList = GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_NPC);
	for (auto& pNpc : NpcList)
	{
		if (pNpc->Get_PrototypeTag().find(L"TreeGrandfa") != wstring::npos)
			continue;

		pNpc->Set_Dead(true);
	}
}

HRESULT CMainQuestNode_Invasion04::Load_Npc()
{
	wstring strNpcFileName = L"Evermore";
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strNpcFileName + L"/" + strNpcFileName + L"NPC_InvasionAfter.map";

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Read);

	for (_uint i = 0; i < LAYER_TYPE::LAYER_END; ++i)
	{
		if (LAYER_TYPE::LAYER_NPC != i)
			continue;

		GI->Clear_Layer(LEVELID::LEVEL_EVERMORE, i);


		_uint iObjectCount = File->Read<_uint>();

		for (_uint j = 0; j < iObjectCount; ++j)
		{
			// 3. Object_Prototype_Tag
			wstring strPrototypeTag = CUtils::ToWString(File->Read<string>());
			wstring strObjectTag = CUtils::ToWString(File->Read<string>());

			// 6. Obejct States
			_float4 vRight, vUp, vLook, vPos;

			File->Read<_float4>(vRight);
			File->Read<_float4>(vUp);
			File->Read<_float4>(vLook);
			File->Read<_float4>(vPos);


			OBJECT_INIT_DESC Init_Data = {};
			Init_Data.vStartPosition = vPos;
			CGameObject* pObj = nullptr;
			if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_EVERMORE, i, strPrototypeTag, &Init_Data, &pObj)))
			{
				MSG_BOX("Load_Objects_Failed.");
				return E_FAIL;
			}

			if (nullptr == pObj)
			{
				MSG_BOX("Add_Object_Failed.");
				return E_FAIL;
			}
			pObj->Set_ObjectTag(strObjectTag);

			CTransform* pTransform = pObj->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr == pTransform)
			{
				MSG_BOX("Get_Transform_Failed.");
				return E_FAIL;
			}

			_uint ObjectType;
			File->Read<_uint>(ObjectType);

			if (OBJ_TYPE::OBJ_NPC == ObjectType)
			{
				CGameNpc* pNpc = dynamic_cast<CGameNpc*>(pObj);

				if (pNpc == nullptr)
				{
					MSG_BOX("Fail Load : NPC");
					return E_FAIL;
				}

				_uint iSize;
				File->Read<_uint>(iSize);

				_uint eState;
				File->Read<_uint>(eState);


				if (iSize != 0)
				{
					vector<Vec4> Points;
					Points.reserve(iSize);

					for (_uint i = 0; i < iSize; ++i)
					{
						Vec4 vPoint;
						File->Read<Vec4>(vPoint);
						Points.push_back(vPoint);
					}

					pNpc->Set_RoamingArea(Points);

					if (Points.size() != 0)
					{
						vPos = Points.front();
						pNpc->Set_Point(true);
					}
				}

				CGameNpc::NPC_STAT eStat;
				File->Read<CGameNpc::NPC_STAT>(eStat);

				pNpc->Set_NpcState(static_cast<CGameNpc::NPC_STATE>(eState));
				CStateMachine* pStateMachine = pNpc->Get_Component<CStateMachine>(TEXT("Com_StateMachine"));
				if (pStateMachine != nullptr)
				{
					pStateMachine->Change_State(eState);
				}
				else
				{
					MSG_BOX("Fail Get : NPC_StateMachine");
					return E_FAIL;
				}
				pNpc->Set_Stat(eStat);
			}

			pTransform->Set_State(CTransform::STATE_RIGHT, XMLoadFloat4(&vRight));
			pTransform->Set_State(CTransform::STATE_UP, XMLoadFloat4(&vUp));
			pTransform->Set_State(CTransform::STATE_LOOK, XMLoadFloat4(&vLook));
			pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));

			CPhysX_Controller* pController = pObj->Get_Component<CPhysX_Controller>(L"Com_Controller");

			if (nullptr != pController)
				pController->Set_EnterLevel_Position(pTransform->Get_Position());
		}

	}

	return S_OK;
}

CMainQuestNode_Invasion04* CMainQuestNode_Invasion04::Create()
{
	CMainQuestNode_Invasion04* pInstance = new CMainQuestNode_Invasion04();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Invasion04");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Invasion04::Free()
{
	__super::Free();
}
