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
	//�򿪶������ļ�
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
//	Billing billing;//�����������������Ϣ
//	char starttime[TIMELENGTH] = { 0 };
//	char endtime[TIMELENGTH] = { 0 };
//	sscanf(pbuf, "%s %s %s %d %f %d", billing.aCardName,starttime,endtime, &billing.nStatus ,&billing.fAmount,&billing.nDel);
//	billing.tStart = StringToTime(starttime);
//	billing.tEnd = StringToTime(endtime);
	//
	//char flag[100][200] = { 0 };	//����ָ����ַ���
	//int index = 0;				//ָʾflag�����±�
	//const char* delims = "##";  //�ַ����еķָ���
	//char* buf = NULL;
	//char* str = NULL;
	//buf = pbuf;					//��һ�ε���strock,bufΪ�����ַ���
	//while ((str = strtok(buf, delims)) != NULL)
	//{
	//	strcpy(flag[index], str);
	//	buf = NULL;				//�������strtok����ʱ����һ������ΪNULL
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
	//���ļ�
	if ((fp = fopen(pPath, "rb")) == NULL)
	{
		return FALSE;
	}
	fseek(fp, 0, 0);
	//��ȡ�ļ�
	while (!feof(fp))
	{
	
		if (fread(&pBilling[i],sizeof(Billing),1,fp)!=0)
		{
			i++;//�᲻��Խ��
		}
	}
	//�ر��ļ�
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
	//�����ļ����ҵ�������Ϣ�����и���
	while (!feof(fp) && nLine < count)
	{
		if (fread(&aBuf, sizeof(Billing),1, fp) != 0)//һ��һ�ж�ȡ
		{
			lPosition = ftell(fp);//�ļ�ָ����ʲôλ��
			nLine++;
		}
	}
	fseek(fp, lPosition, 0);//�ļ�   ��ƫ���� λ��
	//������д���ļ�
	fwrite(pBilling, sizeof(Billing), 1, fp);
	//�ر��ļ�
	fclose(fp);
	return TRUE;
}

