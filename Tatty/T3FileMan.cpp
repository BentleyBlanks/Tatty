#define _CRT_SECURE_NO_WARNINGS
#include "windows.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "T3FileMan.h"
#include "T3Graphics.h"
#include "T3Math.h"
#include "T3Primitive.h"
#define FLAGPOLY 0.3f
#define FLAGNEXT -0.3f
#define FLAGEND -0.33f

POINT T3O;//存放中心坐标
int FlagAntiAliasing = 1;//标识是否抗锯齿

T3Object* T3LoadMap(char * cmf, int & mid)
{
	T3Object *LoadModel;

	//地址、文件名、文件指针、参数序号
	FILE * pmf;
	char cdir[20] = "./map/";
	strcat(cdir, cmf);
	int errnum = 0;
	if(errnum = (fopen_s(&pmf, cdir, "r")))
	{
		T3ErrorLog(errnum, cdir);
		MessageBox(NULL, TEXT("ERROR IN OPEN FILE"), TEXT("ERROR"), 0);
		exit(0);
	}

	//读取多边形个数
	int numObject;
	fscanf(pmf, "%d", &numObject);
	//动态开辟内存
	LoadModel = (T3Object *) malloc(numObject*sizeof(T3Object));

	//读取模型参数...手牵手一个两个三个四个读出来
	int num = 0;
	float32 temp = 0.0;
	mid = 0;
	while(temp != FLAGEND)
	{
		num = 0;

		fscanf(pmf, "%s", &LoadModel[mid].name);
		fscanf(pmf, "%f %f %f %f\n",
			   &LoadModel[mid].worldPosition.x,
			   &LoadModel[mid].worldPosition.y,
			   &LoadModel[mid].worldPosition.z,
			   &LoadModel[mid].worldPosition.w);
		while(fscanf(pmf, "%f", &temp) && temp != FLAGPOLY)
		{
			LoadModel[mid].vertexList_Local[num].x = temp;
			fscanf(pmf, "%f %f\n",
				   &(LoadModel[mid].vertexList_Local[num].y),
				   &(LoadModel[mid].vertexList_Local[num].z));
			num++;
			if(num>T3OBJECT_MAX_VERTICES)
				MessageBox(NULL, TEXT("ERROR IN FILE"), TEXT("ERROR"), 0);
		}
		LoadModel[mid].numVertices = num;
		num = 0;
		int index = 0;
		while(temp == FLAGPOLY)
		{
			fscanf(pmf, "%d", &index);
			if(index == 3)
			{
				fscanf(pmf, "%d %d %d\n",
					   &LoadModel[mid].polyList[num].vertex[0],
					   &LoadModel[mid].polyList[num].vertex[1],
					   &LoadModel[mid].polyList[num].vertex[2]);
				num++;
			}
			else//nto3
			{
				fscanf(pmf, "%d %d %d",
					   &LoadModel[mid].polyList[num].vertex[0],
					   &LoadModel[mid].polyList[num].vertex[1],
					   &LoadModel[mid].polyList[num].vertex[2]);
				num++;
				for(int i = 0; i<index - 3; i++)
				{
					LoadModel[mid].polyList[num].vertex[0] = LoadModel[mid].polyList[num - 1].vertex[0];
					LoadModel[mid].polyList[num].vertex[1] = LoadModel[mid].polyList[num - 1].vertex[2];
					fscanf(pmf, "%d", &LoadModel[mid].polyList[num].vertex[2]);
					num++;
				}
			}
			if(num>T3OBJECT_MAX_POLYS)
				MessageBox(NULL, TEXT("ERROR IN FILE"), TEXT("ERROR"), 0);
			fscanf(pmf, "%f", &temp);
			if(temp == FLAGNEXT) break;
		}
		LoadModel[mid].numPolygons = num;
		float32 sumr = 0, tempr = 0;
		LoadModel[mid].maxRadius = 0;
		for(int i = 0; i<LoadModel[mid].numVertices; i++)
		{
			tempr = sqrt(pow(LoadModel[mid].vertexList_Local[i].x, 2) +
						 pow(LoadModel[mid].vertexList_Local[i].y, 2) +
						 pow(LoadModel[mid].vertexList_Local[i].z, 2));
			if(tempr>LoadModel[mid].maxRadius) 
				LoadModel[mid].maxRadius = tempr;
			sumr += tempr;
		}
		LoadModel[mid].avgRadius = sumr / LoadModel[mid].numVertices;
		mid++;
	}
	fclose(pmf);

	//为场景中的物体进行状态初始化
	for(int32 numObj = 0; numObj<mid; numObj++)
	{
		LoadModel[numObj].state = T3OBJECT_STATE_ACTIVE | T3OBJECT_STATE_VISIBLE;
		for(int i = 0; i<LoadModel[numObj].numPolygons; i++)
		{
			LoadModel[numObj].polyList[i].state = T3POLY_STATE_ACTIVE;

			//默认材质为灰色
			Color color(100.0, 100.0, 100.0);
			LoadModel[numObj].polyList[i].color = color;
		}
	}

	return LoadModel;
}


int T3LoadMap(T3Object **LoadModel, char * cmf, int & mid)
{
	//地址、文件名、文件指针、参数序号
	FILE * pmf;
	char cdir[20] = "./map/";
	strcat(cdir, cmf);
	int errnum = 0;
	if(errnum = (fopen_s(&pmf, cdir, "r")))
	{
		T3ErrorLog(errnum, cdir);
		MessageBox(NULL, TEXT("ERROR IN OPEN FILE"), TEXT("ERROR"), 0);
		exit(0);
	}

	//读取多边形个数
	int numObject;
	fscanf(pmf, "%d", &numObject);
	//动态开辟内存
	(*LoadModel) = (T3Object *) malloc(numObject*sizeof(T3Object));

	//读取模型参数...手牵手一个两个三个四个读出来
	int num = 0;
	float32 temp = 0.0;
	mid = 0;
	while(temp != FLAGEND)
	{
		num = 0;

		fscanf(pmf, "%s", (*LoadModel)[mid].name);
		fscanf(pmf, "%f %f %f %f\n",
			   (*LoadModel)[mid].worldPosition.x,
			   (*LoadModel)[mid].worldPosition.y,
			   (*LoadModel)[mid].worldPosition.z,
			   (*LoadModel)[mid].worldPosition.w);
		while(fscanf(pmf, "%f", &temp) && temp != FLAGPOLY)
		{
			(*LoadModel)[mid].vertexList_Local[num].x = temp;
			fscanf(pmf, "%f %f\n",
				   (*LoadModel)[mid].vertexList_Local[num].y,
				   (*LoadModel)[mid].vertexList_Local[num].z);
			num++;
			if(num>T3OBJECT_MAX_VERTICES)
				MessageBox(NULL, TEXT("ERROR IN FILE"), TEXT("ERROR"), 0);
		}
		(*LoadModel)[mid].numVertices = num;
		num = 0;
		int index = 0;
		while(temp == FLAGPOLY)
		{
			fscanf(pmf, "%d", &index);
			if(index == 3)
			{
				fscanf(pmf, "%d %d %d\n",
					   (*LoadModel)[mid].polyList[num].vertex[0],
					   (*LoadModel)[mid].polyList[num].vertex[1],
					   (*LoadModel)[mid].polyList[num].vertex[2]);
				num++;
			}
			else//nto3
			{
				fscanf(pmf, "%d %d %d",
					   (*LoadModel)[mid].polyList[num].vertex[0],
					   (*LoadModel)[mid].polyList[num].vertex[1],
					   (*LoadModel)[mid].polyList[num].vertex[2]);
				num++;
				for(int i = 0; i<index - 3; i++)
				{
					(*LoadModel)[mid].polyList[num].vertex[0] = (*LoadModel)[mid].polyList[num - 1].vertex[0];
					(*LoadModel)[mid].polyList[num].vertex[1] = (*LoadModel)[mid].polyList[num - 1].vertex[2];
					fscanf(pmf, "%d", (*LoadModel)[mid].polyList[num].vertex[2]);
					num++;
				}
			}
			if(num>T3OBJECT_MAX_POLYS)
				MessageBox(NULL, TEXT("ERROR IN FILE"), TEXT("ERROR"), 0);
			fscanf(pmf, "%f", &temp);
			if(temp == FLAGNEXT) break;
		}
		(*LoadModel)[mid].numPolygons = num;
		float32 sumr = 0, tempr = 0;
		(*LoadModel)[mid].maxRadius = 0;
		for(int i = 0; i<(*LoadModel)[mid].numVertices; i++)
		{
			tempr = sqrt(pow((*LoadModel)[mid].vertexList_Local[i].x, 2) +
						 pow((*LoadModel)[mid].vertexList_Local[i].y, 2) +
						 pow((*LoadModel)[mid].vertexList_Local[i].z, 2));
			if(tempr>(*LoadModel)[mid].maxRadius)
				(*LoadModel)[mid].maxRadius = tempr;
			sumr += tempr;
		}
		(*LoadModel)[mid].avgRadius = sumr / (*LoadModel)[mid].numVertices;
		mid++;
	}
	fclose(pmf);

	//为场景中的物体进行状态初始化
	for(int32 numObj = 0; numObj<mid; numObj++)
	{
		(*LoadModel)[numObj].state = T3OBJECT_STATE_ACTIVE | T3OBJECT_STATE_VISIBLE;
		for(int i = 0; i<(*LoadModel)[numObj].numPolygons; i++)
		{
			(*LoadModel)[numObj].polyList[i].state = T3POLY_STATE_ACTIVE;

			//默认材质为白色
			Color color(200.0, 200.0, 200.0);
			(*LoadModel)[numObj].polyList[i].color = color;
		}
	}

	return 0;
}



int T3Load(T3Object & LoadModel, char * cmf)
{
	//地址、文件名、文件指针、参数序号
	FILE * pmf;
	char cdir[20] = "./model/";
	strcat(cdir, cmf);
	int errnum = 0;
	if(errnum = (fopen_s(&pmf, cdir, "r")))
	{
		T3ErrorLog(errnum, cdir);
		MessageBox(NULL, TEXT("ERROR IN OPEN FILE"), TEXT("ERROR"), 0);
		exit(0);
	}
	//读取模型参数
	int num = 0; float64 temp;
	while(fscanf(pmf, "%f", &temp) && temp != FLAGPOLY)
	{
		LoadModel.vertexList_Local[num].x = temp;
		fscanf(pmf, "%f %f\n",
			   &(LoadModel.vertexList_Local[num].y),
			   &(LoadModel.vertexList_Local[num].z));
		num++;
		if(num>T3OBJECT_MAX_VERTICES)
			MessageBox(NULL, TEXT("ERROR IN FILE"), TEXT("ERROR"), 0);
	}
	LoadModel.numVertices = num;
	num = 0;
	int index = 0;
	while(!feof(pmf) && temp == FLAGPOLY)
	{
		fscanf(pmf, "%d", &index);
		if(index == 3)
		{
			fscanf(pmf, "%d %d %d\n",
				   &LoadModel.polyList[num].vertex[0],
				   &LoadModel.polyList[num].vertex[1],
				   &LoadModel.polyList[num].vertex[2]);
			num++;
		}
		else//nto3
		{
			fscanf(pmf, "%d %d %d",
				   &LoadModel.polyList[num].vertex[0],
				   &LoadModel.polyList[num].vertex[1],
				   &LoadModel.polyList[num].vertex[2]);
			num++;
			for(int i = 0; i<index - 3; i++)
			{
				LoadModel.polyList[num].vertex[0] = LoadModel.polyList[num - 1].vertex[0];
				LoadModel.polyList[num].vertex[1] = LoadModel.polyList[num - 1].vertex[2];
				fscanf(pmf, "%d", &LoadModel.polyList[num].vertex[2]);
				num++;
			}
		}
		if(!feof(pmf))
			fscanf(pmf, "%lf", &temp);
		if(num>T3OBJECT_MAX_POLYS)
			MessageBox(NULL, TEXT("ERROR IN FILE"), TEXT("ERROR"), 0);
	}
	LoadModel.numPolygons = num;
	fclose(pmf);
	float64 sumr = 0, tempr = 0;
	LoadModel.maxRadius = 0;
	for(int i = 0; i<LoadModel.numVertices; i++)
	{
		tempr = sqrt(pow(LoadModel.vertexList_Local[i].x, 2) +
					 pow(LoadModel.vertexList_Local[i].y, 2) +
					 pow(LoadModel.vertexList_Local[i].z, 2));
		if(tempr>LoadModel.maxRadius) LoadModel.maxRadius = tempr;
		sumr += tempr;
	}
	LoadModel.avgRadius = sumr / LoadModel.numVertices;

	return 0;
}

int T3ErrorLog(int errnum, char * dir)
{
	FILE * pef;
	SYSTEMTIME errt;
	GetLocalTime(&errt);
	if(fopen_s(&pef, "./log/errlog.txt", "a") == 0)
	{
		fprintf(pef, "%d-%d-%d %d:%d:%d : ",
				errt.wYear, errt.wMonth, errt.wDay, errt.wHour, errt.wMinute, errt.wSecond);
		switch(errnum)
		{
		case 2:
			fprintf(pef, "找不到指定的文件 : %s。请检查文件是否存在\n", dir);
			break;
		case 3:
			fprintf(pef, "找不到指定的目录 : %s。请检查目录是否存在\n", dir);
			break;
		case 4:
			fprintf(pef, "系统无法打开文件 : %s。请检查文件是否损坏\n", dir);
			break;
		case 5:
			fprintf(pef, "拒绝访问 : %s。请检查文件权限\n", dir);
			break;
		}
	}
	fclose(pef);
	return 0;
}

//之后是废柴。。。
int T3Load0(T3PLGMODEL & LoadModel, char * cmf)
{
	//地址、文件名、文件指针、参数序号
	FILE * pmf;
	char cdir[20] = "./model/";
	strcat(cdir, cmf);
	int errnum = 0;
	if(errnum = (fopen_s(&pmf, cdir, "r")))
	{
		T3ErrorLog(errnum, cdir);
		MessageBox(NULL, TEXT("ERROR IN OPEN FILE"), TEXT("ERROR"), 0);
		exit(0);
	}
	//读取模型所有参数
	int num = 0;
	while(!feof(pmf))
		fscanf(pmf, "%lf\n", &LoadModel.c[num++]);
	LoadModel.c[num] = 11;
	fclose(pmf);
	//将顶点存入Point3
	int num1 = 0;
	double * pi = &LoadModel.Point3[0].x;
	while(LoadModel.c[num1] != FLAGPOLY)
	{
		*pi++ = LoadModel.c[num1++];
		*pi++ = LoadModel.c[num1++];
		*pi++ = LoadModel.c[num1++];
	}
	LoadModel.NumOfPoint = num1 / 3;
	//关于极坐标计算
	for(int i = 0; i<LoadModel.NumOfPoint; i++)
	{
		LoadModel.RPoint[i].r = sqrt(
			(LoadModel.Point3[i].x)*(LoadModel.Point3[i].x)
			+ (LoadModel.Point3[i].z)*(LoadModel.Point3[i].z));
		LoadModel.RPoint[i].theta = atan2(
			(LoadModel.Point3[i].z),
			(LoadModel.Point3[i].x));
		LoadModel.RPoint[i].y = LoadModel.Point3[i].y;
	}
	return 0;
}

int T3Draw(HDC hdc, HWND hwnd, T3PLGMODEL & PModel, COLORREF DColor)
{
	//点、矩形申明，用于存鼠标位置和窗口位置
	POINT ptc;
	LPRECT lpw;
	lpw = new RECT;
	//创建画笔神马的
	HPEN hPen;
	HPEN hPenOld;
	hPen = CreatePen(PS_SOLID, 3, DColor);
	hPenOld = (HPEN) SelectObject(hdc, hPen);
	//获取鼠标位置、窗口位置
	GetCursorPos(&ptc);
	GetWindowRect(hwnd, lpw);
	//相对位置计算，调教了一下
	T3O.x = ptc.x - lpw->left - 8;
	T3O.y = ptc.y - lpw->top - 30;

	//将点转化为二维
	for(int i = 0; i<PModel.NumOfPoint; i++)
	{
		PModel.Point2[i].x = T3O.x + PModel.Point3[i].x + 1 / 2 * PModel.Point3[i].z;
		PModel.Point2[i].y = T3O.y - PModel.Point3[i].y - 1 / 2 * PModel.Point3[i].z;
	}
	//激动人心的时刻
	int index = 3 * PModel.NumOfPoint;
	int index2 = 0;
	while(PModel.c[index] != 11)
	{
		if(PModel.c[index] == FLAGPOLY)
		{
			index++;
			index2 = PModel.c[index];
			index++;
			for(int i = 0; i<index2; i++)
			{
				PModel.DPoint[i] = PModel.Point2[(int) PModel.c[index + i]];
			}
			if(FlagAntiAliasing)
				CTGraphics::DrawPolyline(hdc, PModel.DPoint, index2, DColor);
			else
				Polyline(hdc, PModel.DPoint, index2);
			index += index2;
		}
	}
	//释放画笔神马的
	SelectObject(hdc, hPenOld);
	DeleteObject(hPen);

	return 0;
}

int T3Revolve(HDC hdc, HWND hwnd, T3PLGMODEL & PModel, POINT RO, COLORREF RColor)
{
	static double thetar = 0;
	POINT ptc;
	LPRECT lpw;
	lpw = new RECT;
	RECT   rect;
	//创建画笔神马的
	HPEN hPen;
	HPEN hPenOld;
	hPen = CreatePen(PS_SOLID, 2, RColor);
	hPenOld = (HPEN) SelectObject(hdc, hPen);
	//获取鼠标位置、窗口位置
	GetCursorPos(&ptc);
	GetClientRect(hwnd, &rect);
	//中心位置计算
	T3O.x = RO.x - rect.left;
	T3O.y = RO.y - rect.top;
	//计算旋转后的点的极坐标
	for(int i = 0; i<PModel.NumOfPoint; i++)
	{
		PModel.RPoint[i].x = PModel.RPoint[i].r*cos(thetar + PModel.RPoint[i].theta);
		PModel.RPoint[i].z = PModel.RPoint[i].r*sin(thetar + PModel.RPoint[i].theta);
	}
	thetar += 0.01;

	//将点转化为二维
	for(int i = 0; i<PModel.NumOfPoint; i++)
	{
		PModel.Point2[i].x = T3O.x + PModel.RPoint[i].x + 1 / 2 * PModel.RPoint[i].z;
		PModel.Point2[i].y = T3O.y - PModel.RPoint[i].y - 1 / 2 * PModel.RPoint[i].z;
	}
	//激动人心的时刻
	int index = 3 * PModel.NumOfPoint;
	int index2 = 0;
	while(PModel.c[index] != 11)
	{
		if(PModel.c[index] == FLAGPOLY)
		{
			index++;
			index2 = PModel.c[index];
			index++;
			for(int i = 0; i<index2; i++)
			{
				PModel.DPoint[i] = PModel.Point2[(int) PModel.c[index + i]];
			}
			if(FlagAntiAliasing)
				CTGraphics::DrawPolyline(hdc, PModel.DPoint, index2, RColor);
			else
				Polyline(hdc, PModel.DPoint, index2);
			index += index2;
		}
	}
	//释放画笔神马的
	SelectObject(hdc, hPenOld);
	DeleteObject(hPen);
	return 0;
}

int T3Refresh(HDC hdc, HWND hwnd)
{
	//创建画笔神马的
	HPEN hPen;
	HPEN hPenOld;
	hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	hPenOld = (HPEN) SelectObject(hdc, hPen);
	HBRUSH hbrush;
	HBRUSH hbrushold;
	hbrush = CreateSolidBrush(RGB(0, 0, 0));
	hbrushold = (HBRUSH) SelectObject(hdc, hbrush);
	//画个圈圈
	Ellipse(hdc, 100, 100, 700, 500);
	//	CTGraphics::DrawEllipse(hdc, 100, 100, 700, 500, RGB(0, 0, 0));
	//释放画笔神马的
	SelectObject(hdc, hPenOld);
	DeleteObject(hPen);
	SelectObject(hdc, hbrush);
	DeleteObject(hbrush);
	return 0;
}
