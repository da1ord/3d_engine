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


#ifndef PX_PHYSICS_EXTENSIONS_TRIANGLE_MESH_H
#define PX_PHYSICS_EXTENSIONS_TRIANGLE_MESH_H
/** \addtogroup extensions
  @{
*/

#include "PxPhysXConfig.h"
#include "common/PxPhysXCommonConfig.h"

#ifndef PX_DOXYGEN
namespace physx
{
#endif

class PxGeometry;
class PxTriangleMeshGeometry;
class PxHeightFieldGeometry;

	class PxMeshOverlapUtil
	{
		public:

										PxMeshOverlapUtil();
										~PxMeshOverlapUtil();

						PxU32			findOverlap(const PxGeometry& geom, const PxTransform& geomPose, const PxTriangleMeshGeometry& triGeom, const PxTransform& meshPose);
						PxU32			findOverlap(const PxGeometry& geom, const PxTransform& geomPose, const PxHeightFieldGeometry& hfGeom, const PxTransform& hfPose);

		PX_FORCE_INLINE	const PxU32*	getResults()	const	{ return mResultsMemory;	}
		PX_FORCE_INLINE	PxU32			getNbResults()	const	{ return mNbResults;		}

		private:
						PxU32*			mResultsMemory;
						PxU32			mResults[64];
						PxU32			mNbResults;
						PxU32			mMaxNbResults;
	};

	/** \brief DEPRECATED typedef for backward compatibility with PhysX 3.2. */
	typedef PxMeshOverlapUtil PxFindOverlapTriangleMeshUtil;


	/**
	\brief Computes an approximate minimum translational distance (MTD) between a geometry object and a mesh.

	This iterative function computes an approximate vector that can be used to depenetrate a geom object
	from a triangle mesh. Returned depenetration vector should be applied to 'geom', to get out of the mesh.

	The function works best when the amount of overlap between the geom object and the mesh is small. If the
	geom object's center goes inside the mesh, backface culling usually kicks in, no overlap is detected,
	and the function does not compute an MTD vector.

	The function early exits if no overlap is detected after a depenetration attempt. This means that if
	maxIter = N, the code will attempt at most N iterations but it might exit earlier if depenetration has
	been successful. Usually N = 4 gives good results.

	\param[in] maxIter Max number of iterations before returning.
	\param[in] geom The geometry object
	\param[in] geomPose Pose for the geometry object
	\param[in] meshGeom The mesh geometry
	\param[in] meshPose Pose for the mesh
	\param[out] nb Number of depenetrations attempts performed during the call. 0 means no overlap has been detected.

	\return Approximate depenetration vector

	@see PxGeometry PxTransform PxTriangleMeshGeometry
	*/
	PxVec3 PxComputeMeshPenetration(PxU32 maxIter, const PxGeometry& geom, const PxTransform& geomPose, const PxTriangleMeshGeometry& meshGeom, const PxTransform& meshPose, PxU32& nb);

	/**
	\brief Computes an approximate minimum translational distance (MTD) between a geometry object and a heightfield.

	This iterative function computes an approximate vector that can be used to depenetrate a geom object
	from a heightfield. Returned depenetration vector should be applied to 'geom', to get out of the heightfield.

	The function works best when the amount of overlap between the geom object and the mesh is small. If the
	geom object's center goes inside the heightfield, backface culling usually kicks in, no overlap is detected,
	and the function does not compute an MTD vector.

	The function early exits if no overlap is detected after a depenetration attempt. This means that if
	maxIter = N, the code will attempt at most N iterations but it might exit earlier if depenetration has
	been successful. Usually N = 4 gives good results.

	\param[in] maxIter Max number of iterations before returning.
	\param[in] geom The geometry object
	\param[in] geomPose Pose for the geometry object
	\param[in] heightFieldGeom The heightfield geometry
	\param[in] heightFieldPose Pose for the heightfield
	\param[out] nb Number of depenetrations attempts performed during the call. 0 means no overlap has been detected.

	\return Approximate depenetration vector

	@see PxGeometry PxTransform PxHeightFieldGeometry
	*/
	PxVec3 PxComputeHeightFieldPenetration(PxU32 maxIter, const PxGeometry& geom, const PxTransform& geomPose, const PxHeightFieldGeometry& heightFieldGeom, const PxTransform& heightFieldPose, PxU32& nb);

#ifndef PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
