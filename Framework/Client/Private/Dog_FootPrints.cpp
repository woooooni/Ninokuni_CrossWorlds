#include "stdafx.h"
#include "Dog_FootPrints.h"

#include "Effect_Manager.h"
#include "GameInstance.h"
#include "Decal.h"

#include "Game_Manager.h"
#include "Character.h"
#include "Player.h"

CDog_FootPrints::CDog_FootPrints(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CStaticObject(pDevice, pContext, strObjectTag, eType)
{
}

CDog_FootPrints::CDog_FootPrints(const CDog_FootPrints& rhs)
	: CStaticObject(rhs)
{
}

HRESULT CDog_FootPrints::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDog_FootPrints::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	OBJECT_INIT_DESC Init_Data = {};

	if (pArg != nullptr)
		Init_Data.vStartPosition = static_cast<OBJECT_INIT_DESC*>(pArg)->vStartPosition;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &Init_Data)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, Init_Data.vStartPosition);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fShowDist = 5.f;

	return S_OK;
}

void CDog_FootPrints::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	// 최초 생성
	if (m_pFootPrints == nullptr)
	{
		CEffect_Manager::GetInstance()->Generate_Decal(TEXT("Dog_FootPrints_Bundle"), m_pTransformCom->Get_WorldMatrix(),
			Vec3(0.f, 0.f, 0.f), Vec3(1.f, 1.f, 1.f), Vec3(0.f, 0.f, 0.f), nullptr, &m_pFootPrints, false);
		Safe_AddRef(m_pFootPrints);
	}
	// 조건이 맞으면 업데이트 하고 보여준다..
	else if(m_pFootPrints && m_pFootPrints->Get_IsDecalShow())
	{
		m_pFootPrints->Get_TransformCom()->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
	}

	if (GI->Get_CurrentLevel() != LEVELID::LEVEL_TOOL)
	{
		// 플레이어와의 거리 구하기
		Vec4 vPlayerPos = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_CharacterPosition();
		Vec4 vResult = vPlayerPos - (Vec4)(m_pTransformCom->Get_Position());
		_float fDist = vResult.Length();

		// 거리에 따른 활성 여부.
		if (fabs(fDist) < m_fShowDist)
			m_pFootPrints->Set_IsDecalShow(true, fTimeDelta);
		else
			m_pFootPrints->Set_IsDecalShow(false, fTimeDelta);
	}

}

void CDog_FootPrints::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	// Shadow 필요하면 ShadowRender 추가?
	//m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	m_pRendererCom->Add_RenderGroup_Instancing(CRenderer::RENDERGROUP::RENDER_SHADOW, CRenderer::INSTANCING_SHADER_TYPE::MODEL, this, m_pTransformCom->Get_WorldFloat4x4());
	m_pRendererCom->Add_RenderGroup_Instancing(CRenderer::RENDER_NONBLEND, CRenderer::INSTANCING_SHADER_TYPE::MODEL, this, m_pTransformCom->Get_WorldFloat4x4());
}

HRESULT CDog_FootPrints::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDog_FootPrints::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	return S_OK;
}


CDog_FootPrints* CDog_FootPrints::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType)
{
	CDog_FootPrints* pInstance = new CDog_FootPrints(pDevice, pContext, strObjectTag, eObjType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CDog_FootPrints");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDog_FootPrints::Clone(void* pArg)
{
	CDog_FootPrints* pInstance = new CDog_FootPrints(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Failed : CDog_FootPrints");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDog_FootPrints::Free()
{
	__super::Free();

	Safe_Release(m_pFootPrints);
}
