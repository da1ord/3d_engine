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


#ifndef PX_COLLISION_NXTRIANGLEMESHDESC
#define PX_COLLISION_NXTRIANGLEMESHDESC
/** \addtogroup cooking
@{
*/

#include "PxPhysXConfig.h"
#include "geometry/PxSimpleTriangleMesh.h"

#ifndef PX_DOXYGEN
namespace physx
{
#endif

/**
\brief Descriptor class for #PxTriangleMesh.

Note that this class is derived from PxSimpleTriangleMesh which contains the members that describe the basic mesh.
The mesh data is *copied* when an PxTriangleMesh object is created from this descriptor. After the call the
user may discard the triangle data.

@see PxTriangleMesh PxTriangleMeshGeometry PxShape
*/
class PxTriangleMeshDesc : public PxSimpleTriangleMesh
{
public:

	/**
	Optional pointer to first material index, or NULL. There are PxSimpleTriangleMesh::numTriangles indices in total.
	Caller may add materialIndexStride bytes to the pointer to access the next triangle.

	When a triangle mesh collides with another object, a material is required at the collision point.
	If materialIndices is NULL, then the material of the PxShape instance is used.
	Otherwise, if the point of contact is on a triangle with index i, then the material index is determined as: 
	PxMaterialTableIndex	index = *(PxMaterialTableIndex *)(((PxU8*)materialIndices) + materialIndexStride * i);

	If the contact point falls on a vertex or an edge, a triangle adjacent to the vertex or edge is selected, and its index
	used to look up a material. The selection is arbitrary but consistent over time. 

	<b>Default:</b> NULL

	@see materialIndexStride
	*/
	PxTypedStridedData<PxMaterialTableIndex> materialIndices;

	/**
	\deprecated
	The SDK computes convex edges of a mesh and use them for collision detection. This parameter allows you to
	setup a tolerance for the convex edge detector.

	<b>Range:</b> (0, PX_MAX_F32)<br>
	<b>Default:</b> 0.001
	*/
	PX_DEPRECATED PxReal					convexEdgeThreshold;

	/**
	\brief Constructor sets to default.
	*/
	PX_INLINE PxTriangleMeshDesc();	

	/**
	\brief (re)sets the structure to the default.	
	*/
	PX_INLINE void setToDefault();

	/**
	\brief Returns true if the descriptor is valid.
	\return true if the current settings are valid
	*/
	PX_INLINE bool isValid() const;
};

PX_INLINE PxTriangleMeshDesc::PxTriangleMeshDesc()	//constructor sets to default
{
	PxSimpleTriangleMesh::setToDefault();
	convexEdgeThreshold			= 0.001f;
}

PX_INLINE void PxTriangleMeshDesc::setToDefault()
{
	*this = PxTriangleMeshDesc();
}

PX_INLINE bool PxTriangleMeshDesc::isValid() const
{
	if(points.count < 3) 	//at least 1 trig's worth of points
		return false;
	if ((!triangles.data) && (points.count%3))		// Non-indexed mesh => we must ensure the geometry defines an implicit number of triangles // i.e. numVertices can't be divided by 3
		return false;
	//add more validity checks here
	if (materialIndices.data && materialIndices.stride < sizeof(PxMaterialTableIndex))
		return false;
	return PxSimpleTriangleMesh::isValid();
}

#ifndef PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
