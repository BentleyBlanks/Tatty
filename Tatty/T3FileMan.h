#define T3POINT2  POINT
#include "T3Primitive.h"
struct T3POINT3;
struct T3REVOLVEPOINT;
struct T3PLGMODEL;
int T3Load0(T3PLGMODEL &, char *);
int T3Load(T3Object &, char *);
int T3LoadMap(T3Object **LoadModel, char * cmf, int & mid);
T3Object* T3LoadMap(char * cmf, int & mid);
int T3Draw(HDC, HWND, T3PLGMODEL &, COLORREF);
int T3Revolve(HDC, HWND, T3PLGMODEL &, POINT, COLORREF);
int T3Refresh(HDC, HWND);
int T3ErrorLog(int, char *);

struct T3POINT3
{
	double x;
	double y;
	double z;
};

struct T3REVOLVEPOINT
{
	double x;
	double y;
	double z;
	double r;
	double theta;
};

struct T3PLGMODEL
{
	double c[500];//���ģ�����в���
	int NumOfPoint;//��Ŷ�����
	T3POINT3 Point3[100];//��Ŷ���
	T3REVOLVEPOINT RPoint[100];//�����ת��Ķ���RevolvePoint
	T3POINT2 Point2[100];//���������Զ���Ķ�ά����
	T3POINT2 DPoint[100];//���ĳ������ζ����ά����DrawPoint
};