#include "stdafx.h"
#include "Stellia.h"

#include "GameInstance.h"

CStellia::CStellia(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
	: CMonster(pDevice, pContext, strObjectTag, tStat)
{
}

CStellia::CStellia(const CStellia& rhs)
	: CMonster(rhs)
{
}

HRESULT CStellia::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStellia::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	return S_OK;
}

void CStellia::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CStellia::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CStellia::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CStellia::Render_ShadowDepth()
{
	return S_OK;
}

void CStellia::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CStellia::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CStellia::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

HRESULT CStellia::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_AnimShader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For. ModelCom */
	if (nullptr != m_pModelCom)
		Safe_Release(m_pModelCom);

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_TEST, TEXT("Prototype_Component_Model_Stellia"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	m_pModelCom->Set_Owner(this);

	return S_OK;
}

HRESULT CStellia::Ready_States()
{
	return S_OK;
}

HRESULT CStellia::Ready_Colliders()
{
	return S_OK;
}

CStellia* CStellia::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
{
	CStellia* pInstance = new CStellia(pDevice, pContext, strObjectTag, tStat);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Craete : CStellia");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CMonster* CStellia::Clone(void* pArg)
{
	CStellia* pInstance = new CStellia(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CStellia");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStellia::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
