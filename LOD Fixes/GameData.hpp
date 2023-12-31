#pragma once

#include "SafeWrite.h"
#include "Utilities.h"

#define ASSERT_SIZE(a, b) static_assert(sizeof(a) == b, "Wrong structure size!");
#define ASSERT_OFFSET(a, b, c) static_assert(offsetof(a, b) == c, "Wrong member offset!");
#define CREATE_OBJECT(CLASS, ADDRESS) static CLASS* CreateObject() { return StdCall<CLASS*>(ADDRESS); };

class BGSDistantObjectBlock;
class bhkBlendCollisionObject;
class bhkCollisionObject;
class bhkLimitedHingeConstraint;
class bhkRigidBody;
class BSFadeNode;
class BSMultiBoundNode;
class BSMultiBound;
class BSResizableTriShape;
class BSSegmentedTriShape;
class NiCloningProcess;
class NiGeometry;
class NiLines;
class NiNode;
class NiParticles;
class NiStream;
class NiTriBasedGeom;
class NiTriShape;
class NiTriStrips;
class NiControllerManager;
class NiObjectGroup;
class NiBound;
class NiViewerStringsArray;
class NiUpdateData;
class NiMatrix3;
class NiCullingProcess;
class NiFixedString;
class NiGeometryData;
class NiSkinInstance;
class bhkNiCollisionObject;
class NiDX9Renderer;
class BSShader;
class BGSTextureUseMap;
class NiSourceTexture;
class RenderPassArray;
class RenderPass;
class BSShaderAccumulator;
class NiAdditionalGeometryData;
class NiGeometryBufferData;
class NiD3DPass;
class NiD3DShaderDeclaration;
class NiD3DRenderStateGroup;
class NiD3DShaderConstantMap;
class NiDX9ShaderDeclaration;
class NiDynamicEffectState;
class BGSDistantTreeBlock;
class BGSTerrainChunk;
class NiProperty;
class NiNode;
class BSMultiBoundRoom;

class NiUpdateData {
public:
	NiUpdateData(float afTime = 0, bool abUpdateControllers = 0, bool abIsMultiThreaded = 0, bool abyte6 = 0, bool abUpdateGeomorphs = 0, bool abUpdateShadowSceneNode = 0)
		: fTime(afTime), bUpdateControllers(abUpdateControllers), bIsMultiThreaded(abIsMultiThreaded), byte6(abyte6), bUpdateGeomorphs(abUpdateGeomorphs), bUpdateShadowSceneNode(abUpdateShadowSceneNode)
	{}
	~NiUpdateData() {};

	float fTime;
	bool bUpdateControllers;
	bool bIsMultiThreaded;
	bool byte6;
	bool bUpdateGeomorphs;
	bool bUpdateShadowSceneNode;

	static NiUpdateData kDefaultUpdateData;
};

class NiMatrix3 {
public:
	float m_pEntry[3][3];
};

class NiPoint2 {
public:
	float x, y;
};

class NiPoint3 {
public:
	float x, y, z;
};

class NiColorA {
public:
	float r, g, b, a;
};

class NiBound {
public:
	NiPoint3	m_kCenter;
	float		m_fRadius;
};

class NiTransform {
public:
	NiMatrix3	m_Rotate;
	NiPoint3	m_Translate;
	float		m_fScale;
};

class NiRTTI {
public:
	const char*		m_pcName;
	const NiRTTI*	m_pkBaseRTTI;
};

template <typename T_Data>
class NiTArray {
public:
	virtual ~NiTArray();

	T_Data* m_pBase;
	UInt16 m_usMaxSize;
	UInt16 m_usSize;
	UInt16 m_usESize;
	UInt16 m_usGrowBy;
};

ASSERT_SIZE(NiTArray<void*>, 0x10);

typedef void* NiTListIterator;

template <typename T_Data>
class NiTListItem {
public:
	NiTListItem*	m_pkNext;
	NiTListItem*	m_pkPrev;
	T_Data			m_element;
};

template <typename T_Data>
class NiTListBase {
public:
	NiTListItem<T_Data>*	m_pkHead;
	NiTListItem<T_Data>*	m_pkTail;
	UInt32					m_uiCount;

	inline UInt32 GetSize() const { return m_uiCount; };
	bool IsEmpty() const { return m_uiCount == 0; };

	NiTListIterator GetHeadPos() const { return m_pkHead; };
	NiTListIterator GetTailPos() const { return m_pkTail; };
};

class NiRefObject {
public:
    virtual			~NiRefObject();
    virtual void	DeleteThis();

    UInt32 m_uiRefCount;

    // 0x40F6E0
    inline void IncRefCount() {
        InterlockedIncrement(&m_uiRefCount);
    }

    // 0x401970
    inline void DecRefCount() {
        if (!InterlockedDecrement(&m_uiRefCount))
            DeleteThis();
    }
};

class NiObject : public NiRefObject {
public:
    virtual const NiRTTI*				GetRTTI() const;												// 02 | Returns NiRTTI of the object
	virtual NiNode*						IsNiNode() const;												// 03 | Returns this if it's a NiNode, otherwise nullptr
	virtual BSFadeNode*					IsFadeNode() const;												// 04 | Returns this if it's a BSFadeNode, otherwise nullptr
	virtual BSMultiBoundNode*			IsMultiBoundNode() const;										// 05 | Returns this if it's a BSMultiBoundNode, otherwise nullptr
	virtual NiGeometry*					IsGeometry() const;												// 06 | Returns this if it's a NiGeometry, otherwise nullptr
	virtual NiTriBasedGeom*				IsTriBasedGeometry() const;										// 07 | Returns this if it's a NiTriBasedGeom, otherwise nullptr
	virtual NiTriStrips*				IsTriStrips() const;											// 08 | Returns this if it's a NiTriStrips, otherwise nullptr
	virtual NiTriShape*					IsTriShape() const;												// 09 | Returns this if it's a NiTriShape, otherwise nullptr
	virtual BSSegmentedTriShape*		IsSegmentedTriShape() const;									// 10 | Returns this if it's a BSSegmentedTriShape, otherwise nullptr
	virtual BSResizableTriShape*		IsResizableTriShape() const;									// 11 | Returns this if it's a BSResizableTriShape, otherwise nullptr
	virtual NiParticles*				IsParticlesGeom() const;										// 12 | Returns this if it's a NiParticles, otherwise nullptr
	virtual NiLines*					IsLinesGeom() const;											// 13 | Returns this if it's a NiLines, otherwise nullptr
	virtual bhkCollisionObject*			IsBhkNiCollisionObject() const;									// 14 | Returns this if it's a bhkCollisionObject, otherwise nullptr
	virtual bhkBlendCollisionObject*	IsBhkBlendCollisionObject() const;								// 15 | Returns this if it's a bhkBlendCollisionObject, otherwise nullptr
	virtual bhkRigidBody*				IsBhkRigidBody() const;											// 16 | Returns this if it's a bhkRigidBody, otherwise nullptr
	virtual bhkLimitedHingeConstraint*	IsBhkLimitedHingeConstraint() const;							// 17 | Returns this if it's a bhkLimitedHingeConstraint, otherwise nullptr
	virtual NiObject*					CreateClone(NiCloningProcess* apCloning);						// 18 | Creates a clone of this object
	virtual void						LoadBinary(NiStream* apStream);									// 19 | Loads objects from disk
	virtual void						LinkObject(NiStream* apStream);									// 20 | Called by the streaming system to resolve links to other objects once it can be guaranteed that all objects have been loaded
	virtual void						RegisterStreamables(NiStream* apStream);						// 21 | When an object is inserted into a stream, it calls register streamables to make sure that any contained objects or objects linked in a scene graph are streamed as well
	virtual void						SaveBinary(NiStream* apStream);									// 22 | Saves objects to disk
	virtual bool						IsEqual(NiObject* apObject) const;								// 23 | Compares this object with another
	virtual void						GetViewerStrings(NiViewerStringsArray* apStrings);				// 24 | Gets strings containing information about the object
	virtual void						AddViewerStrings(NiViewerStringsArray* apStrings);				// 25 | Adds additional strings containing information about the object
	virtual void						ProcessClone(NiCloningProcess* apCloning);						// 26 | Post process for CreateClone
	virtual void						PostLinkObject(NiStream* apStream);								// 27 | Called by the streaming system to resolve any tasks that require other objects to be correctly linked. It is called by the streaming system after LinkObject has been called on all streamed objects
	virtual bool						StreamCanSkip();												// 28
	virtual const NiRTTI*				GetStreamableRTTI();											// 29
	virtual void						SetBound(NiBound* apNewBound);									// 30 | Replaces the bound of the object
	virtual void						GetBlockAllocationSize();										// 31 | Used by geometry data
	virtual NiObjectGroup*				GetGroup();														// 32 | Used by geometry data
	virtual void						SetGroup(NiObjectGroup* apGroup);								// 33 | Used by geometry data
	virtual NiControllerManager*		IsControllerManager() const;									// 34 | Returns this if it's a NiControllerManager, otherwise nullptr
};

class NiObjectNET : public NiObject {
public:
	const char*	m_kName;
	void*		m_spControllers;
	void**		m_ppkExtra;
	UInt16		m_usExtraDataSize;
	UInt16		m_usMaxSize;
};

class NiAVObject : public NiObjectNET {
public:
	virtual void			UpdateControllers(NiUpdateData* arData);
	virtual void			ApplyTransform(NiMatrix3& akMat, NiPoint3& akTrn, bool abOnLeft);
	virtual void			Unk_39();
	virtual NiAVObject*		GetObject_(const NiFixedString& kName);
	virtual NiAVObject*		GetObjectByName(const NiFixedString& kName);
	virtual void			SetSelectiveUpdateFlags(bool* bSelectiveUpdate, bool bSelectiveUpdateTransforms, bool* bRigid);
	virtual void			UpdateDownwardPass(const NiUpdateData& arData, UInt32 uFlags);
	virtual void			UpdateSelectedDownwardPass(const NiUpdateData& arData, UInt32 uFlags);
	virtual void			UpdateRigidDownwardPass(const NiUpdateData& arData, UInt32 uFlags);
	virtual void			Unk_46(void* arg);
	virtual void			UpdateTransform();
	virtual void			UpdateWorldData(const NiUpdateData& arData);
	virtual void			UpdateWorldBound();
	virtual void			UpdateTransformAndBounds(const NiUpdateData& arData);
	virtual void			PreAttachUpdate(NiNode* pEventualParent, const NiUpdateData& arData);
	virtual void			PreAttachUpdateProperties(NiNode* pEventualParent);
	virtual void			DetachParent();
	virtual void			UpdateUpwardPassParent(void* arg);
	virtual void			OnVisible(NiCullingProcess* kCuller);
	virtual void			PurgeRendererData(NiDX9Renderer* apRenderer);

	enum NiFlags : DWORD {
		APP_CULLED = UInt32(1) << 0,
		SELECTIVE_UPDATE = UInt32(1) << 1,
		SELECTIVE_UPDATE_TRANSFORMS = UInt32(1) << 2,
		SELECTIVE_UPDATE_CONTROLLER = UInt32(1) << 3,
		SELECTIVE_UPDATE_RIGID = UInt32(1) << 4,
		DISPLAY_OBJECT = UInt32(1) << 5,
		DISABLE_SORTING = UInt32(1) << 6,
		SELECTIVE_UPDATE_TRANSFORMS_OVERRIDE = UInt32(1) << 7,
		IS_NODE = UInt32(1) << 8,
		SAVE_EXTERNAL_GEOM_DATA = UInt32(1) << 9,
		NO_DECALS = UInt32(1) << 10,
		ALWAYS_DRAW = UInt32(1) << 11,
		ACTOR_NODE = UInt32(1) << 12,
		FIXED_BOUND = UInt32(1) << 13,
		TOP_FADE_NODE = UInt32(1) << 14, // bFadedIn
		IGNORE_FADE = UInt32(1) << 15,
		NO_ANIM_SYNC_X = UInt32(1) << 16,
		LOD_FADING_OUT = UInt32(1) << 16, // BSFadeNode
		NO_ANIM_SYNC_Y = UInt32(1) << 17,
		HAS_MOVING_SOUND = UInt32(1) << 17, // BSFadeNode
		NO_ANIM_SYNC_Z = UInt32(1) << 18,
		NO_ANIM_SYNC_S = UInt32(1) << 19,
		ACTOR_CULLED = UInt32(1) << 20,
		NO_DISMEMBER_VALIDITY = UInt32(1) << 21,
		RENDER_USE = UInt32(1) << 22,
		MATERIALS_APPLIED = UInt32(1) << 23,
		HIGH_DETAIL = UInt32(1) << 24,
		FORCE_UPDATE = UInt32(1) << 25,
		PREPROCESSED_NODE = UInt32(1) << 26,
		UNK_27 = UInt32(1) << 27,
		UNK_28 = UInt32(1) << 28,
		IS_POINTLIGHT = UInt32(1) << 29,
		DONE_INIT_LIGHTS = UInt32(1) << 30,
		IS_INSERTED = UInt32(1) << 31
	};

	NiNode*							m_pkParent;
	bhkNiCollisionObject*			m_spCollisionObject;
	NiBound*						m_kWorldBound;
	NiTListBase<NiProperty*>		m_kPropertyList;
	Bitfield32						m_uiFlags;
	NiTransform						m_kLocal;
	NiTransform						m_kWorld;

	void SetBit(bool bVal, int uMask) {
		m_uiFlags.SetBit(uMask, bVal);
	}

	bool GetBit(int uMask) {
		return m_uiFlags.GetBit(uMask);
	}

	void SetAppCulled(bool bVal) {
		SetBit(bVal, APP_CULLED);
	}

	bool GetAppCulled() {
		return GetBit(APP_CULLED);
	}

	void SetSelectiveUpdate(bool bVal) {
		SetBit(bVal, SELECTIVE_UPDATE);
	}

	bool GetSelectiveUpdate() {
		return GetBit(SELECTIVE_UPDATE);
	}

	void SetSelUpdTransforms(bool bVal) {
		SetBit(bVal, SELECTIVE_UPDATE_TRANSFORMS);
	}

	bool GetSelUpdTransforms() {
		return GetBit(SELECTIVE_UPDATE_TRANSFORMS);
	}

	void SetSelUpdController(bool bVal) {
		SetBit(bVal, SELECTIVE_UPDATE_CONTROLLER);
	}

	bool GetSelUpdController() {
		return GetBit(SELECTIVE_UPDATE_CONTROLLER);
	}

	void SetSelUpdRigid(bool bVal) {
		SetBit(bVal, SELECTIVE_UPDATE_RIGID);
	}

	bool GetSelUpdRigid() {
		return GetBit(SELECTIVE_UPDATE_RIGID);
	}

	void SetDisplayObject(bool bVal) {
		SetBit(bVal, DISPLAY_OBJECT);
	}

	bool GetDisplayObject() {
		return GetBit(DISPLAY_OBJECT);
	}

	void SetDisableSorting(bool bVal) {
		SetBit(bVal, DISABLE_SORTING);
	}

	bool GetDisableSorting() {
		return GetBit(DISABLE_SORTING);
	}

	void SetSelUpdTransformsOverride(bool bVal) {
		SetBit(bVal, SELECTIVE_UPDATE_TRANSFORMS_OVERRIDE);
	}

	bool GetSelUpdTransformsOverride() {
		return GetBit(SELECTIVE_UPDATE_TRANSFORMS_OVERRIDE);
	}

	void SetIsNode(bool bVal) {
		SetBit(bVal, IS_NODE);
	}

	bool GetIsNode() {
		return GetBit(IS_NODE);
	}

	void SetSaveExternalGeomData(bool bVal) {
		SetBit(bVal, SAVE_EXTERNAL_GEOM_DATA);
	}

	bool GetSaveExternalGeomData() {
		return GetBit(SAVE_EXTERNAL_GEOM_DATA);
	}

	void SetNoDecals(bool bVal) {
		SetBit(bVal, NO_DECALS);
	}

	bool GetNoDecals() {
		return GetBit(NO_DECALS);
	}

	void SetAlwaysDraw(bool bVal) {
		SetBit(bVal, ALWAYS_DRAW);
	}

	bool GetAlwaysDraw() {
		return GetBit(ALWAYS_DRAW);
	}

	void SetActorNode(bool bVal) {
		SetBit(bVal, ACTOR_NODE);
	}

	bool GetActorNode() {
		return GetBit(ACTOR_NODE);
	}

	void SetFixedBound(bool bVal) {
		SetBit(bVal, FIXED_BOUND);
	}

	bool GetFixedBound() {
		return GetBit(FIXED_BOUND);
	}

	void SetTopFadeNode(bool bVal) {
		SetBit(bVal, TOP_FADE_NODE);
	}

	bool GetTopFadeNode() {
		return GetBit(TOP_FADE_NODE);
	}

	void SetIgnoreFade(bool bVal) {
		SetBit(bVal, IGNORE_FADE);
	}

	bool GetIgnoreFade() {
		return GetBit(IGNORE_FADE);
	}

	void SetNoAnimSyncX(bool bVal) {
		SetBit(bVal, NO_ANIM_SYNC_X);
	}

	bool GetNoAnimSyncX() {
		return GetBit(NO_ANIM_SYNC_X);
	}

	void SetNoAnimSyncY(bool bVal) {
		SetBit(bVal, NO_ANIM_SYNC_Y);
	}

	bool GetNoAnimSyncY() {
		return GetBit(NO_ANIM_SYNC_Y);
	}

	void SetNoAnimSyncZ(bool bVal) {
		SetBit(bVal, NO_ANIM_SYNC_Z);
	}

	bool GetNoAnimSyncZ() {
		return GetBit(NO_ANIM_SYNC_Z);
	}

	void SetNoAnimSyncS(bool bVal) {
		SetBit(bVal, NO_ANIM_SYNC_S);
	}

	bool GetNoAnimSyncS() {
		return GetBit(NO_ANIM_SYNC_S);
	}

	void SetActorCulled(bool bVal) {
		SetBit(bVal, ACTOR_CULLED);
	}

	bool GetActorCulled() {
		return GetBit(ACTOR_CULLED);
	}

	void SetNoDismemberValidity(bool bVal) {
		SetBit(bVal, NO_DISMEMBER_VALIDITY);
	}

	bool GetNoDismemberValidity() {
		return GetBit(NO_DISMEMBER_VALIDITY);
	}

	// 0x439410
	void AddProperty(NiProperty* apProperty) {
		ThisStdCall(0x439410, this, apProperty);
	}

	// 0xA5B230
	void RemoveProperty(UInt32 auiPropertyType) {
		ThisStdCall(0xA5B230, this, auiPropertyType);
	}

	// 0xA59D30
	NiProperty* GetProperty(UInt32 auiPropertyType) {
		return ThisStdCall<NiProperty*>(0xA59D30, this, auiPropertyType);
	}

	// 0x5467E0
	void CreateWorldBoundIfMissing() {
		ThisStdCall(0x5467E0, this);
	}

	// 0xA5A040
	void UpdatePropertiesUpward() {
		ThisStdCall(0xA5A040, this);
	}
	
	// 0xA59C60
	void Update(NiUpdateData& arData) {
		ThisStdCall(0xA59C60, this, &arData);
	}
};

class NiNode : public NiAVObject {
public:
	virtual void			AttachChild(NiAVObject* apChild, bool abFirstAvail);
	virtual void			InsertChildAt(UInt32 i, NiAVObject* apChild);
	virtual void			DetachChild(NiAVObject* apChild, NiAVObject*& aspAVObject);
	virtual void			DetachChildAlt(NiAVObject* apChild);
	virtual void			DetachChildAt(UInt32 i, NiAVObject*& aspAVObject);
	virtual NiAVObject*		DetachChildAtAlt(UInt32 i);
	virtual void			SetAt(UInt32 i, NiAVObject* apChild, NiAVObject*& aspAVObject);
	virtual void			SetAtAlt(UInt32 i, NiAVObject* apChild);
	virtual void			UpdateUpwardPass();

	NiTArray<NiAVObject*> m_kChildren;

    UInt32 GetChildCount() {
		return m_kChildren.m_usESize;
    }

	NiAVObject* GetAt(UInt32 index) {
		if (index >= GetChildCount())
			return nullptr;

		return m_kChildren.m_pBase[index];
	}
};

class BSNiNode : public NiNode {
public:
	virtual ~BSNiNode();
	virtual void ReparentSkinInstances(NiNode* apNode, NiAVObject* apParent);
};

class BSMultiBoundNode : public BSNiNode {
public:
	virtual BSMultiBoundRoom*	GetMultiBoundRoom();
	virtual bool				GetPointWithin(NiPoint3& akPoint);
	virtual UInt32				CheckBound(BSMultiBound*);
	virtual UInt32				CheckBoundAlt(NiBound*);

	BSMultiBound*	spMultiBound;
	UInt32			uiCullingMode;
};

class NiProperty : public NiObjectNET {
public:
	virtual UInt32	Type();
	virtual void	Update(NiUpdateData* pUpdateData);
};

class NiAlphaProperty : public NiProperty {
public:
	Bitfield16	m_usFlags;
	UInt8		m_ucAlphaTestRef;

	CREATE_OBJECT(NiAlphaProperty, 0xA5CEB0);

	enum AlphaFlags {
		ALPHA_BLEND_MASK = 0x0001,
		SRC_BLEND_MASK = 0x001e,
		SRC_BLEND_POS = 1,
		DEST_BLEND_MASK = 0x01e0,
		DEST_BLEND_POS = 5,
		TEST_ENABLE_MASK = 0x0200,
		TEST_FUNC_MASK = 0x1c00,
		TEST_FUNC_POS = 10,
		ALPHA_NOSORTER_MASK = 0x2000
	};

	enum AlphaFunction {
		ALPHA_ONE = 0x0,
		ALPHA_ZERO = 0x1,
		ALPHA_SRCCOLOR = 0x2,
		ALPHA_INVSRCCOLOR = 0x3,
		ALPHA_DESTCOLOR = 0x4,
		ALPHA_INVDESTCOLOR = 0x5,
		ALPHA_SRCALPHA = 0x6,
		ALPHA_INVSRCALPHA = 0x7,
		ALPHA_DESTALPHA = 0x8,
		ALPHA_INVDESTALPHA = 0x9,
		ALPHA_SRCALPHASAT = 0xA,
		ALPHA_MAX_MODES = 0xB,
	};

	enum TestFunction {
		TEST_ALWAYS = 0x0,
		TEST_LESS = 0x1,
		TEST_EQUAL = 0x2,
		TEST_LESSEQUAL = 0x3,
		TEST_GREATER = 0x4,
		TEST_NOTEQUAL = 0x5,
		TEST_GREATEREQUAL = 0x6,
		TEST_NEVER = 0x7,
		TEST_MAX_MODES = 0x8,
	};

	bool HasAlphaBlend() const {
		return m_usFlags.GetBit(ALPHA_BLEND_MASK);
	}

	bool HasAlphaTest() const {
		return m_usFlags.GetBit(TEST_ENABLE_MASK);
	}

	bool HasAlphaSorter() const {
		return m_usFlags.GetBit(ALPHA_NOSORTER_MASK);
	}

	UInt8 GetAlphaTestRef() const {
		return m_ucAlphaTestRef;
	}

	void SetAlphaTestRef(UInt8 ucRef) {
		m_ucAlphaTestRef = ucRef;
	}

	void SetAlphaBlending(bool abBlend) {
		m_usFlags.SetBit(ALPHA_BLEND_MASK, abBlend);
	}

	void SetAlphaTesting(bool abTest) {
		m_usFlags.SetBit(TEST_ENABLE_MASK, abTest);
	}

	void SetSrcBlendMode(AlphaFunction aeSrcBlend) {
		m_usFlags.SetField(aeSrcBlend, SRC_BLEND_MASK, ALPHA_BLEND_MASK);
	}

	void SetDestBlendMode(AlphaFunction eDestBlend) {
		m_usFlags.SetField(eDestBlend, DEST_BLEND_MASK, DEST_BLEND_POS);
	}

	void SetTestRef(UInt8 aucRef) {
		m_ucAlphaTestRef = aucRef;
	}

	void SetTestMode(TestFunction aeTestFunc) {
		m_usFlags.SetField(aeTestFunc, TEST_FUNC_MASK, TEST_FUNC_POS);
	}
};

class NiShadeProperty : public NiProperty {
public:
	UInt16	m_usFlags;
	UInt32	m_eShaderType;
};

class BSShaderProperty : public NiShadeProperty {
public:
	virtual void						CopyTo(BSShaderProperty* apTarget);
	virtual void						CopyData(BSShaderProperty* apTarget);
	virtual void						SetupGeometry(NiGeometry* apGeometry);
	virtual RenderPassArray*			GetRenderPasses(NiGeometry* apGeometry, UInt32 auiEnabledPasses, UInt16* apusPassCount, UInt32 aeRenderMode, BSShaderAccumulator* apAccumulator, bool abAddPass);
	virtual UInt16						GetNumberofPasses(NiGeometry* apGeometry);
	virtual RenderPassArray*			GetRenderPassArray4C();
	virtual RenderPass*					SetDepthPass(NiGeometry* apGeometry);
	virtual BSShaderProperty*			PickShader(NiGeometry* apGeometry, UInt32 unk0 = 0, UInt32 unk2 = 0);
	virtual NiSourceTexture*			GetDiffuseTexture();
	virtual RenderPassArray*			GetWaterDepthPass(NiGeometry* apGeometry);
	virtual void						CountTextures(void* apCountFunc, BGSTextureUseMap* apTexMap);
	virtual void						PrecacheTextures();

	enum Flags {
		BSSP_SPECULAR = 0X0,
		BSSP_SKINNED = 0X1,
		BSSP_LOW_DETAIL = 0X2,
		BSSP_VERTEX_ALPHA = 0X3,
		BSSP_MOTION_BLUR = 0X4,
		BSSP_SINGLE_PASS = 0X5,
		BSSP_EMPTY = 0X6,
		BSSP_ENVIRONMENT_MAPPING = 0X7,
		BSSP_ALPHA_TEXTURE = 0X8,
		BSSP_ZPREPASS = 0X9,
		BSSP_FACEGEN = 0XA,
		BSSP_PARALLAX = 0XB,
		BSSP_MODEL_SPACE_NORMALS = 0XC,
		BSSP_NON_PROJECTIVE_SHADOWS = 0XD,
		BSSP_LANDSCAPE = 0XE,
		BSSP_REFRACTION = 0XF,
		BSSP_FIRE_REFRACTION = 0X10,
		BSSP_EYE_ENVIRONMENT_MAPPING = 0X11,
		BSSP_HAIR = 0X12,
		BSSP_DYNAMIC_ALPHA = 0X13,
		BSSP_LOCAL_MAP_HIDE_SECRET = 0X14,
		BSSP_WINDOW_ENVIRONMENT_MAPPING = 0X15,
		BSSP_TREE_BILLBOARD = 0X16,
		BSSP_SHADOW_FRUSTUM = 0X17,
		BSSP_MULTIPLE_TEXTURES = 0X18,
		BSSP_REMAPPABLE_TEXTURES = 0X19,
		BSSP_DECAL = 0X1A,
		BSSP_DYNAMIC_DECAL = 0X1B,
		BSSP_PARALLAX_OCCLUSION = 0X1C,
		BSSP_EXTERNAL_EMITTANCE = 0X1D,
		BSSP_SHADOW_MAP = 0X1E,
		BSSP_ZBUFFER_TEST = 0X1F,
		BSSP_ZBUFFER_WRITE = 0X20,
		BSSP_LOD_LANDSCAPE = 0X21,
		BSSP_LOD_BUILDING = 0X22,
		BSSP_NO_FADE = 0X23,
		BSSP_REFRACTION_TINT = 0X24,
		BSSP_VERTEX_COLORS = 0X25,
		BSSP_1ST_PERSON = 0X26,
		BSSP_1ST_LIGHT_IS_POINTLIGHT = 0X27,
		BSSP_2ND_LIGHT = 0X28,
		BSSP_3RD_LIGHT = 0X29,
		BSSP_VERTEX_LIGHTING = 0X2A,
		BSSP_UNIFORM_SCALE = 0X2B,
		BSSP_FIT_SLOPE = 0X2C,
		BSSP_BILLBOARD_ENV_FADE = 0X2D,
		BSSP_NO_LOD_LAND_BLEND = 0X2E,
		BSSP_ENVMAP_LIGHT_FADE = 0X2F,
		BSSP_WIRE_FRAME = 0X30,
		BSSP_VATS_SELECTION = 0X31,
		BSSP_SHOW_IN_LOCAL_MAP = 0X32,
		BSSP_PREMULT_ALPHA = 0X33,
		BSSP_SKIP_NORMAL_MAPS = 0X34,
		BSSP_ALPHA_DECAL = 0X35,
		BSSP_NO_TRANSPARENCY_MULTISAMPLING = 0X36,
		BSSP_STINGER_PROP = 0X37,
		BSSP_UNK3 = 0x38,
		BSSP_UNK4 = 0x39,
		BSSP_UNK5 = 0x3A,
		BSSP_UNK6 = 0x3B,
		BSSP_UNK7 = 0x3C,
		BSSP_UNK8 = 0x3D,
		BSSP_UNK9 = 0x3E,
		BSSP_REALTIME_CUBEMAP = 0x3F,
		BSSP_MAX_FLAGS = 0x40
	};

	class RenderPassArray;

	UInt32				ulFlags[2];
	float				fAlpha;
	float				fFadeAlpha;
	float				fEnvMapScale;
	float				fLODFade;
	SInt32				iLastRenderPassState;
	RenderPassArray*	pRenderPassArray;
	RenderPassArray*	pRenderPassArray_depthMap;
	RenderPassArray*	pRenderPassArray_constAlpha;
	RenderPassArray*	pRenderPassArray_localMap;
	RenderPassArray*	pRenderPassArray_unk4C;
	RenderPassArray*	pRenderPassArray_waterDepth;
	RenderPassArray*	pRenderPassArray_silhouette;
	UInt32				uiShaderIndex;
	float				fDepthBias;

	bool GetFlag(UInt32 auiFlag) const {
		return ((1 << (auiFlag % 0x20u)) & ulFlags[auiFlag >> 5]) != 0;
	}
	void SetFlag(UInt32 auiFlag, bool abEnable) {
		UInt32 uiFlagSet = auiFlag >> 5;
		UInt32 uiFlag = 1 << (auiFlag % 0x20u);
		if (abEnable) {
			if ((uiFlag & ulFlags[uiFlagSet]) == 0)
				iLastRenderPassState = 0;
			ulFlags[uiFlagSet] |= uiFlag;
		}
		else {
			if ((uiFlag & ulFlags[uiFlagSet]) != 0)
				iLastRenderPassState = 0;
			ulFlags[uiFlagSet] &= ~uiFlag;
		}
	}
};

class NiPropertyState {
public:
	enum PropertyID {
		ALPHA = 0,
		CULLING = 1,
		MATERIAL = 2,
		SHADE = 3,
		STENCIL = 4,
		TEXTURING = 5,
		UNK = 6,
		MAX,
	};

	NiProperty* m_aspProps[MAX];
};

class NiGeometry : public NiAVObject {
public:
	NiPropertyState	m_kProperties;
	NiGeometryData*	m_spModelData;
	NiSkinInstance*	m_spSkinInstance;
	BSShader*		m_pShader;

	NiGeometryData* GetModelData() const { return m_spModelData; };
};

class NiGeometryData : public NiObject {
public:
	enum Consistency {
		MUTABLE = 0x0000,
		STATIC = 0x4000,
		VOLATILE = 0x8000,
		CONSISTENCY_MASK = 0xF000,
	};

	enum KeepFlags {
		KEEP_NONE = 0x0,
		KEEP_XYZ = 0x1,
		KEEP_NORM = 0x2,
		KEEP_COLOR = 0x4,
		KEEP_UV = 0x8,
		KEEP_INDICES = 0x10,
		KEEP_BONEDATA = 0x20,
		KEEP_ALL = 0x3F,
	};

	enum Compression {
		COMPRESS_NORM = 0x1,
		COMPRESS_COLOR = 0x2,
		COMPRESS_UV = 0x4,
		COMPRESS_WEIGHT = 0x8,
		COMPRESS_POSITION = 0x10,
		COMPRESS_ALL = 0x1F,
	};

	enum MarkAsChangedFlags {
		VERTEX_MASK = 0x1,
		NORMAL_MASK = 0x2,
		COLOR_MASK = 0x4,
		TEXTURE_MASK = 0x8,
		DIRTY_MASK = 0xFFF,
	};

	UInt16 m_usVertices;
	UInt16 m_usID;
	UInt16 m_usDataFlags;
	UInt16 m_usDirtyFlags;
	NiBound m_kBound;
	NiPoint3* m_pkVertex;
	NiPoint3* m_pkNormal;
	NiColorA* m_pkColor;
	NiPoint2* m_pkTexture;
	NiAdditionalGeometryData* m_spAdditionalGeomData;
	NiGeometryBufferData* m_pkBuffData;
	UInt8 m_ucKeepFlags;
	UInt8 m_ucCompressFlags;
	UInt8 Unk3A;
	UInt8 Unk3B;
	bool m_bCanSave;

	void SetKeepFlags(KeepFlags aeFlags) {
		m_ucKeepFlags = aeFlags;
	}

	void SetCompressFlags(Compression aeFlags) {
		m_ucCompressFlags = aeFlags;
	}

	void SetConsistency(Consistency aeFlags) {
		ThisStdCall(0xA67050, this, aeFlags);
	}
};


class NiTriBasedGeom : public NiGeometry {
public:
};


class NiTriShape : public NiTriBasedGeom {
public:
};


class BSSegmentedTriShape : public NiTriShape {
public:
	struct Segment {
		UInt32 uiStartIndex;
		UInt32 uiNumPrimitives;
		bool bIsEnabled;
		UInt32 uiTriCount;
		bool ucDisabledCount;
	};

	Segment* pSegments;
	UInt32 uiNumSegments;
	bool bSegmentsChanged;
	bool bIgnoreSegments;

	void EnableSegment(UInt32 uiLevel) {
		pSegments[uiLevel].bIsEnabled = pSegments[uiLevel].uiNumPrimitives != 0;
		bSegmentsChanged = true;
	}

	void DisableSegment(UInt32 uiLevel) {
		pSegments[uiLevel].bIsEnabled = false;
		bSegmentsChanged = true;
	}

	bool IsSegmentEmpty(UInt32 uiLevel) const {
		return pSegments[uiLevel].uiNumPrimitives == 0;
	}

	UInt32 GetNumSegments() {
		if (bIgnoreSegments)
			return 1;
		else
			return uiNumSegments;
	}
	void EnableAllSegments() {
		ThisStdCall(0xA872D0, this);
	}

	void UpdateDrawData() {
		ThisStdCall(0xA87040, this);
	}
};

class NiShader : public NiRefObject {
public:
	NiShader();
	virtual ~NiShader();

	virtual NiRTTI*						GetRTTI();
	virtual NiNode*						IsNiNode();
	virtual BSFadeNode*					IsFadeNode();
	virtual BSMultiBoundNode*			IsMultiBoundNode();
	virtual NiGeometry*					IsGeometry();
	virtual NiTriBasedGeom*				IsTriBasedGeometry();
	virtual NiTriStrips*				IsTriStrips();
	virtual NiTriShape*					IsTriShape();
	virtual BSSegmentedTriShape*		IsSegmentedTriShape();
	virtual BSResizableTriShape*		IsResizableTriShape();
	virtual NiParticles*				IsParticlesGeom();
	virtual NiLines*					IsLinesGeom();
	virtual bhkCollisionObject*			IsBhkNiCollisionObject();
	virtual bhkBlendCollisionObject*	IsBhkBlendCollisionObject();
	virtual bhkRigidBody*				IsBhkRigidBody();
	virtual bhkLimitedHingeConstraint*	IsBhkLimitedHingeConstraint();
	virtual bool						IsInitialized();
	virtual void						Initialize();
	virtual bool						SetupGeometry(NiGeometry* apGeometry);
	virtual UInt32						PreProcessPipeline(NiGeometry* apGeometry, const NiSkinInstance* apSkin, NiGeometryBufferData* apRendererData, const NiPropertyState* apState, const NiDynamicEffectState* apEffects, const NiTransform& kWorld, const NiBound& kWorldBound);
	virtual void						UpdatePipeline(NiGeometry* apGeometry, const NiSkinInstance* apSkin, NiGeometryBufferData* apRendererData, const NiPropertyState* apState, const NiDynamicEffectState* apEffects, const NiTransform& kWorld, const NiBound& kWorldBound);
	virtual void						SetupRenderingPass(NiGeometry* apGeometry, const NiSkinInstance* apSkin, NiGeometryBufferData* apRendererData, const NiPropertyState* apState, const NiDynamicEffectState* apEffects, const NiTransform& kWorld, const NiBound& kWorldBound);
	virtual UInt32						SetupTransformations(NiGeometry* apGeometry, const NiSkinInstance* apSkin, const void* apPartition, NiGeometryBufferData* apRendererData, const NiPropertyState* apState, const NiDynamicEffectState* apEffects, const NiTransform& kWorld, const NiBound& kWorldBound);
	virtual void						Func_25(NiGeometry* geo, const NiSkinInstance* skinInstance, const void* skinPartition, NiGeometryBufferData* rendererData, int shaderProperties, int a7, const NiTransform* worldTransform, NiBound* worldBound);
	virtual UInt32						SetupShaderPrograms(NiGeometry* apGeometry, const NiSkinInstance* apSkin, const void* apPartition, NiGeometryBufferData* apRendererData, const NiPropertyState* apState, const NiDynamicEffectState* apEffects, const NiTransform& kWorld, const NiBound& kWorldBound);
	virtual NiGeometryBufferData*		PrepareGeometryForRendering(NiGeometry* apGeometry, const void* apPartition, NiGeometryBufferData* apRendererData, const NiPropertyState* apState);
	virtual UInt32						PostRender(NiGeometry* apGeometry, const NiSkinInstance* apSkin, const void* apPartition, NiGeometryBufferData* apRendererData, const NiPropertyState* apState, const NiDynamicEffectState* apEffects, const NiTransform& kWorld, const NiBound& kWorldBound);
	virtual UInt32						PostProcessPipeline(NiGeometry* apGeometry, const NiSkinInstance* apSkin, NiGeometryBufferData* apRendererData, const NiPropertyState* apState, const NiDynamicEffectState* apEffects, const NiTransform& kWorld, const NiBound& kWorldBound);
	virtual void						SetupTextures(NiPropertyState* apShaderProps);
	virtual void						UpdateConstants(NiPropertyState* apShaderProps);
	virtual void						SetBlendAlpha(NiPropertyState* apShaderProps);
	virtual void						SetTestAlpha(NiPropertyState* apShaderProps);
	virtual void						Func_34(NiPropertyState* apShaderProps, bool bHasAlpha);
	virtual void						Func_35(NiPropertyState* shaderProps);
	virtual int							HasPasses();
	virtual int							EndPass();
	virtual void						StreamCanSkip();
	virtual void						Func_39();
	virtual void						Func_40();
	virtual void						Func_41();
	virtual void						Func_42();
	virtual void						SetShaderConstant();
	virtual void						Func_44();
	virtual void						SetShaderConstantArray();
	virtual void						Func_46();

	const char* m_kName;
	UInt32		m_uiImplementation;
	bool		m_bInitialized;
};

class NiD3DShaderInterface : public NiShader {
public:
	NiD3DShaderInterface();
	virtual ~NiD3DShaderInterface();

	virtual NiD3DShaderDeclaration* GetShaderDecl();
	virtual void					SetShaderDecl(NiD3DShaderDeclaration* apShaderDecl);
	virtual void					HandleResetDevice(void* apShaderDecl);
	virtual void					HandleLostDevice();
	virtual void					DestroyRendererData();
	virtual bool					RecreateRendererData();
	virtual bool					GetVSPresentAllPasses();
	virtual bool					Unk_54();
	virtual bool					Unk_55();
	virtual bool					Unk_56();
	virtual bool					GetIsBestImplementation();
	virtual void					SetIsBestImplementation(bool bIsBest);

	void*			m_pkD3DDevice;
	NiDX9Renderer*	m_pkD3DRenderer;
	void*			m_pkD3DRenderState;
	bool			m_bIsBestImplementation;
};

class NiD3DShader : public NiD3DShaderInterface {
public:
	virtual void ResetPasses();

	bool						m_bUsesNiRenderState;
	bool						m_bUsesNiLightState;
	NiD3DShaderDeclaration*		m_spShaderDecl;
	NiD3DRenderStateGroup*		m_pkRenderStateGroup;
	NiD3DShaderConstantMap*		m_spPixelConstantMap;
	NiD3DShaderConstantMap*		m_spVertexConstantMap;
	UInt32						m_uiCurrentPass;
};



class NiD3DDefaultShader : public NiD3DShader {
public:
	void*	Unk3C;
	UInt32	m_uiMaxTextureIndex;
	UInt32	m_uiRemainingStages;
	UInt32	m_uiRemainingTextures;
	UInt32	m_uiOldLightingValue;
	UInt32* m_peFogPassArray;
	UInt32	m_uiFogPassArraySize;
	void*	m_pkLastState;
};

class BSShader : public NiD3DDefaultShader {
public:
	virtual void SetRenderPass(UInt32 aeType);
	virtual void SetShaders(UInt32 aeType);
	// Seems to focus on tree shadows in most shaders
	virtual void Func_62(UInt32 aeType);
	// Seems to focus on the fog in most shaders
	virtual void Func_63(UInt32 aeType);
	virtual void RestoreSavedStates(UInt32 aeType);
	virtual void RestoreTechnique(UInt32 uiLastPass);
	// Checks for bUnk64
	virtual void ConfigureTextureStages();
	virtual void ConfigureAllTextureStages();
	virtual void Func_68(NiPropertyState* apShaderProps);
	virtual void Func_69();
	virtual NiDX9ShaderDeclaration* GetShaderDeclaration(NiGeometry*, BSShaderProperty* property);
	virtual void InitShaderConstants();
	// Reloads/ loads shader files and setups passes
	virtual void Reinitialize();
	virtual void ClearAllTextureStages();
	virtual void ClearTextureStages(NiD3DPass* apPass);
	virtual void Func_75();
	virtual void CreateNewPass();
	virtual void Func_77();
	virtual void Func_78(UInt32* apeType, int a2);

	NiD3DPass*	spPass;
	void*		pUnk60;
	bool		bUnk64;
	UInt32		iShaderType;
};


class NiDX9Renderer {
public:

   static NiDX9Renderer* GetSingleton() {
        return *(NiDX9Renderer**)0x11C73B4;
    }

   bool PrecacheGeometry(NiGeometry* apGeometry, UInt32 auiBonesPerPartition, UInt32 auiBonesPerVertex, void* apShaderDeclaration) {
	   return ThisStdCall(0xE73F60, this, apGeometry, auiBonesPerPartition, auiBonesPerVertex, apShaderDeclaration);
   }

   void PerformPrecache() {
	   ThisStdCall(0xE74120, this);
   }
};

class TESWorldSpace;
class BGSTerrainNode;

class BGSTerrainManager {
public:
	TESWorldSpace*		pWorld;
	BGSTerrainNode*		pRootNode;
	NiNode*				spLODRoot;

	void UpdateLODAnimations() {
		NiUpdateData kUpdateData = NiUpdateData(*(float*)0x11C3C08, true);
		spLODRoot->UpdateControllers(&kUpdateData);
	}
};

class BGSTerrainNode {
public:
	union Coordinate {
		UInt32 xy;
		struct {
			SInt16 x;
			SInt16 y;
		};
	};

	BGSTerrainManager*		pManager;
	UInt32					uiLODLevel;
	Coordinate				sBaseCellCoords;
	bool					bIsLvlMaxOrLower;
	UInt8					byte0D;
	UInt8					byte0E;
	UInt8					byte0F;
	BGSTerrainChunk*		pTerrainChunk;
	BGSDistantObjectBlock*	pObjectBlock;
	BGSDistantTreeBlock*	pTreeBlock;
	UInt32					ukn1C;
	BGSTerrainNode*			pChildren[4];
	BGSTerrainNode*			pParent;
	NiPoint3				pos34;
	float					fDist;
	float					fSplitDist;
	float					fMorphStartDist;
	float					fMorphEndDist;
	UInt32					uiTerrainFadeTimer;
	bool					bForceFade;
	UInt32					uiLinkID;
	bool					maybeShouldShow_5C;
	bool					byte5D;
	bool					byte5E;
	bool					byte5F;

	static void __fastcall UpdateBlockVisibility(BGSTerrainNode* apThis, void*, bool);

	bool IsPlayerInRange() {
		return ThisStdCall(0x6FEDE0, this);
	}

	SInt16 GetCellX() {
		return sBaseCellCoords.x;
	}

	SInt16 GetCellY() {
		return sBaseCellCoords.y;
	}
};

class BGSDistantObjectBlock {
public:
	BGSTerrainNode*			pTerrainNode;
	BSSegmentedTriShape*	spSegmentedTriShape;
	BSMultiBoundNode*		spMultiboundNode;
    void*					spNode0C; // old ones?
	BSMultiBoundNode*		spNode10;
    void*					spNode14;
    void*					spDistantObjectBlockLoadTask;
    void*					spTask2;
    bool					byte20;
    bool					bIsAddedToDistantBlocksList;
    bool					bPrepared;
    bool					bIsHigh;
    UInt8					byte24;
    UInt32					unk28;

	BSMultiBoundNode* GetMultiBound(bool abUnk) {
		if (abUnk)
			return spNode10;
		else
			return spMultiboundNode;
	}

	static void __fastcall Prepare(BGSDistantObjectBlock* apThis);

	void ShowRecurse(NiNode* apNode) {
		ThisStdCall(0x6F5A70, this, apNode);
	}
};

namespace BSShaderManager {
	static inline BSShader* GetShader(UInt32 aiShaderIndex) {
		return CdeclCall<BSShader*>(0xB55560, aiShaderIndex);
	}
}

class TESObjectCELL {
public:
	UInt32 filler[208 / 4];
	bool bCanHideLOD;
};

class TESWorldSpace {
public:
	TESObjectCELL* GetCellAtCoord(SInt32 x, SInt32 y) {
		return ThisStdCall<TESObjectCELL*>(0x5875A0, this, x, y);
	}

	BGSTerrainManager* GetTerrainManager() {
		return ThisStdCall<BGSTerrainManager*>(0x586170, this);
	}
};

class TES {
public:
	UInt32			filler[34];
	TESWorldSpace*	pWorldSpace;

	static TES* GetSingleton() {
		return *(TES**)0x11DEA10;
	};

	// 0x4FD3E0
	static TESWorldSpace* GetWorldSpace() {
		return GetSingleton()->pWorldSpace;
	}

	// 0x4511E0
	static bool IsCellLoaded(TESObjectCELL* apCell, bool abIgnoreBuffered) {
		return StdCall<bool>(0x4511E0, apCell, abIgnoreBuffered);
	}
};

template <typename T_Data>
class BSSimpleArray {
public:
	virtual			~BSSimpleArray();
	virtual T_Data* Allocate(UInt32 auiCount);
	virtual void    Deallocate(T_Data* apData);
	virtual T_Data* Reallocate(T_Data* apData, UInt32 auiCount);

	T_Data* pBuffer;
	UInt32	uiSize;
	UInt32	uiAllocSize;

	T_Data* GetAt(UInt32 idx) { return &pBuffer[idx]; }
};

template <class T_Key, class T_Data> class BSMapItem {
public:
	BSMapItem* m_pkNext;
	T_Key       m_key;
	T_Data      m_val;
};

template <class T_Key, class T_Data> class BSMapBase {
public:
	virtual                           ~BSMapBase();
	virtual UInt32                    KeyToHashIndex(T_Key key);
	virtual bool                      IsKeysEqual(T_Key key1, T_Key key2);
	virtual void                      SetValue(BSMapItem<T_Key, T_Data>* pkItem, T_Key key, T_Data data);
	virtual void                      ClearValue(BSMapItem<T_Key, T_Data>* pkItem);
	virtual BSMapItem<T_Key, T_Data>* NewItem();
	virtual void                      DeleteItem(BSMapItem<T_Key, T_Data>* pkItem);

	UInt32                      m_uiHashSize;
	BSMapItem<T_Key, T_Data>**	m_ppkHashTable;
	UInt32                      m_uiCount;

	bool GetAt(T_Key key, T_Data& dataOut) {
		UInt32 hashIndex = KeyToHashIndex(key);
		BSMapItem<T_Key, T_Data>* item = m_ppkHashTable[hashIndex];
		while (item) {
			if (IsKeysEqual(item->m_key, key)) {
				dataOut = item->m_val;
				return true;
			}
			item = item->m_pkNext;
		}
		return false;
	}
};

class BGSDistantTreeBlock {
public:
	struct InstanceData {
		NiPoint3	kPos;
		float		fRotZ;
		float		fScale;
		UInt32		uiFormID;
		UInt32		uiTreeGroupIndex;
		bool		bHidden;
	};

	struct TreeType {
		UInt32		uiType;
		NiPoint2	kSize;
		NiPoint2	kPosMin;
		NiPoint2	kPosMax;
		void*		spTriShapeData;
	};


	class TreeGroup {
	public:
		TreeType*						pTreeType;
		void*							spGeometry;
		BSSimpleArray<InstanceData>		kInstances;
		UInt32							uiNum;
		bool							bShaderPropertyUpToDate;
	};

	void* pLoadTask;
	BSSimpleArray<TreeGroup*>			kTreeGroups;
	BSMapBase<UInt32, InstanceData*>	kInstanceMap;
	BSMapBase<UInt32, TreeGroup*>		kNextGroup;
	BSSimpleArray<UInt32>				kUIntArray;
	BGSTerrainNode*						pNode;
	bool								bDoneLoading;
	bool								bAttached;
	bool								bAllVisible;

	static void __fastcall HideLOD(BGSDistantTreeBlock* apThis, void*, UInt32 aID, bool abAddToUIntArray);
};