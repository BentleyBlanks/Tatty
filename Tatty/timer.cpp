#include"timer.h"


timer::timer()
{
	FPS = 0.0;
	//锁帧大小为最大浮点数 即不锁帧
    motionFrameLock = FLT_MAX;

	//计算性能和起始计数器
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startCount);

}

//初始化
void timer::init(float64 motionFrameLock)
{
	count = 1;
    this->motionFrameLock = motionFrameLock;
    interval = 1.0 / motionFrameLock;
	maxFPS = 0.0;
	sumFPS = 0.0;
}

//默认不锁帧的初始化
void timer::init()
{
	count = 1;
	//运动锁帧大小为最大浮点数 即不锁帧
    motionFrameLock = FLT_MAX;
    interval = 0.0;

	maxFPS = 0.0;
	sumFPS = 0.0;
}

//滴答
float64 timer::tick()
{
	LARGE_INTEGER CurrentCounter;

	//do
	//{
		//查询当前的计数器次数
		QueryPerformanceCounter(&CurrentCounter);
		//两帧之间的时间间隔 =(当前计数器 - 上一次计数器) / 运算频率
		double DeltaTime = (double) (CurrentCounter.LowPart - lastCounter.LowPart) / frequency.LowPart;

		//计算实际显示帧率
		FPS = 1.0 / DeltaTime;

	//} while(FPS > 200.0);

	if(FPS > maxFPS)
		maxFPS = FPS;

	sumFPS += FPS;
	
	avgFPS = sumFPS / count;

	//上一帧的计数次数保存 
	lastCounter = CurrentCounter;
	count++;

    //返回两帧之间时间间隔
    return DeltaTime;
}

//设置运动限帧帧率
void timer::setMotionFrameLock(float64 motionFrameLock)
{
    this->motionFrameLock = motionFrameLock;
    interval = 1.0 / motionFrameLock;
}

//获取当前运动限帧帧率大小
float32 timer::getUpdateInterval()
{
    return interval;
}

//获得当前帧率
double timer::getFPS()
{
	return FPS;
}

//获得最大帧率
double timer::getMaxFPS()
{
	return maxFPS;
}

//获得当前平均帧率
double timer::getAvgFPS()
{
	return avgFPS;
}
