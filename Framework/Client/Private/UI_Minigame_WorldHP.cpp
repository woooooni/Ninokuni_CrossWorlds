#include "stdafx.h"
#include "UI_Minigame_WorldHP.h"
#include "GameInstance.h"
#include "Monster.h"
#include "UI_Manager.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Vehicle_Flying_EnemyBiplane.h"

CUI_Minigame_WorldHP::CUI_Minigame_WorldHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Minigame_WorldHP")
{
}

CUI_Minigame_WorldHP::CUI_Minigame_WorldHP(const CUI_Minigame_WorldHP& rhs)
	: CUI(rhs)
{
}

void CUI_Minigame_WorldHP::Set_VehicleInformation(CVehicle_Flying_EnemyBiplane* pOwner)
{
	if (nullptr == pOwner)
		return;

	m_pTarget = pOwner;

	CVehicle_Flying::ENEMY_STAT StatDesc = {};
	memcpy(&StatDesc, &(m_pTarget->Get_Stat()), sizeof(CVehicle_Flying::ENEMY_STAT));

	if (false == StatDesc.bIsEnemy)
		return;

	m_fMaxHP = StatDesc.fMaxHP;
	m_fCurHP = StatDesc.fCurHP;
	m_fPreHP = m_fCurHP;
}

HRESULT CUI_Minigame_WorldHP::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_WorldHP::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;
	m_bLerp = false;

	m_tInfo.fCX = 0.6f;
	m_tInfo.fCY = 0.2f;
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));

	return S_OK;
}

void CUI_Minigame_WorldHP::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (nullptr == m_pTarget)
			return;

		if (Is_Dead())
			return;

		if (m_pTarget->Is_Dead())
		{
			Set_Dead(true);
			return;
		}

		if (nullptr != m_pTarget)
		{
			CTransform* pTransform = m_pTarget->Get_Component<CTransform>(L"Com_Transform");
			_float4 Temp;
			XMStoreFloat4(&Temp, pTransform->Get_Position());

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, pTransform->Get_Position());
		}

		// 현재 체력을 갱신한다.
		CVehicle_Flying::ENEMY_STAT StatDesc = {};
		memcpy(&StatDesc, &(m_pTarget->Get_Stat()), sizeof(CVehicle_Flying::ENEMY_STAT));
		m_fCurHP = StatDesc.fCurHP;

		if (m_fCurHP < m_fPreHP)
			m_bLerp = false;

		if (!m_bLerp && m_fPreHP > m_fCurHP)
		{
			m_fPreHP -= fTimeDelta * 5000.f;

			if (m_fPreHP <= m_fCurHP)
			{
				m_fPreHP = m_fCurHP;
				m_bLerp = true;
			}
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_Minigame_WorldHP::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (nullptr == m_pTarget)
			return;

		if (Is_Dead())
			return;

		if (m_pTarget->Is_Dead() || m_pTarget->Is_ReserveDead())
		{
			Set_Dead(true);
			return;
		}

		// 체력바를 띄운다.
		_float4 vCamPos = GI->Get_CamPosition();
		_vector vTempForDistance = m_pTransformCom->Get_Position() = XMLoadFloat4(&vCamPos);
		_float fDistance = XMVectorGetX(XMVector3Length(vTempForDistance));

		if (fDistance > 0.1f)
		{
			_float3 vTemp = m_pTransformCom->Get_Scale();
			_vector vScale = XMVectorSet(vTemp.x, vTemp.y, vTemp.z, 0.f);

			CTransform* pTransform = m_pTarget->Get_Component<CTransform>(L"Com_Transform");

			_vector vTargetPos = pTransform->Get_Position();
			_float4 vTargetTemp;
			XMStoreFloat4(&vTargetTemp, vTargetPos);
			vCamPos.y = vTargetTemp.y;

			_vector vLook = XMVector3Normalize(m_pTransformCom->Get_Position() - XMLoadFloat4(&vCamPos));
			_vector vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));
			_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
			//_vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)); // Y빌보드를 막기 위해 Up을 0, 1, 0으로 설정함

			m_BarWorldMatrix.r[CTransform::STATE_RIGHT] = XMVectorScale(vRight, vTemp.x);
			m_BarWorldMatrix.r[CTransform::STATE_UP] = XMVectorScale(vUp, vTemp.y);
			m_BarWorldMatrix.r[CTransform::STATE_LOOK] = XMVectorScale(vLook, vTemp.z);

			vTargetTemp.y += 2.4f;
			m_BarWorldMatrix.r[CTransform::STATE_POSITION] = XMLoadFloat4(&vTargetTemp);

			m_pTransformCom->Set_WorldMatrix(m_BarWorldMatrix);
			//m_pTransformCom->LookAt_ForLandObject(XMLoadFloat4(&GI->Get_CamPosition()));

			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
		}
	}
}

HRESULT CUI_Minigame_WorldHP::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(11);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Minigame_WorldHP::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MonsterHP_WhiteBar"),
		TEXT("Com_FXTexture"), (CComponent**)&m_pFXTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MonsterHP_Bar"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Minigame_WorldHP::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Minigame_WorldHP::Bind_ShaderResources()
{
	_float4x4	WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, m_BarWorldMatrix);
	
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &WorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &GI->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &GI->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;




//	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
//		return E_FAIL;
//
//	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
//		return E_FAIL;
//
//	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
//		return E_FAIL;
//	if (FAILED(m_pShaderCom->Bind_RawValue("g_LerpHP", &m_fPreHP, sizeof(_float))))
//		return E_FAIL;
//	if (FAILED(m_pShaderCom->Bind_RawValue("g_CurrentHP", &m_fCurHP, sizeof(_float))))
//		return E_FAIL;
//	if (FAILED(m_pShaderCom->Bind_RawValue("g_MaxHP", &m_fMaxHP, sizeof(_float))))
//		return E_FAIL;
//
//	if (FAILED(m_pFXTextureCom->Bind_ShaderResource(m_pShaderCom, "g_LerpTexture")))
//		return E_FAIL;
//
//	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
//		return E_FAIL;

	return S_OK;
}

CUI_Minigame_WorldHP* CUI_Minigame_WorldHP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Minigame_WorldHP* pInstance = new CUI_Minigame_WorldHP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Minigame_WorldHP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Minigame_WorldHP::Clone(void* pArg)
{
	CUI_Minigame_WorldHP* pInstance = new CUI_Minigame_WorldHP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Minigame_WorldHP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minigame_WorldHP::Free()
{
	__super::Free();

	Safe_Release(m_pFXTextureCom);
	Safe_Release(m_pTextureCom);
}
