#ifndef T3CAMERA_H
#define T3CAMERA_H

#include"T3Math.h"
#include"T3Primitive.h"

//欧拉相机旋转顺序宏
#define CAM_ROT_XYZ 0
#define CAM_ROT_YXZ 1
#define CAM_ROT_XZY 2
#define CAM_ROT_YZX 3
#define CAM_ROT_ZYX 4
#define CAM_ROT_ZXY 5

//物体剔除宏
//根据左右裁剪面进行剔除
#define CULL_T3OBJECT_X  0x0001 
//上下裁剪面
#define CULL_T3OBJECT_Y  0x0002
//根据远近裁剪面
#define CULL_T3OBJECT_Z  0x0004
//同时剔除
#define CULL_T3OBJECT_XYZ  (CULL_T3OBJECT_X|CULL_T3OBJECT_Y|CULL_T3OBJECT_Z)


#define DIRECTION_FRONT 0
#define DIRECTION_BACK 1


typedef struct T3CameraType
{
	//相机状态
	int32 state;
	//项技属性
	int32 attribute;

	//相机位置
	T3Point4D position;
	//相机朝向角度
	T3Vector4D direction;

    //相机注视
	T3Vector4D target;
	//UVN三个分量
	T3Vector4D u, v, n;

	//视距
	float32 viewDistance;
	//水平视距
	float32 viewDistanceHeorizontal;
	//垂直视距
	float32 viewDistanceVertical;

	//视野
	//简单版本中默认设置为90度
	float32 FOV;

	//UVN相机

	//裁剪面
	//远近裁剪面
	float32 farClipZ;
	float32 nearClipZ;

	//视平面宽高
	float32 viewPlaneWidth;
	float32 viewPlaneHeight;

	//视口/屏幕的宽高
	float32 viewPortWidth;
	float32 viewPortHeight;

	//视口中心的位置/坐标
	float32 viewPortCenterX;
	float32 viewPortCenterY;

	//屏幕宽高比
	float32 aspectRadio;

	//世界坐标转换到相机坐标的矩阵
	T3Mat4X4 mCam;
	//相机坐标到透视坐标
	T3Mat4X4 mPer;
	//透视坐标到屏幕坐标
	T3Mat4X4 mScr;

}T3Camera, *T3CameraPtr;

//相机初始化
void T3CameraInit(T3CameraPtr cam, T3Point4DPtr pos, T3Vector4DPtr dir,
				  float32 FOV,
				  float32 farClipZ, float32 nearClipZ,
				  float32 viewPortWidth, float32 viewPortHeight);

//根据三轴角度变换 更新相机三坐标轴
void updateCameraAxis(T3CameraPtr cam);

//根据方向更新相机位置
void updateCameraPosition(T3CameraPtr cam, int32 direction);

//创建相机变换矩阵
void T3CameraBuildMat(T3CameraPtr cam, int32 rotationOrder);

//物体剔除
int32 T3CameraCull_T3Object(T3ObjectPtr obj, T3CameraPtr cam, int32 cullFlag);

#endif