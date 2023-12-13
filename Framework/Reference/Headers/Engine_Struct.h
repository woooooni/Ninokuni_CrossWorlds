#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagCollisionInfoDesc
	{
		class CCollider* pMyCollider;

		class CGameObject* pOther;
		class CCollider* pOtherCollider;
	} COLLISION_INFO;


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
		enum TYPE { TYPE_POINT, TYPE_DIRECTIONAL, TYPE_END };

		TYPE			eType;

		XMFLOAT4		vDirection;

		XMFLOAT4		vPosition;
		float			fRange;

		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;

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
		_float3 vExtents = { 1.f, 1.f, 1.f };
		_float fRadius = 1.f;

		PhysXRigidType eRigidType = PhysXRigidType::RIGID_TYPE_END;
		PhysXColliderType eColliderType = PhysXColliderType::COLLIDER_TYPE_END;
		class CGameObject* pGameObject = nullptr;
	} PHYSX_INIT_DESC;

	typedef struct tagPhysXStaticObjectDesc
	{
		class CGameObject* pObject = nullptr;
		class PxRigidStatic* pActor = nullptr;
	} PHYSX_STATIC_OBJECT_DESC;

	typedef struct tagPhysXDynamicObjectDesc
	{
		class CGameObject* pObject = nullptr;
		class PxRigidDynamic* pActor = nullptr;
	} PHYSX_DYNAMIC_OBJECT_DESC;
#pragma endregion

#pragma region Lerp Desc

	enum class LERP_MODE { DEFAULT, EASE_OUT, EASE_IN, EXPONENTIAL, SMOOTH_STEP, SMOOTHER_STEP, TYPEEND };

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
			bActive = TRUE;

			fCurTime = 0.f;
			fEndTime = _fTime;

			fStartValue = fCurValue = _fStartValue;
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

			fLerpTime = LERP_DESC::Calculate_Time(fCurTime, fEndTime, eMode);

			return fLerpTime;
		}
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
			bActive = TRUE;

			fCurTime = 0.f;
			fEndTime = _fTime;

			vStartVec = vCurVec = _fStartValue;
			vTargetVec = _fTargetValue;

			eMode = _eMode;
		}

		Vec3 Update_Lerp(const _float& fTimeDelta)
		{
			if (!bActive) 
				return vCurVec;

			fCurTime += fTimeDelta;

			if (fCurTime >= fEndTime)
			{
				vCurVec = vTargetVec;
				return vCurVec;
			}

			const _float fTime = LERP_DESC::Calculate_Time(fCurTime, fEndTime, eMode);

			vCurVec = Vec3::Lerp(vStartVec, vTargetVec, fTime);

			return vCurVec;
		}
	}LERP_VEC3_DESC;

	typedef struct	KeyframeDesc
	{
		_int	iAnimIndex = -1;	/* 현재 애니메이션 인덱스*/
		_uint	iCurFrame = 0;		/* 애니메이션의 현재 프레임 */
		_uint	iNextFrame = 1;		/* 애니메이션의 다음 프레임 */
		_float	fRatio = 0.f;		/* 애니메이션의 현재 프레임과 다음 프레임 사이 진행률*/
		_float	fFrameAcc = 0.f;	/* 현재 프레임 누적 시간 */

		void ClearAnim()
		{
			iCurFrame = 0;
			iNextFrame = 1;
			fRatio = 0.f;
			fFrameAcc = 0.f;
		}

	}KEYFRAME_DESC;

	typedef struct	TweenDesc
	{
		KEYFRAME_DESC cur = {};		/* 현재 애니메이션 정보*/
		KEYFRAME_DESC next = {};	/* 다음 애니메이션 정보*/

		_float fTweenDuration = DEFAULT_TWEEN_DURATION;
		_float fTweenRatio = 0.f;
		_float fTweenAcc = 0.f;

		_float fPadding1 = 0.f;
		_float fPadding2 = 0.f;
		_float fPadding3 = 0.f;

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

			fTweenAcc = 0.f;
			fTweenRatio = 0.f;
			fTweenDuration = DEFAULT_TWEEN_DURATION;
		}

	}TWEEN_DESC;
#pragma endregion
}


#endif // Engine_Struct_h__
