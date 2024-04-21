/* date = April 16th 2024 10:18 am */

#ifndef VECTORS_H
#define VECTORS_H

struct Vector2D
{
    float x, y;
    
    Vector2D() = default;
    
    Vector2D(float a, float b)
    {
        x = a;
        y = b;
    };
    
    float & operator[] (int i)
    {
        return ((&x) [i]);
    };
    
    const float & operator [] (int i) const 
    {
        return ((&x) [i]);
    }
    
    Vector2D & operator += (const Vector2D &v)
    {
        x += v.x;
        y += v.y;
        return (*this);
    }
    
    Vector2D & operator -= (const Vector2D &v)
    {
        x -= v.x;
        y -= v.y;
        return (*this);
    }
    
    Vector2D & operator *= (float s)
    {
        x *= s;
        y *= s;
        return (*this);
    }
    
    Vector2D & operator /= (float s)
    {
        s = 1.0F / s;
        x *= s;
        y *= s;
        return (*this);
    }
    
};

struct Vector3D
{
    float x, y, z;
    
    Vector3D() = default;
    
    Vector3D(float a, float b, float c)
    {
        x = a;
        y = b;
        z = c;
    };
    
    
    float & operator[] (int i)
    {
        return ((&x) [i]);
    };
    
    const float & operator [] (int i) const 
    {
        return ((&x) [i]);
    }
    
    
    Vector3D & operator += (const Vector3D &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return (*this);
    }
    
    Vector3D & operator -= (const Vector3D &v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return (*this);
    }
    
    Vector3D & operator *= (float s)
    {
        x *= s;
        y *= s;
        z *= s;
        
        return (*this);
    }
    
    Vector3D & operator /= (float s)
    {
        s = 1.0F / s;
        x *= s;
        y *= s;
        z *= s;
        return (*this);
    }
};

struct Vector4D
{
    float x, y, z, w;
    
    Vector4D() = default;
    
    Vector4D(float a, float b, float c, float d)
    {
        x = a;
        y = b;
        z = c;
        w = d;
    };
    
    
    
    float & operator[] (int i)
    {
        return ((&x) [i]);
    };
    
    const float & operator [] (int i) const 
    {
        return ((&x) [i]);
    }
    
    Vector4D & operator += (const Vector4D &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
        return (*this);
    }
    
    Vector4D & operator -= (const Vector4D &v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
        return (*this);
    }
    
    Vector4D & operator *= (float s)
    {
        x *= s;
        y *= s;
        z *= s;
        w *= s;
        return (*this);
    }
    
    Vector4D & operator /= (float s)
    {
        s = 1.0F / s;
        x *= s;
        y *= s;
        z *= s;
        w *= s;
        return (*this);
    }
};



#endif //VECTORS_H
