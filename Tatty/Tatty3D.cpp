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
		//������Ⱦ
		//Draw_T3RenderList_GDI(&rendList, hMemDC);
		//Draw_T3Object(&obj, hMemDC);
		break;
	case DIRECTX_RENDER:

		driver->begin();
		//������Ⱦ
		Draw_T3RenderList_DirectX_Solid(&rendList, driver);

		//������
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
	////����ľֲ����굽���������ת��
	//ModelToWorld_T3Object(&obj, TRANSFORM_LOCAL_TO_TRANS);
	////�����������굽�������ı任
	//WorldToCamera_T3Object(&cam, &obj);
	////�����������굽͸������
	//CameraToPerspective_T3Object(&obj, &cam);
	////�����͸�����굽��Ļ�����
	//PersepectiveToScreen_T3Object(&obj, &cam);


	for(int32 numObj=0; numObj<numOfObj; numObj++)
	{
		//�����޳�
		if(!T3CameraCull_T3Object(&obj[numObj], &cam, CULL_T3OBJECT_XYZ))
		{
			//��ǰ�����ǿɼ���
			//�ֲ����굽��������
			ModelToWorld_T3Object(&obj[numObj], TRANSFORM_LOCAL_TO_TRANS);

			//��������뵽��Ⱦ�б���
			T3Object_InsertTo_T3RenderList(&rendList, &obj[numObj]);
		}
	}

	//ִ�й��ռ���
	Light_T3RenderList(&rendList, &cam, light, numLight);

	//��������
	RemoveBackfaces_T3RenderList(&rendList, &cam);

	//��Ⱦ�б��������굽�������ı任
	WorldToCamera_T3RenderList(&cam, &rendList);

	//����Ⱦ�б�����������
	SortRenderList(&rendList);

	//��Ⱦ�б��������굽͸������
	CameraToPerspective_T3RenderList(&rendList, &cam);

	//��Ⱦ�б��͸�����굽��Ļ�����
	PersepectiveToScreen_T3RenderList(&rendList, &cam);																				    
}



void Tatty3D::clear()
{
	////��ȡ�豸DC
	//hDC = GetDC(hWnd);
	////�������ݵ���ʱDC �������
	//hMemDC = CreateCompatibleDC(hDC);
	//RECT rect;
	//rect.left = 0;
	//rect.top = 0; 
	//rect.right = 1440;
	//rect.bottom = 960;
	////�ڱ����ϴ���һ�ż��ݻ��� 
	////�˺�Ļ�ͼ���ڻ����Ͻ���
	//HBITMAP hbm = CreateCompatibleBitmap(hDC, 1440, 960);
	//SelectObject(hMemDC, hbm);
	//DeleteObject(hbm);
	////��䱳��Ϊ��ɫ
	//FillRect(hMemDC, &rect, WHITE_BRUSH);

	//���õ�ͼ������״̬
	T3ObjectReset(obj, numOfObj);

	//������Ⱦ�б�
	T3RenderListReset(&rendList);
}



void Tatty3D::update()
{
    //�˶���֡
    static float64 deltaTime;
    //�ۼƼ��ʱ��
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
	
    //��������˶�����
    if(deltaTime < time->getUpdateInterval())
        return;

    //���ʱ�����ۼ�
    deltaTime = 0.0;

	static int32 anglePointLightMotion;
	static int32 anglePointLightMotion2;
	const int32 scala = 10;
	//���ԴΧ��y����ת ������������������仯��ΧΪ1
	//��λԲ
	//--x = sinTheta
	//--z = cosTheta
	//һ�ŵ��Դ
	light[1].position.x += scala * FastSin(anglePointLightMotion);
	light[1].position.z += scala * FastCos(anglePointLightMotion);
	//Χ��y��
	light[1].position.y += scala * FastCos(anglePointLightMotion2);

	//���ŵ��Դ
	light[3].position.x -= scala * FastSin(anglePointLightMotion2);
	light[3].position.z -= scala * FastCos(anglePointLightMotion2);
	//Χ��y��
	light[3].position.y -= scala * FastCos(anglePointLightMotion2);

	//���ŵ��Դ
	light[4].position.x -= scala * FastSin(anglePointLightMotion);
	light[4].position.z -= scala * FastCos(anglePointLightMotion2);
	//Χ��y��
	light[4].position.y -= scala * FastCos(anglePointLightMotion2);



	//ѡ��һ������������һ�µ���Դ����
	obj[0].worldPosition.x += scala * FastSin(anglePointLightMotion);
	obj[0].worldPosition.z += scala * FastCos(anglePointLightMotion);
	obj[0].worldPosition.y += scala * FastCos(anglePointLightMotion);

	//�ڶ���������һ�µ���Դ����
	obj[1].worldPosition.x -= scala * FastSin(anglePointLightMotion2);
	obj[1].worldPosition.z -= scala * FastCos(anglePointLightMotion2);
	obj[1].worldPosition.y -= scala * FastCos(anglePointLightMotion2);

	//������������һ�µ���Դ����
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
	//�汾����
	T3UpdateLog();

	renderMode = DIRECTX_RENDER;

	obj = T3LoadMap("amap1.txt", numOfObj);

	//����SinCos���ұ�
	BuildSinCosTable();
	//��ת�Ƕ�
	T3Vector4D dir = {0.0f, 0.0f, 0.0f, 1.0f};
	//�����������
	T3Point4D pos = {0.0f, 200.0, 0.0, 1};

	T3Point4D target = {0.0, 0.0, 5.0, 1.0};

	T3CameraInit(&cam, &pos, &dir, 90.0, 9000.0, 200.0, screenWidth, screenHeight);


	//���̶�λ�ó�ʼ��
	fixedMousePosition.x = screenWidth / 2;
	fixedMousePosition.y = screenHeight / 2;

	//Ĭ�����û�а���
	previousFrame = 0;
	//fixMouse(hWnd, &fixedMousePosition);

	fps = (char*) malloc(25 * sizeof(char));

	wfps = (wchar_t*) malloc(25 * sizeof(wchar_t));

	frame = 0;

	//��ʼ����ʱ��
	time = new timer();
    //����˶�֡�ʲ�����ָ��֡��
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

	//ƽ�й�Դ
	light[0].init(LIGHTON, LIGHT_INFINITE,
				  black, gray, black,
				  0, 0, 0,
				  infiniteLightPos,		//����λ��
				  lightDirection);  

	//���Դ
	light[1].init(LIGHTON, LIGHT_POINT,
				  black, blue, black,
				  0, 0.0005, 0,		//���Դ����˥�����Ӳ���ȫ������Ϊ0 �������Ϊ����Ϊ0������
				  pointLightPos, lightDirection2);

	//������Դ
	light[2].init(LIGHTON, LIGHT_AMBIENT,
				  gray, black, black,
				  0, 0, 0,
				  ambientLightPos, lightDirection3);	//����λ�úͳ���

	//���Դ
	light[3].init(LIGHTON, LIGHT_POINT,
				  black, blue, black,
				  0, 0.0005, 0,
				  pointLightPos,
				  lightDirection2);

	//���Դ
	light[4].init(LIGHTON, LIGHT_POINT,
				  black, blue, black,
				  0, 0.0005, 0,
				  pointLightPos,
				  lightDirection2);

	numLight = 4;
	/*----------------Windows���----------------*/

	//����������ɫ
	//SetBkMode(hMemDC, TRANSPARENT);

	/*----------------��Ⱦ�������----------------*/
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
	//���������һ֡���²�������λ��
	if(KeyDown(RIGHTBUTTON_DOWN) && previousFrame)
	{
		T3Point2D point;
		getMousePosition(&point);

		//���λ�õ�λ��
		T3Point2D displace;

		displace = T3Vector2DSub(&point, &previousPosition);

		//����û��λ�ƾ�����
		if(point.x == 0.0f && point.y == 0.0f) return;

		cam.direction.y += 0.05f * displace.x;
		cam.direction.x += 0.05f * displace.y;

		//������һ֡��״̬
		previousPosition.x = point.x;
		previousPosition.y = point.y;
	}
	else
	{
		//������һ֡�İ���״̬
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
	//���� ����ѭ��
	clear();

	//��Ӧ�����¼�
	read();

	//��Ⱦ����
	pipeline();

	//����/��Ⱦ
	render();

	//�˶�֡���������Զ���֡��֮�� �����½�����ͼ
	update();

}

