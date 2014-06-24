#ifndef T3IO_H
#define T3IO_H

#include<Windows.h>
#include"T3Math.h"

//鼠标左右键按下
#define LEFTBUTTON_DOWN VK_LBUTTON
#define RIGHTBUTTON_DOWN VK_RBUTTON

//w键按下
#define W_DOWN 0x57
//a键按下
#define A_DOWN 0x41
//s键按下
#define S_DOWN 0x53
//d键按下
#define D_DOWN 0x44
//ESC
#define ESC_DOWN 0x1B
//Space
#define SPACE_DOWN 0x20
//Shift
#define SHIFT_DOWN 0x10

//定义按键
#define KeyDown(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KeyUp(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

//获取鼠标当前坐标
//#define T3GetMousePosition(point) GetCursorPos(point)

//inline void T3Transform_LPPOINT_To_T3Point2D(T3Point2DPtr point2D, LPPOINT p)
//{
//	
//}

inline void getMousePosition(T3Point2DPtr point2D)
{
	//初始化指针
	POINT point;
	GetCursorPos(&point);

	point2D->x = point.x;
	point2D->y = point.y;

}

inline void fixMouse(HWND hwnd, T3Point2DPtr mouse)
{
	POINT pt;
	pt.x = mouse->x;
	pt.y = mouse->y;

	ClientToScreen(hwnd, &pt);
	SetCursorPos(pt.x, pt.y);

	ShowCursor(false);
}
#endif