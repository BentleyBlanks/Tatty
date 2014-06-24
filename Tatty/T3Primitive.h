#ifndef T3PRIMITIVE_H
#define T3PRIMITIVE_H

#include"T3Math.h"
#include"color.h"

//物体存储顶点最大个数
#define T3OBJECT_MAX_VERTICES 100
//物体存储多边形最大数量
#define T3OBJECT_MAX_POLYS 100

//多边形状态
//活跃可见
#define T3POLY_STATE_ACTIVE             0x0001
//被裁剪
#define T3POLY_STATE_CLIPPED            0x0002
//处于背面 被消除无需变换
#define T3POLY_STATE_BACKFACE           0x0004

//物体当前状态
#define T3OBJECT_STATE_ACTIVE	0x0001
#define T3OBJECT_STATE_VISIBLE	0x0002
#define T3OBJECT_STATE_CULLED   0x0004

typedef struct T3PolyType
{
	int32 state;
	//多边形颜色
	Color color;
	//顶点列表
	T3Point4DPtr vertexList;
	//当前多边形所包含的顶点的索引
	//当前引擎采用三角形
	int32 vertex[3];

}T3Poly, *T3PolyPtr;

//提供给渲染列表使用的多边形数据结构
typedef struct T3RenderPolyType
{
	//该三角形所处于的状态
	int32 state;

	//多边形颜色
	Color color;

	//三角形的顶点坐标
	T3Point4D vertexList[3];
	//存储变换后的三角形坐标
	T3Point4D transVertexList[3];

	//链表结构
	//指向前一个和后一个多边形的指针
	T3RenderPolyType *next;
	T3RenderPolyType *previous;

}T3RenderPoly, *T3RenderPolyPtr;

typedef struct T3ObejcteType
{
	//物体的名字
	char name[10];
	//物体当前状态
	int32 state;

	//物体在世界坐标中的位置
	T3Point4D worldPosition;
	//局部坐标轴 用于存储物体的朝向
	//因此在物体被旋转时局部坐标系也会时时更新
	T3Vector4D ux, uy, uz;

	//物体包含顶点个数
	int32 numVertices;
	//存储物体局部/模型坐标的数组
	T3Point4D vertexList_Local[T3OBJECT_MAX_VERTICES];
	//存储模型变换后的坐标
	T3Point4D vertexList_Trans[T3OBJECT_MAX_VERTICES];

	//物体包含多边形的个数
	int32 numPolygons;
	//存放所有多边形的多边形数组
	T3Poly polyList[T3OBJECT_MAX_POLYS];

	//平均半径
	float32 avgRadius;
	//最大半径
	float32 maxRadius;

}T3Object, *T3ObjectPtr;


//重置物体的状态
void T3ObjectReset(T3ObjectPtr obj, int32 numOfObj);

//重置多边形的状态
#endif