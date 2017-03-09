#ifndef DIRECTXDRIVER_H
#define DIRECTXDRIVER_H

#include"color.h"
#include"types.h"
#include"vector2.h"
#include"vector4.h"
#include"vertex3.h"
#include<iostream>
#include<Windows.h>
#include<d3d9.h>	  

#pragma comment(lib , "d3d9.lib")
//#pragma comment(lib , "d3dx9.lib")

#define MAX_POINTS 480000
#define MAX_VERTEX 123456

class DirectXDriver
{
public:
	DirectXDriver(HWND hwnd);

	//D3D是否初始化创建成功
	bool init();

	//开始绘制
	void begin();

	//结束绘制
	void end();

    //绘制单一像素
    void drawPixel(point2f &point, Color &col);

	//绘制一条2D直线
	void draw2DLine(point2f &a, point2f &b, Color &col);

	//绘制一条3D/4D直线
	void draw3DLine(point4f &a, point4f &b, Color &col);

	//绘制一个矩形
	void draw2DRectangle(point2f &top, point2f &down, Color &col);

	//关闭DirectX
	void shutDown();

private:
	//D3D针对的更新绘制窗口
	HWND hwnd;
	HRESULT hr;

    // 用于记录所有需要绘制的点的端点
    vertex3 points[MAX_POINTS];
    //当前像素个数
    uint32 numPoints;

	//用于记录所有需要绘制的线的端点
	vertex3 vertex[MAX_VERTEX];
	//当前的顶点数
    int32 numVertex;

	LPDIRECT3D9 g_D3D;
	LPDIRECT3DDEVICE9 g_D3DDevice;
	LPDIRECT3DVERTEXBUFFER9 g_VertexBuffer;

	IDirect3DTexture9*        g_pRenderTexture;
	IDirect3DSurface9*        g_pRenderSurface;

};

#endif