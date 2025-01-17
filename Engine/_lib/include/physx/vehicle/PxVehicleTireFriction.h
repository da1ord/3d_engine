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

#ifndef PX_VEHICLE_TIREFRICTION_H
#define PX_VEHICLE_TIREFRICTION_H
/** \addtogroup vehicle
  @{
*/

#include "foundation/PxSimpleTypes.h"

#ifndef PX_DOXYGEN
namespace physx
{
#endif

class PxMaterial;

/**
\brief Driving surface type. Each PxMaterial is associated with a corresponding PxVehicleDrivableSurfaceType.
@see PxMaterial, PxVehicleDrivableSurfaceToTireFrictionPairs
*/	
struct PxVehicleDrivableSurfaceType
{
	enum
	{
		eSURFACE_TYPE_UNKNOWN=0xffffffff
	};
	PxU32 mType;
};

/**
\brief Friction for each combination of driving surface type and tire type.
@see PxVehicleDrivableSurfaceType, PxVehicleTireData::mType
*/
class PxVehicleDrivableSurfaceToTireFrictionPairs
{
public:

	friend class VehicleSurfaceTypeHashTable;

	enum
	{
		eMAX_NB_SURFACE_TYPES=256
	};

	/**
	\brief Allocate the memory for a PxVehicleDrivableSurfaceToTireFrictionPairs instance
	that can hold data for combinations of tire type and surface type with up to maxNbTireTypes types of tire and maxNbSurfaceTypes types of surface.
	
	\param[in] maxNbTireTypes is the maximum number of allowed tire types.
	\param[in] maxNbSurfaceTypes is the maximum number of allowed surface types.  Must be less than or equal to eMAX_NB_SURFACE_TYPES
	
	\return a PxVehicleDrivableSurfaceToTireFrictionPairs instance that can be reused later with new type and friction data.

	@see setup
	*/
	static PxVehicleDrivableSurfaceToTireFrictionPairs* allocate
		(const PxU32 maxNbTireTypes, const PxU32 maxNbSurfaceTypes);

	/**
	\brief Set up a PxVehicleDrivableSurfaceToTireFrictionPairs instance for combinations of nbTireTypes tire types and nbSurfaceTypes surface types.
	
	\param[in] nbTireTypes is the number of different types of tire.  This value must be less than or equal to maxNbTireTypes specified in allocate().
	\param[in] nbSurfaceTypes is the number of different types of surface. This value must be less than or equal to maxNbSurfaceTypes specified in allocate().
	\param[in] drivableSurfaceMaterials is an array of PxMaterial pointers of length nbSurfaceTypes.
	\param[in] drivableSurfaceTypes is an array of PxVehicleDrivableSurfaceType instances of length nbSurfaceTypes.
	
	\note If the pointer to the PxMaterial that touches the tire is found in drivableSurfaceMaterials[x] then the surface type is drivableSurfaceTypes[x].mType 
	and the friction is the value that is set with setTypePairFriction(drivableSurfaceTypes[x].mType, PxVehicleTireData::mType, frictionValue).
	
	\note A friction value of 1.0 will be assigned as default to each combination of tire and surface type.  To override this use setTypePairFriction.
	@see release, setTypePairFriction, getTypePairFriction, PxVehicleTireData.mType
	*/
	void setup
		(const PxU32 nbTireTypes, const PxU32 nbSurfaceTypes, 
		 const PxMaterial** drivableSurfaceMaterials, const PxVehicleDrivableSurfaceType* drivableSurfaceTypes);

	/**
	\brief Deallocate a PxVehicleDrivableSurfaceToTireFrictionPairs instance
	*/
	void release();

	/**
	\brief Set the friction for a specified pair of tire type and drivable surface type.

	\param[in] surfaceType describes the surface type
	\param[in] tireType describes the tire type.
	\param[in] value describes the friction coefficient for the combination of surface type and tire type.
	*/
	void setTypePairFriction(const PxU32 surfaceType, const PxU32 tireType, const PxReal value);

	/**
	\brief Return the friction for a specified combination of surface type and tire type.
	\return The friction for a specified combination of surface type and tire type.
	*/
	PxReal getTypePairFriction(const PxU32 surfaceType, const PxU32 tireType) const;

	/**
	\brief Return the maximum number of surface types
	\return The maximum number of surface types
	@see allocate
	*/
	PxU32 getMaxNbSurfaceTypes() const {return mMaxNbSurfaceTypes;}

	/**
	\brief Return the maximum number of tire types
	\return The maximum number of tire types
	@see allocate
	*/
	PxU32 getMaxNbTireTypes() const {return mMaxNbTireTypes;}

private:

	/**
	\brief Ptr to base address of a 2d PxReal array with dimensions [mNbSurfaceTypes][mNbTireTypes]
	
	\note Each element of the array describes the maximum friction provided by a surface type-tire type combination.
	eg the friction corresponding to a combination of surface type x and tire type y is  mPairs[x][y]
	*/
	PxReal* mPairs;					

	/** 
	\brief Ptr to 1d array of material ptrs that is of length mNbSurfaceTypes.
	
	\note If the PxMaterial that touches the tire corresponds to mDrivableSurfaceMaterials[x] then the drivable surface
	type is mDrivableSurfaceTypes[x].mType and the friction for that contact is mPairs[mDrivableSurfaceTypes[x].mType][y], 
	assuming a tire type y.
	
	\note If the PxMaterial that touches the tire is not found in mDrivableSurfaceMaterials then the friction is 
	mPairs[0][y], assuming a tire type y.
	*/
	const PxMaterial** mDrivableSurfaceMaterials;

	/**
	\brief Ptr to 1d array of PxVehicleDrivableSurfaceType that is of length mNbSurfaceTypes.
	
	\note If the PxMaterial that touches the tire is found in mDrivableSurfaceMaterials[x] then the drivable surface
	type is mDrivableSurfaceTypes[x].mType and the friction for that contact is mPairs[mDrivableSurfaceTypes[x].mType][y], 
	assuming a tire type y.

	\note If the PxMaterial that touches the tire is not found in mDrivableSurfaceMaterials then the friction is 
	mPairs[0][y], assuming a tire type y.
	*/
	PxVehicleDrivableSurfaceType* mDrivableSurfaceTypes;

	/**
	\brief Number of different driving surface types.
	
	\note mDrivableSurfaceMaterials and mDrivableSurfaceTypes are both 1d arrays of length mMaxNbSurfaceTypes.
	
	\note mNbSurfaceTypes must be less than or equal to mMaxNbSurfaceTypes.
	*/	
	PxU32 mNbSurfaceTypes;			

	/**
	\brief Maximum number of different driving surface types.
	
	\note mMaxNbSurfaceTypes must be less than or equal to eMAX_NB_SURFACE_TYPES.
	*/	
	PxU32 mMaxNbSurfaceTypes;			

	/**
	\brief Number of different tire types.
	
	\note Tire types stored in PxVehicleTireData.mType
	*/	
	PxU32 mNbTireTypes;			

	/**
	\brief Maximum number of different tire types.
	
	\note Tire types stored in PxVehicleTireData.mType
	*/	
	PxU32 mMaxNbTireTypes;			


#ifndef PX_X64
	PxU32 mPad[1];
#else
	PxU32 mPad[2];
#endif

	PxVehicleDrivableSurfaceToTireFrictionPairs(){}
	~PxVehicleDrivableSurfaceToTireFrictionPairs(){}
};
PX_COMPILE_TIME_ASSERT(0==(sizeof(PxVehicleDrivableSurfaceToTireFrictionPairs) & 15));

#ifndef PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif //PX_VEHICLE_TIREFRICTION_H
