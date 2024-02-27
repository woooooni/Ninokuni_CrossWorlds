#include "stdafx.h"
#include "..\Public\CurlingGame_Manager.h"

#include "GameInstance.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "UIMinigame_Manager.h"
#include "UI_Fade.h"
#include "Effect_Manager.h"

#include "Camera_Group.h"
#include "CurlingGame_Group.h"

#include "Player.h"
#include "Character.h"

#include "Animation.h"



using Ray = DirectX::SimpleMath::Ray;

IMPLEMENT_SINGLETON(CCurlingGame_Manager)

CCurlingGame_Manager::CCurlingGame_Manager()
{
	
}

HRESULT CCurlingGame_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	if (m_bReserved)
		return S_OK;

	m_bReserved = true;

	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Objects()))
		return E_FAIL;

	if (FAILED(Ready_AiPathQueue()))
		return E_FAIL;

#ifdef NDEBUG
	if (m_bDebugRender && FAILED(Ready_DebugDraw()))
		return E_FAIL;
#endif

	m_tParticipants[PARTICIPANT_NPC].iNumStone = g_iMaxStoneCount;
	m_tParticipants[PARTICIPANT_PLAYER].iNumStone = g_iMaxStoneCount;
	return S_OK;
}

void CCurlingGame_Manager::Tick(const _float& fTimeDelta)
{
	if (!m_bReserved || !m_bPlaying)
		return;

	m_pManagerStateMachineCom->Tick(fTimeDelta);

	// ���� ���̵� �ƿ��� �����ٸ� 
	if (m_bFadeOut)
	{
		if (CUI_Manager::GetInstance()->Is_FadeFinished())
		{
			// ���̵� �� ����
			CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 2.f);

			// ĳ���Ͱ� ī�޶� ������ ���� (��ȭ��)
			Ready_EndingTransform();

			// �ø����� UI off 
			CUIMinigame_Manager::GetInstance()->OnOff_CurlingUI(false);

			m_bPlaying = false;
		}
	}
}

void CCurlingGame_Manager::LateTick(const _float& fTimeDelta)
{
	if (!m_bReserved || !m_bPlaying)
		return;

	m_pManagerStateMachineCom->LateTick(fTimeDelta);

	//Debug();
}

void CCurlingGame_Manager::Render_Debug()
{
	if (m_bPlaying)
		m_pManagerStateMachineCom->Render();

#ifdef NDEBUG
	if(m_bDebugRender)
		Render_DebugDraw();
#endif

}

HRESULT CCurlingGame_Manager::Ready_Game()
{
	// ȣ���, ����� �����ִ� ī�޶� �׼� ���� 

	if (m_bPlaying)
		return E_FAIL;

	m_bPlaying = true;

	if (FAILED(m_pManagerStateMachineCom->Change_State(INTRO)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCurlingGame_Manager::Start_Game()
{
	// ȣ��� �÷��̾� ������ ���� ���� ���� 

	if (FAILED(m_pManagerStateMachineCom->Change_State(MOVE)))
		return E_FAIL;
	
	CUIMinigame_Manager::GetInstance()->OnOff_CurlingUI(true);

	return S_OK;
}

HRESULT CCurlingGame_Manager::Finish_Game()
{
	// ���̵� �ƿ�
	if (!m_bFadeOut)
	{
		CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, 2.f);
		m_bFadeOut = true;
	}

	return S_OK;
}

HRESULT CCurlingGame_Manager::Ready_Components()
{
	/* Manager StateMachine */
	{
		/* ������ ������Ʈ�ӽ��� �ƴ϶� Ŭ�� �Ŵ����� ������Ʈ �ӽ��̴�. */

		m_pManagerStateMachineCom = CManager_StateMachine::Create();

		if (nullptr == m_pManagerStateMachineCom)
			return E_FAIL;
	}

	/* Manager States */
	{
		if (FAILED(m_pManagerStateMachineCom->Add_State(CURLINGGAME_STATE::INTRO, CState_CurlingGame_Intro::Create(m_pManagerStateMachineCom))))
			return E_FAIL;

		if (FAILED(m_pManagerStateMachineCom->Add_State(CURLINGGAME_STATE::MOVE, CState_CurlingGame_Move_Character::Create(m_pManagerStateMachineCom))))
			return E_FAIL;

		if (FAILED(m_pManagerStateMachineCom->Add_State(CURLINGGAME_STATE::DIRECTION, CState_CurlingGame_Choose_Direction::Create(m_pManagerStateMachineCom))))
			return E_FAIL;

		if (FAILED(m_pManagerStateMachineCom->Add_State(CURLINGGAME_STATE::INTENSITY, CState_CurlingGame_Adjust_Intensity::Create(m_pManagerStateMachineCom))))
			return E_FAIL;

		if (FAILED(m_pManagerStateMachineCom->Add_State(CURLINGGAME_STATE::LAUNCH, CState_CurlingGame_Launch_Stone::Create(m_pManagerStateMachineCom))))
			return E_FAIL;

		if (FAILED(m_pManagerStateMachineCom->Add_State(CURLINGGAME_STATE::ENDING, CState_CurlingGame_Ending::Create(m_pManagerStateMachineCom))))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CCurlingGame_Manager::Ready_Objects()
{
	/* Create Prototype */
	{
		/* Stones */
		{
			if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Prop_Barrel", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/CurlingGame/Barrel/", L"Prop_Barrel")))
				return E_FAIL;

			if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Prop_Pot", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/CurlingGame/Pot/", L"Prop_AlcoholPotA")))
				return E_FAIL;

			if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_CurlingGame_Stone",
				CCurlingGame_Stone::Create(m_pDevice, m_pContext, TEXT("CurlingGame_Stone")), LAYER_TYPE::LAYER_PROP)))
				return E_FAIL;
		}

		/* Wall */
		{
			if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_CurlingGame_Wall",
				CCurlingGame_Wall::Create(m_pDevice, m_pContext, TEXT("CurlingGame_Wall")), LAYER_TYPE::LAYER_PROP)))
				return E_FAIL;
		}

		/* Arrow */
		{
			if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Prop_Arrow", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/CurlingGame/Arrow/", L"Arrow")))
				return E_FAIL;

			if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_CurlingGame_Arrow",
				CCurlingGame_Arrow::Create(m_pDevice, m_pContext, TEXT("CurlingGame_Arrow")), LAYER_TYPE::LAYER_PROP)))
				return E_FAIL;
		}

		/* DeadZone */
		{
			CGameObject* pClone = nullptr;

			if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_CurlingGame_DeadZone",
				CCurlingGame_DeadZone::Create(m_pDevice, m_pContext, TEXT("CurlingGame_DeadZone")), LAYER_TYPE::LAYER_PROP)))
				return E_FAIL;

			if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_ICELAND, LAYER_TYPE::LAYER_PROP, TEXT("Prorotype_GameObject_CurlingGame_DeadZone"), nullptr, &pClone)))
				return E_FAIL;

			/* Transfrom */
			{
				CTransform* pTransform = pClone->Get_Component_Transform();
				pTransform->Set_Position(m_tStandardDesc.vGoalPosition);

				const Vec4 vPos		= m_tStandardDesc.vGoalPosition + m_tStandardDesc.vStartLook * 13.f;
				const Vec4 vLookAt	= m_tStandardDesc.vGoalPosition + m_tStandardDesc.vStartLook * 5.f;
				pTransform->LookAt_ForLandObject(vLookAt);
				pTransform->Set_Position(vPos);
			}
		}
	}

	/* Wall */
	enum DIR_LOCATION { LEFT, RIGHT, DIR_LOCATION_TYPEEND };

	for (size_t i = 0; i < DIR_LOCATION_TYPEEND; i++)
	{
		CGameObject* pClone = nullptr;
		CCurlingGame_Wall* pWall = nullptr;
		CTransform* pWallTransform = nullptr;

		/* Exception */
		{
			if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_ICELAND, LAYER_TYPE::LAYER_PROP, TEXT("Prorotype_GameObject_CurlingGame_Wall"), nullptr, &pClone)))
				return E_FAIL;

			pWall = dynamic_cast<CCurlingGame_Wall*>(pClone);
			if (nullptr == pWall)
				return E_FAIL;

			pWallTransform = pWall->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr == pWallTransform)
				return E_FAIL;
		}

		/* Left ���� Wall ������*/
		Vec4 vLookAt;
		Vec3 vNormal;

		if (DIR_LOCATION::LEFT == i)
		{
			const Vec4 vPos = { -131.64f, -3.f, 219.8f, 1.f };
			Vec3 vLook = Vec3{ 0.6f, 0.f, 1.9f }.Normalized();

			vLookAt = vPos + (vLook * 10.f);

			pWallTransform->Set_State(CTransform::STATE_POSITION, vPos);
			pWallTransform->LookAt_ForLandObject(vLookAt.OneW());

			vNormal = Vec3(pWallTransform->Get_Look()).ZeroY().Normalized();
		}
		else
		{
			const Vec4 vPos = { -119.931f, -3.f, 241.73f, 1.f };
			Vec3 vLook = Vec3{ 0.55f, 0.f, 1.92f }.Normalized();

			vLookAt = vPos + (vLook * 10.f);

			pWallTransform->Set_State(CTransform::STATE_POSITION, vPos);
			pWallTransform->LookAt_ForLandObject(vLookAt.OneW());
			pWallTransform->Set_State(CTransform::STATE_POSITION, Vec4(vPos + Vec4(pWallTransform->Get_Right() * -5.5f)).OneW());
			vNormal = Vec3(pWallTransform->Get_Look()).ZeroY().Normalized();
		}

		pWall->Set_Normal(vNormal);
	}

	/* Stadium Objects  */
	{
		m_pStadiumObjects.reserve(50);
	}
	return S_OK;
}

HRESULT CCurlingGame_Manager::Ready_AiPathQueue()
{
	/* ť�� ���ִٸ� ����. */
	while (!m_tAiPathQueue.empty())
		m_tAiPathQueue.pop();
	
	/* ť�� ä���. */
	{
		m_tAiPathQueue.push(AI_PATH_DESC(
			0.81f,
			Vec4(-109.064f, -3.4, 226.065f, 1.f),
			Vec4(-0.9343f, 0.f, -0.3562f, 0.f).Normalized()));

		m_tAiPathQueue.push(AI_PATH_DESC(
			0.75f,
			Vec4(-109.7575f, -3.4, 224.1645f, 1.f),
			Vec4(-0.7464f, 0.f, 0.6654f, 0.f).Normalized()));

		m_tAiPathQueue.push(AI_PATH_DESC(
			0.73f,
			Vec4(-110.1323f, -3.4, 223.54f, 1.f),
			Vec4(-0.9881f, 0.f, -0.153f, 0.f).Normalized()));

		m_tAiPathQueue.push(AI_PATH_DESC(
			0.7f,
			Vec4(-109.44f, -3.4, 229.5521f, 1.f),
			Vec4(-0.8086f, 0.f, 0.5882f, 0.f).Normalized()));

		m_tAiPathQueue.push(AI_PATH_DESC(
			0.74f,
			Vec4(-109.7352f, -3.4, 229.9734f, 1.f),
			Vec4(-0.9417f, 0.f, -0.3362f, 0.f).Normalized()));

		m_tAiPathQueue.push(AI_PATH_DESC(
			0.76f,
			Vec4(-109.95f, -3.4, 226.9316f, 1.f),
			Vec4(-0.9654f, 0.f, -0.2606f, 0.f).Normalized()));

		m_tAiPathQueue.push(AI_PATH_DESC(
			0.805f,
			Vec4(-110.6f, -3.4, 221.8f, 1.f),
			Vec4(-0.9595f, 0.f, -0.2814f, 0.f).Normalized()));

		m_tAiPathQueue.push(AI_PATH_DESC(
			0.79f,
			Vec4(-110.77f, -3.4, 222.399f, 1.f),
			Vec4(-0.968f, 0.f, -0.2487f, 0.f).Normalized()));

		m_tAiPathQueue.push(AI_PATH_DESC(
			0.75f,
			Vec4(-108.249f, -3.4, 229.9198f, 1.f),
			Vec4(-0.7858f, 0.f, 0.6183f, 0.f).Normalized()));

		m_tAiPathQueue.push(AI_PATH_DESC(
			0.75f,
			Vec4(-110.44f, -3.4, 225.0506f, 1.f),
			Vec4(-0.804f, 0.f, 0.5941f, 0.f).Normalized()));
	}

	/* ť�� ���´�. */
	{
		vector<AI_PATH_DESC> tempVector;

		while (!m_tAiPathQueue.empty())
		{
			tempVector.push_back(m_tAiPathQueue.front());
			m_tAiPathQueue.pop();
		}

		shuffle(tempVector.begin(), tempVector.end(), std::mt19937(std::random_device()()));

		for (const auto& element : tempVector) 
		{
			m_tAiPathQueue.push(element);
		}
	}


	/* ��Ÿ ���� */
	const Vec3		vRight = Vec3(XMVector3Cross(Vec3::UnitY, m_tStandardDesc.vStartLook)).ZeroY().Normalized();
	const _float	fDelta = 1.5f;

	m_StartPointDeltas.push_back(Vec3::Zero);
	m_StartPointDeltas.push_back(vRight * fDelta * 1.f);
	m_StartPointDeltas.push_back(vRight * fDelta * -1.f);
	m_StartPointDeltas.push_back(vRight * fDelta * 2.f);
	m_StartPointDeltas.push_back(vRight * fDelta * -2.f);
	m_StartPointDeltas.push_back(vRight * fDelta * 3.f);
	m_StartPointDeltas.push_back(vRight * fDelta * -3.f);
	
	return S_OK;
}

HRESULT CCurlingGame_Manager::Ready_EndingTransform()
{
	// �� ĳ���� ������ ���� 
	{
		const Vec3 vLook = m_tStandardDesc.vStartLook;

		const Vec3 vRight = Vec3(XMVector3Cross(Vec3::UnitY, vLook)).ZeroY().Normalized();

		for (_uint i = 0; i < CCurlingGame_Manager::PARTICIPANT_TYPEEND; i++)
		{
			Vec3 vStartPosition = m_tStandardDesc.vStartLinePosition + (vLook * -4.f);

			if (CCurlingGame_Manager::PARTICIPANT_PLAYER == i)
			{
				vStartPosition += vRight * m_tStandardDesc.vStartPosDelta;
			}
			else if (CCurlingGame_Manager::PARTICIPANT_NPC == i)
			{
				vStartPosition += -vRight * m_tStandardDesc.vStartPosDelta;
			}

			m_tParticipants[i].pOwner->Get_Component_Transform()->Set_Position(Vec4(vStartPosition).OneW());
		}

		m_tParticipants[PARTICIPANT_TYPE::PARTICIPANT_NPC].pOwner->Get_Component_Transform()->LookAt_ForLandObject
		(m_tParticipants[PARTICIPANT_TYPE::PARTICIPANT_PLAYER].pOwner->Get_Component_Transform()->Get_Position());
	}

	/* ��ȭ ī�޶� ���� */
	{
		CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
		if (nullptr != pActionCam)
			pActionCam->Start_Action_Talk(m_tParticipants[PARTICIPANT_TYPE::PARTICIPANT_NPC].pOwner);
	}

	/* ��� ���� ��Ȱ��ȭ */
	{
		for (auto& pStone : m_pStonesLaunched)
		{
			if (nullptr != pStone)
				pStone->Set_Active(false);
		}

		if (nullptr != m_pCurStone && m_pCurStone->Is_Active())
			m_pCurStone->Set_Active(false);
	}

	return S_OK;
}

HRESULT CCurlingGame_Manager::Change_Turn()
{
	CGameObject* pClone = nullptr;
	CCurlingGame_Stone::STONE_INIT_DESC tStoneInitDesc;

	m_pCurStone = nullptr;

	/* Inverse Turn*/
	m_bPlayerTurn = !m_bPlayerTurn; // true;
	{
		if (m_bPlayerTurn)
		{
			m_pCurParticipant = m_tParticipants[CCurlingGame_Manager::PARTICIPANT_PLAYER].pOwner;
			m_pPrevParticipant = m_tParticipants[CCurlingGame_Manager::PARTICIPANT_NPC].pOwner;
		}
		else
		{
			m_pCurParticipant = m_tParticipants[CCurlingGame_Manager::PARTICIPANT_NPC].pOwner;
			m_pPrevParticipant = m_tParticipants[CCurlingGame_Manager::PARTICIPANT_PLAYER].pOwner;
		}
	}

	/* ���� ���� */
	{
		if (m_bPlayerTurn)
			tStoneInitDesc.eStoneType = CCurlingGame_Stone::STONE_TYPE::BARREL;
		else
			tStoneInitDesc.eStoneType = CCurlingGame_Stone::STONE_TYPE::POT;

		if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP,
			TEXT("Prorotype_GameObject_CurlingGame_Stone"), &tStoneInitDesc, &pClone)))
			return E_FAIL;

		m_pCurStone = dynamic_cast<CCurlingGame_Stone*>(pClone);
		if (nullptr == m_pCurStone)
			return E_FAIL;
	}

	/* ���� �� �÷��̾� ���� */
	if(nullptr != m_pCurParticipant)
	{
		/* Ʈ������ ���� */
		{
			const Vec4 vPos = Vec4(m_tStandardDesc.vStartLinePosition + (m_tStandardDesc.vStartLook * -5.f)).OneW();

			m_pCurParticipant->Get_Component_Transform()->Set_Position(vPos);
			m_pCurParticipant->Get_Component_Transform()->LookAt_ForLandObject(m_tStandardDesc.vGoalPosition);
		}

		/* �ִϸ��̼� ���� */
		if (m_bPlayerTurn)
		{
			CCharacter* pCharacter = dynamic_cast<CCharacter*>(m_pCurParticipant);
			if (nullptr != pCharacter)
			{
				pCharacter->Set_Target(m_pCurStone);

				pCharacter->Set_Move_Input(true);

				pCharacter->Get_Component_Model()->Set_CanChangeAnimation(true);

				if (FAILED(pCharacter->Get_Component_StateMachine()->Change_State(CCharacter::NEUTRAL_PICK_LARGE_IDLE)))
					return E_FAIL;
			}
		}
		else
		{
			CModel* pModelCom = m_pCurParticipant->Get_Component_Model();
			if (nullptr != pModelCom)
			{
				pModelCom->Set_CanChangeAnimation(true);
				{
					pModelCom->Set_Animation(L"SKM_Destroyer_Merge.ao|Destroyer_PickStandL", MIN_TWEEN_DURATION);
					CAnimation* pAnim = pModelCom->Get_Animation("SKM_Destroyer_Merge.ao|Destroyer_PickStandL");
					if (nullptr != pAnim)
						pAnim->Set_Loop(true);
				}
				pModelCom->Set_CanChangeAnimation(false);
			}
		}
	}

	/* ���� �� �÷��̾� ���� */
	if(nullptr != m_pPrevParticipant)
	{
		/* Ʈ������ ���� */
		{
			_float fDeltaX = 9.f; // ���� 3
			const Vec4 vPos = Vec4(m_tStandardDesc.vStartLinePosition + (m_tStandardDesc.vStartLook * -6.f)).OneW();

			if (m_bPlayerTurn)
				fDeltaX *= -1.f;

			m_pPrevParticipant->Get_Component_Transform()->Set_Position(vPos);
			m_pPrevParticipant->Get_Component_Transform()->LookAt_ForLandObject(m_tStandardDesc.vGoalPosition);

			const Vec4 vOffset = m_pPrevParticipant->Get_Component_Transform()->Get_RelativeOffset(Vec4{ fDeltaX, 0.f, 0.f, 1.f }).ZeroW();
			m_pPrevParticipant->Get_Component_Transform()->Set_Position(vPos + vOffset);
		}

		/* �ִϸ��̼� ���� */
		{
			//m_pPrevParticipant->Get_Component_Model()->Set_CanChangeAnimation(true);
			//if (m_bPlayerTurn)
			//{
			//	m_pPrevParticipant->Get_Component_Model()->Set_Animation(L"SKM_Destroyer_Merge.ao|Destroyer_ChairSitLoop", MIN_TWEEN_DURATION);
			//	CAnimation* pAnim = m_pPrevParticipant->Get_Component_Model()->Get_Animation("SKM_Destroyer_Merge.ao|Destroyer_ChairSitLoop");
			//	if (nullptr != pAnim)
			//		pAnim->Set_Loop(true);
			//}
			//else
			//{
			//	m_pPrevParticipant = m_tParticipants[CCurlingGame_Manager::PARTICIPANT_PLAYER].pOwner;
			//	m_pPrevParticipant->Get_Component_Model()->Set_Animation(L"SKM_Swordsman_Merge.ao|Swordsman_ChairSitLoop", MIN_TWEEN_DURATION);
			//	CAnimation* pAnim = m_pPrevParticipant->Get_Component_Model()->Get_Animation("SKM_Swordsman_Merge.ao|Swordsman_ChairSitLoop");
			//	if (nullptr != pAnim)
			//		pAnim->Set_Loop(true);
			//}
			//m_pPrevParticipant->Get_Component_Model()->Set_CanChangeAnimation(false);
		}
	}

	if (nullptr == m_pCurStone || nullptr == m_pCurParticipant)
		return E_FAIL;

	return S_OK;
}

HRESULT CCurlingGame_Manager::Set_AiPath()
{
	memcpy(&m_tPrevAiPath, &m_tCurAiPath, sizeof(AI_PATH_DESC));
	ZeroMemory(&m_tCurAiPath, sizeof(AI_PATH_DESC));

	/* ��Ÿ�� ����Ʈ 6�� ��Ƽ� �浹 ���� ������ �״�� �߻� */
	{
		/* Create Start Points */
		vector<Vec4>	StartPoints;
		const Vec3		vCenterPos	= m_tStandardDesc.vStartLinePosition + m_tStandardDesc.vStartLook * -3.f;
		const Vec3		vRight		= Vec3(XMVector3Cross(Vec3::UnitY, m_tStandardDesc.vStartLook)).ZeroY().Normalized();
		const _float	fDelta		= 1.5f;


		Vec3 vStartPos = {}, vTargetPos = {};
		const Vec3 vWidthDelta = (vRight * 0.5f).ZeroY(); // ���� 0.5�̰�, �ؿ� ���� ����Ʈ ����Ʈ �𷺼ǿ� ��Ÿ * 0.5 ����� �Ѵ�.

		/* ������ ���� ��Ÿ ���� */
		for (auto& vDelta : m_StartPointDeltas)
		{
			vDelta *= -1.f;
		}


		for (const auto& vDelta : m_StartPointDeltas)
		{
			Ray rayCenter, rayLeft, rayRight;

			/* Center */
			{
				rayCenter.position = vCenterPos + vDelta;
				rayCenter.direction = Vec3((m_tStandardDesc.vGoalPosition + vDelta * 0.5f) - rayCenter.position).ZeroY().Normalized();
			}

			/* Left */
			{
				rayLeft.position = vCenterPos + vDelta - vWidthDelta;
				rayLeft.direction = Vec3((m_tStandardDesc.vGoalPosition + vDelta * 0.5f - vWidthDelta) - rayLeft.position).ZeroY().Normalized();

			}

			/* Right */
			{
				rayRight.position = vCenterPos + vDelta + vWidthDelta;
				rayRight.direction = Vec3((m_tStandardDesc.vGoalPosition + vDelta * 0.5f + vWidthDelta) - rayRight.position).ZeroY().Normalized();
			}

			_bool bCollision = false;

			/* �� ����, �� ���� �浹 �˻� */
			for (const auto& pStone : m_pStonesLaunched)
			{
				if (nullptr == pStone || pStone->Is_Outted() || !pStone->Is_Active())
					continue;

				CCollider_Sphere* pSphereCollider = dynamic_cast<CCollider_Sphere*>(pStone->Get_Collider(CCollider::DETECTION_TYPE::BODY).front());
				if (nullptr == pSphereCollider)
					continue;

				/* Check Collision */
				{
					_float fDist = 0.f;

					if (rayLeft.Intersects(pSphereCollider->Get_Sphere(), fDist))
					{
						const _float fMaxDist = Vec4(m_tStandardDesc.vGoalPosition - Vec4(rayLeft.position)).Length();
						if (fDist < fMaxDist)
						{
							bCollision = true;
							break;
						}
					}
					
					if (rayRight.Intersects(pSphereCollider->Get_Sphere(), fDist))
					{
						const _float fMaxDist = Vec4(m_tStandardDesc.vGoalPosition - Vec4(rayRight.position)).Length();
						if (fDist < fMaxDist)
						{
							bCollision = true;
							break;
						}
					}

					if (rayCenter.Intersects(pSphereCollider->Get_Sphere(), fDist))
					{
						const _float fMaxDist = Vec4(m_tStandardDesc.vGoalPosition - Vec4(rayCenter.position)).Length();
						if (fDist < fMaxDist)
						{
							bCollision = true;
							break;
						}
					}
				}

			}

			/* None Collision */
			if(!bCollision)
			{
				m_tCurAiPath.fPower = 0.55f;
				m_tCurAiPath.vStartPoint = Vec4(rayCenter.position).OneW();
				m_tCurAiPath.vLaunchDir = Vec4(rayCenter.direction).ZeroW();

				if (AI_PATH_DESC::Equal(m_tPrevAiPath, m_tCurAiPath))
				{
					continue;
				}

				return S_OK;
			}
		}
	}

	/* ������ ����� ��η� �߻� */
	do
	{
		if (m_tAiPathQueue.empty())
			Ready_AiPathQueue();

		AI_PATH_DESC tPahtDesc = m_tAiPathQueue.front();
		m_tAiPathQueue.pop();

		memcpy(&m_tCurAiPath, &tPahtDesc, sizeof(AI_PATH_DESC));

	} while (AI_PATH_DESC::Equal(m_tPrevAiPath, m_tCurAiPath));

	return S_OK;
}

void CCurlingGame_Manager::Test(const _float& fTimeDelta)
{
	
}

void CCurlingGame_Manager::Debug()
{

}
#ifdef NDEBUG
HRESULT CCurlingGame_Manager::Ready_DebugDraw()
{
	m_pBatch = new PrimitiveBatch<VertexPositionColor>(GI->Get_Context());
	m_pEffect = new BasicEffect(GI->Get_Device());

	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCodes = nullptr;
	size_t			iLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCodes, &iLength);

	if (FAILED(GI->Get_Device()->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCodes, iLength, &m_pInputLayout)))
		return E_FAIL;

	_float	fRadius = 0.5f;
	Vec3	vOrigin = { 0.f, fRadius * 0.5f, 0.f };
	m_pSphere = new BoundingSphere(vOrigin, fRadius);

	if (nullptr == m_pSphere)
		return E_FAIL;

	return S_OK;
}
#endif

#ifdef NDEBUG
HRESULT CCurlingGame_Manager::Render_DebugDraw()
{
	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(GI->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));

	m_pEffect->Apply(m_pContext);

	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pBatch->Begin();
	{
		// ���⼭ �������̶� ������ �ٽ� ���� �ʿ�
		for (size_t i = 0; i < STANDARD_DESC::RING_TYPEEND; i++)
		{
			m_pSphere->Center = m_tStandardDesc.vGoalPosition;
			m_pSphere->Radius = m_tStandardDesc.fRingScalesForDetection[i];
			DX::Draw(m_pBatch, *m_pSphere, Colors::BlueViolet);
		}
	}
	m_pBatch->End();

	return S_OK;
}
#endif

void CCurlingGame_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

#ifdef NDEBUG
	Safe_Delete(m_pBatch);
	Safe_Delete(m_pEffect);
	Safe_Delete(m_pSphere);
	Safe_Release(m_pInputLayout);
#endif

	Safe_Release(m_pManagerStateMachineCom);
}




