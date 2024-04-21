#include "matrices.h"

// TODO (mo) : do for 2d and 4d matrices..

float Determinant(const Matrix2D &M)
{
  return ( M(0,0) * M(1,1) - M(0,1) * M(1,0) ) ;
}

Matrix3D operator *(const Matrix3D &A, const Matrix3D &B)
{
    return (Matrix3D(A(0,0) * B(0,0) + A(0, 1) * B(1, 0) + A(0,2) * B(2,0),
                     A(0,0) * B(0,1) + A(0, 1) * B(1, 1) + A(0,2) * B(2,1),
                     A(0,0) * B(0,2) + A(0, 1) * B(1, 2) + A(0,2) * B(2,2),

                     A(1, 0) * B(0,0) + A(1, 1) * B(1, 0) + A(1,2) * B(2, 0),
                     A(1, 0) * B(0,1) + A(1, 1) * B(1, 1) + A(1,2) * B(2, 1),
                     A(1, 0) * B(0,2) + A(1, 1) * B(1, 2) + A(1,2) * B(2, 2),

                     A(2, 0) * B(0,0) + A(2, 1) * B(1, 0) + A(2,2) * B(2, 0),
                     A(2, 0) * B(0,1) + A(2, 1) * B(1, 1) + A(2,2) * B(2, 1),
                     A(2, 0) * B(0,2) + A(2, 1) * B(1, 2) + A(2,2) * B(2, 2)
                     ));
}

Vector3D operator *(const Matrix3D &M, const Vector3D& v)
{
    return (Vector3D(M(0,0) * v.x + M(0,1) * v.y + M(0,2) * v.z,
                     M(1,0) * v.x + M(1,1) * v.z + M(1,2) * v.z,
                     M(2,0) * v.x + M(2,1) * v.z + M(2,2) * v.z));
}


float Determinant(const Matrix3D &M)
{
  return (M(0,0) * (M(1,1) * M(2,2) - M(1,2) * M(2,1))
        + M(0,1) * (M(1,2) * M(2,0) - M(1,0) * M(2,2))
        + M(0,2) * (M(1,0) * M(2,1) - M(1,1) * M(2,0)));
}

Matrix3D Inverse(const Matrix3D &M)
{
  const Vector3D &a = M[0];
  const Vector3D &b = M[1];
  const Vector3D &c = M[2];

  Vector3D r0 = Cross(b, c);
  Vector3D r1 = Cross(c, a);
  Vector3D r2 = Cross(a, b);

  float invDet = 1.0F / Dot(r2, c);

  return (Matrix3D(r0.x * invDet, r0.y * invDet, r0.z * invDet,
                   r1.x * invDet, r1.y * invDet, r1.z * invDet,
                   r2.x * invDet, r2.y * invDet, r2.z * invDet));
}


Matrix4D Inverse(const Matrix4D &M)
{
  const Vector3D &a = reinterpret_cast<const Vector3D &>(M[0]);
  const Vector3D &b = reinterpret_cast<const Vector3D &>(M[1]);
  const Vector3D &c = reinterpret_cast<const Vector3D &>(M[2]);
  const Vector3D &d = reinterpret_cast<const Vector3D &>(M[3]);

  const float &x = M(3,0);
  const float &y = M(3,1);
  const float &z = M(3,2);
  const float &w = M(3,3);

  Vector3D s = Cross(a, b);
  Vector3D t = Cross(c, d);
  Vector3D u = a * y - b * x;
  Vector3D v = c * w - d * z;

  float invDet = 1.0F / (Dot(s, v) + Dot(t, u));
  s *= invDet;
  t *= invDet;
  u *= invDet;
  v *= invDet;

  Vector3D r0 = Cross(b, v) + t * y;
  Vector3D r1 = Cross(v, a) - t * x;
  Vector3D r2 = Cross(d, u) + s * w;
  Vector3D r3 = Cross(u, c) - s * z;

  return (Matrix4D(r0.x, r0.y, r0.z, -Dot(b, t),
                   r1.x, r1.y, r1.z, Dot(a, t),
                   r2.x, r2.y, r2.z, -Dot(d, s),
                   r3.x, r3.y, r3.z, Dot(c, s) ));

}