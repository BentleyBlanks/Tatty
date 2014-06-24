#ifndef T3DRAW_H
#define T3DRAW_H

#include"T3Pipeline.h"
#include<Windows.h>
#include"T3Graphics.h"
#include"vector2.h"
#include"DirectXDriver.h"
#include"config.h"

//渲染物体
//void Draw_T3Object(T3ObjectPtr obj, HDC hdc);

//使用DX来渲染渲染列表
void Draw_T3RenderList_DirectX(T3RenderListPtr rendList, DirectXDriver *driver);

void Draw_T3RenderList_DirectX_Solid(T3RenderListPtr rendList, DirectXDriver *driver);

//绘制天空和地面
void drawSkyBox(DirectXDriver *driver);

//两种不同的三角形图元绘制函数
//通用的绘制三角形函数
void drawTriangle(point2f p0, point2f p1, point2f p2, DirectXDriver *driver, Color color);
//平顶
void drawTopTriangle(point2f p0, point2f p1, point2f p2, DirectXDriver *driver, Color color);
//平底
void drawBottomTriangle(point2f p0, point2f p1, point2f p2, DirectXDriver *driver, Color color);

//绘制平滑Gouraud着色三角形
void drawGouraudTriangle(point2f p0, point2f p1, point2f p2, DirectXDriver *driver, Color color);

//使用GDI来渲染渲染列表
//void Draw_T3RenderList_GDI(T3RenderListPtr rendList, HDC hdc);

#endif