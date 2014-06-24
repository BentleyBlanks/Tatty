#ifndef _WINDOWS_BASE_H_
#define _WINDOWS_BASE_H_

#include<Windows.h>
#include"config.h"

class WindowsBase
{
protected:
	//当前实例句柄
	HINSTANCE hInstance;
	//窗口句柄
	HWND hWnd;
	//窗口类名称
	TCHAR szWindowClassName[20];
	//窗口标题名称
	TCHAR szCaptionName[20];
	//窗口类
	WNDCLASSEX wndclass;
	//消息
	MSG msg;

public:
	//构造函数初始化变量
	WindowsBase();

	//供后期使用
	HWND GethWnd();
	HINSTANCE GethInstance();

	//游戏程序框架
	BOOL Run();
	//改变窗口大小
	BOOL Resize(long width, long height);

	//不同的GameApplication实现这三个函数
	//此处为纯虚函数 只是声明一个函数名
	virtual void Init(){};
	virtual void ShutDown(){};
	virtual void Frame(){};
};

static long _stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif


