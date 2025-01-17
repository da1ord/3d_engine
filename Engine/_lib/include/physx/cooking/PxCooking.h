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


#ifndef PX_COOKING_H
#define PX_COOKING_H
/** \addtogroup cooking
@{
*/
#include "common/PxPhysXCommonConfig.h"      
#include "common/PxTolerancesScale.h"
#include "cooking/Pxc.h"

#include "cooking/PxConvexMeshDesc.h"
#include "cooking/PxTriangleMeshDesc.h"

#ifndef PX_DOXYGEN
namespace physx
{
#endif

class PxOutputStream;
class PxBinaryConverter;

struct PxPlatform
{
	enum Enum
	{
		ePC,
		eXENON,
		ePLAYSTATION3,
		eARM,
		eWIIU
	};
};

/**

\brief Enum for the set of mesh pre-processing parameters.

*/

struct PxMeshPreprocessingFlag
{
	enum Enum
	{
		eWELD_VERTICES					=	1 << 0, 
		eREMOVE_UNREFERENCED_VERTICES	=	1 << 1,
		eREMOVE_DUPLICATED_TRIANGLES	=	1 << 2
	};
};

typedef PxFlags<PxMeshPreprocessingFlag::Enum,PxU32> PxMeshPreprocessingFlags;
/**

\brief Structure describing parameters affecting mesh cooking.

@see PxSetCookingParams() PxGetCookingParams()
*/
struct PxCookingParams
{
	/**
	\brief Target platform

	Should be set to the platform which you intend to load the cooked mesh data on. This allows
	the SDK to optimize the mesh data in an appropriate way for the platform and make sure that
	endianness issues are accounted for correctly.

	<b>Default value:</b> Same as the platform on which the SDK is running.
	*/
	PxPlatform::Enum	targetPlatform;

	/**
	\brief Skin width for convexes.

	Specifies the amount to inflate the convex mesh when the PxConvexFlag::eINFLATE_CONVEX is used.

	The value is used for moving planes outward, and beveling sharp edges. This helps the hull generator 
	code produce more stable convexes for collision detection. Please note that the resulting hull will
	increase its size, so contact generation may produce noticeable separation between shapes. The separation
	distance can be reduced by decreasing the contactOffset and restOffset.  See the user's manual on 
	'Shapes - Tuning Shape Collision Behavior' for details.
	
	Change the value if the produced hulls are too thin or improper for your usage. Increasing the value
	too much will result in incorrect hull size and a large separation between shapes.

	<b>Default value:</b> 0.025f*PxToleranceScale.length

	<b>Range:</b> (0.0f, PX_MAX_F32)
	*/
	float		skinWidth;

	/**
	\brief When true, the face remap table is not created.  This saves a significant amount of memory, but the SDK will 
		not be able to provide information about which mesh triangle is hit in collisions, sweeps or raycasts hits.

	<b>Default value:</b> false
	*/
	bool		suppressTriangleMeshRemapTable;

	/**
	\brief When true, the triangle adjacency information is created. You can get the adjacency triangles
	for a given triangle from getTriangle. 

	<b>Default value:</b> false
	*/
	bool		buildTriangleAdjacencies;

	/**
	\brief Tolerance scale is used to check if cooked triangles are not too huge. This check will help with simulation stability.
	
	@see PxTolerancesScale
	*/
	PxTolerancesScale scale;

	/**
	\brief Mesh pre-processing parameters. Used to control options like whether the mesh cooking performs vertex welding before cooking.

	<b>Default value:</b> 0
	*/
	PxMeshPreprocessingFlags	meshPreprocessParams;

	/**
	\brief Mesh weld tolerance. If mesh welding is enabled, this controls the distance at which vertices are welded. 
	If mesh welding is not enabled, this value defines the acceptance distance for mesh validation. Provided no two vertices are within this distance, the mesh is considered to be 
	clean. If not, a warning will be emitted. Having a clean, welded mesh is required to achieve the best possible performance.

	The default vertex welding uses a snap-to-grid approach. This approach effectively truncates each vertex to integer values using meshWeldTolerance. 
	Once these snapped vertices are produced, all vertices that snap to a given vertex on the grid are remapped to reference a single vertex. Following this, 
	all triangles' indices are remapped to reference this subset of clean vertices. It should be noted that	the vertices that we do not alter the 
	position of the vertices; the snap-to-grid is only performed to identify nearby vertices.

	The mesh validation approach also uses the same snap-to-grid approach to identify nearby vertices. If more than one vertex snaps to a given grid coordinate,
	we ensure that the distance between the vertices is at least meshWeldTolerance. If this is not the case, a warning is emitted.

	<b>Default value:</b> 0.0
	*/
	PxReal		meshWeldTolerance;


	PxCookingParams(const PxTolerancesScale& sc):
		skinWidth(0.025f*sc.length),
		suppressTriangleMeshRemapTable(false),
		buildTriangleAdjacencies(false),
		scale(sc),
		meshPreprocessParams(0),
		meshWeldTolerance(0.f)
	{
#if defined(PX_X86) || defined(PX_X64)
		targetPlatform = PxPlatform::ePC;
#elif defined(PX_X360)
		targetPlatform = PxPlatform::eXENON;
#elif defined(PX_PS3)
		targetPlatform = PxPlatform::ePLAYSTATION3;
#elif defined(PX_ARM)
		targetPlatform = PxPlatform::eARM;
#elif defined(PX_WIIU)
		targetPlatform = PxPlatform::eWIIU;
#else
#error Unknown platform
#endif
	}	
};

class PxCooking
{
public:
	/**
	\brief Closes this instance of the interface.

	This function should be called to cleanly shut down the Cooking library before application exit. 

	\note This function is required to be called to release foundation usage.

	*/
	virtual void  release() = 0;

	/**
	\brief Sets cooking parameters

	\param[in] params Cooking parameters

	@see getParams()
	*/
	virtual void  setParams(const PxCookingParams& params) = 0;

	/**
	\brief Gets cooking parameters

	\return Current cooking parameters.

	@see PxCookingParams setParams()
	*/
	virtual const PxCookingParams& getParams() = 0;

	/**
	\brief Checks endianness is the same between cooking & target platforms

	\return True if there is and endian mismatch.
	*/
	virtual bool  platformMismatch() = 0;

	/**
	\brief Cooks a triangle mesh. The results are written to the stream.

	To create a triangle mesh object it is necessary to first 'cook' the mesh data into
	a form which allows the SDK to perform efficient collision detection.

	cookTriangleMesh() allows a mesh description to be cooked into a binary stream
	suitable for loading and performing collision detection at runtime.

	Example

	\include PxCookTriangleMesh_Example.cpp

	\param[in] desc The triangle mesh descriptor to read the mesh from.
	\param[in] stream User stream to output the cooked data.
	\return true on success

	@see cookConvexMesh() setParams() PxPhysics.createTriangleMesh()
	*/
	virtual bool  cookTriangleMesh(const PxTriangleMeshDesc& desc, PxOutputStream& stream) = 0;

	/**
	\brief Cooks a convex mesh. The results are written to the stream.

	To create a triangle mesh object it is necessary to first 'cook' the mesh data into
	a form which allows the SDK to perform efficient collision detection.

	cookConvexMesh() allows a mesh description to be cooked into a binary stream
	suitable for loading and performing collision detection at runtime.

	Example

	\include PxCookConvexMesh_Example.cpp

	\note This method is not reentrant if the convex mesh descriptor has the flag #PxConvexFlag::eCOMPUTE_CONVEX set.
	\note The number of vertices and the number of convex polygons in a cooked convex mesh is limited to 256.
	\note If those limits are exceeded in either the user-provided data or the final cooked mesh, an error is reported.
	\note If this happens, using the #PxConvexFlag::eINFLATE_CONVEX is usually enough to obtain a valid convex.

	\param[in] desc The convex mesh descriptor to read the mesh from.
	\param[in] stream User stream to output the cooked data.
	\return true on success

	@see cookTriangleMesh() setParams()
	*/
	virtual bool  cookConvexMesh(const PxConvexMeshDesc& desc, PxOutputStream& stream) = 0;

	/**
	\brief Computed hull polygons from given vertices and triangles. Polygons are needed for PxConvexMeshDesc rather then triangles.

	\param[in] mesh Simple triangle mesh containing vertices and triangles used to compute polygons.
	\param[in] inCallback Memory allocator for out array allocations.
	\param[out] nbIndices Number of indices used by polygons.
	\param[out] indices Indices array used by polygons.
	\param[out] nbPolygons Number of created polygons.
	\param[out] hullPolygons Polygons array.
	\return true on success

	@see cookConvexMesh() PxConvexFlags PxConvexMeshDesc PxSimpleTriangleMesh
	*/
	virtual bool  computeHullPolygons(const PxSimpleTriangleMesh& mesh, PxAllocatorCallback& inCallback, PxU32& nbIndices, PxU32*& indices,
							PxU32& nbPolygons, PxHullPolygon*& hullPolygons) = 0;

	/**
	\brief Cooks a heightfield. The results are written to the stream.

	To create a heightfield object there is an option to precompute some of calculations done while loading the heightfield data.

	cookHeightField() allows a heightfield description to be cooked into a binary stream
	suitable for loading and performing collision detection at runtime.

	Example

	\include PxCookConvexMesh_Example.cpp

	\param[in] desc The heightfield descriptor to read the HF from.
	\param[in] stream User stream to output the cooked data.
	\return true on success

	@see PxPhysics.createHeightField()
	*/
	virtual bool  cookHeightField(const PxHeightFieldDesc& desc, PxOutputStream& stream) = 0;

protected:
	virtual ~PxCooking(){}
};

#ifndef PX_DOXYGEN
} // namespace physx
#endif

/**
\brief Create an instance of the cooking interface.

Note that the foundation object is handled as an application-wide singleton in statically linked executables 
and a DLL-wide singleton in dynamically linked executables. Therefore, if you are using the runtime SDK in the 
same executable as cooking, you should pass the Physics's copy of foundation (acquired with 
PxPhysics::getFoundation()) to the cooker. This will also ensure correct handling of memory for objects
passed from the cooker to the SDK. 

To use cooking in standalone mode, create an instance of the Foundation object with PxCreateCookingFoundation.
You should pass the same foundation object to all instances of the cooking interface.

\param[in] version the SDK version number
\param[in] foundation the foundation object associated with this instance of the cooking interface.
\param[in] params the parameters for this instance of the cooking interface
\return true on success.
*/
PX_C_EXPORT PX_PHYSX_COOKING_API physx::PxCooking* PX_CALL_CONV PxCreateCooking(physx::PxU32 version,
																				physx::PxFoundation& foundation,
																				const physx::PxCookingParams& params);

/** @} */
#endif
