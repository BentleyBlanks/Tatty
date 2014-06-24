#define _CRT_SECURE_NO_WARNINGS
#include "Tatty3D.h"


Tatty3D::Tatty3D()
{

}


Tatty3D::~Tatty3D(void) 
{

}


void Tatty3D::render()
{

	switch(renderMode)
	{
	case GDI_RENDER:
		//最终渲染
		//Draw_T3RenderList_GDI(&rendList, hMemDC);
		//Draw_T3Object(&obj, hMemDC);
		break;
	case DIRECTX_RENDER:

		driver->begin();
		//最终渲染
		Draw_T3RenderList_DirectX_Solid(&rendList, driver);

		//中央线
		driver->draw2DLine(point2f(screenWidth / 2, 0.0), point2f(screenWidth / 2, screenHeight), Color(0, 0, 0));
		driver->draw2DLine(point2f(0.0, screenHeight / 2), point2f(screenWidth, screenHeight / 2) , Color(0, 0, 0));

		driver->end();

		break;
	case OPENGL_RENDER:
		break;
	}
}


void Tatty3D::pipeline()
{
	////物体的局部坐标到世界坐标的转换
	//ModelToWorld_T3Object(&obj, TRANSFORM_LOCAL_TO_TRANS);
	////物体世界坐标到相机坐标的变换
	//WorldToCamera_T3Object(&cam, &obj);
	////物体的相机坐标到透视坐标
	//CameraToPerspective_T3Object(&obj, &cam);
	////物体的透视坐标到屏幕坐标的
	//PersepectiveToScreen_T3Object(&obj, &cam);


	for(int32 numObj=0; numObj<numOfObj; numObj++)
	{
		//物体剔除
		if(!T3CameraCull_T3Object(&obj[numObj], &cam, CULL_T3OBJECT_XYZ))
		{
			//当前物体是可见的
			//局部坐标到世界坐标
			ModelToWorld_T3Object(&obj[numObj], TRANSFORM_LOCAL_TO_TRANS);

			//将物体插入到渲染列表中
			T3Object_InsertTo_T3RenderList(&rendList, &obj[numObj]);
		}
	}

	//执行光照计算
	Light_T3RenderList(&rendList, &cam, light, numLight);

	//背面消除
	RemoveBackfaces_T3RenderList(&rendList, &cam);

	//渲染列表世界坐标到相机坐标的变换
	WorldToCamera_T3RenderList(&cam, &rendList);

	//对渲染列表进行深度排序
	SortRenderList(&rendList);

	//渲染列表的相机坐标到透视坐标
	CameraToPerspective_T3RenderList(&rendList, &cam);

	//渲染列表的透视坐标到屏幕坐标的
	PersepectiveToScreen_T3RenderList(&rendList, &cam);																				    
}



void Tatty3D::clear()
{
	////获取设备DC
	//hDC = GetDC(hWnd);
	////创建兼容的临时DC 解决屏闪
	//hMemDC = CreateCompatibleDC(hDC);
	//RECT rect;
	//rect.left = 0;
	//rect.top = 0; 
	//rect.right = 1440;
	//rect.bottom = 960;
	////在背景上创建一张兼容画布 
	////此后的绘图都在画布上进行
	//HBITMAP hbm = CreateCompatibleBitmap(hDC, 1440, 960);
	//SelectObject(hMemDC, hbm);
	//DeleteObject(hbm);
	////填充背景为白色
	//FillRect(hMemDC, &rect, WHITE_BRUSH);

	//重置地图中物体状态
	T3ObjectReset(obj, numOfObj);

	//重置渲染列表
	T3RenderListReset(&rendList);
}



void Tatty3D::update()
{
    //运动限帧
    static float64 deltaTime;
    //累计间隔时间
    deltaTime += time->tick();

	double f = time->getFPS();
	double avgFPS = time->getAvgFPS();

	if(++frame == 100.0)
	{
		sprintf(fps, "FPS:%f x:%f y:%f z:%f", f, cam.n.x, cam.n.y, cam.n.z);

		size_t len = strlen(fps) + 1;
		size_t converted = 0;

		mbstowcs_s(&converted, wfps, len, fps, _TRUNCATE);

		DWORD_PTR dwResult;
		SendMessageTimeoutW(hWnd, WM_SETTEXT, 0,
							reinterpret_cast<LPARAM>(wfps),
							SMTO_ABORTIFHUNG, 2000, &dwResult);
		/*SetWindowText(hWnd, wfps);*/
		frame = 0;
	}
	
    //无需更新运动画面
    if(deltaTime < time->getUpdateInterval())
        return;

    //清空时间间隔累计
    deltaTime = 0.0;

	static int32 anglePointLightMotion;
	static int32 anglePointLightMotion2;
	const int32 scala = 10;
	//点光源围绕y轴旋转 若不乘以缩放因子则变化范围为1
	//单位圆
	//--x = sinTheta
	//--z = cosTheta
	//一号点光源
	light[1].position.x += scala * FastSin(anglePointLightMotion);
	light[1].position.z += scala * FastCos(anglePointLightMotion);
	//围绕y轴
	light[1].position.y += scala * FastCos(anglePointLightMotion2);

	//二号点光源
	light[3].position.x -= scala * FastSin(anglePointLightMotion2);
	light[3].position.z -= scala * FastCos(anglePointLightMotion2);
	//围绕y轴
	light[3].position.y -= scala * FastCos(anglePointLightMotion2);

	//三号点光源
	light[4].position.x -= scala * FastSin(anglePointLightMotion);
	light[4].position.z -= scala * FastCos(anglePointLightMotion2);
	//围绕y轴
	light[4].position.y -= scala * FastCos(anglePointLightMotion2);



	//选择一定数量物体充次一下当光源……
	obj[0].worldPosition.x += scala * FastSin(anglePointLightMotion);
	obj[0].worldPosition.z += scala * FastCos(anglePointLightMotion);
	obj[0].worldPosition.y += scala * FastCos(anglePointLightMotion);

	//第二个物体充次一下当光源……
	obj[1].worldPosition.x -= scala * FastSin(anglePointLightMotion2);
	obj[1].worldPosition.z -= scala * FastCos(anglePointLightMotion2);
	obj[1].worldPosition.y -= scala * FastCos(anglePointLightMotion2);

	//第三个物体充次一下当光源……
	obj[2].worldPosition.x -= scala * FastSin(anglePointLightMotion);
	obj[2].worldPosition.z += scala * FastCos(anglePointLightMotion2);
	obj[2].worldPosition.y += scala * FastCos(anglePointLightMotion2);


	anglePointLightMotion += 2;
	if(anglePointLightMotion > 360)
		anglePointLightMotion = 0;

	if(anglePointLightMotion2++ > 360)
		anglePointLightMotion2 = 0;
}


void Tatty3D::Init()
{
	//版本更新
	T3UpdateLog();

	renderMode = DIRECTX_RENDER;

	obj = T3LoadMap("amap1.txt", numOfObj);

	//建立SinCos查找表
	BuildSinCosTable();
	//旋转角度
	T3Vector4D dir = {0.0f, 0.0f, 0.0f, 1.0f};
	//相机世界坐标
	T3Point4D pos = {0.0f, 200.0, 0.0, 1};

	T3Point4D target = {0.0, 0.0, 5.0, 1.0};

	T3CameraInit(&cam, &pos, &dir, 90.0, 9000.0, 200.0, screenWidth, screenHeight);


	//鼠标固定位置初始化
	fixedMousePosition.x = screenWidth / 2;
	fixedMousePosition.y = screenHeight / 2;

	//默认鼠标没有按下
	previousFrame = 0;
	//fixMouse(hWnd, &fixedMousePosition);

	fps = (char*) malloc(25 * sizeof(char));

	wfps = (wchar_t*) malloc(25 * sizeof(wchar_t));

	frame = 0;

	//初始化计时器
	time = new timer();
    //最大运动帧率不超过指定帧率
	time->init(100.0);

	T3Vector4D lightDirection = {-3.0, 0.0, -3.0, 1};
	T3Vector4D lightDirection2 = {0.0, 100.0, 0.0, 1};
	T3Vector4D lightDirection3 = {0, 0, 0, 1};
	T3Vector4D lightDirection4 = {0.0, -1.0, 0.0, 1.0};
	

	Color red(255, 252, 153), blue(30, 41, 61), green(85, 72, 64), 
		black(0.0, 0.0, 0.0), gray(110.0, 110.0, 110.0), white(240.0, 240.0, 240.0);

	T3Point4D infiniteLightPos = {0.0, 0.0, 0.0, 1.0};
	T3Point4D ambientLightPos = {0.0, 0.0, 0.0, 1.0};
	T3Point4D pointLightPos = {100.0, 200.0, 100.0, 1.0};

	//平行光源
	light[0].init(LIGHTON, LIGHT_INFINITE,
				  black, gray, black,
				  0, 0, 0,
				  infiniteLightPos,		//无需位置
				  lightDirection);  

	//点光源
	light[1].init(LIGHTON, LIGHT_POINT,
				  black, blue, black,
				  0, 0.0005, 0,		//点光源三个衰减因子不能全都设置为0 否则会因为除数为0而崩溃
				  pointLightPos, lightDirection2);

	//环境光源
	light[2].init(LIGHTON, LIGHT_AMBIENT,
				  gray, black, black,
				  0, 0, 0,
				  ambientLightPos, lightDirection3);	//无需位置和朝向

	//点光源
	light[3].init(LIGHTON, LIGHT_POINT,
				  black, blue, black,
				  0, 0.0005, 0,
				  pointLightPos,
				  lightDirection2);

	//点光源
	light[4].init(LIGHTON, LIGHT_POINT,
				  black, blue, black,
				  0, 0.0005, 0,
				  pointLightPos,
				  lightDirection2);

	numLight = 4;
	/*----------------Windows相关----------------*/

	//设置字体颜色
	//SetBkMode(hMemDC, TRANSPARENT);

	/*----------------渲染引擎相关----------------*/
	switch (renderMode)
	{
	case GDI_RENDER:
		break;
	case DIRECTX_RENDER:
		driver = new DirectXDriver(hWnd);
		driver->init();
		break;
	case OPENGL_RENDER:
		break;
	default:
		break;
	}
	
}

void Tatty3D::read()
{

	if(KeyDown(W_DOWN))
	{
		//updateCameraPosition(&cam, DIRECTION_FRONT);
		//cam.position.z += FastCos(cam.direction.x);
		//cam.position.y += -FastSin(cam.direction.x);

		cam.position.x += FastSin(cam.direction.y);
		cam.position.z += FastCos(cam.direction.y);
		//changeCamWatch = 1;
	}
	if(KeyDown(S_DOWN))
	{
		//updateCameraPosition(&cam, DIRECTION_BACK);
		//cam.position.z -= FastCos(cam.direction.x);
		//cam.position.y -= -FastSin(cam.direction.x);

		cam.position.x -= FastSin(cam.direction.y);
		cam.position.z -= FastCos(cam.direction.y);

	}
	if(KeyDown(A_DOWN))
	{
		cam.direction.y-=0.01;
        updateCameraAxis(&cam);
	}
	if(KeyDown(D_DOWN))
	{
		cam.direction.y+=0.01;
        updateCameraAxis(&cam);
		//cam.position.x++;
	}
	if(KeyDown(SPACE_DOWN))
	{
		cam.position.y++;
		//cam.direction.x--;
	}
	if(KeyDown(SHIFT_DOWN))
	{
		cam.position.y--;
		//cam.direction.x++;
	}
	if(KeyDown(ESC_DOWN))
	{
		PostQuitMessage(0);
	}
	//若是左键上一帧按下并产生了位移
	if(KeyDown(RIGHTBUTTON_DOWN) && previousFrame)
	{
		T3Point2D point;
		getMousePosition(&point);

		//鼠标位置的位移
		T3Point2D displace;

		displace = T3Vector2DSub(&point, &previousPosition);

		//假若没有位移就跳过
		if(point.x == 0.0f && point.y == 0.0f) return;

		cam.direction.y += 0.05f * displace.x;
		cam.direction.x += 0.05f * displace.y;

		//更新上一帧点状态
		previousPosition.x = point.x;
		previousPosition.y = point.y;
	}
	else
	{
		//更新上一帧的按键状态
		previousFrame = 1;
		getMousePosition(&previousPosition);
	}
}


void Tatty3D::ShutDown()
{	
	switch (renderMode)
	{
	case GDI_RENDER:
		break;
	case DIRECTX_RENDER:
		driver->shutDown();
		break;
	case OPENGL_RENDER:
		break;
	}
}


void Tatty3D::Frame()
{
	//清屏 进行循环
	clear();

	//响应输入事件
	read();

	//渲染管线
	pipeline();

	//绘制/渲染
	render();

	//运动帧率锁定在自定义帧率之内 并更新界面视图
	update();

}

