#include "stdafx.h"
#include "..\Public\CurlingGame_Manager.h"

#include "GameInstance.h"

#include "CurlingGame_Barrel.h"
#include "CurlingGame_Wall.h"

#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"

#include "UI_Manager.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

#include "CurlingGame_Wall.h"

#include "CurlingGame_Barrel.h"
#include "CurlingGame_Wall.h"

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

	if (FAILED(Ready_Objects()))
		return E_FAIL;

	return S_OK;
}

void CCurlingGame_Manager::Tick(const _float& fTimeDelta)
{
	Test(fTimeDelta);

	if (m_tGuageDesc.bActive)
		Tick_Guage(fTimeDelta);

	if (m_tStadiumDesc.bActive)
		Tick_StadiumAction(fTimeDelta);
}

void CCurlingGame_Manager::LateTick(const _float& fTimeDelta)
{
	if (!m_bPlaying)
		return;

	Debug();
}

HRESULT CCurlingGame_Manager::Set_Game(const _bool& bStart)
{
	if (bStart)
	{
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_CurCamera());
		if (nullptr != pFollowCam)
		{
			pFollowCam->Set_CanWideView(false);
			pFollowCam->Set_ViewType(CAMERA_VIEW_TYPE::BACK);
		}

		for (size_t i = 0; i < 5; i++)
		{
			CGameObject* pClone = nullptr;

			if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP, TEXT("Prorotype_GameObject_Shuffleboard_Barrel"), nullptr, &pClone)))
				return E_FAIL;

			/* Transform */
			if (nullptr != pClone)
			{
				CTransform* pPlayerTransform = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CTransform>(L"Com_Transform");
				if (nullptr == pPlayerTransform)
					return E_FAIL;
				Vec4 vPos = pPlayerTransform->Get_Position();
				Vec4 vDir = CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Transform()->Get_Look();
				Vec4 vLookAt = vPos + (vDir * 5.f);

				vPos += pPlayerTransform->Get_RelativeOffset(Vec4(-2.f, 0.f, i * 1.5f, 1.f));

				pClone->Get_Component<CTransform>(L"Com_Transform")->Set_State(CTransform::STATE_POSITION, vPos.OneW());
				pClone->Get_Component<CTransform>(L"Com_Transform")->LookAt_ForLandObject(vLookAt.OneW());

				dynamic_cast<CCurlingGame_Barrel*>(pClone)->Set_Active(true);
			}
		}
	}
	else
	{
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_CurCamera());
		if (nullptr != pFollowCam)
		{
			pFollowCam->Set_CanWideView(true);
			pFollowCam->Set_ViewType(CAMERA_VIEW_TYPE::SHOLDER);
		}
	}

	m_bPlaying = bStart;

	/* On Off Set */
	{
		const _bool bInput = !bStart;

		/* Input */
		CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_Attack_Input(bInput);
		CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_Skill_Input(bInput);

		/* UI */
		if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
		{
			CUI_Manager::GetInstance()->OnOff_GamePlaySetting(bInput);
			//CUI_Manager::GetInstance()->Hide_MouseCursor(bInput);
		}
	}

	return S_OK;
}

HRESULT CCurlingGame_Manager::Start_StadiumAction()
{
	m_tStadiumDesc.bActive = true;

	return S_OK;
}
 
HRESULT CCurlingGame_Manager::Finish_StaduimAction()
{
	m_tStadiumDesc.bActive = false;

	return S_OK;
}

CCurlingGame_Barrel* CCurlingGame_Manager::Pop_Barrel()
{
	if(m_pBarrelPool.empty())
		return nullptr;

	CCurlingGame_Barrel* pBarrel = m_pBarrelPool.front();
	m_pBarrelPool.pop();

	pBarrel->Set_Active(true);

	return pBarrel;
}

HRESULT CCurlingGame_Manager::Push_Barrel(CCurlingGame_Barrel* pBarrel)
{
	if(nullptr == pBarrel)
		return E_FAIL;

	pBarrel->Set_Active(false);
	
	m_pBarrelPool.push(pBarrel);
	return S_OK;
}

void CCurlingGame_Manager::Tick_Guage(const _float& fTimeDelta)
{
	m_tGuageDesc.Tick(fTimeDelta);
	if (KEY_AWAY(KEY::LBTN))
	{
		//CCurlingGame_Barrel* pBarrel = Pop_Barrel();
		CGameObject* pClone = nullptr;

		if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP, TEXT("Prorotype_GameObject_Shuffleboard_Barrel"), nullptr, &pClone)))
			return;

		CCurlingGame_Barrel* pBarrel = dynamic_cast<CCurlingGame_Barrel*>(pClone);
		if (nullptr != pBarrel)
		{			
			/* Transform */
			{
				Vec4 vPos = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CTransform>(L"Com_Transform")->Get_Position();
				Vec4 vDir = CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Transform()->Get_Look();
				Vec4 vLookAt = vPos + (vDir * 5.f);

				pBarrel->Get_Transform()->Set_State(CTransform::STATE_POSITION, vPos.OneW());
				pBarrel->Get_Transform()->LookAt_ForLandObject(vLookAt.OneW());
			}

			/* Rigidbody */
			if (nullptr != pBarrel)
			{
				const _float fPower = m_tGuageDesc.fMaxPower;
				const Vec3 vDir = Vec3(CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CTransform>(L"Com_Transform")->Get_Look()).ZeroY().Normalized();
				pBarrel->Launch(vDir, fPower * m_tGuageDesc.tLerpValue.fCurValue);
			}

			pBarrel->Set_Active(true);

			m_pBarrelsLaunched.push_back(pBarrel);

			m_tGuageDesc.Stop();
		}
	}
}

void CCurlingGame_Manager::Tick_StadiumAction(const _float& fTimeDelta)
{
}

HRESULT CCurlingGame_Manager::Ready_Objects()
{
	/* Prototype */
	{
		/* Barrel */
		{
			if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Prop_Barrel", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/CurlingGame/Barrel/", L"Prop_Barrel")))
				return E_FAIL;

			if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Shuffleboard_Barrel",
				CCurlingGame_Barrel::Create(m_pDevice, m_pContext, TEXT("Shuffleboard_Barrel")), LAYER_TYPE::LAYER_PROP)))
				return E_FAIL;
		}

		/* Wall */
		{
			if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Prop_Wall", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/CurlingGame/Wall/", L"Lobby_Fence_01a")))
				return E_FAIL;

			if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Shuffleboard_Wall",
				CCurlingGame_Wall::Create(m_pDevice, m_pContext, TEXT("Shuffleboard_Wall")), LAYER_TYPE::LAYER_PROP)))
				return E_FAIL;
		}
	}

	/* Pool */
	{
		for (_uint i = 0; i < m_iMaxBarrels; i++)
		{
			CGameObject* pClone = nullptr;

			if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP, TEXT("Prorotype_GameObject_Shuffleboard_Barrel"), nullptr, &pClone)))
				return E_FAIL;

			CCurlingGame_Barrel* pBarrel = dynamic_cast<CCurlingGame_Barrel*>(pClone);
			if (nullptr == pBarrel)
				return E_FAIL;

			m_pBarrelPool.push(pBarrel);
		}
	}

	/* Stadium Objects (ÁØ¿±) */
	{
		m_tStadiumDesc.pStadiumObjects;
	}

	return S_OK;
}

void CCurlingGame_Manager::Test(const _float& fTimeDelta)
{
	if (m_bPlaying)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			m_tGuageDesc.Start();
		}

		if (KEY_TAP(KEY::RBTN))
		{
			/* Create Wall */
			//{
			//	CGameObject* pClone = nullptr;
			//
			//	if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP, TEXT("Prorotype_GameObject_Shuffleboard_Wall"), nullptr, &pClone)))
			//		return;
			//
			//	if (nullptr != pClone)
			//	{
			//		Vec4 vPos = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CTransform>(L"Com_Transform")->Get_Position();
			//		Vec4 vLook = Vec4(CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Transform()->Get_Look()).ZeroY().Normalized();
			//
			//		vPos = vPos + (vLook * 5.f);
			//		vPos.y -= pClone->Get_Component<CTransform>(L"Com_Transform")->Get_Scale().y;
			//
			//		Vec4 vLookAt = vPos + (vLook * 10.f);
			//
			//		pClone->Get_Component<CTransform>(L"Com_Transform")->Set_State(CTransform::STATE_POSITION, vPos.OneW());
			//		pClone->Get_Component<CTransform>(L"Com_Transform")->LookAt_ForLandObject(vLookAt.OneW());
			//
			//		CCurlingGame_Wall* pWall = dynamic_cast<CCurlingGame_Wall*>(pClone);
			//		if (nullptr != pWall)
			//		{
			//			Vec3 vNormal = Vec3(pClone->Get_Component<CTransform>(L"Com_Transform")->Get_Look()).ZeroY().Normalized();
			// 
			//			vNormal *= -1.f;
			//			pWall->Set_Normal(vNormal);
			//		}
			//	}
			//}
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

			desc.strText = L"Score NPC : " + to_wstring(10);
			desc.vPosition = vPos;
			desc.vColor = (Vec4)DirectX::Colors::DarkBlue;
		}
		pRenderer->Add_Text(desc);

		/* Npc */
		{
			vPos += vDelta;

			desc.strText = L"Score Player : " + to_wstring(10);
			desc.vPosition = vPos;
			desc.vColor = (Vec4)DirectX::Colors::DarkGreen;
		}
		pRenderer->Add_Text(desc);
	}
}

void CCurlingGame_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}




