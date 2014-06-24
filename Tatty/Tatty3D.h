//��WindowBase�̳��� ��ʹ�õ�����Windows���Ĳ���
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
	//Ĭ�Ϲ��캯��
	//����Ҫ����Ϊ��Ҫ����֡��
	//GameApplication();

	//���캯��
	Tatty3D();
	//��������
	~Tatty3D(void);

	//ʵ�ֻ��ຯ��
	void Init();
	void ShutDown();
	void Frame();

	//��Ӧ�������¼�
	void read();
	//��Ⱦ����
	void pipeline();
	//���¿ռ���Ⱦ
    void render();
	//���� ����ѭ��
	void clear();
	//��ʾ֡���������Զ���֡��֮�� �����½�����ͼ
	void update();


	//�豸DC
	HDC hDC;
	//��ʱDC
	HDC hMemDC;
private:
	char *fps;
	wchar_t *wfps;

	//��Դ����
	light light[5];
	int32 numLight;

	//�Կ�����
	DirectXDriver *driver;

	//�߾��ȼ�ʱ��
	timer *time;
	int frame;

	//���̶���λ��
	T3Point2D fixedMousePosition;

	//��Ⱦ�б�
	T3RenderList rendList;
	//��ȡģ����Ϣ
	T3Object *obj;
	//�����
	T3Camera cam;
	//��һ֡����״̬
	int32 previousFrame;
	//��һ֡����λ��
	T3Point2D previousPosition;
	//ģ�͵�����
	int32 numOfObj;

	//������Ⱦģʽ
	int32 renderMode;

};




