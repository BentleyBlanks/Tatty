#ifndef T3PIPELINE_H
#define T3PIPELINE_H

#include<stdlib.h>
#include"T3Math.h"
#include"T3Primitive.h"
#include"T3Camera.h"
#include"light.h"

#define Null 0
//��Ⱦ�б��������������Ŀ
#define T3RENDERLIST_MAX_POLY 65535


typedef struct T3RenderListType
{
	//��ǰ��Ⱦ�б��к��еĶ���θ���
	int32 numPolys;

	//����ε�ָ������
	T3RenderPolyPtr polyPtr[T3RENDERLIST_MAX_POLY];

	//���ڹ��ռ���Ķ�������ݸ���
	T3RenderPoly polyData[T3RENDERLIST_MAX_POLY];

}T3RenderList, *T3RenderListPtr;



//��ģ�ͱ���/�ֲ��������ת��
#define TRANSFORM_LOCAL 0
//�Ա任��Ķ����б���б任
#define TRANSFORM_TRANS 1
//��ģ��������б任�洢�ڱ任�б���
#define TRANSFORM_LOCAL_TO_TRANS 2


//��������Ĺ���ת��
//��任��Ⱦ�б�ͬ�����Ƿ�Ҫת��ģ�͵ĳ���
void Transform_T3Object(T3ObjectPtr obj, T3Mat4X4Ptr mt, int32 coordinateSelect, int32 transformOrientation); 

//����ľֲ����굽���������ת��
void ModelToWorld_T3Object(T3ObjectPtr obj, int32 coodinateSelect);

//�����������굽�������ı任
void WorldToCamera_T3Object(T3CameraPtr cam, T3ObjectPtr obj);

//�����������굽͸������
void CameraToPerspective_T3Object(T3ObjectPtr obj, T3CameraPtr cam);

//�����͸�����굽��Ļ�����
void PersepectiveToScreen_T3Object(T3ObjectPtr obj, T3CameraPtr cam);


//���� ����β�����Ⱦ�б���
//�����������Ⱦ�б�(���ö���β�����Ⱦ�б�)
void T3Object_InsertTo_T3RenderList(T3RenderListPtr rendList, T3ObjectPtr obj);

//������β�����Ⱦ�б�
int32 T3Poly_InsertTo_T3RenderList(T3RenderListPtr rendList, T3PolyPtr poly);

//��������
void RemoveBackfaces_T3RenderList(T3RenderListPtr rendList, T3CameraPtr cam);

//������Ⱦ�б���еĹ���ת��
//�ṩҪ�任������ ��Ⱦ�б��Լ��������ת��
void Transform_RenderList(T3RenderListPtr rendList, T3Mat4X4Ptr mt, int32 coordinateSelect);

//��Ⱦ�б�ľֲ����굽���������ת��
void ModelToWorld_T3RenderList(T3RenderListPtr rendList, T3Point4DPtr worldPosition, int32 coodinateSelect);

//��Ⱦ�б��������굽�������ı任
void WorldToCamera_T3RenderList(T3CameraPtr cam, T3RenderListPtr rend);

//��Ⱦ�б��������굽͸������
void CameraToPerspective_T3RenderList(T3RenderListPtr rendList, T3CameraPtr cam);

//�����͸�����굽��Ļ�����
void PersepectiveToScreen_T3RenderList(T3RenderListPtr rendList, T3CameraPtr cam);

//������Ⱦ�б�
void T3RenderListReset(T3RenderListPtr rendList);

//����Ⱦ�б���й��ռ���
//light��ʾΪ��Դ�б� ��Ϊ����ͬʱ���ڵ��Դ�۹�Ƶȹ�Դ����
void Light_T3RenderList(T3RenderListPtr rendList, T3CameraPtr cam, light *light, int32 numLight);

//���ڿ�������ıȽϺ���
int comparePolygonZ(const void* poly1, const void* poly2);

//�Ը�������Ⱦ�б��������
void SortRenderList(T3RenderListPtr rendList);
#endif