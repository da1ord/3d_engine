// This code contains NVIDIA Confidential Information and is disclosed to you
// under a form of NVIDIA software license agreement provided separately to you.
//
// Notice
// NVIDIA Corporation and its licensors retain all intellectual property and
// proprietary rights in and to this software and related documentation and
// any modifications thereto. Any use, reproduction, disclosure, or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA Corporation is strictly prohibited.
//
// ALL NVIDIA DESIGN SPECIFICATIONS, CODE ARE PROVIDED "AS IS.". NVIDIA MAKES
// NO WARRANTIES, EXPRESSED, IMPLIED, STATUTORY, OR OTHERWISE WITH RESPECT TO
// THE MATERIALS, AND EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES OF NONINFRINGEMENT,
// MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Information and code furnished is believed to be accurate and reliable.
// However, NVIDIA Corporation assumes no responsibility for the consequences of use of such
// information or for any infringement of patents or other rights of third parties that may
// result from its use. No license is granted by implication or otherwise under any patent
// or patent rights of NVIDIA Corporation. Details are subject to change without notice.
// This code supersedes and replaces all information previously supplied.
// NVIDIA Corporation products are not authorized for use as critical
// components in life support devices or systems without express written approval of
// NVIDIA Corporation.
//
// Copyright (c) 2008-2013 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  


#ifndef PX_FOUNDATION_PX_PREPROCESSOR_H
#define PX_FOUNDATION_PX_PREPROCESSOR_H

#include <stddef.h>
/** \addtogroup foundation
  @{
*/


/**
List of preprocessor defines used to configure the SDK
- PX_DEBUG: enable asserts (exactly one needs to be defined)
- PX_CHECKED: enable run time checks, mostly unused or equiv. to PX_DEBUG
- PX_SUPPORT_VISUAL_DEBUGGER: ...
- AG_PERFMON: ... (Deprecated)
*/

/**
Compiler define
*/
#ifdef _MSC_VER 
#	define PX_VC
#   if _MSC_VER >= 1600
#       define PX_VC10
#   elif _MSC_VER >= 1500
#       define PX_VC9
#	elif _MSC_VER >= 1400
#		define PX_VC8
#	elif _MSC_VER >= 1300
#		define PX_VC7
#	else
#		define PX_VC6
#	endif
#elif defined(__ghs__)
#   define PX_GHS
#elif __GNUC__ || __SNC__
#	define PX_GNUC
#else
#	error "Unknown compiler"
#endif


/**
Platform define
*/
#ifdef PX_VC
#	ifdef XBOXONE
#		define PX_XBOXONE
#		define PX_X64
#	elif defined(_M_IX86)
#		define PX_X86
#		define PX_WINDOWS
#   elif defined(_M_X64)
#       define PX_X64
#       define PX_WINDOWS
#	elif defined(_M_PPC)
#		define PX_PPC
#		define PX_X360
#		define PX_VMX
#   elif defined(_M_ARM)
#		define PX_ARM
#       define PX_ARM_NEON
#	else
#		error "Unknown platform"
#	endif
#   if defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_PARTITION_APP
#		define PX_WINMODERN
#	endif
#elif defined PX_GNUC
#   ifdef __CELLOS_LV2__
#	define PX_PS3
#		define PX_VMX
#   elif defined(__arm__)
#		define PX_ARM
#		if defined(__SNC__)
#			define PX_PSP2
#		endif
#		if defined(__ARM_NEON__)
#			define PX_ARM_NEON
#		endif
#   elif defined(__i386__)
#       define PX_X86
#		define PX_VMX
#   elif defined(__x86_64__)
#       ifdef __PS4__
#         define PX_PS4
#		  define PX_X64
#       else
#       define PX_X64
#       endif
#   elif defined(__ppc__)
#       define PX_PPC
#   elif defined(__ppc64__)
#       define PX_PPC
#	define PX_PPC64
#   else
#	error "Unknown platform"
#   endif
#	if defined(ANDROID)
#   	define PX_ANDROID
#   	define PX_UNIX
#	elif defined(__linux__)
#   	define PX_LINUX
#   	define PX_UNIX
#	elif defined(__APPLE__)
#   	define PX_APPLE
#   	define PX_UNIX
#		if defined(__arm__)
#			define PX_APPLE_IOS
#		endif
#	elif defined(__CYGWIN__)
#   	define PX_CYGWIN
#   	define PX_LINUX
#   	define PX_UNIX
#	endif
#elif defined PX_GHS
#	define PX_WIIU
#endif


/**
DLL export macros
*/
#if !defined(PX_C_EXPORT) 
#	if defined(PX_WINDOWS) || defined(PX_WINMODERN)
#		define PX_C_EXPORT extern "C"
#	else
#		define PX_C_EXPORT
#	endif
#endif

/**
Define API function declaration

PX_FOUNDATION_EXPORTS - used by the DLL library (PhysXCommon) to export the API
PX_FOUNDATION_NO_EXPORTS - exists because there are windows configurations where 
						   the PX_FOUNDATION_API is linked through standard static linking
no definition - this will allow DLLs and libraries to use the exported API from PhysXCommon				   

*/
#if (defined(PX_WINDOWS) || defined(PX_WINMODERN) ) && !defined(__CUDACC__)
	#if defined PX_FOUNDATION_EXPORTS
		#define PX_FOUNDATION_API __declspec(dllexport)
	#elif defined PX_FOUNDATION_NO_EXPORTS
		#define PX_FOUNDATION_API
	#else
		#define PX_FOUNDATION_API __declspec(dllimport)
	#endif
#else
	#define PX_FOUNDATION_API 
#endif

/**
Calling convention
*/
#ifndef PX_CALL_CONV
#	if defined PX_WINDOWS
#		define PX_CALL_CONV __cdecl
#	else
#		define PX_CALL_CONV
#	endif
#endif

/**
Pack macros - disabled on SPU because they are not supported
*/
#if defined(PX_VC)
#	define PX_PUSH_PACK_DEFAULT	__pragma( pack(push, 8) )
#	define PX_POP_PACK			__pragma( pack(pop) )
#elif (defined(PX_GNUC) && !defined(__SPU__)) || defined(PX_GHS)
#	define PX_PUSH_PACK_DEFAULT	_Pragma("pack(push, 8)")
#	define PX_POP_PACK			_Pragma("pack(pop)")
#else
#	define PX_PUSH_PACK_DEFAULT
#	define PX_POP_PACK
#endif

/**
Inline macro
*/
#if defined(PX_WINDOWS) || defined(PX_X360) || defined(PX_WINMODERN) || defined(PX_XBOXONE)
#	define PX_INLINE inline
#	pragma inline_depth( 255 )
#else
#	define PX_INLINE inline
#endif

/**
Force inline macro
*/
#if defined(PX_VC)
	#define PX_FORCE_INLINE __forceinline
#elif defined(PX_LINUX) // Workaround; Fedora Core 3 do not agree with force inline and PxcPool
	#define PX_FORCE_INLINE inline
#elif defined(PX_GNUC) || defined(PX_GHS)
	#define PX_FORCE_INLINE inline __attribute__((always_inline))
#else
	#define PX_FORCE_INLINE inline
#endif

/**
Noinline macro
*/
#if defined PX_WINDOWS || defined PX_XBOXONE
#	define PX_NOINLINE __declspec(noinline)
#elif defined(PX_GNUC) || defined(PX_GHS)
#	define PX_NOINLINE __attribute__ ((noinline))
#else
#	define PX_NOINLINE 
#endif


/*! restrict macro */
#if __CUDACC__
#	define PX_RESTRICT __restrict__
#elif (defined(PX_GNUC) || defined(PX_VC) || defined(PX_GHS)) && !defined(PX_PS4) // ps4 doesn't like restricted functions
#	define PX_RESTRICT __restrict
#else
#	define PX_RESTRICT
#endif

#if defined(PX_WINDOWS) || defined(PX_X360) || defined(PX_WINMODERN) || defined(PX_XBOXONE)
#define PX_NOALIAS __declspec(noalias)
#else
#define PX_NOALIAS
#endif


/**
Alignment macros

PX_ALIGN_PREFIX and PX_ALIGN_SUFFIX can be used for type alignment instead of aligning individual variables as follows:
PX_ALIGN_PREFIX(16)
struct A {
...
} PX_ALIGN_SUFFIX(16);
This declaration style is parsed correctly by Visual Assist.

*/
#ifndef PX_ALIGN
	#if defined(PX_VC)
		#define PX_ALIGN(alignment, decl) __declspec(align(alignment)) decl
		#define PX_ALIGN_PREFIX(alignment) __declspec(align(alignment))
		#define PX_ALIGN_SUFFIX(alignment)
	#elif defined(PX_GNUC) || defined(PX_GHS) || defined(PX_APPLE_IOS)
		#define PX_ALIGN(alignment, decl) decl __attribute__ ((aligned(alignment)))
		#define PX_ALIGN_PREFIX(alignment)
		#define PX_ALIGN_SUFFIX(alignment) __attribute__ ((aligned(alignment)))
	#else
		#define PX_ALIGN(alignment, decl)
		#define PX_ALIGN_PREFIX(alignment)
		#define PX_ALIGN_SUFFIX(alignment)
	#endif
#endif

/**
Deprecated macro
- To deprecate a function: Place PX_DEPRECATED at the start of the function header (leftmost word).
- To deprecate a 'typdef', a 'struct' or a 'class': Place PX_DEPRECATED directly after the keywords ('typdef', 'struct', 'class').
*/
#if 0 // set to 1 to create warnings for deprecated functions
#	define PX_DEPRECATED __declspec(deprecated)
#else 
#	define PX_DEPRECATED
#endif

// VC6 no '__FUNCTION__' workaround
#if defined PX_VC6 && !defined __FUNCTION__
#	define __FUNCTION__	"Undefined"
#endif

/**
General defines
*/

// static assert
#define PX_COMPILE_TIME_ASSERT(exp)	typedef char PxCompileTimeAssert_Dummy[(exp) ? 1 : -1]

#if defined(PX_GNUC)
#define PX_OFFSET_OF(X, Y) __builtin_offsetof(X, Y)
#else
#define PX_OFFSET_OF(X, Y) offsetof(X, Y)
#endif

#define PX_SIZE_OF(Class, Member)			sizeof(((Class*)0)->Member)

// avoid unreferenced parameter warning (why not just disable it?)
// PT: or why not just omit the parameter's name from the declaration????
#define PX_UNUSED(_P) (void)(_P);

// check that exactly one of NDEBUG and _DEBUG is defined
#if !(defined NDEBUG ^ defined _DEBUG)
	#error Exactly one of NDEBUG and _DEBUG needs to be defined by preprocessor
#endif

// make sure PX_CHECKED is defined in all _DEBUG configurations as well
#if !defined(PX_CHECKED) && _DEBUG
#define PX_CHECKED
#endif

#ifdef __CUDACC__
#define PX_CUDA_CALLABLE __host__ __device__
#else
#define PX_CUDA_CALLABLE
#endif

	// Ensure that the application hasn't tweaked the pack value to less than 8, which would break
	// matching between the API headers and the binaries
	// This assert works on win32/win64/360/ps3, but may need further specialization on other platforms.
	// Some GCC compilers need the compiler flag -malign-double to be set.
	// Apparently the apple-clang-llvm compiler doesn't support malign-double.

struct PxPackValidation { char _; long long a; };

#if !defined(PX_APPLE)
PX_COMPILE_TIME_ASSERT(PX_OFFSET_OF(PxPackValidation, a) == 8);
#endif

// use in a cpp file to suppress LNK4221
#if defined(PX_VC)
#define PX_DUMMY_SYMBOL namespace { char PxDummySymbol; } 
#else
#define PX_DUMMY_SYMBOL
#endif

#ifdef __SPU__
#define PX_IS_SPU 1
#else
#define PX_IS_SPU 0
#endif

#ifdef PX_X64
#define PX_IS_X64 1
#else
#define PX_IS_X64 0
#endif

#ifdef PX_WINDOWS
#define PX_IS_WINDOWS 1
#else
#define PX_IS_WINDOWS 0
#endif

#ifdef PX_X86
#define PX_IS_X86 1
#else
#define PX_IS_X86 0
#endif

#ifdef PX_X64
#define PX_IS_X64 1
#else
#define PX_IS_X64 0
#endif

#if defined(PX_X86) || defined(PX_X64)
#define PX_IS_INTEL 1
#else
#define PX_IS_INTEL 0
#endif

#ifdef PX_X360
#define PX_IS_X360 1
#else
#define PX_IS_X360 0
#endif

#ifdef PX_PS3
#define PX_IS_PS3 1
#else
#define PX_IS_PS3 0
#endif

#define PX_IS_PPU (PX_IS_PS3 && !PX_IS_SPU) // PS3 PPU

#ifdef PX_GNUC
#define PX_WEAK_SYMBOL __attribute__((weak)) // this is to support SIMD constant merging in template specialization
#else
#define PX_WEAK_SYMBOL
#endif

/** @} */
#endif // PX_FOUNDATION_PX_PREPROCESSOR_H
