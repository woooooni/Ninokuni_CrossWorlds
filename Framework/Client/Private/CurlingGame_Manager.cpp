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
}

void CCurlingGame_Manager::LateTick(const _float& fTimeDelta)
{

}

HRESULT CCurlingGame_Manager::Set_Game(const _bool& bStart)
{
	if (bStart)
	{
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_CurCamera());
		if (nullptr != pFollowCam)
		{
			pFollowCam->Set_CanWideView(false);
			pFollowCam->Start_Lerp_Distance(Cam_Dist_Curling_Default, 0.5f, LERP_MODE::SMOOTHER_STEP);
		}
	}
	else
	{
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_CurCamera());
		if (nullptr != pFollowCam)
		{
			pFollowCam->Set_CanWideView(true);
			pFollowCam->Start_Lerp_Distance(Cam_Dist_Follow_Default, 0.5f, LERP_MODE::SMOOTHER_STEP);
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
			if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Prop_Wall", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/CurlingGame/Wall/", L"Common_WoodBox")))
				return E_FAIL;

			if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Shuffleboard_Wall",
				CCurlingGame_Wall::Create(m_pDevice, m_pContext, TEXT("Shuffleboard_Wall")), LAYER_TYPE::LAYER_PROP)))
				return E_FAIL;
		}
	}

	return S_OK;
}

void CCurlingGame_Manager::Test(const _float& fTimeDelta)
{
	if (m_bPlaying)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			CGameObject* pClone = nullptr;

			if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP, TEXT("Prorotype_GameObject_Shuffleboard_Barrel"), nullptr, &pClone)))
				return;

			if (nullptr != pClone)
			{
				Vec4 vPos = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CTransform>(L"Com_Transform")->Get_Position();
				Vec4 vLook = CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Transform()->Get_Look();
				Vec4 vLookAt = vPos + (vLook * 5.f);

				pClone->Get_Component<CTransform>(L"Com_Transform")->Set_State(CTransform::STATE_POSITION, vPos.OneW());
				pClone->Get_Component<CTransform>(L"Com_Transform")->LookAt_ForLandObject(vLookAt.OneW());
			}
		}

		if (KEY_TAP(KEY::RBTN))
		{
			CGameObject* pClone = nullptr;

			if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP, TEXT("Prorotype_GameObject_Shuffleboard_Wall"), nullptr, &pClone)))
				return;

			if (nullptr != pClone)
			{
				Vec4 vPos = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CTransform>(L"Com_Transform")->Get_Position();
				Vec4 vLook = Vec4(CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Transform()->Get_Look()).ZeroY().Normalized();
				
				vPos = vPos + (vLook * 5.f);

				Vec4 vLookAt = vPos + (vLook * 10.f);

				pClone->Get_Component<CTransform>(L"Com_Transform")->Set_State(CTransform::STATE_POSITION, vPos.OneW());
				pClone->Get_Component<CTransform>(L"Com_Transform")->LookAt_ForLandObject(vLookAt.OneW());

				CCurlingGame_Wall* pWall = dynamic_cast<CCurlingGame_Wall*>(pClone);
				if (nullptr != pWall)
				{
					Vec3 vNormal = Vec3(pClone->Get_Component<CTransform>(L"Com_Transform")->Get_Right()).ZeroY().Normalized();

					pWall->Set_Normal(vNormal);
				}
			}
		}
	}
	else
	{
		if (KEY_TAP(KEY::Q))
		{
			Set_Game(!m_bPlaying);
		}
	}
}


void CCurlingGame_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}




