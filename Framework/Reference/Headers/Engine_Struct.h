#include <tchar.h>
#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagObjectInitDesc
	{
		Vec4 vStartPosition = { 0.f, 0.f, 0.f, 1.f };
		Vec4 vCenterPos = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
		vector<Vec4> vRomingPoints;
	} OBJECT_INIT_DESC;

	typedef struct tagCollisionInfoDesc
	{
		class CCollider* pMyCollider;

		class CGameObject* pOther;
		class CCollider* pOtherCollider;
	} COLLISION_INFO;

	typedef struct tagPhysXGroundCollisionDesc
	{
		Vec3 vCollision_Position = {};
		Vec3 vNormal = {};
		CGameObject* pCollideObject = nullptr;
		PxPairFlag::Enum flag = PxPairFlag::eCONTACT_DEFAULT;

	}PHYSX_GROUND_COLLISION_INFO;

	typedef struct tagPhysXControllerGroundCollisionDesc
	{
		PxPairFlag::Enum eFlag = PxPairFlag::Enum::eNOTIFY_TOUCH_CCD;
		_bool bChecked = false;
	} PHYSX_CONTROLLER_GROUND;

	typedef struct tagKeyFrame
	{
		float		fTime;

		XMFLOAT3	vScale;
		XMFLOAT4	vRotation;
		XMFLOAT3	vPosition;

	}KEYFRAME;


	



	/* 빛의 정보를 담아놓기 위한 구조체. */
	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_POINT, TYPE_DIRECTIONAL, TYPE_SPOT, TYPE_END };

		TYPE			eType;

		XMFLOAT4		vDirection;

		XMFLOAT4		vPosition;
		float			fRange;

		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;


		// 개조 전에 임시
		Vec3 vTempPosition;
		Vec3 vTempDirection;
		Vec3 vAmbientLowerColor;
		Vec3 vAmbientUpperColor;

		_float fTempRange = 0.0f;
		_float fTempLength = 0.0f;
		_float fTempInnerAngle = 0.0f;
		Vec3 vTempColor;
	}LIGHTDESC;

	typedef struct tagMaterialDesc
	{
		wchar_t			strName[MAX_PATH];

		XMFLOAT4		vAmbient;
		XMFLOAT4		vDiffuse;
		XMFLOAT4		vSpecular;
		XMFLOAT4		vEmissive;
		class CTexture* pTexture[AI_TEXTURE_TYPE_MAX];
	}MATERIALDESC;

	typedef struct tagLineIndices16
	{
		unsigned short		_0, _1;
	}LINEINDICES16;

	typedef struct tagLineIndices32
	{
		unsigned long		_0, _1;
	}LINEINDICES32;

	typedef struct tagFaceIndices16
	{
		unsigned short		_0, _1, _2;
	}FACEINDICES16;

	typedef struct tagFaceIndices32
	{
		unsigned long		_0, _1, _2;
	}FACEINDICES32;

	///////////////
	/// VtxType ///
	///////////////
	// D3DDECLUSAGE
	typedef struct tagVertexPoint
	{
		XMFLOAT3			vPosition;
		XMFLOAT2			vSize;
	} VTXPOINT;

	typedef struct ENGINE_DLL tagVertexPoint_Declaration
	{
		static const unsigned int iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	} VTXPOINT_DECLARATION;


	typedef struct tagVertexPoistion
	{
		XMFLOAT3			vPosition;
	} VTXPOS;

	typedef struct ENGINE_DLL tagVertex_Position_Declaration
	{
		static const unsigned int				iNumElements = 1;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	} VTXPOS_DECLARATION;




	typedef struct tagVertexTexture
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;
	}VTXPOSTEX;

	typedef struct ENGINE_DLL tagVertexTexture_Declaration
	{
		static const unsigned int iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXTEX_DECLARATION;

	typedef struct tagVertexCubeTexture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexture;
	}VTXCUBETEX;

	typedef struct ENGINE_DLL tagVertexCubeTexture_Declaration
	{
		static const unsigned int iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXCUBETEX_DECLARATION;



	typedef struct tagVertexNormalTexture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertexNormalTexture_Declaration
	{
		static const unsigned int iNumElements = 3;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXNORTEX_DECLARATION;

	typedef struct tagVertexModel
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexture;
		XMFLOAT3		vTangent;
	} VTXMODEL;

	typedef struct ENGINE_DLL tagVertexModel_Declaration
	{
		static const unsigned int iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	} VTXMODEL_DECLARATION;

	typedef struct tagVertexAnimModel
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexture;
		XMFLOAT3		vTangent;
		XMUINT4			vBlendIndex; /* 이 정점에 영향을 주는 뼈의 인덱스 네개. */
		XMFLOAT4		vBlendWeight; /* 영향르 주고 있는 각 뼈대의 영향 비율 */
	}VTXANIMMODEL;

	typedef struct ENGINE_DLL tagVertexAnimModel_Declaration
	{
		static const unsigned int iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXANIMMODEL_DECLARATION;



	typedef struct tagVertexColor
	{
		XMFLOAT3		vPosition;
		XMFLOAT4		vColor;
	} VTXCOL;

	typedef struct ENGINE_DLL tagVertexColor_Declaration
	{
		static const unsigned int iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXCOL_DECLARATION;

	typedef struct tagVertexInstance
	{
		XMFLOAT4		vRight;
		XMFLOAT4		vUp;
		XMFLOAT4		vLook;
		XMFLOAT4		vPosition;
	} VTXINSTANCE;

	typedef struct ENGINE_DLL tagVertexRectInstance_Declaration
	{
		/* 내가 그릴려고 했던 정점(VTXTEX)과 해당 모델의 로컬 상탤르 ㄹ표현하는
		VTXINSTANCE의 정보가 함께 셰이더로 전달되어야한다. */
		static const unsigned int iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	} VTXRECTINSTANCE_DECLARATION;

	typedef struct tagVertexPointInstance
	{
		XMFLOAT3			vPosition;
		XMFLOAT2			vSize;
	} VTXPOINT_INSTANCE;

	typedef struct ENGINE_DLL tagVertexModelInstance_Declaration
	{
		/* 내가 그릴려고 했던 정점(VTXTEX)과 해당 모델의 로컬 상탤르 ㄹ표현하는
		VTXINSTANCE의 정보가 함께 셰이더로 전달되어야한다. */
		static const unsigned int iNumElements = 8;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	} VTXMODELINSTANCE_DECLARATION;

	typedef struct ENGINE_DLL tagVertexAnimModelInstance_Declaration
	{
		/* 내가 그릴려고 했던 정점(VTXTEX)과 해당 모델의 로컬 상탤르 ㄹ표현하는
		VTXINSTANCE의 정보가 함께 셰이더로 전달되어야한다. */
		static const unsigned int iNumElements = 10;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	} VTXANIMMODELINSTANCE_DECLARATION;

	typedef struct ENGINE_DLL tagVertexPointInstance_Declaration
	{
		/* 내가 그릴려고 했던 정점(VTXTEX)과 해당 모델의 로컬 상탤르 ㄹ표현하는
		VTXINSTANCE의 정보가 함께 셰이더로 전달되어야한다. */
		static const unsigned int iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXPOINTINSTANCE_DECLARATION;


	typedef struct tagGraphicDesc
	{
		enum WINMODE { WINMODE_FULL, WINMODE_WIN, WINMODE_END };

		HWND		hWnd;
		WINMODE		eWinMode;
		unsigned int	iWinSizeX, iWinSizeY;

	} GRAPHIC_DESC;



#pragma region PhysXDesc
	typedef struct tagPhysXDesc
	{
		PHYSX_RIGID_TYPE eRigidType = PHYSX_RIGID_TYPE::RIGID_TYPE_END;					// 오브젝트의 (스태틱, 다이나믹, 그라운드 등)타입을 결정합니다.
		PHYSX_COLLIDER_TYPE eColliderType = PHYSX_COLLIDER_TYPE::COLLIDER_TYPE_END;		// 오브젝트의 콜라이더 모양(박스, 구, 메시)을 결정합니다.

		// 콜라이더 관련.
		_float3 vStartPosition = { 0.f, 0.f, 0.f };
		_float3 vExtents = { 1.f, 1.f, 1.f };			// 상자의 너비
		_float fRadius = 1.f;							// 구의 너비


		// 물리 관련.
		_float fStaticFriction = 0.f;			// 스태틱 오브젝트의 마찰.
		_float fDynamicFriction = 0.f;			// 다이나믹 오브젝트의 마찰.
		_float fRestitution = 0.f;				// 반발력 혹은 탄성.		
		_float fDensity = 1.f;					// 질량(무게) & 밀도입니다.
		_float fMaxVelocity = 10000.f;				// 최대 속도입니다.
		_float fAngularDamping = 1.f;			// 회전을 방해하는 힘입니다.

		_bool bKinematic = false;				// 키네마틱이 꺼진 오브젝트는 켜져있는 오브젝트에게 밀려납니다. 키네마틱이 켜져있다면, 밀리지않으며 중력에도 영향받지 않습니다.


		// 회전 관성 관련.
		_bool bLockAngle_X = true;				// x축 회전을 제한할지 선택합니다.
		_bool bLockAngle_Y = true;				// y축 회전을 제한할지 선택합니다.
		_bool bLockAngle_Z = true;				// z축 회전을 제한할지 선택합니다.


		Matrix WorldMatrix = Matrix::Identity;
		class CGameObject* pGameObject = nullptr;										// 콜라이더의 주인 입니다.
	} PHYSX_INIT_DESC;



	// 피직스 매니저에 저장되는 스태틱 액터 구조체.
	typedef struct tagPhysXStaticObjectDesc
	{
		class CGameObject* pObject = nullptr;
		class PxRigidStatic* pActor = nullptr;
	} PHYSX_STATIC_OBJECT_DESC;

	// 피직스 매니저에 저장되는 다이나믹 액터 구조체.
	typedef struct tagPhysXDynamicObjectDesc
	{
		class CGameObject* pObject = nullptr;
		class PxRigidDynamic* pActor = nullptr;
	} PHYSX_DYNAMIC_OBJECT_DESC;
#pragma endregion

#pragma region Lerp Desc

	enum class LERP_MODE { DEFAULT, EASE_OUT, EASE_IN, EXPONENTIAL, SMOOTH_STEP, SMOOTHER_STEP, TYPEEND };
	static string LerpModeNames[(UINT)LERP_MODE::TYPEEND] = { "DEFAULT", "EASE_OUT", "EASE_IN", "EXPONENTIAL", "SMOOTH_STEP", "SMOOTHER_STEP" };

	typedef struct tagLerpDesc
	{
		const _float Calculate_Time(const _float& fCurTime, const _float& fEndTime, const LERP_MODE& eLerpMode)
		{
			_float fLerpTime = fCurTime / fEndTime;

			switch (eLerpMode)
			{
			case LERP_MODE::EASE_OUT:
			{
				fLerpTime = sinf(fLerpTime * XM_PI * 0.5f);
			}
			break;
			case LERP_MODE::EASE_IN:
			{
				fLerpTime = 1.f - cosf(fLerpTime * XM_PI * 0.5f);
			}
			break;
			case LERP_MODE::EXPONENTIAL:
			{
				fLerpTime = fLerpTime * fLerpTime;
			}
			break;
			case LERP_MODE::SMOOTH_STEP:
			{
				fLerpTime = fLerpTime * fLerpTime * (3.f - 2.f * fLerpTime);

			}
			break;
			case LERP_MODE::SMOOTHER_STEP:
			{
				fLerpTime = fLerpTime * fLerpTime * fLerpTime * (fLerpTime * (6.f * fLerpTime - 15.f) + 10.f);
			}
			break;
			default:
				break;
			}

			return fLerpTime;
		}

	}LERP_DESC;

	typedef struct tagLerpFloatDesc : public LERP_DESC
	{
		/* Time */
		_float		fStartTime = 0.f;
		_float		fEndTime = 0.f;
		_float		fCurTime = 0.f;

		/* Value */
		_float		fStartValue = 0.f;
		_float		fTargetValue = 0.f;
		_float		fCurValue = 0.f;

		/* Prop */
		_bool		bActive = FALSE;
		LERP_MODE	eMode = LERP_MODE::DEFAULT;

		void Start(const _float _fStartValue, const _float _fTargetValue, const _float _fTime, const LERP_MODE _eMode = LERP_MODE::DEFAULT)
		{
			if (_fTime < 0)
				return;

			bActive = TRUE;

			fCurTime = 0.f;
			fEndTime = _fTime;

			fStartValue = fCurValue = _fStartValue;
			fTargetValue = _fTargetValue;

			eMode = _eMode;
		}
		void Start(const _float _fTargetValue, const _float _fTime, const LERP_MODE _eMode = LERP_MODE::DEFAULT)
		{
			if (_fTime < 0)
				return;

			bActive = TRUE;

			fCurTime = 0.f;
			fEndTime = _fTime;

			fStartValue = fCurValue;
			fTargetValue = _fTargetValue;

			eMode = _eMode;
		}

		void Clear()
		{
			bActive = FALSE;

			fStartTime = fCurTime = fEndTime = 0.f;

			fStartValue = fCurValue = fTargetValue = 0.f;

			eMode = LERP_MODE::DEFAULT;
		}

		_float Update(const _float& fTimeDelta)
		{
			if (!bActive) 
				return fCurValue;

			fCurTime += fTimeDelta;

			if (fCurTime >= fEndTime)
			{
				fCurValue = fTargetValue;
				bActive = FALSE;
				return fCurValue;
			}

			const _float fTime = LERP_DESC::Calculate_Time(fCurTime, fEndTime, eMode);

			fCurValue = Lerp_Float(fStartValue, fTargetValue, fTime);

			return fCurValue;
		}

		_float Lerp_Float(const _float& _f1, const _float& _f2, const _float _fTime) { return (1 - _fTime) * _f1 + (_fTime * _f2); }

	}LERP_FLOAT_DESC;

	typedef struct tagLerpTimeDesc : public LERP_DESC
	{
		/* Time */
		_float		fStartTime = 0.f;
		_float		fEndTime = 0.f;
		_float		fCurTime = 0.f;

		/* Value */
		_float		fLerpTime = 0.f;

		/* Prop */
		_bool		bActive = FALSE;
		LERP_MODE	eMode = LERP_MODE::DEFAULT;

		void Start(const _float _fTime, const LERP_MODE _eMode = LERP_MODE::DEFAULT)
		{
			if (_fTime < 0)
				return;

			bActive = TRUE;

			fCurTime = 0.f;
			fEndTime = _fTime;

			eMode = _eMode;
		}

		_float Update(const _float& fTimeDelta)
		{
			if (!bActive)
				return fLerpTime;

			fCurTime += fTimeDelta;

			if (fCurTime >= fEndTime)
			{
				fLerpTime = fCurTime = fEndTime;
				bActive = FALSE;

				return fLerpTime;
			}

			const _float fTime = LERP_DESC::Calculate_Time(fCurTime, fEndTime, eMode);

			fLerpTime = Lerp_Float(fStartTime, fEndTime, fTime);

			return fLerpTime;
		}

		_float Lerp_Float(const _float& _f1, const _float& _f2, const _float _fTime) { return (1 - _fTime) * _f1 + (_fTime * _f2); }
	}LERP_TIME_DESC;

	typedef struct tagLerpVec3Desc : public LERP_DESC
	{
		/* Time */
		_float		fStartTime = 0.f;
		_float		fEndTime = 0.f;
		_float		fCurTime = 0.f;

		/* Value */
		Vec3		vStartVec;
		Vec3		vTargetVec;
		Vec3		vCurVec;

		/* Prop */
		_bool		bActive = FALSE;
		LERP_MODE	eMode = LERP_MODE::DEFAULT;

		void Start(const Vec3 _fStartValue, const Vec3& _fTargetValue, const _float& _fTime, const LERP_MODE& _eMode = LERP_MODE::DEFAULT)
		{
			if (_fTime < 0)
				return;

			bActive = TRUE;

			fCurTime = 0.f;
			fEndTime = _fTime;

			vStartVec = vCurVec = _fStartValue;
			vTargetVec = _fTargetValue;

			eMode = _eMode;
		}

		void Clear()
		{
			bActive = FALSE;

			fStartTime = fCurTime = fEndTime = 0.f;

			vStartVec = vCurVec = vTargetVec = Vec3::Zero;

			eMode = LERP_MODE::DEFAULT;
		}

		Vec3 Update_Lerp(const _float& fTimeDelta)
		{
			if (!bActive) 
				return vCurVec;

			fCurTime += fTimeDelta;

			if (fCurTime >= fEndTime)
			{
				vCurVec = vTargetVec;
				bActive = FALSE;
				return vCurVec;
			}

			const _float fTime = LERP_DESC::Calculate_Time(fCurTime, fEndTime, eMode);

			vCurVec = Vec3::Lerp(vStartVec, vTargetVec, fTime);

			return vCurVec;
		}
	}LERP_VEC3_DESC;

	typedef struct tagLerpVec4Desc : public LERP_DESC
	{
		/* Time */
		_float		fStartTime = 0.f;
		_float		fEndTime = 0.f;
		_float		fCurTime = 0.f;

		/* Value */
		Vec4		vStartVec;
		Vec4		vTargetVec;
		Vec4		vCurVec;

		/* Prop */
		_bool		bActive = FALSE;
		LERP_MODE	eMode = LERP_MODE::DEFAULT;

		void Start(const Vec4 _fStartValue, const Vec4& _fTargetValue, const _float& _fTime, const LERP_MODE& _eMode = LERP_MODE::DEFAULT)
		{
			if (_fTime < 0)
				return;

			bActive = TRUE;

			fCurTime = 0.f;
			fEndTime = _fTime;

			vStartVec = vCurVec = _fStartValue;
			vTargetVec = _fTargetValue;

			eMode = _eMode;
		}

		Vec4 Update_Lerp(const _float& fTimeDelta)
		{
			if (!bActive)
				return vCurVec;

			fCurTime += fTimeDelta;

			if (fCurTime >= fEndTime)
			{
				vCurVec = vTargetVec;
				bActive = FALSE;
				return vCurVec;
			}

			const _float fTime = LERP_DESC::Calculate_Time(fCurTime, fEndTime, eMode);

			vCurVec = Vec4::Lerp(vStartVec, vTargetVec, fTime);

			return vCurVec;
		}
	}LERP_VEC4_DESC;


#pragma endregion

#pragma region TweenDesc
	typedef struct	KeyframeDesc
	{
		/* 16 byte Alignment */
		_int	iAnimIndex = -1;		/* 현재 애니메이션 인덱스*/
		_uint	iCurFrame = 0;		/* 애니메이션의 현재 프레임 */
		_uint	iNextFrame = 1;		/* 애니메이션의 다음 프레임 */
		_float	fRatio = 0.f;		/* 애니메이션의 현재 프레임과 다음 프레임 사이 진행률*/
		_float	fFrameAcc = 0.f;	/* 현재 프레임 누적 시간 */

		_uint	iFinish = 0;
		_uint	iFix = 0;
		_uint	iStop = 0;

		void ClearAnim()
		{
			iCurFrame = 0;
			iNextFrame = 1;
			fRatio = 0.f;
			fFrameAcc = 0.f;
			iFinish = 0;
			iStop = 0;
		}

	}KEYFRAME_DESC;

	typedef struct	TweenDesc
	{
		/* 16 byte Alignment */

		KEYFRAME_DESC cur = {};		/* 현재 애니메이션 정보*/
		KEYFRAME_DESC next = {};	/* 다음 애니메이션 정보*/

		_float fTweenDuration = DEFAULT_TWEEN_DURATION;
		_float fTweenRatio = 0.f;
		_float fTweenAcc = 0.f;
		_float fPadding = 0.f;

		TweenDesc()
		{
			cur.iAnimIndex = -1;
			next.iAnimIndex = -1;
		}

		void ClearNextAnim()
		{
			next.iAnimIndex = -1;
			next.iCurFrame = 0;
			next.iNextFrame = 1;
			next.iFinish = 0;
			next.iFix = 0;
			next.iStop = 0;

			fTweenAcc = 0.f;
			fTweenRatio = 0.f;
			fTweenDuration = DEFAULT_TWEEN_DURATION;
		}

	}TWEEN_DESC;

	/* VTF 텍스처 만들기 위한 캐시 (다수 채널) */
	typedef struct	AnimTransformCaches
	{
		using Channels = std::vector<Matrix>;
		std::vector<Channels> transforms; // 벡터 원소 하나가 키프레임 하나에 해당하는 채널을 들고 있음 
		
		
		/*using TransformArrayType = std::array<Matrix, MAX_MODEL_CHANNELS>;
		std::array<TransformArrayType, MAX_MODEL_KEYFRAMES> transforms;*/

	}ANIM_TRANSFORM_CACHES;

	/* 소켓 본을 만들기 위한 캐시 (채널 하나) */
	typedef struct	AnimTransformCache
	{
		using Channels = std::vector<Matrix>;
		std::vector<Channels> transforms;

		/*using TransformArrayType = std::array<Matrix, 1>;
		std::array<TransformArrayType, MAX_MODEL_KEYFRAMES> transforms;*/

	}ANIM_TRANSFORM_CACHE;

#pragma endregion

#pragma region Animation Event 

	typedef struct tagAnimEventDesc
	{
		//ANIM_EVENT_TYPE eType;

		_bool bExecuted = FALSE;

	}ANIM_EVENT_DESC;

	typedef struct tagAnimEventSoundDesc : public ANIM_EVENT_DESC
	{
		string		strSoundKey = "";
		_uint		iChannelID = CHANNELID::MAXCHANNEL;
		_float		fVolume = 0.f;
		_bool		bStop = false;

	}ANIM_EVENT_SOUND_DESC;

	typedef struct tagAnimEventColliderDesc : public ANIM_EVENT_DESC
	{
		_bool	bOnOff;

		Vec3	vOffset;
		Vec3	vExtents;
		_uint	iDetectionType;
		_uint	iAttackType;

	}ANIM_EVENT_COLLIDER_DESC;

#pragma endregion

#pragma region Animation KeyFrame Speed 
	
	typedef struct tagAnimSpeedDesc
	{
		_float fStartFrame = 0.f;
		_float fEndFrame = 0.f;

		_float fStartSpeed = 0.f;
		_float fEndSpeed = 0.f;

		tagAnimSpeedDesc() {};
		tagAnimSpeedDesc(const _float& _fPoint1, const _float& _fPoint2, const _float& _fValue1, const _float& _fValue2)
			: fStartFrame(_fPoint1), fEndFrame(_fPoint2), fStartSpeed(_fValue1), fEndSpeed(_fValue2) {}

	}ANIM_SPEED_DESC;

#pragma endregion


#pragma region SkyPlane
	struct SkyPlaneType
	{
		Vec3 vPosition;
		Vec2 vUV;
	};
#pragma endregion SkyPlane

#pragma region Motion_Trail
	typedef struct tagMotionTrailDesc
	{
		class CModel* pModel = nullptr;
		class CModel* pRenderModel = nullptr;
		TWEEN_DESC TweenDesc = {};

		Matrix WorldMatrix = Matrix::Identity;

		_float fBlurPower = 0.f;
		_float3 vBloomPower = {};
		_float4 vRimColor = {};


		_float fAlphaSpeed = 1.f;

		_float fAccMotionTrail = 0.f;
		_float fMotionTrailTime = 0.05f;

	} MOTION_TRAIL_DESC;
#pragma endregion

#pragma region COUNTER_FX
	typedef struct tagCounterFXDesc
	{
		class CModel* pModel = nullptr;
		class CModel* pRenderModel = nullptr;
		TWEEN_DESC TweenDesc = {};

		Matrix WorldMatrix = Matrix::Identity;

		_float fBlurPower = 0.f;
		_float3 vBloomPower = {};
		_float4 vRimColor = {};


		_float fAlphaSpeed = 1.f;

		_float fAccMotionTrail = 0.f;
		_float fMotionTrailTime = 0.05f;

	} COUNTER_FX_DESC;
#pragma endregion

#pragma region AnimModelInstance
	typedef struct tagAnimModelInstanceDesc
	{
		Vec4 vRimColor = { 0.f, 0.f, 0.f, 0.f };
		Vec4	vDissolveColor = { 0.f, 0.f, 0.f, 0.f };

		_float  fDissolveTotal = 10.f;
		_float  fDissolveDuration = 5.f;
		_float  fDissolveWeight = 0.f;
		_float  fDissolveSpeed = 5.f;
	} ANIMODEL_INSTANCE_DESC;

#pragma endregion

#pragma region
	typedef struct tagBuffDesc
	{
		_float fAccBuffTime = 0.f;
		_float fEndBuffTime = 1.f; // 몇초 동안 수행하는지.

		_float fAccEffectDeleteTime = 0.f; // 이펙트 삭제 Acc.
		_float fDeleteEffect = 0.f; // 이펙트 삭제 시간.

		_int iAccBuffCount = 0; // 몇 번 수행 했는지.
		_int iBuffCount = 1; // 몇 번 수행하는지.
	} BUFF_DESC;

#pragma endregion

#pragma region Functor

#pragma endregion




}


#endif // Engine_Struct_h__
