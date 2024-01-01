#include "stdafx.h"
#include "UI_Milepost.h"
#include "GameInstance.h"

CUI_Milepost::CUI_Milepost(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_MILEPOST eType)
	: CUI(pDevice, pContext, L"UI_Milepost")
	, m_eType(eType)
{
}

CUI_Milepost::CUI_Milepost(const CUI_Milepost& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

HRESULT CUI_Milepost::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Milepost::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;

	return S_OK;
}

void CUI_Milepost::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		// TargetPos와 Player의 Position을 비교해서 1m안쪽으로 들어오면 m_bActive = false처리함.

		__super::Tick(fTimeDelta);
	}
}

void CUI_Milepost::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		/*
		if (nullptr != m_pOwner)
		{
			_float4 vCamPos = GI->Get_CamPosition();
			_vector vTempForDistance = m_pTransformCom->Get_Position() - XMLoadFloat4(&vCamPos);
			_float fDistance = XMVectorGetX(XMVector3Length(vTempForDistance));

			if (fDistance > 1.f)
			{
				CTransform* pTransform = m_pOwner->Get_Component<CTransform>(L"Com_Transform");

				_float4x4 matTargetWorld = pTransform->Get_WorldFloat4x4();
				matTargetWorld._42 += 1.5f;

				_float4x4 matWorld;
				matWorld = matTargetWorld;
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
				m_tInfo.fX = vWindowPos.x * g_iWinSizeX * 0.5f + (g_iWinSizeX * 0.5f);
				m_tInfo.fY = vWindowPos.y * -( g_iWinSizeY * 0.5f ) + (g_iWinSizeY * 0.5f);

				m_pTransformCom->Set_State(CTransform::STATE_POSITION,
					XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

				if (fDistance < 10.f)
				{
					// 몬스터 방향 벡터를 구하고, 카메라 방향과의 각도 계산
					_vector vCameraPos = XMLoadFloat4(&vCamPos);
					_vector vMonsterToCamera = pTransform->Get_Position() - vCameraPos;

					vMonsterToCamera = XMVector3Normalize(vMonsterToCamera);
					CCamera* pCurCamera = CCamera_Manager::GetInstance()->Get_CurCamera();
					if (nullptr == pCurCamera)
						return;

					CTransform* pCameraTrans = pCurCamera->Get_Transform();
					if (nullptr == pCameraTrans)
						return;

					_vector vCameraForward = pCameraTrans->Get_State(CTransform::STATE_LOOK);
					vCameraForward = XMVector3Normalize(vCameraForward);

					_float fAngle = XMVectorGetX(XMVector3Dot(vMonsterToCamera, vCameraForward));
					if (fAngle >= XMConvertToRadians(0.f) && fAngle <= XMConvertToRadians(180.f))
					{
						Set_Text(_float2(m_tInfo.fX, m_tInfo.fY));
						m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
					}
				}
			}
		}
		*/
		
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}

}

HRESULT CUI_Milepost::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Milepost::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eType)
	{
	case UI_MILEPOST::MILEPOST_FLAG:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Map_TargetPosition_Flag"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_MILEPOST::MILEPOST_ARROW:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Map_TargetPosition_Arrow"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	}

	return S_OK;
}

HRESULT CUI_Milepost::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Milepost::Bind_ShaderResources()
{

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

void CUI_Milepost::Calculate_Distance()
{
	// m_fDistance를 채워준다. 거리계산 -> Add_Text하게될 변수
}

CUI_Milepost* CUI_Milepost::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_MILEPOST eType)
{
	CUI_Milepost* pInstance = new CUI_Milepost(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Milepost");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Milepost::Clone(void* pArg)
{
	CUI_Milepost* pInstance = new CUI_Milepost(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Milepost");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Milepost::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
