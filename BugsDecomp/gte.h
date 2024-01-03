
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
PATCH_CODE(0x4079e0, 0x4079c0, SquareRoot0);

// Q12 atan2
extern "C" long ratan2(long y, long x);
PATCH_CODE(0x407a00, 0x4079e0, ratan2);

// Q12 cos
extern "C" int rcos(int a);
PATCH_CODE(0x407a30, 0x407a10, rcos);

// Q12 sin
extern "C" int rsin(int a);
PATCH_CODE(0x407a50, 0x407a30, rsin);

/// <summary>
/// Computes a rotation matrix in XYZ order.
/// </summary>
/// <param name="r">In Q12 vector of angles</param>
/// <param name="m">Out matrix</param>
/// <returns>m</returns>
extern "C" MATRIX *RotMatrix(const SVECTOR *r, MATRIX *m);
PATCH_CODE(0x407a70, 0x407a50, RotMatrix);

/// <summary>
/// Computes a rotation matrix in YXZ order.
/// </summary>
/// <param name="r">In Q12 vector of angles</param>
/// <param name="m">Out matrix</param>
/// <returns>m</returns>
extern "C" MATRIX *RotMatrixYXZ(const SVECTOR *r, MATRIX *m);
PATCH_CODE(0x407b10, 0x407af0, RotMatrixYXZ);

/// <summary>
/// Computes a rotation matrix in ZYX order.
/// </summary>
/// <param name="r">In Q12 vector of angles</param>
/// <param name="m">Out matrix</param>
/// <returns>m</returns>
extern "C" MATRIX *RotMatrixZYX(const SVECTOR *r, MATRIX *m);
PATCH_CODE(0x407bb0, 0x407b90, RotMatrixZYX);

/// <summary>
/// Left-multiplies a matrix by an X-axis rotation.
/// </summary>
/// <param name="r">Q12 angle</param>
/// <param name="m">In/out matrix</param>
/// <returns>m</returns>
extern "C" MATRIX *RotMatrixX(int r, MATRIX *m);
PATCH_CODE(0x407c50, 0x407c30, RotMatrixX);

/// <summary>
/// Left-multiplies a matrix by a Y-axis rotation.
/// </summary>
/// <param name="r">Q12 angle</param>
/// <param name="m">In/out matrix</param>
/// <returns>m</returns>
extern "C" MATRIX *RotMatrixY(int r, MATRIX *m);
PATCH_CODE(0x407dc0, 0x407da0, RotMatrixY);

/// <summary>
/// Left-multiplies a matrix by a Z-axis rotation.
/// </summary>
/// <param name="r">Q12 angle</param>
/// <param name="m">In/out matrix</param>
/// <returns>m</returns>
extern "C" MATRIX *RotMatrixZ(int r, MATRIX *m);
PATCH_CODE(0x407f20, 0x407f00, RotMatrixZ);

/// <summary>
/// Scales a matrix's columns by corresponding vector components.
/// </summary>
/// <param name="m">In/out matrix</param>
/// <param name="v">In Q12 vector</param>
/// <returns>m</returns>
extern "C" MATRIX *ScaleMatrix(MATRIX *m, const VECTOR *v);
PATCH_CODE(0x408080, 0x408060, ScaleMatrix);

/// <summary>
/// Sets the translation vector of a matrix.
/// </summary>
/// <param name="m">In/out matrix</param>
/// <param name="v">In Q12 vector</param>
/// <returns>m</returns>
extern "C" MATRIX *TransMatrix(MATRIX *m, const VECTOR *v);
PATCH_CODE(0x408120, 0x408100, TransMatrix);

/// <summary>
/// Applies the 3x3 part of a matrix to a vector.
/// </summary>
/// <param name="m">In matrix</param>
/// <param name="v0">In Q12 vector</param>
/// <param name="v1">Out Q12 vector</param>
/// <returns>v1</returns>
extern "C" VECTOR *ApplyMatrix(const MATRIX *m, const SVECTOR *v0, VECTOR *v1);
PATCH_CODE(0x408140, 0x408120, ApplyMatrix);

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
PATCH_CODE(0x4081e0, 0x4081c0, ApplyMatrixSV);

/// <summary>
/// Applies the 3x3 part of a matrix to a vector.
/// </summary>
/// <param name="m">In matrix</param>
/// <param name="v0">In Q12 vector</param>
/// <param name="v1">Out Q12 vector</param>
/// <returns>v1</returns>
extern "C" VECTOR *ApplyMatrixLV(const MATRIX *m, const VECTOR *v0, VECTOR *v1);
PATCH_CODE(0x408280, 0x408260, ApplyMatrixLV);

/// <summary>
/// Multiplies the 3x3 part of two matrices.
/// </summary>
/// <param name="m0">In LHS matrix</param>
/// <param name="m1">In RHS matrix</param>
/// <param name="m2">Out matrix</param>
/// <returns>m2</returns>
extern "C" MATRIX *MulMatrix0(const MATRIX *m0, const MATRIX *m1, MATRIX *m2);
PATCH_CODE(0x408310, 0x4082f0, MulMatrix0);

/// <summary>
/// Multiplies the 3x3 part of two matrices.
/// </summary>
/// <param name="m0">In LHS matrix</param>
/// <param name="m1">In RHS/out matrix</param>
/// <returns>m1</returns>
extern "C" MATRIX *MulMatrix2(const MATRIX *m0, MATRIX *m1);
PATCH_CODE(0x4084b0, 0x408490, MulMatrix2);

/// <summary>
/// Multiplies the 3x3 part of two matrices. Same as MulMatrix2.
/// </summary>
/// <param name="m0">In LHS matrix</param>
/// <param name="m1">In RHS/out matrix</param>
/// <returns>m1</returns>
extern "C" MATRIX *MulMatrix2_0(const MATRIX *m0, MATRIX *m1);
PATCH_CODE(0x408660, 0x408640, MulMatrix2_0);

/// <summary>
/// Composes two matrices.
/// </summary>
/// <param name="m0">In LHS matrix</param>
/// <param name="m1">In RHS matrix</param>
/// <param name="m2">Out matrix</param>
/// <returns>m2</returns>
extern "C" MATRIX *CompMatrixLV(const MATRIX *m0, const MATRIX *m1, MATRIX *m2);
PATCH_CODE(0x4086b0, 0x408690, CompMatrixLV);

/// <summary>
/// Composes two matrices.
/// </summary>
/// <param name="m0">In LHS matrix</param>
/// <param name="m1">In LHS/out matrix</param>
/// <returns>m1</returns>
extern "C" MATRIX *CompMatrix2LV(const MATRIX *m0, MATRIX *m1);
PATCH_CODE(0x4088d0, 0x4088b0, CompMatrix2LV);

/// <summary>
/// Sets the 3x3 part of a matrix to the identity transform.
/// </summary>
/// <param name="m">In/out matrix</param>
/// <returns>m</returns>
extern "C" MATRIX *IdentMatrix(MATRIX *m);
PATCH_CODE(0x408a40, 0x408a20, IdentMatrix);

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
PATCH_CODE(0x408a80, 0x408a60, ApplyMatrix_0);

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
PATCH_CODE(0x408b20, 0x408b00, ApplyMatrixSV_0);

/// <summary>
/// Computes the integer square of vector components.
/// </summary>
/// <param name="v0">In integer vector</param>
/// <param name="v1">Out integer vector</param>
extern "C" void Square0(const VECTOR *v0, VECTOR *v1);
PATCH_CODE(0x408bc0, 0x408ba0, Square0);
