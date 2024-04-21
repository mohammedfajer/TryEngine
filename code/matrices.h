/* date = April 16th 2024 0:02 pm */

#ifndef MATRICES_H
#define MATRICES_H


struct Matrix2D
{
    private:
    float n[2][2];

    public:
    Matrix2D() = default;

    Matrix2D(float n00, float n01,
            float n10, float n11)
    {
        n[0][0] = n00; n[0][1] = n10;
        n[1][0] = n01; n[1][1] = n11;
    }

    Matrix2D(const Vector2D &a, const Vector2D &b)
    {
        n[0][0] = a.x; n[0][1] = a.y;
        n[1][0] = b.x; n[1][1] = b.y;

    };

    float &operator () (int i, int j)
    {
        return (n[j][i]);
    }

    const float &operator () (int i, int j) const
    {
        return (n[j][i]);
    }


    Vector2D & operator[](int j)
    {
        return (*reinterpret_cast<Vector2D *>(n[j]));
    }

    const Vector2D &operator[] (int j) const
    {
        return (*reinterpret_cast<const Vector2D *>(n[j]));
    }
};


struct Matrix3D
{
    private:
    float n[3][3];

    public:
    Matrix3D() = default;

    Matrix3D(float n00, float n01, float n02,
             float n10, float n11, float n12,
             float n20, float n21, float n22)
    {
        n[0][0] = n00; n[0][1] = n10; n[0][2] = n20;
        n[1][0] = n01; n[1][1] = n11; n[1][2] = n21;
        n[2][0] = n02; n[2][1] = n12; n[2][2] = n22;
    }

    Matrix3D(const Vector3D &a, const Vector3D &b, const Vector3D &c)
    {
        n[0][0] = a.x; n[0][1] = a.y; n[0][2] = a.z;
        n[1][0] = b.x; n[1][1] = b.y; n[1][2] = b.z;
        n[2][0] = c.x; n[2][1] = c.y; n[2][2] = c.z;
    };

    float &operator () (int i, int j)
    {
        return (n[j][i]);
    }

    const float &operator () (int i, int j) const
    {
        return (n[j][i]);
    }


    Vector3D & operator[](int j)
    {
        return (*reinterpret_cast<Vector3D *>(n[j]));
    }

    const Vector3D &operator[] (int j) const
    {
        return (*reinterpret_cast<const Vector3D *>(n[j]));
    }
};

// TODO(mo): Do for 4d matrices...

struct Matrix4D
{
    private:
    float n[4][4];

    public:
    Matrix4D() = default;

    Matrix4D(float n00, float n01, float n02, float n03,
             float n10, float n11, float n12, float n13,
             float n20, float n21, float n22, float n23,
             float n30, float n31, float n32, float n33)
    {
        n[0][0] = n00; n[0][1] = n10; n[0][2] = n20; n[0][3] = n30;
        n[1][0] = n01; n[1][1] = n11; n[1][2] = n21; n[1][3] = n31;
        n[2][0] = n02; n[2][1] = n12; n[2][2] = n22; n[2][3] = n32;
        n[3][0] = n03; n[3][1] = n13; n[3][2] = n23; n[3][3] = n33;
    }

    Matrix4D(const Vector4D &a, const Vector4D &b, const Vector4D &c, const Vector4D &d)
    {
        n[0][0] = a.x; n[0][1] = a.y; n[0][2] = a.z; n[0][3] = a.w;
        n[1][0] = b.x; n[1][1] = b.y; n[1][2] = b.z; n[1][3] = b.w;
        n[2][0] = c.x; n[2][1] = c.y; n[2][2] = c.z; n[2][3] = c.w;
        n[3][0] = d.x; n[3][1] = d.y; n[3][2] = d.z; n[3][3] = d.w;
    };

    float &operator () (int i, int j)
    {
        return (n[j][i]);
    }

    const float &operator () (int i, int j) const
    {
        return (n[j][i]);
    }


    Vector4D & operator[](int j)
    {
        return (*reinterpret_cast<Vector4D *>(n[j]));
    }

    const Vector4D &operator[] (int j) const
    {
        return (*reinterpret_cast<const Vector4D *>(n[j]));
    }
};

#endif //MATRICES_H
