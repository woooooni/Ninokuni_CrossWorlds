#include "..\Public\GameInstance.h"
#include "Timer_Manager.h"
#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Light_Manager.h"
#include "Font_Manager.h"
#include "Model_Manager.h"
#include "Key_Manager.h"
#include "Collision_Manager.h"
#include "Target_Manager.h"
#include "Frustum.h"
#include "PhysX_Manager.h"
#include "Utils.h"
#include "JsonUtils.h"
#include "Camera_Manager.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pTimer_Manager(CTimer_Manager::GetInstance())
	, m_pGraphic_Device(CGraphic_Device::GetInstance())
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_pLevel_Manager(CLevel_Manager::GetInstance())
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pPipeLine(CPipeLine::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())
	, m_pKey_Manager(CKey_Manager::GetInstance())
	, m_pFont_Manager(CFont_Manager::GetInstance())
	, m_pModel_Manager(CModel_Manager::GetInstance())
	, m_pCollision_Manager(CCollision_Manager::GetInstance())
	, m_pTarget_Manager(CTarget_Manager::GetInstance())
	, m_pFrustum(CFrustum::GetInstance())
	, m_pSound_Manager(CSound_Manager::GetInstance())
	, m_pPhysXManager(CPhysX_Manager::GetInstance())
	// , m_pNetwork_Manager(CNetwork_Manager::GetInstance())
	, m_pCamera_Manager(CCamera_Manager::GetInstance())
	
{
	Safe_AddRef(m_pTarget_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pPipeLine);
	Safe_AddRef(m_pLight_Manager); 
	Safe_AddRef(m_pKey_Manager);
	Safe_AddRef(m_pFont_Manager);
	Safe_AddRef(m_pModel_Manager);
	Safe_AddRef(m_pCollision_Manager);
	Safe_AddRef(m_pFrustum);
	Safe_AddRef(m_pSound_Manager);
	Safe_AddRef(m_pPhysXManager);
	// Safe_AddRef(m_pNetwork_Manager);
	Safe_AddRef(m_pCamera_Manager);
}

HRESULT CGameInstance::Initialize_Engine(_uint iNumLevels, _uint iNumLayerType, 
	const GRAPHIC_DESC& GraphicDesc, 
	_Inout_ ID3D11Device** ppDevice, 
	_Inout_ ID3D11DeviceContext** ppContext, 
	_In_ HWND hWnd,
	_In_ HINSTANCE hInst)
{
	/* 그래픽디바이스 초기화 처리. */
	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(GraphicDesc.hWnd, GraphicDesc.eWinMode, GraphicDesc.iWinSizeX, GraphicDesc.iWinSizeY, ppDevice, ppContext)))
		return E_FAIL;

	/* 사운드디바이스 초기화 처리. */
	/* 입력디바이스 초기화 처리. */

	/* 오브젝트 매니져의 예약 처리. */
	if (FAILED(m_pObject_Manager->Reserve_Manager(iNumLevels, iNumLayerType)))
		return E_FAIL;

	/* 컴포넌트 매니져의 예약 처리. */
	if (FAILED(m_pComponent_Manager->Reserve_Manager(iNumLevels)))
		return E_FAIL;

	if (FAILED(m_pKey_Manager->Reserve_Manager(*ppDevice, *ppContext, hWnd)))
		return E_FAIL;

	if (FAILED(m_pInput_Device->Initialize(hInst, hWnd)))
		return E_FAIL;

	if (FAILED(m_pModel_Manager->Reserve_Manager(*ppDevice, *ppContext)))
		return E_FAIL;

	if (FAILED(m_pCollision_Manager->Reserve_Manager()))
		return E_FAIL;

	if (FAILED(m_pPhysXManager->Reserve_Manager(*ppDevice, *ppContext)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Reserve_Manager(*ppDevice, GraphicDesc.iWinSizeX, GraphicDesc.iWinSizeY)))
		return E_FAIL;

	if (FAILED(m_pFrustum->Initialize()))
		return E_FAIL;

	if (FAILED(m_pSound_Manager->Reserve_Manager()))
		return E_FAIL;
	
	if (FAILED(m_pCamera_Manager->Reserve_Manager(*ppDevice, *ppContext)))
		return E_FAIL;


	return S_OK;
}

void CGameInstance::Priority_Tick(_float fTimeDelta)
{
	m_pCamera_Manager->Tick(fTimeDelta);
	m_pInput_Device->Update();
	m_pKey_Manager->Tick(fTimeDelta);
	m_pObject_Manager->Priority_Tick(fTimeDelta);
}

void CGameInstance::Tick(_float fTimeDelta)
{
	m_pLevel_Manager->Tick(fTimeDelta);
	m_pObject_Manager->Tick(fTimeDelta);
	m_pPhysXManager->Tick(fTimeDelta);
	
  	m_pPipeLine->Tick();
	m_pFrustum->Tick();

}

void CGameInstance::LateTick(_float fTimeDelta)
{
	m_pCamera_Manager->LateTick(fTimeDelta);

	m_pPhysXManager->LateTick(fTimeDelta);
	m_pObject_Manager->LateTick(fTimeDelta);

	m_pCollision_Manager->LateTick(fTimeDelta);
	m_pLevel_Manager->LateTick(fTimeDelta);
}



void CGameInstance::Clear(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager)
		return;

	m_pObject_Manager->Clear(iLevelIndex);
}

_float CGameInstance::Compute_TimeDelta(TIMER_TYPE eTimerType)
{
	return m_pTimer_Manager->Compute_TimeDelta(eTimerType);
}

HRESULT CGameInstance::Set_TimeScale(TIMER_TYPE eTimerType, _float fTimeScale)
{
	return m_pTimer_Manager->Set_TimeScale(eTimerType, fTimeScale);
}

_float CGameInstance::Get_TimeScale(TIMER_TYPE eTimerType)
{
	return m_pTimer_Manager->Get_TimeScale(eTimerType);
}

_float CGameInstance::Get_TimeDelta(TIMER_TYPE eTimerType)
{
	return m_pTimer_Manager->Get_TimeDelta(eTimerType);
}

HRESULT CGameInstance::Set_Slow(TIMER_TYPE eTimerType, _float fSlowTime, _float fTimeScale, _bool bForce)
{
	return m_pTimer_Manager->Set_Slow(eTimerType, fSlowTime, fTimeScale, bForce);
}

HRESULT CGameInstance::Add_Timer(TIMER_TYPE eTimerType)
{
	return m_pTimer_Manager->Add_Timer(eTimerType);
}

CTimer* CGameInstance::Find_Timer(TIMER_TYPE eTimerType)
{
	return m_pTimer_Manager->Find_Timer(eTimerType);
}

ID3D11Device* CGameInstance::Get_Device()
{
	return m_pGraphic_Device->Get_Device();
}

ID3D11DeviceContext* CGameInstance::Get_Context()
{
	return m_pGraphic_Device->Get_Context();
}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	return m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{

	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	return m_pGraphic_Device->Present();
}

_char CGameInstance::Get_DIKState(_uchar eKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIKState(eKeyID);
}

_char CGameInstance::Get_DIMKeyState(DIMK eMouseKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMKeyState(eMouseKeyID);
}

_long CGameInstance::Get_DIMMoveState(DIMM eMouseMoveID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMMoveState(eMouseMoveID);
}

_bool CGameInstance::Mouse_Down(DIMK eMouseKeyID)
{
	return m_pInput_Device->Mouse_Down(eMouseKeyID);
}

_bool CGameInstance::Mouse_Up(DIMK eMouseKeyID)
{
	return m_pInput_Device->Mouse_Up(eMouseKeyID);
}

_bool CGameInstance::Mouse_Pressing(DIMK eMouseKeyID)
{
	return m_pInput_Device->Mouse_Pressing(eMouseKeyID);
}

HRESULT CGameInstance::Open_Level(_uint iLevelIndex, CLevel * pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;
	return m_pLevel_Manager->Open_Level(iLevelIndex, pNewLevel);
}

HRESULT CGameInstance::Render_Debug()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Render_Debug();
}

_uint CGameInstance::Get_CurrentLevel()
{
	return m_pLevel_Manager->Get_CurrentLevel();
}

HRESULT CGameInstance::Add_Prototype(const wstring& strPrototypeTag, CGameObject* pPrototype, _uint iLayerType, _bool bUseLock)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(strPrototypeTag, pPrototype, iLayerType, bUseLock);
}

HRESULT CGameInstance::Add_GameObject(_uint iLevelIndex, _uint iLayerType, const wstring & strPrototypeTag, void* pArg, __out class CGameObject** ppOut, _bool bUseLock)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject(iLevelIndex, iLayerType, strPrototypeTag, pArg, ppOut, bUseLock);
}

HRESULT CGameInstance::Add_GameObject(_uint iLevelIndex, const _uint iLayerType, CGameObject* pGameObject, _bool bUseLock)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject(iLevelIndex, iLayerType, pGameObject, bUseLock);
}

CGameObject* CGameInstance::Clone_GameObject(const wstring& strPrototypeTag, _uint iLayerType, void* pArg, _bool bUseLock)
{
	return m_pObject_Manager->Clone_GameObject(strPrototypeTag, iLayerType, pArg, bUseLock);
}

CGameObject* CGameInstance::Find_Prototype_GameObject(_uint iLayerType, const wstring& strPrototypeTag)
{
	return m_pObject_Manager->Find_Prototype_GameObject(iLayerType, strPrototypeTag);
}

const map<const wstring, class CGameObject*>& CGameInstance::Find_Prototype_GameObjects(_uint iLayerType)
{
	return m_pObject_Manager->Find_Prototype_GameObjects(iLayerType);
}

CGameObject* CGameInstance::Find_GameObject(_uint iLevelIndex, const _uint iLayerType, const wstring& strObjectTag)
{
	return m_pObject_Manager->Find_GameObject(iLevelIndex, iLayerType, strObjectTag);
}

CGameObject* CGameInstance::Find_GameObject(_uint iLevelIndex, const _uint iLayerType, _int iObjectID)
{
	return m_pObject_Manager->Find_GameObject(iLevelIndex, iLayerType, iObjectID);
}

list<CGameObject*>& CGameInstance::Find_GameObjects(_uint iLevelIndex, const _uint iLayerType)
{
	return m_pObject_Manager->Find_GameObjects(iLevelIndex, iLayerType);
}

HRESULT CGameInstance::Clear_Layer(_uint iLevelIndex, const _uint iLayerType)
{
	return m_pObject_Manager->Clear_Layer(iLevelIndex, iLayerType);
}



HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const wstring& strProtoTypeTag, CComponent* pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, strProtoTypeTag, pPrototype);
}

CComponent* CGameInstance::Find_Prototype_Component(_uint iLevelIndex, const wstring& strProtoTypeTag)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;
	return m_pComponent_Manager->Find_Prototype_Component(iLevelIndex, strProtoTypeTag);
}

map<const wstring, CComponent*>& CGameInstance::Find_Prototype_Components(_uint iLevelIndex)
{
	return m_pComponent_Manager->Find_Prototype_Components(iLevelIndex);
}


CComponent* CGameInstance::Clone_Component(_uint iLevelIndex, const wstring& strProtoTypeTag, CGameObject* pOwner, void* pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, strProtoTypeTag, pOwner, pArg);
}

HRESULT CGameInstance::Check_Prototype(_uint iLevelIndex, const wstring& strProtoTypeTag)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;
	return m_pComponent_Manager->Check_Prototype(iLevelIndex, strProtoTypeTag);
}

const LIGHTDESC* CGameInstance::Get_LightDesc(_uint iIndex)
{
	if (nullptr == m_pLight_Manager)
		return nullptr;

	return m_pLight_Manager->Get_LightDesc(iIndex);
}

list<class CLight*>* CGameInstance::Get_LightList()
{
	if (nullptr == m_pLight_Manager)
		return nullptr;

	return m_pLight_Manager->Get_LightList();
}

HRESULT CGameInstance::Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Add_Light(pDevice, pContext, LightDesc);
}

HRESULT CGameInstance::Add_ShadowLight(_uint iLevelIndex, _vector vEye, _vector vAt, _vector vUp)
{
	return m_pLight_Manager->Add_ShadowLight(iLevelIndex, vEye, vAt, vUp);
}

HRESULT CGameInstance::Add_Sun(CGameObject* pSun)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	if(FAILED(m_pLight_Manager->Add_Sun(pSun)))
		return E_FAIL;


	return S_OK;
}

_float4x4 CGameInstance::Get_ShadowViewMatrix(_uint iLevelIndex)
{
	return m_pLight_Manager->Get_ShadowLightViewMatrix(iLevelIndex);
}

_float4x4 CGameInstance::Get_ShadowLightViewMatrix_Inverse(_uint iLevelIndex)
{
	return m_pLight_Manager->Get_ShadowLightViewMatrix_Inverse(iLevelIndex);
}

HRESULT CGameInstance::Reset_Lights()
{
	return m_pLight_Manager->Reset_Lights();
}

HRESULT CGameInstance::Set_ShadowLight(_uint iLevelIndex, _vector vEye, _vector vAt, _vector vUp)
{
	return m_pLight_Manager->Set_ShadowLight(iLevelIndex, vEye, vAt, vUp);
}

HRESULT CGameInstance::Add_ShadowProj(_uint iLevelIndex, _float fFovAngleY, _float fAspectRatio, _float fNearZ, _float fFarZ)
{
	return m_pLight_Manager->Add_ShadowProj(iLevelIndex, fFovAngleY, fAspectRatio, fNearZ, fFarZ);;
}

_float4x4 CGameInstance::Get_ShadowProjMatrix(_uint iLevelIndex)
{
	return m_pLight_Manager->Get_ShadowLightProjMatrix(iLevelIndex);
}

void CGameInstance::Set_Transform(CPipeLine::TRANSFORMSTATE eTransformState, _fmatrix TransformMatrix)
{
	if (nullptr == m_pPipeLine)
		return;

	m_pPipeLine->Set_Transform(eTransformState, TransformMatrix);
}

_matrix CGameInstance::Get_TransformMatrix(CPipeLine::TRANSFORMSTATE eTransformState) const
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_TransformMatrix(eTransformState);
}

_float4x4 CGameInstance::Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATE eTransformState) const
{
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_TransformFloat4x4(eTransformState);

}

_float4x4 CGameInstance::Get_TransformFloat4x4_TransPose(CPipeLine::TRANSFORMSTATE eTransformState) const
{
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_TransformFloat4x4_TransPose(eTransformState);
}

_float4 CGameInstance::Get_CamPosition()
{
	if (nullptr == m_pPipeLine)
		return _float4();

	return m_pPipeLine->Get_CamPosition();
}

_float4x4 CGameInstance::Get_TransformMatrixInverse_Float4x4(CPipeLine::TRANSFORMSTATE eTransformState)
{
	return m_pPipeLine->Get_TransformMatrixInverse_Float4x4(eTransformState);
}

_matrix CGameInstance::Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE eTransformState) const
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_TransformMatrixInverse(eTransformState);
}

HRESULT CGameInstance::Bind_TransformToShader(CShader* pShader, const char* pConstantName, CPipeLine::TRANSFORMSTATE eState)
{
	return m_pPipeLine->Bind_TransformToShader(pShader, pConstantName, eState);
}

KEY_STATE CGameInstance::GetKeyState(KEY _eKey)
{
	if (nullptr == m_pKey_Manager)
		return KEY_STATE::NONE;

	return m_pKey_Manager->GetKeyState(_eKey);
}

const POINT& CGameInstance::GetMousePos()
{
	return m_pKey_Manager->GetMousePos();
}

void CGameInstance::Lock_Mouse()
{
	ShowCursor(false);
	m_pKey_Manager->Lock_Mouse();
}

void CGameInstance::UnLock_Mouse()
{
	ShowCursor(true);
	m_pKey_Manager->UnLock_Mouse();
}


HRESULT CGameInstance::Ready_Model_Data_FromPath(_uint iLevelIndex, _uint eType, const wstring& strFolderPath)
{
	return m_pModel_Manager->Ready_Model_Data_FromPath(iLevelIndex, eType, strFolderPath);
}

HRESULT CGameInstance::Export_Model_Data(CModel* pModel, const wstring& strSubFolderName, wstring strFileName)
{
	return m_pModel_Manager->Export_Model_Data(pModel, strSubFolderName, strFileName);
}

HRESULT CGameInstance::Import_Model_Data(_uint iLevelIndex, const wstring& strProtoTypeTag, _uint eType, wstring strFolderPath, wstring strFileName, __out class CModel** ppOut)
{
	return m_pModel_Manager->Import_Model_Data(iLevelIndex, strProtoTypeTag, eType, strFolderPath, strFileName, ppOut);
}

HRESULT CGameInstance::Export_Model_Data_FromPath(_uint eType, wstring strFolderPath)
{
	return m_pModel_Manager->Export_Model_Data_FromPath(eType, strFolderPath);
}


vector<ANIM_TRANSFORM_CACHE> CGameInstance::Create_AnimationSocketTransform(CModel* pModel, const _uint& iSocketBoneIndex)
{
	return m_pModel_Manager->Create_AnimationSocketTransform(pModel, iSocketBoneIndex);
}

vector<ANIM_TRANSFORM_CACHES> CGameInstance::Create_AnimationTransform_Caches_InTool(class CModel* pModel)
{
	return m_pModel_Manager->Create_AnimationTransform_Caches_InTool(pModel);
}

HRESULT CGameInstance::Add_Fonts(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strFontTag, const wstring& strFontFilePath)
{
	return m_pFont_Manager->Add_Fonts(pDevice, pContext, strFontTag, strFontFilePath);
}

HRESULT CGameInstance::Render_Fonts(const wstring& strFontTag, const wstring& strText, _float2 vPosition, _fvector vColor, _float fAngle, _float2 vOrigin, _float2 vScale)
{
	return m_pFont_Manager->Render_Fonts(strFontTag, strText, vPosition, vColor, fAngle, vOrigin, vScale);
}

HRESULT CGameInstance::Add_CollisionGroup(COLLISION_GROUP eCollisionGroup, CGameObject* pGameObject)
{
	return m_pCollision_Manager->Add_CollisionGroup(eCollisionGroup, pGameObject);
}

void CGameInstance::Reset_CollisionGroup()
{
	m_pCollision_Manager->Reset();
}

_bool CGameInstance::Intersect_Frustum_World(_fvector vWorldPos, _float fRadius)
{
	return m_pFrustum->Intersect_Frustum_World(vWorldPos, fRadius);
}

//PxRigidStatic* CGameInstance::Add_Static_Actor(const PHYSX_INIT_DESC& Desc)
//{
//	return m_pPhysXManager->Add_Static_Actor(Desc);
//}
//
//PxRigidDynamic* CGameInstance::Add_Dynamic_Actor(const PHYSX_INIT_DESC& Desc)
//{
//	return m_pPhysXManager->Add_Dynamic_Actor(Desc);
//}
//
//HRESULT CGameInstance::Add_Static_Mesh_Actor(const PHYSX_INIT_DESC& Desc, __out vector<PxRigidStatic*>& refOut)
//{
//	return m_pPhysXManager->Add_Static_Mesh_Actor(Desc, refOut);
//}
//
//HRESULT CGameInstance::Add_Dynamic_Mesh_Actor(const PHYSX_INIT_DESC& Desc, __out vector<PxRigidDynamic*>& refOut)
//{
//	return m_pPhysXManager->Add_Dynamic_Mesh_Actor(Desc, refOut);
//}

HRESULT CGameInstance::Add_Building(CGameObject* pGameObject, CModel* pModel, Matrix WorldMatrix, const wstring& strCollisionTag)
{
	return m_pPhysXManager->Add_Building(pGameObject, pModel, WorldMatrix, strCollisionTag);
}

HRESULT CGameInstance::Add_Ground(CGameObject* pGameObject, CModel* pModel, _matrix WorldMatrix, const wstring& strCollisionTag)
{
	return m_pPhysXManager->Add_Ground(pGameObject, pModel, WorldMatrix, strCollisionTag);
}

HRESULT CGameInstance::Remove_Actor(class CGameObject* pGameObject)
{
	return m_pPhysXManager->Remove_Actor(pGameObject);
}

HRESULT CGameInstance::Remove_Controller(PxController* pController)
{
	return m_pPhysXManager->Remove_Controller(pController);
}

_bool CGameInstance::Is_PhysX_Valid()
{
	return m_pPhysXManager->Is_PhysX_Valid();
}




//HRESULT CGameInstance::Convert_Transform(CGameObject* pObj, PxTransform& PxTransform)
//{
//	return m_pPhysXManager->Convert_Transform(pObj, PxTransform);
//}


//
//PxMaterial* CGameInstance::Create_PxMaterial(_float fStaticFriction, _float fDynamicFriction, _float fRestitution)
//{
//	return m_pPhysXManager->Create_Material(fStaticFriction, fDynamicFriction, fRestitution);
//}

PxController* CGameInstance::Add_CapsuleController(CGameObject* pGameObject, Matrix WorldMatrix, _float fHeight, _float fRadius, _float3 vOffsetPos, _float fMaxJumpHeight, PxUserControllerHitReport* pCallBack)
{
	return m_pPhysXManager->Add_CapsuleController(pGameObject, WorldMatrix, fHeight, fRadius, vOffsetPos, fMaxJumpHeight, pCallBack);
}

PxController* CGameInstance::Add_BoxController(CGameObject* pGameObject, Matrix WorldMatrix, _float3 fExtents, _float3 vOffsetPos, _float fMaxJumpHeight, PxUserControllerHitReport* pCallBack)
{
	return m_pPhysXManager->Add_BoxController(pGameObject, WorldMatrix, fExtents, vOffsetPos, fMaxJumpHeight, pCallBack);
}

HRESULT CGameInstance::Clear_PhysX_Ground()
{
	return m_pPhysXManager->Clear_PhysX_Ground();
}

//PxParticleClothBuffer* CGameInstance::Get_TestClothBuffer()
//{
//	return m_pPhysXManager->Get_ClothBuffer();
//}
//
//PxCudaContextManager* CGameInstance::Get_CudaContext_Manager()
//{
//	return m_pPhysXManager->Get_CudaContext_Manager();
//}
//
//PxCudaContext* CGameInstance::Get_CudaContext()
//{
//	return m_pPhysXManager->Get_CudaContext();
//}
//
//PxParticleSystem* CGameInstance::Get_ParticleSystem()
//{
//	return m_pPhysXManager->Get_ParticleSystem();
//}

//PxParticleClothBuffer* CGameInstance::Get_TestClothBuffer()
//{
//	return m_pPhysXManager->Get_ClothBuffer();
//}
//
//PxCudaContextManager* CGameInstance::Get_CudaContext_Manager()
//{
//	return m_pPhysXManager->Get_CudaContext_Manager();
//}
//
//
//PxCudaContext* CGameInstance::Get_CudaContext()
//{
//	return m_pPhysXManager->Get_CudaContext();
//}
//
//PxParticleSystem* CGameInstance::Get_ParticleSystem()
//{
//	return m_pPhysXManager->Get_ParticleSystem();
//}




PxTransform CGameInstance::To_PxTransform(Matrix matrix)
{
	return CUtils::To_PxTransform(matrix);
}

Matrix CGameInstance::To_Matrix(PxTransform pxTransform)
{
	return CUtils::To_Matrix(pxTransform);
}

wstring CGameInstance::To_Wstring(const string& str)
{
	return	CUtils::ToWString(str);
}

string CGameInstance::To_String(const wstring& str)
{
	return	CUtils::ToString(str);
}

_float CGameInstance::RandomFloat(_float fMin, _float fMax)
{
	return	CUtils::Random_Float(fMin, fMax);
}

_int CGameInstance::RandomInt(_int iMin, _int iMax)
{
	return	CUtils::Random_Int(iMin, iMax);
}

Matrix CGameInstance::To_RightHanded(Matrix matLeftHanded)
{
	return CUtils::To_RightHanded(matLeftHanded);
}

Matrix CGameInstance::To_LeftHanded(Matrix matRightHanded)
{
	return CUtils::To_LeftHanded(matRightHanded);
}

Vec4 CGameInstance::To_RightHanded(Vec4 vLeftHanded)
{
	return CUtils::To_RightHanded(vLeftHanded);
}

Vec4 CGameInstance::To_LeftHanded(Vec4 vRightHanded)
{
	return CUtils::To_LeftHanded(vRightHanded);
}

Vec3 CGameInstance::To_RightHanded(Vec3 vLeftHanded)
{
	return CUtils::To_RightHanded(vLeftHanded);
}

Vec3 CGameInstance::To_LeftHanded(Vec3 vRightHanded)
{
	return CUtils::To_LeftHanded(vRightHanded);
}

_bool CGameInstance::Is_Compare(const char* szLeft, const char* szRight)
{
	return CUtils::Is_Compare(szLeft, szRight);
}

Json CGameInstance::Json_Load(const wstring& strFilePath)
{
	return CJsonUtils::Json_Load(strFilePath);
}

HRESULT CGameInstance::Json_Save(const wstring& strFilePath, const Json& refJsonFile)
{
	return CJsonUtils::Json_Save(strFilePath, refJsonFile);
}

void CGameInstance::Play_Sound(const wstring& pSoundKey, CHANNELID eID, _float fVolume, _bool bStop)
{
	m_pSound_Manager->Play_Sound(pSoundKey, eID, fVolume, bStop);
}

void CGameInstance::Play_BGM(const wstring& pSoundKey, _float fVolume, _bool bStop)
{
	m_pSound_Manager->Play_BGM(pSoundKey, fVolume, bStop);
}

void CGameInstance::Stop_Sound(CHANNELID eID)
{
	m_pSound_Manager->Stop_Sound(eID);
}

void CGameInstance::Stop_All()
{
	m_pSound_Manager->Stop_All();
}

void CGameInstance::Set_ChannelVolume(CHANNELID eID, float fVolume)
{
	m_pSound_Manager->Set_ChannelVolume(eID, fVolume);
}

FMOD_CHANNEL* CGameInstance::Get_Channel(CHANNELID eID)
{
	return m_pSound_Manager->Get_Channel(eID);
}

const _int CGameInstance::Get_SoundFileIndex(const wstring& pSoundKey)
{
	return m_pSound_Manager->Get_SoundFileIndex(pSoundKey);
}

wstring CGameInstance::Get_SoundFileKey(const _uint iIndex)
{
	return m_pSound_Manager->Get_SoundFileKey(iIndex);
}

const map<wstring, FMOD_SOUND*>& CGameInstance::Get_MapSound()
{
	return m_pSound_Manager->Get_MapSound();
}

_float CGameInstance::Get_ChannelVolume(CHANNELID eID)
{

	return m_pSound_Manager->Get_ChannelVolume(eID);
}

HRESULT CGameInstance::Bind_SRV(CShader* pShader, const wstring& strTargetTag, const _char* pConstantName)
{
	return m_pTarget_Manager->Bind_SRV(pShader, strTargetTag, pConstantName);
}

//void CGameInstance::Set_ServerSession(ServerSessionRef session)
//{
//	m_pNetwork_Manager->Set_ServerSession(session);
//}
//
//void CGameInstance::Send(SendBufferRef sendBuffer)
//{
//	m_pNetwork_Manager->Send(sendBuffer);
//}
//
//bool CGameInstance::Is_Connected()
//{
//	return m_pNetwork_Manager->Is_Connected();
//}
//
//ServerSessionRef& CGameInstance::Get_ServerSession()
//{
//	// TODO: 여기에 return 문을 삽입합니다.
//	return m_pNetwork_Manager->Get_ServerSession();
//}







void CGameInstance::Release_Engine()
{
	CGameInstance::GetInstance()->DestroyInstance();
	CTarget_Manager::GetInstance()->DestroyInstance();
	CKey_Manager::GetInstance()->DestroyInstance();
	CInput_Device::GetInstance()->DestroyInstance();
	CSound_Manager::GetInstance()->DestroyInstance();
	CLevel_Manager::GetInstance()->DestroyInstance();
	CTimer_Manager::GetInstance()->DestroyInstance();
	CFrustum::GetInstance()->DestroyInstance();
	CPipeLine::GetInstance()->DestroyInstance();
	CLight_Manager::GetInstance()->DestroyInstance();
	CFont_Manager::GetInstance()->DestroyInstance();
	CModel_Manager::GetInstance()->DestroyInstance();
	CPhysX_Manager::GetInstance()->DestroyInstance();
	CCollision_Manager::GetInstance()->DestroyInstance();
	CObject_Manager::GetInstance()->DestroyInstance();
	CComponent_Manager::GetInstance()->DestroyInstance();
	CGraphic_Device::GetInstance()->DestroyInstance();
	CCamera_Manager::GetInstance()->DestroyInstance();
}

void CGameInstance::Free()
{
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pKey_Manager);
	Safe_Release(m_pModel_Manager);
	Safe_Release(m_pCollision_Manager);
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pFrustum);
	Safe_Release(m_pSound_Manager);
	Safe_Release(m_pPhysXManager);
	Safe_Release(m_pCamera_Manager);
}
