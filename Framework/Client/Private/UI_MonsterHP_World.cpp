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

	m_tInfo.fCX = 80.f;
	m_tInfo.fCY = 25.f;
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
			_vector vTempForDistance = m_pTransformCom->Get_Position() - XMLoadFloat4(&vCamPos);
			_float fDistance = XMVectorGetX(XMVector3Length(vTempForDistance));

			if (fDistance > 1.f) //
			{
				//CTransform* pTransform = m_pOwner->Get_Component<CTransform>(L"Com_Transform");
				//
				//_matrix matWorld = pTransform->Get_WorldMatrix();
				//// 회전값을 없앤다.
				//_matrix		Matrix = matWorld;
				//Matrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(matWorld.r[0]));
				//Matrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(matWorld.r[1]));
				//Matrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetX(XMVector3Length(matWorld.r[2]));
				//matWorld = Matrix;
				//
				//_matrix matView = GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
				//_matrix matProj = GI->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);
				//
				//_matrix vWVP = matWorld * matView * matProj;
				//_vector vWorldPosition = pTransform->Get_State(CTransform::STATE_POSITION); // 3D공간상의 좌표
				//
				//_vector vScreenPosition = XMVector3TransformCoord(vWorldPosition, vWVP);
				//vScreenPosition = XMVectorDivide(vScreenPosition, XMVectorSplatW(vScreenPosition)); // W나누기
				//
				//_float4 vScreenPos;
				//XMStoreFloat4(&vScreenPos, vScreenPosition);
				//
				//_float2 normalizedScreenPos;
				//normalizedScreenPos.x = (vScreenPos.x + 1.0f) * (g_iWinSizeX * 0.5f);
				//normalizedScreenPos.y = (1.0f - vScreenPos.y) * (g_iWinSizeY * 0.5f);
				//
				//m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				//	XMVectorSet(normalizedScreenPos.x - g_iWinSizeX * 0.5f,
				//		-(normalizedScreenPos.y - g_iWinSizeY * 0.5f), 0.f, 1.f));
				

				// 수정중

				CTransform* pTransform = m_pOwner->Get_Component<CTransform>(L"Com_Transform");

				_float4x4 matTargetWorld = pTransform->Get_WorldFloat4x4();
				matTargetWorld._42 += 1.5f;

				_float4x4 matWorld;
				matWorld = matTargetWorld;
				//XMStoreFloat4x4(&matWorld, pTransform->Get_WorldMatrix());
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
					Set_Text(_float2(m_tInfo.fX, m_tInfo.fY));
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

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
		return E_FAIL;

	return S_OK;
}

void CUI_MonsterHP_World::Set_Text(_float2 ScreenPos)
{
	_float2 vSubPosition = _float2(ScreenPos.x, ScreenPos.y - 45.f);
	_float2 vTextPosition = _float2(ScreenPos.x, ScreenPos.y - 30.f);

	wstring strMonsterTag = m_pOwner->Get_ObjectTag();
	// Todo : Text글자길이로 x값 정렬하기
	if (strMonsterTag == TEXT("Shadow_Thief"))
	{
		m_strSubName = TEXT("서릿별 나무");
		m_strName = TEXT("코부리");
		
		CRenderer::TEXT_DESC MonsterDesc;
		MonsterDesc.strText = m_strSubName;
		MonsterDesc.strFontTag = L"Default_Medium";
		MonsterDesc.vScale = { 0.3f, 0.3f };
		MonsterDesc.vPosition = vSubPosition;
		MonsterDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
		m_pRendererCom->Add_Text(MonsterDesc);

		MonsterDesc.strText = m_strName;
		MonsterDesc.vScale = { 0.4f, 0.4f };
		MonsterDesc.vPosition = vTextPosition;
		MonsterDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
		m_pRendererCom->Add_Text(MonsterDesc);
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
