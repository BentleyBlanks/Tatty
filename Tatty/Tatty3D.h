//由WindowBase继承来 会使用到部分Windows风格的参数
#include<iostream>
#include"WindowsBase.h"
#include<Windows.h>
#include"T3Pipeline.h"
#include"T3FileMan.h"
#include"T3Draw.h"
#include"T3IO.h"
#include"T3UpdateLog.h"
#include"DirectXDriver.h"
#include"timer.h"

#define GDI_RENDER 0
#define DIRECTX_RENDER 1
#define OPENGL_RENDER 2

class Tatty3D : public WindowsBase
{
public:
	//默认构造函数
	//不需要是因为需要给定帧率
	//GameApplication();

	//构造函数
	Tatty3D();
	//析构函数
	~Tatty3D(void);

	//实现基类函数
	void Init();
	void ShutDown();
	void Frame();

	//响应鼠标键盘事件
	void read();
	//渲染管线
	void pipeline();
	//更新空间渲染
    void render();
	//清屏 进行循环
	void clear();
	//显示帧率锁定在自定义帧率之内 并更新界面视图
	void update();


	//设备DC
	HDC hDC;
	//临时DC
	HDC hMemDC;
private:
	char *fps;
	wchar_t *wfps;

	//光源数组
	light light[5];
	int32 numLight;

	//显卡驱动
	DirectXDriver *driver;

	//高精度计时器
	timer *time;
	int frame;

	//鼠标固定的位置
	T3Point2D fixedMousePosition;

	//渲染列表
	T3RenderList rendList;
	//读取模型信息
	T3Object *obj;
	//摄像机
	T3Camera cam;
	//上一帧鼠标的状态
	int32 previousFrame;
	//上一帧鼠标的位置
	T3Point2D previousPosition;
	//模型的数量
	int32 numOfObj;

	//引擎渲染模式
	int32 renderMode;

};




