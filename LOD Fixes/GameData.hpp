#pragma once

#include "SafeWrite.h"
#include "Utilities.h"

#define ASSERT_SIZE(a, b) static_assert(sizeof(a) == b, "Wrong structure size!");
#define ASSERT_OFFSET(a, b, c) static_assert(offsetof(a, b) == c, "Wrong member offset!");
#define CREATE_OBJECT(CLASS, ADDRESS) static CLASS* CreateObject() { return StdCall<CLASS*>(ADDRESS); };

// JIP macros to check types without virtual funcs
#define IS_NODE(object) ((*(UInt32**)object)[3 * 4 >> 2] == 0x6815C0)
#define IS_FADENODE(object) ((*(UInt32**)object)[4 * 4 >> 2] == 0x6815C0)
#define IS_MULTIBOUNDNODE(object) ((*(UInt32**)object)[5 * 4 >> 2] == 0x6815C0)
#define IS_GEOMETRY(object) ((*(UInt32**)object)[6 * 4 >> 2] == 0xE68810)
#define IS_TRIGEOMETRY(object) ((*(UInt32**)object)[7 * 4 >> 2] == 0xE68810)
#define IS_TRISTRIPS(object) ((*(UInt32**)object)[8 * 4 >> 2] == 0xE68810)
#define IS_TRISHAPE(object) ((*(UInt32**)object)[9 * 4 >> 2] == 0xE68810)
#define IS_SEGMENTEDTRISHAPE(object) ((*(UInt32**)object)[10 * 4 >> 2] == 0xE68810)
#define IS_RESIZABLETRISHAPE(object) ((*(UInt32**)object)[11 * 4 >> 2] == 0xE68810)
#define IS_PARTICLE(object) ((*(UInt32**)object)[12 * 4 >> 2] == 0xE68810)
#define IS_LINE(object) ((*(UInt32**)object)[13 * 4 >> 2] == 0xE68810)
#define IS_NICOLLISION(object) ((*(UInt32**)object)[14 * 4 >> 2] == 0x6815C0)
#define IS_BLENDCOLLISION(object) ((*(UInt32**)object)[15 * 4 >> 2] == 0xE68810)
#define IS_RIGIDBODY(object) ((*(UInt32**)object)[16 * 4 >> 2] == 0x6815C0)
#define IS_HINGECONSTRAINT(object) ((*(UInt32**)object)[17 * 4 >> 2] == 0xE68810)
#define IS_CONTROLLERMANAGER(object) ((*(UInt32**)object)[34 * 4 >> 2] == 0xE68810)

#define IS_PACKED(object) ((*(UInt32**)object)[35 * 4 >> 2] == 0x8D0360)

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
class NiObjectNET;
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
class TESObjectREFR;



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

class NiColor {
public:
	float r, g, b;
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

typedef void* BSMapIterator;

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
	virtual void                      SetValue(BSMapItem<T_Key, T_Data>* apTiem, T_Key key, T_Data data);
	virtual void                      ClearValue(BSMapItem<T_Key, T_Data>* apTiem);
	virtual BSMapItem<T_Key, T_Data>* NewItem();
	virtual void                      DeleteItem(BSMapItem<T_Key, T_Data>* apTiem);

	UInt32                      m_uiHashSize;
	BSMapItem<T_Key, T_Data>** m_ppkHashTable;
	UInt32                      m_uiCount;

	bool GetAt(T_Key key, T_Data& dataOut) {
		UInt32 index = KeyToHashIndex(key);
		BSMapItem<T_Key, T_Data>* item = m_ppkHashTable[index];
		while (item) {
			if (IsKeysEqual(item->m_key, key)) {
				dataOut = item->m_val;
				return true;
			}
			item = item->m_pkNext;
		}
		return false;
	}

	BSMapIterator GetFirstPos() const {
		for (UInt32 i = 0; i < m_uiHashSize; i++) {
			if (m_ppkHashTable[i])
				return m_ppkHashTable[i];
		}
		return 0;
	}

	void GetNext(BSMapIterator& pos, T_Key& key, T_Data& val) {
		BSMapItem<T_Key, T_Data>* item = (BSMapItem<T_Key, T_Data>*) pos;

		key = item->m_key;
		val = item->m_val;

		if (item->m_pkNext) {
			pos = item->m_pkNext;
			return;
		}

		UInt32 i = KeyToHashIndex(item->m_key);
		for (++i; i < m_uiHashSize; i++) {
			item = m_ppkHashTable[i];
			if (item) {
				pos = item;
				return;
			}
		}

		pos = 0;
	}
};


template <class T_Data>
class NiPointer {
public:
	NiPointer(T_Data* apObject = (T_Data*)0) {
		m_pObject = apObject;
		if (m_pObject)
			m_pObject->IncRefCount();
	}
	NiPointer(const NiPointer& arPointer) {
		m_pObject = arPointer.m_pObject;
		if (m_pObject)
			m_pObject->IncRefCount();
	}
	~NiPointer() {
		if (m_pObject)
			m_pObject->DecRefCount();
	}

	T_Data* m_pObject;

	__forceinline NiPointer<T_Data>& operator =(const NiPointer& arPointer) {
		if (m_pObject != arPointer.m_pObject) {
			if (m_pObject)
				m_pObject->DecRefCount();
			m_pObject = arPointer.m_pObject;
			if (m_pObject)
				m_pObject->IncRefCount();
		}
		return *this;
	}

	__forceinline NiPointer<T_Data>& operator =(T_Data* apObject) {
		if (m_pObject != apObject) {
			if (m_pObject)
				m_pObject->DecRefCount();
			m_pObject = apObject;
			if (m_pObject)
				m_pObject->IncRefCount();
		}
		return *this;
	}

	__forceinline bool operator==(T_Data* apObject) const { return (m_pObject == apObject); }
	__forceinline bool operator==(const NiPointer& arPointer) const { return (m_pObject == arPointer.m_pObject); }
	__forceinline operator bool() const { return m_pObject != nullptr; }
	__forceinline operator T_Data* () const { return m_pObject; }
	__forceinline T_Data& operator*() const { return *m_pObject; }
	__forceinline T_Data* operator->() const { return m_pObject; }
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

	// 0x6532C0
	bool IsKindOf(const NiRTTI& apRTTI) const {
		for (const NiRTTI* i = GetRTTI(); i; i = i->m_pkBaseRTTI) {
			if (i == &apRTTI)
				return true;
		}
		return false;
	}
};

class NiTimeController : public NiObject {
public:
	NiTimeController();
	virtual ~NiTimeController();

	virtual void				Start(float afTime = INFINITY);	
	virtual void				Stop();							
	virtual void				Update(NiUpdateData& arUpdateData);
	virtual void				SetTarget(NiObjectNET* apTarget);
	virtual bool				IsTransformController() const;	
	virtual bool				IsVertexController() const;		
	virtual float				ComputeScaledTime(float fTime);	
	virtual void				OnPreDisplay() const;			
	virtual bool				IsStreamable() const;			
	virtual bool				TargetIsRequiredType() const;	

	enum AnimType {
		APP_TIME,
		APP_INIT
	};

	enum CycleType {
		LOOP,
		REVERSE,
		CLAMP,
		MAX_CYCLE_TYPES
	};

	Bitfield16						m_usFlags;
	float							m_fFrequency;
	float							m_fPhase;
	float							m_fLoKeyTime;
	float							m_fHiKeyTime;
	float							m_fStartTime;
	float							m_fLastTime;
	float							m_fWeightedLastTime;
	float							m_fScaledTime;
	NiObjectNET*					m_pkTarget;
	NiPointer<NiTimeController>		m_spNext;
};

class NiObjectNET : public NiObject {
public:
	const char*						m_kName;
	NiPointer<NiTimeController>		m_spControllers;
	void**							m_ppkExtra;
	UInt16							m_usExtraDataSize;
	UInt16							m_usMaxSize;
};

class NiAVObject : public NiObjectNET {
public:
	virtual void			UpdateControllers(NiUpdateData& arData);
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
        APP_CULLED                              = 1u <<  0,
        SELECTIVE_UPDATE                        = 1u <<  1,
        SELECTIVE_UPDATE_TRANSFORMS             = 1u <<  2,
        SELECTIVE_UPDATE_CONTROLLER             = 1u <<  3,
        SELECTIVE_UPDATE_RIGID                  = 1u <<  4,
        DISPLAY_OBJECT                          = 1u <<  5,
        DISABLE_SORTING                         = 1u <<  6,	// Gamebryo's sorter is used only on Tiles
        SELECTIVE_UPDATE_TRANSFORMS_OVERRIDE    = 1u <<  7,
		UNK_8									= 1u <<  8,
        SAVE_EXTERNAL_GEOM_DATA                 = 1u <<  9,
        NO_DECALS                               = 1u << 10,
        ALWAYS_DRAW                             = 1u << 11,
        ACTOR_NODE                              = 1u << 12,
        FIXED_BOUND                             = 1u << 13,
        TOP_FADE_NODE                           = 1u << 14,
        IGNORE_FADE                             = 1u << 15,
        LOD_FADING_OUT                          = 1u << 16,
        HAS_MOVING_SOUND                        = 1u << 17,
		HAS_PROPERTY_CONTROLLER					= 1u << 18,
        HAS_BOUND		                        = 1u << 19,
        ACTOR_CULLED                            = 1u << 20,
        IGNORES_PICKING			                = 1u << 21,
        RENDER_USE                              = 1u << 22,
		UNK_23									= 1u << 23,
        UNK_24		                            = 1u << 24,
		UNK_25									= 1u << 25,
		UNK_26									= 1u << 26,
        UNK_27                                  = 1u << 27,
        UNK_28                                  = 1u << 28,
        IS_POINTLIGHT                           = 1u << 29, // Added by JIP
        DONE_INIT_LIGHTS                        = 1u << 30, // Added by JIP
        IS_INSERTED                             = 1u << 31  // Added by JIP
	};

	NiNode*							m_pkParent;
	bhkNiCollisionObject*			m_spCollisionObject;
	NiBound*						m_kWorldBound;
	NiTListBase<NiProperty*>		m_kPropertyList;
	Bitfield32						m_uiFlags;
	NiTransform						m_kLocal;
	NiTransform						m_kWorld;

	// FLAGS

	void SetBit(bool bVal, UInt32 uMask) {
		m_uiFlags.SetBit(uMask, bVal);
	}

	bool GetBit(UInt32 uMask) const {
		return m_uiFlags.GetBit(uMask);
	}

	void SetAppCulled(bool bVal) {
		SetBit(bVal, APP_CULLED);
	}

	bool GetAppCulled() const {
		return GetBit(APP_CULLED);
	}

	void SetSelectiveUpdate(bool bVal) {
		SetBit(bVal, SELECTIVE_UPDATE);
	}

	bool GetSelectiveUpdate() const {
		return GetBit(SELECTIVE_UPDATE);
	}

	void SetSelUpdTransforms(bool bVal) {
		SetBit(bVal, SELECTIVE_UPDATE_TRANSFORMS);
	}

	bool GetSelUpdTransforms() const {
		return GetBit(SELECTIVE_UPDATE_TRANSFORMS);
	}

	void SetSelUpdController(bool bVal) {
		SetBit(bVal, SELECTIVE_UPDATE_CONTROLLER);
	}

	bool GetSelUpdController() const {
		return GetBit(SELECTIVE_UPDATE_CONTROLLER);
	}

	void SetSelUpdRigid(bool bVal) {
		SetBit(bVal, SELECTIVE_UPDATE_RIGID);
	}

	bool GetSelUpdRigid() const {
		return GetBit(SELECTIVE_UPDATE_RIGID);
	}

	void SetDisplayObject(bool bVal) {
		SetBit(bVal, DISPLAY_OBJECT);
	}

	bool GetDisplayObject() const {
		return GetBit(DISPLAY_OBJECT);
	}

	void SetDisableSorting(bool bVal) {
		SetBit(bVal, DISABLE_SORTING);
	}

	bool GetDisableSorting() const {
		return GetBit(DISABLE_SORTING);
	}

	void SetSelUpdTransformsOverride(bool bVal) {
		SetBit(bVal, SELECTIVE_UPDATE_TRANSFORMS_OVERRIDE);
	}

	bool GetSelUpdTransformsOverride() const {
		return GetBit(SELECTIVE_UPDATE_TRANSFORMS_OVERRIDE);
	}

	void SetSaveExternalGeomData(bool bVal) {
		SetBit(bVal, SAVE_EXTERNAL_GEOM_DATA);
	}

	bool GetSaveExternalGeomData() const {
		return GetBit(SAVE_EXTERNAL_GEOM_DATA);
	}

	void SetNoDecals(bool bVal) {
		SetBit(bVal, NO_DECALS);
	}

	bool GetNoDecals() const {
		return GetBit(NO_DECALS);
	}

	void SetAlwaysDraw(bool bVal) {
		SetBit(bVal, ALWAYS_DRAW);
	}

	bool GetAlwaysDraw() const {
		return GetBit(ALWAYS_DRAW);
	}

	void SetActorNode(bool bVal) {
		SetBit(bVal, ACTOR_NODE);
	}

	bool GetActorNode() const {
		return GetBit(ACTOR_NODE);
	}

	void SetFixedBound(bool bVal) {
		SetBit(bVal, FIXED_BOUND);
	}

	bool GetFixedBound() const {
		return GetBit(FIXED_BOUND);
	}

	void SetTopFadeNode(bool bVal) {
		SetBit(bVal, TOP_FADE_NODE);
	}

	bool GetTopFadeNode() const {
		return GetBit(TOP_FADE_NODE);
	}

	void SetIgnoreFade(bool bVal) {
		SetBit(bVal, IGNORE_FADE);
	}

	bool GetIgnoreFade() const {
		return GetBit(IGNORE_FADE);
	}

	void SetLODFadingOut(bool bVal) {
		SetBit(bVal, LOD_FADING_OUT);
	}

	bool GetLODFadingOut() const {
		return GetBit(LOD_FADING_OUT);
	}

	void SetHasMovingSound(bool bVal) {
		SetBit(bVal, HAS_MOVING_SOUND);
	}

	bool GetHasMovingSound() const {
		return GetBit(HAS_MOVING_SOUND);
	}

	void SetHasPropertyController(bool bVal) {
		SetBit(bVal, HAS_PROPERTY_CONTROLLER);
	}

	bool GetHasPropertyController() const {
		return GetBit(HAS_PROPERTY_CONTROLLER);
	}

	void SetHasBound(bool bVal) {
		SetBit(bVal, HAS_BOUND);
	}

	bool GetHasBound() const {
		return GetBit(HAS_BOUND);
	}

	void SetActorCulled(bool bVal) {
		SetBit(bVal, ACTOR_CULLED);
	}

	bool GetActorCulled() const {
		return GetBit(ACTOR_CULLED);
	}

	void SetIgnoresPicking(bool bVal) {
		SetBit(bVal, IGNORES_PICKING);
	}

	bool GetIgnoresPicking() const {
		return GetBit(IGNORES_PICKING);
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
	NiProperty* GetProperty(UInt32 auiPropertyType) const {
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

	bool HasPropertyController() const {
		return ThisStdCall<bool>(0xA5A110, this);
	}

	NiNode* GetParent() const {
		return m_pkParent;
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

    UInt32 GetChildCount() const {
		return m_kChildren.m_usESize;
    }

	UInt32 GetArrayCount() const {
		return m_kChildren.m_usSize;
	}

	NiAVObject* GetAt(UInt32 index) const {
		if (index >= GetArrayCount())
			return nullptr;

		return m_kChildren.m_pBase[index];
	}

	static bool HasControllers(const NiAVObject* apObject) {
		bool bHasControllers = false;
		bHasControllers = apObject->m_spControllers;

		if (bHasControllers)
			return true;

		if (apObject->IsKindOf(*(NiRTTI*)0x11F4280))
			bHasControllers = apObject->HasPropertyController();

		if (bHasControllers)
			return true;

		if (IS_NODE(apObject)) {
			const NiNode* pNode = static_cast<const NiNode*>(apObject);
			for (UInt32 i = 0; i < pNode->GetArrayCount(); i++) {
				NiAVObject* pChild = pNode->GetAt(i);
				if (pChild && HasControllers(pChild))
					return true;
			}
		}
		
		return false;
	}
};

class BSNiNode : public NiNode {
public:
	virtual ~BSNiNode();
	virtual void ReparentSkinInstances(NiNode* apNode, NiAVObject* apParent);
};

class BSMultiBoundShape : public NiObject {
public:
	BSMultiBoundShape();
	virtual ~BSMultiBoundShape();
	virtual int		GetType();

	enum BSMBShapeType {
		BSMB_SHAPE_NONE = 0,
		BSMB_SHAPE_AABB = 1,
		BSMB_SHAPE_OBB = 2,
		BSMB_SHAPE_SPHERE = 3,
		BSMB_SHAPE_CAPSULE = 4,
	};

	UInt32 eCullResult;
};

class BSMultiBoundAABB : public BSMultiBoundShape {
public:
	BSMultiBoundAABB();
	virtual ~BSMultiBoundAABB();

	NiPoint3 Center;
	NiPoint3 HalfExtents;
};


class BSMultiBound : public NiObject {
public:
	BSMultiBound();
	virtual ~BSMultiBound();

	UInt32							uiBoundFrameCount;
	NiPointer<BSMultiBoundShape>	spShape;

	BSMultiBoundShape* GetShape() const {
		return spShape.m_pObject;
	}
};

class BSMultiBoundNode : public BSNiNode {
public:
	virtual BSMultiBoundRoom*	GetMultiBoundRoom();
	virtual bool				GetPointWithin(NiPoint3& akPoint);
	virtual UInt32				CheckBound(BSMultiBound*);
	virtual UInt32				CheckBoundAlt(NiBound*);

	NiPointer<BSMultiBound>		spMultiBound;
	UInt32						uiCullingMode;

	BSMultiBound* GetMultiBound() const {
		return spMultiBound.m_pObject;
	}
};

class NiProperty : public NiObjectNET {
public:
	virtual UInt32	Type();
	virtual void	Update(NiUpdateData* pUpdateData);

	enum PropertyType {
		ALPHA,
		CULLING,
		MATERIAL,
		SHADE,
		STENCIL,
		TEXTURING,
		MAX_TYPES
	};
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

class NiMaterialProperty : public NiProperty {
public:
	NiMaterialProperty();
	virtual ~NiMaterialProperty();

	SInt32		m_iIndex;
	NiColor		m_spec;
	NiColor		m_emit;
	NiColor*	m_pExternalEmittance;
	float		m_fShine;
	float		m_fAlpha;
	float		m_fEmitMult;
	UInt32		m_uiRevID;
	void*		m_pvRendererData;

	CREATE_OBJECT(NiMaterialProperty, 0xA756D0); 
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
		MUTABLE				= 0x0000,
		STATIC				= 0x4000,
		CONSISTENCY_MASK	= 0x7000,
	};

	enum KeepFlags {
		KEEP_NONE		= 0,
		KEEP_XYZ		= 1 << 0,
		KEEP_NORM		= 1 << 1,
		KEEP_COLOR		= 1 << 2,
		KEEP_UV			= 1 << 3,
		KEEP_INDICES	= 1 << 4,
		KEEP_BONEDATA	= 1 << 5,
		KEEP_ALL		= 0x3F,
	};

	enum Compression {
		COMPRESS_NORM		= 1 << 0,
		COMPRESS_COLOR		= 1 << 1,
		COMPRESS_UV			= 1 << 2,
		COMPRESS_WEIGHT		= 1 << 3,
		COMPRESS_POSITION	= 1 << 4,
		COMPRESS_ALL		= 0x1F,
	};

	enum MarkAsChangedFlags {
		VERTEX_MASK		= 1 << 0,
		NORMAL_MASK		= 1 << 1,
		COLOR_MASK		= 1 << 2,
		TEXTURE_MASK	= 1 << 3,
		ALL_MASK		= VERTEX_MASK | NORMAL_MASK | COLOR_MASK | TEXTURE_MASK,
		DIRTY_MASK		= 0xFFF,
	};

	UInt16								m_usVertices;
	UInt16								m_usID;
	UInt16								m_usDataFlags;
	UInt16								m_usDirtyFlags;
	NiBound								m_kBound;
	NiPoint3*							m_pkVertex;
	NiPoint3*							m_pkNormal;
	NiColorA*							m_pkColor;
	NiPoint2*							m_pkTexture;
	NiPointer<NiAdditionalGeometryData>	m_spAdditionalGeomData;
	NiGeometryBufferData*				m_pkBuffData;
	UInt8								m_ucKeepFlags;
	UInt8								m_ucCompressFlags;
	bool								bIsReadingData;
	bool								bUnk3B;
	bool								bCanSave;

	__forceinline void SetKeepFlags(KeepFlags aeFlags) {
		m_ucKeepFlags = aeFlags;
	}

	__forceinline void SetCompressFlags(Compression aeFlags) {
		m_ucCompressFlags = aeFlags;
	}

	__forceinline void SetConsistency(Consistency aeFlags) {
		if (!m_spAdditionalGeomData.m_pObject || !IS_PACKED(m_spAdditionalGeomData.m_pObject))
			m_usDirtyFlags = (m_usDirtyFlags & ~CONSISTENCY_MASK) | aeFlags;
	}

	__forceinline NiPoint3* GetVertices() const {
		return m_pkVertex;
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
		UInt32	uiStartIndex;
		UInt32	uiNumPrimitives;
		bool	bIsEnabled;
		UInt32	uiTriCount;
		bool	bVisible;
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
class TESObjectREFR;

class BGSTerrainManager {
public:
	TESWorldSpace*		pWorld;
	BGSTerrainNode*		pRootNode;
	NiNode*				spLODRoot;
	NiNode*				spWaterLODNode;
	UInt32				kCoordNW;
	UInt32				kCoordSE;
	UInt32				uiMaxLevel;
	UInt32				uiMinLevel;
	UInt32				uiRootLevel;
	UInt32				uiLODLevel;
	bool				bNeedsImmediateUpdate;
	bool				bHasLOD;
	bool				byte2A;
	bool				byte2B;
	BSSimpleArray<TESObjectREFR*> kTreeRefs; // 0x2C

	void Update(const NiPoint3* apPos, UInt32 aeUpdateType) {
		ThisStdCall(0x6FCA90, this, apPos, aeUpdateType);
	}

	BGSDistantTreeBlock* GetDistantTreeBlock(NiPoint3* apPos) {
		return ThisStdCall<BGSDistantTreeBlock*>(0x6FCEF0, this, apPos);
	}

	static void __fastcall HideTreeLOD(BGSTerrainManager* apThis, void*, TESObjectREFR* apRefr, bool abRegisterRefr);
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

	void UpdateBlockVisibility(bool);

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

class BGSTerrainChunk {
public:
	BGSTerrainNode*							pNode;
	NiPointer<NiGeometry>					spLandMesh;
	NiPointer<NiGeometry>					spWaterMesh;
	NiPointer<NiGeometry>					spWaterReflectMesh;
	NiPointer<BSMultiBoundNode>				spLandMultiBoundNode;
	NiPointer<BSMultiBoundNode>				spWaterMultiBoundNode;
	NiPointer<BSMultiBoundNode>				spWaterReflectMultiBoundNode;
	// Cut off here...

	void AttachWaterLOD(bool abForce);

	void InitializeShaderProperty();
};

class BGSDistantObjectBlock {
public:
	BGSTerrainNode*					pTerrainNode;
	NiPointer<BSSegmentedTriShape>	spShape;
	NiPointer<BSMultiBoundNode>		spBlock;
	NiPointer<BSSegmentedTriShape>	spPreviousShape;
	NiPointer<BSMultiBoundNode>		spStingerBlock;
	NiPointer<BSMultiBoundNode>		spPreviousBlock;
    void*							spLoadTask;
    void*							spStingerLoadTask;
    bool							bSwapBlocks;
    bool							bIsAddedToDistantBlocksList;
    bool							bPrepared;
    bool							bIsHigh;
    UInt8							byte24;
    UInt32							unk28;

	BSMultiBoundNode* GetBlock(const bool abStinger) const {
		if (abStinger)
			return spStingerBlock.m_pObject;
		else
			return spBlock.m_pObject;
	}

	void Prepare();

	void PrepareStinger(NiAVObject* apObject) {
		ThisStdCall(0x6F6A90, this, apObject);
	}

	void ShowRecurse(NiNode* apNode) const {
		ThisStdCall(0x6F5A70, this, apNode);
	}

	void ToggleVisibilityRecurse(NiNode* apNode, UInt32 auiLODLevel, SInt16 asCellX, SInt16 asCellY) const {
		ThisStdCall(0x6F5970, this, apNode, auiLODLevel, asCellX, asCellY);
	}
};

namespace BSShaderManager {
	static inline BSShader* GetShader(UInt32 aiShaderIndex) {
		return CdeclCall<BSShader*>(0xB55560, aiShaderIndex);
	}

	static inline void AssignShaders(NiAVObject* apObject, bool abKeepUV, bool abNoPrecache) {
		CdeclCall(0xB57E30, apObject, abKeepUV, abNoPrecache);
	}

	static float GetLODLandDrop() {
		return *(float*)0x11AD808;
	}

	static void SetLODLandDrop(float afDrop) {
		*(float*)0x11AD808 = afDrop;
	}
}

class TESObjectCELL {
public:
	UInt32	filler[0x26 / 4];
	UInt8	ucCellFlags;
	UInt8	ucCellGameFlags;
	UInt8	ucCellState; // 0x26
	UInt32	filler2[42];
	bool	bCanHideLOD; // 0xD0

	enum CellState : UInt32 {
		CS_NOT_LOADED	= 0,
		CS_UNLOADING	= 1,
		CS_LOADING		= 2,
		CS_LOADED		= 3,
		CS_DETACHING	= 4,
		CS_ATTACHING	= 5,
		CS_ATTACHED		= 6,
	};


	bool IsAttached() const {
		return ucCellState == CS_ATTACHED;
	}
};

class TESWorldSpace {
public:
	TESObjectCELL* GetCellAtCoord(SInt32 x, SInt32 y) const {
		return ThisStdCall<TESObjectCELL*>(0x5875A0, this, x, y);
	}

	BGSTerrainManager* GetTerrainManager() const {
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
	BSSimpleArray<UInt32>				kFormIDs;
	BGSTerrainNode*						pNode;
	bool								bDoneLoading;
	bool								bAttached;
	bool								bAllVisible;

	static void __fastcall HideLOD(BGSDistantTreeBlock* apThis, void*, UInt32 aID, bool abRegisterFormID);
};

class TESFile {
public:
	UInt32	padding[0x20 / 4];
	char	cFilename[260];
	char	cPath[260];

	void* GetListMasters() const {
		return ThisStdCall<void*>(0x464DF0, this);
	}

	bool HasMasters() const {
		return !ThisStdCall<bool>(0x8256D0, GetListMasters());
	}
};
ASSERT_OFFSET(TESFile, cFilename, 0x20);

class TESForm {
public:
	void*	vtable;
	UInt8	cFormType;
	UInt8	jipFormFlags1;
	UInt8	jipFormFlags2;
	UInt8	jipFormFlags3;
	UInt32	iFormFlags;
	UInt32	uiFormID;
	UInt32	kMods[2];

	UInt32 GetFormID() const {
		return uiFormID;
	}

	UInt32 GetMasterCount() const {
		return ThisStdCall(0x5504E0, this);
	}

	TESFile* GetFile(SInt32 aiFile) const {
		return ThisStdCall<TESFile*>(0x484E60, this, aiFile);
	}
};

class TESObjectREFR : public TESForm {
public:
	UInt32 padding[0x28 / 4];
	TESObjectCELL* pParentCell;

	NiPoint3* GetPos() const {
		return ThisStdCall<NiPoint3*>(0x436AA0, this);
	}
};

ASSERT_OFFSET(TESObjectREFR, pParentCell, 0x40);

class Actor : public TESObjectREFR {
public:
	NiPoint3* GetPos() const {
		return ThisStdCall<NiPoint3*>(0x8AE4C0, this);
	}
};

class PlayerCharacter : public Actor {
public:
	struct FlyCamData {
		float		fRotX;
		float		fRotZ;
		NiPoint3	kPosition;
	};

	UInt32		padding2[0x79C / 4];
	FlyCamData	kFlycamPos;

	static PlayerCharacter* GetSingleton() {
		return *(PlayerCharacter**)0x11DEA3C;
	}

	TESObjectCELL* GetParentCell() const {
		return pParentCell;
	}
};

ASSERT_OFFSET(PlayerCharacter, kFlycamPos, 0x7E0);

class TESMain {
public:
	bool					bOneMore;
	bool					bQuitGame;
	bool					bExitToMainMenu;
	bool					bGameActive;
	bool					unk04;
	bool					unk05;
	bool					bIsFlyCam;
	bool					bFreezeTime;

	static __forceinline TESMain* GetSingleton() { return *(TESMain**)0x11DEA0C; };
};