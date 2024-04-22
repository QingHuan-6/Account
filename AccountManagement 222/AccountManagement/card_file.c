#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>//�ļ�
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
	//���ļ�
	if ((fp = fopen(pPath, "ab+"))==NULL)
	{
		return FALSE;
	}
	//������д���ļ�
	// ��ʱ��ת��Ϊ�ַ���
	/*TimeToString(pCard->tStart, starttime);
	TimeToString(pCard->tEnd,endtime);
	TimeToString(pCard->tLastTime, lasttime);*/
	//��ʽΪ������ ���� ״̬ ����ʱ�䣬����ʱ�� �ۼƽ�� ���ʹ��ʱ�� ʹ�ô��� ��ǰ��� ɾ����ʶ
	//fprintf(fp, "%s %s %d %s %s %f %s %d %f %d\n", pCard->aName, pCard->aPwd, pCard->nStatus, starttime, endtime, pCard->fTotalUse, lasttime, pCard->nUseCount, pCard->fBalance, pCard->nDel);
	//�ر��ļ�
	fwrite(pCard, sizeof(Card), 1, fp);
	fclose(fp);
	return TRUE;
}

int ReadCard(Card* pCard, const char* pPath)//��һ�����ṹ����ṹ������pCard
{
	int i = 0;
	FILE* fp = NULL;
	//char aBuf[CARDCHARNUM] = { 0 };
	//���ļ�
	if ((fp = fopen(pPath, "rb")) == NULL)
	{
		return FALSE;
	}
	//��ȡ�ļ�
	while (!feof(fp))
	{
		//memset(aBuf, 0, CARDCHARNUM);//������� ����ʼ��Ϊ0
		if (fread(&pCard[i],sizeof(Card),1,fp)!=0)
		{
				i++;
		}
	}
	//�ر��ļ�
	fclose(fp);
	return TRUE;
}

Card praseCard()//���ļ��н�������Ϣ��һ�����ṹ��
{
	
	//char starttime[TIMELENGTH] = { 0 };
	//char endtime[TIMELENGTH] = { 0 };
	//char lasttime[TIMELENGTH] = { 0 };
	//Card card;//�����������������Ϣ
	//sscanf(pBuf, "%s %s %d %s %s %f %s %d %f %d", card.aName, card.aPwd, &card.nStatus, starttime, endtime, &card.fTotalUse, lasttime, &card.nUseCount, &card.fBalance, &card.nDel);
	////char flag[10][20]={ 0 };	//����ָ����ַ���
	//card.tStart = StringToTime(starttime);
	//card.tEnd = StringToTime(endtime);
	//card.tLastTime = StringToTime(lasttime);

	//int index = 0;				//ָʾflag�����±�
	//const char* delims = "##";  //�ַ����еķָ���
	//char* buf = NULL;
	//char* str = NULL;
	//buf = pBuf;					//��һ�ε���strock,bufΪ�����ַ���
	//while ((str = strtok(buf, delims)) != NULL)
	//{
	//	strcpy(flag[index], str);
	//	buf = NULL;				//�������strtok����ʱ����һ������ΪNULL
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

int getCardCount(const char* pPath)//��ȡ������Ŀ
{
	int nCount = 0;
	FILE* fp = NULL;
	Card pCard;
	//���ļ�
	if ((fp = fopen(pPath,"rb")) == NULL)
	{
		return FALSE;
	}
	//��ȡ�ļ�
	while (!feof(fp))
	{
		//memset(aBuf, 0, CARDCHARNUM);//������� ����ʼ��Ϊ0
		if (fread(&pCard,sizeof(Card),1,fp)!= 0)
		{
				nCount++;
		}
	}
	//�ر��ļ�
	fclose(fp);
	return nCount;
}


int updateCard(const Card* pCard,const const char* pPath,int nIndex)//�����ļ��п���Ϣ
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
	fp = fopen(pPath, "rb+");//��ֻ���ķ�ʽ���ļ�
	if (fp == NULL)
	{
		return FALSE;
	}
	//�����ļ����ҵ�������Ϣ�����и���
	while (!feof(fp) && nLine < nIndex)
	{
		if (fread(&aBuf, sizeof(Card),1, fp) != 0)//һ��һ�ж�ȡ
		{
			lPosition = ftell(fp);//�ļ�ָ����ʲôλ��
			nLine++;
		}
	}
	fseek(fp, lPosition, 0);//�ļ� �����ļ�ָ���λ��  �����ƫ����
	//������д���ļ�
	//fprintf(fp, "%s %s %d %s %s %f %s %d %f %d", pCard->aName, pCard->aPwd, pCard->nStatus, starttime, endtime, pCard->fTotalUse, lasttime, pCard->nUseCount, pCard->fBalance, pCard->nDel);
	fwrite(pCard, sizeof(Card), 1, fp);
	//�ر��ļ�
	fclose(fp);
	return TRUE;
	
}