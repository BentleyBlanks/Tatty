#include"T3Math.h"

//一个为存放角度正弦余弦的查找表
float32 CosLook[361];
float32 SinLook[361];

//矩阵初始化
void T3MatInit2X2(T3Mat2X2Ptr ma, float32 m00, float32 m01, float32 m10, float32 m11)
{
	//使用给定参数初始化矩阵
	ma->M00 = m00;	ma->M01 = m01;
	ma->M10 = m10;	ma->M11 = m11;
}


void T3MatInit3X3(T3Mat3X3Ptr ma,
				  float32 m00, float32 m01, float32 m02,
				  float32 m10, float32 m11, float32 m12,
				  float32 m20, float32 m21, float32 m22)
{
	//使用给定参数初始化矩阵
	ma->M00 = m00; ma->M01 = m01; ma->M02 = m02;
	ma->M10 = m10; ma->M11 = m11; ma->M12 = m12;
	ma->M20 = m20; ma->M21 = m21; ma->M22 = m22;
}

void T3MatInit4X4(T3Mat4X4Ptr ma,
				  float32 m00, float32 m01, float32 m02, float32 m03,
				  float32 m10, float32 m11, float32 m12, float32 m13,
				  float32 m20, float32 m21, float32 m22, float32 m23,
				  float32 m30, float32 m31, float32 m32, float32 m33)
{
	//使用给定参数初始化矩阵
	ma->M00 = m00; ma->M01 = m01; ma->M02 = m02; ma->M03 = m03;
	ma->M10 = m10; ma->M11 = m11; ma->M12 = m12; ma->M13 = m13;
	ma->M20 = m20; ma->M21 = m21; ma->M22 = m22; ma->M23 = m23;
	ma->M30 = m30; ma->M31 = m31; ma->M32 = m32; ma->M33 = m33;
}

//求矩阵的逆
//先判断A是否为奇异矩阵 因为只有非奇异矩阵才存在逆矩阵(|奇异矩阵| = 0)
//逆矩阵 = 1/|A|·A*(矩阵的代数余子式)
int32 T3MatInverse2X2(T3Mat2X2Ptr m, T3Mat2X2Ptr mi)
{
	//二阶方阵的代数余子式 
	//|m11 m10|
	//|m01 m00|

	//首先判断是否为奇异矩阵
	float32 det = m->M00 * m->M11 - m->M01 * m->M10;

	//精度低的物体进行的判断与0的关系
	if(T3Abs(det) < EPSILON5)
		return 0;

	float32 detInverse = 1 / det;
	//为非奇异矩阵
	mi->M00 = detInverse * m->M11;
	mi->M01 = detInverse * m->M10;
	mi->M10 = detInverse * m->M01;
	mi->M11 = detInverse * m->M00;

	//计算成功
	return 1;
}

int32 T3MatInverse3X3(T3Mat3X3Ptr m, T3Mat3X3Ptr mi)
{
	//三阶方阵的代数余子式 
	//|+|m11 m12|-|m01 m02|+|m01 m02| |
	//|	|m21 m22| |m21 m22| |m11 m12| |

	//|-|m10 m12|+|m00 m02|-|m00 m02| |
	//|	|m20 m22| |m20 m22| |m10 m12| |

	//|+|m10 m11|-|m00 m01|+|m00 m01| |
	//|	|m20 m21| |m20 m21| |m10 m11| |

	//首先判断是否为奇异矩阵
	float32 det = (m->M00 * m->M11 * m->M22 + m->M01 * m->M12 * m->M20 + m->M10 * m->M21 *m->M02)
		- (m->M20 * m->M11 * m->M02 + m->M10 * m->M01 * m->M22 + m->M21 * m->M12 * m->M00);

	//精度低的物体进行的判断与0的关系
	if(T3Abs(det) < EPSILON5)
		return 0;

	float32 detInverse = 1 / det;

	//为非奇异矩阵
	mi->M00 = detInverse*(m->M11*m->M22 - m->M21*m->M12);
	mi->M01 = -detInverse*(m->M01*m->M22 - m->M21*m->M02);
	mi->M02 = detInverse*(m->M01*m->M12 - m->M11*m->M02);

	mi->M10 = -detInverse*(m->M10*m->M22 - m->M20*m->M12);
	mi->M11 = detInverse*(m->M00*m->M22 - m->M20*m->M02);
	mi->M12 = -detInverse*(m->M00*m->M12 - m->M10*m->M02);

	mi->M20 = detInverse*(m->M10*m->M21 - m->M20*m->M11);
	mi->M21 = -detInverse*(m->M00*m->M21 - m->M20*m->M01);
	mi->M22 = detInverse*(m->M00*m->M11 - m->M10*m->M01);

	//计算成功
	return 1;
}
int32 T3MatInverse4X4(T3Mat4X4Ptr m, T3Mat4X4Ptr mi)
{
	//四阶方阵的最后一列始终默认为[0, 0, 0, 1]t
	//其代数余子式太麻烦……就先不画了

	//首先判断是否为奇异矩阵
	//根据最后一列的1按照代数余子式展开
	//(-1)4 = 1因此为正号
	float32 det = (m->M00 * m->M11 * m->M22 + m->M01 * m->M12 * m->M20 + m->M10 * m->M21 *m->M02)
		- (m->M20 * m->M11 * m->M02 + m->M10 * m->M01 * m->M22 + m->M21 * m->M12 * m->M00);

	//精度低的物体进行的判断与0的关系
	if(T3Abs(det) < EPSILON5)
		return 0;

	float32 detInverse = 1 / det;

	//为非奇异矩阵
	mi->M00 = detInverse * (m->M11 * m->M22 - m->M12 * m->M21);
	mi->M01 = -detInverse * (m->M01 * m->M22 - m->M02 * m->M21);
	mi->M02 = detInverse * (m->M01 * m->M12 - m->M02 * m->M11);
	mi->M03 = 0.0f;

	mi->M10 = -detInverse * (m->M10 * m->M22 - m->M12 * m->M20);
	mi->M11 = detInverse * (m->M00 * m->M22 - m->M02 * m->M20);
	mi->M12 = -detInverse * (m->M00 * m->M12 - m->M02 * m->M10);
	mi->M13 = 0.0f;

	mi->M20 = detInverse * (m->M10 * m->M21 - m->M11 * m->M20);
	mi->M21 = -detInverse * (m->M00 * m->M21 - m->M01 * m->M20);
	mi->M22 = detInverse * (m->M00 * m->M11 - m->M01 * m->M10);
	mi->M23 = 0.0f;

	mi->M30 = -((m->M00*m->M21*m->M32 + m->M01*m->M22*m->M32 + m->M20*m->M31*m->M02) -
				((m->M30*m->M21*m->M02 + m->M20*m->M01*m->M32 + m->M31*m->M22*m->M00)));
	mi->M31 = (m->M00*m->M21*m->M02 + m->M01*m->M22*m->M30 + m->M20*m->M31*m->M02) -
		(m->M30*m->M21*m->M02 + m->M20*m->M01*m->M32 + m->M31*m->M22*m->M00);
	mi->M32 = -((m->M00*mi->M32*m->M11 + m->M10*mi->M31*m->M02 + m->M01*mi->M12*m->M30) -
				(m->M30*m->M11*m->M02 + m->M10*m->M01*m->M32 + m->M31*m->M12*m->M00));
	mi->M33 = 1.0f;

	//计算成功
	return 1;
}

//矩阵基本运算
//加法
void T3MatAdd2X2(T3Mat2X2Ptr ma, T3Mat2X2Ptr mb, T3Mat2X2Ptr msum)
{
	//对应元素相加
	msum->M00 = ma->M00 + mb->M00;
	msum->M01 = ma->M01 + mb->M01;
	msum->M10 = ma->M10 + mb->M10;
	msum->M11 = ma->M11 + mb->M11;
}

void T3MatAdd3X3(T3Mat3X3Ptr ma, T3Mat3X3Ptr mb, T3Mat3X3Ptr msum)
{
	//使用手工效率更高 但是仅在需要的时候再手工书写出来
	for(int32 row = 0; row<3; row++)
	for(int32 column = 0; column<3; column++)
		ma->M[row][column] = ma->M[row][column] + mb->M[row][column];
}

void T3MatAdd4X4(T3Mat4X4Ptr ma, T3Mat4X4Ptr mb, T3Mat4X4Ptr msum)
{
	for(int32 row = 0; row<4; row++)
	for(int32 column = 0; column<4; column++)
		ma->M[row][column] = ma->M[row][column] + mb->M[row][column];
}

//矩阵乘法
void T3MatMul2X2(T3Mat2X2Ptr ma, T3Mat2X2Ptr mb, T3Mat2X2Ptr mprod)
{
	//相乘的结果并没有改变ab矩阵本身而是存储在了production矩阵里
	mprod->M00 = ma->M00*mb->M00 + ma->M01*mb->M10;
	mprod->M01 = ma->M00*mb->M01 + ma->M01*mb->M11;

	mprod->M10 = ma->M10*mb->M00 + ma->M11*mb->M10;
	mprod->M11 = ma->M10*mb->M01 + ma->M11*mb->M11;
}

void T3MatMulVECTOR3D_3X3(T3Vector3DPtr  va, T3Mat3X3Ptr mb, T3Vector3DPtr  vprod)
{
	int32 sum;
	for(int32 column = 0; column <3; column++)
	{
		//将和置空
		sum = 0;
		for(int32 row = 0; row<3; row++)
			sum += va->M[row] * mb->M[row][column];

		//将结果保存在production内
		vprod->M[column] = sum;
	}
}

void T3MatMul4X4(T3Mat4X4Ptr ma, T3Mat4X4Ptr mb, T3Mat4X4Ptr mprod)
{
	//行
	for(int32 row = 0; row<4; row++)
	{
		//列
		for(int32 index = 0; index<4; index++)
		{
			float32 sum = 0;
			for(int32 column = 0; column<4; column++)
			{
				sum += (ma->M[row][column] * mb->M[column][index]);
			}

			//保存结果
			mprod->M[row][index] = sum;
		}
	}
}

void T3MatMul1X4_4X4(T3Mat1X4Ptr ma, T3Mat4X4Ptr mb, T3Mat1X4Ptr mprod)
{
	int32 sum;
	for(int32 row = 0; row<4; row++)
	{
		sum = 0;
		for(int32 column = 0; column<4; column++)
		{
			sum += ma->M[column] * mb->M[row][column];
		}

		mprod->M[row] = sum;
	}
}

void T3Mat_T3Vector3D_Mul_4X4(T3Vector4DPtr  va, T3Mat4X4Ptr mb, T3Vector4DPtr  vprod)
{
	int32 sum;
	//此处假定3D向量为4D 也就是最后一列的数据是1 [a, b, c, 1]
	for(int32 column = 0; column<3; column++)
	{
		sum = 0;
		for(int32 row = 0; row<3; row++)
		{
			sum += va->M[column] * mb->M[row][column];
		}
		//3D向量的假定最后一元为1与4X4的某一列相乘之后即第四行第某列的值本身
		sum += mb->M[4][column];

		vprod->M[column] = sum;
	}
}

void T3Mat_T3Vector3D_Mul_4X3(T3Vector4DPtr  va, T3Mat4X4Ptr mb, T3Vector4DPtr  vprod)
{
	//我估计我暂时用不到这个变态的理想函数吧……
}

void T3Mat_T3Vector4D_Mul_4X4(T3Vector4DPtr  va,
							  T3Mat4X4Ptr mb,
							  T3Vector4DPtr  vprod)
{
	for(int32 row = 0; row<4; row++)
	{
		float32 sum = 0;
		for(int32 column = 0; column<4; column++)
		{
			sum += (va->M[column] * mb->M[column][row]);
		}

		vprod->M[row] = sum;
	}
}

//求矩阵的行列式
float32 T3MatDet2X2(T3Mat2X2Ptr m)
{
	return(m->M00*m->M11 - m->M01*m->M10);
}

float32 T3MatDet3X3(T3Mat3X3Ptr m)
{
	return (m->M00 * m->M11 * m->M22 + m->M01 * m->M12 * m->M20 + m->M10 * m->M21 *m->M02)
		- (m->M20 * m->M11 * m->M02 + m->M10 * m->M01 * m->M22 + m->M21 * m->M12 * m->M00);
}

//向量相加
void T3Vector2DAdd(T3Vector2DPtr va, T3Vector2DPtr vb, T3Vector2DPtr vsum)
{
	vsum->x = va->x + vb->x;
	vsum->y = va->y + vb->y;
}

T3Vector2D T3Vector2DAdd(T3Vector2DPtr va, T3Vector2DPtr vb)
{
	T3Vector2D vsum;

	vsum.x = va->x + vb->x;
	vsum.y = va->y + vb->y;

	return(vsum);
}

void T3Vector3DAdd(T3Vector3DPtr va, T3Vector3DPtr vb, T3Vector3DPtr vsum)
{
	vsum->x = va->x + vb->x;
	vsum->y = va->y + vb->y;
	vsum->z = va->z + vb->z;
}

T3Vector3D T3Vector3DAdd(T3Vector3DPtr va, T3Vector3DPtr vb)
{
	T3Vector3D vsum;

	vsum.x = va->x + vb->x;
	vsum.y = va->y + vb->y;
	vsum.z = va->z + vb->z;

	return(vsum);
}

void T3Vector4DAdd(T3Vector4DPtr va, T3Vector4DPtr vb, T3Vector4DPtr vsum)
{
	vsum->x = va->x + vb->x;
	vsum->y = va->y + vb->y;
	vsum->z = va->z + vb->z;
	//4D的齐次向量最后一位默认为1 因此相加后仍然是1
	vsum->w = 1.0f;
}

T3Vector4D T3Vector4DAdd(T3Vector4DPtr va, T3Vector4DPtr vb)
{
	T3Vector4D vsum;

	vsum.x = va->x + vb->x;
	vsum.y = va->y + vb->y;
	vsum.z = va->z + vb->z;
	vsum.w = 1.0f;

	return(vsum);
}

//向量相减
void T3Vector2DSub(T3Vector2DPtr va, T3Vector2DPtr vb, T3Vector2DPtr vdiff)
{
	vdiff->x = va->x - vb->x;
	vdiff->y = va->y - vb->y;
}

T3Vector2D T3Vector2DSub(T3Vector2DPtr va, T3Vector2DPtr vb)
{
	T3Vector2D vdiff;

	vdiff.x = va->x - vb->x;
	vdiff.y = va->y - vb->y;

	// return result
	return(vdiff);
}

void T3Vector3DSub(T3Vector3DPtr va, T3Vector3DPtr vb, T3Vector3DPtr vdiff)
{
	vdiff->x = va->x - vb->x;
	vdiff->y = va->y - vb->y;
	vdiff->z = va->z - vb->z;
}

T3Vector3D T3Vector3DSub(T3Vector3DPtr va, T3Vector3DPtr vb)
{
	T3Vector3D vdiff;

	vdiff.x = va->x - vb->x;
	vdiff.y = va->y - vb->y;
	vdiff.z = va->z - vb->z;

	return(vdiff);
}

void T3Vector4DSub(T3Vector4DPtr va, T3Vector4DPtr vb, T3Vector4DPtr vdiff)
{
	vdiff->x = va->x - vb->x;
	vdiff->y = va->y - vb->y;
	vdiff->z = va->z - vb->z;
	vdiff->w = 1;
}

T3Vector4D T3Vector4DSub(T3Vector4DPtr va, T3Vector4DPtr vb)
{
	T3Vector4D vdiff;

	vdiff.x = va->x - vb->x;
	vdiff.y = va->y - vb->y;
	vdiff.z = va->z - vb->z;
	vdiff.w = 1;

	// return result
	return(vdiff);
}

//缩放(因子) 向量与标量相乘积
void T3Vector2DScale(float32 k, T3Vector2DPtr va)
{
	va->x *= k;
	va->y *= k;
}

void T3Vector2DScale(float32 k, T3Vector2DPtr va, T3Vector2DPtr vscaled)
{
	vscaled->x = k*va->x;
	vscaled->y = k*va->y;
}

void T3Vector3DScale(float32 k, T3Vector3DPtr va)
{
	va->x *= k;
	va->y *= k;
	va->z *= k;
}

void T3Vector3DScale(float32 k, T3Vector3DPtr va, T3Vector3DPtr vscaled)
{
	vscaled->x = k*va->x;
	vscaled->y = k*va->y;
	vscaled->z = k*va->z;
}

void T3Vector4DScale(float32 k, T3Vector4DPtr va)
{
	va->x *= k;
	va->y *= k;
	va->z *= k;
	//最后一位只是为了齐次
	va->w = 1;
}

void T3Vector4DScale(float32 k, T3Vector4DPtr va, T3Vector4DPtr vscaled)
{
	vscaled->x = k*va->x;
	vscaled->y = k*va->y;
	vscaled->z = k*va->z;
	vscaled->w = 1;
}

//返回向量长度
float32 T3Vector2DLength(T3Vector2DPtr va)
{
	return(sqrtf(va->x*va->x + va->y*va->y));
}

float32 T3Vector2DLengthFast(T3Vector2DPtr va)
{
	return((float) FastSqrt2D(va->x, va->y));
}

float32 T3Vector3DLength(T3Vector3DPtr va)
{
	return((float) sqrtf(va->x*va->x + va->y*va->y + va->z*va->z));
}

float32 T3Vector3DLengthFast(T3Vector3DPtr va)
{
	return(FastSqrt3D(va->x, va->y, va->z));
}

float32 T3Vector4DLength(T3Vector4DPtr va)
{
	return(sqrtf(va->x*va->x + va->y*va->y + va->z*va->z));
}

float32 T3Vector4DLengthFast(T3Vector4DPtr va)
{
	//最后的齐次量1可以忽略 便成为误差
	return(FastSqrt3D(va->x, va->y, va->z));
}

//向量标准化(单位化)
//对形参本身进行转换
void T3Vector2DNormalize(T3Vector2DPtr va)
{
	float length = sqrtf(va->x*va->x + va->y*va->y);

	//计算长度判断与0关系
	if(length < EPSILON5)
		return;

	float length_inv = 1 / length;

	//标准化
	va->x = va->x*length_inv;
	va->y = va->y*length_inv;
}

//讲标准化的结果保存在另一个向量中
void T3Vector2DNormalize(T3Vector2DPtr va, T3Vector2DPtr vn)
{
	T3Vector2DZero(vn);

	//计算长度
	float length = (float) sqrtf(va->x*va->x + va->y*va->y);

	if(length < EPSILON5)
		return;

	float length_inv = 1 / length;

	vn->x = va->x*length_inv;
	vn->y = va->y*length_inv;
}

void T3Vector3DNormalize(T3Vector3DPtr va)
{
	float length = sqrtf(va->x*va->x + va->y*va->y + va->z*va->z);

	if(length < EPSILON5)
		return;

	float length_inv = 1 / length;

	va->x *= length_inv;
	va->y *= length_inv;
	va->z *= length_inv;
}

void T3Vector3DNormalize(T3Vector3DPtr va, T3Vector3DPtr vn)
{
	T3Vector3DZero(vn);

	float length = T3Vector3DLength(va);

	if(length < EPSILON5)
		return;

	float length_inv = 1.0 / length;

	vn->x = va->x*length_inv;
	vn->y = va->y*length_inv;
	vn->z = va->z*length_inv;
}

void T3Vector4DNormalize(T3Vector4DPtr va)
{
	float length = sqrtf(va->x*va->x + va->y*va->y + va->z*va->z);

	if(length < EPSILON5)
		return;

	float length_inv = 1.0 / length;

	va->x *= length_inv;
	va->y *= length_inv;
	va->z *= length_inv;
	va->w = 1;
}

void T3Vector4DNormalize(T3Vector4DPtr va, T3Vector4DPtr vn)
{
	T3Vector4DZero(vn);

	float length = sqrt(va->x*va->x + va->y*va->y + va->z*va->z);

	if(length < EPSILON5)
		return;

	float length_inv = 1.0 / length;

	vn->x = va->x*length_inv;
	vn->y = va->y*length_inv;
	vn->z = va->z*length_inv;
	vn->w = 1;
}

//向量点积
float32 T3Vector2DDot(T3Vector2DPtr va, T3Vector2DPtr vb)
{
	return((va->x * vb->x) + (va->y * vb->y));
}

float32 T3Vector3DDot(T3Vector3DPtr va, T3Vector3DPtr vb)
{
	return((va->x * vb->x) + (va->y * vb->y) + (va->z * vb->z));
}

float32 T3Vector4DDot(T3Vector4DPtr va, T3Vector4DPtr vb)
{
	//最后的其次坐标不参与计算
	return((va->x * vb->x) + (va->y * vb->y) + (va->z * vb->z));
}

//向量叉积
//2D向量没有实质性意义上的叉积
//叉积的结果经常用下图记忆
//|i j k|
//|a b c|
//|x y z|
void T3Vector3DCross(T3Vector3DPtr va, T3Vector3DPtr vb, T3Vector3DPtr vn)
{
	vn->x = va->y*vb->z - va->z*vb->y;
	vn->y = va->z*vb->x - va->x*vb->z;
	vn->z = va->x*vb->y - va->y*vb->x;
}

T3Vector3D T3Vector3DCross(T3Vector3DPtr va, T3Vector3DPtr vb)
{
	T3Vector3D vn;

	vn.x = va->y*vb->z - va->z*vb->y;
	vn.y = va->z*vb->x - va->x*vb->z;
	vn.z = va->x*vb->y - va->y*vb->x;

	return vn;
}

void T3Vector4DCross(T3Vector4DPtr va, T3Vector4DPtr vb, T3Vector4DPtr vn)
{
	//与3D的叉积唯一不同的就是保留齐次项
	vn->x = va->y*vb->z - va->z*vb->y;
	vn->y = va->z*vb->x - va->x*vb->z;
	vn->z = va->x*vb->y - va->y*vb->x;
	vn->w = 1;
}

T3Vector4D T3Vector4DCross(T3Vector4DPtr va, T3Vector4DPtr vb)
{
	T3Vector4D vn;

	vn.x = va->y*vb->z - va->z*vb->y;
	vn.y = va->z*vb->x - va->x*vb->z;
	vn.z = va->x*vb->y - va->y*vb->x;
	vn.w = 1;

	return vn;
}

//计算两向量夹角
//这里返回的是夹角的余弦 因为并不需要真正的求出来角度的大小 那样效率过低
float32 T3Vector2DCosTheta(T3Vector2DPtr va, T3Vector2DPtr vb)
{
	return (T3Vector2DDot(va, vb) / (T3Vector2DLength(va)*T3Vector2DLength(vb)));
}

float32 T3Vector3DCosTheta(T3Vector3DPtr va, T3Vector3DPtr vb)
{
	return (T3Vector3DDot(va, vb) / (T3Vector3DLength(va)*T3Vector3DLength(vb)));
}

float32 T3Vector4DCosTheta(T3Vector4DPtr va, T3Vector4DPtr vb)
{
	return (T3Vector4DDot(va, vb) / (T3Vector4DLength(va)*T3Vector4DLength(vb)));
}

//使用两个向量来创建一个向量
void T3Vector2DBuild(T3Vector2DPtr init, T3Vector2DPtr term, T3Vector2DPtr result)
{
	//向量的差
	result->x = term->x - init->x;
	result->y = term->y - init->y;
}

void T3Vector3DBuild(T3Vector3DPtr init, T3Vector3DPtr term, T3Vector3DPtr result)
{
	result->x = term->x - init->x;
	result->y = term->y - init->y;
	result->z = term->z - init->z;
}

void T3Vector4DBuild(T3Vector4DPtr init, T3Vector4DPtr term, T3Vector4DPtr result)
{
	result->x = term->x - init->x;
	result->y = term->y - init->y;
	result->z = term->z - init->z;
	result->w = 1;
}

void BuildSinCosTable()
{

	for(int ang = 0; ang <= 360; ang++)
	{
		//将角度转化为弧度
		float theta = (float) ang*PI / (float) 180;

		//插入值到表中
		CosLook[ang] = cos(theta);
		SinLook[ang] = sin(theta);
	}
}

//快速计算正弦余弦
float32 FastSin(float32 theta)
{
	//将角度转换到0-360度
	theta = fmodf(theta, 360);

	//将负角度转正
	if(theta < 0) theta += 360.0;

	//角度整数部分
	int32 thetaInt = (int32) theta;
	//角度小数部分
	float32 thetaFrac = theta - thetaInt;

	//一度之间正弦差值*小数部分近似求出小数部分对应正弦值
	return(SinLook[thetaInt] +
		   thetaFrac*(SinLook[thetaInt + 1] - SinLook[thetaInt]));
}

float32 FastCos(float32 theta)
{
	//将角度转换到0-360度
	theta = fmodf(theta, 360);

	//将负角度转正
	if(theta < 0) 
		theta += 360.0;

	//角度整数部分
	int32 thetaInt = (int32) theta;
	//角度小数部分
	float32 thetaFrac = theta - thetaInt;

	//一度之间余弦差值*小数部分近似求出小数部分对应余弦值
	return(CosLook[thetaInt] +
		   thetaFrac*(CosLook[thetaInt + 1] - CosLook[thetaInt]));

}

//快速计算平方根
//---借鉴---
int FastSqrt2D(int x, int y)
{
	x = abs(x);
	y = abs(y);

	// compute the minimum of x,y
	int mn = T3Min(x, y);

	// return the distance
	return(x + y - (mn >> 1) - (mn >> 2) + (mn >> 4));
}

float FastSqrt3D(float fx, float fy, float fz)
{
	int temp;  // used for swaping
	int x, y, z; // used for algorithm

	// make sure values are all positive
	x = fabs(fx) * 1024;
	y = fabs(fy) * 1024;
	z = fabs(fz) * 1024;

	// sort values
	if(y < x) T3Swap(x, y, temp)

	if(z < y) T3Swap(y, z, temp)

	if(y < x) T3Swap(x, y, temp)

		int dist = (z + 11 * (y >> 5) + (x >> 2));

	// compute distance with 8% error
	return((float) (dist >> 10));
}

