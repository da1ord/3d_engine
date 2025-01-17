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


#ifndef PX_FOUNDATION_PX_MAT33_H
#define PX_FOUNDATION_PX_MAT33_H
/** \addtogroup foundation
@{
*/

#include "foundation/PxVec3.h"
#include "foundation/PxQuat.h"

#ifndef PX_DOXYGEN
namespace physx
{
#endif
/*!
\brief 3x3 matrix class

Some clarifications, as there have been much confusion about matrix formats etc in the past.

Short:
- Matrix have base vectors in columns (vectors are column matrices, 3x1 matrices).
- Matrix is physically stored in column major format
- Matrices are concaternated from left

Long:
Given three base vectors a, b and c the matrix is stored as
         
|a.x b.x c.x|
|a.y b.y c.y|
|a.z b.z c.z|

Vectors are treated as columns, so the vector v is 

|x|
|y|
|z|

And matrices are applied _before_ the vector (pre-multiplication)
v' = M*v

|x'|   |a.x b.x c.x|   |x|   |a.x*x + b.x*y + c.x*z|
|y'| = |a.y b.y c.y| * |y| = |a.y*x + b.y*y + c.y*z|
|z'|   |a.z b.z c.z|   |z|   |a.z*x + b.z*y + c.z*z|


Physical storage and indexing:
To be compatible with popular 3d rendering APIs (read D3d and OpenGL)
the physical indexing is

|0 3 6|
|1 4 7|
|2 5 8|

index = column*3 + row

which in C++ translates to M[column][row]

The mathematical indexing is M_row,column and this is what is used for _-notation 
so _12 is 1st row, second column and operator(row, column)!

*/
class PxMat33
{
public:
	//! Default constructor
	PX_CUDA_CALLABLE PX_INLINE PxMat33()
	{}

	//! identity constructor
	PX_CUDA_CALLABLE PX_INLINE PxMat33(PxIDENTITY r)
		: column0(1,0,0), column1(0,1,0), column2(0,0,1)
	{
		PX_UNUSED(r);
	}

	//! zero constructor
	PX_CUDA_CALLABLE PX_INLINE PxMat33(PxZERO r)
		: column0(0), column1(0), column2(0)
	{
		PX_UNUSED(r);
	}


	//! Construct from three base vectors
	PX_CUDA_CALLABLE PxMat33(const PxVec3& col0, const PxVec3& col1, const PxVec3& col2)
		: column0(col0), column1(col1), column2(col2)
	{}


	//! constructor from a scalar, which generates a multiple of the identity matrix
	explicit PX_CUDA_CALLABLE PX_INLINE PxMat33(PxReal r)
		: column0(r,0,0), column1(0,r,0), column2(0,0,r)
	{}


	//! Construct from float[9]
	explicit PX_CUDA_CALLABLE PX_INLINE PxMat33(PxReal values[]):
		column0(values[0],values[1],values[2]),
		column1(values[3],values[4],values[5]),
		column2(values[6],values[7],values[8])
	{
	}

	//! Construct from a quaternion
	explicit PX_CUDA_CALLABLE PX_FORCE_INLINE PxMat33(const PxQuat& q)
	{
		const PxReal x = q.x;
		const PxReal y = q.y;
		const PxReal z = q.z;
		const PxReal w = q.w;

		const PxReal x2 = x + x;
		const PxReal y2 = y + y;
		const PxReal z2 = z + z;

		const PxReal xx = x2*x;
		const PxReal yy = y2*y;
		const PxReal zz = z2*z;

		const PxReal xy = x2*y;
		const PxReal xz = x2*z;
		const PxReal xw = x2*w;

		const PxReal yz = y2*z;
		const PxReal yw = y2*w;
		const PxReal zw = z2*w;

		column0 = PxVec3(1.0f - yy - zz, xy + zw, xz - yw);
		column1 = PxVec3(xy - zw, 1.0f - xx - zz, yz + xw);
		column2 = PxVec3(xz + yw, yz - xw, 1.0f - xx - yy);
	}

	//! Copy constructor
	PX_CUDA_CALLABLE PX_INLINE PxMat33(const PxMat33& other)
		: column0(other.column0), column1(other.column1), column2(other.column2)
	{}

	//! Assignment operator
	PX_CUDA_CALLABLE PX_FORCE_INLINE PxMat33& operator=(const PxMat33& other)
	{
		column0 = other.column0;
		column1 = other.column1;
		column2 = other.column2;
		return *this;
	}

	//! \deprecated Set to identity matrix. Deprecated. use PxMat33(PxIdentity)
	PX_DEPRECATED PX_CUDA_CALLABLE PX_INLINE static PxMat33 createIdentity()
	{
		return PxMat33(PxIdentity);
	}

	//! \deprecated Set to zero matrix. Deprecated. use PxMat33(PxZero).
	PX_DEPRECATED PX_CUDA_CALLABLE PX_INLINE static PxMat33 createZero()
	{
		return PxMat33(PxZero);	// PxMat33(0) is ambiguous, it can either be the array constructor or the scalar constructor
	}

	//! Construct from diagonal, off-diagonals are zero.
	PX_CUDA_CALLABLE PX_INLINE static PxMat33 createDiagonal(const PxVec3& d)
	{
		return PxMat33(PxVec3(d.x,0.0f,0.0f), PxVec3(0.0f,d.y,0.0f), PxVec3(0.0f,0.0f,d.z));
	}


	//! Get transposed matrix
	PX_CUDA_CALLABLE PX_FORCE_INLINE PxMat33 getTranspose() const
	{
		const PxVec3 v0(column0.x, column1.x, column2.x);
		const PxVec3 v1(column0.y, column1.y, column2.y);
		const PxVec3 v2(column0.z, column1.z, column2.z);

		return PxMat33(v0,v1,v2);   
	}

	//! Get the real inverse
	PX_CUDA_CALLABLE PX_INLINE PxMat33 getInverse() const
	{
		const PxReal det = getDeterminant();
		PxMat33 inverse;

		if(det != 0)
		{
			const PxReal invDet = 1.0f/det;

			inverse.column0[0] = invDet * (column1[1]*column2[2] - column2[1]*column1[2]);							
			inverse.column0[1] = invDet *-(column0[1]*column2[2] - column2[1]*column0[2]);
			inverse.column0[2] = invDet * (column0[1]*column1[2] - column0[2]*column1[1]);

			inverse.column1[0] = invDet *-(column1[0]*column2[2] - column1[2]*column2[0]);
			inverse.column1[1] = invDet * (column0[0]*column2[2] - column0[2]*column2[0]);
			inverse.column1[2] = invDet *-(column0[0]*column1[2] - column0[2]*column1[0]);

			inverse.column2[0] = invDet * (column1[0]*column2[1] - column1[1]*column2[0]);
			inverse.column2[1] = invDet *-(column0[0]*column2[1] - column0[1]*column2[0]);
			inverse.column2[2] = invDet * (column0[0]*column1[1] - column1[0]*column0[1]);

			return inverse;
		}
		else
		{
			return createIdentity();
		}
	}

	//! Get determinant
	PX_CUDA_CALLABLE PX_INLINE PxReal getDeterminant() const
	{
		return column0.dot(column1.cross(column2));
	}

	//! Unary minus
	PX_CUDA_CALLABLE PX_INLINE PxMat33 operator-() const
	{
		return PxMat33(-column0, -column1, -column2);
	}

	//! Add
	PX_CUDA_CALLABLE PX_INLINE PxMat33 operator+(const PxMat33& other) const
	{
		return PxMat33( column0+other.column0,
					  column1+other.column1,
					  column2+other.column2);
	}

	//! Subtract
	PX_CUDA_CALLABLE PX_INLINE PxMat33 operator-(const PxMat33& other) const
	{
		return PxMat33( column0-other.column0,
					  column1-other.column1,
					  column2-other.column2);
	}

	//! Scalar multiplication
	PX_CUDA_CALLABLE PX_INLINE PxMat33 operator*(PxReal scalar) const
	{
		return PxMat33(column0*scalar, column1*scalar, column2*scalar);
	}

	friend PxMat33 operator*(PxReal, const PxMat33&);

	//! Matrix vector multiplication (returns 'this->transform(vec)')
	PX_CUDA_CALLABLE PX_INLINE PxVec3 operator*(const PxVec3& vec) const
	{
		return transform(vec);
	}

	//! Matrix multiplication
	PX_CUDA_CALLABLE PX_FORCE_INLINE PxMat33 operator*(const PxMat33& other) const
	{
		//Rows from this <dot> columns from other
		//column0 = transform(other.column0) etc
		return PxMat33(transform(other.column0), transform(other.column1), transform(other.column2));
	}

	// a <op>= b operators

	//! Equals-add
	PX_CUDA_CALLABLE PX_INLINE PxMat33& operator+=(const PxMat33& other)
	{
		column0 += other.column0;
		column1 += other.column1;
		column2 += other.column2;
		return *this;
	}

	//! Equals-sub
	PX_CUDA_CALLABLE PX_INLINE PxMat33& operator-=(const PxMat33& other)
	{
		column0 -= other.column0;
		column1 -= other.column1;
		column2 -= other.column2;
		return *this;
	}

	//! Equals scalar multiplication
	PX_CUDA_CALLABLE PX_INLINE PxMat33& operator*=(PxReal scalar)
	{
		column0 *= scalar;
		column1 *= scalar;
		column2 *= scalar;
		return *this;
	}

	//! Element access, mathematical way!
	PX_CUDA_CALLABLE PX_FORCE_INLINE PxReal operator()(unsigned int row, unsigned int col) const
	{
		return (*this)[col][row];
	}

	//! Element access, mathematical way!
	PX_CUDA_CALLABLE PX_FORCE_INLINE PxReal& operator()(unsigned int row, unsigned int col)
	{
		return (*this)[col][row];
	}

	// Transform etc
	
	//! Transform vector by matrix, equal to v' = M*v
	PX_CUDA_CALLABLE PX_FORCE_INLINE PxVec3 transform(const PxVec3& other) const
	{
		return column0*other.x + column1*other.y + column2*other.z;
	}

	//! Transform vector by matrix transpose, v' = M^t*v
	PX_CUDA_CALLABLE PX_INLINE PxVec3 transformTranspose(const PxVec3& other) const
	{
		return PxVec3(	column0.dot(other),
						column1.dot(other),
						column2.dot(other));
	}

	PX_CUDA_CALLABLE PX_FORCE_INLINE const PxReal* front() const
	{
		return &column0.x;
	}

	PX_CUDA_CALLABLE PX_FORCE_INLINE			PxVec3& operator[](int num)			{return (&column0)[num];}
	PX_CUDA_CALLABLE PX_FORCE_INLINE const	PxVec3& operator[](int num) const	{return (&column0)[num];}

	//Data, see above for format!

	PxVec3 column0, column1, column2; //the three base vectors
};

// implementation from PxQuat.h
PX_CUDA_CALLABLE PX_INLINE PxQuat::PxQuat(const PxMat33& m)
{
	PxReal tr = m(0,0) + m(1,1) + m(2,2), h;
	if(tr >= 0)
	{
		h = PxSqrt(tr +1);
		w = PxReal(0.5) * h;
		h = PxReal(0.5) / h;

		x = (m(2,1) - m(1,2)) * h;
		y = (m(0,2) - m(2,0)) * h;
		z = (m(1,0) - m(0,1)) * h;
	}
	else
	{
		int i = 0; 
		if (m(1,1) > m(0,0))
			i = 1; 
		if (m(2,2) > m(i,i))
			i = 2; 
		switch (i)
		{
		case 0:
			h = PxSqrt((m(0,0) - (m(1,1) + m(2,2))) + 1);
			x = PxReal(0.5) * h;
			h = PxReal(0.5) / h;

			y = (m(0,1) + m(1,0)) * h; 
			z = (m(2,0) + m(0,2)) * h;
			w = (m(2,1) - m(1,2)) * h;
			break;
		case 1:
			h = PxSqrt((m(1,1) - (m(2,2) + m(0,0))) + 1);
			y = PxReal(0.5) * h;
			h = PxReal(0.5) / h;

			z = (m(1,2) + m(2,1)) * h;
			x = (m(0,1) + m(1,0)) * h;
			w = (m(0,2) - m(2,0)) * h;
			break;
		case 2:
			h = PxSqrt((m(2,2) - (m(0,0) + m(1,1))) + 1);
			z = PxReal(0.5) * h;
			h = PxReal(0.5) / h;

			x = (m(2,0) + m(0,2)) * h;
			y = (m(1,2) + m(2,1)) * h;
			w = (m(1,0) - m(0,1)) * h;
			break;
		default: // Make compiler happy
			x = y = z = w = 0;
			break;
		}
	}
}

#ifndef PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif // PX_FOUNDATION_PX_MAT33_H
