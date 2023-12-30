
// High-level emulation of PS1's GTE (Geometry Transform Engine) API.
// Non-integer numbers are typically stored in Q12 format (i.e. fixed 12 bits
// after decimal point). Angles are normalized so that 1.0 (in Q12) represents
// 360-degrees.

#pragma once


// short vector with explicit padding
struct SVECTOR
{
    short vx, vy, vz;
    short pad;
};


// (long) vector with explicit padding
struct VECTOR
{
    long vx, vy, vz;
    long pad;
};


// 3x3 (rotation) matrix plus long translation vector
struct MATRIX
{
    short m[3][3];
    long t[3];
};

// integer square root
extern "C" long SquareRoot0(long a);

// Q12 atan2
extern "C" long ratan2(long y, long x);

// Q12 cos
extern "C" int rcos(int a);

// Q12 sin
extern "C" int rsin(int a);

/// <summary>
/// Computes a rotation matrix in XYZ order.
/// </summary>
/// <param name="r">In Q12 vector of angles</param>
/// <param name="m">Out matrix</param>
/// <returns>m</returns>
extern "C" MATRIX *RotMatrix(const SVECTOR *r, MATRIX *m);

/// <summary>
/// Computes a rotation matrix in YXZ order.
/// </summary>
/// <param name="r">In Q12 vector of angles</param>
/// <param name="m">Out matrix</param>
/// <returns>m</returns>
extern "C" MATRIX *RotMatrixYXZ(const SVECTOR *r, MATRIX *m);

/// <summary>
/// Computes a rotation matrix in ZYX order.
/// </summary>
/// <param name="r">In Q12 vector of angles</param>
/// <param name="m">Out matrix</param>
/// <returns>m</returns>
extern "C" MATRIX *RotMatrixZYX(const SVECTOR *r, MATRIX *m);

/// <summary>
/// Left-multiplies a matrix by an X-axis rotation.
/// </summary>
/// <param name="r">Q12 angle</param>
/// <param name="m">In/out matrix</param>
/// <returns>m</returns>
extern "C" MATRIX *RotMatrixX(int r, MATRIX *m);

/// <summary>
/// Left-multiplies a matrix by a Y-axis rotation.
/// </summary>
/// <param name="r">Q12 angle</param>
/// <param name="m">In/out matrix</param>
/// <returns>m</returns>
extern "C" MATRIX *RotMatrixY(int r, MATRIX *m);

/// <summary>
/// Left-multiplies a matrix by a Z-axis rotation.
/// </summary>
/// <param name="r">Q12 angle</param>
/// <param name="m">In/out matrix</param>
/// <returns>m</returns>
extern "C" MATRIX *RotMatrixZ(int r, MATRIX *m);

/// <summary>
/// Scales a matrix's columns by corresponding vector components.
/// </summary>
/// <param name="m">In/out matrix</param>
/// <param name="v">In Q12 vector</param>
/// <returns>m</returns>
extern "C" MATRIX *ScaleMatrix(MATRIX *m, const VECTOR *v);

/// <summary>
/// Sets the translation vector of a matrix.
/// </summary>
/// <param name="m">In/out matrix</param>
/// <param name="v">In Q12 vector</param>
/// <returns>m</returns>
extern "C" MATRIX *TransMatrix(MATRIX *m, const VECTOR *v);

/// <summary>
/// Applies the 3x3 part of a matrix to a vector.
/// </summary>
/// <param name="m">In matrix</param>
/// <param name="v0">In Q12 vector</param>
/// <param name="v1">Out Q12 vector</param>
/// <returns>v1</returns>
extern "C" VECTOR *ApplyMatrix(const MATRIX *m, const SVECTOR *v0, VECTOR *v1);

/// <summary>
/// Applies the 3x3 part of a matrix to a vector.
/// </summary>
/// <param name="m">In matrix</param>
/// <param name="v0">In Q12 vector</param>
/// <param name="v1">Out Q12 vector</param>
/// <returns>v1</returns>
extern "C" SVECTOR *ApplyMatrixSV(
    const MATRIX *m,
    const SVECTOR *v0,
    SVECTOR *v1
);

/// <summary>
/// Applies the 3x3 part of a matrix to a vector.
/// </summary>
/// <param name="m">In matrix</param>
/// <param name="v0">In Q12 vector</param>
/// <param name="v1">Out Q12 vector</param>
/// <returns>v1</returns>
extern "C" VECTOR *ApplyMatrixLV(const MATRIX *m, const VECTOR *v0, VECTOR *v1);

/// <summary>
/// Multiplies the 3x3 part of two matrices.
/// </summary>
/// <param name="m0">In LHS matrix</param>
/// <param name="m1">In RHS matrix</param>
/// <param name="m2">Out matrix</param>
/// <returns>m2</returns>
extern "C" MATRIX *MulMatrix0(const MATRIX *m0, const MATRIX *m1, MATRIX *m2);

/// <summary>
/// Multiplies the 3x3 part of two matrices
/// </summary>
/// <param name="m0">In LHS matrix</param>
/// <param name="m1">In RHS/out matrix</param>
/// <returns>m1</returns>
extern "C" MATRIX *MulMatrix2(const MATRIX *m0, MATRIX *m1);

/// <summary>
/// Multiplies the 3x3 part of two matrices. Same as MulMatrix2
/// </summary>
/// <param name="m0">In LHS matrix</param>
/// <param name="m1">In RHS/out matrix</param>
/// <returns>m1</returns>
extern "C" MATRIX *MulMatrix2_0(const MATRIX *m0, MATRIX *m1);

/// <summary>
/// Sets the 3x3 part of a matrix to the identity transform.
/// </summary>
/// <param name="m">In/out matrix</param>
/// <returns>m</returns>
extern "C" MATRIX *IdentMatrix(MATRIX *m);

/// <summary>
/// Applies the 3x3 part of a matrix to a vector. Same as ApplyMatrix.
/// </summary>
/// <param name="m">In matrix</param>
/// <param name="v0">In Q12 vector</param>
/// <param name="v1">Out Q12 vector</param>
/// <returns>v1</returns>
extern "C" VECTOR *ApplyMatrix_0(
    const MATRIX *m,
    const SVECTOR *v0,
    VECTOR *v1
);

/// <summary>
/// Applies the 3x3 part of a matrix to a vector. Same as ApplyMatrixSV.
/// </summary>
/// <param name="m">In matrix</param>
/// <param name="v0">In Q12 vector</param>
/// <param name="v1">Out Q12 vector</param>
/// <returns>v1</returns>
extern "C" SVECTOR *ApplyMatrixSV_0(
    const MATRIX *m,
    const SVECTOR *v0,
    SVECTOR *v1
);

/// <summary>
/// Computes the integer square of vector components.
/// </summary>
/// <param name="v0">In integer vector</param>
/// <param name="v1">Out integer vector</param>
extern "C" void Square0(const VECTOR *v0, VECTOR *v1);
