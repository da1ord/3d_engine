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


#ifndef PX_PHYSICS_NX_FILTERING
#define PX_PHYSICS_NX_FILTERING
/** \addtogroup physics
@{
*/

#include "PxPhysXConfig.h"
#include "foundation/PxFlags.h"

#ifndef PX_DOXYGEN
namespace physx
{
#endif

class PxActor;
class PxShape;


/**
\brief Collection of flags describing the actions to take for a collision pair.

@see PxPairFlags PxSimulationFilterShader.filter() PxSimulationFilterCallback
*/
struct PxPairFlag
{
	enum Enum
	{
		/**
		\brief Process the contacts of this collision pair in the dynamics solver.

		\note Only takes effect if the colliding actors are rigid bodies.
		*/
		eSOLVE_CONTACT					= (1<<0),

		/**
		\brief Call contact modification callback for this collision pair

		\note Only takes effect if the colliding actors are rigid bodies.

		@see PxContactModifyCallback
		*/
		eMODIFY_CONTACTS					= (1<<1),

		/**
		\brief Call contact report callback or trigger callback when this collision pair starts to be in contact.

		If one of the two collision objects is a trigger shape (see #PxShapeFlag::eTRIGGER_SHAPE) 
		then the trigger callback will get called as soon as the other object enters the trigger volume. 
		If none of the two collision objects is a trigger shape then the contact report callback will get 
		called when the actors of this collision pair start to be in contact.

		\note Only takes effect if the colliding actors are rigid bodies.

		@see PxSimulationEventCallback.onContact() PxSimulationEventCallback.onTrigger()
		*/
		eNOTIFY_TOUCH_FOUND					= (1<<2),

		/**
		\brief Call contact report callback while this collision pair is in contact

		If none of the two collision objects is a trigger shape then the contact report callback will get 
		called while the actors of this collision pair are in contact.

		\note Triggers do not support this event. Persistent trigger contacts need to be tracked separately by observing eNOTIFY_TOUCH_FOUND/eNOTIFY_TOUCH_LOST events.

		\note Only takes effect if the colliding actors are rigid bodies.

		\note No report will get sent if the objects in contact are sleeping.

		@see PxSimulationEventCallback.onContact() PxSimulationEventCallback.onTrigger()
		*/
		eNOTIFY_TOUCH_PERSISTS				= (1<<3),

		/**
		\brief Call contact report callback or trigger callback when this collision pair stops to be in contact

		If one of the two collision objects is a trigger shape (see #PxShapeFlag::eTRIGGER_SHAPE) 
		then the trigger callback will get called as soon as the other object leaves the trigger volume. 
		If none of the two collision objects is a trigger shape then the contact report callback will get 
		called when the actors of this collision pair stop to be in contact.

		\note Only takes effect if the colliding actors are rigid bodies.

		\note This event will also get triggered if one of the colliding objects gets deleted.

		@see PxSimulationEventCallback.onContact() PxSimulationEventCallback.onTrigger()
		*/
		eNOTIFY_TOUCH_LOST					= (1<<4),

		/**
		\brief Call contact report callback when the contact force between the actors of this collision pair exceeds one of the actor-defined force thresholds.

		\note Only takes effect if the colliding actors are rigid bodies.

		@see PxSimulationEventCallback.onContact()
		*/
		eNOTIFY_THRESHOLD_FORCE_FOUND		= (1<<5),

		/**
		\brief Call contact report callback when the contact force between the actors of this collision pair continues to exceed one of the actor-defined force thresholds.

		\note Only takes effect if the colliding actors are rigid bodies.

		\note If a pair gets re-filtered and this flag has previously been disabled, then the report will not get fired in the same frame even if the force threshold has been reached in the
		previous one (unless #eNOTIFY_THRESHOLD_FORCE_FOUND has been set in the previous frame).

		@see PxSimulationEventCallback.onContact()
		*/
		eNOTIFY_THRESHOLD_FORCE_PERSISTS	= (1<<6),

		/**
		\brief Call contact report callback when the contact force between the actors of this collision pair falls below one of the actor-defined force thresholds (includes the case where this collision pair stops being in contact).

		\note Only takes effect if the colliding actors are rigid bodies.

		\note If a pair gets re-filtered and this flag has previously been disabled, then the report will not get fired in the same frame even if the force threshold has been reached in the
		previous one (unless #eNOTIFY_THRESHOLD_FORCE_FOUND or #eNOTIFY_THRESHOLD_FORCE_PERSISTS has been set in the previous frame).

		@see PxSimulationEventCallback.onContact()
		*/
		eNOTIFY_THRESHOLD_FORCE_LOST		= (1<<7),

		/**
		\brief Provide contact points in contact reports for this collision pair.

		\note Only takes effect if the colliding actors are rigid bodies and if used in combination with the flags eNOTIFY_TOUCH_... or eNOTIFY_THRESHOLD_FORCE_...

		@see PxSimulationEventCallback.onContact() PxContactPair PxContactPair.extractContacts()
		*/
		eNOTIFY_CONTACT_POINTS				= (1<<8),

		/**
		\brief This flag is used to indicate whether this pair generates discrete collision detection contacts. 

		\note Contacts are only responded to if eSOLVE_CONTACT is enabled.
		*/

		eDETECT_DISCRETE_CONTACT			= (1<<9),
		

		/**
		\brief This flag is used to indicate whether this pair generates CCD contacts. 

		\note The contacts will only be responded to if eSOLVE_CONTACT is enabled on this pair.
		\note The scene must have PxSceneFlag::eENABLE_CCD enabled to use this feature.
		\note Non-static bodies of the pair should have PxRigidBodyFlag::eENABLE_CCD specified for this feature to work correctly.

		@see PxRigidBodyFlag::eENABLE_CCD
		@see PxSceneFlag::eENABLE_CCD
		*/

		eDETECT_CCD_CONTACT					= (1<<10),

		/**
		\deprecated
		\brief Provides default flag for resolving contacts
		*/

		PX_DEPRECATED eRESOLVE_CONTACTS					= eSOLVE_CONTACT | eDETECT_DISCRETE_CONTACT,

		/**
		\deprecated
		\brief Provided default flag to enable performing linear CCD sweeps and response for this collision pair.
		*/
		PX_DEPRECATED eCCD_LINEAR							= eSOLVE_CONTACT | eDETECT_CCD_CONTACT,

		/**
		\brief Provided default flag to do simple contact processing for this collision pair.
		*/
		eCONTACT_DEFAULT					= eSOLVE_CONTACT | eDETECT_DISCRETE_CONTACT,

		/**
		\brief Provided default flag to get commonly used trigger behavior for this collision pair.
		*/
		eTRIGGER_DEFAULT					= eNOTIFY_TOUCH_FOUND | eNOTIFY_TOUCH_LOST
	};
};

/**
\brief Bitfield that contains a set of raised flags defined in PxPairFlag.

@see PxPairFlag
*/
typedef PxFlags<PxPairFlag::Enum, PxU16> PxPairFlags;
PX_FLAGS_OPERATORS(PxPairFlag::Enum, PxU16);



/**
\brief Collection of flags describing the filter actions to take for a collision pair.

@see PxFilterFlags PxSimulationFilterShader PxSimulationFilterCallback
*/
struct PxFilterFlag
{
	enum Enum
	{
		/**
		\brief Ignore the collision pair as long as the bounding volumes of the pair objects overlap.

		Killed pairs will be ignored by the simulation and won't run through the filter again until one
		of the following occurs:

		\li The bounding volumes of the two objects overlap again (after being separated)
		\li The user enforces a re-filtering (see #PxScene::resetFiltering())

		@see PxScene::resetFiltering()
		*/
		eKILL				= (1<<0),

		/**
		\brief Ignore the collision pair as long as the bounding volumes of the pair objects overlap or until filtering relevant data changes for one of the collision objects.

		Suppressed pairs will be ignored by the simulation and won't make another filter request until one
		of the following occurs:

		\li Same conditions as for killed pairs (see #eKILL)
		\li The filter data or the filter object attributes change for one of the collision objects

		@see PxFilterData PxFilterObjectAttributes
		*/
		eSUPPRESS			= (1<<1),

		/**
		\brief Invoke the filter callback (#PxSimulationFilterCallback::pairFound()) for this collision pair.

		@see PxSimulationFilterCallback
		*/
		eCALLBACK			= (1<<2),

		/**
		\brief Track this collision pair with the filter callback mechanism.

		When the bounding volumes of the collision pair lose contact, the filter callback #PxSimulationFilterCallback::pairLost()
		will be invoked. Furthermore, the filter status of the collision pair can be adjusted through #PxSimulationFilterCallback::statusChange()
		once per frame (until a pairLost() notification occurs).

		@see PxSimulationFilterCallback
		*/
		eNOTIFY				= (1<<3) | eCALLBACK,

		/**
		\brief Provided default to get standard behavior:

		The application configure the pair's collision properties once when bounding volume overlap is found and
		doesn't get asked again about that pair until overlap status or filter properties changes, or re-filtering is requested.

		No notification is provided when bounding volume overlap is lost

		The pair will not be killed or suppressed, so collision detection will be processed
		*/

		eDEFAULT = 0
	};
};

/**
\brief Bitfield that contains a set of raised flags defined in PxFilterFlag.

@see PxFilterFlag
*/
typedef PxFlags<PxFilterFlag::Enum, PxU16> PxFilterFlags;
PX_FLAGS_OPERATORS(PxFilterFlag::Enum, PxU16);


/**
\brief PxFilterData is user-definable data which gets passed into the collision filtering shader and/or callback.

@see PxShape.setSimulationFilterData() PxShape.getSimulationFilterData()  PxSimulationFilterShader PxSimulationFilterCallback
*/
struct PxFilterData
{
	PX_INLINE PxFilterData(const PxEMPTY&)
	{
	}

	/**
	\brief Default constructor.
	*/
	PX_INLINE PxFilterData() 
	{
		word0 = word1 = word2 = word3 = 0;
	}

	/**
	\brief Constructor to set filter data initially.
	*/
	PX_INLINE PxFilterData(PxU32 w0, PxU32 w1, PxU32 w2, PxU32 w3) : word0(w0), word1(w1), word2(w2), word3(w3) {}

	/**
	\brief (re)sets the structure to the default.	
	*/
	PX_INLINE void setToDefault()
	{
		*this = PxFilterData();
	}

	PxU32 word0;
	PxU32 word1;
	PxU32 word2;
	PxU32 word3;
};


/**
\brief Identifies each type of filter object.

@see PxGetFilterObjectType()
*/
struct PxFilterObjectType
{
	enum Enum
	{
		/**
		\brief A static rigid body
		@see PxRigidStatic
		*/
		eRIGID_STATIC,

		/**
		\brief A dynamic rigid body
		@see PxRigidDynamic
		*/
		eRIGID_DYNAMIC,

		/**
		\brief A particle system
		@see PxParticleSystem
		*/
		ePARTICLE_SYSTEM,

		/**
		\brief A particle fluid
		@see PxParticleFluid
		*/
		ePARTICLE_FLUID,

		/**
		\brief An articulation
		@see PxArticulation
		*/
		eARTICULATION,
		
		/**
		\brief A cloth object
		@see PxCloth
		*/
		eCLOTH,

		//brief internal use only!
		eMAX_TYPE_COUNT = 16,

		//brief internal use only!
		eUNDEFINED = eMAX_TYPE_COUNT-1
	};
};


// For internal use only
struct PxFilterObjectFlag
{
	enum Enum
	{
		eKINEMATIC		= (1<<4),
		eTRIGGER		= (1<<5)
	};
};


/**
\brief Structure which gets passed into the collision filtering shader and/or callback providing additional information on objects of a collision pair

@see PxSimulationFilterShader PxSimulationFilterCallback getActorType() PxFilterObjectIsKinematic() PxFilterObjectIsTrigger()
*/
typedef PxU32 PxFilterObjectAttributes;


/**
\brief Extract filter object type from the filter attributes of a collision pair object

\param[in] attr The filter attribute of a collision pair object
\return The type of the collision pair object.

@see PxFilterObjectType
*/
PX_INLINE PxFilterObjectType::Enum PxGetFilterObjectType(PxFilterObjectAttributes attr)
{
	return static_cast<PxFilterObjectType::Enum>( (attr & (PxFilterObjectType::eMAX_TYPE_COUNT-1)) );
}


/**
\brief Specifies whether the collision object belongs to a kinematic rigid body

\param[in] attr The filter attribute of a collision pair object
\return True if the object belongs to a kinematic rigid body, else false

@see PxRigidBodyFlag::eKINEMATIC
*/
PX_INLINE bool PxFilterObjectIsKinematic(PxFilterObjectAttributes attr)
{
	return ((attr & PxFilterObjectFlag::eKINEMATIC) != 0);
}


/**
\brief Specifies whether the collision object is a trigger shape

\param[in] attr The filter attribute of a collision pair object
\return True if the object is a trigger shape, else false

@see PxShapeFlag::eTRIGGER_SHAPE
*/
PX_INLINE bool PxFilterObjectIsTrigger(PxFilterObjectAttributes attr)
{
	return ((attr & PxFilterObjectFlag::eTRIGGER) != 0);
}


/**
\brief Filter shader to specify handling of collision pairs.

Collision filtering is a mechanism to specify how a pair of potentially colliding objects should be processed by the
simulation. A pair of objects is potentially colliding if the bounding volumes of the two objects overlap.
In short, a collision filter decides whether a collision pair should get processed, temporarily ignored or discarded.
If a collision pair should get processed, the filter can additionally specify how it should get processed, for instance,
whether contacts should get resolved, which callbacks should get invoked or which reports should be sent etc.

\note A default implementation of a filter shader is provided in the PhysX extensions library, see #PxDefaultSimulationFilterShader.

@see PxSceneDesc.filterShader PxSimulationFilterCallback
*/

/**
\brief Filter method to specify how a pair of potentially colliding objects should be processed.

Return the PxFilterFlag flags and set the PxPairFlag flags to define what the simulation should do with the given collision pair.

This methods gets called when:
\li The bounding volumes of two objects start to overlap.
\li The bounding volumes of two objects overlap and the filter data or filter attributes of one of the objects changed
\li A re-filtering was forced through resetFiltering() (see #PxScene::resetFiltering())
\li Filtering is requested in scene queries

\note Certain pairs of objects are always ignored and this method does not get called. This is the case for the
following pairs:

\li Pair of static rigid actors
\li A static rigid actor and a kinematic actor (unless one is a trigger or if explicitly enabled through #PxSceneFlag::eENABLE_KINEMATIC_STATIC_PAIRS)
\li Two kinematic actors (unless one is a trigger or if explicitly enabled through #PxSceneFlag::eENABLE_KINEMATIC_PAIRS)
\li Pair of particle systems
\li Two jointed rigid bodies and the joint was defined to disable collision
\li Two articulation links if connected through an articulation joint
\li Cloth objects and rigid body actors

\note This is a performance critical method and should be stateless. You should neither access external objects 
from within this method nor should you call external methods that are not inlined. If you need a more complex
logic to filter a collision pair then use the filter callback mechanism for this pair (see #PxSimulationFilterCallback,
#PxFilterFlag::eCALLBACK, #PxFilterFlag::eNOTIFY).

\param[in] attributes0 The filter attribute of the first object
\param[in] filterData0 The custom filter data of the first object
\param[in] attributes1 The filter attribute of the second object
\param[in] filterData1 The custom filter data of the second object
\param[out] pairFlags Flags giving additional information on how an accepted pair should get processed
\param[in] constantBlock The constant global filter data (see #PxSceneDesc.filterShaderData)
\param[in] constantBlockSize Size of the global filter data (see #PxSceneDesc.filterShaderDataSize)
\return Filter flags defining whether the pair should be discarded, temporarily ignored, processed and whether the
filter callback should get invoked for this pair.

@see PxSimulationFilterCallback PxFilterData PxFilterObjectAttributes PxFilterFlag PxFilterFlags PxPairFlag PxPairFlags
*/

typedef PxFilterFlags (*PxSimulationFilterShader)
	(PxFilterObjectAttributes attributes0, PxFilterData filterData0, 
	 PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	 PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize);



/**
\brief Filter callback to specify handling of collision pairs.

This class is provided to implement more complex and flexible collision pair filtering logic, for instance, taking
the state of the user application into account. Filter callbacks also give the user the opportunity to track collision
pairs and update their filter state.

You might want to check the documentation on #PxSimulationFilterShader as well since it includes more general information
on filtering.

\note The callbacks may execute in user threads or simulation threads, possibly simultaneously. The corresponding objects 
may have been deleted by the application earlier in the frame. It is the application's responsibility to prevent race conditions
arising from using the SDK API in the callback while an application thread is making write calls to the scene, and to ensure that
the callbacks are thread-safe. Return values which depend on when the callback is called during the frame will introduce nondeterminism 
into the simulation. On PS3 use of this callback may compromise simulation performance.

@see PxSceneDesc.filterCallback PxSimulationFilterShader
*/
class PxSimulationFilterCallback
{
public:

	/**
	\brief Filter method to specify how a pair of potentially colliding objects should be processed.

	This method gets called when the filter flags returned by the filter shader (see #PxSimulationFilterShader)
	indicate that the filter callback should be invoked (#PxFilterFlag::eCALLBACK or #PxFilterFlag::eNOTIFY set).
	Return the PxFilterFlag flags and set the PxPairFlag flags to define what the simulation should do with the given 
	collision pair.

	\param[in] pairID Unique ID of the collision pair used to issue filter status changes for the pair (see #statusChange())
	\param[in] attributes0 The filter attribute of the first object
	\param[in] filterData0 The custom filter data of the first object
	\param[in] a0 Actor pointer of the first object
	\param[in] s0 Shape pointer of the first object (NULL if the object has no shapes, for example in the case of a particle system)
	\param[in] attributes1 The filter attribute of the second object
	\param[in] filterData1 The custom filter data of the second object
	\param[in] a1 Actor pointer of the second object
	\param[in] s1 Shape pointer of the second object (NULL if the object has no shapes, for example in the case of a  particle system)
	\param[in,out] pairFlags In: Pair flags returned by the filter shader. Out: Additional information on how an accepted pair should get processed
	\return Filter flags defining whether the pair should be discarded, temporarily ignored or processed and whether the pair
	should be tracked and send a report on pair deletion through the filter callback

	@see PxSimulationFilterShader PxFilterData PxFilterObjectAttributes PxFilterFlag PxPairFlag
	*/
	virtual		PxFilterFlags	pairFound(	PxU32 pairID,
		PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxActor* a0, PxShape* s0,
		PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxActor* a1, PxShape* s1,
		PxPairFlags& pairFlags) = 0;

	/**
	\brief Callback to inform that a tracked collision pair is gone.

	This method gets called when a collision pair disappears or gets re-filtered. Only applies to
	collision pairs which have been marked as filter callback pairs (#PxFilterFlag::eNOTIFY set in #pairFound()).

	\param[in] pairID Unique ID of the collision pair that disappeared
	\param[in] attributes0 The filter attribute of the first object
	\param[in] filterData0 The custom filter data of the first object
	\param[in] attributes1 The filter attribute of the second object
	\param[in] filterData1 The custom filter data of the second object
	\param[in] objectDeleted True if the pair was lost because one of the objects got deleted

	@see pairFound() PxSimulationFilterShader PxFilterData PxFilterObjectAttributes
	*/
	virtual		void			pairLost(	PxU32 pairID,
		PxFilterObjectAttributes attributes0,
		PxFilterData filterData0,
		PxFilterObjectAttributes attributes1,
		PxFilterData filterData1,
		bool objectDeleted) = 0;

	/**
	\brief Callback to give the opportunity to change the filter state of a tracked collision pair.

	This method gets called once per simulation step to let the application change the filter and pair
	flags of a collision pair that has been reported in #pairFound() and requested callbacks by
	setting #PxFilterFlag::eNOTIFY. To request a change of filter status, the target pair has to be
	specified by its ID, the new filter and pair flags have to be provided and the method should return true.

	\note If this method changes the filter status of a collision pair and the pair should keep being tracked
	by the filter callbacks then #PxFilterFlag::eNOTIFY has to be set.

	\note The application is responsible to ensure that this method does not get called for pairs that have been
	reported as lost, see #pairLost().

	\param[out] pairID ID of the collision pair for which the filter status should be changed
	\param[out] pairFlags The new pairFlags to apply to the collision pair
	\param[out] filterFlags The new filterFlags to apply to the collision pair
	\return True if the changes should be applied. In this case the method will get called again. False if
	no more status changes should be done in the current simulation step. In that case the provided flags will be discarded.

	@see pairFound() pairLost() PxFilterFlag PxPairFlag
	*/
	virtual		bool			statusChange(PxU32& pairID, PxPairFlags& pairFlags, PxFilterFlags& filterFlags) = 0;

protected:
	virtual						~PxSimulationFilterCallback() {}
};

#ifndef PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
