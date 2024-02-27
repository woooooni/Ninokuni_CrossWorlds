#include "stdafx.h"
#include "..\Public\Witch_Barricade.h"
#include "GameInstance.h"
#include "Quest_Manager.h"

CWitch_Barricade::CWitch_Barricade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& eStat)
	: CMonster(pDevice, pContext, strObjectTag, eStat)
{
}

CWitch_Barricade::CWitch_Barricade(const CWitch_Barricade& rhs)
	: CMonster(rhs)
{
}

HRESULT CWitch_Barricade::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWitch_Barricade::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	return S_OK;
}

void CWitch_Barricade::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;


	if (m_tStat.fHp <= 0.0f)
	{
		Set_Dead(true);	
		return;
	}

	if (m_pControllerCom != nullptr)
		m_pControllerCom->Tick_Controller(fTimeDelta);

	GI->Add_CollisionGroup(COLLISION_GROUP::MONSTER, this);
}

void CWitch_Barricade::LateTick(_float fTimeDelta)
{
	LateUpdate_Collider(fTimeDelta);

	if (m_pControllerCom != nullptr)
		m_pControllerCom->LateTick_Controller(fTimeDelta);


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONBLEND, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_SHADOW, this);

#ifdef NDEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

HRESULT CWitch_Barricade::Render()
{
	if (true == m_bDead)
		return S_OK;

	Matrix WorldMatrix = m_pTransformCom->Get_WorldFloat4x4();

	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &WorldMatrix.Transpose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		_uint		iPassIndex = 0;

		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		//if (FAILED(m_pModelCom->SetUp_OnShader(pShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
		//	iPassIndex = 0;
		//else
		//	iPassIndex++;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
			return E_FAIL;
	}

	return S_OK;
}

void CWitch_Barricade::Collision_Enter(const COLLISION_INFO& tInfo)
{
	if (m_tStat.fHp > 0.f)
	{
		if ((tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER_PROJECTILE) &&
			tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::ATTACK)
		{
			if (tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
				On_Damaged(tInfo);
		}
	}
}

void CWitch_Barricade::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CWitch_Barricade::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

HRESULT CWitch_Barricade::Ready_Components(void* pArg)
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;
	OBJECT_INIT_DESC Init_Data = {};

	if (pArg != nullptr)
		Init_Data.vStartPosition = static_cast<OBJECT_INIT_DESC*>(pArg)->vStartPosition;


	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom),&Init_Data)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, Init_Data.vStartPosition);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Witch_Barricade"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

		/* For.Com_PhysXBody */
	CPhysX_Controller::CONTROLLER_DESC ControllerDesc;
	ControllerDesc.eType = CPhysX_Controller::BOX;
	ControllerDesc.pTransform = m_pTransformCom;
	ControllerDesc.vOffset = { 0.f, 0.0f, 0.f };
	ControllerDesc.vExtents = { 2.f, 3.f, 10.f };
	//ControllerDesc.fHeight = 1.f;
	//ControllerDesc.fMaxJumpHeight = 10.f;
	//ControllerDesc.fRaidus = 5.f;
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

HRESULT CWitch_Barricade::Ready_Colliders()
{
	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof OBBDesc);

	BoundingOrientedBox OBBBox;
	ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));

	XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
	OBBBox.Extents = { 400.0f, 190.f, 100.0f };

	OBBDesc.tBox = OBBBox;
	OBBDesc.pNode = nullptr;
	OBBDesc.pOwnerTransform = m_pTransformCom;
	OBBDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	OBBDesc.vOffsetPosition = Vec3(20.f, 190.0f, 0.f);

	/* Body */
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::BODY, &OBBDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWitch_Barricade::Ready_States()
{
	strKorName = TEXT("¹Ù¸®°ÔÀÌÆ®");
	strSubName = TEXT("¸¶³àÀÇ ½£");
	m_tStat.eElementType = ELEMENTAL_TYPE::WOOD;

	m_tStat.iLv = 1;
	m_tStat.fMaxHp = 40000.0f;
	m_tStat.fHp = 40000.0f;


	return S_OK;
}

CWitch_Barricade* CWitch_Barricade::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& eStat)
{
	CWitch_Barricade* pInstance = new CWitch_Barricade(pDevice, pContext, strObjectTag, eStat);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWitch_Barricade");
		Safe_Release<CWitch_Barricade*>(pInstance);
	}

	return pInstance;
}

CGameObject* CWitch_Barricade::Clone(void* pArg)
{
	CWitch_Barricade* pInstance = new CWitch_Barricade(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWitch_Barricade");
		Safe_Release<CWitch_Barricade*>(pInstance);
	}

	return pInstance;
}

void CWitch_Barricade::Free()
{
	__super::Free();

}
