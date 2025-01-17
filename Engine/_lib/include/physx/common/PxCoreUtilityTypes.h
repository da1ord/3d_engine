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


#ifndef PX_CORE_UTILTY_TYPES_H
#define PX_CORE_UTILTY_TYPES_H
/** \addtogroup common
@{
*/

#include "foundation/PxAssert.h"

#ifndef PX_DOXYGEN
namespace physx
{
#endif


struct PxStridedData
{
	/**
	\brief The offset in bytes between consecutive samples in the data.

	<b>Default:</b> 0
	*/
	PxU32 stride;
	const void* data;

	PxStridedData() : stride( 0 ), data( NULL ) {}

	template<typename TDataType>
	PX_INLINE const TDataType& at( PxU32 idx ) const
	{
		PxU32 theStride( stride );
		if ( theStride == 0 )
			theStride = sizeof( TDataType );
		PxU32 offset( theStride * idx );
		return *(reinterpret_cast<const TDataType*>( reinterpret_cast< const PxU8* >( data ) + offset ));
	}
};

template<typename TDataType>
struct PxTypedStridedData
{
	PxU32 stride;
	const TDataType* data;

	PxTypedStridedData()
		: stride( 0 )
		, data( NULL )
	{
	}

};

struct PxBoundedData : public PxStridedData
{
	PxU32 count;
	PxBoundedData() : count( 0 ) {}
};

template<PxU8 TNumBytes>
struct PxPadding
{
	PxU8 mPadding[TNumBytes];
	PxPadding()
	{
		for ( PxU8 idx =0; idx < TNumBytes; ++idx )
			mPadding[idx] = 0;
	}
};

template <PxU32 NB_ELEMENTS> class PxFixedSizeLookupTable
{
public:
	
	PxFixedSizeLookupTable() 
		: mNbDataPairs(0)
	{
	}

	PxFixedSizeLookupTable(const PxEMPTY&) {}

	PxFixedSizeLookupTable(const PxReal* dataPairs, const PxU32 numDataPairs)
	{
		memcpy(mDataPairs,dataPairs,sizeof(PxReal)*2*numDataPairs);
		mNbDataPairs=numDataPairs;
	}

	PxFixedSizeLookupTable(const PxFixedSizeLookupTable& src)
	{
		memcpy(mDataPairs,src.mDataPairs,sizeof(PxReal)*2*src.mNbDataPairs);
		mNbDataPairs=src.mNbDataPairs;
	}

	~PxFixedSizeLookupTable()
	{
	}

	PxFixedSizeLookupTable& operator=(const PxFixedSizeLookupTable& src)
	{
		memcpy(mDataPairs,src.mDataPairs,sizeof(PxReal)*2*src.mNbDataPairs);
		mNbDataPairs=src.mNbDataPairs;
		return *this;
	}

	PX_FORCE_INLINE void addPair(const PxReal x, const PxReal y)
	{
		PX_ASSERT(mNbDataPairs<NB_ELEMENTS);
		mDataPairs[2*mNbDataPairs+0]=x;
		mDataPairs[2*mNbDataPairs+1]=y;
		mNbDataPairs++;
	}

	PX_FORCE_INLINE PxReal getYVal(const PxReal x) const
	{
		if(0==mNbDataPairs)
		{
			PX_ASSERT(false);
			return 0;
		}

		if(1==mNbDataPairs || x<getX(0))
		{
			return getY(0);
		}

		PxReal x0=getX(0);
		PxReal y0=getY(0);

		for(PxU32 i=1;i<mNbDataPairs;i++)
		{
			const PxReal x1=getX(i);
			const PxReal y1=getY(i);

			if((x>=x0)&&(x<x1))
			{
				return (y0+(y1-y0)*(x-x0)/(x1-x0));
			}

			x0=x1;
			y0=y1;
		}

		PX_ASSERT(x>=getX(mNbDataPairs-1));
		return getY(mNbDataPairs-1);
	}

	PxU32 getNbDataPairs() const {return mNbDataPairs;}
	
	void clear()
	{
		memset(mDataPairs, 0, NB_ELEMENTS*2*sizeof(PxReal));
		mNbDataPairs = 0;
	}

	PX_FORCE_INLINE PxReal getX(const PxU32 i) const
	{
		return mDataPairs[2*i];
	}
	PX_FORCE_INLINE PxReal getY(const PxU32 i) const
	{
		return mDataPairs[2*i+1];
	}

	PxReal mDataPairs[2*NB_ELEMENTS];
	PxU32 mNbDataPairs;
	PxU32 mPad[3];

	
};

#ifndef PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
