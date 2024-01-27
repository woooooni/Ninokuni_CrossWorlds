#include "stdafx.h"
#include "Vfx_Engineer_Skill_Destruction.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"
#include "Character_Projectile.h"

CVfx_Engineer_Skill_Destruction::CVfx_Engineer_Skill_Destruction(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Engineer_Skill_Destruction::CVfx_Engineer_Skill_Destruction(const CVfx_Engineer_Skill_Destruction& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Engineer_Skill_Destruction::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::SKILL_BURST;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 범위 표시
	{
		m_pFrameTriger[TYPE_ET1_D_RECT] = 2;
		m_pPositionOffset[TYPE_ET1_D_RECT] = _float3(0.f, 0.f, 0.5f);
		m_pScaleOffset[TYPE_ET1_D_RECT]    = _float3(7.5f, 5.f, 14.f);
		m_pRotationOffset[TYPE_ET1_D_RECT] = _float3(0.f, 0.f, 0.f);
	}

	// 대포 생성
	{
		m_pFrameTriger[TYPE_ET2_O_CANNON] = 20;
		m_pPositionOffset[TYPE_ET2_O_CANNON] = _float3(1.8f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_O_CANNON]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_O_CANNON] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_METAL] = 20;
		m_pPositionOffset[TYPE_ET2_E_METAL] = _float3(1.8f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_METAL]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_E_METAL] = _float3(0.f, 0.f, 0.f);

		// 
		m_pFrameTriger[TYPE_ET2_P_SMOKE] = 20;
		m_pPositionOffset[TYPE_ET2_P_SMOKE] = _float3(1.8f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_SMOKE] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Engineer_Skill_Destruction::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Engineer_Skill_Destruction::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		// 범위 표시
		if (m_iCount == TYPE_ET1_D_RECT && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_D_RECT])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_FrozenStorm_Square"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_D_RECT], m_pScaleOffset[TYPE_ET1_D_RECT], m_pRotationOffset[TYPE_ET1_D_RECT]);
			m_iCount++;
		}

		// 대포 생성
		else if (m_iCount == TYPE_ET2_O_CANNON && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_O_CANNON])
		{
			Create_Cannon();
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_E_METAL && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_METAL])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_SMOKE])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Engineer_Skill_Destruction::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Engineer_Skill_Destruction::Render()
{
	return S_OK;
}

HRESULT CVfx_Engineer_Skill_Destruction::Ready_Components()
{
	return S_OK;
}

void CVfx_Engineer_Skill_Destruction::Create_Cannon()
{
	CCharacter* pPlayer = static_cast<CCharacter*>(m_pOwnerObject);
	if (nullptr == pPlayer)
		return;

	CCharacter_Projectile::CHARACTER_PROJECTILE_DESC ProjectileDesc = {};
	ProjectileDesc.pOwner = pPlayer;

	for (_int i = -1; i <= 1; i += 2)
	{
		CGameObject* pObject = nullptr;
		GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_CHARACTER, L"Prototype_GameObject_Engineer_Burst_DestructionCannon", &ProjectileDesc, &pObject);
		if (nullptr == pObject)
			return;

		CTransform* pTransform = pObject->Get_Component<CTransform>(L"Com_Transform");
		if (nullptr == pTransform)
			return;

		_matrix InitMatirx = XMLoadFloat4x4(&m_WorldMatrix);
		InitMatirx.r[CTransform::STATE_POSITION] += i * m_pPositionOffset[TYPE_ET2_O_CANNON].x * XMVector3Normalize(InitMatirx.r[CTransform::STATE_RIGHT]);
		pTransform->Set_WorldMatrix(InitMatirx);
	}
}

CVfx_Engineer_Skill_Destruction* CVfx_Engineer_Skill_Destruction::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Engineer_Skill_Destruction* pInstance = new CVfx_Engineer_Skill_Destruction(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Engineer_Skill_Destruction");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Engineer_Skill_Destruction::Clone(void* pArg)
{
	CVfx_Engineer_Skill_Destruction* pInstance = new CVfx_Engineer_Skill_Destruction(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Engineer_Skill_Destruction");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Engineer_Skill_Destruction::Free()
{
	__super::Free();

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}