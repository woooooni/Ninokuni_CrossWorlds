#include "stdafx.h"
#include "..\Public\Part.h"

#include "GameInstance.h"

CPart::CPart(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObejctTag, _uint iObjectType)
	: CGameObject(pDevice, pContext, strObejctTag, iObjectType)
{

}

CPart::CPart(const CPart& rhs)
	: CGameObject(rhs)
{

}


HRESULT CPart::Set_Owner(CGameObject* pOwner)
{
	if (nullptr == pOwner)
		return E_FAIL;

	if (nullptr != m_pOwner)
		Safe_Release(m_pOwner);

	Safe_AddRef(m_pOwner);

	return S_OK;
}

HRESULT CPart::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPart::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CPart::Tick(_float fTimeDelta)
{
}

void CPart::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom || nullptr == m_pModelCom)
		return;

	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	m_pModelCom->LateTick(fTimeDelta);

#ifdef NDEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
#endif // DEBUG
	
}

HRESULT CPart::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	return S_OK;
}

void CPart::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pOwner);
}
