#include "stdafx.h"
#include "UI_MonsterHP_World.h"
#include "GameInstance.h"
#include "Monster.h"
#include "UI_Basic.h"
#include "Camera_Manager.h"
#include "Camera.h"

CUI_MonsterHP_World::CUI_MonsterHP_World(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_MonsterHP_World")
{
}

CUI_MonsterHP_World::CUI_MonsterHP_World(const CUI_MonsterHP_World& rhs)
	: CUI(rhs)
{
}

void CUI_MonsterHP_World::Set_Owner(CMonster* pOwner, _int iElementalType)
{
	m_pOwner = pOwner;
	m_iTextureIndex = iElementalType;

	CMonster::MONSTER_STAT StatDesc = {};
	ZeroMemory(&StatDesc, sizeof(CMonster::MONSTER_STAT));

	memcpy(&StatDesc, &(m_pOwner->Get_Stat()), sizeof(CMonster::MONSTER_STAT));

	m_fCurHP = StatDesc.fHp;
	m_fMaxHP = StatDesc.fMaxHp;
	
	m_strName = m_pOwner->Get_KorName();

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

	_float2 vArrowSize = _float2(40.f * 0.8f, 40.f * 0.8f);

	m_Arrow.reserve(2);
	CGameObject* pArrow = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Monster_WorldHPBar_ArrowLeft"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pArrow)
		return E_FAIL;
	m_Arrow.push_back(dynamic_cast<CUI_Basic*>(pArrow));
	CTransform* pUITransform = dynamic_cast<CUI_Basic*>(pArrow)->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pUITransform)
		return E_FAIL;
	pUITransform->Set_Scale(XMVectorSet(vArrowSize.x, vArrowSize.y, 1.f, 0.f));

	pArrow = nullptr;
	pArrow = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Monster_WorldHPBar_ArrowRight"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pArrow)
		return E_FAIL;
	m_Arrow.push_back(dynamic_cast<CUI_Basic*>(pArrow));
	pUITransform = nullptr;
	pUITransform = dynamic_cast<CUI_Basic*>(pArrow)->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pUITransform)
		return E_FAIL;
	pUITransform->Set_Scale(XMVectorSet(vArrowSize.x, vArrowSize.y, 1.f, 0.f));

	m_bIsTarget = false;

	return S_OK;
}

void CUI_MonsterHP_World::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (Is_Dead())
			return;


		if (nullptr != m_pOwner)
		{
			CTransform* pTransform = m_pOwner->Get_Component<CTransform>(L"Com_Transform");

			_float4 Temp;
			XMStoreFloat4(&Temp, pTransform->Get_Position());

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, pTransform->Get_Position());

			if (m_bIsTarget)
			{
				for (auto& iter : m_Arrow)
					iter->Tick(fTimeDelta);
			}

			// 체력을 update한다
			CMonster::MONSTER_STAT StatDesc = {};
			ZeroMemory(&StatDesc, sizeof(CMonster::MONSTER_STAT));
			memcpy(&StatDesc, &(m_pOwner->Get_Stat()), sizeof(CMonster::MONSTER_STAT));

			m_fCurHP = StatDesc.fHp;
		}

		if (m_pOwner->Is_Dead())
			Set_Dead(true);

		__super::Tick(fTimeDelta);
	}
}

void CUI_MonsterHP_World::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (Is_Dead())
			return;

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
						if (m_bIsTarget)
						{
							_float2 vfOffset = _float2(15.f, -25.f);
							_float2 vArrowPos;
							// Target인 경우에 Arrow를 보여준다. 위치 조정이 필요하다.
							for (auto& iter : m_Arrow)
							{
								CTransform* pArrowTransform = iter->Get_Component<CTransform>(L"Com_Transform");
								if (nullptr == pArrowTransform)
									continue;

								if (iter->Get_ObjectTag() == TEXT("UI_Monster_WorldHP_ArrowLeft"))
								{
									vArrowPos = _float2(m_tInfo.fX - ((m_tInfo.fCX * 0.5f) + vfOffset.x), m_tInfo.fY + vfOffset.y);

									pArrowTransform->Set_State(CTransform::STATE_POSITION,
										XMVectorSet(vArrowPos.x - g_iWinSizeX * 0.5f, -(vArrowPos.y - g_iWinSizeY * 0.5f), 1.f, 1.f));
								}
								else
								{
									vArrowPos = _float2(m_tInfo.fX + ((m_tInfo.fCX * 0.5f) + vfOffset.x), m_tInfo.fY + vfOffset.y);

									pArrowTransform->Set_State(CTransform::STATE_POSITION,
										XMVectorSet(vArrowPos.x - g_iWinSizeX * 0.5f, -(vArrowPos.y - g_iWinSizeY * 0.5f), 1.f, 1.f));
								}
							}

							for (auto& iter : m_Arrow)
								iter->LateTick(fTimeDelta);
						}

						Set_Text(_float2(m_tInfo.fX, m_tInfo.fY));
						m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
					}
				}
			}
		}

		if (m_pOwner->Is_Dead())
			Set_Dead(true);
	}

}

HRESULT CUI_MonsterHP_World::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(14);

	m_pVIBufferCom->Render();

	for (auto& iter : m_Arrow)
		iter->Render();

	return S_OK;
}

HRESULT CUI_MonsterHP_World::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Monster_WorldHPBars"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Monster_WorldHP_Frame"),
		TEXT("Com_Texture_Background"), (CComponent**)&m_pBackTextureCom)))
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
	if (FAILED(m_pShaderCom->Bind_RawValue("g_CurrentHP", &m_fCurHP, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_MaxHP", &m_fMaxHP, sizeof(_float))))
		return E_FAIL;


	if (FAILED(m_pBackTextureCom->Bind_ShaderResource(m_pShaderCom, "g_HPGaugeTexture")))
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
		//m_strName = TEXT("코부리");

		_int iSubLength = m_strSubName.empty() ? 0 : static_cast<_int>(m_strSubName.length());
		_float fSubX = vSubPosition.x - iSubLength * 5.f;

		_int iNameLength = m_strName.empty() ? 0 : static_cast<_int>(m_strName.length());
		_float fNameX = vTextPosition.x - iNameLength * 8.f;

		// 몬스터 설명부분 (이름 위에 뜨는 Sub Text) -> 외곽선
		CRenderer::TEXT_DESC MonsterDesc;
		MonsterDesc.strText = m_strSubName;
		MonsterDesc.strFontTag = L"Default_Medium";
		MonsterDesc.vScale = { 0.3f, 0.3f };
		MonsterDesc.vPosition = _float2(fSubX - 1, vSubPosition.y);
		MonsterDesc.vColor = { 0.f, 0.f, 0.f, 1.f };
		m_pRendererCom->Add_Text(MonsterDesc);
		MonsterDesc.vPosition = _float2(fSubX + 1, vSubPosition.y);
		m_pRendererCom->Add_Text(MonsterDesc);
		MonsterDesc.vPosition = _float2(fSubX, vSubPosition.y - 1);
		m_pRendererCom->Add_Text(MonsterDesc);
		MonsterDesc.vPosition = _float2(fSubX + 1, vSubPosition.y + 1);
		m_pRendererCom->Add_Text(MonsterDesc);
		// Origin Text
		MonsterDesc.vPosition = _float2(fSubX, vSubPosition.y);
		MonsterDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
		m_pRendererCom->Add_Text(MonsterDesc);

		// 몬스터 이름 -> 외곽선
		MonsterDesc.strText = m_strName;
		MonsterDesc.strFontTag = L"Default_Bold";
		MonsterDesc.vScale = { 0.4f, 0.4f };
		MonsterDesc.vPosition = _float2(fNameX - 1.f, vTextPosition.y);
		MonsterDesc.vColor = { 0.f, 0.f, 0.f, 1.f };
		m_pRendererCom->Add_Text(MonsterDesc);
		MonsterDesc.vPosition = _float2(fNameX + 1.f, vTextPosition.y);
		m_pRendererCom->Add_Text(MonsterDesc);
		MonsterDesc.vPosition = _float2(fNameX, vTextPosition.y - 1.f);
		m_pRendererCom->Add_Text(MonsterDesc);
		MonsterDesc.vPosition = _float2(fNameX, vTextPosition.y + 1.f);
		m_pRendererCom->Add_Text(MonsterDesc);
		// 몬스터 이름
		MonsterDesc.vPosition = _float2(fNameX, vTextPosition.y);
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

	for (auto& iter : m_Arrow)
		Safe_Release(iter);
	m_Arrow.clear();

	Safe_Release(m_pBackTextureCom);
	Safe_Release(m_pTextureCom);
}
