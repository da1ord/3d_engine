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


#ifndef PX_PHYSICS_METADATA_FLAGS
#define PX_PHYSICS_METADATA_FLAGS

#include "foundation/Px.h"

#ifndef PX_DOXYGEN
namespace physx
{
#endif

	/**
	\brief Flags used to configure binary meta data entries, typically set through PX_DEF_BIN_METADATA defines.

	@see PxMetaDataEntry
	*/
    struct PxMetaDataFlag
	{
		enum Enum
		{
			eCLASS					= (1<<0),		//!< declares a class
			eVIRTUAL				= (1<<1),		//!< declares class to be virtual
			eTYPEDEF				= (1<<2),		//!< declares a typedef
			ePTR					= (1<<3),		//!< declares a pointer
			eEXTRA_DATA				= (1<<4),		//!< declares extra data exported with PxSerializer::exportExtraData
			eEXTRA_ITEM				= (1<<5),		//!< specifies one element of extra data
			eEXTRA_ITEMS			= (1<<6),		//!< specifies an array of extra data
			eEXTRA_NAME             = (1<<7),       //!< specifies a name of extra data
			eUNION					= (1<<8),		//!< declares a union
			ePADDING				= (1<<9),		//!< declares explicit padding data
			eALIGNMENT				= (1<<10),		//!< declares aligned data
			eCOUNT_MASK_MSB			= (1<<11),		//!< specifies that the count value's most significant bit needs to be masked out
			eCOUNT_SKIP_IF_ONE		= (1<<12),		//!< specifies that the count value is treated as zero for a variable value of one - special case for single triangle meshes
			eCONTROL_FLIP			= (1<<13),		//!< specifies that the control value is the negate of the variable value
			eCONTROL_MASK			= (1<<14),		//!< specifies that the control value is masked - mask bits are assumed to be within eCONTROL_MASK_RANGE
			eCONTROL_MASK_RANGE		= 0x000000FF,	//!< mask range allowed for eCONTROL_MASK 
			eFORCE_DWORD			= 0x7fffffff
		};
	};

#ifndef PX_DOXYGEN
}
#endif

#endif
