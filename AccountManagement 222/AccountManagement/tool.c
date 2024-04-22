#define _CRT_SECURE_NO_WARNINGS
#include<time.h>
#include<stdio.h>//sscanf
#include"global.h"
void TimeToString(time_t t, char* pbuf)
{
	struct tm* pTimeInfo;
	pTimeInfo = localtime(&t);
	strftime(pbuf, TIMELENGTH, "%Y-%m-%d_%H:%M:%S",pTimeInfo);
}

time_t StringToTime(char* pTime)
{
	struct tm tm1;//创建一个时间结构体去接收
	time_t time1;
	sscanf(pTime, "%d-%d-%d_%d:%d:%d", &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday, &tm1.tm_hour, &tm1.tm_min,&tm1.tm_sec);
	tm1.tm_year = tm1.tm_year-1900;
	tm1.tm_mon -= 1;
	//tm1.tm_sec = 0;
	tm1.tm_isdst = -1;
	time1 = mktime(&tm1);
	return time1;
}