#include "stdafx.h"
#include "GameInstance.h"
#include "SwordMan_AuraBlade.h"

#include "Effect_Manager.h"
#include "Character.h"

CSwordMan_AuraBlade::CSwordMan_AuraBlade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCharacter_Projectile(pDevice, pContext, L"SwordMan_AuraBlade")
{

}

CSwordMan_AuraBlade::CSwordMan_AuraBlade(const CSwordMan_AuraBlade& rhs)
	: CCharacter_Projectile(rhs)
{
}


HRESULT CSwordMan_AuraBlade::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	

	return S_OK;

}

HRESULT CSwordMan_AuraBlade::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_fDeletionTime = 1.f;
	m_fAccDeletionTime = 0.f;

	Set_Collider_Elemental(m_pOwner->Get_ElementalType());
	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, true);

	return S_OK;
}

void CSwordMan_AuraBlade::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_fMoveSpeed, fTimeDelta);
}

void CSwordMan_AuraBlade::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CSwordMan_AuraBlade::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	// 그리지 않는다.
	return S_OK;
}

void CSwordMan_AuraBlade::Collision_Enter(const COLLISION_INFO& tInfo)
{
	wstring strSoundKey = L"Hit_PC_Combo_Slash_Flesh_" + to_wstring(GI->RandomInt(1, 4)) + L".mp3";
	GI->Play_Sound(strSoundKey, SOUND_MONSTERL_HIT, 0.3f, false);
}

HRESULT CSwordMan_AuraBlade::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 1.f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	SphereDesc.vOffsetPosition = Vec3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::ATTACK, &SphereDesc)))
		return E_FAIL;



	return S_OK;
}


CSwordMan_AuraBlade* CSwordMan_AuraBlade::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSwordMan_AuraBlade* pInstance = new CSwordMan_AuraBlade(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CSwordMan_AuraBlade");
		Safe_Release<CSwordMan_AuraBlade*>(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CSwordMan_AuraBlade::Clone(void* pArg)
{
	CSwordMan_AuraBlade* pInstance = new CSwordMan_AuraBlade(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CSwordMan_AuraBlade");
		Safe_Release<CSwordMan_AuraBlade*>(pInstance);
	}

	return pInstance;
}

void CSwordMan_AuraBlade::Free()
{
	__super::Free();
}
