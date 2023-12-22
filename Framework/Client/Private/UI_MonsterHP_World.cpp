#include "stdafx.h"
#include "UI_MonsterHP_World.h"
#include "GameInstance.h"
#include "Monster.h"

CUI_MonsterHP_World::CUI_MonsterHP_World(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_MonsterHP_World")
{
}

CUI_MonsterHP_World::CUI_MonsterHP_World(const CUI_MonsterHP_World& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_MonsterHP_World::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_MonsterHP_World::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;

	m_UIWorldMatrix = XMMatrixIdentity();

	m_tInfo.fCX = 0.4f;
	m_tInfo.fCY = 0.13f;
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));

	return S_OK;
}

void CUI_MonsterHP_World::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (nullptr != m_pOwner)
		{
			CTransform* pTransform = m_pOwner->Get_Component<CTransform>(L"Com_Transform");
			//pTransform->Get_Position();
			_float4 Temp;
			XMStoreFloat4(&Temp, pTransform->Get_Position());

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, pTransform->Get_Position());
		}
		__super::Tick(fTimeDelta);
	}
}

void CUI_MonsterHP_World::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (nullptr != m_pOwner)
		{
			_float4 vCamPos = GI->Get_CamPosition();
			_vector vTempForDistance = m_pTransformCom->Get_Position() - XMLoadFloat4(&vCamPos); //
			_float fDistance = XMVectorGetX(XMVector3Length(vTempForDistance)); //

			if (fDistance > 1.f) //
			{
				// WorldMatrix를 Update한다.
				_float3 vTemp = m_pTransformCom->Get_Scale();
				_vector vScale = XMVectorSet(vTemp.x, vTemp.y, vTemp.z, 0.f);

				CTransform* pTransform = m_pOwner->Get_Component<CTransform>(L"Com_Transform");

				_vector vOwnerPos = pTransform->Get_Position();
				_float4 vOwnerTemp;
				XMStoreFloat4(&vOwnerTemp, vOwnerPos);
				// CameraPosition의 y값을 Owner의 y값으로 채워준다.
				vCamPos.y = vOwnerTemp.y;

				//_vector vLook = XMVector3Normalize(XMVectorSubtract(m_pTransformCom->Get_Position(), XMLoadFloat4(&vCamPos)));
				_vector vLook = XMVector3Normalize(m_pTransformCom->Get_Position() - XMLoadFloat4(&vCamPos));
				_vector vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));
				_vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));

				m_UIWorldMatrix.r[CTransform::STATE_RIGHT] = XMVectorScale(vRight, vTemp.x);
				m_UIWorldMatrix.r[CTransform::STATE_UP] = XMVectorScale(vUp, vTemp.y);
				m_UIWorldMatrix.r[CTransform::STATE_LOOK] = XMVectorScale(vLook, vTemp.z);

				vOwnerTemp.y += 1.5f;
				m_UIWorldMatrix.r[CTransform::STATE_POSITION] = XMLoadFloat4(&vOwnerTemp);

				m_pTransformCom->Set_WorldMatrix(m_UIWorldMatrix); // Text를 띄우기 위해서 추가(문제시 삭제할 것)

				if (fDistance < 10.f)
				{
					Set_Text();
					m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
				}
			}
		}
	}
}

HRESULT CUI_MonsterHP_World::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(11);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_MonsterHP_World::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Monster_WorldHPBars"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_MonsterHP_World::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_MonsterHP_World::Bind_ShaderResources()
{
	_float4x4	WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, m_UIWorldMatrix);

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &WorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &GI->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &GI->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
		return E_FAIL;

	return S_OK;
}

void CUI_MonsterHP_World::Set_Text()
{
	wstring strMonsterTag = m_pOwner->Get_ObjectTag();


	if (strMonsterTag == TEXT("Shadow_Thief"))
	{
		m_strSubName = TEXT("서릿별 나무");
		m_strName = TEXT("코부리");

		//_matrix matWorld = m_pTransformCom->Get_WorldMatrix();
		//m_UIWorldMatrix
		_matrix matWorld = m_UIWorldMatrix;
		_matrix matView = GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
		_matrix matProj = GI->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);

		_matrix vWVP = matWorld * matView * matProj;

		_vector vWorldPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION); // 3D공간상의 좌표
		_vector vScreenPosition = XMVector3TransformCoord(vWorldPosition, vWVP);

		// 
		_float2 normalizedScreenPos;
		normalizedScreenPos.x = (vScreenPosition.m128_f32[0] + 1.0f) * 0.5f * g_iWinSizeX;
		normalizedScreenPos.y = (1.0f - vScreenPosition.m128_f32[1]) * 0.5f * g_iWinSizeY;
		
		CRenderer::TEXT_DESC MaxHPDesc;
		MaxHPDesc.strText = m_strSubName;
		MaxHPDesc.strFontTag = L"Default_Medium";
		MaxHPDesc.vScale = { 0.4f, 0.4f };
		MaxHPDesc.vPosition = normalizedScreenPos;
		MaxHPDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
		m_pRendererCom->Add_Text(MaxHPDesc);
	}

}

CUI_MonsterHP_World* CUI_MonsterHP_World::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_MonsterHP_World* pInstance = new CUI_MonsterHP_World(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_MonsterHP_World");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_MonsterHP_World::Clone(void* pArg)
{
	CUI_MonsterHP_World* pInstance = new CUI_MonsterHP_World(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_MonsterHP_World");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_MonsterHP_World::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
