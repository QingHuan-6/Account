#define _CRT_SECURE_NO_WARNINGS
#include "model.h"
#include <stdlib.h>
#include <time.h>
#include<string.h>
#include"billing_service.h"
#include "card_service.h"	//��ҵ���߼�����
#include"model.h"
#include "global.h"		
#include "card_file.h"		//���ļ�����
#include"billing_service.h"//�Ʒ���Ϣ�߼�����
#include"billing_file.h"	//�Ʒ���Ϣ�ļ�����

void ShowInfo(Card* pCard);
double getAmount(time_t t);



int addCard(Card card)
{

	Billing* pBilling = (Billing*)malloc(sizeof(Billing));
	strcpy(pBilling->aCardName, card.aName);
	pBilling->nStatus = 0;
	pBilling->nDel = 0;
	pBilling->fAmount = 0;//���ѽ�� 
	pBilling->tStart = time(NULL);
	pBilling->tEnd = pBilling->tStart + 31536000;
	if (FALSE == saveBilling(pBilling, BillingPath))//����ͬʱ��ͨ�˻�
	{
		return FALSE;
	}
	if (FALSE == SaveCard(&card, CardPath))
	{
		return FALSE;
	}
	return TRUE;
}


void addCardInfo()
{

	Card card;
	char aName[32] = { '\0' };
	char aPwd[20] = { '\0' };
	int nNameSize = 0;
	int nPwdSize = 0;
	printf("-------��ӿ�-------\n");
	printf("�����뿨�ţ�");
	scanf("%s", aName);
	int index = 0;
	Billing* pBilling = checkBilling(aName, &index);
	if (pBilling != NULL)
	{
		printf("�������Ѿ�����,������\n");
		return;
	}
	printf("���������룺");
	scanf("%s", aPwd);
	nNameSize = getSize(aName);
	nPwdSize = getSize(aPwd);

	//�жϿ��ź������Ƿ�Ϸ�
	if (nNameSize > 18 || nPwdSize > 8)
	{
		printf("����Ŀ��Ż����볤�Ȳ��Ϸ�");
		return;
	}
	else
	{
		strcpy(card.aName, aName);
		strcpy(card.aPwd, aPwd);
	}

	printf("�����뿪����");
	scanf("%f", &card.fBalance);
	card.fTotalUse = 0;
	card.nUseCount = 0;
	card.nStatus = 0;
	card.nDel = 0;
	card.tStart = card.tEnd = card.tLastTime = time(NULL);
	addCard(card);
	printf("\n");
	printf("-------��ӿ�����Ϣ����-------\n");
	printf("����\t����\t��״̬\t���\n");
	printf("%s\t%s\t%d\t%0.1f\n", card.aName, card.aPwd, card.nStatus, card.fBalance);
}


int doLogon(const char* pName, const char* pPwd)
{
	Card* pCard = NULL;
	int index1 = 0;//����Ϣ�������е�����
	int index2 = 0;//��billing�е�����
	Billing* pBilling=NULL;// �Ʒ���Ϣ
	//���������Ƿ���ȷ
	//�ж��ܷ�����ϻ�
	pCard=checkCard(pName,pPwd,&index1);//�ҵ����˾ͷ�����
	if (pCard == NULL)
	{
		return FALSE;
	}
	//ֻ��û�б�ɾ���Ŀ������ϻ�
	if (pCard->nDel != 0)
	{
		return DEL;
	}
	//ֻ��δ��ʹ�õ�,û��ע���ģ��������ϻ�
	if (pCard->nStatus == 1||pCard->nStatus==2||pCard->nStatus==3)
	{
		return Unuse;
	}
	//ֻ��������0�����ϻ�
	if (pCard->fBalance <= 0)
	{
		return EnoughMoney;
	}
	//��������ϻ�����ô���¿���Ϣ
	pCard->nStatus = 1;
	pCard->nUseCount++;
	pCard->tStart = time(NULL);
	ShowInfo(pCard);
	if (FALSE == updateCard(pCard, CardPath, index1))
	{
		return FALSE;
	}
	//���������Ϣ
	pBilling = checkBilling(pName, &index2);
	//����billing�����е���Ϣ
	strcpy(pBilling->aCardName, pName);
	pBilling->nStatus = 0;	//����״̬ ��Ϊδ����
	//���ѽ��
	pBilling->nDel = 0;
	updateBillingfile(pBilling, BillingPath, index2);
	return TRUE;
}

int doSettle(const char*pName,const char* pPwd)
{
	Card* pCard = NULL;
	Billing* pBilling = NULL;
	int index1 = 0;//��card�����е�����
	int index2 = 0;//��billing�����е�����
	double dbAmount = 0.0;
	float fBalance = 0.0;
	//��ȡ����Ϣ��ֻ�������ϻ��Ŀ����ܽ����»�����
	pCard = checkCard(pName, pPwd, &index1);
	pBilling = checkBilling(pName, &index2);
	//��ȡ�Ʒ���Ϣ
	//�ϻ��Ѿ��������Ƿ�ɾ������ע���������ټ���
	if (pCard == NULL)
	{
		return FALSE;
	}
	if (pCard->nStatus != 1)
	{
		return Unuse;
	}
	//�������ѽ��
	dbAmount = getAmount(pCard->tStart);
	//�жϿ���������ѽ��
	//���¿����
	if (dbAmount > pCard->fBalance)
	{
		printf("���㣬���ֵ\n");
	}
	pCard->nStatus = 0;
	pCard->fBalance = pCard->fBalance - dbAmount;
	pCard->fTotalUse = pCard->fTotalUse + dbAmount;
	pCard->tLastTime = time(NULL);
	//���¿����
	pBilling->fAmount = pBilling->fAmount + dbAmount;
	pBilling->nStatus = 1;
	printf("�˴�����%.1fԪ\n", dbAmount);
	ShowInfo(pCard);
	updateCard(pCard, CardPath, index1);
	updateBillingfile(pBilling, BillingPath, index2);
	return TRUE;
}

double getAmount(time_t t)
{
	int nSec;
	int nMin;
	int nCount;
	double dbAmount;
	time_t tEnd = time(NULL);
	nSec = tEnd - t;
	/*nMin = nSec / 60;*/
	//if (nMin / unit == 0)
	//{
	//	nCount = nMin / unit;//������һ��15���ӾͲ��շ�
	//}
	//else
	//{
	//	nCount = nMin / unit + 1;//����һ��15���Ӿ�+1
	//}
	/*dbAmount = nCount * charge2;*/
	dbAmount = nSec * charge2;
	return dbAmount;
}

int doRecharge(const char* pName, const char* pPwd, float money,float *fbalance)
{
	int index1 = 0;
	Card* pCard = NULL;

	if ((pCard = checkCard(pName, pPwd, &index1)) == NULL)
	{
		return NameOrPwdWrong;
	}
	if (pCard->nDel != 0)
	{
		return -2;
	}
	pCard->fBalance = pCard->fBalance + money;
	*fbalance = pCard->fBalance;
	if (TRUE == updateCard(pCard, CardPath, index1))
	{
	
		return TRUE;
	}
	return FALSE;
}


int doRefund(const char* pName, const char* pPwd, float refundMoney)
{
	int index1 = 0;
	Card* pCard = NULL;
	pCard = checkCard(pName, pPwd, &index1);
	if (pCard != NULL)
	{
		pCard->fBalance = pCard->fBalance - refundMoney;
		updateCard(pCard, CardPath, index1);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void releaseList()
{
	releaseCardList();
	releaseBillingList();//�ͷżƷ���Ϣ�����ڴ�
}

void ShowInfo(Card*pCard)
{
	printf("��ǰ�˺�Ϊ��%s\n", pCard->aName);
	printf("��ǰ���Ϊ��%.1f\n", pCard->fBalance);
	printf("��ǰ״̬Ϊ��");
	if (pCard->nStatus == 1)
	{
		printf("�ϻ�\n");
	}
	else if (pCard->nStatus == 0)
	{
		printf("�»�\n");
	}
}

void present()
{
	char aName[32] = { '\0' };
	char aPwd[20] = { '\0' };
	printf("�����������˺ţ�");
	scanf("%s", aName);
	printf("������������룺");
	scanf("%s", aPwd);
	int index1 = 0;
	Card* pCard = checkCard(aName, aPwd, &index1);
	float score = pCard->fTotalUse * 2;
	int flag = 0;
	printf("20���ֶһ��豭һ��,40���ֶһ�����һ��,80���ֶһ�ƽ��һ��\n");
	printf("����ǰ�л���%.1f\n",score);
	if (score < 10.0)
	{
		printf("��Ŀǰ�Ļ���û�п��Զһ��Ľ�Ʒ\n");
		return;
	}
	else
	{
		printf("0.���һ� 1.�豭 2.���� 3.ƽ��\n");
		printf("����������ѡ��");
		scanf("%d", &flag);
		switch (flag)
		{
			case 0:break;
			case 1:
				if (pCard->fTotalUse < 10)
					{
					printf("���ֲ���");
					return;
					}
					pCard->fTotalUse -= 10.0;
					updateCard(pCard, CardPath, index1);
					printf("��л�һ���\n"); break;
			case 2:
				if (pCard->fTotalUse < 20)
				{
					printf("���ֲ���");
					return;
				}pCard->fTotalUse -= 20.0;
					updateCard(pCard, CardPath, index1);
					printf("��л�һ���\n"); break;
			case 3:
				if (pCard->fTotalUse < 30)
				{
					printf("���ֲ���");
					return;
				}pCard->fTotalUse -= 40.0;
					updateCard(pCard, CardPath, index1);
					printf("��л�һ���\n"); break;
			default:printf("���벻�Ϸ�\n"); break;
		}
	}
	return;
}