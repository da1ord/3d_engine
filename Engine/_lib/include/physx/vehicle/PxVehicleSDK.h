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

#ifndef PX_VEHICLE_SDK_H
#define PX_VEHICLE_SDK_H
/** \addtogroup vehicle
  @{
*/

#include "foundation/Px.h"


#ifndef PX_DOXYGEN
namespace physx
{
#endif

class PxPhysics;
class PxSerializationRegistry;
/**
\brief Initialize the PhysXVehicle library. 

Call this before using any of the vehicle functions.

\param physics The PxPhysics instance.
\param serializationRegistry PxSerializationRegistry instance, if NULL vehicle serialization is not supported.

\note This function must be called after PxFoundation and PxPhysics instances have been created.
\note If a PxSerializationRegistry instance is specified then PhysXVehicle is also dependent on PhysXExtensions.

@see PxCloseVehicleSDK
*/
PX_C_EXPORT bool PX_CALL_CONV PxInitVehicleSDK(PxPhysics& physics, PxSerializationRegistry* serializationRegistry = NULL);

/**
\brief Shut down the PhysXVehicle library. 

Call this function as part of the physx shutdown process.

\param serializationRegistry PxSerializationRegistry instance, if non-NULL must be the same as passed into PxInitVehicleSDK.

\note This function must be called prior to shutdown of PxFoundation and PxPhysics.
\note If the PxSerializationRegistry instance is specified this function must additionally be called prior to shutdown of PhysXExtensions.

@see PxInitVehicleSDK
*/
PX_C_EXPORT void PX_CALL_CONV PxCloseVehicleSDK(PxSerializationRegistry* serializationRegistry = NULL);


/**
\brief This number is the maximum number of wheels allowed for a vehicle.
*/
#define PX_MAX_NB_WHEELS (20)


/**
\brief Compiler setting to enable recording of telemetry data

@see PxVehicleUpdateSingleVehicleAndStoreTelemetryData, PxVehicleTelemetryData
*/
#define PX_DEBUG_VEHICLE_ON (1)

/**
@see PxVehicleDrive4W, PxVehicleDriveTank, PxVehicleDriveNW, PxVehicleNoDrive, PxVehicleWheels::getVehicleType
*/
struct PxVehicleTypes
{
	enum Enum
	{
		eDRIVE4W=0,
		eDRIVENW,
		eDRIVETANK,
		eNODRIVE,
		eUSER1,
		eUSER2,
		eUSER3,
		eMAX_NB_VEHICLE_TYPES
	};
};

/**
\brief Set the basis vectors of the vehicle simulation 

Default values PxVec3(0,1,0), PxVec3(0,0,1)

Call this function before using PxVehicleUpdates unless the default values are correct.
*/
void PxVehicleSetBasisVectors(const PxVec3& up, const PxVec3& forward);

/**
@see PxVehicleSetUpdateMode
*/
struct PxVehicleUpdateMode
{
	enum Enum
	{
		eVELOCITY_CHANGE,
		eACCELERATION	
	};
};

/**
\brief Set the effect of PxVehicleUpdates to be either to modify each vehicle's rigid body actor

with an acceleration to be applied in the next PhysX SDK update or as an immediate velocity modification.

Default behavior is immediate velocity modification.

Call this function before using PxVehicleUpdates for the first time if the default is not the desired behavior.

@see PxVehicleUpdates
*/
void PxVehicleSetUpdateMode(PxVehicleUpdateMode::Enum vehicleUpdateMode);

#ifndef PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif //PX_VEHICLE_SDK_H
