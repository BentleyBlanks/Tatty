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
	double c[500];//存放模型所有参数
	int NumOfPoint;//存放顶点数
	T3POINT3 Point3[100];//存放顶点
	T3REVOLVEPOINT RPoint[100];//存放旋转后的顶点RevolvePoint
	T3POINT2 Point2[100];//存放最终相对顶点的二维坐标
	T3POINT2 DPoint[100];//存放某个多边形顶点二维坐标DrawPoint
};