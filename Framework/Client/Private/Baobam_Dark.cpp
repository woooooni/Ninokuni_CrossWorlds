#include "stdafx.h"
#include "Baobam_Dark.h"

#include "GameInstance.h"

#include "Baobam_DarkBT.h"
#include "UI_MonsterHP_World.h"
#include "UIDamage_Manager.h"
#include "UI_Minimap_Icon.h"
#include "UIMinimap_Manager.h"

CBaobam_Dark::CBaobam_Dark(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
	: CMonster(pDevice, pContext, strObjectTag, tStat)
{
}

CBaobam_Dark::CBaobam_Dark(const CBaobam_Dark& rhs)
	: CMonster(rhs)
{
}

HRESULT CBaobam_Dark::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBaobam_Dark::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	OBJECT_INIT_DESC tInfo = *(OBJECT_INIT_DESC*)pArg;

	if (pArg != nullptr)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, tInfo.vStartPosition);
		m_pControllerCom->Set_EnterLevel_Position(m_pTransformCom->Get_Position());
		m_vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	}

	if (FAILED(__super::Ready_RoamingPoint()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	CGameObject* pHPBar = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Monster_WorldHPBar"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pHPBar)
		return E_FAIL;

	m_pHPBar = dynamic_cast<CUI_MonsterHP_World*>(pHPBar);
	m_pHPBar->Set_Owner(this, m_tStat.eElementType);

	m_pModelCom->Set_Animation(0);

	//m_pMinimapIcon
	CGameObject* pIcon = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Minimap_Icon"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pIcon)
		return E_FAIL;
	if (nullptr == dynamic_cast<CUI_Minimap_Icon*>(pIcon))
		return E_FAIL;
	m_pMinimapIcon = dynamic_cast<CUI_Minimap_Icon*>(pIcon);
	m_pMinimapIcon->Set_Owner(this);

	return S_OK;
}

void CBaobam_Dark::Tick(_float fTimeDelta)
{
	if (KEY_TAP(KEY::F7))
	{
		m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATK] = !m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATK];
		m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = !m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND];
	}
	if (KEY_TAP(KEY::T) || KEY_TAP(KEY::Y))
	{
		m_tStat.fHp -= 10.f;
	}

	// << : Test 
	if (KEY_TAP(KEY::HOME))
	{
		_uint iCurAnimIndex = m_pModelCom->Get_CurrAnimationIndex();
		m_pModelCom->Set_Animation(iCurAnimIndex + 1);
	}
	else if (KEY_TAP(KEY::DEL))
	{
		_int iCurAnimIndex = m_pModelCom->Get_CurrAnimationIndex() - 1;
		if (iCurAnimIndex < 0)
			iCurAnimIndex = 0;
		m_pModelCom->Set_Animation(iCurAnimIndex);
	}
	// >> 

	if (nullptr != m_pHPBar)
		m_pHPBar->Tick(fTimeDelta);

	if (nullptr != m_pMinimapIcon)
	{
		if (true == CUIMinimap_Manager::GetInstance()->Is_InMinimap(m_pTransformCom))
			m_pMinimapIcon->Set_Active(true);
		else
			m_pMinimapIcon->Set_Active(false);
	}

	if (nullptr != m_pMinimapIcon)
		m_pMinimapIcon->Tick(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CBaobam_Dark::LateTick(_float fTimeDelta)
{
	if (nullptr != m_pHPBar)
		m_pHPBar->LateTick(fTimeDelta);

	if (nullptr != m_pMinimapIcon)
		m_pMinimapIcon->LateTick(fTimeDelta);

	__super::LateTick(fTimeDelta);
}

HRESULT CBaobam_Dark::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CBaobam_Dark::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	return S_OK;
}

void CBaobam_Dark::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CBaobam_Dark::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CBaobam_Dark::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

HRESULT CBaobam_Dark::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-4.f, 1.f, 4.f, 1.f));
	m_vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	/* ·Î¹Ö °æ·Î(ÀÓ½Ã) */
	m_vecRoamingArea.push_back(XMVectorSet(-8.f, 1.f, 0.f, 1.f));
	m_vecRoamingArea.push_back(XMVectorSet(-6.f, 1.f, 6.f, 1.f));
	m_vecRoamingArea.push_back(m_vOriginPos);

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For. Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Baobam_Dark"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_PhysXBody */
	CPhysX_Controller::CONTROLLER_DESC ControllerDesc;

	ControllerDesc.eType = CPhysX_Controller::CAPSULE;
	ControllerDesc.pTransform = m_pTransformCom;
	ControllerDesc.vOffset = { 0.f, 1.125f, 0.f };
	ControllerDesc.fHeight = 1.f;
	ControllerDesc.fMaxJumpHeight = 10.f;
	ControllerDesc.fRaidus = 1.f;
	ControllerDesc.pOwner = this;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_PhysXController"), TEXT("Com_Controller"), (CComponent**)&m_pControllerCom, &ControllerDesc)))
		return E_FAIL;

	CRigidBody::RIGID_BODY_DESC RigidDesc;
	RigidDesc.pTransform = m_pTransformCom;

	/* For. Com_RigidBody*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBodyCom, &RigidDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBaobam_Dark::Ready_States()
{
	m_pBTCom = CBaobam_DarkBT::Create(m_pDevice, m_pContext, this);

	strKorName = TEXT("¹ã¹Ù¿À¹ã");
	m_tStat.iLv = 10;
	m_tStat.fMaxHp = 300;
	m_tStat.fHp = 300;
	m_tStat.iAtk = 15;
	m_tStat.iDef = 3;

	return S_OK;
}

HRESULT CBaobam_Dark::Ready_Colliders()
{
	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof OBBDesc);

	BoundingOrientedBox OBBBox;
	ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));

	XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
	OBBBox.Extents = { 50.f, 50.f, 50.f };

	OBBDesc.tBox = OBBBox;
	OBBDesc.pNode = nullptr;
	OBBDesc.pOwnerTransform = m_pTransformCom;
	OBBDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	OBBDesc.vOffsetPosition = Vec3(0.f, 50.f, 0.f);

	/* Body */
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::BODY, &OBBDesc)))
		return E_FAIL;

	/* Atk */
	OBBDesc.vOffsetPosition = Vec3(0.f, 50.f, -100.f);
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::ATTACK, &OBBDesc)))
		return E_FAIL;

	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 2.f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	SphereDesc.vOffsetPosition = Vec3(0.f, 50.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BOUNDARY, &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

CBaobam_Dark* CBaobam_Dark::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
{
	CBaobam_Dark* pInstance = new CBaobam_Dark(pDevice, pContext, strObjectTag, tStat);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Craete : CBaobam_Dark");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBaobam_Dark::Clone(void* pArg)
{
	CBaobam_Dark* pInstance = new CBaobam_Dark(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CBaobam_Dark");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaobam_Dark::Free()
{
	__super::Free();

	Safe_Release(m_pMinimapIcon);
	Safe_Release(m_pHPBar);
}
