#ifndef T3CAMERA_H
#define T3CAMERA_H

#include"T3Math.h"
#include"T3Primitive.h"

//ŷ�������ת˳���
#define CAM_ROT_XYZ 0
#define CAM_ROT_YXZ 1
#define CAM_ROT_XZY 2
#define CAM_ROT_YZX 3
#define CAM_ROT_ZYX 4
#define CAM_ROT_ZXY 5

//�����޳���
//�������Ҳü�������޳�
#define CULL_T3OBJECT_X  0x0001 
//���²ü���
#define CULL_T3OBJECT_Y  0x0002
//����Զ���ü���
#define CULL_T3OBJECT_Z  0x0004
//ͬʱ�޳�
#define CULL_T3OBJECT_XYZ  (CULL_T3OBJECT_X|CULL_T3OBJECT_Y|CULL_T3OBJECT_Z)


#define DIRECTION_FRONT 0
#define DIRECTION_BACK 1


typedef struct T3CameraType
{
	//���״̬
	int32 state;
	//�����
	int32 attribute;

	//���λ��
	T3Point4D position;
	//�������Ƕ�
	T3Vector4D direction;

    //���ע��
	T3Vector4D target;
	//UVN��������
	T3Vector4D u, v, n;

	//�Ӿ�
	float32 viewDistance;
	//ˮƽ�Ӿ�
	float32 viewDistanceHeorizontal;
	//��ֱ�Ӿ�
	float32 viewDistanceVertical;

	//��Ұ
	//�򵥰汾��Ĭ������Ϊ90��
	float32 FOV;

	//UVN���

	//�ü���
	//Զ���ü���
	float32 farClipZ;
	float32 nearClipZ;

	//��ƽ����
	float32 viewPlaneWidth;
	float32 viewPlaneHeight;

	//�ӿ�/��Ļ�Ŀ��
	float32 viewPortWidth;
	float32 viewPortHeight;

	//�ӿ����ĵ�λ��/����
	float32 viewPortCenterX;
	float32 viewPortCenterY;

	//��Ļ��߱�
	float32 aspectRadio;

	//��������ת�����������ľ���
	T3Mat4X4 mCam;
	//������굽͸������
	T3Mat4X4 mPer;
	//͸�����굽��Ļ����
	T3Mat4X4 mScr;

}T3Camera, *T3CameraPtr;

//�����ʼ��
void T3CameraInit(T3CameraPtr cam, T3Point4DPtr pos, T3Vector4DPtr dir,
				  float32 FOV,
				  float32 farClipZ, float32 nearClipZ,
				  float32 viewPortWidth, float32 viewPortHeight);

//��������Ƕȱ任 ���������������
void updateCameraAxis(T3CameraPtr cam);

//���ݷ���������λ��
void updateCameraPosition(T3CameraPtr cam, int32 direction);

//��������任����
void T3CameraBuildMat(T3CameraPtr cam, int32 rotationOrder);

//�����޳�
int32 T3CameraCull_T3Object(T3ObjectPtr obj, T3CameraPtr cam, int32 cullFlag);

#endif