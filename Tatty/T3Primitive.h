#ifndef T3PRIMITIVE_H
#define T3PRIMITIVE_H

#include"T3Math.h"
#include"color.h"

//����洢����������
#define T3OBJECT_MAX_VERTICES 100
//����洢������������
#define T3OBJECT_MAX_POLYS 100

//�����״̬
//��Ծ�ɼ�
#define T3POLY_STATE_ACTIVE             0x0001
//���ü�
#define T3POLY_STATE_CLIPPED            0x0002
//���ڱ��� ����������任
#define T3POLY_STATE_BACKFACE           0x0004

//���嵱ǰ״̬
#define T3OBJECT_STATE_ACTIVE	0x0001
#define T3OBJECT_STATE_VISIBLE	0x0002
#define T3OBJECT_STATE_CULLED   0x0004

typedef struct T3PolyType
{
	int32 state;
	//�������ɫ
	Color color;
	//�����б�
	T3Point4DPtr vertexList;
	//��ǰ������������Ķ��������
	//��ǰ�������������
	int32 vertex[3];

}T3Poly, *T3PolyPtr;

//�ṩ����Ⱦ�б�ʹ�õĶ�������ݽṹ
typedef struct T3RenderPolyType
{
	//�������������ڵ�״̬
	int32 state;

	//�������ɫ
	Color color;

	//�����εĶ�������
	T3Point4D vertexList[3];
	//�洢�任�������������
	T3Point4D transVertexList[3];

	//����ṹ
	//ָ��ǰһ���ͺ�һ������ε�ָ��
	T3RenderPolyType *next;
	T3RenderPolyType *previous;

}T3RenderPoly, *T3RenderPolyPtr;

typedef struct T3ObejcteType
{
	//���������
	char name[10];
	//���嵱ǰ״̬
	int32 state;

	//���������������е�λ��
	T3Point4D worldPosition;
	//�ֲ������� ���ڴ洢����ĳ���
	//��������屻��תʱ�ֲ�����ϵҲ��ʱʱ����
	T3Vector4D ux, uy, uz;

	//��������������
	int32 numVertices;
	//�洢����ֲ�/ģ�����������
	T3Point4D vertexList_Local[T3OBJECT_MAX_VERTICES];
	//�洢ģ�ͱ任�������
	T3Point4D vertexList_Trans[T3OBJECT_MAX_VERTICES];

	//�����������εĸ���
	int32 numPolygons;
	//������ж���εĶ��������
	T3Poly polyList[T3OBJECT_MAX_POLYS];

	//ƽ���뾶
	float32 avgRadius;
	//���뾶
	float32 maxRadius;

}T3Object, *T3ObjectPtr;


//���������״̬
void T3ObjectReset(T3ObjectPtr obj, int32 numOfObj);

//���ö���ε�״̬
#endif