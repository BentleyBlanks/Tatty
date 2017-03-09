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

	//D3D�Ƿ��ʼ�������ɹ�
	bool init();

	//��ʼ����
	void begin();

	//��������
	void end();

    //���Ƶ�һ����
    void drawPixel(point2f &point, Color &col);

	//����һ��2Dֱ��
	void draw2DLine(point2f &a, point2f &b, Color &col);

	//����һ��3D/4Dֱ��
	void draw3DLine(point4f &a, point4f &b, Color &col);

	//����һ������
	void draw2DRectangle(point2f &top, point2f &down, Color &col);

	//�ر�DirectX
	void shutDown();

private:
	//D3D��Եĸ��»��ƴ���
	HWND hwnd;
	HRESULT hr;

    // ���ڼ�¼������Ҫ���Ƶĵ�Ķ˵�
    vertex3 points[MAX_POINTS];
    //��ǰ���ظ���
    uint32 numPoints;

	//���ڼ�¼������Ҫ���Ƶ��ߵĶ˵�
	vertex3 vertex[MAX_VERTEX];
	//��ǰ�Ķ�����
    int32 numVertex;

	LPDIRECT3D9 g_D3D;
	LPDIRECT3DDEVICE9 g_D3DDevice;
	LPDIRECT3DVERTEXBUFFER9 g_VertexBuffer;

	IDirect3DTexture9*        g_pRenderTexture;
	IDirect3DSurface9*        g_pRenderSurface;

};

#endif