#ifndef T3IO_H
#define T3IO_H

#include<Windows.h>
#include"T3Math.h"

//������Ҽ�����
#define LEFTBUTTON_DOWN VK_LBUTTON
#define RIGHTBUTTON_DOWN VK_RBUTTON

//w������
#define W_DOWN 0x57
//a������
#define A_DOWN 0x41
//s������
#define S_DOWN 0x53
//d������
#define D_DOWN 0x44
//ESC
#define ESC_DOWN 0x1B
//Space
#define SPACE_DOWN 0x20
//Shift
#define SHIFT_DOWN 0x10

//���尴��
#define KeyDown(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KeyUp(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

//��ȡ��굱ǰ����
//#define T3GetMousePosition(point) GetCursorPos(point)

//inline void T3Transform_LPPOINT_To_T3Point2D(T3Point2DPtr point2D, LPPOINT p)
//{
//	
//}

inline void getMousePosition(T3Point2DPtr point2D)
{
	//��ʼ��ָ��
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