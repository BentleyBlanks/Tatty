#include"timer.h"


timer::timer()
{
	FPS = 0.0;
	//��֡��СΪ��󸡵��� ������֡
    motionFrameLock = FLT_MAX;

	//�������ܺ���ʼ������
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startCount);

}

//��ʼ��
void timer::init(float64 motionFrameLock)
{
	count = 1;
    this->motionFrameLock = motionFrameLock;
    interval = 1.0 / motionFrameLock;
	maxFPS = 0.0;
	sumFPS = 0.0;
}

//Ĭ�ϲ���֡�ĳ�ʼ��
void timer::init()
{
	count = 1;
	//�˶���֡��СΪ��󸡵��� ������֡
    motionFrameLock = FLT_MAX;
    interval = 0.0;

	maxFPS = 0.0;
	sumFPS = 0.0;
}

//�δ�
float64 timer::tick()
{
	LARGE_INTEGER CurrentCounter;

	//do
	//{
		//��ѯ��ǰ�ļ���������
		QueryPerformanceCounter(&CurrentCounter);
		//��֮֡���ʱ���� =(��ǰ������ - ��һ�μ�����) / ����Ƶ��
		double DeltaTime = (double) (CurrentCounter.LowPart - lastCounter.LowPart) / frequency.LowPart;

		//����ʵ����ʾ֡��
		FPS = 1.0 / DeltaTime;

	//} while(FPS > 200.0);

	if(FPS > maxFPS)
		maxFPS = FPS;

	sumFPS += FPS;
	
	avgFPS = sumFPS / count;

	//��һ֡�ļ����������� 
	lastCounter = CurrentCounter;
	count++;

    //������֮֡��ʱ����
    return DeltaTime;
}

//�����˶���֡֡��
void timer::setMotionFrameLock(float64 motionFrameLock)
{
    this->motionFrameLock = motionFrameLock;
    interval = 1.0 / motionFrameLock;
}

//��ȡ��ǰ�˶���֡֡�ʴ�С
float32 timer::getUpdateInterval()
{
    return interval;
}

//��õ�ǰ֡��
double timer::getFPS()
{
	return FPS;
}

//������֡��
double timer::getMaxFPS()
{
	return maxFPS;
}

//��õ�ǰƽ��֡��
double timer::getAvgFPS()
{
	return avgFPS;
}
