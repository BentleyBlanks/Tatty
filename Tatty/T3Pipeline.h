#ifndef T3PIPELINE_H
#define T3PIPELINE_H

#include<stdlib.h>
#include"T3Math.h"
#include"T3Primitive.h"
#include"T3Camera.h"
#include"light.h"

#define Null 0
//渲染列表最大包含多边形数目
#define T3RENDERLIST_MAX_POLY 65535


typedef struct T3RenderListType
{
	//当前渲染列表中含有的多边形个数
	int32 numPolys;

	//多边形的指针数组
	T3RenderPolyPtr polyPtr[T3RENDERLIST_MAX_POLY];

	//用于光照计算的多边形数据副本
	T3RenderPoly polyData[T3RENDERLIST_MAX_POLY];

}T3RenderList, *T3RenderListPtr;



//对模型本身/局部坐标进行转换
#define TRANSFORM_LOCAL 0
//对变换后的顶点列表进行变换
#define TRANSFORM_TRANS 1
//对模型坐标进行变换存储在变换列表内
#define TRANSFORM_LOCAL_TO_TRANS 2


//基于物体的管线转换
//与变换渲染列表不同在于是否要转换模型的朝向
void Transform_T3Object(T3ObjectPtr obj, T3Mat4X4Ptr mt, int32 coordinateSelect, int32 transformOrientation); 

//物体的局部坐标到世界坐标的转换
void ModelToWorld_T3Object(T3ObjectPtr obj, int32 coodinateSelect);

//物体世界坐标到相机坐标的变换
void WorldToCamera_T3Object(T3CameraPtr cam, T3ObjectPtr obj);

//物体的相机坐标到透视坐标
void CameraToPerspective_T3Object(T3ObjectPtr obj, T3CameraPtr cam);

//物体的透视坐标到屏幕坐标的
void PersepectiveToScreen_T3Object(T3ObjectPtr obj, T3CameraPtr cam);


//物体 多边形插入渲染列表中
//将物体插入渲染列表(调用多边形插入渲染列表)
void T3Object_InsertTo_T3RenderList(T3RenderListPtr rendList, T3ObjectPtr obj);

//将多边形插入渲染列表
int32 T3Poly_InsertTo_T3RenderList(T3RenderListPtr rendList, T3PolyPtr poly);

//背面消除
void RemoveBackfaces_T3RenderList(T3RenderListPtr rendList, T3CameraPtr cam);

//基于渲染列表进行的管线转换
//提供要变换的坐标 渲染列表以及矩阵进行转换
void Transform_RenderList(T3RenderListPtr rendList, T3Mat4X4Ptr mt, int32 coordinateSelect);

//渲染列表的局部坐标到世界坐标的转换
void ModelToWorld_T3RenderList(T3RenderListPtr rendList, T3Point4DPtr worldPosition, int32 coodinateSelect);

//渲染列表世界坐标到相机坐标的变换
void WorldToCamera_T3RenderList(T3CameraPtr cam, T3RenderListPtr rend);

//渲染列表的相机坐标到透视坐标
void CameraToPerspective_T3RenderList(T3RenderListPtr rendList, T3CameraPtr cam);

//物体的透视坐标到屏幕坐标的
void PersepectiveToScreen_T3RenderList(T3RenderListPtr rendList, T3CameraPtr cam);

//重置渲染列表
void T3RenderListReset(T3RenderListPtr rendList);

//对渲染列表进行光照计算
//light表示为光源列表 因为可能同时存在点光源聚光灯等光源类型
void Light_T3RenderList(T3RenderListPtr rendList, T3CameraPtr cam, light *light, int32 numLight);

//用于快速排序的比较函数
int comparePolygonZ(const void* poly1, const void* poly2);

//对给定的渲染列表进行排序
void SortRenderList(T3RenderListPtr rendList);
#endif