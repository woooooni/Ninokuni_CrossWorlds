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

	CRenderer::TEXT_DESC desc = {};
	{
		desc.strText	= L"Power : " + to_wstring(_int(m_tGuageDesc.tLerpValue.fCurValue * 100.f));
		desc.strFontTag = L"Default_Bold";
		desc.vScale		= { 0.75f, 0.75f };
		desc.vPosition	= _float2(g_iWinSizeX * 0.75f, g_iWinSizeY * 0.75f);
		desc.vColor		= (Vec4)DirectX::Colors::Black;
	}

	CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CRenderer>(L"Com_Renderer")->Add_Text(desc);

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

void CCurlingGame_Manager::Tick_Guage(const _float& fTimeDelta)
{
	m_tGuageDesc.Tick(fTimeDelta);
	if (KEY_AWAY(KEY::LBTN))
	{
		CGameObject* pClone = nullptr;

		if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP, TEXT("Prorotype_GameObject_Shuffleboard_Barrel"), nullptr, &pClone)))
			return;

		/* Transform */
		if (nullptr != pClone)
		{
			Vec4 vPos = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CTransform>(L"Com_Transform")->Get_Position();
			Vec4 vDir = CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Transform()->Get_Look();
			Vec4 vLookAt = vPos + (vDir * 5.f);

			pClone->Get_Component<CTransform>(L"Com_Transform")->Set_State(CTransform::STATE_POSITION, vPos.OneW());
			pClone->Get_Component<CTransform>(L"Com_Transform")->LookAt_ForLandObject(vLookAt.OneW());
		}

		/* Rigidbody */
		CCurlingGame_Barrel* pBarrel = dynamic_cast<CCurlingGame_Barrel*>(pClone);
		if (nullptr != pBarrel)
		{
			const _float fPower = 40.f;
			const Vec3 vDir = Vec3(CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CTransform>(L"Com_Transform")->Get_Look()).ZeroY().Normalized();
			pBarrel->Launch(vDir, fPower * m_tGuageDesc.tLerpValue.fCurValue);
		}

		m_tGuageDesc.Stop();
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

void CCurlingGame_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}




