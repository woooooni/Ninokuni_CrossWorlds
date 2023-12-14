#pragma once
#include "Component.h"



BEGIN(Engine)

class ENGINE_DLL CRigidBody : public CComponent
{
public:
    typedef struct tagRigidBodyDesc
    {
        PHYSX_INIT_DESC PhysXDesc = {};
        class CTransform* pTransform = nullptr;
        class CNavigation* pNavigation = nullptr;
    } RIGID_BODY_DESC;

private:
    CRigidBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CRigidBody(CRigidBody& rhs);
    virtual ~CRigidBody() = default;


public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);

#ifdef _DEBUG
    virtual HRESULT Render() override;
#endif

public:
    void Update_RigidBody(_float fTimeDelta);

public:
    void Set_PhysXBody(PxRigidDynamic* pDynamicBody) { m_pXBody = pDynamicBody; }

    void Set_Sleep(_bool bSleep);
    _bool Is_Sleep();

public:
    void Add_Force(_vector vDir, _float fForce, _bool bClear);
    void Add_Velocity(_vector vDir, _float fForce, _bool bClear);

    void Set_Velocity(_float3 vVelocity);
    _float3 Get_Velocity();

    

#ifdef _DEBUG
protected:
    PrimitiveBatch<VertexPositionColor>* m_pBatch = nullptr;
    BasicEffect* m_pEffect = nullptr;
    ID3D11InputLayout* m_pInputLayout = nullptr;
    _bool m_bCollision = false;


private:
    BoundingOrientedBox* m_pOBB = nullptr;
    BoundingOrientedBox* m_pOriginal_OBB = nullptr;
    BoundingSphere* m_pSphere = nullptr;
    BoundingSphere* m_pOriginal_Sphere = nullptr;


#endif

private:
    _bool m_bFirst = false;
    PHYSX_COLLIDER_TYPE m_eColliderType = PHYSX_COLLIDER_TYPE::COLLIDER_TYPE_END;
    PHYSX_RIGID_TYPE m_eRigidType = PHYSX_RIGID_TYPE::RIGID_TYPE_END;

private:
    class CTransform* m_pTransformCom = nullptr;
    class CNavigation* m_pNavigationCom = nullptr;
    class PxRigidDynamic* m_pXBody = nullptr;




public:
    static CRigidBody* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CComponent* Clone(void* pArg);
    virtual void Free() override;

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