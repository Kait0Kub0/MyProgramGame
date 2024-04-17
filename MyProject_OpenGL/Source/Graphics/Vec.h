//----------------------------------------------------------------------
//  Vector 演算
//----------------------------------------------------------------------
#pragma once

#include <math.h>

typedef struct VEC2 {
    union {
        struct {
            float   x, y;
        };
        struct {
            float   s, t;       // OpenGL=strq : GLSL=stpq
        };
		struct {
			float   u, v;       // DirectX=uv
		};

        float   V[2];
    };

	void set(float xx, float yy)
	{
		x = xx;
		y = yy;
	}

	VEC2 operator + (const VEC2& v) //オペレーターを使用して参照渡し
	{
		VEC2 vv;
		vv.x = x + v.x;
		vv.y = y + v.y;

		return vv;
	}

	VEC2 operator - (const VEC2& v) //オペレーターを使用して参照渡し
	{
		VEC2 vv;
		vv.x = x - v.x;
		vv.y = y - v.y;

		return vv;
	}


} VEC2;


typedef struct VEC3 {
    union {
        struct {
            float   x, y, z;
        };
        struct {
            float   r, g, b;
        };
        struct {
            float   s, t, p;     // OpenGL=strq : GLSL=stpq
        };
        float   V[3];
    };


	// デフォルトコンストラクタ
	//VEC3()
	//{
	//	x = 0;
	//	y = 0;
	//	z = 0;
	//}


	// コンストラクタ
	//VEC3(float xx, float yy, float zz)
	//{
	//	x = xx;
	//	y = yy;
	//	z = zz;
	//}

	void set(float xx, float yy, float zz)
	{
		x = xx;
		y = yy;
		z = zz;
	}

	VEC3 operator + (const VEC3 &v) //オペレーターを使用して参照渡し
	{
		VEC3 vv;
		vv.x = x + v.x;
		vv.y = y + v.y;
		vv.z = z + v.z;

		return vv;
	}

	VEC3 operator - (const VEC3 &v)
	{
		VEC3 vv;
		vv.x = x - v.x;
		vv.y = y - v.y;
		vv.z = z - v.z;

		return vv;
	}

	VEC3 operator * (const float &v)
	{
		VEC3 vv;
		vv.x = x * v;
		vv.y = y * v;
		vv.z = z * v;

		return vv;
	}

	// 単位ベクトル変換
	VEC3 normal()
	{
		//長さを求める
		float sc = scalar();
		if (sc == 0)
		{
			sc = 1;
		}

		//各軸を割る
		VEC3	v;
		v.x = x / sc;
		v.y = y / sc;
		v.z = z / sc;

		return v;
	}

	// 長さを求める
	float scalar()
	{
		float n;

		n = (x * x) + (y * y) + (z * z);
		n = sqrtf(n);

		return n;
	}


    // 内積計算
    float dotProduct(VEC3 p)
    {
        float n;

        n = x * p.x + y * p.y + z * p.z;

        return n;
    }


    // 外積計算
    VEC3 crossProduct(VEC3 p)
    {
        VEC3 v;

        v.x = y * p.z - z * p.y;
        v.y = z * p.x - x * p.z;
        v.z = x * p.y - y * p.x;

        return v;
    }



} VEC3;



typedef struct _VEC3{
    union {
        struct {
            float   x, y, z, w;
        };
        struct {
            float   r, g, b, a;
        };
        struct {
            float   s, t, p, q;     // OpenGL=strq : GLSL=stpq
        };
        float   V[4];

        struct {
            VEC3  v3;
            float dm;
        };

    };

	void set(float xx, float yy, float zz, float ww)
	{
		x = xx;
		y = yy;
		z = zz;
		w = ww;
	}

} VEC4;


typedef struct _VEC4UB {
    unsigned char   r, g, b, a;
} VEC4UB;

