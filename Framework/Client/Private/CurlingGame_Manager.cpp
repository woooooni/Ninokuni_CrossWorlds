#include "stdafx.h"
#include "..\Public\CurlingGame_Manager.h"

#include "GameInstance.h"

#include "CurlingGame_Stone.h"
#include "CurlingGame_Wall.h"

#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"

#include "UI_Manager.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

#include "Effect_Manager.h"

#include "Manager_StateMachine.h"

#include "State_CurlingGame_Intro.h"
#include "State_CurlingGame_Move_Character.h"
#include "State_CurlingGame_Choose_Direction.h"
#include "State_CurlingGame_Adjust_Intensity.h"
#include "State_CurlingGame_Launch_Stone.h"

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

	if (m_bDebugRender && FAILED(Ready_DebugDraw()))
		return E_FAIL;

	return S_OK;
}

void CCurlingGame_Manager::Tick(const _float& fTimeDelta)
{
	if (m_bPlaying)
		m_pManagerStateMachineCom->Tick(fTimeDelta);

	Test(fTimeDelta);

	if (m_tGuageDesc.bActive)
		Tick_Guage(fTimeDelta);

	Tick_Score();
}

void CCurlingGame_Manager::LateTick(const _float& fTimeDelta)
{
	if (m_bPlaying)
		m_pManagerStateMachineCom->LateTick(fTimeDelta);

	if (!m_bPlaying)
		return;

	Debug();
}

void CCurlingGame_Manager::Render_Debug()
{
	if (m_bPlaying)
		m_pManagerStateMachineCom->Render();

	if(m_bDebugRender)
		Render_DebugDraw();
}

HRESULT CCurlingGame_Manager::Set_Game(const _bool& bStart)
{
	m_bPlaying = bStart;

	if (bStart)
	{
		/* Set State */
		if (FAILED(m_pManagerStateMachineCom->Change_State(INTRO)))
			return E_FAIL;
	}

	return S_OK;
}

void CCurlingGame_Manager::Tick_Guage(const _float& fTimeDelta)
{
	m_tGuageDesc.Tick(fTimeDelta);
	if (KEY_AWAY(KEY::LBTN))
	{
		CGameObject* pClone = nullptr;


		CCurlingGame_Stone::STONE_INIT_DESC tStoneInitDesc;

		tStoneInitDesc.eStoneType = CCurlingGame_Stone::STONE_TYPE::BARREL;
		//tStoneInitDesc.eStoneType = CCurlingGame_Stone::STONE_TYPE::POT;

		if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP, TEXT("Prorotype_GameObject_CurlingGame_Stone"), &tStoneInitDesc, &pClone)))
			return;

		CCurlingGame_Stone* pStone = dynamic_cast<CCurlingGame_Stone*>(pClone);
		if (nullptr != pStone)
		{			
			pStone->Launch(m_tGuageDesc.fMaxPower * m_tGuageDesc.tLerpValue.fCurValue);

			m_pBarrelsLaunched.push_back(pStone);

			m_tGuageDesc.Stop();
		}
	}
}

HRESULT CCurlingGame_Manager::Ready_Components()
{
	/* Manager StateMachine */
	{
		/* 엔진의 스테이트머신이 아니라 클라 매니저용 스테이트 머신이다. */

		m_pManagerStateMachineCom = CManager_StateMachine::Create();

		if (nullptr == m_pManagerStateMachineCom)
			return E_FAIL;
	}

	/* Manager States */
	{
		if (FAILED(m_pManagerStateMachineCom->Add_State(CURLINGGAME_STATE::INTRO, CState_CurlingGame_Intro::Create(m_pManagerStateMachineCom))))
			return E_FAIL;

		if (FAILED(m_pManagerStateMachineCom->Add_State(CURLINGGAME_STATE::MOVE, CState_CurlingGame_Intro::Create(m_pManagerStateMachineCom))))
			return E_FAIL;

		if (FAILED(m_pManagerStateMachineCom->Add_State(CURLINGGAME_STATE::DIRECTION, CState_CurlingGame_Intro::Create(m_pManagerStateMachineCom))))
			return E_FAIL;

		if (FAILED(m_pManagerStateMachineCom->Add_State(CURLINGGAME_STATE::INTENSITY, CState_CurlingGame_Intro::Create(m_pManagerStateMachineCom))))
			return E_FAIL;

		if (FAILED(m_pManagerStateMachineCom->Add_State(CURLINGGAME_STATE::LAUNCH, CState_CurlingGame_Intro::Create(m_pManagerStateMachineCom))))
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

		/* Left 기준 Wall 데이터*/
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
			vNormal = Vec3(pWallTransform->Get_Look()).ZeroY().Normalized() * -1.f;
		}

		pWall->Set_Normal(vNormal);
	}

	/* Stadium Objects  */
	{
		m_pStadiumObjects.reserve(50);
	}
	return S_OK;
}


void CCurlingGame_Manager::Tick_Score()
{
	/* 발사된 모든 스톤이 정지된 상태라면 점수를 갱신한다. */
	//for (auto& pBarrel : m_pBarrelsLaunched)
	//{
	//	if (nullptr == pBarrel)
	//		continue;
	//
	//	if (pBarrel->Is_Moving())
	//		return;
	//}

	_uint iScores[PARTICIPANT_TYPE::PARTICIPANT_TYPEEND] = { 0, 0 };

	for (auto& pBarrel : m_pBarrelsLaunched)
	{
		/* Exception */
		if (nullptr == pBarrel || pBarrel->Is_Outted())
			continue;

		/* Calculate Point */
		_int iPoint = 0;
		{
			const _float fDistance = Vec3::Distance(pBarrel->Get_Transform()->Get_Position(), m_tStandardDesc.vGoalPosition);
			
			if (fDistance <= m_tStandardDesc.fRingScalesForDetection[STANDARD_DESC::RING_TYPE::FIRST])
			{
				iPoint = m_tStandardDesc.iPoints[STANDARD_DESC::RING_TYPE::FIRST];
			}
			else if (m_tStandardDesc.fRingScalesForDetection[STANDARD_DESC::RING_TYPE::FIRST] <= fDistance
				&& fDistance < m_tStandardDesc.fRingScalesForDetection[STANDARD_DESC::RING_TYPE::SECOND])
			{
				iPoint = m_tStandardDesc.iPoints[STANDARD_DESC::RING_TYPE::SECOND];
			}
			else if (m_tStandardDesc.fRingScalesForDetection[STANDARD_DESC::RING_TYPE::SECOND] <= fDistance
				&& fDistance < m_tStandardDesc.fRingScalesForDetection[STANDARD_DESC::RING_TYPE::THIRD])
			{
				iPoint = m_tStandardDesc.iPoints[STANDARD_DESC::RING_TYPE::THIRD];
			}
		}

		/* Accumulate */
		switch (pBarrel->Get_OwnerType())
		{
		case OBJ_TYPE::OBJ_PLAYER:
		{
			iScores[PARTICIPANT_PLAYER] += iPoint;
		}
		break;
		case OBJ_TYPE::OBJ_NPC:
		{
			iScores[PARTICIPANT_NPC] += iPoint;
		}
		break;
		default:
			break;
		}
	}

	m_tParticipants[PARTICIPANT_PLAYER].iScore = iScores[PARTICIPANT_PLAYER];
	m_tParticipants[PARTICIPANT_NPC].iScore = iScores[PARTICIPANT_NPC];
}

void CCurlingGame_Manager::Test(const _float& fTimeDelta)
{
	if (m_bPlaying)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			m_tGuageDesc.Start();
		}
	}

	if (KEY_TAP(KEY::Q))
	{
		Set_Game(!m_bPlaying);
	}
}

void CCurlingGame_Manager::Debug()
{
	Vec2		vPos = { g_iWinSizeX * 0.2f, g_iWinSizeY * 0.75f };
	const Vec2	vDelta = { 0.f, 30.f };
	const Vec2	vScale(0.4f);
	const wstring wstrFont = L"Default_Bold";
	CRenderer*	pRenderer = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CRenderer>(L"Com_Renderer");

	/* Debug */
	CRenderer::TEXT_DESC desc = {};
	{
		vPos += vDelta;

		desc.strText = L"Debug Data";
		desc.strFontTag = wstrFont;
		desc.vScale = vScale * 1.5f;
		desc.vPosition = vPos;
		desc.vColor = (Vec4)DirectX::Colors::Black;
	}
	pRenderer->Add_Text(desc);


	/* Power */
	{
		vPos += vDelta;

		desc.strText = L"Power : " + to_wstring(_int(m_tGuageDesc.tLerpValue.fCurValue * 100.f));
		desc.vPosition = vPos;
		desc.vColor = (Vec4)DirectX::Colors::DarkViolet;
	}
	pRenderer->Add_Text(desc);

	/* Score */
	{
		/* Player */
		{
			vPos += vDelta;

			desc.strText = L"Score Player : " + to_wstring(m_tParticipants[PARTICIPANT_PLAYER].iScore);
			desc.vPosition = vPos;
			desc.vColor = (Vec4)DirectX::Colors::DarkBlue;
		}
		pRenderer->Add_Text(desc);

		/* Npc */
		{
			vPos += vDelta;

			desc.strText = L"Score Npc : " + to_wstring(m_tParticipants[PARTICIPANT_NPC].iScore);
			desc.vPosition = vPos;
			desc.vColor = (Vec4)DirectX::Colors::DarkGreen;
		}
		pRenderer->Add_Text(desc);
	}
}

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

HRESULT CCurlingGame_Manager::Render_DebugDraw()
{
	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(GI->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));

	m_pEffect->Apply(m_pContext);

	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pBatch->Begin();
	{
		// 여기서 포지션이랑 스케일 다시 세팅 필요
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

void CCurlingGame_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Delete(m_pBatch);
	Safe_Delete(m_pEffect);
	Safe_Delete(m_pSphere);
	Safe_Release(m_pInputLayout);

	Safe_Release(m_pManagerStateMachineCom);
}




