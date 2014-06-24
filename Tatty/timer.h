#ifndef TIMER_H
#define TIMER_H

#include"types.h"
#include<Windows.h>


#include<float.h>

class timer
{
public:
	timer();
	//��ʼ��
    void init(float64 motionFrameLock);
	//Ĭ�ϲ���֡�ĳ�ʼ��
	void init();
	//�δ� ������֮֡��ʱ����
    float64 tick();
	//��õ�ǰ֡��
	double getFPS();
	//������֡��
	double getMaxFPS();
	//��õ�ǰƽ��֡��
	double getAvgFPS();
    //�����˶���֡֡��
    void setMotionFrameLock(float64 motionFrameLock);
    //��ȡ��ǰ�˶���֡֡���¸���ʱ����
    float32 getUpdateInterval();

private:
	double FPS;
    //�˶���֡
	double motionFrameLock;
    //�˶�ʱ����
    double interval;
	//����ʱ���֡�� ƽ��֡��(�Լ����ڼ���ƽ��֡�ʺ͵�sumFPS) 
	double maxFPS;
	double avgFPS, sumFPS;
	//��ǰ��ʱ�����е�ѭ������
	int count;

	//����������
	LARGE_INTEGER frequency;
	//�տ�ʼ(����һ��ʼ/ĳ��ѭ���е�һ��ʼ)�ļ�������
	LARGE_INTEGER startCount;
	//��һ֡��������
	LARGE_INTEGER lastCounter;

};


#endif