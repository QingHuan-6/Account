#pragma once
#include <time.h>
void TimeToString(time_t t, char* pbuf);//p buf指向容器的指针，存时间
time_t StringToTime(char* pTime);