#include "windows.h"
#include "stdio.h"
#include "T3UpdateLog.h"
#define T3UPDATE /*是否更新日志*/false
#define T3VERSION /*引号内输入版本号*/"版本号：Alpha 1.2\n"
#define T3UPDATEINFORMATION /*引号内输入更新内容*/"更新内容：1.对于相机切换逻辑进行改进 更易于操控\n 2.优化对鼠标的交互 提高程序运行效率\n 3.解决程序效率极度低下的问题 运行效率提升50~100倍不等\n 4.修正了退出会崩溃的BUG\n 5.新增平均帧率来显示真实帧率\n "

int T3UpdateLog()
{
	if(T3UPDATE)
	{
		FILE * puf;
		SYSTEMTIME upt;
		GetLocalTime(&upt); 
		if(fopen_s(&puf,"./log/updatelog.txt","a")==0)
		{
			fprintf(puf,"%d-%d-%d %d:%d:%d : ",
		upt.wYear,upt.wMonth,upt.wDay,upt.wHour,upt.wMinute,upt.wSecond);
			fprintf(puf,T3VERSION);
			fprintf(puf,T3UPDATEINFORMATION);
			fclose(puf);
		}
	}
	return 0;
}