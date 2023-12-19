#include "stdafx.h"
#include "Vfx.h"

CVfx::CVfx(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_PARTICLE)
{
}

CVfx::CVfx(const CVfx& rhs)
	: CGameObject(rhs)
{
}

HRESULT CVfx::Initialize_Prototype()
{
 	return S_OK;
}

HRESULT CVfx::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_vPosition = *static_cast<_vector*>(pArg);

	return S_OK;
}

void CVfx::Tick(_float fTimeDelta)
{
}

void CVfx::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx::Render()
{
	return S_OK;
}

HRESULT CVfx::Ready_Components()
{
	return S_OK;
}

void CVfx::Free()
{
	__super::Free();
}