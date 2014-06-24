#ifndef TIMER_H
#define TIMER_H

#include"types.h"
#include<Windows.h>


#include<float.h>

class timer
{
public:
	timer();
	//初始化
    void init(float64 motionFrameLock);
	//默认不锁帧的初始化
	void init();
	//滴答 返回两帧之间时间间隔
    float64 tick();
	//获得当前帧率
	double getFPS();
	//获得最大帧率
	double getMaxFPS();
	//获得当前平均帧率
	double getAvgFPS();
    //设置运动限帧帧率
    void setMotionFrameLock(float64 motionFrameLock);
    //获取当前运动限帧帧率下更新时间间隔
    float32 getUpdateInterval();

private:
	double FPS;
    //运动锁帧
	double motionFrameLock;
    //运动时间间隔
    double interval;
	//运行时最大帧率 平均帧率(以及用于计算平均帧率和的sumFPS) 
	double maxFPS;
	double avgFPS, sumFPS;
	//当前计时器运行的循环次数
	int count;

	//性能运算数
	LARGE_INTEGER frequency;
	//刚开始(程序一开始/某个循环中的一开始)的计数次数
	LARGE_INTEGER startCount;
	//上一帧计数次数
	LARGE_INTEGER lastCounter;

};


#endif