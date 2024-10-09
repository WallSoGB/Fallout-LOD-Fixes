#pragma once

[[nodiscard]]
extern __declspec(allocator) void* BSNew(size_t stSize);
[[nodiscard]]
extern __declspec(allocator) void* BSNewAligned(size_t stAlign, size_t stSize);
[[nodiscard]]
extern __declspec(allocator) void* BSReallocate(void* pvMem, size_t stSize);
extern void  BSFree(void* pvMem);

extern size_t BSSize(void* pvMem);

template <typename T_Data>
[[nodiscard]]
__declspec(restrict) __declspec(allocator) T_Data* BSNew() {
	return static_cast<T_Data*>(BSNew(sizeof(T_Data)));
};

template <typename T_Data>
[[nodiscard]]
__declspec(restrict) __declspec(allocator) T_Data* BSNew(size_t aCount) {
	return static_cast<T_Data*>(BSNew(sizeof(T_Data) * aCount));
};

template <typename T, const UInt32 ConstructorPtr = 0, typename... Args>
[[nodiscard]]
__declspec(restrict)T* BSCreate(Args &&... args)
{
	auto* alloc = BSNew<T>();
	if constexpr (ConstructorPtr) {
		ThisStdCall(ConstructorPtr, alloc, std::forward<Args>(args)...);
	}
	else {
		memset(alloc, 0, sizeof(T));
	}
	return static_cast<T*>(alloc);
}

template <typename T, const UInt32 DestructorPtr = 0, typename... Args>
void BSDelete(T* t, Args &&... args) {
	if constexpr (DestructorPtr) {
		ThisStdCall(DestructorPtr, t, std::forward<Args>(args)...);
	}
	BSFree(t);
}

// Use this macro in your main.cpp to use the allocator
// -------------------------------------------------------------------------
#define BS_ALLOCATORS \
_VCRT_EXPORT_STD _NODISCARD _Ret_notnull_ _Post_writable_byte_size_(_Size) _VCRT_ALLOCATOR \
void* __CRTDECL operator new(size_t _Size) { return BSNew(_Size); } \
_VCRT_EXPORT_STD _NODISCARD _Ret_notnull_ _Post_writable_byte_size_(_Size) _VCRT_ALLOCATOR \
void* __CRTDECL operator new[](size_t _Size) { return BSNew(_Size); } \
_VCRT_EXPORT_STD void __CRTDECL operator delete(void* _Block) noexcept { BSFree(_Block); } \
_VCRT_EXPORT_STD void __CRTDECL operator delete(void* _Block, ::std::nothrow_t const&) noexcept { BSFree(_Block); } \
_VCRT_EXPORT_STD void __CRTDECL operator delete[](void* _Block) noexcept { BSFree(_Block); } \
_VCRT_EXPORT_STD void __CRTDECL operator delete[](void* _Block, ::std::nothrow_t const&) noexcept { BSFree(_Block); } \
_VCRT_EXPORT_STD void __CRTDECL operator delete(void* _Block, size_t _Size) noexcept { BSFree(_Block); } \
_VCRT_EXPORT_STD void __CRTDECL operator delete[](void* _Block, size_t _Size) noexcept { BSFree(_Block); }