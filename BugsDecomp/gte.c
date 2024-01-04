
#include "stdafx.h"
#include "common.h"
#include "gte.h"


#define rsin_tbl (*(const int (*)[4096])0x45f3f0)
#define rcos_tbl (*(const int (*)[4096])0x4633f0)


long SquareRoot0(long a)
{
    return (long)sqrt((double)a);
}

long ratan2(long y, long x)
{
    return (long)(atan2((double)y, (double)x) * BDBL(0x45c270, 4096.0 / TAU));
}

int rcos(int a)
{
    return rcos_tbl[abs(a) & 0xfff];
}

int rsin(int a)
{
    return rsin_tbl[(a + 0x1000) & 0xfff];
}

MATRIX *RotMatrix(const SVECTOR *r, MATRIX *m)
{
    // initialize m to Z-rotation matrix
    int c = rcos(r->vz), s = rsin(r->vz);
    m->m[0][0] = c;      m->m[0][1] = -s;     m->m[0][2] = 0x0000;
    m->m[1][0] = s;      m->m[1][1] = c;      m->m[1][2] = 0x0000;
    m->m[2][0] = 0x0000; m->m[2][1] = 0x0000; m->m[2][2] = 0x1000;

    // apply additional rotations
    RotMatrixY(r->vy, m);
    RotMatrixX(r->vx, m);

    return m;
}

MATRIX *RotMatrixYXZ(const SVECTOR *r, MATRIX *m)
{
    // initialize m to Z-rotation matrix
    int c = rcos(r->vz), s = rsin(r->vz);
    m->m[0][0] = c;      m->m[0][1] = -s;     m->m[0][2] = 0x0000;
    m->m[1][0] = s;      m->m[1][1] = c;      m->m[1][2] = 0x0000;
    m->m[2][0] = 0x0000; m->m[2][1] = 0x0000; m->m[2][2] = 0x1000;

    // apply additional rotations
    RotMatrixX(r->vx, m);
    RotMatrixY(r->vy, m);

    return m;
}

MATRIX *RotMatrixZYX(const SVECTOR *r, MATRIX *m)
{
    // initialize m to X-rotation matrix
    int c = rcos(r->vx), s = rsin(r->vx);
    m->m[0][0] = 0x1000; m->m[0][1] = 0x0000; m->m[0][2] = 0x0000;
    m->m[1][0] = 0x0000; m->m[1][1] = c;      m->m[1][2] = -s;
    m->m[2][0] = 0x0000; m->m[2][1] = s;      m->m[2][2] = c;

    // apply additional rotations
    RotMatrixY(r->vy, m);
    RotMatrixZ(r->vz, m);

    return m;
}

MATRIX *RotMatrixX(int r, MATRIX *m)
{
    // precompute cos/sin of angle
    int c = rcos(r), s = rsin(r);

    // compute middle row of results into temp vars (since original elements are
    // still needed for computing bottom row)
    // NOTE: game does the normalizing shifts BEFORE addition/subtraction;
    // replicating that here
    short m10 = ((c * m->m[1][0]) >> 12) - ((s * m->m[2][0]) >> 12);
    short m11 = ((c * m->m[1][1]) >> 12) - ((s * m->m[2][1]) >> 12);
    short m12 = ((c * m->m[1][2]) >> 12) - ((s * m->m[2][2]) >> 12);

    // compute bottom row in-place
    m->m[2][0] = ((s * m->m[1][0]) >> 12) + ((c * m->m[2][0]) >> 12);
    m->m[2][1] = ((s * m->m[1][1]) >> 12) + ((c * m->m[2][1]) >> 12);
    m->m[2][2] = ((s * m->m[1][2]) >> 12) + ((c * m->m[2][2]) >> 12);

    // write middle row
    m->m[1][0] = m10;
    m->m[1][1] = m11;
    m->m[1][2] = m12;

    return m;
}

MATRIX *RotMatrixY(int r, MATRIX *m)
{
    // NOTE: Psy-Q documentation shows the wrong Y rotation matrix (it shows the
    // inverse/transpose of the correct matrix)

    // see RotMatrixX above for more notes

    // precompute cos/sin of angle
    int c = rcos(r), s = rsin(r);

    // compute top row of results into temp vars
    short m00 = ((c * m->m[0][0]) >> 12) + ((s * m->m[2][0]) >> 12);
    short m01 = ((c * m->m[0][1]) >> 12) + ((s * m->m[2][1]) >> 12);
    short m02 = ((c * m->m[0][2]) >> 12) + ((s * m->m[2][2]) >> 12);

    // compute bottom row in-place
    m->m[2][0] = ((-s * m->m[0][0]) >> 12) + ((c * m->m[2][0]) >> 12);
    m->m[2][1] = ((-s * m->m[0][1]) >> 12) + ((c * m->m[2][1]) >> 12);
    m->m[2][2] = ((-s * m->m[0][2]) >> 12) + ((c * m->m[2][2]) >> 12);

    // write top row
    m->m[0][0] = m00;
    m->m[0][1] = m01;
    m->m[0][2] = m02;

    return m;
}

MATRIX *RotMatrixZ(int r, MATRIX *m)
{
    // see RotMatrixX above for more notes

    // precompute cos/sin of angle
    int c = rcos(r), s = rsin(r);

    // compute top row of results into temp vars
    short m00 = ((c * m->m[0][0]) >> 12) - ((s * m->m[1][0]) >> 12);
    short m01 = ((c * m->m[0][1]) >> 12) - ((s * m->m[1][1]) >> 12);
    short m02 = ((c * m->m[0][2]) >> 12) - ((s * m->m[1][2]) >> 12);

    // compute middle row in-place
    m->m[1][0] = ((s * m->m[0][0]) >> 12) + ((c * m->m[1][0]) >> 12);
    m->m[1][1] = ((s * m->m[0][1]) >> 12) + ((c * m->m[1][1]) >> 12);
    m->m[1][2] = ((s * m->m[0][2]) >> 12) + ((c * m->m[1][2]) >> 12);

    // write top row
    m->m[0][0] = m00;
    m->m[0][1] = m01;
    m->m[0][2] = m02;

    return m;
}

MATRIX *ScaleMatrix(MATRIX *m, const VECTOR *v)
{
    m->m[0][0] = (short)((m->m[0][0] * v->vx) >> 12);
    m->m[0][1] = (short)((m->m[0][1] * v->vy) >> 12);
    m->m[0][2] = (short)((m->m[0][2] * v->vz) >> 12);
    m->m[1][0] = (short)((m->m[1][0] * v->vx) >> 12);
    m->m[1][1] = (short)((m->m[1][1] * v->vy) >> 12);
    m->m[1][2] = (short)((m->m[1][2] * v->vz) >> 12);
    m->m[2][0] = (short)((m->m[2][0] * v->vx) >> 12);
    m->m[2][1] = (short)((m->m[2][1] * v->vy) >> 12);
    m->m[2][2] = (short)((m->m[2][2] * v->vz) >> 12);

    return m;
}

MATRIX *TransMatrix(MATRIX *m, const VECTOR *v)
{
    m->t[0] = v->vx;
    m->t[1] = v->vy;
    m->t[2] = v->vz;
    return m;
}

VECTOR *ApplyMatrix(const MATRIX *m, const SVECTOR *v0, VECTOR *v1)
{
    // despite Psy-Q documentation, translation is NOT applied
    v1->vx =
        (m->m[0][0] * v0->vx + m->m[0][1] * v0->vy + m->m[0][2] * v0->vz) >> 12;
    v1->vy =
        (m->m[1][0] * v0->vx + m->m[1][1] * v0->vy + m->m[1][2] * v0->vz) >> 12;
    v1->vz =
        (m->m[2][0] * v0->vx + m->m[2][1] * v0->vy + m->m[2][2] * v0->vz) >> 12;
    return v1;
}

SVECTOR *ApplyMatrixSV(
    const MATRIX *m,
    const SVECTOR *v0,
    SVECTOR *v1
)
{
    SVECTOR tmp;

    tmp.vx =
        (m->m[0][0] * v0->vx + m->m[0][1] * v0->vy + m->m[0][2] * v0->vz) >> 12;
    tmp.vy =
        (m->m[1][0] * v0->vx + m->m[1][1] * v0->vy + m->m[1][2] * v0->vz) >> 12;
    tmp.vz =
        (m->m[2][0] * v0->vx + m->m[2][1] * v0->vy + m->m[2][2] * v0->vz) >> 12;

    *v1 = tmp;
    return v1;
}

VECTOR *ApplyMatrixLV(const MATRIX *m, const VECTOR *v0, VECTOR *v1)
{
    VECTOR tmp;

    tmp.vx =
        (m->m[0][0] * v0->vx + m->m[0][1] * v0->vy + m->m[0][2] * v0->vz) >> 12;
    tmp.vy =
        (m->m[1][0] * v0->vx + m->m[1][1] * v0->vy + m->m[1][2] * v0->vz) >> 12;
    tmp.vz =
        (m->m[2][0] * v0->vx + m->m[2][1] * v0->vy + m->m[2][2] * v0->vz) >> 12;

    *v1 = tmp;
    return v1;
}

MATRIX *MulMatrix0(const MATRIX *m0, const MATRIX *m1, MATRIX *m2)
{
    for (unsigned r = 0; r < 3; r++)
    {
        for (unsigned c = 0; c < 3; c++)
        {
            m2->m[r][c] = (
                m0->m[r][0] * m1->m[0][c] +
                m0->m[r][1] * m1->m[1][c] +
                m0->m[r][2] * m1->m[2][c]
            ) >> 12;
        }
    }
    return m2;
}

MATRIX *MulMatrix2(const MATRIX *m0, MATRIX *m1)
{
    MATRIX tmp;

    MulMatrix0(m0, m1, &tmp);

    // only copy up to translation part
    memcpy(m1, &tmp, FIELD_OFFSET(MATRIX, t));

    return m1;
}

MATRIX *MulMatrix2_0(const MATRIX *m0, MATRIX *m1)
{
    MATRIX tmp;

    MulMatrix0(m0, m1, &tmp);

    // copy translation from dst...
    tmp.t[0] = m1->t[0];
    tmp.t[1] = m1->t[1];
    tmp.t[2] = m1->t[2];
    // ... then copy everything to dst
    *m1 = tmp;

    return m1;
}

MATRIX *CompMatrixLV(const MATRIX *m0, const MATRIX *m1, MATRIX *m2)
{
    // multiply 3x3 parts
    MulMatrix0(m0, m1, m2);

    // compute translation
    m2->t[0] = m0->t[0] + ((
        m0->m[0][0] * m1->t[0] +
        m0->m[0][1] * m1->t[1] +
        m0->m[0][2] * m1->t[2]
    ) >> 12);
    m2->t[1] = m0->t[1] + ((
        m0->m[1][0] * m1->t[0] +
        m0->m[1][1] * m1->t[1] +
        m0->m[1][2] * m1->t[2]
    ) >> 12);
    m2->t[2] = m0->t[2] + ((
        m0->m[2][0] * m1->t[0] +
        m0->m[2][1] * m1->t[1] +
        m0->m[2][2] * m1->t[2]
    ) >> 12);

    return m2;
}

MATRIX *CompMatrix2LV(const MATRIX *m0, MATRIX *m1)
{
    MATRIX tmp;

    CompMatrixLV(m0, m1, &tmp);
    *m1 = tmp;

    return m1;
}

MATRIX *IdentMatrix(MATRIX *m)
{
    m->m[0][0] = 0x1000; m->m[0][1] = 0x0000; m->m[0][2] = 0x0000;
    m->m[1][0] = 0x0000; m->m[1][1] = 0x1000; m->m[1][2] = 0x0000;
    m->m[2][0] = 0x0000; m->m[2][1] = 0x0000; m->m[2][2] = 0x1000;
    return m;
}

VECTOR *ApplyMatrix_0(const MATRIX *m, const SVECTOR *v0, VECTOR *v1)
{
    return ApplyMatrix(m, v0, v1);
}

SVECTOR *ApplyMatrixSV_0(
    const MATRIX *m,
    const SVECTOR *v0,
    SVECTOR *v1
)
{
    return ApplyMatrixSV(m, v0, v1);
}

void Square0(const VECTOR *v0, VECTOR *v1)
{
    v1->vx = v0->vx * v0->vx;
    v1->vy = v0->vy * v0->vy;
    v1->vz = v0->vz * v0->vz;
}
