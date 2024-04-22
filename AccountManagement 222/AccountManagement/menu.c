#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include"service.h"
#include "model.h"
#include"global.h"
#include <string.h>
#include"card_service.h"
#include "card_file.h"
#include"billing_file.h"
#include"billing_service.h"
#include"tool.h"
void showMenu()
 {
	printf("-----------�˵�----------\n");
	printf("1.��ӿ�\n");
	printf("2.��ѯ��\n");
	printf("3.�ϻ�\n");
	printf("4.�»�\n");
	printf("5.��ֵ\n");
	printf("6.�˷�\n");
	printf("7.��ѯͳ��\n");
	printf("8.ע����\n");
	printf("9.���ֶһ�\n");
	printf("0.�˳�\n");
	printf("��ѡ��˵����ţ�0~8����");
}


int getSize(const char* pInfo)
{
	int nSize = 0;
	while (*(pInfo + nSize))
	{
		nSize++;
	}
	return nSize;
}


void add()
{
	addCardInfo();
}

void query()
{
	char aName[18] = { 0 };//��������Ŀ���
	Card* pCard = NULL;//�����ѯ������Ϣ
	char aTime[20] = { 0 };
	int nIndex = 0;

	printf("---------- ��ѯ��----------\n");
	printf("�������ѯ�Ŀ���(����Ϊ1-18):");
	scanf("%s", aName);
	pCard = queryCards(aName,&nIndex);
	if (pCard == NULL || nIndex == 0)
	{
		printf("û�в�ѯ��������Ϣ��\n");
	}
	else
	{
		printf("����\t״̬\t���\tʹ�ô���\t�ϴ�ʹ��ʱ��\n");
		for (int i = 0; i < nIndex; i++)
		{
			TimeToString(pCard[i].tLastTime, aTime);//�ӡ����൱�ڽ�������
			printf("%s\t%d\t%0.1f\t      %d\t         %s\n",pCard[i].aName,pCard[i].nStatus,pCard[i].fBalance,pCard[i].nUseCount,aTime);
		}
		
	}

	
}

void copy(const char aInput[], char aOutput[], int nSize)
{
	int i = 0;
	for (i = 0; i < nSize; i++)
	{
		aOutput[i] = aInput[i];
		if (aInput[i] == 0)
		{
			break;
		}
	}
}

void exitApp()
{
	releaseList();
}


void logon()
{
	int flag = 0;
	char aName[20] = { 0 };
	char aPwd[8] = { 0 };
	Card* pCard = NULL;
	//�����û�����Ŀ��ź�����
	printf("---------- �ϻ�----------\n");
	printf("�����������˺ţ�");
	scanf("%s", aName);
	printf("������������룺");
	scanf("%s", aPwd);
	//�����ϻ�
	flag = doLogon(aName, aPwd);
	if (flag==TRUE)
	{
		printf("�ϻ��ɹ�\n");
	}
	else if(flag==DEL)
	{
		printf("�ÿ���ɾ�����ϻ�ʧ��\n");
	}
	else if (flag == Unuse)
	{
		printf("�ÿ������ϻ����ѱ�ɾ����ʧЧ\n");
	}
	else if (flag == EnoughMoney)
	{
		printf("���㣬���ֵ\n");
	}
	else if(flag==FALSE)
	{
		printf("�˺Ż��������\n");
	}
	
}

void settle()
{
	int flag = 0;
	char aName[20] = { 0 };
	char aPwd[8] = { 0 };
	Card* pCard = NULL;
	//�����û�����Ŀ��ź�����
	printf("---------- �»�----------\n");
	printf("�����������˺ţ�");
	scanf("%s", aName);
	printf("������������룺");
	scanf("%s", aPwd);
	//�����ϻ�
	flag = doSettle(aName, aPwd);
	if (flag==TRUE)
	{
		printf("�»��ɹ�����лʹ��\n");
	}
	else if(flag==FALSE)
	{
		printf("�˺Ż��������\n");
	}
	else if (flag=Unuse)
	{
		printf("���˻�δ�ϻ�\n");
	}
	//�»�
}

void recharge()
{
	float fbalance = 0;
	int flag = 0;
	char aName[20] = { 0 };
	char aPwd[8] = { 0 };
	float money = 0;
	Card* pCard = NULL;
	//�����û�����Ŀ��ź�����
	printf("---------- ��ֵ----------\n");
	printf("�����������˺ţ�");
	scanf("%s", aName);
	printf("������������룺");
	scanf("%s", aPwd);
	printf("�������ֵ�Ľ�");
	scanf("%f", &money);
	flag = doRecharge(aName, aPwd, money,&fbalance);
	if (flag==TRUE)
	{
		printf("�Ѿ�Ϊ����ֵ%.1f,�������%.1f\n",money,fbalance);
	}
	else if(flag==NameOrPwdWrong)
	{
		printf("�˺Ż��������\n");
	}
	else if (flag = -2)
	{
		printf("�ÿ���ע�������ܳ�ֵ\n");
	}
	else
	{
		printf("ϵͳ��������\n");
	}
}

int refund()
{
	float refundMoney = 0.0;
	int flag = 0;
	char aName[20] = { 0 };
	char aPwd[8] = { 0 };
	int index = 0;
	Card* pCard = NULL;
	printf("---------- �˷�----------\n");
	printf("�����������˺ţ�");
	scanf("%s", aName);
	printf("������������룺");
	scanf("%s", aPwd);
	pCard = checkCard(aName, aPwd, &index);
	if (pCard != NULL)
	{
		printf("��������%.1f\n",pCard->fBalance);
	}
	printf("������Ҫ�˷ѵĽ�");
	scanf("%f", &refundMoney);
	if (refundMoney > pCard->fBalance)
	{
		printf("�˷ѽ�����������\n");
		return FALSE;
	}
	else
	{
		if (TRUE == doRefund(aName, aPwd, refundMoney))
		{
			int index2 = 0;
			printf("�Ѿ��˷�%.1f����ǰ���%.1f\n", refundMoney, (pCard=checkCard(aName,aPwd,&index2))->fBalance);
		}
		else
		{
			return FALSE;
		}
	}
}

int logoff()
{
	char aName[20] = { 0 };
	char aPwd[8] = { 0 };
	int index1 = 0,index2=0;
	printf("---------- ע��----------\n");
	printf("�����������˺ţ�");
	scanf("%s", aName);
	printf("������������룺");
	scanf("%s", aPwd);
	Card* pCard = checkCard(aName, aPwd, &index1);
	Billing* pBilling = checkBilling(aName, &index2);
	if (pCard != NULL&&pBilling!=NULL)
	{
		//�ж��Ƿ���ע��
		if (pCard->nStatus != 0)
		{
			printf("���˻������ϻ����Ѿ�ע��������ע��\n");
			return FALSE;
		}
		pBilling->nDel = 1;
		pCard->nStatus = 2;
		pCard->nDel = 1;
		if (TRUE == updateCard(pCard, CardPath, index1)&&TRUE==updateBillingfile(pBilling,BillingPath,index2))
		{
			printf("ע���ɹ���\n");
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		printf("�˺Ż��������\n");
		return FALSE;
	}
}

int statistics()
{
	//ͳ�ư쿨��������ͳ��δע����������ͳ�����ϻ�������ͳ��Ӫ���ܶ���ѽ�
	int mTotal = 0;
	int mPerson = 0;
	int mOnlinePerson = 0;
	float TotalAmount = 0.0;
	if (TRUE == doStatistics12(&mTotal, &mPerson, &mOnlinePerson) && TRUE == doStatistics3(&TotalAmount))
	{
		printf("��ǰ�쿨��������%d\nδע������������%d\n��ǰ�ϻ�������%d\n��Ӫҵ����ѽ���%.1f\n", mTotal, mPerson, mOnlinePerson, TotalAmount);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}