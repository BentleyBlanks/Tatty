#ifndef T3Math_H
#define T3Math_H

#include <math.h>
#include <memory.h>

//规范C/C++基本数据类型描述方式
typedef signed short int16;
typedef signed int int32;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef float float32;
typedef double float64;


//非常小的常量 用于判断一定精度下的数字是否等于0
#define EPSILON4 (float32)(1E-4)
#define EPSILON5 (float32)(1E-5)
#define EPSILON6 (float32)(1E-6)

//PI常量
#define PI		   (float32)3.1415926535897932384626f
#define PI2        (float32))6.283185307f
#define PI_DIV_2   (float32)1.570796327f
#define PI_DIV_4   (float32))0.785398163f
#define PI_INV     (float32)0.318309886f

//取得最大最小值
#define T3Min(a, b)  (((a) < (b)) ? (a) : (b)) 
#define T3Max(a, b)  (((a) > (b)) ? (a) : (b)) 

//取绝对值
#define T3Abs(a) fabs(a)

//交换形参
#define T3Swap(a,b,temp) {temp=a; a=b; b=temp;}

//弧度角度转换
#define AngleToRadian(angle) ((angle)*PI/180.0)
#define RadianToAngle(radians) ((radians)*180.0/PI)

//随机数范围
#define RandRange(x,y) ((x) + (rand()%((y)-(x)+1)))

//设置位状态
#define SetBit(word,bit_flag)   ((word)=((word) | (bit_flag)))
#define ResetBit(word,bit_flag) ((word)=((word) & (~bit_flag)))

//弧度角度查找表 需要建立时调用T3BulidSinCosTablel来初始化正弦余弦查找表
//用于查找计算sin和cos值
extern float32 CosLook[361];
extern float32 SinLook[361];

//初始化查找表
void BuildSinCosTable();

//数据结构定义
//向量和点
//使用联合体来支持多种方式命名调用内容
typedef struct T3Vector2DType
{
	union
	{
		//数组存储方式
		float32 M[2];
		//独立变量组成结构体存储
		struct
		{
			float32 x, y;
		};
	};
}T3Vector2D, T3Point2D, *T3Vector2DPtr, *T3Point2DPtr;


typedef struct T3Vector3DType
{
	union
	{
		//数组存储方式
		float32 M[3];
		//独立变量组成结构体存储
		struct
		{
			float32 x, y, z;
		};
	};
}T3Vector3D, T3Point3D, *T3Vector3DPtr, *T3Point3DPtr;


typedef struct T3Vector4DType
{
	union
	{
		//数组存储方式
		float32 M[4];
		//独立变量组成结构体存储
		struct
		{
			//齐次变量w
			float32 x, y, z, w;
		};
	};
}T3Vector4D, *T3Vector4DPtr, T3Point4D, *T3Point4DPtr;

//矩阵
//1*2矩阵
typedef struct T3Mat1X2Type
{
	union
	{
		//数组存储方式
		float32 M[2];
		//独立变量组成结构体存储

		struct
		{
			float32 M00, M01;
			float32 M10, M11;
		};
	};
}T3Mat1X2, *T3Mat1X2Ptr;

//2*2矩阵
typedef struct T3Mat2X2Type
{
	union
	{
		//数组存储方式
		float32 M[2][2];
		//独立变量组成结构体存储

		struct
		{
			float32 M00, M01;
			float32 M10, M11;
		};
	};
}T3Mat2X2, *T3Mat2X2Ptr;

//1*3矩阵
typedef struct T3Mat1X3Type
{
	union
	{
		//数组存储方式
		float32 M[3];
		//独立变量组成结构体存储
		struct
		{
			float32 M00, M01, M02;
		};

	};
} T3Mat1X3, *T3Mat1X3Ptr;

//1*4矩阵
typedef struct T3Mat1X4Type
{
	union
	{
		//数组存储方式
		float32 M[4];
		//独立变量组成结构体存储
		struct
		{
			float32 M00, M01, M02, M03;
		};

	};
} T3Mat1X4, *T3Mat1X4Ptr;

//3*3矩阵
typedef struct T3Mat3X3Type
{
	union
	{
		//数组存储方式
		float32 M[3][3];
		//独立变量组成结构体存储

		struct
		{
			float32 M00, M01, M02;
			float32 M10, M11, M12;
			float32 M20, M21, M22;
		};
	};
}T3Mat3X3, *T3Mat3X3Ptr;

//4*3矩阵
typedef struct T3Mat4X3Type
{
	union
	{
		//数组存储方式
		float32 M[4][3];
		//独立变量组成结构体存储
		struct
		{
			float32 M00, M01, M02;
			float32 M10, M11, M12;
			float32 M20, M21, M22;
			float32 M30, M31, M32;
		};
	};
} T3Mat4X3, *T3Mat4X3Ptr;

//4*4矩阵
typedef struct T3Mat4X4Type
{
	union
	{
		//数组存储方式
		float32 M[4][4];
		//独立变量组成结构体存储
		struct
		{
			float32 M00, M01, M02, M03;
			float32 M10, M11, M12, M13;
			float32 M20, M21, M22, M23;
			float32 M30, M31, M32, M33;
		};
	};
} T3Mat4X4, *T3Mat4X4Ptr;

//单位矩阵
//4x4单位矩阵
const T3Mat4X4 T3IdentityMat4X4 = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1};

//4x3单位矩阵 尽管在数学上并不成立但是默认其第四列就是[0, 0, 0, 1]
const T3Mat4X3 T3IdentityMat4X3 = {
	1, 0, 0,
	0, 1, 0,
	0, 0, 1,
	0, 0, 0, };


//3x3单位矩阵
const T3Mat3X3 T3IdentityMat3X3 = {
	1, 0, 0,
	0, 1, 0,
	0, 0, 1};

//2x2单位矩阵
const T3Mat2X2 T3IdentityMat2X2 = {
	1, 0,
	0, 1};


//将给定矩阵置零
#define T3MatZero2X2(m) {memset((void *)(m), 0, sizeof(T3Mat2X2));}
#define T3MatZero3X3(m) {memset((void *)(m), 0, sizeof(T3Mat3X3));}
#define T3MatZero4X4(m) {memset((void *)(m), 0, sizeof(T3Mat4X4));}
#define T3MatZero4X3(m) {memset((void *)(m), 0, sizeof(T3Mat4X3));}

//将给定矩阵单位化
#define T3MatIdentity2X2(m) {memcpy((void *)(m), (void *)&T3IdentityMat2X2, sizeof(T3Mat2X2));}
#define T3MatIdentity3X3(m) {memcpy((void *)(m), (void *)&T3IdentityMat3X3, sizeof(T3Mat3X3));}
#define T3MatIdentity4X4(m) {memcpy((void *)(m), (void *)&T3IdentityMat4X4, sizeof(T3Mat4X4));}
#define T3MatIdentity4X3(m) {memcpy((void *)(m), (void *)&T3IdentityMat4X3, sizeof(T3Mat4X3));}

//拷贝源矩阵到目标矩阵中
#define T3MatCopy2X2(srcMat, destMat) {memcpy((void *)(destMat), (void *)(srcMat), sizeof(MATRIX2X2) ); }
#define T3MatCopy3X3(srcMat, destMat) {memcpy((void *)(destMat), (void *)(srcMat), sizeof(MATRIX3X3) ); }
#define T3MatCopy4X4(srcMat, destMat) {memcpy((void *)(destMat), (void *)(srcMat), sizeof(MATRIX4X4) ); }
#define T3MatCopy4X3(srcMat, destMat) {memcpy((void *)(destMat), (void *)(srcMat), sizeof(MATRIX4X3) ); }

//向量与矩阵交换指定列内容
inline void T3MatColunmSwap2X2(T3Mat2X2Ptr m, int32 c, T3Mat1X2Ptr v)
{
	m->M[0][c] = v->M[0]; m->M[1][c] = v->M[1];
}

inline void T3MatColunmSwap3X3(T3Mat3X3Ptr m, int c, T3Mat1X3Ptr v)
{
	m->M[0][c] = v->M[0]; m->M[1][c] = v->M[1]; m->M[2][c] = v->M[2];
}

inline void T3MatColunmSwap4X4(T3Mat4X4Ptr m, int c, T3Mat1X4Ptr v)
{
	m->M[0][c] = v->M[0]; m->M[1][c] = v->M[1]; m->M[2][c] = v->M[2]; m->M[3][c] = v->M[3];
}

inline void T3MatColunmSwap4X3(T3Mat4X3Ptr m, int c, T3Mat1X4Ptr v)
{
	m->M[0][c] = v->M[0]; m->M[1][c] = v->M[1]; m->M[2][c] = v->M[2]; m->M[3][c] = v->M[3];
}

//转置矩阵
inline void T3MatTranspos3X3(T3Mat3X3Ptr m)
{
	T3Mat3X3 mt;
	mt.M00 = m->M00; mt.M01 = m->M10; mt.M02 = m->M20;
	mt.M10 = m->M01; mt.M11 = m->M11; mt.M12 = m->M21;
	mt.M20 = m->M02; mt.M21 = m->M12; mt.M22 = m->M22;
	memcpy((void *) m, (void *) &mt, sizeof(T3Mat3X3));
}

inline void T3MatTranspos4X4(T3Mat4X4Ptr m)
{
	T3Mat4X4 mt;
	mt.M00 = m->M00; mt.M01 = m->M10; mt.M02 = m->M20; mt.M03 = m->M30;
	mt.M10 = m->M01; mt.M11 = m->M11; mt.M12 = m->M21; mt.M13 = m->M31;
	mt.M20 = m->M02; mt.M21 = m->M12; mt.M22 = m->M22; mt.M23 = m->M32;
	mt.M30 = m->M03; mt.M31 = m->M13; mt.M32 = m->M22; mt.M33 = m->M33;
	memcpy((void *) m, (void *) &mt, sizeof(T3Mat4X4));
}

inline void T3MatTranspos3X3(T3Mat3X3Ptr m, T3Mat3X3Ptr mt)
{
	mt->M00 = m->M00; mt->M01 = m->M10; mt->M02 = m->M20;
	mt->M10 = m->M01; mt->M11 = m->M11; mt->M12 = m->M21;
	mt->M20 = m->M02; mt->M21 = m->M12; mt->M22 = m->M22;
}

inline void T3MatTranspos4X4(T3Mat4X4Ptr m, T3Mat4X4Ptr mt)
{
	mt->M00 = m->M00; mt->M01 = m->M10; mt->M02 = m->M20; mt->M03 = m->M30;
	mt->M10 = m->M01; mt->M11 = m->M11; mt->M12 = m->M21; mt->M13 = m->M31;
	mt->M20 = m->M02; mt->M21 = m->M12; mt->M22 = m->M22; mt->M23 = m->M32;
	mt->M30 = m->M03; mt->M31 = m->M13; mt->M32 = m->M22; mt->M33 = m->M33;
}

//矩阵初始化
void T3MatInit2X2(T3Mat2X2Ptr ma, float32 m00, float32 m01, float32 m10, float32 m11);

void T3MatInit3X3(T3Mat3X3Ptr ma,
				  float32 m00, float32 m01, float32 m02,
				  float32 m10, float32 m11, float32 m12,
				  float32 m20, float32 m21, float32 m22);

void T3MatInit4X4(T3Mat4X4Ptr ma,
				  float32 m00, float32 m01, float32 m02, float32 m03,
				  float32 m10, float32 m11, float32 m12, float32 m13,
				  float32 m20, float32 m21, float32 m22, float32 m23,
				  float32 m30, float32 m31, float32 m32, float32 m33);

//求矩阵的逆
int32 T3MatInverse2X2(T3Mat2X2Ptr m, T3Mat2X2Ptr mi);
int32 T3MatInverse3X3(T3Mat3X3Ptr m, T3Mat3X3Ptr mi);
int32 T3MatInverse4X4(T3Mat4X4Ptr m, T3Mat4X4Ptr mi);

//矩阵基本运算
//加法
void T3MatAdd2X2(T3Mat2X2Ptr ma, T3Mat2X2Ptr mb, T3Mat2X2Ptr msum);
void T3MatAdd3X3(T3Mat3X3Ptr ma, T3Mat3X3Ptr mb, T3Mat3X3Ptr msum);
void T3MatAdd4X4(T3Mat4X4Ptr ma, T3Mat4X4Ptr mb, T3Mat4X4Ptr msum);

//乘法
void T3MatMul2X2(T3Mat2X2Ptr ma, T3Mat2X2Ptr mb, T3Mat2X2Ptr mprod);

void T3MatMulVECTOR3D_3X3(T3Vector3DPtr  va, T3Mat3X3Ptr mb, T3Vector3DPtr  vprod);

void T3MatMul4X4(T3Mat4X4Ptr ma, T3Mat4X4Ptr mb, T3Mat4X4Ptr mprod);
void T3MatMul4X4(T3Mat4X4Ptr ma, T3Mat4X4Ptr mb, T3Mat4X4Ptr mprod);
void T3MatMul1X4_4X4(T3Mat1X4Ptr ma, T3Mat4X4Ptr mb, T3Mat4X4Ptr mprod);

void T3Mat_T3Vector3D_Mul_4X4(T3Vector4DPtr  va, T3Mat4X4Ptr mb, T3Vector4DPtr  vprod);
void T3Mat_T3Vector3D_Mul_4X3(T3Vector4DPtr  va, T3Mat4X4Ptr mb, T3Vector4DPtr  vprod);

void T3Mat_T3Vector4D_Mul_4X4(T3Vector4DPtr  va,
							  T3Mat4X4Ptr mb,
							  T3Vector4DPtr  vprod);
//求矩阵的行列式
float32 T3MatDet2X2(T3Mat2X2Ptr m);
float32 T3MatDet3X3(T3Mat3X3Ptr m);



//点与向量涵义相同
//点初始化
inline void T3Point2DInit(T3Point2DPtr vdst, T3Point2DPtr vsrc)
{
	(vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;
}

inline void T3Point3DInit(T3Point3DPtr vdst, T3Point3DPtr vsrc)
{
	(vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;  (vdst)->z = (vsrc)->z;
}

inline void T3Point4DInit(T3Point4DPtr vdst, T3Point4DPtr vsrc)
{
	(vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;
	(vdst)->z = (vsrc)->z; (vdst)->w = (vsrc)->w;
}

//点拷贝
inline void T3Point2DCopy(T3Point2DPtr vdst, T3Point2DPtr vsrc)
{
	(vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;
}

inline void T3Point3DCopy(T3Point3DPtr vdst, T3Point3DPtr vsrc)
{
	(vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;  (vdst)->z = (vsrc)->z;
}

inline void T3Point4DCopy(T3Point4DPtr vdst, T3Point4DPtr vsrc)
{
	(vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;
	(vdst)->z = (vsrc)->z; (vdst)->w = (vsrc)->w;
}



//向量初始化
inline void T3Vector2DInit(T3Vector2DPtr vdst, T3Vector2DPtr vsrc)
{
	(vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;
}

inline void T3Vector3DInit(T3Vector3DPtr vdst, T3Vector3DPtr vsrc)
{
	(vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;  (vdst)->z = (vsrc)->z;
}

inline void T3Vector4DInit(T3Vector4DPtr vdst, T3Vector4DPtr vsrc)
{
	(vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;
	(vdst)->z = (vsrc)->z; (vdst)->w = (vsrc)->w;
}

//向量拷贝
inline void T3Vector2DCopy(T3Vector2DPtr vdst, T3Vector2DPtr vsrc)
{
	(vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;
}


inline void T3Vector3DCopy(T3Vector3DPtr vdst, T3Vector3DPtr vsrc)
{
	(vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;  (vdst)->z = (vsrc)->z;
}

inline void T3Vector4DCopy(T3Vector4DPtr vdst, T3Vector4DPtr vsrc)
{
	(vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;
	(vdst)->z = (vsrc)->z; (vdst)->w = (vsrc)->w;
}

//4D齐次向量转换为3D向量
inline void T3Vector4D_DIV_W(T3Vector4DPtr v)
{
	(v)->x /= (v)->w; (v)->y /= (v)->w; (v)->z /= (v)->w;
}

//4D齐次向量转换为非齐次4D
inline void T3Vector4D_DIV_W_To_T3Vector3D(T3Vector4DPtr v4, T3Vector4DPtr v3)
{
	(v3)->x = (v4)->x / (v4)->w; (v3)->y = (v4)->y / (v4)->w; (v3)->z = (v4)->z / (v4)->w;
}

//置零
inline void T3Vector2DZero(T3Vector2DPtr v)
{
	(v)->x = (v)->y = 0.0;
}

inline void T3Vector3DZero(T3Vector3DPtr v)
{
	(v)->x = (v)->y = (v)->z = 0.0;
}

inline void T3Vector4DZero(T3Vector4DPtr v)
{
	(v)->x = (v)->y = (v)->z = 0.0; (v)->w = 1.0;
}

//向量相加
void T3Vector2DAdd(T3Vector2DPtr va, T3Vector2DPtr vb, T3Vector2DPtr vsum);
T3Vector2D T3Vector2DAdd(T3Vector2DPtr va, T3Vector2DPtr vb);

void T3Vector3DAdd(T3Vector3DPtr va, T3Vector3DPtr vb, T3Vector3DPtr vsum);
T3Vector3D T3Vector3DAdd(T3Vector3DPtr va, T3Vector3DPtr vb);

void T3Vector4DAdd(T3Vector4DPtr va, T3Vector4DPtr vb, T3Vector4DPtr vsum);
T3Vector4D T3Vector4DAdd(T3Vector4DPtr va, T3Vector4DPtr vb);

//向量相减
void T3Vector2DSub(T3Vector2DPtr va, T3Vector2DPtr vb, T3Vector2DPtr vdiff);
T3Vector2D T3Vector2DSub(T3Vector2DPtr va, T3Vector2DPtr vb);

void T3Vector3DSub(T3Vector3DPtr va, T3Vector3DPtr vb, T3Vector3DPtr vdiff);
T3Vector3D T3Vector3DSub(T3Vector3DPtr va, T3Vector3DPtr vb);

void T3Vector4DSub(T3Vector4DPtr va, T3Vector4DPtr vb, T3Vector4DPtr vdiff);
T3Vector4D T3Vector4DSub(T3Vector4DPtr va, T3Vector4DPtr vb);

//缩放(因子) 向量与标量相乘积
void T3Vector2DScale(float32 k, T3Vector2DPtr va);
void T3Vector2DScale(float32 k, T3Vector2DPtr va, T3Vector2DPtr vscaled);

void T3Vector3DScale(float32 k, T3Vector3DPtr va);
void T3Vector3DScale(float32 k, T3Vector3DPtr va, T3Vector3DPtr vscaled);

void T3Vector4DScale(float32 k, T3Vector4DPtr va);
void T3Vector4DScale(float32 k, T3Vector4DPtr va, T3Vector4DPtr vscaled);

//返回向量长度
float32 T3Vector2DLength(T3Vector2DPtr va);
float32 T3Vector2DLengthFast(T3Vector2DPtr va);

float32 T3Vector3DLength(T3Vector3DPtr va);
float32 T3Vector3DLengthFast(T3Vector3DPtr va);

float32 T3Vector4DLength(T3Vector4DPtr va);
float32 T3Vector4DLengthFast(T3Vector4DPtr va);

//向量标准化(单位化)
void T3Vector2DNormalize(T3Vector2DPtr va);
void T3Vector2DNormalize(T3Vector2DPtr va, T3Vector2DPtr vn);

void T3Vector3DNormalize(T3Vector3DPtr va);
void T3Vector3DNormalize(T3Vector3DPtr va, T3Vector3DPtr vn);

void T3Vector4DNormalize(T3Vector4DPtr va);
void T3Vector4DNormalize(T3Vector4DPtr va, T3Vector4DPtr vn);

//向量点积
float32 T3Vector2DDot(T3Vector2DPtr va, T3Vector2DPtr vb);
float32 T3Vector3DDot(T3Vector3DPtr va, T3Vector3DPtr vb);
float32 T3Vector4DDot(T3Vector4DPtr va, T3Vector4DPtr vb);

//向量叉积
//2D向量没有实质性意义上的叉积
void T3Vector3DCross(T3Vector3DPtr va, T3Vector3DPtr vb, T3Vector3DPtr vn);
T3Vector3D T3Vector3DCross(T3Vector3DPtr va, T3Vector3DPtr vb);

void T3Vector4DCross(T3Vector4DPtr va, T3Vector4DPtr vb, T3Vector4DPtr vn);
T3Vector4D T3Vector4DCross(T3Vector4DPtr va, T3Vector4DPtr vb);

//计算两向量夹角
float32 T3Vector2DCosTheta(T3Vector2DPtr va, T3Vector2DPtr vb);
float32 T3Vector3DCosTheta(T3Vector3DPtr va, T3Vector3DPtr vb);
float32 T3Vector4DCosTheta(T3Vector4DPtr va, T3Vector4DPtr vb);

//使用两个向量来创建一个向量
void T3Vector2DBuild(T3Vector2DPtr init, T3Vector2DPtr term, T3Vector2DPtr result);
void T3Vector3DBuild(T3Vector3DPtr init, T3Vector3DPtr term, T3Vector3DPtr result);
void T3Vector4DBuild(T3Vector4DPtr init, T3Vector4DPtr term, T3Vector4DPtr result);

//快速计算正弦余弦
float32 FastSin(float32 theta);
float32 FastCos(float32 theta);

//快速计算平方根
int FastSqrt2D(int x, int y);
float FastSqrt3D(float fx, float fy, float fz);
#endif