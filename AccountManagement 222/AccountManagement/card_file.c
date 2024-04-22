#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>//文件
#include<string.h>
#include "tool.h"
#include "model.h"
#include"global.h"
#include <stdlib.h>//atoi atof
#define CARDCHARNUM 128
Card praseCard(char* pBuf);
int SaveCard(const Card* pCard, const char* pPath)
{

	Card aBuf;
	/*char starttime[TIMELENGTH] = { 0 };
	char endtime[TIMELENGTH] = { 0 };
	char lasttime[TIMELENGTH] = { 0 };*/
	FILE* fp = NULL;
	//打开文件
	if ((fp = fopen(pPath, "ab+"))==NULL)
	{
		return FALSE;
	}
	//将数据写入文件
	// 将时间转换为字符串
	/*TimeToString(pCard->tStart, starttime);
	TimeToString(pCard->tEnd,endtime);
	TimeToString(pCard->tLastTime, lasttime);*/
	//格式为：卡号 密码 状态 开卡时间，截至时间 累计金额 最后使用时间 使用次数 当前余额 删除标识
	//fprintf(fp, "%s %s %d %s %s %f %s %d %f %d\n", pCard->aName, pCard->aPwd, pCard->nStatus, starttime, endtime, pCard->fTotalUse, lasttime, pCard->nUseCount, pCard->fBalance, pCard->nDel);
	//关闭文件
	fwrite(pCard, sizeof(Card), 1, fp);
	fclose(fp);
	return TRUE;
}

int ReadCard(Card* pCard, const char* pPath)//将一个个结构体给结构体数组pCard
{
	int i = 0;
	FILE* fp = NULL;
	//char aBuf[CARDCHARNUM] = { 0 };
	//打开文件
	if ((fp = fopen(pPath, "rb")) == NULL)
	{
		return FALSE;
	}
	//读取文件
	while (!feof(fp))
	{
		//memset(aBuf, 0, CARDCHARNUM);//清空数组 都初始化为0
		if (fread(&pCard[i],sizeof(Card),1,fp)!=0)
		{
				i++;
		}
	}
	//关闭文件
	fclose(fp);
	return TRUE;
}

Card praseCard()//从文件中解析卡信息成一个个结构体
{
	
	//char starttime[TIMELENGTH] = { 0 };
	//char endtime[TIMELENGTH] = { 0 };
	//char lasttime[TIMELENGTH] = { 0 };
	//Card card;//保存解析出来卡的信息
	//sscanf(pBuf, "%s %s %d %s %s %f %s %d %f %d", card.aName, card.aPwd, &card.nStatus, starttime, endtime, &card.fTotalUse, lasttime, &card.nUseCount, &card.fBalance, &card.nDel);
	////char flag[10][20]={ 0 };	//保存分割后的字符串
	//card.tStart = StringToTime(starttime);
	//card.tEnd = StringToTime(endtime);
	//card.tLastTime = StringToTime(lasttime);

	//int index = 0;				//指示flag数组下标
	//const char* delims = "##";  //字符串中的分隔符
	//char* buf = NULL;
	//char* str = NULL;
	//buf = pBuf;					//第一次调用strock,buf为解析字符串
	//while ((str = strtok(buf, delims)) != NULL)
	//{
	//	strcpy(flag[index], str);
	//	buf = NULL;				//后面调用strtok函数时，第一个参数为NULL
	//	index++;
	//}
	//strcpy(card.aName, flag[0]);
	//strcpy(card.aPwd, flag[1]);
	//card.nStatus = atoi(flag[2]);
	//card.tStart = StringToTime(flag[3]);
	//card.tEnd = StringToTime(flag[4]);
	//card.fTotalUse = (float)atof(flag[5]);
	//card.tLastTime = StringToTime(flag[6]);
	//card.nUseCount = atoi(flag[7]);
	//card.fBalance = (float)atof(flag[8]);
	//card.nDel = atoi(flag[9]);
	//return card;
}

int getCardCount(const char* pPath)//获取卡的数目
{
	int nCount = 0;
	FILE* fp = NULL;
	Card pCard;
	//打开文件
	if ((fp = fopen(pPath,"rb")) == NULL)
	{
		return FALSE;
	}
	//读取文件
	while (!feof(fp))
	{
		//memset(aBuf, 0, CARDCHARNUM);//清空数组 都初始化为0
		if (fread(&pCard,sizeof(Card),1,fp)!= 0)
		{
				nCount++;
		}
	}
	//关闭文件
	fclose(fp);
	return nCount;
}


int updateCard(const Card* pCard,const const char* pPath,int nIndex)//更新文件中卡信息
{
	Card abuf;
	/*char starttime[TIMELENGTH] = { 0 };
	char endtime[TIMELENGTH] = { 0 };
	char lasttime[TIMELENGTH] = { 0 };
	TimeToString(pCard->tStart, starttime);
	TimeToString(pCard->tEnd, endtime);
	TimeToString(pCard->tLastTime, lasttime);*/
	FILE* fp = NULL;
	int nLine = 0;
	/*char aBuf[CARDCHARNUM] = { 0 };*/
	Card aBuf;
	long lPosition = 0;
	fp = fopen(pPath, "rb+");//以只读的方式打开文件
	if (fp == NULL)
	{
		return FALSE;
	}
	//遍历文件，找到该条信息，进行更新
	while (!feof(fp) && nLine < nIndex)
	{
		if (fread(&aBuf, sizeof(Card),1, fp) != 0)//一行一行读取
		{
			lPosition = ftell(fp);//文件指针在什么位置
			nLine++;
		}
	}
	fseek(fp, lPosition, 0);//文件 设置文件指针的位置  最后是偏移量
	//将数据写入文件
	//fprintf(fp, "%s %s %d %s %s %f %s %d %f %d", pCard->aName, pCard->aPwd, pCard->nStatus, starttime, endtime, pCard->fTotalUse, lasttime, pCard->nUseCount, pCard->fBalance, pCard->nDel);
	fwrite(pCard, sizeof(Card), 1, fp);
	//关闭文件
	fclose(fp);
	return TRUE;
	
}