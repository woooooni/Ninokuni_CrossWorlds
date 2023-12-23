#pragma once


#include "Component.h"
BEGIN(Engine)

class ENGINE_DLL CPhysX_Controller : public CComponent, public PxUserControllerHitReport
{
public:
    enum CONTROLLER_TYPE { BOX, CAPSULE, TYPE_END };
public:
    typedef struct tagPxController
    {
        class CTransform* pTransform;
        class CGameObject* pOwner;
        
        // Box
        Vec3 vExtents = { 1.f, 1.f, 1.f };

        // Capsule
        _float fHeight = { 0.f };
        _float fRaidus = { 0.f };
        _float fMaxJumpHeight = { 10.f };

        Vec3 vOffset = { 0.f, 0.f ,0.f };


        CONTROLLER_TYPE eType = CONTROLLER_TYPE::TYPE_END;
    } CONTROLLER_DESC;

private:
    CPhysX_Controller(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CPhysX_Controller(CPhysX_Controller& rhs);
    virtual ~CPhysX_Controller() = default;


public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);

public:
    void Tick_Controller(_float fTimeDelta);
    void LateTick_Controller(_float fTimeDelta);
    

private:
    class PxController* m_pPhysXController = nullptr;
    class CTransform* m_pTransformCom = nullptr;

    class PxControllerFilters m_Filters = {};
    PxFilterData m_FilterData = {};

private:
    CONTROLLER_TYPE m_eType = CONTROLLER_TYPE::TYPE_END;
    Vec3 m_vOffsetPos = {};
    Vec3 m_vPrevPosition = {};
    _bool m_bInitialize = true;
    _bool m_bGroundChecked = false;

public:
    static CPhysX_Controller* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CComponent* Clone(void* pArg);
    virtual void Free() override;
    
private:
    PxPairFlag::Enum m_eGroundFlag = PxPairFlag::eCONTACT_DEFAULT;
    

public:
    // PxUserControllerHitReport을(를) 통해 상속됨
    virtual void onShapeHit(const PxControllerShapeHit& hit) override;
    virtual void onControllerHit(const PxControllersHit& hit) override;
    virtual void onObstacleHit(const PxControllerObstacleHit& hit) override;

};
END


//public:
//    void Set_Gravity(_float fGravity) { m_fGravity = fGravity; }
//    void Set_Gravity(_bool bGravity)
//    {
//        if (true == bGravity)
//            m_fGravity = 14.7f;
//        else
//            m_fGravity = 0.f;
//
//        m_bIsGravity = bGravity;
//    }
//
//    _bool Is_Gravity() { return m_bIsGravity; }
//
//    _bool Is_Ground() { return m_bIsGround; }
//    void Set_Ground(_bool bGround) { m_bIsGround = bGround; }
//
//    void Set_PushVelocity(_fvector vPushVelocity, _float fTimeDelta);
//
//    void Add_Velocity(_fvector vVelocity);
//    void Add_Velocity(_fvector vDir, _float fForce);
//
//    void Add_Velocity_Acc(_fvector vVelocity);
//    void Add_Velocity_Acc(_fvector vDir, _float fForce);
//
//    void Set_RefHeight(_float fHeight) { m_fRefHeight = fHeight; }
//    _float Get_RefHeight() { return m_fRefHeight; }
//
//    _float3 Get_Velocity() { return m_vVelocity; }
//    void Set_Velocity(_float3 vVelocity) { m_vVelocity = vVelocity; }
//
//    void Set_Friction_Scale(_float fScale) { m_fFrictionScale = fScale; }
//
//private:
//    void Update_Gravity(_float fTimeDelta);
//    void Update_Velocity(_float fTimeDelta);
//
//private:
//    _float m_fGravity = 14.7f;               // 중력 계수
//    _float3 m_vVelocity = { 0.f, 0.f, 0.f };    // 현재 속도
//
//    _float3 m_vMaxVelocity = { 100.f, 100.f, 100.f }; // 속도 최대치.
//    _float3 m_vFriction = { 0.f, 0.f, 0.f };    // 현재 속도에 대응하는 마찰력.
//
//    _bool m_bIsGround = false;   // 현재 땅에 있는가?
//    _bool m_bIsGravity = true;  // 중력의 영향을 받는가?
//
//    _float m_fFrictionScale = 20.f; // 마찰력 크기.
//    _float m_fRefHeight = 0.f; // 기준 Y