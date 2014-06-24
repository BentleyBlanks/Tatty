#ifndef T3DRAW_H
#define T3DRAW_H

#include"T3Pipeline.h"
#include<Windows.h>
#include"T3Graphics.h"
#include"vector2.h"
#include"DirectXDriver.h"
#include"config.h"

//��Ⱦ����
//void Draw_T3Object(T3ObjectPtr obj, HDC hdc);

//ʹ��DX����Ⱦ��Ⱦ�б�
void Draw_T3RenderList_DirectX(T3RenderListPtr rendList, DirectXDriver *driver);

void Draw_T3RenderList_DirectX_Solid(T3RenderListPtr rendList, DirectXDriver *driver);

//������պ͵���
void drawSkyBox(DirectXDriver *driver);

//���ֲ�ͬ��������ͼԪ���ƺ���
//ͨ�õĻ��������κ���
void drawTriangle(point2f p0, point2f p1, point2f p2, DirectXDriver *driver, Color color);
//ƽ��
void drawTopTriangle(point2f p0, point2f p1, point2f p2, DirectXDriver *driver, Color color);
//ƽ��
void drawBottomTriangle(point2f p0, point2f p1, point2f p2, DirectXDriver *driver, Color color);

//����ƽ��Gouraud��ɫ������
void drawGouraudTriangle(point2f p0, point2f p1, point2f p2, DirectXDriver *driver, Color color);

//ʹ��GDI����Ⱦ��Ⱦ�б�
//void Draw_T3RenderList_GDI(T3RenderListPtr rendList, HDC hdc);

#endif