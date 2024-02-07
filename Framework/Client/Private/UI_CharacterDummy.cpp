#include "stdafx.h"
#include "UI_CharacterDummy.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"
#include "Character_Manager.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Sword.h"
#include "UI_Dummy_Weapon.h"
#include "Weapon_Manager.h"
#include "UI_Manager.h"


CUI_CharacterDummy::CUI_CharacterDummy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, CHARACTER_TYPE eType)
	: CCharacter(pDevice, pContext, strObjectTag, eType)
	, m_eCharacterType(eType)
{
}

CUI_CharacterDummy::CUI_CharacterDummy(const CUI_CharacterDummy& rhs)
	: CCharacter(rhs)
	, m_eCharacterType(rhs.m_eCharacterType)
{

}

void CUI_CharacterDummy::Set_Active(_bool bActive)
{
	if (bActive)
	{
		m_pRendererCom->Set_RenderSwitch(CRenderer::RENDER_SWITCH::UIMESH_SWITCH, true);
		m_pTransformCom->LookAt_ForLandObject(m_vCamPosition);
	}
	else
	{
		m_pRendererCom->Set_RenderSwitch(CRenderer::RENDER_SWITCH::UIMESH_SWITCH, false);
	}

	m_bActive = bActive;
}

HRESULT CUI_CharacterDummy::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_CharacterDummy::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Weapon()))
		return E_FAIL;

	_float3 vCamPos, vLook, vUp;

	vCamPos = _float3(0.f, 0.9f, -3.f);
	vLook = _float3(0.f, 0.9f, 0.f);
	vUp = _float3(0.f, 1.f, 0.f);

	m_vCamMatrix = XMMatrixLookAtLH(XMLoadFloat3(&vCamPos), XMLoadFloat3(&vLook), XMLoadFloat3(&vUp));
	m_vCamPosition = XMVectorSet(vCamPos.x, vCamPos.y, vCamPos.z, 1.f);

	XMStoreFloat4x4(&m_ViewMatrix, m_vCamMatrix); // 카메라 행렬을 전치시킴
	//	_float fAspectRatio = (_float)g_iWinSizeX / g_iWinSizeY;
	//	_float fNearZ       = 0.2f;
	//	_float fFarZ        = 1000.f;

	m_ProjMatrix = XMMatrixPerspectiveFovLH(::XMConvertToRadians(60.0f), static_cast<_float>(g_iWinSizeX) / static_cast<_float>(g_iWinSizeY), 0.2f, 1000.0f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-0.5f, 0.f, 0.4f, 1.f));
	m_pTransformCom->LookAt_ForLandObject(m_vCamPosition);

	m_bActive = false;

	if(nullptr != m_pDummyWeapon)
		m_pDummyWeapon->Set_Owner(this, m_eCharacterType);

	switch (m_eCharacterType)
	{
	case CHARACTER_TYPE::SWORD_MAN:
		m_pModelCom->Set_Animation(TEXT("SKM_Swordsman_Merge.ao|Swordsman_BattleStand"));
		break;
	case CHARACTER_TYPE::DESTROYER:
		m_pModelCom->Set_Animation(TEXT("SKM_Destroyer_Merge.ao|Destroyer_BattleStand"));
		break;
	case CHARACTER_TYPE::ENGINEER:
		m_pModelCom->Set_Animation(TEXT("SKM_Engineer_SoulDiver.ao|Engineer_BattleStand"));
		break;
	}

	return S_OK;
}

void CUI_CharacterDummy::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (GI->Mouse_Pressing(DIMK_WHEEL))
		{
			_long	MouseMove = 0;

			if (MouseMove = GI->Get_DIMMoveState(DIMM_X))
			{
				m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), MouseMove * -0.1f, fTimeDelta);
			}
		}

		if (m_pDummyWeapon != nullptr)
		{ 
			Matrix matSocketLocal = m_pModelCom->Get_SocketLocalMatrix(0);
			Matrix matSocketWorld = matSocketLocal * m_pTransformCom->Get_WorldMatrix();

			m_pDummyWeapon->Set_SocketWorld(matSocketWorld);
			m_pDummyWeapon->Tick(fTimeDelta);
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_CharacterDummy::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pModelCom->LateTick(fTimeDelta);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_UI, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_REFLECT, this);

		if (nullptr != m_pDummyWeapon)
			m_pDummyWeapon->LateTick(fTimeDelta);
	}
	
}

HRESULT CUI_CharacterDummy::Render()
{
	if (m_bActive)
	{
		if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_vCamPosition, sizeof(_float4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &GI->Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATE::D3DTS_PROJ))))
			return E_FAIL;
		//if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		//	return E_FAIL;

		_float4 vRimColor = { 0.f, 0.f, 0.f, 0.f };
		if (m_bInfinite)
		{
			vRimColor = { 0.f, 0.5f, 1.f, 1.f };
		}


		if (FAILED(m_pShaderCom->Bind_RawValue("g_vRimColor", &vRimColor, sizeof(_float4))))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
			return E_FAIL;


		for (size_t i = 0; i < PART_TYPE::PART_END; i++)
		{
			if (nullptr == m_pCharacterPartModels[i])
				continue;

			const _uint		iNumMeshes = m_pCharacterPartModels[i]->Get_NumMeshes();

			for (_uint j = 0; j < iNumMeshes; ++j)
			{
				if (FAILED(m_pCharacterPartModels[i]->SetUp_OnShader(m_pShaderCom, m_pCharacterPartModels[i]->Get_MaterialIndex(j), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
					return E_FAIL;

				if (FAILED(m_pCharacterPartModels[i]->Render(m_pShaderCom, j, 11)))
					return E_FAIL;
			}
		}
	}

	return S_OK;
}

HRESULT CUI_CharacterDummy::Render_Reflect()
{
	if (m_bActive)
	{
		if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
			return E_FAIL;

		SimpleMath::Plane mirrorPlane = Vec4(0.0f, 0.0f, 1.0f, -1.2f);
		XMMATRIX matReflect = Matrix::CreateReflection(mirrorPlane);
		Matrix world = m_pTransformCom->Get_WorldMatrix();
		world._41 -= 0.65f;
		Matrix result = world * matReflect;

		Matrix worldInvTranspose = m_pTransformCom->Get_WorldMatrixInverse();
		worldInvTranspose.Transpose();

		if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_vCamPosition, sizeof(_float4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &result)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &GI->Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATE::D3DTS_PROJ))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldInv", &worldInvTranspose)))
			return E_FAIL;
		//if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		//	return E_FAIL;

		_float4 vRimColor = { 0.f, 0.f, 0.f, 0.f };
		if (m_bInfinite)
		{
			vRimColor = { 0.f, 0.5f, 1.f, 1.f };
		}


		if (FAILED(m_pShaderCom->Bind_RawValue("g_vRimColor", &vRimColor, sizeof(_float4))))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
			return E_FAIL;


		for (size_t i = 0; i < PART_TYPE::PART_END; i++)
		{
			if (nullptr == m_pCharacterPartModels[i])
				continue;

			const _uint		iNumMeshes = m_pCharacterPartModels[i]->Get_NumMeshes();

			for (_uint j = 0; j < iNumMeshes; ++j)
			{
				if (FAILED(m_pCharacterPartModels[i]->SetUp_OnShader(m_pShaderCom, m_pCharacterPartModels[i]->Get_MaterialIndex(j), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
					return E_FAIL;

				if (FAILED(m_pCharacterPartModels[i]->Render(m_pShaderCom, j, 4)))
					return E_FAIL;
			}
		}
	}

	return S_OK;
}


HRESULT CUI_CharacterDummy::Ready_Components()
{

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel" ), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	switch (m_eCharacterType)
	{
	case CHARACTER_TYPE::SWORD_MAN:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_SwordMan_Dummy"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		m_iAnimIndex = 54;
		break;

	case CHARACTER_TYPE::ENGINEER:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Engineer_Dummy"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;

	case CHARACTER_TYPE::DESTROYER:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Destroyer_Dummy"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	}


	for (_uint i = 0; i < PART_TYPE::PART_END; ++i)
 		m_pCharacterPartModels[i] = CCharacter_Manager::GetInstance()->Get_PartModel(m_eCharacterType, PART_TYPE(i), 0);

	//m_pModelCom->Set_Animation(m_iAnimIndex);

	return S_OK;
}

#pragma region Ready_States
HRESULT CUI_CharacterDummy::Ready_States()
{
	return S_OK;
}

#pragma endregion


#pragma region Ready_Colliders
HRESULT CUI_CharacterDummy::Ready_Colliders()
{
	return S_OK;
}

#pragma endregion

#pragma region Ready_Sockets
HRESULT CUI_CharacterDummy::Ready_Sockets()
{
	return S_OK;
}
#pragma endregion

#pragma region Ready_Parts
HRESULT CUI_CharacterDummy::Ready_Parts()
{
	return S_OK;
}
HRESULT CUI_CharacterDummy::Ready_Weapon()
{
	switch (m_eCharacterType)
	{
	case CHARACTER_TYPE::SWORD_MAN:
		m_pDummyWeapon = CUI_Dummy_Weapon::Create(m_pDevice, m_pContext, L"SwordMan_Sword");
		if (nullptr == m_pDummyWeapon)
			return S_OK;

		m_pDummyWeapon->Set_WeaponModelCom(CWeapon_Manager::GetInstance()->Get_WeaponModel(m_eCharacterType, L"Sword_Fire02"));
		if (nullptr == m_pDummyWeapon->Get_WeaponModelCom())
		{
			Safe_Release(m_pDummyWeapon);
			return S_OK;
		}
		break;

	case CHARACTER_TYPE::DESTROYER:
		m_pDummyWeapon = CUI_Dummy_Weapon::Create(m_pDevice, m_pContext, L"Destroyer_Hammer");
		if (nullptr == m_pDummyWeapon)
			return S_OK;

		m_pDummyWeapon->Set_WeaponModelCom(CWeapon_Manager::GetInstance()->Get_WeaponModel(m_eCharacterType, L"Fire02"));

		if (nullptr == m_pDummyWeapon->Get_WeaponModelCom())
		{
			Safe_Release(m_pDummyWeapon);
			return S_OK;
		}
		break;

	case CHARACTER_TYPE::ENGINEER:
		m_pDummyWeapon = CUI_Dummy_Weapon::Create(m_pDevice, m_pContext, L"Engineer_Rifle");
		if (nullptr == m_pDummyWeapon)
			return S_OK;

		m_pDummyWeapon->Set_WeaponModelCom(CWeapon_Manager::GetInstance()->Get_WeaponModel(m_eCharacterType, L"Fire"));

		if (nullptr == m_pDummyWeapon->Get_WeaponModelCom())
		{
			Safe_Release(m_pDummyWeapon);
			return S_OK;
		}
		break;
	}

	return S_OK;
}
#pragma endregion

_float2 CUI_CharacterDummy::Transpose_ProjectionPosition()
{
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float4x4 matWorld = m_pTransformCom->Get_WorldFloat4x4();
	_matrix matView = GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	_matrix matProj = GI->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);

	_float4x4 matWindow;
	XMStoreFloat4x4(&matWindow, XMLoadFloat4x4(&matWorld) * matView * matProj);

	_float3 vWindowPos = *(_float3*)&matWindow.m[3][0];
	// &matWindow.m[3][0] -> 포지션의 시작 주소를 얻고,
	// (_float3*) -> _float3 포인터로 캐스팅
	// * -> 그 값을 가져온다.

	vWindowPos.x /= vWindowPos.z;
	vWindowPos.y /= vWindowPos.z;
	_float fScreenX = vWindowPos.x * g_iWinSizeX * 0.5f + (g_iWinSizeX * 0.5f);
	_float fScreenY = vWindowPos.y * -(g_iWinSizeY * 0.5f) + (g_iWinSizeY * 0.5f);

	return _float2(fScreenX, fScreenY);
}

CUI_CharacterDummy* CUI_CharacterDummy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, CHARACTER_TYPE eType)
{
	CUI_CharacterDummy* pInstance = new CUI_CharacterDummy(pDevice, pContext, strObjectTag, eType);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CUI_CharacterDummy");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CUI_CharacterDummy::Clone(void* pArg)
{
	CUI_CharacterDummy* pInstance = new CUI_CharacterDummy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_CharacterDummy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_CharacterDummy::Free()
{
	__super::Free();

	Safe_Release(m_pDummyWeapon);
}

