#define _CRT_SECURE_NO_WARNINGS
#include"model.h"
#include"global.h"
#include"tool.h"
#include<stdio.h>
#include"global.h"
#include"string.h"
#include<stdlib.h>
#include<stdio.h>
#define BillingCharLen 128
int saveBilling(const Billing* pBilling, const char* pPath)
{
	/*char timeS[TIMELENGTH] = { 0 };
	char timeE[TIMELENGTH] = {0};*/
	FILE* fp = NULL;
	//打开二进制文件
	if ((fp = fopen(pPath, "ab")) == NULL)
	{
		return FALSE;
	}
	/*TimeToString(pBilling->tStart, timeS);
	TimeToString(pBilling->tEnd, timeE);
	fprintf(fp,"%s %s %s %d %f %d\n", pBilling->aCardName,timeS,timeE,pBilling->nStatus, pBilling->fAmount, pBilling->nDel);*/
	fwrite(pBilling, sizeof(Billing), 1, fp);
	fclose(fp);
	return TRUE;
}

Billing praseBilling()
{
//	Billing billing;//保存解析出来卡的信息
//	char starttime[TIMELENGTH] = { 0 };
//	char endtime[TIMELENGTH] = { 0 };
//	sscanf(pbuf, "%s %s %s %d %f %d", billing.aCardName,starttime,endtime, &billing.nStatus ,&billing.fAmount,&billing.nDel);
//	billing.tStart = StringToTime(starttime);
//	billing.tEnd = StringToTime(endtime);
	//
	//char flag[100][200] = { 0 };	//保存分割后的字符串
	//int index = 0;				//指示flag数组下标
	//const char* delims = "##";  //字符串中的分隔符
	//char* buf = NULL;
	//char* str = NULL;
	//buf = pbuf;					//第一次调用strock,buf为解析字符串
	//while ((str = strtok(buf, delims)) != NULL)
	//{
	//	strcpy(flag[index], str);
	//	buf = NULL;				//后面调用strtok函数时，第一个参数为NULL
	//	index++;
	//}
	//strcpy(billing.aCardName, flag[0]);
	//billing.tStart = StringToTime(flag[1]);
	//billing.tEnd = StringToTime(flag[2]);
	//billing.nStatus = atoi(flag[3]);
	//billing.fAmount = (float)atof(flag[4]);
	//billing.nDel = atoi(flag[5]);
	//return billing;

}

int readBilling(Billing* pBilling, const char* pPath)
{
	int i = 0;
	FILE* fp = NULL;
	//打开文件
	if ((fp = fopen(pPath, "rb")) == NULL)
	{
		return FALSE;
	}
	fseek(fp, 0, 0);
	//读取文件
	while (!feof(fp))
	{
	
		if (fread(&pBilling[i],sizeof(Billing),1,fp)!=0)
		{
			i++;//会不会越界
		}
	}
	//关闭文件
	fclose(fp);
	return TRUE;
}

int updateBillingfile(Billing* pBilling, const char* pPath, int count)
{
	Billing aBuf;
	FILE* fp = NULL;
	int nLine = 0;
	long lPosition = 0;

	fp = fopen(pPath, "rb+");
	if (fp == NULL)
	{
		return FALSE;
	}
	//遍历文件，找到该条信息，进行更新
	while (!feof(fp) && nLine < count)
	{
		if (fread(&aBuf, sizeof(Billing),1, fp) != 0)//一行一行读取
		{
			lPosition = ftell(fp);//文件指针在什么位置
			nLine++;
		}
	}
	fseek(fp, lPosition, 0);//文件   是偏移量 位置
	//将数据写入文件
	fwrite(pBilling, sizeof(Billing), 1, fp);
	//关闭文件
	fclose(fp);
	return TRUE;
}

