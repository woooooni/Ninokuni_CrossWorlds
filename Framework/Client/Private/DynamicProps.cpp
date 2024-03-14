#include "stdafx.h"
#include "..\Public\DynamicProps.h"
#include "GameInstance.h"

CDynamicProps::CDynamicProps(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int iObjectType)
	: CDynamicObject(pDevice, pContext, strObjectTag, iObjectType)
{
}

CDynamicProps::CDynamicProps(const CDynamicObject& rhs)
	: CDynamicObject(rhs)
{
}

HRESULT CDynamicProps::Initialize_Prototype(_uint eType, const wstring& strFilePath, const wstring& strFileName)
{
	if (FAILED(Ready_AnimModelCom(eType, strFilePath, strFileName)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDynamicProps::Initialize(void* pArg)
{
	m_eType = DYNAMIC_TYPE::DYNAMIC_PROP;
	
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	return S_OK;
}

void CDynamicProps::Tick(_float fTimeDelta)
{
	m_pStateMachineCom->Tick_State(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CDynamicProps::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CDynamicProps::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CDynamicProps::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CDynamicProps::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

HRESULT CDynamicProps::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"),
		TEXT("Com_AnimShader"), reinterpret_cast<CComponent**>(&m_pAnimShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Cat"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"),
		TEXT("Com_StateMachine"), reinterpret_cast<CComponent**>(&m_pStateMachineCom))))
		return E_FAIL;

	m_pModelCom->Set_Animation(0);

	return S_OK;
}

HRESULT CDynamicProps::Ready_State()
{
	return S_OK;
}

HRESULT CDynamicProps::Ready_AnimModelCom(_uint eType, const wstring& strFilePath, const wstring& strFileName)
{
	if (nullptr != m_pModelCom)
		Safe_Release<CModel*>(m_pModelCom);

	_tchar szFileName[MAX_PATH];
	_tchar szExt[MAX_PATH];
	_wsplitpath_s(strFileName.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

	m_strDynamicName = szFileName;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, wstring(L"Prototype_Component_Model_") + szFileName,
		eType, strFilePath, strFileName)))
		return E_FAIL;


	return S_OK;
}

CDynamicProps* CDynamicProps::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const wstring& strFilePath, const wstring& strFileName, _int eObjType, _uint eModelType)
{
	CDynamicProps* pInstance = new CDynamicProps(pDevice, pContext, strObjectTag, eObjType);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, strFilePath, strFileName)))
	{
		MSG_BOX("Create Failed to ProtoType : CDynamicProps");
		Safe_Release<CDynamicProps*>(pInstance);
	}

	return pInstance;
}

CGameObject* CDynamicProps::Clone(void* pArg)
{
	CDynamicProps* pInstance = new CDynamicProps(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CDynamicProps");
		Safe_Release<CDynamicProps*>(pInstance);
	}

	return pInstance;
}

void CDynamicProps::Free()
{
	__super::Free();
}
