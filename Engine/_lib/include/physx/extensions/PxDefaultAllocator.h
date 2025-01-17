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


#ifndef PX_DEFAULT_ALLOCATOR_H
#define PX_DEFAULT_ALLOCATOR_H
/** \addtogroup extensions
  @{
*/

#include "foundation/PxAllocatorCallback.h"
#include "common/PxPhysXCommonConfig.h"
#include "foundation/PxAssert.h"
#include <stdlib.h>

#if defined(PX_WINDOWS) || defined(PX_LINUX) || defined(PX_ANDROID)
#include <malloc.h>
#endif

#ifndef PX_DOXYGEN
namespace physx
{
#endif

#if defined(PX_WINDOWS) || defined(PX_WINMODERN)
// on win32 we only have 8-byte alignment guaranteed, but the CRT provides special aligned allocation fns
PX_FORCE_INLINE void* platformAlignedAlloc(size_t size)
{
	return _aligned_malloc(size, 16);
}

PX_FORCE_INLINE void platformAlignedFree(void* ptr)
{
	_aligned_free(ptr);
}
#elif defined(PX_LINUX) || defined(PX_ANDROID)
PX_FORCE_INLINE void* platformAlignedAlloc(size_t size)
{
	return ::memalign(16, size);
}

PX_FORCE_INLINE void platformAlignedFree(void* ptr)
{
	::free(ptr);
}
#elif defined(PX_WIIU)
PX_FORCE_INLINE void* platformAlignedAlloc(size_t size)
{
	size_t pad = 15 + sizeof(size_t); // store offset for delete.
	PxU8* base = (PxU8*)::malloc(size+pad);
	if(!base)
		return NULL;

	PxU8* ptr = (PxU8*)(size_t(base + pad) & ~(15)); // aligned pointer
	((size_t*)ptr)[-1] = ptr - base; // store offset

	return ptr;
}

PX_FORCE_INLINE void platformAlignedFree(void* ptr)
{
	if(ptr == NULL)
		return;

	PxU8* base = ((PxU8*)ptr) - ((size_t*)ptr)[-1];
	::free(base);
}
#else
// on all other platforms we get 16-byte alignment by default
PX_FORCE_INLINE void* platformAlignedAlloc(size_t size)
{
	return ::malloc(size);	
}

PX_FORCE_INLINE void platformAlignedFree(void* ptr)
{
	::free(ptr);
}
#endif

/**
\brief default implementation of the allocator interface required by the SDK
*/
class PxDefaultAllocator : public PxAllocatorCallback
{
public:
	void* allocate(size_t size, const char*, const char*, int)
	{
		void* ptr = platformAlignedAlloc(size);
		PX_ASSERT((reinterpret_cast<size_t>(ptr) & 15)==0);
		return ptr;
	}

	void deallocate(void* ptr)
	{
		platformAlignedFree(ptr);
	}
};

#ifndef PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
