#include "windows.h"
#include "stdio.h"
#include "T3UpdateLog.h"
#define T3UPDATE /*�Ƿ������־*/false
#define T3VERSION /*����������汾��*/"�汾�ţ�Alpha 1.2\n"
#define T3UPDATEINFORMATION /*�����������������*/"�������ݣ�1.��������л��߼����иĽ� �����ڲٿ�\n 2.�Ż������Ľ��� ��߳�������Ч��\n 3.�������Ч�ʼ��ȵ��µ����� ����Ч������50~100������\n 4.�������˳��������BUG\n 5.����ƽ��֡������ʾ��ʵ֡��\n "

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