#include "vectors.h"

/// V2
inline Vector2D operator +(const Vector2D &lhs, const Vector2D &rhs)
{
    return (Vector2D(lhs.x + rhs.x, lhs.y + rhs.y));
}

inline Vector2D operator-(const Vector2D &lhs, const Vector2D &rhs)
{
    return (Vector2D(lhs.x - rhs.x, lhs.y - rhs.y));
}

inline Vector2D operator *(const Vector2D & v, float s)
{
    return (Vector2D(v.x * s, v.y *s));
}

inline Vector2D operator /(const Vector2D & v, float s)
{
    s = 1.0F / s;
    return (Vector2D(v.x * s, v.y * s));
}

inline Vector2D operator -(const Vector2D & v)
{
    return (Vector2D(-v.x, -v.y));
}

inline float Magnitude(const Vector2D &v)
{
    return (sqrt(v.x * v.x + v.y * v.y));
}

inline Vector2D Normalize(const Vector2D &v)
{
    return (v / Magnitude(v));
}


inline float Dot(const Vector2D & a, const Vector2D & b)
{
    return (a.x * b.x + a.y * b.y);
}


/// V3
inline Vector3D operator +(const Vector3D &lhs, const Vector3D &rhs)
{
    return (Vector3D(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z));
}

inline Vector3D operator-(const Vector3D &lhs, const Vector3D &rhs)
{
    return (Vector3D(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z));
}

inline Vector3D operator *(const Vector3D & v, float s)
{
    return (Vector3D(v.x * s, v.y *s, v.z * s));
}

inline Vector3D operator /(const Vector3D & v, float s)
{
    s = 1.0F / s;
    return (Vector3D(v.x * s, v.y * s, v.z * s));
}

inline Vector3D operator -(const Vector3D & v)
{
    return (Vector3D(-v.x, -v.y, -v.z));
}

inline float Magnitude(const Vector3D &v)
{
    return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

inline Vector3D Normalize(const Vector3D &v)
{
    return (v / Magnitude(v));
}

inline float Dot(const Vector3D & a, const Vector3D & b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

inline Vector3D Cross(const Vector3D &a, const Vector3D &b)
{
    return (Vector3D(a.y * b.z - a.z * b.y,
                     a.z * b.x - a.x * b.z,
                     a.x * b.y - a.y * b.x));
}

inline Vector3D Project(const Vector3D &a, const Vector3D &b)
{
    return (b * (Dot(a, b) / Dot(b,b)));
}

inline Vector3D Reject(const Vector3D &a, const Vector3D &b)
{
    return (a - b * (Dot(a,b) / Dot(b,b)));
}



/// V4 FVector

inline Vector4D operator +(const Vector4D &lhs, const Vector4D &rhs)
{
    return (Vector4D(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w));
}

inline Vector4D operator-(const Vector4D &lhs, const Vector4D &rhs)
{
    return (Vector4D(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w));
}

inline Vector4D operator *(const Vector4D & v, float s)
{
    return (Vector4D(v.x * s, v.y *s, v.z * s, v.w * s));
}

inline Vector4D operator /(const Vector4D & v, float s)
{
    s = 1.0F / s;
    return (Vector4D(v.x * s, v.y * s, v.z * s, v.w * s));
}

inline Vector4D operator -(const Vector4D & v)
{
    return (Vector4D(-v.x, -v.y, -v.z, -v.w));
}

inline float Magnitude(const Vector4D &v)
{
    return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w));
}

inline Vector4D Normalize(const Vector4D &v)
{
    return (v / Magnitude(v));
}

inline float Dot(const Vector4D & a, const Vector4D & b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}


internal void
test_vector2()
{
    Vector2D spritePosition;
    spritePosition.x = 100;
    spritePosition.y = 200;
}

internal void
test_vector3()
{
    // Position Vector
    Vector3D position(1.3f, 2.0f, 4.0f);

    const float x = position[0];
    Vector3D normalizedPos = Normalize(position);

}

internal void
test_vector4()
{
    // Position Vector
    Vector4D position(1.3f, 2.0f, 4.0f, 1.0f);

    const float x = position[0];
    const float y = position[1];
    const float z = position[2];
    const float w = position[3];
}
