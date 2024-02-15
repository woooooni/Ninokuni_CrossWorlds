#include "stdafx.h"
#include "Vfx_Witch_Attack.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "DreamMazeWitch_Npc.h"

#include "Effect.h"

CVfx_Witch_Attack::CVfx_Witch_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Witch_Attack::CVfx_Witch_Attack(const CVfx_Witch_Attack& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Witch_Attack::Initialize_Prototype()
{
	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 0
	m_pFrameTriger[TYPE_E_ATTACK_HANDREADY] = 0;
	m_pPositionOffset[TYPE_E_ATTACK_HANDREADY] = _float3(0.4f, .4f, 0.5f);
	m_pScaleOffset[TYPE_E_ATTACK_HANDREADY] = _float3(0.1f, 0.1f, 0.1f);
	m_pRotationOffset[TYPE_E_ATTACK_HANDREADY] = _float3(0.f, 0.f, 0.f);

	// 1
	m_pFrameTriger[TYPE_E_ATTACK_BODYREADY] = 1;
	m_pPositionOffset[TYPE_E_ATTACK_BODYREADY] = _float3(0.f, .4f, 0.f);
	m_pScaleOffset[TYPE_E_ATTACK_BODYREADY] = _float3(0.1f, 0.1f, 0.1f);
	m_pRotationOffset[TYPE_E_ATTACK_BODYREADY] = _float3(0.f, 0.f, 0.f);

	// 2
	m_pFrameTriger[TYPE_E_ATTACK_HANDDELETE] = 70;
	m_pPositionOffset[TYPE_E_ATTACK_HANDDELETE] = _float3(0.4f, .4f, 0.5f);
	m_pScaleOffset[TYPE_E_ATTACK_HANDDELETE] = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_E_ATTACK_HANDDELETE] = _float3(0.f, 0.f, 0.f);

	// 3
	m_pFrameTriger[TYPE_E_ATTACK_BODYDELETE] = 85;
	m_pPositionOffset[TYPE_E_ATTACK_BODYDELETE] = _float3(0.f, .4f, 0.f);
	m_pScaleOffset[TYPE_E_ATTACK_BODYDELETE] = _float3(3.f, 3.f, 3.f);
	m_pRotationOffset[TYPE_E_ATTACK_BODYDELETE] = _float3(0.f, 0.f, 0.f);

	return S_OK;
}

HRESULT CVfx_Witch_Attack::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Witch_Attack::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_E_ATTACK_HANDREADY && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_ATTACK_HANDREADY])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_Attack_Ready_Hand"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_ATTACK_HANDREADY], m_pScaleOffset[TYPE_E_ATTACK_HANDREADY], m_pRotationOffset[TYPE_E_ATTACK_HANDREADY], nullptr, &m_pHandCreate);
			m_iCount++;
		}

		else if (m_iCount == TYPE_E_ATTACK_BODYREADY && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_ATTACK_BODYREADY])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_Attack_Ready_Body"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_ATTACK_BODYREADY], m_pScaleOffset[TYPE_E_ATTACK_BODYREADY], m_pRotationOffset[TYPE_E_ATTACK_BODYREADY], nullptr, &m_pBodyCreate);
			m_iCount++;
		}

		else if (m_iCount == TYPE_E_ATTACK_HANDDELETE && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_ATTACK_HANDDELETE])
		{
			m_pHandCreate->Set_Dead(true);
			m_pHandCreate = nullptr;

			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_Attack_Delete_Hand"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_ATTACK_HANDDELETE], m_pScaleOffset[TYPE_E_ATTACK_HANDDELETE], m_pRotationOffset[TYPE_E_ATTACK_HANDDELETE], nullptr, &m_pHandDelete);
			m_iCount++;
		}

		else if (m_iCount == TYPE_E_ATTACK_BODYDELETE && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_ATTACK_BODYDELETE])
		{
			m_pBodyCreate->Set_Dead(true);
			m_pBodyCreate = nullptr;

			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_Attack_Delete_Body"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_ATTACK_BODYDELETE], m_pScaleOffset[TYPE_E_ATTACK_BODYDELETE], m_pRotationOffset[TYPE_E_ATTACK_BODYDELETE], nullptr, &m_pBodyDelete);
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;

		// HandCreate
		if (nullptr != m_pHandCreate)
		{
			CTransform* pOwnerTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");
			CModel* pOwnerModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
			if (pOwnerTransform != nullptr && pOwnerModel != nullptr)
			{
				CTransform* pEffectTransform = m_pHandCreate->Get_Component<CTransform>(L"Com_Transform");
				if (pEffectTransform != nullptr)
				{
					Matrix EffectMatrix = pOwnerModel->Get_SocketLocalMatrix(1);
					Matrix OwnerWorldMatrix = pOwnerTransform->Get_WorldMatrix();

					// Scale / Rotation
					Matrix matScale = matScale.CreateScale(pEffectTransform->Get_Scale());//m_pScaleOffset[TYPE_E_CIRCLE_LHAND_06]);
					Matrix matRotation = matScale.CreateFromYawPitchRoll(Vec3(XMConvertToRadians(m_pRotationOffset[TYPE_E_ATTACK_HANDREADY].x), XMConvertToRadians(m_pRotationOffset[TYPE_E_ATTACK_HANDREADY].y), XMConvertToRadians(m_pRotationOffset[TYPE_E_ATTACK_HANDREADY].z)));
					Matrix matResult = matScale * matRotation * (EffectMatrix * OwnerWorldMatrix);
					pEffectTransform->Set_WorldMatrix(matResult);

					// Position
					_vector vCurrentPosition = pEffectTransform->Get_Position();
					_vector vFinalPosition = vCurrentPosition;
					vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_RIGHT) * m_pPositionOffset[TYPE_E_ATTACK_HANDREADY].x;
					vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_UP) * m_pPositionOffset[TYPE_E_ATTACK_HANDREADY].y;
					vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_LOOK) * m_pPositionOffset[TYPE_E_ATTACK_HANDREADY].z;
					pEffectTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(XMVectorGetX(vFinalPosition), XMVectorGetY(vFinalPosition), XMVectorGetZ(vFinalPosition), 1.f));
				}
			}
		}

		// BodyCreate
		if (nullptr != m_pBodyCreate)
		{
			CTransform* pOwnerTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");
			CModel* pOwnerModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
			if (pOwnerTransform != nullptr && pOwnerModel != nullptr)
			{
				CTransform* pEffectTransform = m_pBodyCreate->Get_Component<CTransform>(L"Com_Transform");
				if (pEffectTransform != nullptr)
				{
					Matrix EffectMatrix = pOwnerModel->Get_SocketLocalMatrix(1);
					Matrix OwnerWorldMatrix = pOwnerTransform->Get_WorldMatrix();

					// Scale / Rotation
					Matrix matScale = matScale.CreateScale(pEffectTransform->Get_Scale());//m_pScaleOffset[TYPE_E_CIRCLE_LHAND_06]);
					Matrix matRotation = matScale.CreateFromYawPitchRoll(Vec3(XMConvertToRadians(m_pRotationOffset[TYPE_E_ATTACK_BODYREADY].x), XMConvertToRadians(m_pRotationOffset[TYPE_E_ATTACK_BODYREADY].y), XMConvertToRadians(m_pRotationOffset[TYPE_E_ATTACK_BODYREADY].z)));
					Matrix matResult = matScale * matRotation * (EffectMatrix * OwnerWorldMatrix);
					pEffectTransform->Set_WorldMatrix(matResult);

					// Position
					_vector vCurrentPosition = pEffectTransform->Get_Position();
					_vector vFinalPosition = vCurrentPosition;
					vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_RIGHT) * m_pPositionOffset[TYPE_E_ATTACK_BODYREADY].x;
					vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_UP) * m_pPositionOffset[TYPE_E_ATTACK_BODYREADY].y;
					vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_LOOK) * m_pPositionOffset[TYPE_E_ATTACK_BODYREADY].z;
					pEffectTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(XMVectorGetX(vFinalPosition), XMVectorGetY(vFinalPosition), XMVectorGetZ(vFinalPosition), 1.f));
				}
			}
		}

		// HandDelete
		if (nullptr != m_pHandDelete)
		{
			CTransform* pOwnerTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");
			CModel* pOwnerModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
			if (pOwnerTransform != nullptr && pOwnerModel != nullptr)
			{
				CTransform* pEffectTransform = m_pHandDelete->Get_Component<CTransform>(L"Com_Transform");
				if (pEffectTransform != nullptr)
				{
					Matrix EffectMatrix = pOwnerModel->Get_SocketLocalMatrix(1);
					Matrix OwnerWorldMatrix = pOwnerTransform->Get_WorldMatrix();

					// Scale / Rotation
					Matrix matScale = matScale.CreateScale(pEffectTransform->Get_Scale());//m_pScaleOffset[TYPE_E_CIRCLE_LHAND_06]);
					Matrix matRotation = matScale.CreateFromYawPitchRoll(Vec3(XMConvertToRadians(m_pRotationOffset[TYPE_E_ATTACK_HANDDELETE].x), XMConvertToRadians(m_pRotationOffset[TYPE_E_ATTACK_HANDDELETE].y), XMConvertToRadians(m_pRotationOffset[TYPE_E_ATTACK_HANDDELETE].z)));
					Matrix matResult = matScale * matRotation * (EffectMatrix * OwnerWorldMatrix);
					pEffectTransform->Set_WorldMatrix(matResult);

					// Position
					_vector vCurrentPosition = pEffectTransform->Get_Position();
					_vector vFinalPosition = vCurrentPosition;
					vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_RIGHT) * m_pPositionOffset[TYPE_E_ATTACK_HANDDELETE].x;
					vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_UP) * m_pPositionOffset[TYPE_E_ATTACK_HANDDELETE].y;
					vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_LOOK) * m_pPositionOffset[TYPE_E_ATTACK_HANDDELETE].z;
					pEffectTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(XMVectorGetX(vFinalPosition), XMVectorGetY(vFinalPosition), XMVectorGetZ(vFinalPosition), 1.f));
				}
			}
		}

		// BodyDelete
		if (nullptr != m_pHandDelete)
		{
			CTransform* pOwnerTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");
			CModel* pOwnerModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
			if (pOwnerTransform != nullptr && pOwnerModel != nullptr)
			{
				CTransform* pEffectTransform = m_pHandDelete->Get_Component<CTransform>(L"Com_Transform");
				if (pEffectTransform != nullptr)
				{
					Matrix EffectMatrix = pOwnerModel->Get_SocketLocalMatrix(1);
					Matrix OwnerWorldMatrix = pOwnerTransform->Get_WorldMatrix();

					// Scale / Rotation
					Matrix matScale = matScale.CreateScale(pEffectTransform->Get_Scale());//m_pScaleOffset[TYPE_E_CIRCLE_LHAND_06]);
					Matrix matRotation = matScale.CreateFromYawPitchRoll(Vec3(XMConvertToRadians(m_pRotationOffset[TYPE_E_ATTACK_BODYDELETE].x), XMConvertToRadians(m_pRotationOffset[TYPE_E_ATTACK_BODYDELETE].y), XMConvertToRadians(m_pRotationOffset[TYPE_E_ATTACK_BODYDELETE].z)));
					Matrix matResult = matScale * matRotation * (EffectMatrix * OwnerWorldMatrix);
					pEffectTransform->Set_WorldMatrix(matResult);

					// Position
					_vector vCurrentPosition = pEffectTransform->Get_Position();
					_vector vFinalPosition = vCurrentPosition;
					vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_RIGHT) * m_pPositionOffset[TYPE_E_ATTACK_BODYDELETE].x;
					vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_UP) * m_pPositionOffset[TYPE_E_ATTACK_BODYDELETE].y;
					vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_LOOK) * m_pPositionOffset[TYPE_E_ATTACK_BODYDELETE].z;
					pEffectTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(XMVectorGetX(vFinalPosition), XMVectorGetY(vFinalPosition), XMVectorGetZ(vFinalPosition), 1.f));
				}
			}
		}

	}
}

void CVfx_Witch_Attack::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Witch_Attack::Render()
{
	return S_OK;
}

HRESULT CVfx_Witch_Attack::Ready_Components()
{
	return S_OK;
}

CVfx_Witch_Attack* CVfx_Witch_Attack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Witch_Attack* pInstance = new CVfx_Witch_Attack(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Witch_Attack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Witch_Attack::Clone(void* pArg)
{
	CVfx_Witch_Attack* pInstance = new CVfx_Witch_Attack(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Witch_Attack");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Witch_Attack::Free()
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